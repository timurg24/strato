#pragma once

/**
 * State
 * Has different states (not the USA kind)
 */

// core
#include "Type/Types.hpp"

// std
#include <array>

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

// Engine controls
struct EngineState {

    bool            starter;
    bool            running;

    UnsignedNormal  throttle;
    UnsignedNormal  mixture;
    UnsignedNormal  propAdvance;
    UnsignedNormal  feather;

    // piston
    UnsignedNormal  magnetos;

    // turbine
    UnsignedNormal  augmentation;
    UnsignedNormal  ignition;

    // turboprop and turbine
    UnsignedNormal  reverser;
    UnsignedNormal  cutOff;

    // rocket
    // the example doesnt do rockets...

    // turboprop
    UnsignedNormal  generatorPower;
    UnsignedNormal  condition;

};


// What is being commanded
// User inputs go here
struct ControlState {
    // Trim
    bool            enableStartupTrim; // automatically selects the best trim at startup
    bool            trimmed;
    SignedNormal    pitchTrim;

    SignedNormal    aileron;
    SignedNormal    rudder;
    SignedNormal    elevator;

    UnsignedNormal  speedBrake;
    UnsignedNormal  spoiler;

    bool            parkingBrake;
    UnsignedNormal  leftBrake;
    UnsignedNormal  rightBrake;

    bool            gear; // true is up

    int engineCount;
    std::array<EngineState, 10> engines;
};

// What affects the aircraft
struct EnvironmentState {
    double    temperature;
    double    pressure;
    double    pressureSL;
    double    groundWind; 
    double    turbelanceGain; 
    double    turbelanceRate; 
    double    turbelanceModel;

    double    windNorth; 
    double    windEast; 
    double    windDown; 

    /*
        TODO:
        - Turbulence
        - Weather
    */
};

// System states
struct SystemState {

};