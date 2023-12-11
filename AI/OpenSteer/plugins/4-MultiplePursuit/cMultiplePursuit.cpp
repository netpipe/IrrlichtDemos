/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
#include "cMultiplePursuit.h"

//! extern Irrlicht stuff
extern datIrrPointers irrPointers;

// constructor
MpBase::MpBase(irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent)
:ISceneNode(parent,smgr){
  reset();
  //! disable lighting
  Material.Lighting = false;
}

// reset state
void MpBase::reset(){
  SimpleVehicle::reset (); // reset the vehicle
  speed (0);            // speed along Forward direction.
  maxForce (5.0);       // steering force is clipped to this magnitude
  maxSpeed (3.0);       // velocity is clipped to this magnitude
  gaudyPursuitAnnotation = true; // select use of 9-color annotation
  clearTrailHistory();    // prevent long streaks due to teleportation
  // load the mesh
  Mesh = getMesh_VehicleDisk();
  Box = Mesh.getBoundingbox();
  setScale(irr::core::vector3df(0.5,0.25,0.5));
}
// per frame simulation update
void MpBase::update(const float currentTime, const float elapsedTime){
}
// draw this character/vehicle into the scene
void MpBase::draw(){
  // Irrlicht does all the work ;)
}

void MpBase::clearTrailHistory(){
  // we use our own trail mesh
  Trail.faceIndices.clear();
  Trail.faceVertices.clear();
  Trail.lineIndices.clear();
  Trail.lineVertices.clear();
  // create initial vertex
  irr::video::S3DVertex vert;
  vert.Pos = position().vector3df();
  vert.Color = irr::video::SColor(255, 255,0,0);
  Trail.lineVertices.push_back(vert);

}
void MpBase::recordTrailVertex(const float currentTime, const Vec3& pos){
  // we use our own trail mesh
  Trail.lineIndices.push_back(Trail.lineVertices.size() - 1);
  Trail.lineIndices.push_back(Trail.lineVertices.size());
  irr::video::S3DVertex vert;
  vert.Pos = irr::core::vector3df(pos.x,pos.y,pos.z);
  vert.Color = irr::video::SColor(255, 255,0,0);
  Trail.lineVertices.push_back(vert);
}

// SceneNode
void MpBase::OnRegisterSceneNode(){
  if(IsVisible) irrPointers.SMgr->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}
void MpBase::render(){
  irrPointers.Driver->setMaterial(Material);
  irrPointers.Driver->setTransform(irr::video::ETS_WORLD, irr::scene::ISceneNode::AbsoluteTransformation);
  Mesh.draw(irrPointers.Driver);
  irrPointers.Driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
  Trail.draw(irrPointers.Driver);
}
const irr::core::aabbox3d<irr::f32>& MpBase::getBoundingBox() const{
  return Box;
}
irr::u32 MpBase::getMaterialCount(){
  return 1;
}
irr::video::SMaterial& MpBase::getMaterial(irr::u32 i){
  return Material;
}
void MpBase::setMaterial(irr::video::SMaterial newMaterial){
  Material = newMaterial;
}

//! The wanderer
// constructor
MpWanderer::MpWanderer(irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent)
:MpBase(smgr, parent){
  reset();
}
// reset state
void MpWanderer::reset(){
  MpBase::reset();
  Mesh.setColor(irr::video::SColor(255, 100,250,100));
  clearTrailHistory();
}
// per frame simulation update
void MpWanderer::update(const float currentTime, const float elapsedTime){
  const Vec3 wander2d = steerForWander(elapsedTime).setYtoZero();
  const Vec3 steer = forward() + (wander2d * 3);
  applySteeringForce(steer, elapsedTime);
  // update Irrlicht node
  setPosition(position().vector3df());
  irr::scene::ISceneNode::setRotation(forward().vector3df().getHorizontalAngle());
  // update motion trail
//  recordTrailVertex(currentTime, position());
}

//! The pursuer
// constructor
MpPursuer::MpPursuer(MpWanderer* w, irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent)
:MpBase(smgr, parent){
  wanderer = w;
  reset();
}
// reset state
void MpPursuer::reset(){
  MpBase::reset();
  Mesh.setColor(irr::video::SColor(255, 250,100,100));
  randomizeStartingPositionAndHeading();
  clearTrailHistory();
}
void MpPursuer::randomizeStartingPositionAndHeading(){
  // randomize position on a ring between inner and outer radii
  // centered around the home base
  const float inner = 20;
  const float outer = 30;
  const float radius = frandom2(inner, outer);
  const Vec3 randomOnRing = RandomUnitVectorOnXZPlane() * radius;
  position(wanderer->position() + randomOnRing);
  // randomize 2D heading
  randomizeHeadingOnXZPlane();
}
// per frame simulation update
void MpPursuer::update(const float currentTime, const float elapsedTime){
  // when pursuer touches quarry ("wanderer"), reset its position
  const float d = Vec3::distance(position(), wanderer->position());
  const float r = radius() + wanderer->radius();
  if(d < r) reset();
  const float maxTime = 20; // xxx hard-to-justify value
  applySteeringForce(steerForPursuit(*wanderer, maxTime), elapsedTime);
  // update Irrlicht node
  setPosition(position().vector3df());
  irr::scene::ISceneNode::setRotation(forward().vector3df().getHorizontalAngle());
  // update motion trail
//  recordTrailVertex(currentTime, position());
}

