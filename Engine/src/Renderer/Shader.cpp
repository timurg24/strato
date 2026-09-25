#include "Wrangler/Renderer/Shader.hpp"

// tul
#include <tul/ErrorOps.hpp>

/// @brief Generates a shader handle based on shader data
/// @param data Shader data
/// @return Shader Handle
bgfx::ShaderHandle Wrangler::Shader::loadShader(const std::vector<byte> &data)
{
    if(data.empty()) tul::FatalError({"Attempted to load an empty shader"});
    const bgfx::Memory* memory = bgfx::copy(
        data.data(),
        static_cast<uint32_t>(data.size())
    );

    bgfx::ShaderHandle shader = bgfx::createShader(memory);

    if(!bgfx::isValid(shader)) tul::FatalError({"Failed to create shader"});

    return shader;
}

/// @brief Initalizes this shader
/// @param vertexData 
/// @param fragmentData 
void Wrangler::Shader::init(const std::vector<byte> &vertexData, const std::vector<byte> &fragmentData, const Payload& payload)
{
    if(!payload.content.contains("name")) tul::Alert({"Attempted to load a shader with no name"});
    std::string shaderName = payload.content["name"].get<std::string>();

    //=======================
    // Shader initialization
    //=======================
    bgfx::ShaderHandle vertex = loadShader(vertexData);
    bgfx::ShaderHandle fragment = loadShader(fragmentData);

    program = bgfx::createProgram(
        vertex,
        fragment,
        true
    );

    if(!bgfx::isValid(program)) tul::FatalError({"Failed to create shader program for shader: ", shaderName});
    initialized = true;

    //=======================
    //   Global Uniforms
    //=======================
    createUniform("cameraPos", bgfx::UniformType::Vec4, 1);

    // directional light
    createUniform("sunDirection", bgfx::UniformType::Vec4, 1);
    // w = active light count
    createUniform("sunAmbient", bgfx::UniformType::Vec4, 1);
    createUniform("sunDiffuse", bgfx::UniformType::Vec4, 1);
    createUniform("sunSpecular", bgfx::UniformType::Vec4, 1);

    // point lights
    createUniform("pointPosition", bgfx::UniformType::Vec4, MAX_LIGHT_COUNT);
    createUniform("pointMath", bgfx::UniformType::Vec4, MAX_LIGHT_COUNT);
    // x = constant
    // y = linear
    // z = quadratic
    // w = intensity
    createUniform("pointAmbient", bgfx::UniformType::Vec4, MAX_LIGHT_COUNT);
    createUniform("pointDiffuse", bgfx::UniformType::Vec4, MAX_LIGHT_COUNT);
    createUniform("pointSpecular", bgfx::UniformType::Vec4, MAX_LIGHT_COUNT);

    //=======================
    //   Payload Uniforms
    //=======================

    // TODO: Add better shader payload syntax error handling
    if(payload.contentType != "Shader" || payload.contentVersion != 1) {
        tul::Alert({"Attempted to invalid Shader payload file:\nType: ", payload.contentType, "\nVersion: ", std::to_string(payload.contentVersion), "\nShader Name: ", shaderName});
        return;
    }

    if(!payload.content.contains("uniforms") || !payload.content["uniforms"].is_array()) {
        tul::Alert({"Attempted to load a Shader payload with no uniform data\nShader: ", shaderName});
        return;
    }

    int shaderCount = 0;
    for(const auto& uniform : payload.content["uniforms"]) {
        std::string name = uniform["name"].get<std::string>();
        std::string typeStr = uniform["type"].get<std::string>();
        bgfx::UniformType::Enum type;

        if(typeStr == "vec4") type = bgfx::UniformType::Vec4;
        else if(typeStr == "mat3") type = bgfx::UniformType::Mat3;
        else if(typeStr == "mat4") type = bgfx::UniformType::Mat4;
        else if(typeStr == "sampler") type = bgfx::UniformType::Sampler;
        else {
            tul::Alert({"Invalid uniform type\nUniform: ", name, "\nShader: ", shaderName});
            continue;
        }
        shaderCount++;

        createUniform(name, type, 1);
    }

    tul::Print({"[Shader] Loaded a shader [", shaderName, ", ", std::to_string(shaderCount), " uniforms]\n"});

}

/// @brief Returns the shader program handle
/// @return Shader program handle
const bgfx::ProgramHandle &Wrangler::Shader::handle() const
{
    return program;
}

/// @brief Creates a new shader uniform
/// @param name Uniform name
/// @param type Uniform type
/// @param count Count
void Wrangler::Shader::createUniform(const std::string &name, bgfx::UniformType::Enum type, uint16_t count)
{
    if(!initialized) tul::Alert({"Attempted to create a uniform while Shader is unintialized: ", name});
    if(uniforms.contains(name)) tul::Alert({"Attempted to create a uniform while uniform with same name exists"});
    else uniforms[name] = bgfx::createUniform(name.c_str(), type, count);
}

