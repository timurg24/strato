#pragma once

/**
 * Application class
 * The main engine class, contains the window, renderer, fs, etc.
 */

// std
#include <string>
#include <filesystem>

// 3rd party
#include <GLFW/glfw3.h>

// Wrangler
#include "Wrangler/AssetManager/AssetManager.hpp"
#include "Wrangler/Renderer/Renderer.hpp"

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