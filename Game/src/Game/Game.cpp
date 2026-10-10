#include "Game/Game.hpp"

StratoGame::StratoGame(Settings settings):
    app({
        .title = "Strato Flight Simulator",
        .width = settings.width,
        .height = settings.height,
        .archivePath = "Content"
    }),
    renderer({
        .filamentEngine = app.filamentEngine,
        .assets = app.assets,
        .fs = app.fs,
        .window = app.window,
        .width = settings.width,
        .height = settings.height
    })
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

    renderer.setSun(sun);
    renderer.submitModel(aircraft.entity);
}

/// @brief Renders the game
void StratoGame::render()
{
    renderer.renderScene();
}
