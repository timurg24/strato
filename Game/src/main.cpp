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

    Input input;

    using Clock = std::chrono::steady_clock;

    constexpr double maxFrameTime = 0.25;
    constexpr int maxPhysicsSteps = 16;

    double accumulator = 0.0;

    auto previousTime = Clock::now();
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

        game.app.swapBuffers();
    }
}