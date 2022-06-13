
#ifdef OPENSTEER
/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com) */

#include "cOpenSteerDemo.h"
#include "cMeshTool.h"

namespace{ char* appVersionName = "IrrOpenSteer - Demo v1.0.0 by A.Buschhueter\n(http://abusoft.g0dsoft.com)"; }
irr::gui::IGUIStaticText* Text1;
irr::gui::IGUIStaticText* Text2;
irr::gui::IGUIStaticText* Text3;

// global Irrlicht pointers
datIrrPointers irrPointers;
// Array for drawing obstacles
irr::core::array<datMesh> lstObstacleMesh;

// clock keeps track of both "real time" and "simulation time"
OpenSteer::Clock OpenSteer::OpenSteerDemo::clock;

// currently selected plug-in (user can choose or cycle through them)
OpenSteer::PlugIn* OpenSteer::OpenSteerDemo::selectedPlugIn = NULL;
// currently selected vehicle.  Generally the one the camera follows
OpenSteer::AbstractVehicle* OpenSteer::OpenSteerDemo::selectedVehicle = NULL;

// phase: identifies current phase of the per-frame update cycle
int OpenSteer::OpenSteerDemo::phase = OpenSteer::OpenSteerDemo::overheadPhase;
// graphical annotation: master on/off switch
bool OpenSteer::enableAnnotation = true;

// event receiver
class cUserInput : public irr::IEventReceiver::IEventReceiver{
  public:
    virtual bool OnEvent(const irr::SEvent& event){
      static bool O_AlreadyPressed = false;
      if(event.EventType == irr::EET_KEY_INPUT_EVENT){
        if(!event.KeyInput.PressedDown){
          switch(event.KeyInput.Key){
            case irr::KEY_TAB:{ // select next PlugIn
              OpenSteer::OpenSteerDemo::selectNextPlugIn();
              std::cout << "select next PlugIn: "
                        << '"' << OpenSteer::OpenSteerDemo::nameOfSelectedPlugIn () << '"'
                        << std::endl << std::flush;
            }break;
            case irr::KEY_ESCAPE:{ // exit application with normal status
              irrPointers.Device->closeDevice();
              irrPointers.Device->run();
              irrPointers.Device->drop();
              irrPointers.Device = 0;
              OpenSteer::OpenSteerDemo::printMessage("exit.");
              OpenSteer::OpenSteerDemo::exit(0);
            }break;
            case irr::KEY_KEY_R:{ // reset selected PlugIn
              OpenSteer::OpenSteerDemo::resetSelectedPlugIn();
              std::cout << "reset PlugIn "
                        << '"' << OpenSteer::OpenSteerDemo::nameOfSelectedPlugIn () << '"'
                        << std::endl << std::flush;
            }break;
            case irr::KEY_KEY_N:{ // cycle selection to next vehicle
              std::cout << "select next vehicle/agent"
                        << std::endl << std::flush;
              OpenSteer::OpenSteerDemo::selectNextVehicle();
            }break;
            case irr::KEY_SPACE:{ // toggle run/pause state
              OpenSteer::OpenSteerDemo::
                printMessage(OpenSteer::OpenSteerDemo::clock.togglePausedState() ? "pause" : "run");
              irr::core::stringw lbl(OpenSteer::OpenSteerDemo::selectedPlugIn->name());
              if(OpenSteer::OpenSteerDemo::clock.getPausedState()) lbl += L" (pause)";
              irrPointers.Device->setWindowCaption(lbl.c_str());
            }break;
            case irr::KEY_KEY_F:{ // cycle through frame rate (clock mode) presets
              OpenSteer::OpenSteerDemo::selectNextPresetFrameRate();
              std::cout << "set clock to ";
              if(OpenSteer::OpenSteerDemo::clock.getAnimationMode()){
                std::cout << "animation mode, fixed frame rate ("
                          << OpenSteer::OpenSteerDemo::clock.getFixedFrameRate() << " fps)";
              }else{
                std::cout << "real-time mode, ";
                if(OpenSteer::OpenSteerDemo::clock.getVariableFrameRateMode())
                  std::cout << "variable frame rate";
                else
                  std::cout << "fixed frame rate ("
                            << OpenSteer::OpenSteerDemo::clock.getFixedFrameRate() << " fps)";
              }
              std::cout << std::endl << std::flush;
            }break;
            case irr::KEY_KEY_H:{ // print minimal help for single key commands
              OpenSteer::OpenSteerDemo::keyboardMiniHelp();
            }break;
            case irr::KEY_KEY_O:{
              O_AlreadyPressed = false;
            }break;

            case irr::KEY_F1: case irr::KEY_F2: case irr::KEY_F3: case irr::KEY_F4:
            case irr::KEY_F5: case irr::KEY_F6: case irr::KEY_F7: case irr::KEY_F8:
            case irr::KEY_F9: case irr::KEY_F10: case irr::KEY_F11: case irr::KEY_F12:{
              OpenSteer::OpenSteerDemo::functionKeyForPlugIn(event.KeyInput.Key - irr::KEY_F1 + 1);
            }break;

            case irr::KEY_KEY_C:{ // camera mode cycle
// TODO (Acki#5#): maybe implement later ???
  //            OpenSteer::OpenSteerDemo::camera.selectNextMode ();
  //            message << "select camera mode "
  //                    << '"' << OpenSteer::OpenSteerDemo::camera.modeName () << '"' << std::ends;
            }break;

          }
        }else{
          switch(event.KeyInput.Key){
            case irr::KEY_KEY_O:{ // print minimal help for single key commands
              OpenSteer::OpenSteerDemo::clock.setPausedState(true);
              if(!O_AlreadyPressed)
                std::cout << "single step forward (frame time: "
                          << OpenSteer::OpenSteerDemo::clock.advanceSimulationTimeOneFrame()
                          << ")" << std::endl << std::flush;
              else
                OpenSteer::OpenSteerDemo::clock.advanceSimulationTimeOneFrame();
              O_AlreadyPressed = true;
            }break;
          }
        }
      }

      else if(event.EventType == irr::EET_MOUSE_INPUT_EVENT){
        if(event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP){
          OpenSteer::OpenSteerDemo::selectVehicleNearestScreenPosition(event.MouseInput.X, event.MouseInput.Y);
          std::cout << "select vehicle/agent by mouse click"
                    << std::endl << std::flush;

        }
      }
      return false;
    }
};
cUserInput UserInput;

