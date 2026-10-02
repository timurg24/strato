#pragma once

/**
 * State
 * Has different states (not the USA kind)
 */

// wrangler
#include <Wrangler/Core/Types.hpp>

// core
#include "Type/Types.hpp"

// std
#include <array>

// limits
#define STRATO_MAX_ENGINES 10

struct FlightVec3 {
    double x,y,z;
};
// What the aircraft is doing now
// No user inputs go into here
struct AircraftState {
    // =========================
    // General
    // =========================

    bool crashed = false;

    // JSBSim stall warning is not necessarily just on/off.
    double stallWarning = 0.0;

    // Normal load factor (G)
    double loadFactor = 1.0;


    // =========================
    // Position
    // =========================

    double latitude  = 0.0;
    double longitude = 0.0;

    double altitudeASL = 0.0;  // ft above sea level
    double altitudeAGL = 0.0;  // ft above ground

    double runwayAltitude = 0.0;

    // Visual reference point position
    FlightVec3 vrpPosition{};


    // =========================
    // Attitude
    // =========================

    double roll    = 0.0;  // rad
    double pitch   = 0.0;  // rad
    double heading = 0.0;  // rad

    // Euler angle rates
    FlightVec3 eulerAngles{};
    FlightVec3 eulerRates{};


    // =========================
    // Aerodynamics
    // =========================

    double angleOfAttack = 0.0; // alpha, rad
    double sideslip      = 0.0; // beta, rad

    double flightPathAngle = 0.0; // gamma, rad

    double mach = 0.0;


    // =========================
    // Airspeeds
    // =========================

    double trueAirspeed       = 0.0;
    double equivalentAirspeed = 0.0; // knots
    double calibratedAirspeed = 0.0; // knots
    double groundSpeed        = 0.0;

    double climbRate = 0.0;

    double groundTrack = 0.0;


    // =========================
    // Velocities
    // =========================

    // North / East / Down
    FlightVec3 velocityLocal{};

    // U / V / W aircraft body axes
    FlightVec3 velocityBody{};

    FlightVec3 angularVelocityBody{};


    // =========================
    // Accelerations / CG
    // =========================

    FlightVec3 cgPosition{};

    FlightVec3 accelsBody{};

    // Normalized acceleration/load at CG
    FlightVec3 accelsCgBodyN{};

    // Acceleration at pilot location
    FlightVec3 accelsPilotBody{};


    // =========================
    // Control surface positions
    // =========================

    SignedNormal elevatorPos     = 0.0;
    SignedNormal leftAileronPos  = 0.0;
    SignedNormal rightAileronPos = 0.0;
    SignedNormal rudderPos       = 0.0;

    UnsignedNormal flapPos       = 0.0;
    UnsignedNormal speedBrakePos = 0.0;
    UnsignedNormal spoilerPos    = 0.0;

    UnsignedNormal gearPos       = 1.0;
    UnsignedNormal wingFoldPos   = 0.0;
    UnsignedNormal tailHookPos   = 0.0;


    // =========================
    // Coordinate / Earth state
    // =========================

    double earthPositionAngle = 0.0;

    // Local -> body transformation matrix.
    // Replace with Wrangler::Mat3 if you already have one.
    double localToBody[3][3]{};
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
    std::array<EngineState, STRATO_MAX_ENGINES> engines;
};

// What affects the aircraft
struct EnvironmentState {
    double    temperature           = 15.0; // c
    double    pressure;
    double    pressureSL            = 29.92; // inHg
    double    groundWind; 
    double    turbelanceGain; 
    double    turbelanceRate; 
    double    turbelanceModel;

    double    windNorth             = 0.0; 
    double    windEast              = 0.0; 
    double    windDown              = 0.0; 

    /*
        TODO:
        - Turbulence
        - Weather
    */
};

// System states
struct SystemState {

};