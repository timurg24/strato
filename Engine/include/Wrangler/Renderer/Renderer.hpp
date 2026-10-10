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
#include <filament/RenderableManager.h>
#include <filament/Renderer.h>
#include <filament/Scene.h>

namespace Wrangler {

    struct RenderableEntity {
        AssetID model;
        AssetID material;

        filament::math::float3 position{0.0f, 0.0f, 0.0f};
        filament::math::float3 rotation{0.0f, 0.0f, 0.0f};
        filament::math::float3 scale{1.0f, 1.0f, 1.0f};
    };

    struct RendererParameters {
        filament::Engine* filamentEngine;
        AssetManager& assets;
        const Filesystem& fs;

        int& width;
        int& height;
    };

    class Renderer {
    private:
        const AssetManager& assets;
        const Filesystem& fs;
        int& width;
        int& height;
    public:
        filament::Renderer* filamentRenderer;
        Renderer(const RendererParameters& params);

        ~Renderer();
    };

}