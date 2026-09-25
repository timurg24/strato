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

        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;
        float intensity = 1.0f;

        bx::Vec3 pointAmbient{0.0f, 0.0f, 0.0f};
        bx::Vec3 pointDiffuse{1.0f, 1.0f, 1.0f};
        bx::Vec3 pointSpecular{1.0f, 1.0f, 1.0f};
    };

}