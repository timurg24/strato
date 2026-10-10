cmake_minimum_required(VERSION 3.5)

project(Strato
    VERSION 0.1.0
    LANGUAGES CXX
)

include(FetchContent)

# =========================================================
# C++ configuration
# =========================================================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POLICY_VERSION_MINIMUM 3.10)

include(FetchContent)


# =========================================================
# Global build configuration
# =========================================================

# Prefer static libraries for third-party dependencies.
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# =========================================================
# GLFW
# =========================================================

message(STATUS "Adding GLFW")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glfw)


# =========================================================
# TUL
# =========================================================

message(STATUS "Adding TUL")

FetchContent_Declare(
    tul
    GIT_REPOSITORY https://github.com/timurg24/tul.git
    GIT_TAG main
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(tul)


# =========================================================
# PhysFS
# =========================================================

message(STATUS "Adding PhysFS")

set(PHYSFS_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(PHYSFS_BUILD_STATIC ON CACHE BOOL "" FORCE)
set(PHYSFS_BUILD_TEST OFF CACHE BOOL "" FORCE)
set(PHYSFS_INSTALL OFF CACHE BOOL "" FORCE)
set(PHYSFS_TARGETNAME_DIST "physfs-dist" CACHE STRING "" FORCE)

FetchContent_Declare(
    physfs
    GIT_REPOSITORY https://github.com/icculus/physfs.git
    GIT_TAG release-3.2.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(physfs)

# =========================================================
# Filament
# =========================================================
set(FILAMENT_SUPPORTS_OPENGL OFF CACHE BOOL "" FORCE)
set(FILAMENT_SUPPORTS_VULKAN ON CACHE BOOL "" FORCE)
set(FILAMENT_ENABLE_MATDBG OFF CACHE BOOL "" FORCE)

set(FILAMENT_ENABLE_EXPERIMENTAL_GCC_SUPPORT ON CACHE BOOL "" FORCE)

# Build Filament
add_subdirectory(
    thirdparty/filament
    ${CMAKE_BINARY_DIR}/thirdparty/filament
    EXCLUDE_FROM_ALL
)

message(STATUS "Adding Google Filament")

message(STATUS "Creating Strato dependencies")

add_library(StratoDependencies INTERFACE)

target_link_libraries(
    StratoDependencies
    INTERFACE
        filament
        ktxreader
        gltfio
        glfw
        tul
        PhysFS::PhysFS-static
)

target_include_directories(
    StratoDependencies
    INTERFACE
        "${CMAKE_SOURCE_DIR}/thirdparty/single_include"
)

message(STATUS "Adding Wrangler Engine")
add_subdirectory(Engine)
message(STATUS "Adding Strato Game")
add_subdirectory(Game)

