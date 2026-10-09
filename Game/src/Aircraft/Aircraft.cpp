#include "Aircraft/Aircraft.hpp"

// debug hud
#include <numbers>

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
    controlState.engineCount = propulsion->GetNumEngines();
    if(controlState.engineCount > STRATO_MAX_ENGINES) {
        tul::Alert({"Engine count (", std::to_string(controlState.engineCount), ") exceeds max limit of ", std::to_string(STRATO_MAX_ENGINES), " engines"});
        controlState.engineCount = STRATO_MAX_ENGINES;
    }

    // load 3d
    Wrangler::AssetID model = assets.loadModel((aircraftFolder / "assets" / "Models" / "Cessna172.fbx").generic_string());
}

/// @brief Copies user inputs (ControlState) to JSBSim
void Aircraft::coypToJSBSim()
{
    // control surfaces
    fcs->SetDaCmd(controlState.aileron); // set aileron
    // skip roll trim
    fcs->SetDeCmd(controlState.elevator); // set elevator
    fcs->SetPitchTrimCmd(controlState.pitchTrim);
    fcs->SetDrCmd(controlState.rudder); // set rudder
    fcs->SetDsCmd(controlState.rudder); // set rudder
    fcs->SetDsbCmd(controlState.speedBrake); // set speedbrake
    fcs->SetDspCmd(controlState.spoiler); // set spoiler

    // brakes
    UnsignedNormal leftBrake = controlState.leftBrake;
    UnsignedNormal rightBrake = controlState.rightBrake;
    if(controlState.parkingBrake) {
        leftBrake = 1.0f;
        rightBrake = 1.0f;
    }

    fcs->SetLBrake(leftBrake);
    fcs->SetRBrake(rightBrake);
    fcs->SetCBrake(0.0); // why

    // gear
    fcs->SetGearCmd(controlState.gearUp ? 0.0 : 1.0);

    // engine
    for(int i = 0; i < controlState.engineCount; i++) {
        const auto& engine = controlState.engines[i];
        fcs->SetThrottleCmd(i, engine.throttle);
        fcs->SetMixtureCmd(i, engine.mixture);
        fcs->SetPropAdvanceCmd(i, engine.propAdvance);
        fcs->SetFeatherCmd(i, engine.feather);

        // set engine type specific values
        switch(propulsion->GetEngine(i)->GetType()) {
            case JSBSim::FGEngine::EngineType::etPiston:
            {
                // not even the creator of this type knows what this does
                auto eng =
                    std::dynamic_pointer_cast<JSBSim::FGPiston>(
                        propulsion->GetEngine(i)
                    );

                eng->SetMagnetos(
                    static_cast<int>(controlState.engines[i].magnetos)
                );
            }
            break;
            case JSBSim::FGEngine::EngineType::etTurbine:
            {
                // not even the creator of this type knows what this does
                auto eng =
                    std::dynamic_pointer_cast<JSBSim::FGTurbine>(
                        propulsion->GetEngine(i)
                    );

                eng->SetAugmentation(controlState.engines[i].augmentation);
                eng->SetReverse(controlState.engines[i].reverser);
                eng->SetCutoff(controlState.engines[i].cutOff);
                eng->SetIgnition(controlState.engines[i].ignition);
            }
            break;
            case JSBSim::FGEngine::EngineType::etTurboprop:
            {
                // not even the creator of this type knows what this does
                auto eng =
                    std::dynamic_pointer_cast<JSBSim::FGTurboProp>(
                        propulsion->GetEngine(i)
                    );

                eng->SetReverse(controlState.engines[i].reverser);
                eng->SetCutoff(controlState.engines[i].cutOff);
                eng->SetGeneratorPower(controlState.engines[i].generatorPower);
                eng->SetCondition(controlState.engines[i].condition);
            }
            break;
            default:
            break;
        }
        // set values for the engine
        auto eng = propulsion->GetEngine(i);
        eng->SetStarter(controlState.engines[i].starter);
        eng->SetRunning(controlState.engines[i].running);
    }

    // atmosphere
    atmosphere->SetTemperature(envState.temperature, getAltitude(), JSBSim::FGAtmosphere::eCelsius);
    atmosphere->SetPressureSL(JSBSim::FGAtmosphere::eInchesHg, envState.pressureSL);

    winds->SetWindNED(
        envState.windNorth,
        envState.windEast,
        envState.windDown
    );
}

