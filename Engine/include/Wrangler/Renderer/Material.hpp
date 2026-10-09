#pragma once

/**
 * Material struct
 * Contains material data
 */

// core
#include "Wrangler/Core/Types.hpp"
#include "Wrangler/Filesystem/Payload.hpp"

// 3rd party
#include <math/vec3.h>

namespace Wrangler {

    class AssetManager;

    struct MaterialUniform {
        std::string name;

        // TODO: Replace with GLM or something
        // TODO: Don't store data for each uniform type, ok for now
        float vector4[4] {
            1.0f,
            1.0f,
            1.0f,
            1.0f
        };

        float mat3[9] = {
            1, 0, 0,
            0, 1, 0,
            0, 0, 1
        };

        float mat4[16] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };

        AssetID sampler;
    };

    class Material {
    public:
    };

}