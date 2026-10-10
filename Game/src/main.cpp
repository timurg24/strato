#include "Game/Game.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>

// Globals
int width = 1920;
int height = 1080;

int main(int argc, char** argv)
{
    tul::SetupArguments(argc, argv);

    StratoGame game({width, height});

    using Clock = std::chrono::steady_clock;

    constexpr double maxFrameTime = 0.25;
    constexpr int maxPhysicsSteps = 16;
    constexpr double targetFPS = 60.0;

    const auto targetFrameDuration =
        std::chrono::duration<double>(1.0 / targetFPS);

    double accumulator = 0.0;

    // FPS counter
    int frameCount = 0;
    auto fpsStartTime = Clock::now();

    auto previousTime = Clock::now();

    while (game.app.running())
    {
        const auto frameStart = Clock::now();

        // Calculate actual frame delta
        const double frameDt =
            std::chrono::duration<double>(
                frameStart - previousTime
            ).count();

        previousTime = frameStart;

        // Clamp only the physics accumulator
        accumulator += std::min(frameDt, maxFrameTime);

        // Process window events
        game.app.pollEvents();

        // Fixed timestep physics
        int physicsSteps = 0;

        while (
            accumulator >= game.aircraft.physicsDt &&
            physicsSteps < maxPhysicsSteps
        )
        {
            game.input.keyboardInput(
                game.app.window,
                game.aircraft.controlState,
                game.aircraft.physicsDt
            );

            game.aircraft.update();

            accumulator -= game.aircraft.physicsDt;
            ++physicsSteps;
        }

        // Prevent runaway physics catch-up
        if (physicsSteps == maxPhysicsSteps)
        {
            accumulator = 0.0;
        }

        // Interpolation factor for future visual smoothing
        const double alpha =
            accumulator / game.aircraft.physicsDt;

        // TODO: Use alpha in updateVisuals(alpha)
        (void)alpha;

        // Render frame (Filament handles swap chain presentation)
        game.render();

        // Frame pacing
        std::this_thread::sleep_until(
            frameStart +
            std::chrono::duration_cast<Clock::duration>(
                targetFrameDuration
            )
        );

        // FPS statistics
        ++frameCount;

        const auto now = Clock::now();

        const double fpsElapsed =
            std::chrono::duration<double>(
                now - fpsStartTime
            ).count();

        if (fpsElapsed >= 1.0)
        {
            const double fps =
                static_cast<double>(frameCount) / fpsElapsed;

            std::cout
                << "FPS: " << fps
                << '\n';

            frameCount = 0;
            fpsStartTime = now;
        }
    }

    return 0;
}