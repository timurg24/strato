#include "Wrangler/Core/Application.hpp"

// 3rd party
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <bgfx/bgfx.h>

// tul
#include <tul/ErrorOps.hpp>
#include <tul/CliOps.hpp>

Wrangler::Application::Application(const ApplicationParameters& params): 
    width(params.width), 
    height(params.height),
    fs(tul::globalArgs[0].c_str(), params.archivePath),
    assets(fs)
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
    
    // BGFX
    bool badAPI = false;

    bgfx::Init init{};
    std::string api = tul::GetValue("graphics");
    if(api == "dx11")
        init.type           = bgfx::RendererType::Direct3D11;
    else if(api == "dx12")
        init.type           = bgfx::RendererType::Direct3D12;
    else if(api == "vulkan")
        init.type           = bgfx::RendererType::Vulkan;
    else if(api == "gl")
        init.type           = bgfx::RendererType::OpenGL;
    else if(api != "") {
        tul::Alert({"Uknown graphics API: ", api, "\nAutomatically selecting API..."});
        init.type           = bgfx::RendererType::Count;
        badAPI = true;
    }
    init.vendorId           = BGFX_PCI_ID_NONE;

    init.swapChain.nwh      = glfwGetWin32Window(window);
    init.swapChain.ndt      = nullptr;

    init.swapChain.width    = width;
    init.swapChain.height   = height;

    init.reset              = BGFX_RESET_VSYNC;


    // todo: add custom callback to get why it crashed
    if(!bgfx::init(init)) tul::FatalError({"Failed to initialize BGFX, an unsupported graphics API may be in use"});
    
    tul::Print({"[Application] Initialized BGFX (", bgfx::getRendererName(bgfx::getRendererType()), ")\n"});
    if(badAPI) tul::Alert({"Selected graphics API: ", bgfx::getRendererName(bgfx::getRendererType())});;
    
    Wrangler::Vertex::init();
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
    bgfx::frame();
}

/// @brief Polls events
void Wrangler::Application::pollEvents()
{
    glfwPollEvents();
}

Wrangler::Application::~Application()
{
    bgfx::shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
}
