
#ifdef OPENSTEER
/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com) */
#ifndef OPENSTEER_OPENSTEERDEMO_H
#define OPENSTEER_OPENSTEERDEMO_H



//!!! remove this if you don't use my IrrExtensions !!!
//#define useIrrExtensions13



#include "IrrOpenSteer.h"

struct datIrrPointers{
  irr::IrrlichtDevice* Device;
  irr::video::IVideoDriver* Driver;
  irr::scene::ISceneManager* SMgr;
  irr::gui::IGUIEnvironment* GUIEnv;
  irr::scene::ICameraSceneNode* Camera;
};

namespace OpenSteer{

class Color;
class Vec3;

class OpenSteerDemo{

  public:
    // currently selected plug-in and vehicle.
    static PlugIn* selectedPlugIn;
    static AbstractVehicle* selectedVehicle;
    // clock keeps track of both "real time" and "simulation time"
    static Clock clock;

    // -------------------------------------------- initialize, update and events
    // initialize OpenSteerDemo
    static void initialize();
    // main update function: step simulation forward and redraw scene
    static void updateSimulationAndRedraw();

    static void setInfoText(irr::gui::IGUIStaticText* st);

    // ------------------------------------------------------- PlugIn interface
    // select the default PlugIn
    static void selectDefaultPlugIn();
    // select the "next" plug-in, cycling through "plug-in selection order"
    static void selectNextPlugIn();
    // handle function keys an a per-plug-in basis
    static void functionKeyForPlugIn(int keyNumber);
    // return name of currently selected plug-in
    static const char* nameOfSelectedPlugIn();
    // open the currently selected plug-in
    static void openSelectedPlugIn();
    // do a simulation update for the currently selected plug-in
    static void updateSelectedPlugIn(const float currentTime, const float elapsedTime);
    // redraw graphics for the currently selected plug-in
    static void redrawSelectedPlugIn(const float currentTime, const float elapsedTime);
    // close the currently selected plug-in
    static void closeSelectedPlugIn();
    // reset the currently selected plug-in
    static void resetSelectedPlugIn();
    static const AVGroup& allVehiclesOfSelectedPlugIn();

    // ---------------------------------------------------- OpenSteerDemo phase
    static bool phaseIsDraw    () {return phase == drawPhase;}
    static bool phaseIsUpdate  () {return phase == updatePhase;}
    static bool phaseIsOverhead() {return phase == overheadPhase;}
    static float phaseTimerDraw    () {return phaseTimers[drawPhase];}
    static float phaseTimerUpdate  () {return phaseTimers[updatePhase];}
    // XXX get around shortcomings in current implementation, see note
    // XXX in updateSimulationAndRedraw
    //static float phaseTimerOverhead(){return phaseTimers[overheadPhase];}
    static float phaseTimerOverhead(){
        return(clock.getElapsedRealTime() -
               (phaseTimerDraw() + phaseTimerUpdate()));
    }

    // ------------------------------------------------------ delayed reset XXX
    // XXX to be reconsidered
    static void queueDelayedResetPlugInXXX();
    static void doDelayedResetPlugInXXX();

    // ------------------------------------------------------ vehicle selection
    // select the "next" vehicle: cycle through the registry
    static void selectNextVehicle();
    // select vehicle nearest the given screen position(e.g.: of the mouse)
    static void selectVehicleNearestScreenPosition(int x, int y);

    // ---------------------------------------------------------- mouse support
    // Find the AbstractVehicle whose screen position is nearest the
    // current the mouse position.  Returns NULL if mouse is outside
    // this window or if there are no AbstractVehicles.
    static AbstractVehicle* vehicleNearestToMouse();
    // Find the AbstractVehicle whose screen position is nearest the
    // given window coordinates, typically the mouse position.  Note
    // this will return NULL if there are no AbstractVehicles.
    static AbstractVehicle* findVehicleNearestScreenPosition(int x, int y);

    // cycle through frame rate presets
    static void selectNextPresetFrameRate();

    // ----------------------------------------------------------- console text
    // print a line on the console with "OpenSteerDemo: " then the given ending
    static void printMessage(const char* message);
    // like printMessage but prefix is "OpenSteerDemo: Warning: "
    static void printWarning(const char* message);
    // exit OpenSteerDemo with a given text message or error code
    static void errorExit(const char* message);
    static void exit(int exitCode);
    // print key commands
    static void keyboardMiniHelp();

  private:
    static int phase;
    static int phaseStack[];
    static int phaseStackIndex;
    static float phaseTimers[];
    static float phaseTimerBase;
    static const int phaseStackSize;
    static void pushPhase(const int newPhase);
    static void popPhase();
    static void initPhaseTimers();
    static void updatePhaseTimers();
    static const int drawPhase;
    static const int updatePhase;
    static const int overheadPhase;
};

// ----------------------------------------------------------------------------
// run graphics event loop
void initializeGraphics(IrrlichtDevice *device); //irr::IrrlichtDevice* Device
void runGraphics();
// accessors for window dimensions
float getWindowHeight();
float getWindowWidth();

} // namespace OpenSteer

#endif // OPENSTEER_OPENSTEERDEMO_H
#endif
