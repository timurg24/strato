add_library(Wrangler
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

target_include_directories(Wrangler
    PUBLIC
        $"{CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(Wrangler
    PUBLIC
        StratoDependencies
)