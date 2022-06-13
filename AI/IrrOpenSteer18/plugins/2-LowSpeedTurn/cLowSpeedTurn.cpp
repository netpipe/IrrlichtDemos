#include "../../../../config.h"
#ifdef OPENSTEER
/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
#include "cLowSpeedTurn.h"

//! extern Irrlicht stuff
extern datIrrPointers irrPointers;

float LowSpeedTurn::startX;
float LowSpeedTurn::startSpeed;
// constructor
LowSpeedTurn::LowSpeedTurn(irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent)
:ISceneNode(parent,smgr){
  reset();
}

// reset state
void LowSpeedTurn::reset(){
  // reset vehicle state
  SimpleVehicle::reset();
  // speed along Forward direction.
  speed(startSpeed);
  // initial position along X axis
  position(startX, 0, 0);
  // steering force clip magnitude
  maxForce(0.3f);
  // velocity  clip magnitude
  maxSpeed(1.5f);
  // for next instance: step starting location
  startX += 2;
  // for next instance: step speed
  startSpeed += 0.15f;
  // 15 seconds and 150 points along the trail
//  setTrailParameters(15, 150);

  clearTrailHistory();    // prevent long streaks due to teleportation
  // load the mesh
  Mesh = getMesh_VehicleDisk();
  Box = Mesh.getBoundingbox();
  setScale(irr::core::vector3df(2.0, 1.0, 2.0));
  // disable lighting
  Material.Lighting = false;
}
void LowSpeedTurn::resetStarts(){
  startX = 0;
  startSpeed = 0;
}
Vec3 LowSpeedTurn::steering(){
  return Vec3(1, 0, -1);
}
// per frame simulation update
void LowSpeedTurn::update(const float currentTime, const float elapsedTime){
  // apply force to vehicle
  applySteeringForce(steering(), elapsedTime);
  // update Irrlicht node
  setPosition(position().vector3df());
  irr::scene::ISceneNode::setRotation(forward().vector3df().getHorizontalAngle());
  // update motion trail
  recordTrailVertex(currentTime, position());
}
// draw this character/vehicle into the scene
void LowSpeedTurn::draw(){
  // Irrlicht does all the work ;)
}

void LowSpeedTurn::clearTrailHistory(){
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
void LowSpeedTurn::recordTrailVertex(const float currentTime, const Vec3& pos){
  // we use our own trail mesh
  Trail.lineIndices.push_back(Trail.lineVertices.size() - 1);
  Trail.lineIndices.push_back(Trail.lineVertices.size());
  irr::video::S3DVertex vert;
  vert.Pos = irr::core::vector3df(pos.x,pos.y,pos.z);
  vert.Color = irr::video::SColor(255, 255,0,0);
  Trail.lineVertices.push_back(vert);
}

// SceneNode
void LowSpeedTurn::OnRegisterSceneNode(){
  if(IsVisible) irrPointers.SMgr->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}
void LowSpeedTurn::render(){
  irrPointers.Driver->setMaterial(Material);
  irrPointers.Driver->setTransform(irr::video::ETS_WORLD, irr::scene::ISceneNode::AbsoluteTransformation);
  Mesh.draw(irrPointers.Driver);
  irrPointers.Driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
  Trail.draw(irrPointers.Driver);
}
const irr::core::aabbox3d<irr::f32>& LowSpeedTurn::getBoundingBox() const{
  return Box;
}
irr::u32 LowSpeedTurn::getMaterialCount(){
  return 1;
}
irr::video::SMaterial& LowSpeedTurn::getMaterial(irr::u32 i){
  return Material;
}
void LowSpeedTurn::setMaterial(irr::video::SMaterial newMaterial){
  Material = newMaterial;
}
#endif


