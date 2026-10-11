#pragma once

/**
 * Strato class
 * Contains the game
 */

// Wrangler
#include <Wrangler/Core/Application.hpp>
#include <Wrangler/Renderer/Renderer.hpp>

// TUL
#include <tul/CliOps.hpp>
#include <tul/ErrorOps.hpp>

// Strato
#include "Game/Settings.hpp"
#include "Aircraft/Aircraft.hpp"
#include "Input/Input.hpp"

namespace Strato {
    class Game {
    private:
        void renderDebugMenus();
        void updateAircraftTransform();

        void physicsUpdate();
        void render();
    public:
        Wrangler::Application app;
        Wrangler::Renderer renderer;
        Input input;
        Aircraft aircraft;

        Game(Settings settings);

        void run();
    };
}