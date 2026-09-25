# =========================================================
# Wrangler Engine
# =========================================================

add_library(
    WranglerEngine
    STATIC

    src/stb_image.cpp

    src/Core/Application.cpp

    src/Filesystem/File.cpp
    src/Filesystem/Filesystem.cpp
    src/Filesystem/Payload.cpp

    src/Renderer/Shader.cpp
    src/Renderer/Model.cpp
    src/Renderer/Renderer.cpp
    src/Renderer/Material.cpp

    src/AssetManager/AssetManager.cpp
)


# =========================================================
# Wrangler public headers
# =========================================================

target_include_directories(
    WranglerEngine
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)


# =========================================================
# Wrangler dependencies
# =========================================================

target_link_libraries(
    WranglerEngine
    PUBLIC
        WranglerDependencies
)