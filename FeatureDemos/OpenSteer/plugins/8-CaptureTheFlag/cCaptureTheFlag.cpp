
#ifdef OPENSTEER
/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
#include "cCaptureTheFlag.h"

//! extern Irrlicht stuff
extern datIrrPointers irrPointers;
//extern int ctfEnemyCount;
extern CtfEnemy* ctfEnemies[maxEnemyCount];
extern irr::core::array<datMesh> lstObstacleMesh;

const int CtfBase::maxObstacleCount = 100;
const Vec3 gHomeBaseCenter(0, 0, 0);
const float gHomeBaseRadius = 1.5;
const float gMinStartRadius = 30;
const float gMaxStartRadius = 40;
const float gBrakingRate = 0.75;
const Color evadeColor(0.6f, 0.6f, 0.3f); // annotation
const Color seekColor(0.3f, 0.6f, 0.6f); // annotation
const Color clearPathColor(0.3f, 0.6f, 0.3f); // annotation
const float gAvoidancePredictTimeMin  = 0.9f;
const float gAvoidancePredictTimeMax  = 2;
float gAvoidancePredictTime = gAvoidancePredictTimeMin;
bool enableAttackSeek  = true; // for testing (perhaps retain for UI control?)
bool enableAttackEvade = true; // for testing (perhaps retain for UI control?)
CtfSeeker* gSeeker = NULL;

// constructor
CtfBase::CtfBase(irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent)
:ISceneNode(parent,smgr){
  reset();
  //! disable lighting
  Material.Lighting = false;
}
CtfBase::~CtfBase(){
}

// reset state
void CtfBase::reset(){
  SimpleVehicle::reset();  // reset the vehicle
  speed(3);             // speed along Forward direction.
  maxForce(3.0);        // steering force is clipped to this magnitude
  maxSpeed(3.0);        // velocity is clipped to this magnitude
  avoiding = false;         // not actively avoiding
  randomizeStartingPositionAndHeading();  // new starting position
  clearTrailHistory();     // prevent long streaks due to teleportation

  // load the mesh
  Mesh = getMesh_VehicleDisk();
  Box = Mesh.getBoundingbox();
  setScale(irr::core::vector3df(0.6, 0.3, 0.6));
}
// per frame simulation update
void CtfBase::update(const float currentTime, const float elapsedTime){
}
// draw this character/vehicle into the scene
void CtfBase::draw(){
  // Irrlicht does all the work ;)
}

void CtfBase::randomizeStartingPositionAndHeading(){
  // randomize position on a ring between inner and outer radii
  // centered around the home base
  const float rRadius = frandom2 (gMinStartRadius, gMaxStartRadius);
  const Vec3 randomOnRing = RandomUnitVectorOnXZPlane () * rRadius;
  position(gHomeBaseCenter + randomOnRing);

  // are we are too close to an obstacle?
  if(minDistanceToObstacle(position()) < radius()*5){
    // if so, retry the randomization (this recursive call may not return
    // if there is too little free space)
    randomizeStartingPositionAndHeading();
  }else{
    // otherwise, if the position is OK, randomize 2D heading
    randomizeHeadingOnXZPlane();
  }
}

void CtfBase::initializeObstacles(){
  // add the Goal (the Flag) at 1st
  datMesh nm = getMesh_VehicleDisk(0.5, 1.0, false);
  nm.setColor(irr::video::SColor(255, 0,150,0));
  lstObstacleMesh.push_back(nm);
  // start with 40% of possible obstacles
  if(obstacleCount == -1){
    obstacleCount = 0;
    for(int i = 0; i < (maxObstacleCount * 0.4); i++) addOneObstacle();
  }
}


void CtfBase::addOneObstacle(){
  if(obstacleCount < maxObstacleCount){
    // pick a random center and radius,
    // loop until no overlap with other obstacles and the home base
    float r;
    Vec3 c;
    float minClearance;
    const float requiredClearance = gSeeker->radius() * 4; // 2 x diameter
    do{
      r = frandom2 (1.5, 4);
      c = randomVectorOnUnitRadiusXZDisk () * gMaxStartRadius * 1.1f;
      minClearance = FLT_MAX;

      for(SOI so = allObstacles.begin(); so != allObstacles.end(); so++){
        testOneObstacleOverlap((**so).radius(), (**so).position());
      }

      testOneObstacleOverlap(gHomeBaseRadius - requiredClearance, gHomeBaseCenter);
    }
    while(minClearance < requiredClearance);

   // add new non-overlapping obstacle to registry
    datMesh nm = getMesh_VehicleDisk(r, 0.2, false);
    nm.Transform.setTranslation(c.vector3df());
    nm.setColor(irr::video::SColor(255, 200,0,0));
    lstObstacleMesh.push_back(nm);

    allObstacles.push_back (new SphereObstacle(r, c));
    obstacleCount++;
  }
}
float CtfBase::minDistanceToObstacle(const Vec3 point){
  float r = 0;
  Vec3 c = point;
  float minClearance = FLT_MAX;
  for(SOI so = allObstacles.begin(); so != allObstacles.end(); so++)
    testOneObstacleOverlap ((**so).radius(), (**so).position());
  return minClearance;
}
void CtfBase::removeOneObstacle(){
  if(obstacleCount > 0){
    obstacleCount--;
    if(lstObstacleMesh.size() > 1) // 1st one is the flag
      lstObstacleMesh.erase(lstObstacleMesh.size()-1);
    delete allObstacles[obstacleCount];
    allObstacles.pop_back();
  }
}

