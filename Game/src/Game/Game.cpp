#include "Game/Game.hpp"

#include <tul/ErrorOps.hpp>
#include <filament/TransformManager.h>

/// @brief Renders debug menus
void StratoGame::renderDebugMenus()
{
}

/// @brief Updates aircraft rotations
void StratoGame::updateAircraftTransform()
{
    auto model = app.assets.getModel(aircraft.entity.model);
    if(!model) {
        tul::Alert({"Attempted to update aircraft with no model set"});
        return;
    }

    auto& tm = app.filamentEngine->getTransformManager();

    auto root = model->asset->getRoot();
    auto instance = tm.getInstance(root);

    if(!instance) {
        tul::Alert({"Attempted to submit an entity with no instance set"});
        return;
    };

    using namespace filament::math;

    const float roll    = aircraft.entity.rotation.x;
    const float pitch   = aircraft.entity.rotation.y;
    const float heading = aircraft.entity.rotation.z;

    // JSBSim radians -> Filament Y-up rotation
    const mat4f rotation =
        mat4f::rotation(-heading, float3{0, 1, 0}) *
        mat4f::rotation( pitch,   float3{1, 0, 0}) *
        mat4f::rotation(-roll,    float3{0, 0, 1});

    // Translation * Rotation * Scale
    const mat4f transform =
        mat4f::translation(aircraft.entity.position) *
        rotation *
        mat4f::scaling(aircraft.entity.scale);

    tm.setTransform(instance, transform);
}

StratoGame::StratoGame(Settings settings) : app({.title = "Strato Flight Simulator",
                                                 .width = settings.width,
                                                 .height = settings.height,
                                                 .archivePath = "Content"}),
                                            renderer({.filamentEngine = app.filamentEngine,
                                                      .assets = app.assets,
                                                      .fs = app.fs,
                                                      .window = app.window,
                                                      .width = settings.width,
                                                      .height = settings.height})
{


    StartupState startup = {};
    aircraft.init(
        "Aircraft/Cessna 172P Skyhawk", "c172p", 
        startup,
        renderer,
        app.assets);

    Wrangler::Light sun{
        .position = {0.0f, 0.0f, 0.0f},
        .rotation = {0.4f, -1.0f, 0.3f},
        .color = {1.0f, 0.98f, 0.92f},
        .intensity = 100000.0f,
        .fallOff = 20.0f
    };

    renderer.addModel(aircraft.entity);
    renderer.setSun(sun);
}

/// @brief Update loop
void StratoGame::run()
{

    constexpr double fixedDt = 1.0 / STRATO_PHYS_HZ;

    double lastTime = glfwGetTime();
    double acc = 0.0;

    while(app.running()) {

        double now = glfwGetTime();
        double frameDt = std::min(now - lastTime, 0.1);
        lastTime = now;

        acc += frameDt;

        app.pollEvents();
        input.keyboardInput(app.window, aircraft.controlState, frameDt);

        while(acc >= fixedDt) {
            aircraft.update();
            acc -= fixedDt;
        }

        render();
    }
}

/// @brief Renders the game
void StratoGame::render()
{
    updateAircraftTransform();
    renderer.renderScene();
}
