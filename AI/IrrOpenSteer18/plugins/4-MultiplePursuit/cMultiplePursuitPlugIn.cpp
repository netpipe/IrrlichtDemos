#include "../../../../config.h"
#ifdef OPENSTEER
/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
#include "cMultiplePursuitPlugIn.h"

//! extern Irrlicht stuff
extern datIrrPointers irrPointers;
extern irr::gui::IGUIStaticText* Text2;

//! !!! Create an instance of the plugin !!!
MpPlugIn PI04_MultiPursuit;

//! Create Scene
void MpPlugIn::open(){
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

  //! create the wanderer, saving a pointer to it
  wanderer = new MpWanderer(SMgr, SRoot);
  allMP.push_back(wanderer);
  // create the specified number of pursuers, save pointers to them
  const int pursuerCount = 30;
  for(int i = 0; i < pursuerCount; i++)
    allMP.push_back(new MpPursuer(wanderer, SMgr, SRoot));
  pBegin = allMP.begin() + 1;  // iterator pointing to first pursuer
  pEnd = allMP.end();          // iterator pointing to last pursuer
  setInfoText(Text2);
}
//! Reset Scene
void MpPlugIn::reset(){
  // reset wanderer and pursuers
  wanderer->reset();
  for(iterator i = pBegin; i != pEnd; i++)
    ((MpPursuer&)(**i)).reset();
}
//! Clear Scene
void MpPlugIn::close(){
  // delete wanderer, all pursuers, and clear list
#ifdef useIrrExtensions13
  if(nGround) irrPointers.SMgr->addToDeletionQueue(nGround);
  nGround = NULL;
#endif
  irrPointers.SMgr->addToDeletionQueue(wanderer);
  wanderer = NULL;
  for(iterator i = pBegin; i != pEnd; i++)
    irrPointers.SMgr->addToDeletionQueue((irr::scene::ISceneNode*)*i);
  allMP.clear();
}
//! Update Simulation
void MpPlugIn::update(const float currentTime, const float elapsedTime){
  // update the wanderer
  wanderer->update(currentTime, elapsedTime);
  // update each pursuer
  for(iterator i = pBegin; i != pEnd; i++){
    ((MpPursuer&)(**i)).update(currentTime, elapsedTime);
  }
}
//! Redraw Scene (Irrlicht does all the stuff)
void MpPlugIn::redraw(const float currentTime, const float elapsedTime){
  // draw test vehicle
//  wanderer->draw();
}
//! Returns a list of all vehicles
const AVGroup& MpPlugIn::allVehicles(){
  return (const AVGroup&)allMP;
}

void MpPlugIn::setInfoText(irr::gui::IGUIStaticText* st){
  irr::core::stringw txt;

  st->setText(txt.c_str());
}
#endif