void CtfBase::clearTrailHistory(){
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
void CtfBase::recordTrailVertex(const float currentTime, const Vec3& pos){
  // we use our own trail mesh
  Trail.lineIndices.push_back(Trail.lineVertices.size() - 1);
  Trail.lineIndices.push_back(Trail.lineVertices.size());
  irr::video::S3DVertex vert;
  vert.Pos = irr::core::vector3df(pos.x,pos.y,pos.z);
  vert.Color = irr::video::SColor(255, 255,0,0);
  Trail.lineVertices.push_back(vert);
}

// SceneNode
void CtfBase::OnRegisterSceneNode(){
  if(IsVisible) irrPointers.SMgr->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}
void CtfBase::render(){
  irrPointers.Driver->setMaterial(Material);
  irrPointers.Driver->setTransform(irr::video::ETS_WORLD, irr::scene::ISceneNode::AbsoluteTransformation);
  Mesh.draw(irrPointers.Driver);
  irrPointers.Driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
  Trail.draw(irrPointers.Driver);
}
const irr::core::aabbox3d<irr::f32>& CtfBase::getBoundingBox() const{
  return Box;
}
irr::u32 CtfBase::getMaterialCount(){
  return 1;
}
irr::video::SMaterial& CtfBase::getMaterial(irr::u32 i){
  return Material;
}
void CtfBase::setMaterial(irr::video::SMaterial newMaterial){
  Material = newMaterial;
}


void CtfSeeker::reset(){
  CtfBase::reset();
  Mesh.setColor(irr::video::SColor(255, 90,90,120));
  gSeeker = this;
  state = running;
  evading = false;
}
bool CtfSeeker::clearPathToGoal(){
  const float sideThreshold = radius() * 8.0f;
  const float behindThreshold = radius() * 2.0f;

  const Vec3 goalOffset = gHomeBaseCenter - position();
  const float goalDistance = goalOffset.length ();
  const Vec3 goalDirection = goalOffset / goalDistance;

  const bool goalIsAside = isAside (gHomeBaseCenter, 0.5);

  // for annotation: loop over all and save result, instead of early return
  bool xxxReturn = true;

  // loop over enemies
  for(int i = 0; i < maxEnemyCount; i++){
    // short name for this enemy
    const CtfEnemy& e = *ctfEnemies[i];
    const float eDistance = Vec3::distance (position(), e.position());
    const float timeEstimate = 0.3f * eDistance / e.speed(); //xxx
    const Vec3 eFuture = e.predictFuturePosition (timeEstimate);
    const Vec3 eOffset = eFuture - position();
    const float alongCorridor = goalDirection.dot (eOffset);
    const bool inCorridor = ((alongCorridor > -behindThreshold) &&
                             (alongCorridor < goalDistance));
    const float eForwardDistance = forward().dot (eOffset);

    // xxx temp move this up before the conditionals
    annotationXZCircle (e.radius(), eFuture, clearPathColor, 20); //xxx

    // consider as potential blocker if within the corridor
    if(inCorridor){
      const Vec3 perp = eOffset - (goalDirection * alongCorridor);
      const float acrossCorridor = perp.length();
      if(acrossCorridor < sideThreshold){
        // not a blocker if behind us and we are perp to corridor
        const float eFront = eForwardDistance + e.radius ();

        const bool eIsBehind = eFront < -behindThreshold;
        const bool eIsWayBehind = eFront < (-2 * behindThreshold);
        const bool safeToTurnTowardsGoal =
            ((eIsBehind && goalIsAside) || eIsWayBehind);

        if(!safeToTurnTowardsGoal){
          // this enemy blocks the path to the goal, so return false
          annotationLine (position(), e.position(), clearPathColor);
          // return false;
          xxxReturn = false;
        }
      }
    }
  }
  return xxxReturn;
}
Vec3 CtfSeeker::steerToEvadeAllDefenders(){
  Vec3 evade (0, 0, 0);
  const float goalDistance = Vec3::distance(gHomeBaseCenter, position());

  // sum up weighted evasion
  for(int i = 0; i < maxEnemyCount; i++){
    const CtfEnemy& e = *ctfEnemies[i];
    const Vec3 eOffset = e.position() - position();
    const float eDistance = eOffset.length();

    const float eForwardDistance = forward().dot (eOffset);
    const float behindThreshold = radius() * 2;
    const bool behind = eForwardDistance < behindThreshold;
    if((!behind) || (eDistance < 5)){
      if(eDistance < (goalDistance * 1.2)){
        // const float timeEstimate = 0.5f * eDistance / e.speed;//xxx
        const float timeEstimate = 0.15f * eDistance / e.speed();//xxx
        const Vec3 future = e.predictFuturePosition (timeEstimate);
        annotationXZCircle (e.radius(), future, evadeColor, 20); // xxx
        const Vec3 offset = future - position();
        const Vec3 lateral = offset.perpendicularComponent(forward());
        const float d = lateral.length();
        const float weight = -1000 / (d * d);
        evade += (lateral / d) * weight;
      }
    }
  }
  return evade;
}

Vec3 CtfSeeker::XXXsteerToEvadeAllDefenders(){
  // sum up weighted evasion
  Vec3 evade(0, 0, 0);
  for(int i = 0; i < maxEnemyCount; i++){
    const CtfEnemy& e = *ctfEnemies[i];
    const Vec3 eOffset = e.position() - position();
    const float eDistance = eOffset.length();

    // xxx maybe this should take into account e's heading? xxx
    const float timeEstimate = 0.5f * eDistance / e.speed(); //xxx
    const Vec3 eFuture = e.predictFuturePosition(timeEstimate);

    // annotation
    annotationXZCircle(e.radius(), eFuture, evadeColor, 20);

    // steering to flee from eFuture (enemy's future position)
    const Vec3 flee = steerForFleeEx(eFuture);

    const float eForwardDistance = forward().dot(eOffset);
    const float behindThreshold = radius() * -2;

    const float distanceWeight = 4 / eDistance;
    const float forwardWeight = ((eForwardDistance > behindThreshold) ?
                                 1.0f : 0.5f);

    const Vec3 adjustedFlee = flee * distanceWeight * forwardWeight;

    evade += adjustedFlee;
  }
  return evade;
}
Vec3 CtfSeeker::steeringForSeeker(){
  // determine if obstacle avodiance is needed
  const bool clearPath = clearPathToGoal ();
  adjustObstacleAvoidanceLookAhead (clearPath);
  const Vec3 obstacleAvoidance =
      steerToAvoidObstacles (gAvoidancePredictTime,
                             (ObstacleGroup&) allObstacles);

  // saved for annotation
  avoiding = (obstacleAvoidance != Vec3::zero);

  if(avoiding){
    // use pure obstacle avoidance if needed
    return obstacleAvoidance;
  }else{
    // otherwise seek home base and perhaps evade defenders
    const Vec3 seek = steerForSeekEx(gHomeBaseCenter);
    if(clearPath){
      // we have a clear path (defender-free corridor), use pure seek
      Vec3 s = limitMaxDeviationAngle(seek, 0.707f, forward());
      return s;
    }else{
      if(0){ // xxx testing new evade code xxx
        // combine seek and (forward facing portion of) evasion
        const Vec3 evade = steerToEvadeAllDefenders ();
        const Vec3 steer =
            seek + limitMaxDeviationAngle (evade, 0.5f, forward());

        // annotation: show evasion steering force
        annotationLine (position(),position()+(steer*0.2f),evadeColor);
        return steer;
      }else{
        const Vec3 evade = XXXsteerToEvadeAllDefenders ();
        const Vec3 steer = limitMaxDeviationAngle (seek + evade,
                                                   0.707f, forward());
        return steer;
      }
    }
  }
}
// adjust obstacle avoidance look ahead time: make it large when we are far
// from the goal and heading directly towards it, make it small otherwise.
void CtfSeeker::adjustObstacleAvoidanceLookAhead(const bool clearPath){
  if(clearPath){
    evading = false;
    const float goalDistance = Vec3::distance(gHomeBaseCenter,position());
    const bool headingTowardGoal = isAhead(gHomeBaseCenter, 0.98f);
    const bool isNear = (goalDistance/speed()) < gAvoidancePredictTimeMax;
    const bool useMax = headingTowardGoal && !isNear;
    gAvoidancePredictTime =
        (useMax ? gAvoidancePredictTimeMax : gAvoidancePredictTimeMin);
  }else{
    evading = true;
    gAvoidancePredictTime = gAvoidancePredictTimeMin;
  }
}
void CtfSeeker::updateState(const float currentTime){
  // if we reach the goal before being tagged, switch to atGoal state
  if(state == running){
    const float baseDistance = Vec3::distance(position(),gHomeBaseCenter);
    if(baseDistance < (radius() + gHomeBaseRadius)) state = atGoal;
  }

  // update lastRunningTime (holds off reset time)
  if(state == running){
    lastRunningTime = currentTime;
  }else{
    const float resetDelay = 4;
    const float resetTime = lastRunningTime + resetDelay;
    if (currentTime > resetTime){
      // xxx a royal hack (should do this internal to CTF):
      OpenSteerDemo::queueDelayedResetPlugInXXX ();
    }
  }
}
void CtfSeeker::draw(){
}
// update method for goal seeker
void CtfSeeker::update(const float currentTime, const float elapsedTime){
  // do behavioral state transitions, as needed
  updateState(currentTime);
  // determine and apply steering/braking forces
  Vec3 steer(0, 0, 0);
  if(state == running){
    steer = steeringForSeeker();
  }else{
    applyBrakingForce(gBrakingRate, elapsedTime);
  }
  applySteeringForce(steer, elapsedTime);
  // update Irrlicht node
  setPosition(position().vector3df());
  irr::scene::ISceneNode::setRotation(forward().vector3df().getHorizontalAngle());
}
// SceneNode
void CtfSeeker::OnRegisterSceneNode(){
  if(IsVisible) irrPointers.SMgr->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}
void CtfSeeker::render(){
  irrPointers.Driver->setMaterial(Material);
  irrPointers.Driver->setTransform(irr::video::ETS_WORLD, irr::scene::ISceneNode::AbsoluteTransformation);
  Mesh.draw(irrPointers.Driver);
  irrPointers.Driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
  Trail.draw(irrPointers.Driver);
}
const irr::core::aabbox3d<irr::f32>& CtfSeeker::getBoundingBox() const{
  return Box;
}
irr::u32 CtfSeeker::getMaterialCount(){
  return 1;
}
irr::video::SMaterial& CtfSeeker::getMaterial(irr::u32 i){
  return Material;
}
void CtfSeeker::setMaterial(irr::video::SMaterial newMaterial){
  Material = newMaterial;
}

void CtfEnemy::reset(){
  CtfBase::reset();
  Mesh.setColor(irr::video::SColor(255, 120,90,90));
}
void CtfEnemy::update(const float currentTime, const float elapsedTime){
  // determine upper bound for pursuit prediction time
  const float seekerToGoalDist = Vec3::distance(gHomeBaseCenter, gSeeker->position());
  const float adjustedDistance = seekerToGoalDist - radius() - gHomeBaseRadius;
  const float seekerToGoalTime = ((adjustedDistance < 0 ) ?
                                  0 : (adjustedDistance/gSeeker->speed()));
  const float maxPredictionTime = seekerToGoalTime * 0.9f;

  // determine steering (pursuit, obstacle avoidance, or braking)
  Vec3 steer(0, 0, 0);
  if(gSeeker->state == running){
    const Vec3 avoidance =
        steerToAvoidObstacles(gAvoidancePredictTimeMin,
                               (ObstacleGroup&) allObstacles);
    // saved for annotation
    avoiding = (avoidance == Vec3::zero);
    if(avoiding)
      steer = steerForPursuit(*gSeeker, maxPredictionTime);
    else
      steer = avoidance;
  }else{
    applyBrakingForce(gBrakingRate, elapsedTime);
  }
  applySteeringForce(steer, elapsedTime);

  // detect and record interceptions ("tags") of seeker
  const float seekerToMeDist = Vec3::distance(position(),
                                              gSeeker->position());
  const float sumOfRadii = radius() + gSeeker->radius();
  if(seekerToMeDist < sumOfRadii){
    if(gSeeker->state == running) gSeeker->state = tagged;
  }
  // update Irrlicht node
  setPosition(position().vector3df());
  irr::scene::ISceneNode::setRotation(forward().vector3df().getHorizontalAngle());
}
// SceneNode
void CtfEnemy::OnRegisterSceneNode(){
  if(IsVisible) irrPointers.SMgr->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}
void CtfEnemy::render(){
  irrPointers.Driver->setMaterial(Material);
  irrPointers.Driver->setTransform(irr::video::ETS_WORLD, irr::scene::ISceneNode::AbsoluteTransformation);
  Mesh.draw(irrPointers.Driver);
  irrPointers.Driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
  Trail.draw(irrPointers.Driver);
}
const irr::core::aabbox3d<irr::f32>& CtfEnemy::getBoundingBox() const{
  return Box;
}
irr::u32 CtfEnemy::getMaterialCount(){
  return 1;
}
irr::video::SMaterial& CtfEnemy::getMaterial(irr::u32 i){
  return Material;
}
void CtfEnemy::setMaterial(irr::video::SMaterial newMaterial){
  Material = newMaterial;
}
#endif
