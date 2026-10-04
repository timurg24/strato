add_executable(Strato
    src/main.cpp

    src/Game/Game.cpp
    src/Aircraft/Aircraft.cpp
    src/Input/Input.cpp
)

target_include_directories(Strato
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# =========================================================
# JSBSim
# =========================================================

message(STATUS "Adding JSBSim")

set(JSBSIM_BUILD_PROGRAMS OFF CACHE BOOL "" FORCE)
set(JSBSIM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(JSBSIM_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    jsbsim
    GIT_REPOSITORY https://github.com/JSBSim-Team/jsbsim.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(jsbsim)

target_link_libraries(Strato
    PRIVATE
        Wrangler
        libJSBSim
)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

add_dependencies(Strato shaderc)

add_custom_target(
    cook_content ALL

    COMMAND
        ${Python3_EXECUTABLE}
        ${CMAKE_SOURCE_DIR}/Tools/scripts/cook.py
        ${CMAKE_SOURCE_DIR}/Content
        ${CMAKE_BINARY_DIR}
        $<TARGET_FILE_DIR:Strato>

    DEPENDS
        Strato
        shaderc

    COMMENT "Cooking Wrangler content..."
    VERBATIM
)