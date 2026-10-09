#pragma once

/**
 * Renderer class
 * Renders stuff to the screen, yk
 */

// wrangler
#include "Wrangler/AssetManager/AssetManager.hpp"
#include "Wrangler/Filesystem/Filesystem.hpp"

// 3rd party
#include <math/vec3.h>
namespace Wrangler {

    struct RenderableEntity {
        AssetID model;
        AssetID material;

        filament::math::float3 position{0.0f, 0.0f, 0.0f};
        filament::math::float3 rotation{0.0f, 0.0f, 0.0f};
        filament::math::float3 scale{1.0f, 1.0f, 1.0f};
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
    public:
        Renderer(const RendererParameters& params);

        ~Renderer();
    };

}