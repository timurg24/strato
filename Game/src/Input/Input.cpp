#include "Input/Input.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>

void Input::clampSignedNormal(SignedNormal& value)
{
    value = std::clamp(
        value,
        SignedNormal{-1.0},
        SignedNormal{1.0}
    );
}

void Input::clampUnsignedNormal(UnsignedNormal& value)
{
    value = std::clamp(
        value,
        UnsignedNormal{0.0},
        UnsignedNormal{1.0}
    );
}

/// @brief Generates a ControlState via the keyboard
/// @param window GLFW Window
/// @param state Control State
void Input::keyboardInput(GLFWwindow *window, ControlState& state, double dt)
{
    // control surfaces
    if(glfwGetKey(window, GLFW_KEY_A)) state.aileron += state.keyboardControlRate  * dt;
    if(glfwGetKey(window, GLFW_KEY_D)) state.aileron -= state.keyboardControlRate  * dt ;
    clampSignedNormal(state.aileron);

    if(glfwGetKey(window, GLFW_KEY_W)) state.elevator -= state.keyboardControlRate  * dt ;
    if(glfwGetKey(window, GLFW_KEY_S)) state.elevator += state.keyboardControlRate  * dt ;
    clampSignedNormal(state.elevator);

    if(glfwGetKey(window, GLFW_KEY_Q)) state.rudder -= state.keyboardControlRate  * dt ;
    if(glfwGetKey(window, GLFW_KEY_E)) state.rudder += state.keyboardControlRate  * dt ;
    clampSignedNormal(state.rudder);

    // engine
    if(glfwGetKey(window, GLFW_KEY_T)) {
        for(auto& engine : state.engines) {
            engine.throttle += state.keyboardControlRate  * dt ;
            clampUnsignedNormal(engine.throttle);
        }
    }
    if(glfwGetKey(window, GLFW_KEY_G)) {
        for(auto& engine : state.engines) {
            engine.throttle -= state.keyboardControlRate  * dt ;
            clampUnsignedNormal(engine.throttle);
        }
    }
}