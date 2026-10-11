#pragma once

/**
 * Terrain system
 */

#include <utils/debug.h>

#include <Wrangler/Renderer/Renderer.hpp>
#include <filament/Texture.h>

#define STRATO_TERRAIN_QUALITY 256

namespace Strato {
    class Terrain {
    private:
        filament::Texture* elevationTexture;
        void updateTerrainTexture();
    public:
        Terrain(filament::Engine& engine);
    };
}

