#include "Game/Game.hpp"

// globals
int width = 1920;
int height = 1080;

int main(int argc, char** argv) {
    tul::SetupArguments(argc, argv);

    StratoGame game({width, height});

    // Flight
    StartupState startup = {};
    game.aircraft.init(
        "Aircraft/Cessna 172P Skyhawk", "c172p", 
        startup,
        game.renderer,
        game.app.assets);

    // Camera
    Wrangler::Camera camera;

    camera.position = bx::Vec3{
        0.0f,
        200.0f,
        800.0f
    };

    // 180 is facing the aircraft
    camera.rotation = {
        bx::toRad(-10.0f),
        bx::toRad(0.0f),
        0.0f
    };

    Wrangler::ShaderSceneParameters params{};

    params.cameraPosition = camera.position;

    // Sun
    params.sunDirection = bx::Vec3{
        -0.2f,
        -1.0f,
        0.15f
    };

    params.sunColor = bx::Vec3{
        0.15f,
        0.15f,
        0.15f
    };

    params.sunIntensity = 1.0f;
    Input input;

    params.pointLightCount = 0;

    using Clock = std::chrono::steady_clock;

    constexpr double maxFrameTime = 0.25;
    constexpr int maxPhysicsSteps = 16;

    double accumulator = 0.0;

    auto previousTime = Clock::now();

    bgfx::setDebug(BGFX_DEBUG_TEXT);
    while(game.app.running()) {

        // measure
        const auto currentTime = Clock::now();

        double frameDt =
            std::chrono::duration<double>(
                currentTime - previousTime
            ).count();
        
        previousTime = currentTime;

        if(frameDt > maxFrameTime) frameDt = maxFrameTime;

        accumulator += frameDt;

        game.app.pollEvents();

        // fixed step physics
        int physicsSteps = 0;

        while(accumulator >= game.aircraft.physicsDt && physicsSteps < maxPhysicsSteps) {
            input.keyboardInput(game.app.window, game.aircraft.controlState, game.aircraft.physicsDt);
            game.aircraft.update();
            accumulator -= game.aircraft.physicsDt;
            ++physicsSteps;
        }
        
        if (physicsSteps == maxPhysicsSteps)
        {
            accumulator = 0.0;
        }


        bgfx::setViewClear(
            0,
            BGFX_CLEAR_COLOR |
            BGFX_CLEAR_DEPTH,
            0x808080ff,
            1.0f,
            0
        );

        game.renderer.begin(camera);

        camera.updateValues(
            width,
            height
        );

        game.renderer.renderEntity(game.aircraft.entity, params);

        game.aircraft.drawDebugHUD();

        game.app.swapBuffers();
    }
}