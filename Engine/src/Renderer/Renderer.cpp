#include "Wrangler/Renderer/Renderer.hpp"

// fs
#include "Wrangler/Filesystem/Payload.hpp"

// tul
#include <tul/ErrorOps.hpp>

constexpr bgfx::ViewId VIEW_ID = 0;

void Wrangler::Renderer::loadShaderFromPath(const std::string &path, Shader &target)
{
    Payload payload;
    payload.read(fs.readFile(path).asString());

    std::string vertexPath = payload.content["vertex"].get<std::string>();
    std::string fragmentPath = payload.content["fragment"].get<std::string>();

    std::vector<byte> vertexData = fs.readFile(vertexPath).asBinary();
    std::vector<byte> fragmentData = fs.readFile(fragmentPath).asBinary();

    target.init(vertexData, fragmentData, payload);

    if(!bgfx::isValid(target.program)) tul::FatalError({"Failed to create shader program: ", path});
}

Wrangler::Renderer::Renderer(const RendererParameters &params) : assets(params.assets),
                                                                 fs(params.fs),
                                                                 width(params.width),
                                                                 height(params.height)
{

    bgfx::setViewClear(
        VIEW_ID,
        BGFX_CLEAR_COLOR |
        BGFX_CLEAR_DEPTH,
        0x303030ff,
        1.0f,
        0
    );

    bgfx::setViewRect(
        VIEW_ID,
        0,
        0,
        width,
        height
    );

    tul::Print({"[Renderer] Renderer intialized\n"});
    

    // load pbr shader
    if (!params.shadowShaderPath.empty())
    {
        mainPipeline.shadowShader =
            std::make_unique<Shader>();

        loadShaderFromPath(
            params.shadowShaderPath,
            *mainPipeline.shadowShader
        );
    }

    if (!params.sceneShaderPath.empty())
    {
        mainPipeline.sceneShader =
            std::make_unique<Shader>();

        loadShaderFromPath(
            params.sceneShaderPath,
            *mainPipeline.sceneShader
        );
    }

    if (!params.postProcessShaderPath.empty())
    {
        mainPipeline.postProcessShader =
            std::make_unique<Shader>();

        loadShaderFromPath(
            params.postProcessShaderPath,
            *mainPipeline.postProcessShader
        );
    }
    // set pbr shader uniforms
    mainPipeline.sceneShader->createUniform("u_baseColor", bgfx::UniformType::Vec4, 1);

}

/// @brief Prepares a frame
void Wrangler::Renderer::begin(const Camera &camera)
{
    this->camera = &camera;
    bgfx::setViewRect(0, 0, 0, uint16_t(width), uint16_t(height));
    bgfx::touch(0);

    bgfx::setViewTransform(
        VIEW_ID,
        camera.view,
        camera.projection
    );
}

/// @brief Renders an entity to the screen
/// @param entity Entity data
/// @param params Shader scene params (camera position is overwriten)
void Wrangler::Renderer::renderEntity(const RenderableEntity& entity, const ShaderSceneParameters& params)
{
    // data checking
    if(!entity.model || !entity.material)  {
        tul::Alert({"Attempted to render model with missing data"});
        return;
    }

    ShaderSceneParameters newParams = params;
    newParams.cameraPosition = camera->position;

    const auto model =
        assets.getModel(entity.model);

    // set uniforms
    const auto mat = 
        assets.getMaterial(entity.material);

    // =========================================
    // Transform
    // =========================================

    float transform[16];

    bx::mtxSRT(
        transform,

        entity.scale.x,
        entity.scale.y,
        entity.scale.z,

        entity.rotation.x,
        entity.rotation.y,
        entity.rotation.z,

        entity.position.x,
        entity.position.y,
        entity.position.z
    );

    bgfx::setTransform(transform);

    // =========================================
    // Meshes
    // =========================================

    for (const GPUMesh& mesh : model->meshes)
    {
        // SCENE SHADER
        mainPipeline.sceneShader->setGlobalUniforms(newParams);
        uint8_t textureStage = 0;
        for (const auto& uniform : mat->uniformData)
        {
            switch (uniform.type)
            {
                case bgfx::UniformType::Vec4:
                    mainPipeline.sceneShader->setUniform(uniform.name, uniform.vector4);
                    break;

                case bgfx::UniformType::Mat3:
                    mainPipeline.sceneShader->setUniform(uniform.name, uniform.mat3);
                    break;

                case bgfx::UniformType::Mat4:
                    mainPipeline.sceneShader->setUniform(uniform.name, uniform.mat4);
                    break;

                case bgfx::UniformType::Sampler:
                {
                    const auto texture = assets.getTexture(uniform.sampler);

                    mainPipeline.sceneShader->setTexture(
                        uniform.name,
                        textureStage,
                        texture->handle()
                    );

                    ++textureStage;
                    break;
                }

                default:
                    break;
            }
        }

        bgfx::setVertexBuffer(
            0,
            mesh.vertexBuffer
        );

        bgfx::setIndexBuffer(
            mesh.indexBuffer
        );


        bgfx::setTransform(transform);

        // bind material here

        bgfx::setState(
            BGFX_STATE_WRITE_RGB |
            BGFX_STATE_WRITE_A |
            BGFX_STATE_WRITE_Z |
            BGFX_STATE_DEPTH_TEST_LESS |
            BGFX_STATE_CULL_CCW
        );

        bgfx::submit(
            0,
            mainPipeline.sceneShader->handle()
        );
    }
}

/// @brief Ends the frame
void Wrangler::Renderer::end()
{
    camera = nullptr;
}

Wrangler::Renderer::~Renderer() {}