// callback for printing all plugin name
void printPlugInName(OpenSteer::PlugIn& pi){
  std::cout << " " << pi << std::endl;
}
// initialize OpenSteerDemo application
void OpenSteer::OpenSteerDemo::initialize(){
  // select the default PlugIn
  selectDefaultPlugIn();

  selectNextPresetFrameRate();

  std::cout << std::endl << appVersionName << "\n" << std::endl;
  OpenSteer::OpenSteerDemo::keyboardMiniHelp();

  std::cout << std::endl << "Known plugins:" << std::endl;
  PlugIn::applyToAll(printPlugInName);
  std::cout << std::endl;

  // identify default PlugIn
  if(!selectedPlugIn) errorExit("no default PlugIn");
  std::cout << "Default plugin:" << std::endl;
  std::cout << " " << *selectedPlugIn << std::endl;
  std::cout << std::endl;

  // initialize the default PlugIn
  openSelectedPlugIn();
}
// do all initialization related to graphics
void OpenSteer::initializeGraphics( IrrlichtDevice *device){
  // Irrlicht settings

//  irr::SIrrlichtCreationParameters irrParams;
//  irrParams.DriverType = irr::video::EDT_OPENGL;
//  irrParams.EventReceiver = &UserInput;
//  irrParams.Fullscreen = false;
//  irrParams.Bits = 16;
//  irrParams.Vsync = false;
//  irrParams.WindowSize = irr::core::dimension2du(800,600);
  // create Irrlicht
  irrPointers.Driver = 0; irrPointers.SMgr = 0; irrPointers.GUIEnv = 0;

  irrPointers.Device = device;

//  createDeviceEx(irrParams);

  if(irrPointers.Device){
    irrPointers.Device->getCursorControl()->setVisible(false);
    irrPointers.Driver = irrPointers.Device->getVideoDriver();
    irrPointers.SMgr = irrPointers.Device->getSceneManager();
    irrPointers.GUIEnv = irrPointers.Device->getGUIEnvironment();
    //irrPointers.Camera = irrPointers.SMgr->addCameraSceneNodeFPS(0,90,0.2);
    //    irrPointers.Camera =device->getSceneManager()->getActiveCamera();
//    irrPointers.Camera->setPosition(irr::core::vector3df(0,50,-40));
//    irrPointers.Camera->setTarget(irr::core::vector3df(0,0,0));
//    irrPointers.Camera->setInputReceiverEnabled(true);
    //device->setEventReceiver(&UserInput);
    Text1 = irrPointers.GUIEnv->addStaticText(L"", irr::core::recti(10,480,250,600), false, true);
    Text2 = irrPointers.GUIEnv->addStaticText(L"", irr::core::recti(600,500,800,600), false, true);
    Text3 = irrPointers.GUIEnv->addStaticText(L"", irr::core::recti(0,10,800,30), false, true);
#ifdef useIrrExtensions13
//    irr::gui::IGUIFont* f5 = irrPointers.GUIEnv->getFont("Arial", 8);
//    irr::gui::IGUIFont* f6 = irrPointers.GUIEnv->getFont("Arial", 12, true);
//    Text1->setOverrideFont(f5);
 //   Text2->setOverrideFont(f5);
//    Text3->setOverrideFont(f6);
#endif
   Text3->setTextAlignment (irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
    OpenSteerDemo::setInfoText(Text1);
  }
}

// select the default PlugIn
void OpenSteer::OpenSteerDemo::selectDefaultPlugIn(){
  PlugIn::sortBySelectionOrder();
  selectedPlugIn = PlugIn::findDefault();
}
// select the "next" plug-in, cycling through "plug-in selection order"
void OpenSteer::OpenSteerDemo::selectNextPlugIn(){
  closeSelectedPlugIn();
  selectedPlugIn = selectedPlugIn->next();
  openSelectedPlugIn();
}
// return name of currently selected plug-in
const char* OpenSteer::OpenSteerDemo::nameOfSelectedPlugIn(){
  return (selectedPlugIn ? selectedPlugIn->name() : "no PlugIn");
}
// open the currently selected plug-in
void OpenSteer::OpenSteerDemo::openSelectedPlugIn(){
  selectedVehicle = NULL;
  selectedPlugIn->open();
  irr::core::stringw lbl(selectedPlugIn->name());
  irrPointers.Device->setWindowCaption(lbl.c_str());
  Text3->setText(lbl.c_str());
}
// do a simulation update for the currently selected plug-in
void OpenSteer::OpenSteerDemo::updateSelectedPlugIn(const float currentTime, const float elapsedTime){
  // switch to Update phase
  pushPhase(updatePhase);

  // service queued reset request, if any
  doDelayedResetPlugInXXX();

  // if no vehicle is selected, and some exist, select the first one
  if(selectedVehicle == NULL){
    const AVGroup& vehicles = allVehiclesOfSelectedPlugIn();
    if(vehicles.size() > 0) selectedVehicle = vehicles.front();
  }

  // invoke selected PlugIn's Update method
  selectedPlugIn->update(currentTime, elapsedTime);

  // return to previous phase
  popPhase();
}
// reset the currently selected plug-in
void OpenSteer::OpenSteerDemo::resetSelectedPlugIn(){
  selectedPlugIn->reset();
}
// close the currently selected plug-in
void OpenSteer::OpenSteerDemo::closeSelectedPlugIn(){
  selectedPlugIn->close();
  selectedVehicle = NULL;
}

// handle function keys an a per-plug-in basis
void OpenSteer::OpenSteerDemo::functionKeyForPlugIn(int keyNumber){
  selectedPlugIn->handleFunctionKeys(keyNumber);
}

// Irrlicht main loop
void OpenSteer::runGraphics(){
  static irr::video::SMaterial Material;
  Material.Lighting = false;
	while(irrPointers.Device && irrPointers.Device->run()) if(irrPointers.Device->isWindowActive()){
    // update simulation
    OpenSteer::OpenSteerDemo::updateSimulationAndRedraw();
    // draw scene
    irrPointers.Driver->beginScene(true, true, irr::video::SColor(255, 190,190,250));
      irrPointers.SMgr->drawAll();
      irrPointers.GUIEnv->drawAll();
      // draw obstacles
      irrPointers.Driver->setMaterial(Material);
      for(irr::u32 t = 0; t < lstObstacleMesh.size(); ++t){
        if(lstObstacleMesh[t].Visible){
          irrPointers.Driver->setTransform(irr::video::ETS_WORLD, lstObstacleMesh[t].Transform);
          lstObstacleMesh[t].draw(irrPointers.Driver);
        }
      }
    irrPointers.Driver->endScene();

	}
	irrPointers.Device->drop();
}

// main update function: step simulation forward and redraw scene
void OpenSteer::OpenSteerDemo::updateSimulationAndRedraw(){
    // update global simulation clock
    clock.update ();

    //  start the phase timer (XXX to accurately measure "overhead" time this
    //  should be in displayFunc, or somehow account for time outside this
    //  routine)
    initPhaseTimers ();

    // run selected PlugIn (with simulation's current time and step size)
    updateSelectedPlugIn (clock.getTotalSimulationTime (),
                          clock.getElapsedSimulationTime ());

    // redraw selected PlugIn (based on real time)
    redrawSelectedPlugIn (clock.getTotalRealTime (),
                          clock.getElapsedRealTime ());
}
// redraw graphics for the currently selected plug-in
void OpenSteer::OpenSteerDemo::redrawSelectedPlugIn(const float currentTime, const float elapsedTime){
  // switch to Draw phase
  pushPhase(drawPhase);
  // invoke selected PlugIn's Draw method
  selectedPlugIn->redraw(currentTime, elapsedTime);
  // return to previous phase
  popPhase ();
}
// accessors for GLUT's window dimensions
float OpenSteer::getWindowHeight(){
  if(!irrPointers.Driver) return 0;
  return irrPointers.Driver->getScreenSize().Height;
}
float OpenSteer::getWindowWidth(){
  if(!irrPointers.Driver) return 0;
  return irrPointers.Driver->getScreenSize().Width;
}

// exit OpenSteerDemo with a given text message or error code
void OpenSteer::OpenSteerDemo::errorExit(const char* message){
  printMessage(message);
  exit(-1);
}
void OpenSteer::OpenSteerDemo::exit(int exitCode){
  ::exit(exitCode);
}

// return a group (an STL vector of AbstractVehicle pointers) of all
// vehicles(/agents/characters) defined by the currently selected PlugIn
const OpenSteer::AVGroup& OpenSteer::OpenSteerDemo::allVehiclesOfSelectedPlugIn(){
  return selectedPlugIn->allVehicles();
}
// select the "next" vehicle: the one listed after the currently selected one
// in allVehiclesOfSelectedPlugIn
void OpenSteer::OpenSteerDemo::selectNextVehicle(){
  if(selectedVehicle != NULL){
    // get a container of all vehicles
    const AVGroup& all = allVehiclesOfSelectedPlugIn();
    const AVIterator first = all.begin();
    const AVIterator last = all.end();
    // find selected vehicle in container
    const AVIterator s = std::find(first, last, selectedVehicle);
    // normally select the next vehicle in container
    selectedVehicle = *(s+1);
    // if we are at the end of the container, select the first vehicle
    if(s == last-1) selectedVehicle = *first;
    // if the search failed, use NULL
    if(s == last) selectedVehicle = NULL;
  }
}
// select vehicle nearest the given screen position (e.g.: of the mouse)
void OpenSteer::OpenSteerDemo::selectVehicleNearestScreenPosition(int x, int y){
  selectedVehicle = findVehicleNearestScreenPosition(x, y);
}
// Find the AbstractVehicle whose screen position is nearest the current the
// mouse position.  Returns NULL if mouse is outside this window or if
// there are no AbstractVehicle.
OpenSteer::AbstractVehicle* OpenSteer::OpenSteerDemo::vehicleNearestToMouse(){
  int mouseX = irrPointers.Device->getCursorControl()->getPosition().X;
  int mouseY = irrPointers.Device->getCursorControl()->getPosition().Y;
  return findVehicleNearestScreenPosition(mouseX, mouseY);
}
// Find the AbstractVehicle whose screen position is nearest the given window
// coordinates, typically the mouse position.  Returns NULL if there are no
// AbstractVehicles.
OpenSteer::AbstractVehicle* OpenSteer::OpenSteerDemo::findVehicleNearestScreenPosition(int x, int y){
  // pickin is easy, because we have a scene node
  irr::scene::ISceneNode* nSelected =
  irrPointers.SMgr->getSceneCollisionManager()
    ->getSceneNodeFromScreenCoordinatesBB(irr::core::position2di(x,y));


//  // find the direction from the camera position to the given pixel
//  const Vec3 direction = directionFromCameraToScreenPosition(x, y, 600);
//  // iterate over all vehicles to find the one whose center is nearest the
//  // "eye-mouse" selection line
//  float minDistance = FLT_MAX;       // smallest distance found so far
//  const AVGroup& vehicles = allVehiclesOfSelectedPlugIn();
//  for(AVIterator i = vehicles.begin(); i != vehicles.end(); i++){
//    // distance from this vehicle's center to the selection line:
//    const float d = distanceFromLine((**i).position(),
//                                      camera.position(),
//                                      direction);
//    // if this vehicle-to-line distance is the smallest so far,
//    // store it and this vehicle in the selection registers.
//    if(d < minDistance){
//      minDistance = d;
//      nearest = *i;
//    }
//  }

  return (AbstractVehicle*)nSelected;
}

namespace{ // XXX this is used by CaptureTheFlag
           // XXX needs to be reconsidered.
  bool gDelayedResetPlugInXXX = false;
} // anonymous namespace
void OpenSteer::OpenSteerDemo::queueDelayedResetPlugInXXX(){
  gDelayedResetPlugInXXX = true;
}
void OpenSteer::OpenSteerDemo::doDelayedResetPlugInXXX(){
  if(gDelayedResetPlugInXXX){
    resetSelectedPlugIn();
    gDelayedResetPlugInXXX = false;
  }
}

// ------------------------------ phase update ----------------------------------------------
// manage OpenSteerDemo phase transitions (xxx and maintain phase timers)
const int OpenSteer::OpenSteerDemo::overheadPhase = 0;
const int OpenSteer::OpenSteerDemo::updatePhase = 1;
const int OpenSteer::OpenSteerDemo::drawPhase = 2;
int OpenSteer::OpenSteerDemo::phaseStackIndex = 0;
const int OpenSteer::OpenSteerDemo::phaseStackSize = 5;
int OpenSteer::OpenSteerDemo::phaseStack[OpenSteer::OpenSteerDemo::phaseStackSize];
float OpenSteer::OpenSteerDemo::phaseTimerBase = 0;
float OpenSteer::OpenSteerDemo::phaseTimers[drawPhase+1];
namespace OpenSteer{
bool updatePhaseActive = false;
bool drawPhaseActive = false;
}
void OpenSteer::OpenSteerDemo::pushPhase(const int newPhase){
  updatePhaseActive = newPhase == OpenSteer::OpenSteerDemo::updatePhase;
  drawPhaseActive = newPhase == OpenSteer::OpenSteerDemo::drawPhase;
  // update timer for current (old) phase: add in time since last switch
  updatePhaseTimers();
  // save old phase
  phaseStack[phaseStackIndex++] = phase;
  // set new phase
  phase = newPhase;
  // check for stack overflow
  if(phaseStackIndex >= phaseStackSize) errorExit("phaseStack overflow");
}
void OpenSteer::OpenSteerDemo::popPhase(){
  // update timer for current (old) phase: add in time since last switch
  updatePhaseTimers ();
  // restore old phase
  phase = phaseStack[--phaseStackIndex];
  updatePhaseActive = phase == OpenSteer::OpenSteerDemo::updatePhase;
  drawPhaseActive = phase == OpenSteer::OpenSteerDemo::drawPhase;
}
void OpenSteer::OpenSteerDemo::initPhaseTimers(){
  phaseTimers[drawPhase] = 0;
  phaseTimers[updatePhase] = 0;
  phaseTimers[overheadPhase] = 0;
  phaseTimerBase = clock.getTotalRealTime();
}
void OpenSteer::OpenSteerDemo::updatePhaseTimers(){
  const float currentRealTime = clock.realTimeSinceFirstClockUpdate();
  phaseTimers[phase] += currentRealTime - phaseTimerBase;
  phaseTimerBase = currentRealTime;
}

// cycle through frame rate presets
void OpenSteer::OpenSteerDemo::selectNextPresetFrameRate(){
  // note that the cases are listed in reverse order, and that
  // the default is case 0 which causes the index to wrap around
  static int frameRatePresetIndex = 2;
  switch(++frameRatePresetIndex){
    case 3:
      // animation mode at 60 fps
      OpenSteer::OpenSteerDemo::clock.setFixedFrameRate(60);
      OpenSteer::OpenSteerDemo::clock.setAnimationMode(true);
      OpenSteer::OpenSteerDemo::clock.setVariableFrameRateMode(false);
    break;
    case 2:
      // real-time fixed frame rate mode at 60 fps
      OpenSteer::OpenSteerDemo::clock.setFixedFrameRate(60);
      OpenSteer::OpenSteerDemo::clock.setAnimationMode(false);
      OpenSteer::OpenSteerDemo::clock.setVariableFrameRateMode(false);
    break;
    case 1:
      // real-time fixed frame rate mode at 24 fps
      OpenSteer::OpenSteerDemo::clock.setFixedFrameRate(24);
      OpenSteer::OpenSteerDemo::clock.setAnimationMode(false);
      OpenSteer::OpenSteerDemo::clock.setVariableFrameRateMode(false);
    break;
    case 0:
    default:
      // real-time variable frame rate mode ("as fast as possible")
      frameRatePresetIndex = 0;
      OpenSteer::OpenSteerDemo::clock.setFixedFrameRate(0);
      OpenSteer::OpenSteerDemo::clock.setAnimationMode(false);
      OpenSteer::OpenSteerDemo::clock.setVariableFrameRateMode(true);
    break;
  }
}

// ---------------------- concole output ------------------------------------------------------
void OpenSteer::OpenSteerDemo::printMessage(const char* message){
  std::cout << "OpenSteerDemo: " <<  message << std::endl << std::flush;
}
void OpenSteer::OpenSteerDemo::printWarning(const char* message){
  std::cout << "OpenSteerDemo: Warning: " <<  message << std::endl << std::flush;
}
void OpenSteer::OpenSteerDemo::setInfoText(irr::gui::IGUIStaticText* st){
  irr::core::stringw txt;
  txt  = L" Tab: Select next PlugIn.\n";
  txt += L" Space: Toggle run/pause state.\n";
  txt += L" O: Next simulation step.\n";
  txt += L" R: Reset selected PlugIn.\n";
  txt += L" F: Cycle through frame rate presets.\n";
  txt += L" wsad: Move camera.\n";
  txt += L" Esc: Close program.";
  st->setText(txt.c_str());
}
void OpenSteer::OpenSteerDemo::keyboardMiniHelp(){
  std::cout << "Key Commands:" << std::endl << std::flush;
  std::cout << "    Tab: Select next PlugIn." << std::endl << std::flush;
  std::cout << " F1-F12: Function keys for selected PlugIn." <<  std::endl << std::flush;
  std::cout << "  Space: Toggle run/pause state." << std::endl << std::flush;
  std::cout << "      O: Next simulation step." << std::endl << std::flush;
  std::cout << "      R: Reset selected PlugIn." << std::endl << std::flush;
  std::cout << "      N: Select next vehicle." << std::endl << std::flush;
  std::cout << "      F: Cycle through frame rate presets." << std::endl << std::flush;
  std::cout << "      H: Print minimal help (this text)." << std::endl << std::flush;
  std::cout << "    Esc: Close program." <<  std::endl << std::flush;
}

#endif
