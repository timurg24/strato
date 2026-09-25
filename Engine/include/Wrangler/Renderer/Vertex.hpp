#pragma once

/**
 * Vertex struct
 * Contains per vertex data
 */

// core
#include "Wrangler/Core/Types.hpp"

// 3rd party
#include <bgfx/bgfx.h>
#include <bx/math.h>

namespace Wrangler {

    struct Vertex {

        bx::Vec3 position{0.0f, 0.0f, 0.0f};
        bx::Vec3 normal{0.0f, 0.0f, 0.0f};
        Vec2 texCoords{0.0f, 0.0f};

        
        static inline bgfx::VertexLayout layout;

        static void init()
        {
            layout
                .begin()
                .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Normal,    3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
                .end();
        
        }
    };

}