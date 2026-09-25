#pragma once

/**
 * Types
 * Contains different types and defines
 */

// std
#include <cstdint>

#define MAX_LIGHTS 16 // per scene, nearby only

namespace Wrangler {
    using byte = uint8_t;
    using AssetID = uint32_t;

    struct Vec2 {
        float x, y;
    };
}