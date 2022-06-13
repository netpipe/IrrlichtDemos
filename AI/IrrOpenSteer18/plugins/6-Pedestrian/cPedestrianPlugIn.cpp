////#ifdef OPENSTEER
///*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
//#include "cPedestrianPlugIn.h"
//
////! extern Irrlicht stuff
//extern datIrrPointers irrPointers;
//extern bool gUseDirectedPathFollowing;
//extern bool gWanderSwitch;
//extern irr::core::array<datMesh> lstObstacleMesh;
//extern irr::gui::IGUIStaticText* Text2;
//
////! !!! Create an instance of the plugin !!!
//PedestrianPlugIn PI06_Pedestrians;
//
////! Create Scene
//void PedestrianPlugIn::open(){
//  irr::scene::ISceneManager* SMgr = 0;
//  irr::scene::ISceneNode* SRoot = 0;
//  if(irrPointers.SMgr){
//    SMgr = irrPointers.SMgr;
//    SRoot = irrPointers.SMgr->getRootSceneNode();
//#ifdef useIrrExtensions13
//    //! Create "ground plane"
//    if(nGround) irrPointers.SMgr->addToDeletionQueue(nGround);
//    nGround = irrPointers.SMgr->addGridSceneNode();
//    nGround->setPosition(irr::core::vector3df(0,0,0));
//    nGround->setAccentlineColor(nGround->getGridColor());
//#endif
//  }
//
//  //! make the database used to accelerate proximity queries
//  cyclePD = -1;
//  nextPD ();
//  //! create the specified number of Pedestrians
//  population = 0;
//  for(int i = 0; i < gPedestrianStartCount; i++) addPedestrianToCrowd();
//  //! initialize selectedVehicle
//  Pedestrian& firstPedestrian = **crowd.begin();
//
//  //! create mesh for path way
//  lstObstacleMesh.clear();
//  datMesh nm = getMesh_Path(getTestPath());
//  nm.setColor(irr::video::SColor(255, 0,200,0));
//  lstObstacleMesh.push_back(nm);
//
//  setInfoText(Text2);
//}
////! Reset Scene
//void PedestrianPlugIn::reset(){
//  //! reset each Pedestrian
//  for(iterator i = crowd.begin(); i != crowd.end(); i++) (**i).reset();
//}
////! Clear Scene
//void PedestrianPlugIn::close(){
//#ifdef useIrrExtensions13
//  if(nGround) irrPointers.SMgr->addToDeletionQueue(nGround);
//  nGround = NULL;
//#endif
//  while(population > 0) removePedestrianFromCrowd();
//  lstObstacleMesh.clear();
//}
////! Update Simulation
//void PedestrianPlugIn::update(const float currentTime, const float elapsedTime){
//  // update pedestrians
//  for(iterator i = crowd.begin(); i != crowd.end(); i++)
//    (**i).update(currentTime, elapsedTime);
//}
////! Redraw Scene (Irrlicht does all the stuff)
//void PedestrianPlugIn::redraw(const float currentTime, const float elapsedTime){
//}
////! Returns a list of all vehicles
//const AVGroup& PedestrianPlugIn::allVehicles(){
//  return (const AVGroup&)crowd;
//}
//
//void PedestrianPlugIn::addPedestrianToCrowd(){
//  irr::scene::ISceneManager* SMgr = 0;
//  irr::scene::ISceneNode* SRoot = 0;
//  if(irrPointers.SMgr){
//    SMgr = irrPointers.SMgr;
//    SRoot = irrPointers.SMgr->getRootSceneNode();
//  }
//  population++;
//  Pedestrian* pedestrian = new Pedestrian(*pd, SMgr, SRoot);
//
//  crowd.push_back(pedestrian);
////  if(population == 1) OpenSteerDemo::selectedVehicle = pedestrian;
//}
//void PedestrianPlugIn::removePedestrianFromCrowd(){
//  if(population > 0){
//    // save pointer to last pedestrian, then remove it from the crowd
//    const Pedestrian* pedestrian = crowd.back();
//    crowd.pop_back();
//    population--;
//    // if it is OpenSteerDemo's selected vehicle, unselect it
//    if(pedestrian == OpenSteerDemo::selectedVehicle) OpenSteerDemo::selectedVehicle = NULL;
//    // delete the Pedestrian
//    irrPointers.SMgr->addToDeletionQueue((irr::scene::ISceneNode*)pedestrian);
//  }
//}
//
//void PedestrianPlugIn::nextPD(){
//  // save pointer to old PD
//  ProximityDatabase* oldPD = pd;
//  // allocate new PD
//  const int totalPD = 2;
//  switch(cyclePD = (cyclePD + 1) % totalPD){
//    case 0:{
//      const Vec3 center;
//      const float div = 20.0f;
//      const Vec3 divisions(div, 1.0f, div);
//      const float diameter = 80.0f; //XXX need better way to get this
//      const Vec3 dimensions (diameter, diameter, diameter);
//      typedef LQProximityDatabase<AbstractVehicle*> LQPDAV;
//      pd = new LQPDAV(center, dimensions, divisions);
//    }break;
//    case 1:{
//      pd = new BruteForceProximityDatabase<AbstractVehicle*>();
//    }break;
//  }
//  // switch each boid to new PD
//  for (iterator i=crowd.begin(); i!=crowd.end(); i++) (**i).newPD(*pd);
//  // delete old PD (if any)
//  delete oldPD;
//}
//
//void PedestrianPlugIn::handleFunctionKeys(int keyNumber){
//  switch(keyNumber){
//    case 1:  addPedestrianToCrowd();                                break;
//    case 2:  removePedestrianFromCrowd();                           break;
//    case 3:  nextPD();                                              break;
//    case 4: gUseDirectedPathFollowing = !gUseDirectedPathFollowing; break;
//    case 5: gWanderSwitch = !gWanderSwitch;                         break;
//  }
//}
//void PedestrianPlugIn::printMiniHelpForFunctionKeys(){
////  std::ostringstream message;
////  message << "Function keys handled by ";
////  message << '"' << name() << '"' << ':' << std::ends;
////  OpenSteerDemo::printMessage (message);
////  OpenSteerDemo::printMessage (message);
////  OpenSteerDemo::printMessage ("  F1     add a pedestrian to the crowd.");
////  OpenSteerDemo::printMessage ("  F2     remove a pedestrian from crowd.");
////  OpenSteerDemo::printMessage ("  F3     use next proximity database.");
////  OpenSteerDemo::printMessage ("  F4     toggle directed path follow.");
////  OpenSteerDemo::printMessage ("  F5     toggle wander component on/off.");
////  OpenSteerDemo::printMessage ("");
//}
//void PedestrianPlugIn::setInfoText(irr::gui::IGUIStaticText* st){
//  irr::core::stringw txt;
//  txt  = L"F1     add a pedestrian to the crowd.\n";
//  txt += L"F2     remove a pedestrian from crowd.\n";
//  txt += L"F3     use next proximity database.\n";
//  txt += L"F4     toggle directed path follow.\n";
//  txt += L"F5     toggle wander component on/off.";
//  st->setText(txt.c_str());
//}
//#endif
