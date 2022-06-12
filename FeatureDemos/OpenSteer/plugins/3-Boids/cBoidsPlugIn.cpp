
#ifdef OPENSTEER
/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
#include "cBoidsPlugIn.h"

//! extern Irrlicht stuff
extern datIrrPointers irrPointers;
extern irr::core::array<datMesh> lstObstacleMesh;
extern irr::gui::IGUIStaticText* Text2;

//! !!! Create an instance of the plugin !!!
BoidsPlugIn PI03_Boids;

//! Create Scene
void BoidsPlugIn::open(){
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
  // make the database used to accelerate proximity queries
  cyclePD = -1;
  nextPD();
  // make default-sized flock
  population = 0;
  for(int i = 0; i < 200; i++) addBoidToFlock();
  // set up obstacles
  initObstacles();
  setInfoText(Text2);
}
//! Reset Scene
void BoidsPlugIn::reset(){
  //! reset each boid in flock
  for(iterator i = flock.begin(); i != flock.end(); i++) (**i).reset();
  OpenSteerDemo::selectedVehicle = NULL;
}
//! Clear Scene
void BoidsPlugIn::close(){
#ifdef useIrrExtensions13
  if(nGround) irrPointers.SMgr->addToDeletionQueue(nGround);
  nGround = NULL;
#endif
  // delete each member of the flock
  while(population > 0) removeBoidFromFlock();
  // delete the proximity database
  delete pd;
  pd = NULL;
  // delete obstacle nodes
  lstObstacleMesh.clear();
}
//! Update Simulation
void BoidsPlugIn::update(const float currentTime, const float elapsedTime){
  Boid::maxNeighbors = Boid::totalNeighbors = 0;
  Boid::minNeighbors = std::numeric_limits<int>::max();
  // update flock simulation for each boid
  for(iterator i = flock.begin(); i != flock.end(); i++){
    (**i).update(currentTime, elapsedTime);
  }
}
//! Redraw Scene (Irrlicht does all the stuff)
void BoidsPlugIn::redraw(const float currentTime, const float elapsedTime){
}
//! Returns a list of all vehicles
const AVGroup& BoidsPlugIn::allVehicles(){
  return (const AVGroup&)flock;
}
void BoidsPlugIn::nextPD(){
  // save pointer to old PD
  ProximityDatabase* oldPD = pd;
  // allocate new PD
  const int totalPD = 2;
  switch(cyclePD = (cyclePD + 1) % totalPD){
    case 0:{
      const Vec3 center;
      const float div = 10.0f;
      const Vec3 divisions(div, div, div);
      const float diameter = Boid::worldRadius * 1.1f * 2;
      const Vec3 dimensions(diameter, diameter, diameter);
      typedef LQProximityDatabase<AbstractVehicle*> LQPDAV;
      pd = new LQPDAV(center, dimensions, divisions);
    }break;
    case 1:{
      pd = new BruteForceProximityDatabase<AbstractVehicle*>();
    }break;
  }
  // switch each boid to new PD
  for(iterator i=flock.begin(); i!=flock.end(); i++) (**i).newPD(*pd);
  // delete old PD (if any)
  delete oldPD;
}
void BoidsPlugIn::printLQbinStats(){
  int min, max; float average;
  Boid& aBoid = **(flock.begin());
  aBoid.proximityToken->getBinPopulationStats (min, max, average);
//  std::cout << std::setprecision (2)
//            << std::setiosflags (std::ios::fixed);
//  std::cout << "Bin populations: min, max, average: "
//            << min << ", " << max << ", " << average
//            << " (non-empty bins)" << std::endl;
//  std::cout << "Boid neighbors:  min, max, average: "
//            << Boid::minNeighbors << ", "
//            << Boid::maxNeighbors << ", "
//            << ((float)Boid::totalNeighbors) / ((float)population)
//            << std::endl;
}

