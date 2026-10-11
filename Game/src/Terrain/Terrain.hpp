#pragma once

/**
 * Terrain system
 */

#include <Wrangler/Renderer/Renderer.hpp>

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

