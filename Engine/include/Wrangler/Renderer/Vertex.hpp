#pragma once

/**
 * Vertex struct
 * Contains per vertex data
 */

// core
#include "Wrangler/Core/Types.hpp"

// 3rd party
#include <math/vec3.h>

namespace Wrangler {

    struct Vertex {

        filament::math::float3 position{0.0f, 0.0f, 0.0f};
        filament::math::float3 normal{0.0f, 0.0f, 0.0f};
        Vec2 texCoords{0.0f, 0.0f};

        static void init()
        {
            
        
        }
    };

}