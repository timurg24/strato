#pragma once

/**
 * Light struct
 * Store light data
 */

// core
#include "Wrangler/Core/Types.hpp"

// 3rd party
#include <math/vec3.h>

namespace Wrangler {

    struct PointLight
    {
        filament::math::float3 position{0.0f, 0.0f, 0.0f};

        float range = 1.0f;
        float intensity = 1.0f;

        filament::math::float3 pointColor{0.0f, 0.0f, 0.0f};
    };

}