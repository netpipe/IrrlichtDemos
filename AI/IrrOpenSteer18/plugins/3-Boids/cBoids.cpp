#ifdef OPENSTEER3
/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
#include "cBoids.h"

//! extern Irrlicht stuff
extern datIrrPointers irrPointers;

AVGroup Boid::neighbors;
float Boid::worldRadius = 50.0f;
ObstacleGroup Boid::obstacles;
long Boid::minNeighbors, Boid::maxNeighbors, Boid::totalNeighbors;

// constructor
Boid::Boid(ProximityDatabase& pd, irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent)
:ISceneNode(parent,smgr){
  // allocate a token for this boid in the proximity database
  proximityToken = NULL;
  newPD(pd);
  //! disable lighting
  Material.Lighting = false;
  reset();
}

// reset state
void Boid::reset(){
  // reset the vehicle
  SimpleVehicle::reset();
  // steering force is clipped to this magnitude
  maxForce(27);
  // velocity is clipped to this magnitude
  maxSpeed(9);
  // initial slow speed
  speed(maxSpeed() * 0.3f);
  // randomize initial orientation
  regenerateOrthonormalBasisUF(RandomUnitVector());
  // randomize initial position
  position(RandomVectorInUnitRadiusSphere () * 20);
  // notify proximity database that our position has changed
  proximityToken->updateForNewPosition(position());
  clearTrailHistory();
  // load the mesh
  Mesh = getMesh_VehicleDisk();
  Box = Mesh.getBoundingbox();
  setScale(irr::core::vector3df(0.5, 0.25, 0.5));
}
// per frame simulation update
void Boid::update(const float currentTime, const float elapsedTime){
  OPENSTEER_UNUSED_PARAMETER(currentTime);
  // steer to flock and avoid obstacles if any
  applySteeringForce(steerToFlock(), elapsedTime);
  // wrap around to contrain boid within the spherical boundary
  sphericalWrapAround();
  // notify proximity database that our position has changed
  proximityToken->updateForNewPosition(position());
  // update Irrlicht node
  setPosition(position().vector3df());
  irr::scene::ISceneNode::setRotation(forward().vector3df().getHorizontalAngle());
  // update motion trail
//  recordTrailVertex(currentTime, position());
}
// draw this character/vehicle into the scene
void Boid::draw(){
  // Irrlicht does all the work ;)
}

