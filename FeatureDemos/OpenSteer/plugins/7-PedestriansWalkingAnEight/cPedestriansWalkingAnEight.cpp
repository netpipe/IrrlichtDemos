/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
#include "cPedestriansWalkingAnEight.h"

//! extern Irrlicht stuff
extern datIrrPointers irrPointers;

// creates a path for the PlugIn
PolylineSegmentedPathwaySingleRadius* getTestPath8();
PolylineSegmentedPathwaySingleRadius* gTestPath8 = NULL;
ObstacleGroup gObstacles8;
Vec3 gEndpoint8_0;
Vec3 gEndpoint8_1;
bool gUseDirectedPathFollowing8 = true;
// this was added for debugging tool, but I might as well leave it in
bool gWanderSwitch8 = true;
AVGroup Pedestrian8::neighbors;

/**
 * Creates a path of the form of an eight. Data provided by Nick Porcino.
 */
PolylineSegmentedPathwaySingleRadius* getTestPath8(){
  if(gTestPath8 == NULL){
    core::array<Vec3> pathPoints;

    pathPoints.push_back(Vec3( -12.678730011f, 0.0144290002063f, 0.523285984993f ));
    pathPoints.push_back(Vec3( -10.447640419f, 0.0149269998074f, -3.44138407707f ));
    pathPoints.push_back(Vec3( -5.88988399506f, 0.0128290001303f, -4.1717581749f ));
    pathPoints.push_back(Vec3( 0.941263973713f, 0.00330199999735f, 0.350513994694f ));
    pathPoints.push_back(Vec3( 5.83484792709f, -0.00407700007781f, 6.56243610382f ));
    pathPoints.push_back(Vec3( 11.0144147873f, -0.0111180003732f, 10.175157547f ));
    pathPoints.push_back(Vec3( 15.9621419907f, -0.0129949999973f, 8.82364273071f ));
    pathPoints.push_back(Vec3( 18.697883606f, -0.0102310003713f, 2.42084693909f ));
    pathPoints.push_back(Vec3( 16.0552558899f, -0.00506500015035f, -3.57153511047f ));
    pathPoints.push_back(Vec3( 10.5450153351f, 0.00284500000998f, -9.92683887482f ));
    pathPoints.push_back(Vec3( 5.88374519348f, 0.00683500012383f, -8.51393127441f ));
    pathPoints.push_back(Vec3( 3.17790007591f, 0.00419700006023f, -2.33129906654f ));
    pathPoints.push_back(Vec3( 1.94371795654f, 0.00101799995173f, 2.78656601906f ));
    pathPoints.push_back(Vec3( -1.04967498779f, 0.000867999973707f, 5.57114219666f ));
    pathPoints.push_back(Vec3( -7.58111476898f, 0.00634300010279f, 6.13661909103f ));
    pathPoints.push_back(Vec3( -12.4111375809f, 0.0108559997752f, 3.5670940876f ));

    gEndpoint8_0 = pathPoints[0];
    gEndpoint8_1 = pathPoints[pathPoints.size()-1];
    gTestPath8 = new PolylineSegmentedPathwaySingleRadius(
                                                          pathPoints.size(),
                                                          &pathPoints[0],
                                                          2,
                                                          false);
  }
  return gTestPath8;
}

// constructor
Pedestrian8::Pedestrian8(ProximityDatabase& pd, irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent)
:ISceneNode(parent,smgr){
  // allocate a token for this boid in the proximity database
  proximityToken = NULL;
  newPD(pd);
  reset();
  //! disable lighting
  Material.Lighting = false;
}
Pedestrian8::~Pedestrian8(){
  delete proximityToken;
}
// reset state
void Pedestrian8::reset(){
  // reset the vehicle
  SimpleVehicle::reset();
  // max speed and max steering force (maneuverability)
  maxSpeed(2.0);
  maxForce(8.0);
  // initially stopped
  speed(0);
  // size of bounding sphere, for obstacle avoidance, etc.
  radius(0.5); // width = 0.7, add 0.3 margin, take half
  // set the path for this Pedestrian to follow
  path = getTestPath8();
  // set initial position
  // (random point on path + random horizontal offset)
  const float d = path->length() * frandom01();
  const float r = path->radius();
  const Vec3 randomOffset = randomVectorOnUnitRadiusXZDisk () * r;
  position(path->mapPathDistanceToPoint (d) + randomOffset);
  // randomize 2D heading
  randomizeHeadingOnXZPlane();
  // pick a random direction for path following (upstream or downstream)
  pathDirection = (frandom01() > 0.5) ? -1 : +1;
  // trail parameters: 3 seconds with 60 points along the trail
  setTrailParameters(3, 60);
  // notify proximity database that our position has changed
  proximityToken->updateForNewPosition(position());
  // load the mesh
  Mesh = getMesh_VehicleDisk();
  Box = Mesh.getBoundingbox();
  setScale(irr::core::vector3df(2.0, 1.0, 2.0));
}
// per frame simulation update
void Pedestrian8::update(const float currentTime, const float elapsedTime){
  // apply steering force to our momentum
  applySteeringForce(determineCombinedSteering (elapsedTime), elapsedTime);
  // reverse direction when we reach an endpoint
  if(gUseDirectedPathFollowing8){
    const Color darkRed(0.7, 0, 0);
    float const pathRadius = path->radius();
    if(Vec3::distance(position(), gEndpoint8_0) < pathRadius){
      pathDirection = +1;
    }
    if(Vec3::distance(position(), gEndpoint8_1) < pathRadius){
      pathDirection = -1;
    }
  }
  // notify proximity database that our position has changed
  proximityToken->updateForNewPosition(position());
  // update Irrlicht node
  setPosition(position().vector3df());
  irr::scene::ISceneNode::setRotation(forward().vector3df().getHorizontalAngle());
  // update motion trail
//  recordTrailVertex(currentTime, position());
}
// draw this character/vehicle into the scene
void Pedestrian8::draw(){
  // Irrlicht does all the work ;)
}

