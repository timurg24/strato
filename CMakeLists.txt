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
# bgfx
# =========================================================

message(STATUS "Adding BGFX")

set(BGFX_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BGFX_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BGFX_INSTALL OFF CACHE BOOL "" FORCE)

# Needed for shaderc.
set(BGFX_BUILD_TOOLS ON CACHE BOOL "" FORCE)
set(BGFX_BUILD_TOOLS_SHADER ON CACHE BOOL "" FORCE)

set(BGFX_BUILD_TOOLS_GEOMETRY OFF CACHE BOOL "" FORCE)
set(BGFX_BUILD_TOOLS_TEXTURE OFF CACHE BOOL "" FORCE)
set(BGFX_BUILD_TOOLS_BIN2C OFF CACHE BOOL "" FORCE)

add_subdirectory(
    thirdparty/bgfx.cmake
    EXCLUDE_FROM_ALL
)


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

FetchContent_Declare(
    physfs
    GIT_REPOSITORY https://github.com/icculus/physfs.git
    GIT_TAG release-3.2.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(physfs)


# =========================================================
# Assimp
# =========================================================

message(STATUS "Adding Assimp")

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)

# Disable exporters.
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)

# Disable every importer by default.
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)

# Enable only the formats Strato uses.
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v6.0.2
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(assimp)

message(STATUS "Creating Strato dependencies")

add_library(StratoDependencies INTERFACE)

target_link_libraries(
    StratoDependencies
    INTERFACE
        bgfx
        bx
        bimg
        glfw
        tul
        PhysFS::PhysFS-static
        assimp
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

