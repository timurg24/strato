#include "Wrangler/Renderer/Renderer.hpp"

#include <iostream>

// fs
#include "Wrangler/Filesystem/Payload.hpp"

// tul
#include <tul/ErrorOps.hpp>

// filament
#include <filament/LightManager.h>
#include <filament/Skybox.h>
#include <filament/TransformManager.h>
#include <utils/EntityManager.h>
#include <filament/Viewport.h>
#include <math/mat4.h>
#include <math/quat.h>

#ifdef assert_invariant
    #undef assert_invariant
#endif

Wrangler::Renderer::Renderer(const RendererParameters &params) : assets(params.assets),
                                                                 fs(params.fs),
                                                                 width(params.width),
                                                                 height(params.height),
                                                                 filamentEngine(params.filamentEngine)
{

    void* nativeWindow =  nullptr;

    #ifdef _WIN32
        nativeWindow = glfwGetWin32Window(params.window);
    #elif defined(__linux__)
        nativeWindow = reinterpret_cast<void*>(
            static_cast<uintptr_t>(glfwGetX11Window(params.window))
        );
    #endif

    filamentSwapChain = filamentEngine->createSwapChain(nativeWindow);

    cameraEntity = utils::EntityManager::get().create();
    

    filamentCamera = filamentEngine->createCamera(cameraEntity);
    filamentCamera->lookAt(
        {0.0, 5, 7.0},     // Camera position
        {0.0, 1.58, -1.89},   // Actual aircraft center
        {0.0, 1.0, 0.0}      // Up direction
    );

    filamentCamera->setProjection(
        60.0,
        static_cast<double>(params.width) / params.height,
        0.1,
        10000.0,
        filament::Camera::Fov::VERTICAL
    );

    filamentRenderer = filamentEngine->createRenderer();
    filamentScene = filamentEngine->createScene();
    filamentView = filamentEngine->createView();

    filamentView->setScene(filamentScene);
    filamentView->setCamera(filamentCamera);

    filamentView->setViewport(
        filament::Viewport{
            0,
            0,
            static_cast<uint32_t>(params.width),
            static_cast<uint32_t>(params.height)
        }
    );

    filament::Skybox* skybox = filament::Skybox::Builder()
        .color({0.2f, 0.5f, 0.9f, 1.0f})
        .build(*filamentEngine);

    filamentScene->setSkybox(skybox);

    filamentView->setFrustumCullingEnabled(false);
}

/// @brief Sets the sun
/// @param sun Sun values
void Wrangler::Renderer::setSun(Light sun)
{
    utils::Entity sunEntity = utils::EntityManager::get().create();
    
    filament::LightManager::Builder(
        filament::LightManager::Type::SUN
    )
        .color(sun.color)
        .intensity(sun.intensity)
        .direction(sun.rotation)
        .castShadows(true)
        .sunAngularRadius(0.545f)
        .sunHaloSize(10.0f)
        .sunHaloFalloff(sun.fallOff)
        .build(*filamentEngine, sunEntity);

    filamentScene->addEntity(sunEntity);
}

/// @brief Submits an entity to the scene to be drawn
/// @param entity Entity
void Wrangler::Renderer::submitModel(const RenderableEntity &entity)
{
    auto asset = assets.getModel(entity.model); // copy not reference
    if(!asset) {
        tul::Alert({"Attempted to submit an entity with no model set"});
        return;
    }

    // set transform
    auto& tm = filamentEngine->getTransformManager();

    auto root = asset->asset->getRoot();
    auto instance = tm.getInstance(root);

    if(!instance) {
        tul::Alert({"Attempted to submit an entity with no instance set"});
        return;
    };

    using namespace filament::math;

    // Convert Euler angles from degrees to radians.
    constexpr float DEG_TO_RAD = 0.017453292519943295f;

    const float rx = entity.rotation.x * DEG_TO_RAD;
    const float ry = entity.rotation.y * DEG_TO_RAD;
    const float rz = entity.rotation.z * DEG_TO_RAD;

    // Rotation order: Z * Y * X
    const mat4f rotation =
        mat4f::rotation(rz, float3{0.0f, 0.0f, 1.0f}) *
        mat4f::rotation(ry, float3{0.0f, 1.0f, 0.0f}) *
        mat4f::rotation(rx, float3{1.0f, 0.0f, 0.0f});

    // Translation * Rotation * Scale
    const mat4f transform =
        mat4f::translation(entity.position) *
        rotation *
        mat4f::scaling(entity.scale);

    tm.setTransform(instance, transform);

    filamentScene->addEntities(
        asset->asset->getEntities(),
        asset->asset->getEntityCount()
    );

    std::cout << "Model entities: "
        << asset->asset->getEntityCount() << '\n';

    std::cout << "Scene renderables: "
            << filamentScene->getRenderableCount() << '\n';

    std::cout << "Scene lights: "
            << filamentScene->getLightCount() << '\n';

    const auto bounds = asset->asset->getBoundingBox();

const auto center = bounds.center();
const auto extent = bounds.extent();

std::cout << "Model center: "
          << center.x << ", "
          << center.y << ", "
          << center.z << '\n';

std::cout << "Model half extent: "
          << extent.x << ", "
          << extent.y << ", "
          << extent.z << '\n';
}

/// @brief Draws the scene and clears it for the next frame
void Wrangler::Renderer::renderScene()
{
    if (filamentRenderer->beginFrame(filamentSwapChain))
    {
        filamentRenderer->render(filamentView);
        filamentRenderer->endFrame();
    }
}

Wrangler::Renderer::~Renderer() {}
