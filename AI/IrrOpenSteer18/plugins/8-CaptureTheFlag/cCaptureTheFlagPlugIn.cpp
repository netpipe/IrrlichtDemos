#ifdef OPENSTEER2
/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
#include "cCaptureTheFlagPlugIn.h"

//! extern Irrlicht stuff
extern datIrrPointers irrPointers;
extern irr::core::array<datMesh> lstObstacleMesh;
extern irr::gui::IGUIStaticText* Text2;

// count the number of times the simulation has reset (e.g. for overnight runs)
int resetCount = 0;
// state for OpenSteerDemo PlugIn
CtfSeeker* ctfSeeker;
CtfEnemy* ctfEnemies[maxEnemyCount];
int CtfBase::obstacleCount = -1; // this value means "uninitialized"
SOG CtfBase::allObstacles;

//! !!! Create an instance of the plugin !!!
CtfPlugIn PI08_CaptureTheFlag;

//! Create Scene
void CtfPlugIn::open(){
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
  resetCount = 0;
  // create the seeker ("hero"/"attacker")
  ctfSeeker = new CtfSeeker(SMgr, SRoot);
  all.push_back(ctfSeeker);
  // create the specified number of enemies,
  // storing pointers to them in an array.
  for(int i = 0; i < maxEnemyCount; i++){
    ctfEnemies[i] = new CtfEnemy(SMgr, SRoot);
    all.push_back(ctfEnemies[i]);
  }
  lstObstacleMesh.clear();
  CtfBase::obstacleCount = -1;
  CtfBase::initializeObstacles();

  setInfoText(Text2);
}
//! Reset Scene
void CtfPlugIn::reset(){
  // count resets
  resetCount++;
  // reset the seeker ("hero"/"attacker") and enemies
  ctfSeeker->reset();
  for(int i = 0; i < maxEnemyCount; i++) ctfEnemies[i]->reset();
}
//! Clear Scene
void CtfPlugIn::close(){
#ifdef useIrrExtensions13
  if(nGround) irrPointers.SMgr->addToDeletionQueue(nGround);
  nGround = NULL;
#endif
  // delete seeker
  irrPointers.SMgr->addToDeletionQueue(ctfSeeker);
  ctfSeeker = NULL;
  // delete each enemy
  for(int i = 0; i < maxEnemyCount; i++){
    irrPointers.SMgr->addToDeletionQueue(ctfEnemies[i]);
    ctfEnemies[i] = NULL;
  }
  // clear the group of all vehicles
  all.clear();
  // delete obstacles
  while(CtfBase::obstacleCount > 0) CtfBase::removeOneObstacle();
  CtfBase::obstacleCount = -1;
  lstObstacleMesh.clear();
}
//! Update Simulation
void CtfPlugIn::update(const float currentTime, const float elapsedTime){
  // update the seeker
  ctfSeeker->update(currentTime, elapsedTime);
  // update each enemy
  for(int i = 0; i < maxEnemyCount; i++)
    ctfEnemies[i]->update(currentTime, elapsedTime);
}
//! Redraw Scene (Irrlicht does all the stuff)
void CtfPlugIn::redraw(const float currentTime, const float elapsedTime){
  // draw test vehicle
//  gVehicle->draw();
}
//! Returns a list of all vehicles
const AVGroup& CtfPlugIn::allVehicles(){
  return (const AVGroup&)all;
}

void CtfPlugIn::handleFunctionKeys(int keyNumber){
  switch(keyNumber){
    case 1: CtfBase::addOneObstacle ();    break;
    case 2: CtfBase::removeOneObstacle (); break;
  }
}
void CtfPlugIn::printMiniHelpForFunctionKeys(){
//  std::ostringstream message;
//  message << "Function keys handled by ";
//  message << '"' << name() << '"' << ':' << std::ends;
//  OpenSteerDemo::printMessage (message);
//  OpenSteerDemo::printMessage ("  F1     add one obstacle.");
//  OpenSteerDemo::printMessage ("  F2     remove one obstacle.");
//  OpenSteerDemo::printMessage ("");
}
void CtfPlugIn::setInfoText(irr::gui::IGUIStaticText* st){
  irr::core::stringw txt;
  txt  = L"F1     add one obstacle.\n";
  txt += L"F2     remove one obstacle.";
  st->setText(txt.c_str());
}
#endif
