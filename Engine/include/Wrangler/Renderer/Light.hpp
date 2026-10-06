#pragma once

/**
 * Light struct
 * Store light data
 */

// core
#include "Wrangler/Core/Types.hpp"

// 3rd party
#include <bx/math.h>

namespace Wrangler {

    struct PointLight
    {
        bx::Vec3 position{0.0f, 0.0f, 0.0f};

        float range = 1.0f;
        float intensity = 1.0f;

        bx::Vec3 pointColor{0.0f, 0.0f, 0.0f};
    };

}