void BoidsPlugIn::addBoidToFlock(){
  irr::scene::ISceneManager* SMgr = 0;
  irr::scene::ISceneNode* SRoot = 0;
  if(irrPointers.SMgr){
    SMgr = irrPointers.SMgr;
    SRoot = irrPointers.SMgr->getRootSceneNode();
  }
  population++;
  Boid* boid = new Boid(*pd, SMgr, SRoot);
  flock.push_back(boid);
//  if(population == 1) OpenSteerDemo::selectedVehicle = boid;
}
void BoidsPlugIn::removeBoidFromFlock(){
  if(population > 0){
    // save a pointer to the last boid, then remove it from the flock
    const Boid* boid = flock.back();
    flock.pop_back();
    population--;
    // if it is OpenSteerDemo's selected vehicle, unselect it
    if(boid == OpenSteerDemo::selectedVehicle)
      OpenSteerDemo::selectedVehicle = NULL;
    irrPointers.SMgr->addToDeletionQueue((irr::scene::ISceneNode*)boid);
  }
}
// select next "boundary condition / constraint / obstacle"
void BoidsPlugIn::nextBoundaryCondition(){
  constraint = (ConstraintType)((int)constraint + 1);
  updateObstacles();
}

void BoidsPlugIn::initObstacles(){
  constraint = none;
  lstObstacleMesh.clear();

  insideBigSphere.radius(Boid::worldRadius);
  insideBigSphere.setSeenFrom(Obstacle::inside);
  {datMesh nm = getMesh_Sphere(Boid::worldRadius);
  nm.setColor(irr::video::SColor(255, 50,50,100));
  lstObstacleMesh.push_back(nm);}

  outsideSphere0.radius(Boid::worldRadius * 0.5);
  {datMesh nm = getMesh_Sphere(Boid::worldRadius * 0.5);
  nm.setColor(irr::video::SColor(255, 25,50,50));
  lstObstacleMesh.push_back(nm);}

  const float r = Boid::worldRadius * 0.33f;
  const float p = Boid::worldRadius * 0.5f;
  const float m = -p;
  const float z = 0.0f;

  outsideSphere1.radius(r);
  outsideSphere1.setPosition(p, z, z);
  {datMesh nm = getMesh_Sphere(r);
  nm.setColor(irr::video::SColor(255, 25,50,50));
  nm.Transform.setTranslation(irr::core::vector3df(p, z, z));
  lstObstacleMesh.push_back(nm);}

  outsideSphere2.radius(r);
  outsideSphere2.setPosition(m, z, z);
  {datMesh nm = getMesh_Sphere(r);
  nm.setColor(irr::video::SColor(255, 25,50,50));
  nm.Transform.setTranslation(irr::core::vector3df(m, z, z));
  lstObstacleMesh.push_back(nm);}

  outsideSphere3.radius(r);
  outsideSphere3.setPosition(z, p, z);
  {datMesh nm = getMesh_Sphere(r);
  nm.setColor(irr::video::SColor(255, 25,50,50));
  nm.Transform.setTranslation(irr::core::vector3df(z, p, z));
  lstObstacleMesh.push_back(nm);}

  outsideSphere4.radius(r);
  outsideSphere4.setPosition(z, m, z);
  {datMesh nm = getMesh_Sphere(r);
  nm.setColor(irr::video::SColor(255, 25,50,50));
  nm.Transform.setTranslation(irr::core::vector3df(z, m, z));
  lstObstacleMesh.push_back(nm);}

  outsideSphere5.radius(r);
  outsideSphere5.setPosition(z, z, p);
  {datMesh nm = getMesh_Sphere(r);
  nm.setColor(irr::video::SColor(255, 25,50,50));
  nm.Transform.setTranslation(irr::core::vector3df(z, z, p));
  lstObstacleMesh.push_back(nm);}

  outsideSphere6.radius(r);
  outsideSphere6.setPosition(z, z, m);
  {datMesh nm = getMesh_Sphere(r);
  nm.setColor(irr::video::SColor(255, 25,50,50));
  nm.Transform.setTranslation(irr::core::vector3df(z, z, m));
  lstObstacleMesh.push_back(nm);}

  Vec3 tiltF = Vec3(1.0f, 1.0f, 0.0f).normalize ();
  Vec3 tiltS(0.0f, 0.0f, 1.0f);
  Vec3 tiltU = Vec3(-1.0f, 1.0f, 0.0f).normalize ();

  bigRectangle.width(50.0);
  bigRectangle.height(80.0);
  bigRectangle.setSeenFrom(Obstacle::both);
  bigRectangle.forward(tiltF);
  bigRectangle.side(tiltS);
  bigRectangle.up(tiltU);
  {datMesh nm = getMesh_Plane(50.0, 80.0);
  nm.setColor(irr::video::SColor(255, 25,50,50));
  nm.Transform.setRotationDegrees(tiltF.vector3df().getHorizontalAngle());
  lstObstacleMesh.push_back(nm);}

  outsideBigBox.width(50.0f);
  outsideBigBox.height(80.0f);
  outsideBigBox.depth(20.0f);
  outsideBigBox.forward(tiltF);
  outsideBigBox.side(tiltS);
  outsideBigBox.up(tiltU);
  {datMesh nm = getMesh_Box(50.0, 80.0, 20.0);
  nm.setColor(irr::video::SColor(255, 25,50,50));
  nm.Transform.setRotationDegrees(tiltF.vector3df().getHorizontalAngle());
  lstObstacleMesh.push_back(nm);}

  insideBigBox = outsideBigBox;
  insideBigBox.setSeenFrom(Obstacle::inside);

  updateObstacles();
}
// update Boid::obstacles list when constraint changes
void BoidsPlugIn::updateObstacles(){
  // first clear out obstacle list
  Boid::obstacles.clear();
  for(irr::u32 t = 0; t < lstObstacleMesh.size(); ++t)
    lstObstacleMesh[t].Visible = false;

  // add back obstacles based on mode
  switch(constraint){
    default:
    case none:
      // reset for wrap-around, fall through to first case:
      constraint = none;
    break;
    case insideSphere:
      Boid::obstacles.push_back(&insideBigSphere);
      lstObstacleMesh[0].Visible = true;
    break;
    case outsideSphere:
      Boid::obstacles.push_back(&insideBigSphere);
      lstObstacleMesh[0].Visible = true;
      Boid::obstacles.push_back(&outsideSphere0);
      lstObstacleMesh[1].Visible = true;
    break;
    case outsideSpheres:
      Boid::obstacles.push_back(&insideBigSphere);
      lstObstacleMesh[0].Visible = true;
    case outsideSpheresNoBig:
      Boid::obstacles.push_back(&outsideSphere1);
      lstObstacleMesh[2].Visible = true;
      Boid::obstacles.push_back(&outsideSphere2);
      lstObstacleMesh[3].Visible = true;
      Boid::obstacles.push_back(&outsideSphere3);
      lstObstacleMesh[4].Visible = true;
      Boid::obstacles.push_back(&outsideSphere4);
      lstObstacleMesh[5].Visible = true;
      Boid::obstacles.push_back(&outsideSphere5);
      lstObstacleMesh[6].Visible = true;
      Boid::obstacles.push_back(&outsideSphere6);
      lstObstacleMesh[7].Visible = true;
    break;
    case rectangle:
      Boid::obstacles.push_back(&insideBigSphere);
      lstObstacleMesh[0].Visible = true;
      Boid::obstacles.push_back(&bigRectangle);
      lstObstacleMesh[8].Visible = true;
    case rectangleNoBig:
      Boid::obstacles.push_back(&bigRectangle);
      lstObstacleMesh[8].Visible = true;
    break;
    case outsideBox:
      Boid::obstacles.push_back(&insideBigSphere);
      lstObstacleMesh[0].Visible = true;
      Boid::obstacles.push_back(&outsideBigBox);
      lstObstacleMesh[9].Visible = true;
    break;
    case insideBox:
      Boid::obstacles.push_back(&insideBigBox);
      lstObstacleMesh[9].Visible = true;
    break;
  }
}

