// Wrangler
#include <Wrangler/Core/Application.hpp>
#include <Wrangler/Renderer/Renderer.hpp>

// TUL
#include <tul/CliOps.hpp>
#include <tul/ErrorOps.hpp>

#include "Aircraft/Aircraft.hpp"

// globals
int width = 1920;
int height = 1080;

int main(int argc, char** argv) {
    tul::SetupArguments(argc, argv);

    //=============
    // Application
    //=============

    Wrangler::ApplicationParameters appParams = {
        .title = "Flight Simulator",
        .width = width,
        .height = height,
        .archivePath = "../Content/Flight Simulator - Assets.pak"
    };

    Wrangler::Application app(appParams);

    //=============
    //  Renderer
    //=============

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
    startup.setDefaultValues();
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
        0.0f,
        -5.0f
    };

    camera.rotation = bx::Vec3{
        0.0f, // pitch
        0.0f, // yaw
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

    params.pointLightCount = 0;

    while(app.running()) {
        app.pollEvents();

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

        app.swapBuffers();
    }
}