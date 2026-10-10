#pragma once

/**
 * Renderer class
 * Renders stuff to the screen, yk
 */

// wrangler
#include "Wrangler/AssetManager/AssetManager.hpp"
#include "Wrangler/Filesystem/Filesystem.hpp"
#include "Wrangler/Light/Light.hpp"

// 3rd party
#include <GLFW/glfw3.h>

#if defined(_WIN32)
    #define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(__linux__)
    #define GLFW_EXPOSE_NATIVE_X11
#endif

#include <GLFW/glfw3native.h>

#ifdef Success
    #undef Success
#endif


#include <math/vec3.h>
#include <filament/Camera.h>
#include <filament/ColorGrading.h>
#include <filament/RenderableManager.h>
#include <filament/Renderer.h>
#include <filament/Scene.h>
#include <filament/View.h>
#include <filament/SwapChain.h>

namespace Wrangler {

    struct RenderableEntity {
        AssetID model;
        [[maybe_unused]] AssetID material;

        filament::math::float3 position{0.0f, 0.0f, 0.0f};
        filament::math::float3 rotation{0.0f, 0.0f, 0.0f};
        filament::math::float3 scale{1.0f, 1.0f, 1.0f};
    };

    struct RendererParameters {
        filament::Engine* filamentEngine;
        AssetManager& assets;
        const Filesystem& fs;
        GLFWwindow* window;

        int& width;
        int& height;
    };

    class Renderer {
    private:
        const AssetManager& assets;
        const Filesystem& fs;
        int& width;
        int& height;
        filament::Engine* filamentEngine;
        utils::Entity cameraEntity;
    public:
        filament::Renderer* filamentRenderer;
        filament::Camera* filamentCamera;
        filament::Scene* filamentScene;
        filament::View* filamentView;
        filament::SwapChain* filamentSwapChain;

        Renderer(const RendererParameters& params);

        void setSun(Light sun);
        void updateModelTransform(const RenderableEntity& entity);
        void addModel(const RenderableEntity& entity);
        void renderScene();

        ~Renderer();
    };

}