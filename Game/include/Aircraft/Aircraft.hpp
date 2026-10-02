#pragma once

/**
 * Aircraft
 * Aircraft simulation
 */


// std
#include <memory>
#include <filesystem>

// JSBSim
#include <FGFDMExec.h>

// Core
#include "Aircraft/State.hpp"

// Wrangler
#include <Wrangler/AssetManager/AssetManager.hpp>
#include <Wrangler/Renderer/Renderer.hpp>

namespace JSBSim {
    class FGAtmosphere;
    class FGWinds;
    class FGFCS;
    class FGPropulsion;
    class FGMassBalance;
    class FGAerodynamics;
    class FGInertial;
    class FGAircraft;
    class FGPropagate;
    class FGAuxiliary;
    class FGOutput;
    class FGInitialCondition;
    class FGLocation;
    class FGAccelerations;
    class FGPropertyManager;
}

struct StartupState {
    double payloadWeightLbs                 = 170.0;
    std::vector<double> fuelLbs             = {80.0, 80.0};

    std::string registration                = "N881OK";

    double latitude                         = 31.719943;
    double longitude                        = -110.070554;
    double altitude                         = 200.0;

    double airspeed                         = 120.0;
};

class Aircraft {
public:
    Aircraft() = default;
    void init(const std::filesystem::path& aircraftFolder, const std::string& aircraftFileName, const StartupState& state, Wrangler::Renderer& renderer, Wrangler::AssetManager& assets);
    
    // public update
    void setPhysicsRate(double hz);
    void update();
    double getAltitude();

    Wrangler::RenderableEntity entity;
    
    std::string registration;

    AircraftState                               aircraftState;
    ControlState                                controlState;
    EnvironmentState                            envState;

    // jsbsim
    std::unique_ptr<JSBSim::FGFDMExec>          fdm; // jsb sims instance
    std::shared_ptr<JSBSim::FGInitialCondition> startupConditions; // startup conditions, duh
    bool needTrim;

    std::shared_ptr<JSBSim::FGAtmosphere>       atmosphere;
    std::shared_ptr<JSBSim::FGWinds>            winds;
    std::shared_ptr<JSBSim::FGFCS>              fcs;
    std::shared_ptr<JSBSim::FGPropulsion>       propulsion;
    std::shared_ptr<JSBSim::FGMassBalance>      massBalance;
    std::shared_ptr<JSBSim::FGAircraft>         aircraft;
    std::shared_ptr<JSBSim::FGPropagate>        propagate;
    std::shared_ptr<JSBSim::FGAuxiliary>        aux;
    std::shared_ptr<JSBSim::FGAerodynamics>     aerodynamics;
    std::shared_ptr<JSBSim::FGGroundReactions>  groundReactions;
    std::shared_ptr<JSBSim::FGInertial>         inertial;
    std::shared_ptr<JSBSim::FGAccelerations>    accelerations;

    // TODO: Make env state a reference to some other environment object

    // data
    double physicsHz    = 120.0;
    double physicsDt    = 1.0 / physicsHz;
private:

    // initializers
    bool initJSBSystems();
    bool loadFlightModels(const std::filesystem::path& aircraftFolder, const std::string& aircraftFileName);
    bool setupStartupState(const StartupState& state);

    // update
    void coypToJSBSim();
    void readFromJSBSim();

    void update3DRotation();

};