
cmake_minimum_required(VERSION 3.22.1)

project(Strato
    VERSION 0.1.0
    LANGUAGES C CXX
)

# =========================================================
# MSVC Runtime Configuration
# =========================================================

if(MSVC)
    # Release: /MT
    # Debug:   /MTd
    set(CMAKE_MSVC_RUNTIME_LIBRARY
        "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    )

    # Match Filament's static CRT configuration
    set(USE_STATIC_CRT ON CACHE BOOL
        "Use static MSVC runtime" FORCE
    )
endif()

# =========================================================
# C++ Configuration
# =========================================================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POLICY_VERSION_MINIMUM 3.10)

include(FetchContent)

# =========================================================
# Global Build Configuration
# =========================================================

# Prefer static libraries
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

set(
    PHYSFS_TARGETNAME_DIST
    "physfs-dist"
    CACHE STRING "" FORCE
)

FetchContent_Declare(
    physfs
    GIT_REPOSITORY https://github.com/icculus/physfs.git
    GIT_TAG release-3.2.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(physfs)

# =========================================================
# Google Filament
# =========================================================

message(STATUS "Adding Google Filament")

set(FILAMENT_DIR
    "${CMAKE_SOURCE_DIR}/thirdparty/filament"
)

# Rendering backends
set(FILAMENT_SUPPORTS_OPENGL OFF CACHE BOOL "" FORCE)
set(FILAMENT_SUPPORTS_VULKAN ON CACHE BOOL "" FORCE)

# Optional features
set(FILAMENT_ENABLE_MATDBG OFF CACHE BOOL "" FORCE)
set(FILAMENT_SKIP_SAMPLES ON CACHE BOOL "" FORCE)
set(FILAMENT_BUILD_TESTING OFF CACHE BOOL "" FORCE)

# GCC support (experimental upstream)
set(
    FILAMENT_ENABLE_EXPERIMENTAL_GCC_SUPPORT
    ON CACHE BOOL "" FORCE
)

add_subdirectory(
    "${FILAMENT_DIR}"
    "${CMAKE_BINARY_DIR}/thirdparty/filament"
    EXCLUDE_FROM_ALL
)

# =========================================================
# Dear ImGui - GLFW Platform Backend
# =========================================================

message(STATUS "Adding ImGui GLFW backend")

# Filament already provides Dear ImGui and filagui.
# We only compile the GLFW platform backend.

set(IMGUI_DIR
    "${FILAMENT_DIR}/third_party/imgui"
)

set(IMGUI_BACKENDS_DIR
    "${IMGUI_DIR}/backends"
)

if(NOT EXISTS
    "${IMGUI_BACKENDS_DIR}/imgui_impl_glfw.cpp"
)
    message(FATAL_ERROR
        "ImGui GLFW backend not found in Filament: "
        "${IMGUI_BACKENDS_DIR}"
    )
endif()

add_library(imgui_glfw STATIC
    "${IMGUI_BACKENDS_DIR}/imgui_impl_glfw.cpp"
)

target_include_directories(imgui_glfw
    PUBLIC
        "${IMGUI_BACKENDS_DIR}"
)

target_link_libraries(imgui_glfw
    PUBLIC
        imgui
        glfw
)

# =========================================================
# libcurl
# =========================================================

message(STATUS "Adding libcurl")

set(BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)
set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CURL_DISABLE_INSTALL ON CACHE BOOL "" FORCE)

# HTTP and HTTPS only
set(HTTP_ONLY ON CACHE BOOL "" FORCE)

# Disable unnecessary dependencies
set(CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)
set(CURL_BROTLI OFF CACHE STRING "" FORCE)
set(CURL_ZSTD OFF CACHE STRING "" FORCE)
set(USE_NGHTTP2 OFF CACHE BOOL "" FORCE)

# TLS configuration
if(WIN32)
    set(CURL_USE_SCHANNEL ON CACHE BOOL "" FORCE)
    set(CURL_USE_OPENSSL OFF CACHE BOOL "" FORCE)

    if(MSVC)
        set(CURL_STATIC_CRT ON CACHE BOOL "" FORCE)
    endif()
else()
    set(CURL_USE_OPENSSL ON CACHE BOOL "" FORCE)
endif()

FetchContent_Declare(
    curl
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG curl-8_22_0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(curl)

# =========================================================
# Strato Dependencies
# =========================================================

message(STATUS "Creating Strato dependencies")

add_library(StratoDependencies INTERFACE)

target_link_libraries(StratoDependencies
    INTERFACE

        # Rendering
        filament
        gltfio
        ktxreader

        # GUI
        filagui
        imgui_glfw

        # Window and Input
        glfw

        # Utilities
        tul

        # Filesystem
        PhysFS::PhysFS-static

        # Networking
        CURL::libcurl
)

target_include_directories(StratoDependencies
    INTERFACE
        "${CMAKE_SOURCE_DIR}/thirdparty/single_include"
)

# =========================================================
# Wrangler Engine
# =========================================================

message(STATUS "Adding Wrangler Engine")

add_subdirectory(Engine)

# =========================================================
# Strato Game
# =========================================================

message(STATUS "Adding Strato Game")

add_subdirectory(Game)

message(STATUS "Strato configuration complete")
