#pragma once

/**
 * Shader class
 * Contains a shader program
 */

// core
#include "Wrangler/Core/Types.hpp"
#include "Wrangler/Renderer/Light.hpp"
#include "Wrangler/Filesystem/Payload.hpp"

// 3rd party
#include <bgfx/bgfx.h>
#include <bx/math.h>

// std
#include <string>
#include <vector>
#include <unordered_map>

// constants
#include "Wrangler/Constants.h"

namespace Wrangler {

    struct ShaderSceneParameters
    {
        bx::Vec3 cameraPosition { 0.0f, 0.0f, 0.0f };

        // Sun
        bx::Vec3 sunDirection { 0.0f, -1.0f, 0.0f };
        bx::Vec3 sunAmbient   { 0.1f, 0.1f, 0.1f };
        bx::Vec3 sunDiffuse   { 1.0f, 1.0f, 1.0f };
        bx::Vec3 sunSpecular  { 1.0f, 1.0f, 1.0f };

        // Point lights
        PointLight pointLights[MAX_LIGHT_COUNT];
        int pointLightCount = 0;
    };

    class Shader {
    private:
        bool initialized = false;
        std::unordered_map<std::string, bgfx::UniformHandle> uniforms;
    public:
        bgfx::ProgramHandle program = BGFX_INVALID_HANDLE;
        bgfx::ShaderHandle loadShader(const std::vector<byte>& data);
        Shader() = default;
        void init(const std::vector<byte>& vertexData, const std::vector<byte>& fragmentData, const Payload& payload);
        const bgfx::ProgramHandle& handle() const;

        void createUniform(const std::string& name, bgfx::UniformType::Enum type, uint16_t count = 1);
        void setUniform(const std::string& name, const void* data, uint16_t count = 1);
        void setTexture(const std::string& name, uint8_t stage, bgfx::TextureHandle handle);
        bool uniformExists(const std::string& name);
        
        void setGlobalUniforms(const ShaderSceneParameters& params);

        ~Shader();
    };

}