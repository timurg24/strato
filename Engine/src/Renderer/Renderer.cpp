#include "Wrangler/Renderer/Renderer.hpp"

// fs
#include "Wrangler/Filesystem/Payload.hpp"

// tul
#include <tul/ErrorOps.hpp>

constexpr bgfx::ViewId VIEW_ID = 0;

Wrangler::Renderer::Renderer(const RendererParameters& params): 
    assets(params.assets),
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
    Payload pbrShaderPayload;
    pbrShaderPayload.read(fs.readFile(params.pbrShaderPath).asString());

    std::string vertexPath = pbrShaderPayload.content["vertex"].get<std::string>();
    std::string fragmentPath = pbrShaderPayload.content["fragment"].get<std::string>();

    std::vector<byte> vertexData = fs.readFile(vertexPath).asBinary();
    std::vector<byte> fragmentData = fs.readFile(fragmentPath).asBinary();

    pbrShader.init(vertexData, fragmentData, pbrShaderPayload);

    if(!bgfx::isValid(pbrShader.program)) tul::FatalError({"Failed to create shader program"});

    // set pbr shader uniforms
    pbrShader.createUniform("u_baseColor", bgfx::UniformType::Vec4, 1);

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
    
    pbrShader.setGlobalUniforms(newParams);

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

        uint8_t textureStage = 0;
        for (const auto& uniform : mat->uniformData)
        {
            switch (uniform.type)
            {
                case bgfx::UniformType::Vec4:
                    pbrShader.setUniform(uniform.name, uniform.vector4);
                    break;

                case bgfx::UniformType::Mat3:
                    pbrShader.setUniform(uniform.name, uniform.mat3);
                    break;

                case bgfx::UniformType::Mat4:
                    pbrShader.setUniform(uniform.name, uniform.mat4);
                    break;

                case bgfx::UniformType::Sampler:
                {
                    const auto texture = assets.getTexture(uniform.sampler);

                    pbrShader.setTexture(
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
            pbrShader.handle()
        );
    }
}

/// @brief Ends the frame
void Wrangler::Renderer::end()
{
    camera = nullptr;
}
