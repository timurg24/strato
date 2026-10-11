#pragma once

/**
 * Input class
 * Converts any allowed user input into the ControlState struct
 */

#include "Aircraft/State.hpp"

struct GLFWwindow;

namespace Strato {
    class Input {
    private:
        void clampSignedNormal(SignedNormal& value);
        void clampUnsignedNormal(UnsignedNormal& value);
    public:
        void keyboardInput(GLFWwindow* window, ControlState& state, double dt);
    };
}

