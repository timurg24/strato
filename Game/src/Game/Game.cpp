#include "Game/Game.hpp"

StratoGame::StratoGame(Settings settings):
    app({
        .title = "Strato Flight Simulator",
        .width = settings.width,
        .height = settings.height,
        .archivePath = "Content"
    }),
    renderer({
        .assets = app.assets,
        .fs = app.fs,
        .width = settings.width,
        .height = settings.height,
        .sceneShaderPath = "Shaders/pbr/pbr.pay"
    })
{

}