void BoidsPlugIn::handleFunctionKeys(int keyNumber){
  switch(keyNumber){
    case 1: addBoidToFlock();         break;
    case 2: removeBoidFromFlock();    break;
    case 3: nextPD();                 break;
    case 4: nextBoundaryCondition();  break;
  }
}
void BoidsPlugIn::printMiniHelpForFunctionKeys(){
//  std::ostringstream message;
//  message << "Function keys handled by ";
//  message << '"' << name() << '"' << ':' << std::ends;
//  OpenSteerDemo::printMessage (message);
//  OpenSteerDemo::printMessage ("  F1     add a boid to the flock.");
//  OpenSteerDemo::printMessage ("  F2     remove a boid from the flock.");
//  OpenSteerDemo::printMessage ("  F3     use next proximity database.");
//  OpenSteerDemo::printMessage ("  F4     next flock boundary condition.");
//  OpenSteerDemo::printMessage ("");
}
void BoidsPlugIn::setInfoText(irr::gui::IGUIStaticText* st){
  irr::core::stringw txt;
  txt  = L"F1     add a boid to the flock.\n";
  txt += L"F2     remove a boid from the flock.\n";
  txt += L"F3     use next proximity database.\n";
  txt += L"F4     next flock boundary condition.";
  st->setText(txt.c_str());
}
#endif