void Boid::clearTrailHistory(){
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
void Boid::recordTrailVertex(const float currentTime, const Vec3& pos){
  // we use our own trail mesh
  Trail.lineIndices.push_back(Trail.lineVertices.size() - 1);
  Trail.lineIndices.push_back(Trail.lineVertices.size());
  irr::video::S3DVertex vert;
  vert.Pos = irr::core::vector3df(pos.x,pos.y,pos.z);
  vert.Color = irr::video::SColor(255, 255,0,0);
  Trail.lineVertices.push_back(vert);
}

// SceneNode
void Boid::OnRegisterSceneNode(){
  if(IsVisible) irrPointers.SMgr->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}
void Boid::render(){
  irrPointers.Driver->setMaterial(Material);
  irrPointers.Driver->setTransform(irr::video::ETS_WORLD, irr::scene::ISceneNode::AbsoluteTransformation);
  Mesh.draw(irrPointers.Driver);
  irrPointers.Driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
  Trail.draw(irrPointers.Driver);
}
const irr::core::aabbox3d<irr::f32>& Boid::getBoundingBox() const{
  return Box;
}
irr::u32 Boid::getMaterialCount(){
  return 1;
}
irr::video::SMaterial& Boid::getMaterial(irr::u32 i){
  return Material;
}
void Boid::setMaterial(irr::video::SMaterial newMaterial){
  Material = newMaterial;
}

Vec3 Boid::steerToFlock(){
  // avoid obstacles if needed
  // XXX this should probably be moved elsewhere
  const Vec3 avoidance = steerToAvoidObstacles(1.0f, obstacles);
  if(avoidance != Vec3::zero) return avoidance;
  const float separationRadius =  5.0f;
  const float separationAngle  = -0.707f;
  const float separationWeight =  12.0f;
  const float alignmentRadius = 7.5f;
  const float alignmentAngle  = 0.7f;
  const float alignmentWeight = 8.0f;
  const float cohesionRadius = 9.0f;
  const float cohesionAngle  = -0.15f;
  const float cohesionWeight = 8.0f;
  const float maxRadius = maxXXX(separationRadius, maxXXX(alignmentRadius, cohesionRadius));
  // find all flockmates within maxRadius using proximity database
  neighbors.clear();
  proximityToken->findNeighbors(position(), maxRadius, neighbors);
  // maintain stats on max/min/ave neighbors per boids
  long count = neighbors.size();
  if(maxNeighbors < count) maxNeighbors = count;
  if(minNeighbors > count) minNeighbors = count;
  totalNeighbors += count;
  // determine each of the three component behaviors of flocking
  const Vec3 separation = steerForSeparation (separationRadius,
                                              separationAngle,
                                              neighbors);
  const Vec3 alignment  = steerForAlignment  (alignmentRadius,
                                              alignmentAngle,
                                              neighbors);
  const Vec3 cohesion   = steerForCohesion   (cohesionRadius,
                                              cohesionAngle,
                                              neighbors);

  // apply weights to components (save in variables for annotation)
  const Vec3 separationW = separation * separationWeight;
  const Vec3 alignmentW = alignment * alignmentWeight;
  const Vec3 cohesionW = cohesion * cohesionWeight;

  return separationW + alignmentW + cohesionW;
}
void Boid::sphericalWrapAround(){
  // when outside the sphere
  if(position().length() > worldRadius){
    // wrap around (teleport)
    position(position().sphericalWrapAround(Vec3::zero, worldRadius));
    if(this == OpenSteerDemo::selectedVehicle){
//      irrPointers.Camera->setPosition(OpenSteerDemo::selectedVehicle->position().vector3df());
    }
  }
}
void Boid::regenerateLocalSpace(const Vec3& newVelocity, const float elapsedTime){
  // 3d flight with banking
  regenerateLocalSpaceForBanking(newVelocity, elapsedTime);
}
// XXX experiment:
// XXX   herd with terrain following
// XXX   special case terrain: a sphere at the origin, radius 40
void Boid::regenerateLocalSpaceForTerrainFollowing (const Vec3& newVelocity, const float /* elapsedTime */){
  // XXX this is special case code, these should be derived from arguments //
  const Vec3 surfaceNormal = position().normalize();                       //
  const Vec3 surfacePoint = surfaceNormal * 40.0f;                         //
  // XXX this is special case code, these should be derived from arguments //
  const Vec3 newUp = surfaceNormal;
  const Vec3 newPos = surfacePoint;
  const Vec3 newVel = newVelocity.perpendicularComponent(newUp);
  const float newSpeed = newVel.length();
  const Vec3 newFor = newVel / newSpeed;
  speed(newSpeed);
  position(newPos);
  up(newUp);
  forward(newFor);
  setUnitSideFromForwardAndUp();
}
// switch to new proximity database -- just for demo purposes
void Boid::newPD(ProximityDatabase& pd){
  // delete this boid's token in the old proximity database
  delete proximityToken;
  // allocate a token for this boid in the proximity database
  proximityToken = pd.allocateToken(this);
}
void Boid::annotateAvoidObstacle(const float minDistanceToCollision){
  const Vec3 boxSide = side() * radius();
  const Vec3 boxFront = forward() * minDistanceToCollision;
  const Vec3 FR = position() + boxFront - boxSide;
  const Vec3 FL = position() + boxFront + boxSide;
  const Vec3 BR = position()            - boxSide;
  const Vec3 BL = position()            + boxSide;
  const Color white (1,1,1);
  annotationLine(FR, FL, white);
  annotationLine(FL, BL, white);
  annotationLine(BL, BR, white);
  annotationLine(BR, FR, white);
}
#endif

