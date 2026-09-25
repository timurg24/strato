#include "Wrangler/Renderer/Material.hpp"

#include "Wrangler/AssetManager/AssetManager.hpp"

// tul
#include <tul/ErrorOps.hpp>
#include <tul/StringOps.hpp>

/// @brief Loads a Material payload file
/// @note TODO: Drasticaly improve safety with Material2
/// @param payload Material payload
void Wrangler::Material::loadPayload(AssetManager& assets, const Payload& payload, Shader& shader)
{
    // Payload checking
    if(!payload.content.contains("name")) {
        tul::Alert({"Attempted to load a material with no name"});
        return;
    }

    std::string materialName = payload.content["name"].get<std::string>();

    if(!payload.content.contains("shader")) {
        tul::Alert({"Attempted to load a material with no required shader"});
        return;
    }

    if(payload.contentType != "Material" || payload.contentVersion != 1) {
        tul::Alert({"Attempted to invalid Material payload file:\nType: ", payload.contentType, "\nVersion: ", std::to_string(payload.contentVersion), "\nMaterial Name: ", materialName});
        return;
    }

    if(!payload.content.contains("uniforms") || !payload.content["uniforms"].is_array()) {
        tul::Alert({"Attempted to load a Material payload with no uniform data\nShader: ", materialName});
        return;
    }


    // loop thru the uniforms
    for(const auto& uniform : payload.content["uniforms"]) {

        // name
        if(!uniform.contains("name")) {
            tul::Alert({"Attempted to load a uniform with no name\nMaterial Name: ", materialName});
            return;
        }
        std::string name = uniform["name"].get<std::string>();
        if(!shader.uniformExists(name)) {
            tul::Alert({"Attempted to load data for a non-existant uniform\nMaterial Name: ", materialName, "\nUniform Name: ", name});
            return;
        }

        // type
        if(!uniform.contains("type")) {
            tul::Alert({"Attempted to load a uniform with no type\nMaterial Name: ", materialName, "\nUniform Name: ", name});
            return;
        }
        std::string type = uniform["type"].get<std::string>();

        // type checking
        if (type != "vec4" &&
            type != "mat3" &&
            type != "mat4" &&
            type != "sampler")
        {
            tul::Alert({
                "Attempted to load a uniform with a bad type\nMaterial Name: ",
                materialName,
                "\nUniform Name: ",
                name,
                "\nType: ",
                type
            });
            return;
        }

        // formulate MaterialUniform
        MaterialUniform result;
        result.name = name;

        // type loading
        if (type == "sampler")
        {
            AssetID texture = assets.loadTexture(
                uniform["data"].get<std::string>()
            );

            result.type = bgfx::UniformType::Sampler;
            result.sampler = texture;
        }
        else if (type == "vec4")
        {
            result.type = bgfx::UniformType::Vec4;

            const auto data =
                uniform["data"].get<std::vector<float>>();

            if (data.size() != 4)
            {
                tul::Alert({
                    "Material uniform has invalid vec4 data\nMaterial Name: ",
                    materialName,
                    "\nUniform Name: ",
                    result.name,
                    "\nExpected Values: 4\nReceived Values: ",
                    std::to_string(data.size())
                });
                return;
            }

            for (size_t i = 0; i < 4; ++i)
            {
                result.vector4[i] = data[i];
            }
        }
        else if (type == "mat3")
        {
            result.type = bgfx::UniformType::Mat3;

            const auto data =
                uniform["data"].get<std::vector<float>>();

            if (data.size() != 9)
            {
                tul::Alert({
                    "Material uniform has invalid mat3 data\nMaterial Name: ",
                    materialName,
                    "\nUniform Name: ",
                    result.name,
                    "\nExpected Values: 9\nReceived Values: ",
                    std::to_string(data.size())
                });
                return;
            }

            for (size_t i = 0; i < 9; ++i)
            {
                result.mat3[i] = data[i];
            }
        }
        else if (type == "mat4")
        {
            result.type = bgfx::UniformType::Mat4;

            const auto data =
                uniform["data"].get<std::vector<float>>();

            if (data.size() != 16)
            {
                tul::Alert({
                    "Material uniform has invalid mat4 data\nMaterial Name: ",
                    materialName,
                    "\nUniform Name: ",
                    result.name,
                    "\nExpected Values: 16\nReceived Values: ",
                    std::to_string(data.size())
                });
                return;
            }

            for (size_t i = 0; i < 16; ++i)
            {
                result.mat4[i] = data[i];
            }
        }
        uniformData.push_back(result);
    }

    tul::Print({"[Material] Loaded a material [", materialName, "]"});
}