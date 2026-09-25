#pragma once

/**
 * State
 * Has different states (not the USA kind)
 */

// core
#include "Type/Types.hpp"

// What the aircraft is doing now
// No user inputs go into here
struct AircraftState {
    bool            stallWarning            = false;
    bool            crashed                 = false;

    // Control surfaces
    SignedNormal    elevatorPos             = 0.0f;
    SignedNormal    leftAileronPos          = 0.0f;
    SignedNormal    rightAileronPos         = 0.0f;
    SignedNormal    rudderPos               = 0.0f;
    UnsignedNormal  flapPos                 = 0.0f;
    UnsignedNormal  speedBreakPos           = 0.0f;

    // Brakes
    bool            autoBreakEngaged        = false;
    UnsignedNormal  autoBreakLeftPower      = 0.0f;
    UnsignedNormal  autoBreakRightPower     = 0.0f;

    // Gear (and other things)
    UnsignedNormal  gearPos                 = 1.0f;
    UnsignedNormal  wingFoldPos             = 0.0f;
    UnsignedNormal  tailHookPos             = 0.0f;

    double          altitude;
    double          latitude;
    double          longitude;
    double          airspeed;
};

// What is being commanded
// User inputs go here
struct ControlState {
    // Trim
    bool            enableStartupTrim; // automatically selects the best trim at startup
    bool            trimmed;
    SignedNormal    pitchTrim;
    
    UnsignedNormal  throttle;
    SignedNormal  aileron;
    SignedNormal  rudder;
};

// What affects the aircraft
struct EnvironmentState {
    SignedNormal    temperature;
    SignedNormal    pressure;
    SignedNormal    pressureSL;
    SignedNormal    groundWind; 
    SignedNormal    turbelanceGain; 
    SignedNormal    turbelanceRate; 
    SignedNormal    turbelanceModel;
    SignedNormal    southWind; 
    SignedNormal    eastWind; 
    SignedNormal    bottomWind; 

    /*
        TODO:
        - Turbulance
        - Weather
    */
};

// System states
struct SystemState {

};