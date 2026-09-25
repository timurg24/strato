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
    double payloadWeightLbs;
    std::vector<double> fuelLbs; // in lbd

    std::string registration;

    double latitude, longitude, altitude;

    double airspeed;

    void setDefaultValues();
};

class Aircraft {
public:
    Aircraft() = default;
    void init(const std::filesystem::path& aircraftFolder, const std::string& aircraftFileName, const StartupState& state, Wrangler::Renderer& renderer, Wrangler::AssetManager& assets);
    void readInputs();
    void update(double dt);
    
    Wrangler::RenderableEntity entity;
    
    std::string registration;
private:

    // =================
    //      JSBSim
    // =================

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

    AircraftState                               aircraftState;
    ControlState                                controlState;
    EnvironmentState                            envState;

    // TODO: Follow JSB sim example and add hooks

    // =================
    //   Initializers
    // =================
    bool initJSBSystems();
    bool loadFlightModels(const std::filesystem::path& aircraftFolder, const std::string& aircraftFileName);
    bool setupStartupState(const StartupState& state);

};