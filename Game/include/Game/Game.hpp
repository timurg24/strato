#pragma once

/**
 * Strato class
 * Contains the game
 */

// Wrangler
#include <Wrangler/Core/Application.hpp>
#include <Wrangler/Renderer/Renderer.hpp>

// std
#include <chrono>

// TUL
#include <tul/CliOps.hpp>
#include <tul/ErrorOps.hpp>

// Strato
#include "Game/Settings.hpp"
#include "Aircraft/Aircraft.hpp"
#include "Input/Input.hpp"

class StratoGame {
public:
    Wrangler::Application app;
    Wrangler::Renderer renderer;

    StratoGame(Settings settings);

    Aircraft aircraft;
};