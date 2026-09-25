#include "Aircraft/Aircraft.hpp"

// JSBSim
#include <FGJSBBase.h>

#include <initialization/FGInitialCondition.h>
#include <initialization/FGTrim.h>

#include <models/FGModel.h>
#include <models/FGAircraft.h>
#include <models/FGFCS.h>
#include <models/FGPropagate.h>
#include <models/FGAuxiliary.h>
#include <models/FGInertial.h>
#include <models/FGAtmosphere.h>
#include <models/FGMassBalance.h>
#include <models/FGAerodynamics.h>
#include <models/FGLGear.h>
#include <models/FGGroundReactions.h>
#include <models/FGPropulsion.h>
#include <models/FGAccelerations.h>

#include <models/atmosphere/FGWinds.h>

#include <models/propulsion/FGEngine.h>
#include <models/propulsion/FGPiston.h>
#include <models/propulsion/FGTurbine.h>
#include <models/propulsion/FGTurboProp.h>
#include <models/propulsion/FGRocket.h>
#include <models/propulsion/FGElectric.h>
#include <models/propulsion/FGNozzle.h>
#include <models/propulsion/FGPropeller.h>
#include <models/propulsion/FGRotor.h>
#include <models/propulsion/FGTank.h>

// TUL
#include <tul/ErrorOps.hpp>
#include <tul/StringOps.hpp>

/// @brief Sets some default values
void StartupState::setDefaultValues()
{
    // 31.719943321617503, -110.07055444789162
    payloadWeightLbs    = 170.0;

    fuelLbs             = {80.0, 80.0};

    registration        = "N881OK";

    latitude            = 31.719943;
    longitude           = -110.070554;
    altitude            = 5000.0;

    airspeed            = 125.0;
}

/// @brief Initializes the aircraft and JSBSim systems
void Aircraft::init(const std::filesystem::path& aircraftFolder, const std::string& aircraftFileName, const StartupState& state, Wrangler::Renderer& renderer, Wrangler::AssetManager& assets) {
    initJSBSystems();
    loadFlightModels(aircraftFolder, aircraftFileName);
    if(!setupStartupState(state)) tul::FatalError({"Failed to set a startup state for aircraft"});
    
    tul::Print({"[Liberty] [Aircraft] Loaded aircraft: ", aircraftFileName, "\n"});
    int engineCount = propulsion->GetNumEngines();
    tul::Print({"\t- Engines: ", std::to_string(engineCount), "\n"});

    tul::Print({"\t- Registration: ", registration, "\n"});
    tul::Print({"\t- Position: ", std::to_string(state.latitude), ", ", std::to_string(state.longitude), "\n"});
    tul::Print({"\t- Altitude: ", std::to_string(state.altitude), "\n"});

    // pitch trim
    controlState.pitchTrim = fcs->GetPitchTrimCmd(); // TODO: Review what changes this makes

    // load 3d
    Wrangler::AssetID model = assets.loadModel("models/Cessna172.fbx");
    Wrangler::AssetID material = assets.loadMaterial("payloads/materials/cessna.pay", renderer.pbrShader);

    // setup 3d
    entity = {
        .model = model,
        .material = material,
        .position = {1.0f, -200.0f, 500.0f}
    };   
}

/// @brief Reads the flight control inputs and sends them to JBSSim
void Aircraft::readInputs()
{
}

/// @brief Initializes the pointers for the subsystes
/// @return True on success
bool Aircraft::initJSBSystems()
{
    fdm = std::make_unique<JSBSim::FGFDMExec>();

    // simulation rate
    constexpr double dt = 1.0 / 120.0;
    fdm->Setdt(dt);

    atmosphere      = fdm->GetAtmosphere();
    winds           = fdm->GetWinds();
    fcs             = fdm->GetFCS();
    massBalance     = fdm->GetMassBalance();
    propulsion      = fdm->GetPropulsion();
    aircraft        = fdm->GetAircraft();
    propagate       = fdm->GetPropagate();
    aux             = fdm->GetAuxiliary();
    inertial        = fdm->GetInertial();
    aerodynamics    = fdm->GetAerodynamics();
    groundReactions = fdm->GetGroundReactions();
    accelerations   = fdm->GetAccelerations();

    return true;
}

/// @brief Loads the required flight model files
/// @return True on success
bool Aircraft::loadFlightModels(const std::filesystem::path& aircraftFolder, const std::string& aircraftFileName)
{
    std::filesystem::path aircraftRootFolder = aircraftFolder.filename();
    fdm->SetRootDir(SGPath(aircraftFolder.parent_path()));
    fdm->SetAircraftPath(SGPath(aircraftRootFolder));
    fdm->SetEnginePath(SGPath(aircraftRootFolder / "engine"));
    fdm->SetSystemsPath(SGPath(aircraftRootFolder / "systems"));

    if(!fdm->LoadModel(aircraftFileName, false))
        tul::FatalError({
                        "Failed to load aircraft!\nAircraft Folder: ", 
                        aircraftFolder.string(), 
                        "\nFile Name: ", 
                        aircraftFileName,
                        "\nCheck console for details"});

    return true;
}

/// @brief Sets up the aircraft startup state
/// @return True on success
bool Aircraft::setupStartupState(const StartupState& state)
{

    // SET THE STATE
    aircraftState.latitude  = state.latitude;
    aircraftState.longitude = state.longitude;
    aircraftState.altitude  = state.altitude;
    aircraftState.airspeed  = state.airspeed;
    registration            = state.registration;

    // FUEL THE AIRCRAFT
    int fuelTankCount = propulsion->GetNumTanks();
    if(fuelTankCount != state.fuelLbs.size())
        tul::Alert({
        "Fuel tank count mismatch in startup state and aircraft XML",
        "\nXML count: ", std::to_string(fuelTankCount),
        "\nStartup count: ", std::to_string(state.fuelLbs.size())});
        
    for(unsigned int i = 0; i < propulsion->GetNumTanks(); i++) {
        if(i >= state.fuelLbs.size()) break;
        propulsion->GetTank(i)->SetContents(state.fuelLbs[i]);
    }

    // SET JSBSIM STARTUP CONDITIONS
    startupConditions = fdm->GetIC();

    startupConditions->SetLatitudeDegIC(
        aircraftState.latitude
    );

    startupConditions->SetLongitudeDegIC(
        aircraftState.longitude
    );

    startupConditions->SetAltitudeASLFtIC(
        aircraftState.altitude
    );

    startupConditions->SetVtrueKtsIC(
        aircraftState.airspeed
    );

    return fdm->RunIC();
}

// X is left right
// Y is up down
// Z is forward backward