cmake_minimum_required(VERSION 3.5)

project(Strato
    VERSION 0.1.0
    LANGUAGES CXX
)

add_library(StratoDependencies INTERFACE)

target_link_libraries(StratoDependencies
    INTERFACE
        glfw
)

add_subdirectory(Engine)
add_subdirectory(Game)

