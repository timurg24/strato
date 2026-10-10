#pragma once

/**
 * Application class
 * The main engine class, contains the window, renderer, fs, etc.
 */

// std
#include <string>
#include <filesystem>

// Wrangler
#include "Wrangler/AssetManager/AssetManager.hpp"
#include "Wrangler/Renderer/Renderer.hpp"

// 3rd party
#include <GLFW/glfw3.h>
#include <filament/Engine.h>
#include <filament/IndexBuffer.h>
#include <filament/RenderableManager.h>
#include <filament/Renderer.h>

namespace Wrangler {

    struct ApplicationParameters {
        std::string title;
        int width, height;
        std::filesystem::path archivePath;
    };

    class Application {
    private:
        int width, height;
    public:
    
        GLFWwindow* window = nullptr;
        filament::Engine* filamentEngine = nullptr;
        Filesystem fs;
        AssetManager assets;

        Application(const ApplicationParameters& params);
        bool keyDown(int key) const;
        bool running();
        void swapBuffers();
        void pollEvents();
        ~Application();
    };

}