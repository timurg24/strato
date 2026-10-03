// Wrangler
#include <Wrangler/Core/Application.hpp>
#include <Wrangler/Renderer/Renderer.hpp>

// std
#include <chrono>

// TUL
#include <tul/CliOps.hpp>
#include <tul/ErrorOps.hpp>

// Strato
#include "Aircraft/Aircraft.hpp"
#include "Input/Input.hpp"

// globals
int width = 1920;
int height = 1080;

int main(int argc, char** argv) {
    tul::SetupArguments(argc, argv);

    Wrangler::ApplicationParameters appParams = {
        .title = "Flight Simulator",
        .width = width,
        .height = height,
        .archivePath = "../Content/Strato"
    };

    Wrangler::Application app(appParams);

    Wrangler::RendererParameters rendererParams = {
        .assets = app.assets,
        .fs = app.fs,
        .width = width,
        .height = height,
        .pbrShaderPath = "payloads/shaders/pbr.pay"
    };

    Wrangler::Renderer renderer(rendererParams);

    // Flight
    StartupState startup = {};
    Aircraft cessna;
    cessna.init(
        "../Content/Aircraft/Cessna 172P Skyhawk", "c172p", 
        startup,
        renderer,
        app.assets);

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
        -0.6f,
        -1.0f,
        0.4f
    };

    params.sunAmbient = bx::Vec3{
        0.15f,
        0.15f,
        0.15f
    };

    params.sunDiffuse = bx::Vec3{
        1.0f,
        0.95f,
        0.85f
    };

    params.sunSpecular = bx::Vec3{
        1.0f,
        1.0f,
        1.0f
    };

    Input input;

    params.pointLightCount = 0;

    using Clock = std::chrono::steady_clock;

    constexpr double maxFrameTime = 0.25;
    constexpr int maxPhysicsSteps = 16;

    double accumulator = 0.0;

    auto previousTime = Clock::now();

    bgfx::setDebug(BGFX_DEBUG_TEXT);
    while(app.running()) {

        // measure
        const auto currentTime = Clock::now();

        double frameDt =
            std::chrono::duration<double>(
                currentTime - previousTime
            ).count();
        
        previousTime = currentTime;

        if(frameDt > maxFrameTime) frameDt = maxFrameTime;

        accumulator += frameDt;

        app.pollEvents();

        // fixed step physics
        int physicsSteps = 0;

        while(accumulator >= cessna.physicsDt && physicsSteps < maxPhysicsSteps) {
            input.keyboardInput(app.window, cessna.controlState, cessna.physicsDt);
            cessna.update();
            accumulator -= cessna.physicsDt;
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

        renderer.begin(camera);

        camera.updateValues(
            width,
            height
        );

        renderer.renderEntity(cessna.entity, params);

        cessna.drawDebugHUD();

        app.swapBuffers();
    }
}