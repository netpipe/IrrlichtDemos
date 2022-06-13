////#ifdef OPENSTEER
///*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
//#include "cPedestrian.h"
//
////! extern Irrlicht stuff
//extern datIrrPointers irrPointers;
//
//PolylineSegmentedPathwaySingleRadius* gTestPath = NULL;
//SphereObstacle gObstacle1;
//SphereObstacle gObstacle2;
//ObstacleGroup gObstacles;
//Vec3 gEndpoint0;
//Vec3 gEndpoint1;
//bool gUseDirectedPathFollowing = true;
//bool gWanderSwitch = true;
//RectangleObstacle gObstacle3(7,7);
//AVGroup Pedestrian::neighbors;
//
//PolylineSegmentedPathwaySingleRadius* getTestPath(){
//// ----------------------------------------------------------------------------
//// create path for PlugIn
////
////
////        | gap |
////
////        f      b
////        |\    /\        -
////        | \  /  \       ^
////        |  \/    \      |
////        |  /\     \     |
////        | /  \     c   top
////        |/    \g  /     |
////        /        /      |
////       /|       /       V      z     y=0
////      / |______/        -      ^
////     /  e      d               |
////   a/                          |
////    |<---out-->|               o----> x
////
//  if(gTestPath == NULL){
//    const float pathRadius = 2;
//
//    const PolylineSegmentedPathwaySingleRadius::size_type pathPointCount = 7;
//    const float size = 30;
//    const float top = 2 * size;
//    const float gap = 1.2f * size;
//    const float out = 2 * size;
//    const float h = 0.5;
//    const Vec3 pathPoints[pathPointCount] =
//        {Vec3 (h+gap-out,     0,  h+top-out),  // 0 a
//         Vec3 (h+gap,         0,  h+top),      // 1 b
//         Vec3 (h+gap+(top/2), 0,  h+top/2),    // 2 c
//         Vec3 (h+gap,         0,  h),          // 3 d
//         Vec3 (h,             0,  h),          // 4 e
//         Vec3 (h,             0,  h+top),      // 5 f
//         Vec3 (h+gap,         0,  h+top/2)};   // 6 g
//
//    gObstacle1.position(interpolate(0.2f, pathPoints[0], pathPoints[1]));
//    gObstacle2.position(interpolate(0.5f, pathPoints[2], pathPoints[3]));
//    gObstacle1.radius(3);
//    gObstacle2.radius(5);
//    gObstacles.push_back(&gObstacle1);
//    gObstacles.push_back(&gObstacle2);
//    gObstacles.push_back(&gObstacle3);
//
//    // tilted 45 degrees
//    gObstacle3.forward(Vec3(1,1,0).normalize());
//    gObstacle3.side(0,0,1);
//    gObstacle3.up(Vec3(-1,1,0).normalize());
//    gObstacle3.position(20, 0, h);
//
//    gObstacle3.setSeenFrom(Obstacle::both);
//
//    gEndpoint0 = pathPoints[0];
//    gEndpoint1 = pathPoints[pathPointCount-1];
//
//    gTestPath = new PolylineSegmentedPathwaySingleRadius(pathPointCount, pathPoints, pathRadius, false);
//  }
//  return gTestPath;
//}
//
//// constructor
//Pedestrian::Pedestrian(ProximityDatabase& pd, irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent)
//:ISceneNode(parent,smgr){
//  // allocate a token for this boid in the proximity database
//  proximityToken = NULL;
//  newPD(pd);
//  // reset Pedestrian state
//  reset();
//  //! disable lighting
//  Material.Lighting = false;
//}
//Pedestrian::~Pedestrian(){
//  // delete this boid's token in the proximity database
//  delete proximityToken;
//}
//
//// reset state
//void Pedestrian::reset(){
//  // reset the vehicle
//  SimpleVehicle::reset();
//  // max speed and max steering force (maneuverability)
//  maxSpeed(2.0);
//  maxForce(8.0);
//  // initially stopped
//  speed(0);
//  // size of bounding sphere, for obstacle avoidance, etc.
//  radius(0.5); // width = 0.7, add 0.3 margin, take half
//  // set the path for this Pedestrian to follow
//  path = getTestPath();
//  // set initial position
//  // (random point on path + random horizontal offset)
//  const float d = path->length() * frandom01();
//  const float r = path->radius();
//  const Vec3 randomOffset = randomVectorOnUnitRadiusXZDisk() * r;
//  position(path->mapPathDistanceToPoint(d) + randomOffset);
//
//  // randomize 2D heading
//  randomizeHeadingOnXZPlane();
//  // pick a random direction for path following (upstream or downstream)
//  pathDirection = (frandom01() > 0.5) ? -1 : +1;
//  // trail parameters: 3 seconds with 60 points along the trail
////  setTrailParameters(3, 60);
//  // notify proximity database that our position has changed
//  proximityToken->updateForNewPosition(position());
//  // load the mesh
//  Mesh = getMesh_VehicleDisk();
//  Box = Mesh.getBoundingbox();
//  setScale(irr::core::vector3df(0.6, 0.3, 0.6));
//}
//// per frame simulation update
//void Pedestrian::update(const float currentTime, const float elapsedTime){
//  // apply steering force to our momentum
//  applySteeringForce(determineCombinedSteering (elapsedTime), elapsedTime);
//  // reverse direction when we reach an endpoint
//  if(gUseDirectedPathFollowing){
//    const Color darkRed (0.7f, 0, 0);
//    float const pathRadius = path->radius();
//    if(Vec3::distance(position(), gEndpoint0) < pathRadius){
//      pathDirection = +1;
/////      annotationXZCircle(pathRadius, gEndpoint0, darkRed, 20);
//    }
//    if(Vec3::distance(position(), gEndpoint1) < pathRadius){
//      pathDirection = -1;
//  ///    annotationXZCircle(pathRadius, gEndpoint1, darkRed, 20);
//    }
//  }
//  // notify proximity database that our position has changed
//  proximityToken->updateForNewPosition(position());
//  // update Irrlicht node
//  setPosition(position().vector3df());
//  irr::scene::ISceneNode::setRotation(forward().vector3df().getHorizontalAngle());
//  // update motion trail
////  recordTrailVertex(currentTime, position());
//}
//// draw this character/vehicle into the sAVGroup Pedestrian::neighbors;
//void Pedestrian::draw(){
//  // Irrlicht does all the work ;)
//}
//
//void Pedestrian::clearTrailHistory(){
//  // we use our own trail mesh
//  Trail.faceIndices.clear();
//  Trail.faceVertices.clear();
//  Trail.lineIndices.clear();
//  Trail.lineVertices.clear();
//  // create initial vertex
//  irr::video::S3DVertex vert;
//  vert.Pos = position().vector3df();
//  vert.Color = irr::video::SColor(255, 255,0,0);
//  Trail.lineVertices.push_back(vert);
//
//}
//void Pedestrian::recordTrailVertex(const float currentTime, const Vec3& pos){
//  // we use our own trail mesh
//  Trail.lineIndices.push_back(Trail.lineVertices.size() - 1);
//  Trail.lineIndices.push_back(Trail.lineVertices.size());
//  irr::video::S3DVertex vert;
//  vert.Pos = irr::core::vector3df(pos.x,pos.y,pos.z);
//  vert.Color = irr::video::SColor(255, 255,0,0);
//  Trail.lineVertices.push_back(vert);
//}
//
//// SceneNode
//void Pedestrian::OnRegisterSceneNode(){
//  if(IsVisible) irrPointers.SMgr->registerNodeForRendering(this);
//  ISceneNode::OnRegisterSceneNode();
//}
//void Pedestrian::render(){
//  irrPointers.Driver->setMaterial(Material);
//  irrPointers.Driver->setTransform(irr::video::ETS_WORLD, irr::scene::ISceneNode::AbsoluteTransformation);
//  Mesh.draw(irrPointers.Driver);
//  irrPointers.Driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
//  Trail.draw(irrPointers.Driver);
//}
//const irr::core::aabbox3d<irr::f32>& Pedestrian::getBoundingBox() const{
//  return Box;
//}
//irr::u32 Pedestrian::getMaterialCount(){
//  return 1;
//}
//irr::video::SMaterial& Pedestrian::getMaterial(irr::u32 i){
//  return Material;
//}
//void Pedestrian::setMaterial(irr::video::SMaterial newMaterial){
//  Material = newMaterial;
//}
//
//// compute combined steering force: move forward, avoid obstacles
//// or neighbors if needed, otherwise follow the path and wander
//Vec3 Pedestrian::determineCombinedSteering(const float elapsedTime){
//  // move forward
//  Vec3 steeringForce = forward();
//
//  // probability that a lower priority behavior will be given a
//  // chance to "drive" even if a higher priority behavior might
//  // otherwise be triggered.
//  const float leakThrough = 0.1f;
//
//  // determine if obstacle avoidance is required
//  Vec3 obstacleAvoidance;
//  if(leakThrough < frandom01()){
//    const float oTime = 6; // minTimeToCollision = 6 seconds
//// ------------------------------------ xxxcwr11-1-04 fixing steerToAvoid
//// just for testing
////             obstacleAvoidance = steerToAvoidObstacles (oTime, gObstacles);
////             obstacleAvoidance = steerToAvoidObstacle (oTime, gObstacle1);
////             obstacleAvoidance = steerToAvoidObstacle (oTime, gObstacle3);
//    obstacleAvoidance = steerToAvoidObstacles(oTime, gObstacles);
//// ------------------------------------ xxxcwr11-1-04 fixing steerToAvoid
//  }
//
//  // if obstacle avoidance is needed, do it
//  if(obstacleAvoidance != Vec3::zero){
//    steeringForce += obstacleAvoidance;
//  }else{
//    // otherwise consider avoiding collisions with others
//    Vec3 collisionAvoidance;
//    const float caLeadTime = 3;
//
//    // find all neighbors within maxRadius using proximity database
//    // (radius is largest distance between vehicles traveling head-on
//    // where a collision is possible within caLeadTime seconds.)
//    const float maxRadius = caLeadTime * maxSpeed() * 2;
//    neighbors.clear();
//    proximityToken->findNeighbors (position(), maxRadius, neighbors);
//
//    if(leakThrough < frandom01())
//      collisionAvoidance = steerToAvoidNeighbors(caLeadTime, neighbors) * 10;
//
//    // if collision avoidance is needed, do it
//    if(collisionAvoidance != Vec3::zero){
//      steeringForce += collisionAvoidance;
//    }else{
//      // add in wander component (according to user switch)
//      if(gWanderSwitch) steeringForce += steerForWander (elapsedTime);
//      // do (interactively) selected type of path following
//      const float pfLeadTime = 3;
//      const Vec3 pathFollow =
//          (gUseDirectedPathFollowing ?
//           steerToFollowPath(pathDirection, pfLeadTime, *path) :
//           steerToStayOnPath(pfLeadTime, *path));
//      // add in to steeringForce
//      steeringForce += pathFollow * 0.5;
//    }
//  }
//  // return steering constrained to global XZ "ground" plane
//  return steeringForce.setYtoZero();
//}
//
//
//// switch to new proximity database -- just for demo purposes
//void Pedestrian::newPD(ProximityDatabase& pd){
//  // delete this boid's token in the old proximity database
//  delete proximityToken;
//  // allocate a token for this boid in the proximity database
//  proximityToken = pd.allocateToken (this);
//}
//
//#endif
//
