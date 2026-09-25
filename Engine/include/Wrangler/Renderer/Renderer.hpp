#pragma once

/**
 * Renderer class
 * Renders stuff to the screen, yk
 */

// wrangler
#include "Wrangler/AssetManager/AssetManager.hpp"
#include "Wrangler/Filesystem/Filesystem.hpp"
#include "Wrangler/Renderer/Camera.hpp"

// 3rd party
#include <bgfx/bgfx.h>
#include <bx/math.h>

namespace Wrangler {

    struct RenderableEntity {
        AssetID model;
        AssetID material;

        bx::Vec3 position{0.0f, 0.0f, 0.0f};
        bx::Vec3 rotation{0.0f, 0.0f, 0.0f};
        bx::Vec3 scale{1.0f, 1.0f, 1.0f};
    };

    struct RendererParameters {
        const AssetManager& assets;
        const Filesystem& fs;
        int& width;
        int& height;
        const std::string pbrShaderPath;
    };

    class Renderer {
    private:
        const AssetManager& assets;
        const Filesystem& fs;
        int& width;
        int& height;
        const Camera* camera = nullptr;
        
    public:
        Shader pbrShader;
        Renderer(const RendererParameters& params);

        void begin(const Camera &camera);
        void renderEntity(const RenderableEntity& entity, const ShaderSceneParameters& params);
        void end();
    };

}