/// @brief Sets a value to a uniform
/// @param name Uniform name
/// @param data Data
/// @param count Count
void Wrangler::Shader::setUniform(const std::string &name, const void *data, uint16_t count)
{
    if(!initialized) tul::Alert({"Attempted to create a uniform while Shader is unintialized: ", name});
    if(!uniforms.contains(name)) tul::FatalError({"Attempted to write data to non-existant shader uniform"});
    bgfx::setUniform(uniforms[name], data, count);
}

/// @brief Sets a texture
/// @param name Name
/// @param stage Stage
/// @param handle Texture Handle
void Wrangler::Shader::setTexture(const std::string &name, uint8_t stage, bgfx::TextureHandle handle)
{
    auto it = uniforms.find(name);

    if (it == uniforms.end())
    {
        tul::Alert({
            "Attempted to set non-existent sampler: ",
            name
        });
        return;
    }

    bgfx::setTexture(
        stage,
        it->second,
        handle
    );
}

/// @brief Checks if a uniform exists
/// @param name Uniform name
/// @note This is the cause for the C++20
/// @return True if it exsits
bool Wrangler::Shader::uniformExists(const std::string &name)
{
    if(uniforms.contains(name)) return true;
    return false;
}

/// @brief Sets the global uniforms
/// @param params 
void Wrangler::Shader::setGlobalUniforms(const ShaderSceneParameters &params)
{
     const uint16_t lightCount = static_cast<uint16_t>(
        std::min(params.pointLightCount, MAX_LIGHT_COUNT)
    );

    // Camera
    const float cameraPos[4] = {
        params.cameraPosition.x,
        params.cameraPosition.y,
        params.cameraPosition.z,
        0.0f
    };

    setUniform("cameraPos", cameraPos, 1);

    // Sun
    // w = active point light count
    const float sunDirection[4] = {
        params.sunDirection.x,
        params.sunDirection.y,
        params.sunDirection.z,
        static_cast<float>(lightCount)
    };

    const float sunAmbient[4] = {
        params.sunAmbient.x,
        params.sunAmbient.y,
        params.sunAmbient.z,
        0.0f
    };

    const float sunDiffuse[4] = {
        params.sunDiffuse.x,
        params.sunDiffuse.y,
        params.sunDiffuse.z,
        0.0f
    };

    const float sunSpecular[4] = {
        params.sunSpecular.x,
        params.sunSpecular.y,
        params.sunSpecular.z,
        0.0f
    };

    setUniform("sunDirection", sunDirection);
    setUniform("sunAmbient", sunAmbient);
    setUniform("sunDiffuse", sunDiffuse);
    setUniform("sunSpecular", sunSpecular);

    // Point lights
    float pointPosition[MAX_LIGHT_COUNT][4] = {};
    float pointMath[MAX_LIGHT_COUNT][4] = {};
    float pointAmbient[MAX_LIGHT_COUNT][4] = {};
    float pointDiffuse[MAX_LIGHT_COUNT][4] = {};
    float pointSpecular[MAX_LIGHT_COUNT][4] = {};

    for(uint16_t i = 0; i < lightCount; ++i)
    {
        const PointLight &light = params.pointLights[i];

        pointPosition[i][0] = light.position.x;
        pointPosition[i][1] = light.position.y;
        pointPosition[i][2] = light.position.z;

        pointMath[i][0] = light.constant;
        pointMath[i][1] = light.linear;
        pointMath[i][2] = light.quadratic;
        pointMath[i][3] = light.intensity;

        pointAmbient[i][0] = light.pointAmbient.x;
        pointAmbient[i][1] = light.pointAmbient.y;
        pointAmbient[i][2] = light.pointAmbient.z;

        pointDiffuse[i][0] = light.pointDiffuse.x;
        pointDiffuse[i][1] = light.pointDiffuse.y;
        pointDiffuse[i][2] = light.pointDiffuse.z;

        pointSpecular[i][0] = light.pointSpecular.x;
        pointSpecular[i][1] = light.pointSpecular.y;
        pointSpecular[i][2] = light.pointSpecular.z;
    }

    if(lightCount > 0)
    {
        setUniform("pointPosition", pointPosition, lightCount);
        setUniform("pointMath", pointMath, lightCount);
        setUniform("pointAmbient", pointAmbient, lightCount);
        setUniform("pointDiffuse", pointDiffuse, lightCount);
        setUniform("pointSpecular", pointSpecular, lightCount);
    }
}

Wrangler::Shader::~Shader()
{
    if(bgfx::isValid(program)) bgfx::destroy(program);
}
