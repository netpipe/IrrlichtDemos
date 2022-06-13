#include "../../../../config.h"
#ifdef OPENSTEER

/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
#include "cLowSpeedTurnPlugIn.h"

//! extern Irrlicht stuff
extern datIrrPointers irrPointers;
extern irr::gui::IGUIStaticText* Text2;

//! !!! Create an instance of the plugin !!!
LowSpeedTurnPlugIn PI02_LowSpeedTurn;

const int lstCount = 5;
const float lstLookDownDistance = 18;
const Vec3 lstViewCenter(7, 0, -2);
const Vec3 lstPlusZ(0, 0, 1);

//! Create Scene
void LowSpeedTurnPlugIn::open(){
  //! Create new vehicles !!!
  LowSpeedTurn::resetStarts();
  if(irrPointers.SMgr){
#ifdef useIrrExtensions13
    //! Create "ground plane"
    if(nGround) irrPointers.SMgr->addToDeletionQueue(nGround);
    nGround = irrPointers.SMgr->addGridSceneNode();
    nGround->setPosition(irr::core::vector3df(0,0,0));
    nGround->setAccentlineColor(nGround->getGridColor());
#endif
    // create a given number of agents with stepped inital parameters,
    // store pointers to them in an array.
    for(int i = 0; i < lstCount; i++)
      all.push_back(new LowSpeedTurn(irrPointers.SMgr, irrPointers.SMgr->getRootSceneNode()));
    // initial selected vehicle
    OpenSteerDemo::selectedVehicle = *all.begin();
  }else{
    for(int i = 0; i < lstCount; i++)
      all.push_back(new LowSpeedTurn(0, 0));
    // initial selected vehicle
    OpenSteerDemo::selectedVehicle = *all.begin();
  }
  setInfoText(Text2);
}
//! Reset Scene
void LowSpeedTurnPlugIn::reset(){
  LowSpeedTurn::resetStarts();
  for(iterator i = all.begin(); i != all.end(); i++) (**i).reset();
}
//! Clear Scene
void LowSpeedTurnPlugIn::close(){
#ifdef useIrrExtensions13
  if(nGround) irrPointers.SMgr->addToDeletionQueue(nGround);
  nGround = NULL;
#endif
  for(iterator i = all.begin(); i != all.end(); i++)
    irrPointers.SMgr->addToDeletionQueue(*i);
}
//! Update Simulation
void LowSpeedTurnPlugIn::update(const float currentTime, const float elapsedTime){
  // update each agent
  for(iterator i = all.begin(); i != all.end(); i++)
    (**i).update (currentTime, elapsedTime);
}
//! Redraw Scene (Irrlicht does all the stuff)
void LowSpeedTurnPlugIn::redraw(const float currentTime, const float elapsedTime){
  // draw all vehicles
  for(iterator i = all.begin(); i != all.end(); i++){
    // draw this agent
    (*i)->draw();
  }
}
//! Returns a list of all vehicles
const AVGroup& LowSpeedTurnPlugIn::allVehicles(){
  return (const AVGroup&)all;
}

void LowSpeedTurnPlugIn::setInfoText(irr::gui::IGUIStaticText* st){
  irr::core::stringw txt;

  st->setText(txt.c_str());
}
#endif
