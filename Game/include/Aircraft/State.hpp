#pragma once

/**
 * State
 * Has different states that are used with JSBSim
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

    double trueAirspeedKts       = 0.0; // knots
    double equivalentAirspeedKts = 0.0; // knots
    double calibratedAirspeedKts = 0.0; // knots
    double groundSpeedFps        = 0.0; // feet/second

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

enum class MagnetoState : int {
    Off     = 1,
    Left,
    Right,
    Both
};

// Engine controls
struct EngineState {

    bool            starter         = false;
    bool            running         = false;

    UnsignedNormal  throttle        = 0.0;
    UnsignedNormal  mixture         = 0.0;
    UnsignedNormal  propAdvance     = 0.0;
    UnsignedNormal  feather         = 0.0;

    // piston
    MagnetoState  magnetos          = MagnetoState::Off;

    // turbine
    UnsignedNormal  augmentation    = 0.0;
    UnsignedNormal  ignition        = 0.0;

    // turboprop and turbine
    UnsignedNormal  reverser        = 0.0;
    UnsignedNormal  cutOff          = 0.0;

    // rocket
    // the example doesnt do rockets...

    // turboprop
    UnsignedNormal  generatorPower  = 0.0;
    UnsignedNormal  condition       = 0.0;

};


// What is being commanded
// User inputs go here
struct ControlState {

    UnsignedNormal  keyboardControlRate     = 1.1;

    // Trim
    bool            enableStartupTrim       = false; // automatically selects the best trim at startup
    bool            trimmed                 = false;
    SignedNormal    pitchTrim               = 0.0;

    SignedNormal    aileron                 = 0.0;
    SignedNormal    rudder                  = 0.0;
    SignedNormal    elevator                = 0.0;

    UnsignedNormal  speedBrake              = 0.0;
    UnsignedNormal  spoiler                 = 0.0;

    bool            parkingBrake            = false;
    UnsignedNormal  leftBrake               = 0.0;
    UnsignedNormal  rightBrake              = 0.0;

    bool            gearUp                  = false; // true is up

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