Vec3 Pedestrian8::determineCombinedSteering(const float elapsedTime){
  // move forward
  Vec3 steeringForce = forward();
  // probability that a lower priority behavior will be given a
  // chance to "drive" even if a higher priority behavior might
  // otherwise be triggered.
  const float leakThrough = 0.1f;
  // determine if obstacle avoidance is required
  Vec3 obstacleAvoidance;
  if(leakThrough < frandom01()){
    const float oTime = 6; // minTimeToCollision = 6 seconds
                           // ------------------------------------ xxxcwr11-1-04 fixing steerToAvoid
                           // just for testing
                           //             obstacleAvoidance = steerToAvoidObstacles (oTime, gObstacles8);
                           //             obstacleAvoidance = steerToAvoidObstacle (oTime, gObstacle1);
                           //             obstacleAvoidance = steerToAvoidObstacle (oTime, gObstacle3);
    obstacleAvoidance = steerToAvoidObstacles(oTime, gObstacles8);
  }
  // if obstacle avoidance is needed, do it
  if(obstacleAvoidance != Vec3::zero){
    steeringForce += obstacleAvoidance;
  }else{
    // otherwise consider avoiding collisions with others
    Vec3 collisionAvoidance;
    const float caLeadTime = 3;

    // find all neighbors within maxRadius using proximity database
    // (radius is largest distance between vehicles traveling head-on
    // where a collision is possible within caLeadTime seconds.)
    const float maxRadius = caLeadTime * maxSpeed() * 2;
    neighbors.clear();
    proximityToken->findNeighbors(position(), maxRadius, neighbors);

    if(leakThrough < frandom01())
      collisionAvoidance = steerToAvoidNeighbors(caLeadTime, neighbors) * 10;

    // if collision avoidance is needed, do it
    if(collisionAvoidance != Vec3::zero){
      steeringForce += collisionAvoidance;
    }else{
      // add in wander component (according to user switch)
      if(gWanderSwitch8) steeringForce += steerForWander(elapsedTime);
      // do (interactively) selected type of path following
      const float pfLeadTime = 3;
      const Vec3 pathFollow =
          (gUseDirectedPathFollowing8
             ? steerToFollowPath(pathDirection, pfLeadTime, *path)
             : steerToStayOnPath(pfLeadTime, *path)
           );

      // add in to steeringForce
      steeringForce += pathFollow * 0.5;
    }
  }

  // return steering constrained to global XZ "ground" plane
  return steeringForce.setYtoZero();
}

// switch to new proximity database -- just for demo purposes
void Pedestrian8::newPD(ProximityDatabase& pd){
  // delete this boid's token in the old proximity database
  delete proximityToken;
  // allocate a token for this boid in the proximity database
  proximityToken = pd.allocateToken(this);
}



void Pedestrian8::clearTrailHistory(){
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
void Pedestrian8::recordTrailVertex(const float currentTime, const Vec3& pos){
  // we use our own trail mesh
  Trail.lineIndices.push_back(Trail.lineVertices.size() - 1);
  Trail.lineIndices.push_back(Trail.lineVertices.size());
  irr::video::S3DVertex vert;
  vert.Pos = irr::core::vector3df(pos.x,pos.y,pos.z);
  vert.Color = irr::video::SColor(255, 255,0,0);
  Trail.lineVertices.push_back(vert);
}

// SceneNode
void Pedestrian8::OnRegisterSceneNode(){
  if(IsVisible) irrPointers.SMgr->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}
void Pedestrian8::render(){
  irrPointers.Driver->setMaterial(Material);
  irrPointers.Driver->setTransform(irr::video::ETS_WORLD, irr::scene::ISceneNode::AbsoluteTransformation);
  Mesh.draw(irrPointers.Driver);
  irrPointers.Driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
  Trail.draw(irrPointers.Driver);
}
const irr::core::aabbox3d<irr::f32>& Pedestrian8::getBoundingBox() const{
  return Box;
}
irr::u32 Pedestrian8::getMaterialCount(){
  return 1;
}
irr::video::SMaterial& Pedestrian8::getMaterial(irr::u32 i){
  return Material;
}
void Pedestrian8::setMaterial(irr::video::SMaterial newMaterial){
  Material = newMaterial;
}