/// @brief Reads AircraftState from JSBSim
void Aircraft::readFromJSBSim()
{
    // altitude
    aircraftState.altitudeASL = propagate->GetAltitudeASL();
    aircraftState.altitudeAGL = propagate->GetDistanceAGL();

    // euler
    aircraftState.eulerAngles = {
        propagate->GetEuler(JSBSim::FGJSBBase::ePhi),
        propagate->GetEuler(JSBSim::FGJSBBase::eTht),
        propagate->GetEuler(JSBSim::FGJSBBase::ePsi)
    };

    aircraftState.roll =
        aircraftState.eulerAngles.x;

    aircraftState.pitch =
        aircraftState.eulerAngles.y;

    aircraftState.heading =
        aircraftState.eulerAngles.z;

    aircraftState.eulerRates = {
        aux->GetEulerRates(JSBSim::FGJSBBase::ePhi),
        aux->GetEulerRates(JSBSim::FGJSBBase::eTht),
        aux->GetEulerRates(JSBSim::FGJSBBase::ePsi)
    };

    // velocity
    aircraftState.velocityLocal = {
        propagate->GetVel(JSBSim::FGJSBBase::eNorth),
        propagate->GetVel(JSBSim::FGJSBBase::eEast),
        propagate->GetVel(JSBSim::FGJSBBase::eDown)
    };

    aircraftState.velocityBody = {
        propagate->GetUVW(1),
        propagate->GetUVW(2),
        propagate->GetUVW(3)
    };

    aircraftState.angularVelocityBody = {
        propagate->GetPQR(JSBSim::FGJSBBase::eP),
        propagate->GetPQR(JSBSim::FGJSBBase::eQ),
        propagate->GetPQR(JSBSim::FGJSBBase::eR)
    };

    // air data
    aircraftState.trueAirspeedKts = aux->GetVtrueKTS();
    aircraftState.equivalentAirspeedKts = aux->GetVequivalentKTS();
    aircraftState.calibratedAirspeedKts = aux->GetVcalibratedKTS();
    aircraftState.groundSpeedFps = aux->GetVground();
    aircraftState.mach = aux->GetMach();

    aircraftState.angleOfAttack = aux->Getalpha();
    aircraftState.sideslip = aux->Getbeta();
    aircraftState.flightPathAngle = aux->GetGamma();

    aircraftState.loadFactor = aux->GetNlf();

    // accelerations
    aircraftState.accelsBody = {
        accelerations->GetBodyAccel(1),
        accelerations->GetBodyAccel(2),
        accelerations->GetBodyAccel(3)
    };

    aircraftState.accelsCgBodyN = {
        aux->GetNcg(1),
        aux->GetNcg(2),
        aux->GetNcg(3)
    };

    aircraftState.accelsPilotBody = {
        aux->GetPilotAccel(1),
        aux->GetPilotAccel(2),
        aux->GetPilotAccel(3)
    };

    // cg
    aircraftState.cgPosition = {
        massBalance->GetXYZcg(1),
        massBalance->GetXYZcg(2),
        massBalance->GetXYZcg(3)
    };

    // position
    const auto& location = propagate->GetLocation();

    aircraftState.latitude = location.GetGeodLatitudeDeg();
    aircraftState.longitude = location.GetLongitudeDeg();

    aircraftState.altitudeASL = propagate->GetAltitudeASL();
    aircraftState.altitudeAGL = propagate->GetDistanceAGL();

    aircraftState.climbRate = propagate->Gethdot();
    aircraftState.groundTrack = aux->GetGroundTrack();
}

/// @brief Sets the RenderEntity rotation to that of data from JSBSim
void Aircraft::update3DRotation()
{
    entity.rotation = {
        static_cast<float>(-aircraftState.pitch), // pitch / theta
        static_cast<float>( aircraftState.heading), // heading / psi
        static_cast<float>( aircraftState.roll)  // roll / phi
    };
}

void Aircraft::setPhysicsRate(double hz)
{
    physicsHz = hz;
    physicsDt = 1.0 / hz;

    fdm->Setdt(physicsDt);
}

void Aircraft::update()
{
    coypToJSBSim();
    fdm->Run();
    readFromJSBSim();

    update3DRotation();
}

/// @brief Returns the altitude
/// @return 
double Aircraft::getAltitude()
{
    return aircraftState.altitudeASL;
}

/// @brief Displays some information using the BGFX debug text function
void Aircraft::drawDebugHUD() const
{
    
}

/// @brief Initializes the pointers for the subsystes
/// @return True on success
bool Aircraft::initJSBSystems()
{
    fdm = std::make_unique<JSBSim::FGFDMExec>();

    // simulation rate
    fdm->Setdt(physicsDt);

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
bool Aircraft::loadFlightModels(
    const std::filesystem::path& aircraftFolder,
    const std::string& aircraftFileName)
{
    const auto folder =
        std::filesystem::absolute(
            std::filesystem::path("Content") / aircraftFolder
        ).lexically_normal();

    fdm->SetAircraftPath(SGPath(folder));
    fdm->SetEnginePath(SGPath(folder / "engine"));
    fdm->SetSystemsPath(SGPath(folder / "systems"));

    if (!fdm->LoadModel(aircraftFileName, false))
    {
        tul::FatalError({
            "Failed to load aircraft!\n",
            "Aircraft Folder: ", folder.string(),
            "\nFile Name: ", aircraftFileName,
            "\nCheck console for details"
        });
    }

    return true;
}

/// @brief Sets up the aircraft startup state
/// @return True on success
bool Aircraft::setupStartupState(const StartupState& state)
{

    // SET THE STATE
    aircraftState.latitude  = state.latitude;
    aircraftState.longitude = state.longitude;
    aircraftState.altitudeASL  = state.altitude;
    aircraftState.trueAirspeedKts  = state.airspeed;
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
        aircraftState.altitudeASL
    );

    startupConditions->SetVtrueKtsIC(
        aircraftState.trueAirspeedKts
    );

    return fdm->RunIC();
}

// X is left right
// Y is up down
// Z is forward backward