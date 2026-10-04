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

    struct Pipeline {
        std::unique_ptr<Shader> shadowShader;
        std::unique_ptr<Shader> sceneShader;
        std::unique_ptr<Shader> postProcessShader;
    };

    struct RenderableEntity {
        AssetID model;
        AssetID material;

        bx::Vec3 position{0.0f, 0.0f, 0.0f};
        bx::Vec3 rotation{0.0f, 0.0f, 0.0f};
        bx::Vec3 scale{1.0f, 1.0f, 1.0f};
    };

    struct RendererParameters {
        AssetManager& assets;
        const Filesystem& fs;

        int& width;
        int& height;

        std::string shadowShaderPath;
        std::string sceneShaderPath;
        std::string postProcessShaderPath;
    };

    class Renderer {
    private:
        const AssetManager& assets;
        const Filesystem& fs;
        int& width;
        int& height;
        const Camera* camera = nullptr;
        
        void loadShaderFromPath(const std::string& path, Shader& target);
    public:
        Pipeline mainPipeline;
        Renderer(const RendererParameters& params);

        void begin(const Camera &camera);
        void renderEntity(const RenderableEntity& entity, const ShaderSceneParameters& params);
        void end();

        ~Renderer();
    };

}