#pragma once

/**
 * Camera struct
 * Contains data for the camera, what did you expect
 */

// 3rd party
#include <bgfx/bgfx.h>
#include <bx/math.h>

namespace Wrangler {

    struct Camera {

        bx::Vec3 position{0.0f, 0.0f, -5.0f};

        // radians
        // x = pitch
        // y = yaw
        // z = roll (unused for now)
        bx::Vec3 rotation{0.0f, 0.0f, 1.0f};
        float fov = 75;

        // do not edit
        float nearPlane = 0.01f;
        float farPlane = 10000.0f;

        float view[16];
        float projection[16];


        void updateValues(uint32_t width, uint32_t height) {
            const float pitch = rotation.x;
            const float yaw = rotation.y;

            // Camera forward direction
            bx::Vec3 forward
            {
                bx::cos(pitch) * bx::sin(yaw),
                bx::sin(pitch),
                bx::cos(pitch) * bx::cos(yaw)
            };

            // Point the camera is looking at
            bx::Vec3 target
            {
                position.x + forward.x,
                position.y + forward.y,
                position.z + forward.z
            };

            const bx::Vec3 up
            {
                0.0f,
                1.0f,
                0.0f
            };

            // View matrix
            bx::mtxLookAt(
                view,
                position,
                target,
                up
            );

            // Projection matrix
            const float aspect =
                static_cast<float>(width) /
                static_cast<float>(height);

            bx::mtxProj(
                projection,
                fov,
                aspect,
                nearPlane,
                farPlane,
                bgfx::getCaps()->homogeneousDepth
            );
        }
    };

}