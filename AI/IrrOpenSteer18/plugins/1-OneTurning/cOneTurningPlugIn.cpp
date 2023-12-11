#ifdef OPENSTEER
/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
#include "cOneTurningPlugIn.h"

//! extern Irrlicht stuff
extern datIrrPointers irrPointers;
extern irr::gui::IGUIStaticText* Text2;

//! !!! Create an instance of the plugin !!!
OneTurningPlugIn PI01_OneTurning;

//! Create Scene
void OneTurningPlugIn::open(){
  //! Create a new vehicle scene node !!!
  if(irrPointers.SMgr){
    gOneTurning = new OneTurning(irrPointers.SMgr, irrPointers.SMgr->getRootSceneNode());
#ifdef useIrrExtensions13
    //! Create "ground plane"
    if(nGround) irrPointers.SMgr->addToDeletionQueue(nGround);
    nGround = irrPointers.SMgr->addGridSceneNode();
    nGround->setPosition(irr::core::vector3df(0,0,0));
    nGround->setAccentlineColor(nGround->getGridColor());
#endif
  }else{
    gOneTurning = new OneTurning(0, 0);
    OpenSteerDemo::selectedVehicle = gOneTurning;
    theVehicle.push_back(gOneTurning);
  }
  setInfoText(Text2);
}
//! Reset Scene
void OneTurningPlugIn::reset(){
  // reset vehicle
  gOneTurning->reset();
  gOneTurning->setMaterialFlag(irr::video::EMF_WIREFRAME, true);
}
//! Clear Scene
void OneTurningPlugIn::close(){
  theVehicle.clear();
#ifdef useIrrExtensions13
  if(nGround) irrPointers.SMgr->addToDeletionQueue(nGround);
  nGround = NULL;
#endif
  if(gOneTurning) irrPointers.SMgr->addToDeletionQueue(gOneTurning);
  gOneTurning = NULL;
}
//! Update Simulation
void OneTurningPlugIn::update(const float currentTime, const float elapsedTime){
  // update simulation of test vehicle
  gOneTurning->update(currentTime, elapsedTime);
}
//! Redraw Scene (Irrlicht does all the stuff)
void OneTurningPlugIn::redraw(const float currentTime, const float elapsedTime){
  // draw test vehicle
  gOneTurning->draw();
}
//! Returns a list of all vehicles
const AVGroup& OneTurningPlugIn::allVehicles(){
  return (const AVGroup&)theVehicle;
}

void OneTurningPlugIn::setInfoText(irr::gui::IGUIStaticText* st){
  irr::core::stringw txt;

  st->setText(txt.c_str());
}
#endif
