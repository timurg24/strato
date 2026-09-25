#pragma once

/**
 * CPUMesh struct
 * Contains vertices, indices, etc.
 */

// core
#include "Wrangler/Core/Types.hpp"

// renderer
#include "Wrangler/Renderer/Vertex.hpp"

// std
#include <vector>

namespace Wrangler {
    class CPUMesh {
    public:
        std::vector<Vertex>         vertices;
        std::vector<unsigned int>   indices;
        int materialSlot = 0;
    };
}