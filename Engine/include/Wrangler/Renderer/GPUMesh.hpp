#pragma once

/**
 * Mesh class
 * Contains GPU mesh data
 */

// core
#include "Wrangler/Core/Types.hpp"

// renderer
#include "Wrangler/Renderer/Vertex.hpp"

// std
#include <vector>

// 3rd party
#include <bgfx/bgfx.h>

namespace Wrangler {

    struct GPUMesh {
        bgfx::VertexBufferHandle vertexBuffer = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle indexBuffer = BGFX_INVALID_HANDLE;
        int materialSlot = 0;
    };

}