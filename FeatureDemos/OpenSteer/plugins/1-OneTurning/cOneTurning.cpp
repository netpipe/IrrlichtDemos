/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
#include "cOneTurning.h"

//! extern Irrlicht stuff
extern datIrrPointers irrPointers;

// constructor
OneTurning::OneTurning(irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent)
:ISceneNode(parent,smgr){
  reset();
  //! disable lighting
  Material.Lighting = false;
}

// reset state
void OneTurning::reset(){
  SimpleVehicle::reset(); // reset the vehicle
  speed(2.5f);         // speed along Forward direction.
  maxForce(0.5f);      // steering force is clipped to this magnitude
  maxSpeed(10);         // velocity is clipped to this magnitude
  clearTrailHistory();    // prevent long streaks due to teleportation
  // load the mesh
  Mesh = getMesh_VehicleDisk();
  setScale(irr::core::vector3df(2.0, 1.0, 2.0));
  Mesh.getBoundingbox(&BBox);
}
// per frame simulation update
void OneTurning::update(const float currentTime, const float elapsedTime){
  // apply force to vehicle
  applySteeringForce(Vec3(-2, 0, -3), elapsedTime);
  // check for position
  if((position().x < -100.0) || (position().z < -100.0)) reset();
  // update Irrlicht node
  setPosition(position().vector3df());
  irr::scene::ISceneNode::setRotation(forward().vector3df().getHorizontalAngle());
  // update motion trail
  recordTrailVertex(currentTime, position());
}
// draw this character/vehicle into the scene
void OneTurning::draw(){
  // Irrlicht does all the work ;)
}

void OneTurning::clearTrailHistory(){
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
void OneTurning::recordTrailVertex(const float currentTime, const Vec3& pos){
  // we use our own trail mesh
  Trail.lineIndices.push_back(Trail.lineVertices.size() - 1);
  Trail.lineIndices.push_back(Trail.lineVertices.size());
  irr::video::S3DVertex vert;
  vert.Pos = irr::core::vector3df(pos.x,pos.y,pos.z);
  vert.Color = irr::video::SColor(255, 255,0,0);
  Trail.lineVertices.push_back(vert);
}

// SceneNode
void OneTurning::OnRegisterSceneNode(){
  if(IsVisible) irrPointers.SMgr->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}
void OneTurning::render(){
  irrPointers.Driver->setMaterial(Material);
  irrPointers.Driver->setTransform(irr::video::ETS_WORLD, irr::scene::ISceneNode::AbsoluteTransformation);
  Mesh.draw(irrPointers.Driver);
  irrPointers.Driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
  Trail.draw(irrPointers.Driver);
}
const irr::core::aabbox3d<irr::f32>& OneTurning::getBoundingBox() const{
  return BBox;
}
irr::u32 OneTurning::getMaterialCount(){
  return 1;
}
irr::video::SMaterial& OneTurning::getMaterial(irr::u32 i){
  return Material;
}
void OneTurning::setMaterial(irr::video::SMaterial newMaterial){
  Material = newMaterial;
}


