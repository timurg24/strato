#pragma once

/**
 * Light struct
 */

#include <math/vec3.h>

namespace Wrangler {
    struct Light {
        filament::math::float3 position{0.0f, 0.0f, 0.0f};
        filament::math::float3 rotation{0.0f, 0.0f, 0.0f};
        filament::math::float3 color{1.0f, 1.0f, 1.0f};
        float intensity = 1500.0f;
        float fallOff = 20.0f;
    };
}