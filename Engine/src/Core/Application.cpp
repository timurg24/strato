#include "Wrangler/Core/Application.hpp"

// 3rd party
#include <GLFW/glfw3.h>

#ifdef _WIN32
    #define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(__linux__)
    #define GLFW_EXPOSE_NATIVE_X11
#endif

#include <GLFW/glfw3native.h>
// tul
#include <tul/ErrorOps.hpp>
#include <tul/CliOps.hpp>

Wrangler::Application::Application(const ApplicationParameters& params): 
    width(params.width), 
    height(params.height),
    fs(tul::globalArgs[0].c_str(), params.archivePath),
    filamentEngine(filament::Engine::create(
        filament::Engine::Backend::VULKAN
    )),
    assets(fs, filamentEngine)
{
    // GLFW
    if(!glfwInit()) tul::FatalError({"Failed to initialize GLFW"});
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    
    window = glfwCreateWindow(
        width,
        height,
        params.title.c_str(),
        nullptr,
        nullptr
    );
    
    if(!window) tul::FatalError({"Failed to create GLFW window"});
    tul::Print({"[Application] Created GLFW window\n"});
}

/// @brief Returns if a key is down
/// @param key GLFW key
/// @return Bool if pressed
bool Wrangler::Application::keyDown(int key) const
{
    return glfwGetKey(window, key) == GLFW_PRESS;
}

/// @brief If the game should still be open
/// @return True if running
bool Wrangler::Application::running()
{
    return !glfwWindowShouldClose(window);
}

/// @brief Swaps buffers
void Wrangler::Application::swapBuffers()
{
}

/// @brief Polls events
void Wrangler::Application::pollEvents()
{
    glfwPollEvents();
}

Wrangler::Application::~Application()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}
