/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
#include "cMapDrivePlugIn.h"

//! extern Irrlicht stuff
extern datIrrPointers irrPointers;
extern irr::core::array<datMesh> lstObstacleMesh;
extern irr::gui::IGUIStaticText* Text2;

//! !!! Create an instance of the plugin !!!
MapDrivePlugIn PI09_MapDrive;

//! Create Scene
void MapDrivePlugIn::open(){
  irr::scene::ISceneManager* SMgr = 0;
  irr::scene::ISceneNode* SRoot = 0;
  if(irrPointers.SMgr){
    SMgr = irrPointers.SMgr;
    SRoot = irrPointers.SMgr->getRootSceneNode();
#ifdef useIrrExtensions13
    //! Create "ground plane"
    if(nGround) irrPointers.SMgr->addToDeletionQueue(nGround);
    nGround = irrPointers.SMgr->addGridSceneNode();
    nGround->setPosition(irr::core::vector3df(0,0,0));
    nGround->setAccentlineColor(nGround->getGridColor());
#endif
  }

  // make new MapDriver
  vehicle = new MapDriver(SMgr, SRoot);
  vehicles.push_back(vehicle);
  OpenSteerDemo::selectedVehicle = vehicle;
  // marks as obstacles map cells adjacent to the path
  usePathFences = true;
  // scatter random rock clumps over map
  useRandomRocks = true;
  setInfoText(Text2);
}
//! Update Simulation
void MapDrivePlugIn::update(const float currentTime, const float elapsedTime){
  // update simulation of test vehicle
  vehicle->update(currentTime, elapsedTime);
  // when vehicle drives outside the world
  if (vehicle->handleExitFromMap()) regenerateMap();
  // QQQ first pass at detecting "stuck" state
  if (vehicle->stuck && (vehicle->relativeSpeed () < 0.001f)){
    vehicle->stuckCount++;
    reset();
  }
}
//! Reset Scene
void MapDrivePlugIn::reset(){
  //! reset map
  regenerateMap();
  //! reset vehicle
  vehicle->reset();
}
//! Clear Scene
void MapDrivePlugIn::close(){
#ifdef useIrrExtensions13
  if(nGround) irrPointers.SMgr->addToDeletionQueue(nGround);
  nGround = NULL;
#endif
  vehicles.clear();
  irrPointers.SMgr->addToDeletionQueue(vehicle);
  lstObstacleMesh.clear();
}
//! Redraw Scene (Irrlicht does all the stuff)
void MapDrivePlugIn::redraw(const float currentTime, const float elapsedTime){
  // draw test vehicle
//  gVehicle->draw();
}
//! Returns a list of all vehicles
const AVGroup& MapDrivePlugIn::allVehicles(){
  return (const AVGroup&)vehicles;
}

void MapDrivePlugIn::selectNextDemo(){
  switch(++vehicle->demoSelect){
    default:
      vehicle->demoSelect = 0; // wrap-around, falls through to case 0:
    case 0:
      reset();
      break;
    case 1:
      reset();
      break;
    case 2:
      reset ();
      break;
  }
}

void MapDrivePlugIn::reversePathFollowDirection(){
  int& pfd = vehicle->pathFollowDirection;
  pfd = (pfd > 0) ? -1 : +1;
}

void MapDrivePlugIn::togglePathFences(){
  usePathFences = ! usePathFences;
  reset();
}

void MapDrivePlugIn::toggleRandomRocks(){
  useRandomRocks = ! useRandomRocks;
  reset();
}

void MapDrivePlugIn::toggleCurvedSteering(){
  vehicle->curvedSteering = ! vehicle->curvedSteering;
  vehicle->incrementalSteering = ! vehicle->incrementalSteering;
  reset();
}

void MapDrivePlugIn::regenerateMap(){
  // regenerate map: clear and add random "rocks"
  vehicle->map->clear();
  clearCenterOfMap(*vehicle->map);

  // randomize path widths
  if(vehicle->demoSelect == 2){
    const OpenSteer::size_t count = vehicle->path->segmentCount();
    const bool upstream = vehicle->pathFollowDirection > 0;
    const OpenSteer::size_t entryIndex = upstream ? 0 : count-1;
    const OpenSteer::size_t exitIndex  = upstream ? count-1 : 0;
    const float lastExitRadius = vehicle->path->segmentRadius(exitIndex);
    for(OpenSteer::size_t i = 0; i < count; i++){
      vehicle->path->setSegmentRadius(i, frandom2(4, 19));
    }
    vehicle->path->setSegmentRadius(entryIndex, lastExitRadius);
  }
}

void MapDrivePlugIn::clearCenterOfMap(TerrainMap& map){
  const int o = map.cellwidth() >> 4;
  const int p = (map.cellwidth() - o) >> 1;
  const int q = (map.cellwidth() + o) >> 1;
  for(int i = p; i <= q; i++)
    for(int j = p; j <= q; j++)
      map.setMapBit(i, j, 0);
}

void MapDrivePlugIn::handleFunctionKeys(int keyNumber){
  switch(keyNumber){
    case 1: selectNextDemo();             break;
    case 2: reversePathFollowDirection(); break;
    case 3: togglePathFences();           break;
    case 4: toggleRandomRocks();          break;
    case 5: toggleCurvedSteering();       break;
  }
}
void MapDrivePlugIn::printMiniHelpForFunctionKeys(){
//  std::ostringstream message;
//  message << "Function keys handled by ";
//  message << '"' << name() << '"' << ':' << std::ends;
//  OpenSteerDemo::printMessage (message);
//  OpenSteerDemo::printMessage ("  F1     select next driving demo.");
//  OpenSteerDemo::printMessage ("  F2     reverse path following direction.");
//  OpenSteerDemo::printMessage ("  F3     toggle path fences.");
//  OpenSteerDemo::printMessage ("  F4     toggle random rock clumps.");
//  OpenSteerDemo::printMessage ("  F5     toggle curved prediction.");
//  OpenSteerDemo::printMessage ("");
}
void MapDrivePlugIn::setInfoText(irr::gui::IGUIStaticText* st){
  irr::core::stringw txt;
  txt  = L"F1     select next driving demo.\n";
  txt += L"F2     reverse path following direction.\n";
  txt += L"F3     toggle path fences.\n";
  txt += L"F4     toggle random rock clumps.\n";
  txt += L"F5     toggle curved prediction.";
  st->setText(txt.c_str());
}
