/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
#include "cMapDrive.h"

//! extern Irrlicht stuff
extern datIrrPointers irrPointers;
extern irr::core::array<datMesh> lstObstacleMesh;

// define map size (and compute its half diagonal)
float MapDriver::worldSize = 200;
float MapDriver::worldDiag = sqrtXXX (square (worldSize) / 2);

// 0 = obstacle avoidance and speed control
// 1 = wander, obstacle avoidance and speed control
// 2 = path following, obstacle avoidance and speed control
// int MapDriver::demoSelect = 0;
int MapDriver::demoSelect = 2;

float MapDriver::savedNearestWR = 0;
float MapDriver::savedNearestR = 0;
float MapDriver::savedNearestL = 0;
float MapDriver::savedNearestWL = 0;

// constructor
MapDriver::MapDriver(irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent)
:ISceneNode(parent,smgr), map(makeMap()), path(makePath()){
  reset();
  //! disable lighting
  Material.Lighting = false;
}
MapDriver::~MapDriver(){
  delete (map);
  delete (path);
}

// reset state
void MapDriver::reset(){
  // reset the underlying vehicle class
  SimpleVehicle::reset();
  // initially stopped
  speed(0);
  // Assume top speed is 20 meters per second (44.7 miles per hour).
  // This value will eventually be supplied by a higher level module.
  maxSpeed(20);
  // steering force is clipped to this magnitude
  maxForce(maxSpeed() * 0.4f);
  // vehicle is 2 meters wide and 3 meters long
  halfWidth = 1.0f;
  halfLength = 1.5f;
  // init dynamically controlled radius
  adjustVehicleRadiusForSpeed();
  pathFollowDirection = 1;
  // not previously avoiding
  annotateAvoid = Vec3::zero;
  // first pass at detecting "stuck" state
  stuck = false;
  // QQQ need to clean up this hack
  qqqLastNearestObstacle = Vec3::zero;
  // master look ahead (prediction) time
  baseLookAheadTime = 3;
  if(demoSelect == 2){
    lapsStarted++;
    const float s = worldSize;
    const float d = (float)pathFollowDirection;
    position(Vec3 (s * d * 0.6f, 0, s * -0.4f));
    regenerateOrthonormalBasisUF(Vec3::side * d);
  }
  // reset bookeeping to detect stuck cycles
  resetStuckCycleDetection();
  // assume no previous steering
  currentSteering = Vec3::zero;
  // assume normal running state
  dtZero = false;
  // QQQ temporary global QQQoaJustScraping
  QQQoaJustScraping = false;
  clearTrailHistory();

  map = makeMap();
  //! create mesh for path way
  lstObstacleMesh.clear();
  {datMesh nm = getMesh_Path((PolylineSegmentedPathwaySegmentRadii*)path);
  nm.setColor(irr::video::SColor(255, 0,200,0));
  lstObstacleMesh.push_back(nm);}
  //! create mesh for obstacles
  {datMesh nm = map->getDrawMap();
  nm.setColor(irr::video::SColor(255, 130,50,50));
  lstObstacleMesh.push_back(nm);}

  // load the mesh
  Mesh = getMesh_VehicleDisk();
  Box = Mesh.getBoundingbox();
  setScale(irr::core::vector3df(2.0, 1.0, 2.0));
}
// per frame simulation update
void MapDriver::update(const float currentTime, const float elapsedTime){
  // take note when current dt is zero (as in paused) for stat counters
  dtZero = (elapsedTime == 0);
  // pretend we are bigger when going fast
  adjustVehicleRadiusForSpeed();
  // determine combined steering
  Vec3 steering;
  const bool offPath = !bodyInsidePath();
  if(stuck || offPath || detectImminentCollision()){
    // bring vehicle to a stop if we are stuck (newly or previously
    // stuck, because off path or collision seemed imminent)
    // (QQQ combine with stuckCycleCount code at end of this function?)
//          applyBrakingForce (curvedSteering ? 3 : 2, elapsedTime); // QQQ
    applyBrakingForce((curvedSteering?3.0f:2.0f), elapsedTime); // QQQ
    // count "off path" events
    if (offPath && !stuck && (demoSelect == 2)) stuckOffPathCount++;
    stuck = true;

    // QQQ trying to prevent "creep" during emergency stops
    resetSmoothedAcceleration();
    currentSteering = Vec3::zero;
  }else{
    // determine steering for obstacle avoidance (save for annotation)
    const Vec3 avoid = annotateAvoid =
        steerToAvoidObstaclesOnMap(lookAheadTimeOA (),
                                   *map,
                                   hintForObstacleAvoidance ());
    const bool needToAvoid = avoid != Vec3::zero;

    // any obstacles to avoid?
    if(needToAvoid){
      // slow down and turn to avoid the obstacles
      const float targetSpeed = ((curvedSteering && QQQoaJustScraping)
                                ? maxSpeedForCurvature () : 0);
      annoteMaxRelSpeed = targetSpeed / maxSpeed();
      const float avoidWeight = 3 + (3 * relativeSpeed()); // ad hoc
      steering = avoid * avoidWeight;
      steering += steerForTargetSpeed(targetSpeed);
    }else{
      // otherwise speed up and...
      steering = steerForTargetSpeed (maxSpeedForCurvature ());

      // wander for demo 1
      if(demoSelect == 1){
        const Vec3 wander = steerForWander(elapsedTime);
        const Vec3 flat = wander.setYtoZero();
        const Vec3 weighted = flat.truncateLength(maxForce()) * 6;
        const Vec3 a = position() + Vec3(0, 0.2f, 0);
        steering += weighted;
      }

      // follow the path in demo 2
      if(demoSelect == 2){
        const Vec3 pf = steerToFollowPath(pathFollowDirection, lookAheadTimePF(), *path);
        if(pf != Vec3::zero){
          // steer to remain on path
          if(pf.dot (forward()) < 0)
            steering = pf;
          else
            steering = pf + steering;
        }else{
          // path aligment: when neither obstacle avoidance nor
          // path following is required, align with path segment
          const Vec3 pathHeading = mapPointAndDirectionToTangent(*path, position(), pathFollowDirection);
          steering += (steerTowardHeading(pathHeading) *
                      (isNearWaypoint(*path, position()) /* path->nearWaypoint (position () ) */
                       ? 0.5f : 0.1f));
        }
      }
    }
  }

  if(!stuck){
    // convert from absolute to incremental steering signal
    if(incrementalSteering)
      steering = convertAbsoluteToIncrementalSteering(steering, elapsedTime);
    // enforce minimum turning radius
    steering = adjustSteeringForMinimumTurningRadius(steering);
  }

  // apply selected steering force to vehicle, record data
  applySteeringForce(steering, elapsedTime);
  collectReliabilityStatistics(currentTime, elapsedTime);

  // detect getting stuck in cycles -- we are moving but not
  // making progress down the route (annotate smoothedPosition)
  if(demoSelect == 2){
    const bool circles = weAreGoingInCircles();
    if(circles && !stuck) stuckCycleCount++;
    if(circles) stuck = true;
  }

  // update Irrlicht node
  setPosition(position().vector3df());
  irr::scene::ISceneNode::setRotation(forward().vector3df().getHorizontalAngle());
  // update motion trail
  recordTrailVertex(currentTime, position());
}
// draw this character/vehicle into the scene
void MapDriver::draw(){
  // Irrlicht does all the work ;)
}

void MapDriver::adjustVehicleRadiusForSpeed(){
  const float minRadius = sqrtXXX(square(halfWidth)+square(halfLength));
  const float safetyMargin = (curvedSteering ?
                              interpolate (relativeSpeed(), 0.0f, 1.5f) : 0.0f);
  radius(minRadius + safetyMargin);
}
void MapDriver::collectReliabilityStatistics(const float currentTime, const float elapsedTime){
  // detect obstacle avoidance failure and keep statistics
  collisionDetected = map->scanLocalXZRectangle(*this,
                                                -halfWidth, halfWidth,
                                                -halfLength,halfLength);

  // record stats to compute mean time between collisions
  const float timeSinceLastCollision = currentTime - timeOfLastCollision;
  if(collisionDetected && !collisionLastTime && (timeSinceLastCollision > 1)){
    sumOfCollisionFreeTimes += timeSinceLastCollision;
    countOfCollisionFreeTimes++;
    timeOfLastCollision = currentTime;
  }
  collisionLastTime = collisionDetected;

  // keep track of average speed
  totalDistance += speed () * elapsedTime;
  totalTime += elapsedTime;

  // keep track of path following failure rate
  // QQQ for now, duplicating this code from the draw method:
  // if we are following a path but are off the path,
  // draw a red line to where we should be
  if(demoSelect == 2){
    pathFollowTime += elapsedTime;
    if(!bodyInsidePath()) pathFollowOffTime += elapsedTime;
  }
}
Vec3 MapDriver::hintForObstacleAvoidance(){
  // used only when path following, return zero ("no hint") otherwise
  if(demoSelect != 2) return Vec3::zero;

  // are we heading roughly parallel to the current path segment?
  const Vec3 p = position();
  const Vec3 pathHeading = mapPointAndDirectionToTangent(*path, p, pathFollowDirection);
  if(pathHeading.dot (forward ()) < 0.8f){
    // if not, the "hint" is to turn to align with path heading
    const Vec3 s = side () * halfWidth;
    const float f = halfLength * 2;
    return pathHeading;
  }else{
    // when there is a valid nearest obstacle position
    const Vec3 obstacle = qqqLastNearestObstacle;
    const Vec3 o = obstacle + (up () * 0.1f);
    if(obstacle != Vec3::zero){
      // get offset, distance from obstacle to its image on path
      float outside;
      const Vec3 onPath = mapPointToPointOnCenterLineAndOutside( *path, obstacle, outside );// path->mapPointToPath (obstacle, outside);
      const Vec3 offset = onPath - obstacle;
      const float offsetDistance = offset.length();

      // when the obstacle is inside the path tube
      if(outside < 0){
        // when near the outer edge of a sufficiently wide tube
        // const int segmentIndex = path->indexOfNearestSegment (onPath);
        // const float segmentRadius = path->segmentRadius( segmentIndex );
        float const segmentRadius = mapPointToRadius( *path, onPath );
        const float w = halfWidth * 6;
        const bool nearEdge = offsetDistance > w;
        const bool wideEnough = segmentRadius > (w * 2);
        if(nearEdge && wideEnough){
          const float obstacleDistance = (obstacle - p).length();
          const float range = speed () * lookAheadTimeOA();
          const float farThreshold = range * 0.8f;
          const bool usableHint = obstacleDistance>farThreshold;
          if(usableHint){
            const Vec3 q = p + (offset.normalize() * 5);
            return offset;
          }
        }
      }
    }
  }
  // otherwise, no hint
  return Vec3::zero;
}

// like steerToAvoidObstacles, but based on a BinaryTerrainMap indicating
// the possitions of impassible regions
//
Vec3 MapDriver::steerToAvoidObstaclesOnMap(const float minTimeToCollision, const TerrainMap& map){
  return steerToAvoidObstaclesOnMap(minTimeToCollision, map, Vec3::zero); // no steer hint
}
// given a map of obstacles (currently a global, binary map) steer so as
// to avoid collisions within the next minTimeToCollision seconds.
//
Vec3 MapDriver::steerToAvoidObstaclesOnMap(const float minTimeToCollision, const TerrainMap& map,  const Vec3& steerHint){
  const float spacing = map.minSpacing() / 2;
  const float maxSide = radius();
  const float maxForward = minTimeToCollision * speed();
  const int maxSamples = (int)(maxForward / spacing);
  const Vec3 step = forward() * spacing;
  const Vec3 fOffset = position();
  Vec3 sOffset;
  float s = spacing / 2;

  const int infinity = 9999; // qqq
  int nearestL = infinity;
  int nearestR = infinity;
  int nearestWL = infinity;
  int nearestWR = infinity;
  Vec3 nearestO;
  wingDrawFlagL = false;
  wingDrawFlagR = false;

  const bool hintGiven = steerHint != Vec3::zero;
  if (hintGiven && !dtZero) hintGivenCount++;

  // QQQ temporary global QQQoaJustScraping
  QQQoaJustScraping = true;

  const float signedRadius = 1 / nonZeroCurvatureQQQ ();
  const Vec3 localCenterOfCurvature = side () * signedRadius;
  const Vec3 center = position () + localCenterOfCurvature;
  const float sign = signedRadius < 0 ? 1.0f : -1.0f;
  const float arcRadius = signedRadius * -sign;
  const float twoPi = 2 * OPENSTEER_M_PI;
  const float circumference = twoPi * arcRadius;
  const float rawLength = speed() * minTimeToCollision * sign;
  const float fracLimit = 1.0f / 6.0f;
  const float distLimit = circumference * fracLimit;
  const float arcLength = arcLengthLimit (rawLength, distLimit);
  const float arcAngle = twoPi * arcLength / circumference;

  // XXX temp annotation to show limit on arc angle
  if(curvedSteering){
    if((speed() * minTimeToCollision) > (circumference * fracLimit)){
      const float q = twoPi * fracLimit;
      const Vec3 fooz = position () - center;
      const Vec3 booz = fooz.rotateAboutGlobalY (sign * q);
      annotationLine (center, center + fooz, gRed);
      annotationLine (center, center + booz, gRed);
    }
  }

  // assert loops will terminate
  assert(spacing > 0);

  // scan corridor straight ahead of vehicle,
  // keep track of nearest obstacle on left and right sides
  while(s < maxSide){
    sOffset = side() * s;
    s += spacing;
    const Vec3 lOffset = fOffset + sOffset;
    const Vec3 rOffset = fOffset - sOffset;

    Vec3 lObsPos, rObsPos;
    const int L = (curvedSteering ?
                   (int)(scanObstacleMap(lOffset, center, arcAngle, maxSamples, 0, gYellow, gRed, lObsPos) / spacing)
                   : map.scanXZray(lOffset, step, maxSamples));
    const int R = (curvedSteering ?
                   (int)(scanObstacleMap(rOffset, center, arcAngle, maxSamples, 0, gYellow, gRed, rObsPos) / spacing)
                   : map.scanXZray(rOffset, step, maxSamples));

    if((L > 0) && (L < nearestL)){
      nearestL = L;
      if(L < nearestR) nearestO = ((curvedSteering) ?
                                    lObsPos : lOffset + ((float)L * step));
    }
    if((R > 0) && (R < nearestR)){
      nearestR = R;
      if(R < nearestL) nearestO = ((curvedSteering) ?
                                   rObsPos : rOffset + ((float)R * step));
    }

    if(curvedSteering){
      // QQQ temporary global QQQoaJustScraping
      const bool outermost = s >= maxSide;
      const bool eitherSide = (L > 0) || (R > 0);
      if(!outermost && eitherSide) QQQoaJustScraping = false;
    }
  }
  qqqLastNearestObstacle = nearestO;

  // scan "wings"
  {
    const int wingScans = 4;
    // see duplicated code at: QQQ draw sensing "wings"
    // QQQ should be a parameter of this method
    const Vec3 wingWidth = side() * wingSlope() * maxForward;

    const Color beforeColor(0.75f, 0.9f, 0.0f);  // for annotation
    const Color afterColor(0.9f,  0.5f, 0.0f);  // for annotation

    for(int i=1; i<=wingScans; i++){
      const float fraction = (float)i / (float)wingScans;
      const Vec3 endside = sOffset + (wingWidth * fraction);
      const Vec3 corridorFront = forward() * maxForward;

      // "loop" from -1 to 1
      for(int j = -1; j < 2; j += 2){
        float k = (float)j; // prevent VC7.1 warning
        const Vec3 start = fOffset + (sOffset * k);
        const Vec3 end = fOffset + corridorFront + (endside * k);
        const Vec3 ray = end - start;
        const float rayLength = ray.length();
        const Vec3 step = ray * spacing / rayLength;
        const int raySamples = (int)(rayLength / spacing);
        const float endRadius =
            wingSlope () * maxForward * fraction *
            (signedRadius < 0 ? 1 : -1) * (j==1?1:-1);
        Vec3 ignore;
        const int scan = (curvedSteering ?
                          (int)(scanObstacleMap(start,
                                                center,
                                                arcAngle,
                                                raySamples,
                                                endRadius,
                                                beforeColor,
                                                afterColor,
                                                ignore)
                                / spacing) : map.scanXZray (start, step, raySamples));

        if(j == 1){
          if((scan > 0) && (scan < nearestWL)) nearestWL = scan;
        }else{
          if((scan > 0) && (scan < nearestWR)) nearestWR = scan;
        }
      }
    }
    wingDrawFlagL = nearestWL != infinity;
    wingDrawFlagR = nearestWR != infinity;
  }

  // for annotation
  savedNearestWR = (float)nearestWR;
  savedNearestR  = (float)nearestR;
  savedNearestL  = (float)nearestL;
  savedNearestWL = (float)nearestWL;

  // flags for compound conditions, used below
  const bool obstacleFreeC  = nearestL==infinity && nearestR==infinity;
  const bool obstacleFreeL  = nearestL==infinity && nearestWL==infinity;
  const bool obstacleFreeR  = nearestR==infinity && nearestWR==infinity;
  const bool obstacleFreeWL = nearestWL==infinity;
  const bool obstacleFreeWR = nearestWR==infinity;
  const bool obstacleFreeW  = obstacleFreeWL && obstacleFreeWR;

  // when doing curved steering and we have already detected "just
  // scarping" but neither wing is free, recind the "just scarping"
  // QQQ temporary global QQQoaJustScraping
  const bool JS = curvedSteering && QQQoaJustScraping;
  const bool cancelJS = !obstacleFreeWL && !obstacleFreeWR;
  if(JS && cancelJS) QQQoaJustScraping = false;


  // ----------------------------------------------------------
  // now we have measured everything, decide which way to steer
  // ----------------------------------------------------------

  // no obstacles found on path, return zero steering
  if(obstacleFreeC){
    qqqLastNearestObstacle = Vec3::zero;

    // qqq  this may be in the wrong place (what would be the right
    // qqq  place?!) but I'm trying to say "even if the path is
    // qqq  clear, don't go too fast when driving between obstacles
    if (obstacleFreeWL || obstacleFreeWR || relativeSpeed () < 0.7f)
      return Vec3::zero;
    else
      return -forward();
  }

  // if the nearest obstacle is way out there, take hint if any
//      if (hintGiven && (minXXX (nearestL, nearestR) > (maxSamples * 0.8f)))
  if (hintGiven && (minXXX ((float)nearestL, (float)nearestR) > (maxSamples * 0.8f))){
    if(steerHint.dot(side())>0)
      return side();
    else
      return -side();
  }

  // QQQ experiment 3-9-04
  //
  // since there are obstacles ahead, if we are already near
  // maximum curvature, we MUST turn in opposite direction
  //
  // are we turning more sharply than the minimum turning radius?
  // (code from adjustSteeringForMinimumTurningRadius)
  const float maxCurvature = 1 / (minimumTurningRadius() * 1.2f);
  if(absXXX (curvature ()) > maxCurvature){
    return side () * sign;
  }

  if(obstacleFreeL) return side();
  if(obstacleFreeR) return -side();

  // if wings are clear, turn away from nearest obstacle straight ahead
  if(obstacleFreeW){
    // distance to obs on L and R side of corridor roughtly the same
    const bool same = absXXX(nearestL - nearestR) < 5; // within 5
    // if they are about the same and a hint is given, use hint
    if(same && hintGiven){
      if(steerHint.dot(side()) > 0)
        return side();
      else
        return -side();
    }else{
      // otherwise steer toward the less cluttered side
      if(nearestL > nearestR)
        return side();
      else
        return -side();
    }
  }

  // if the two wings are about equally clear and a steering hint is
  // provided, use it
  const bool equallyClear = absXXX (nearestWL-nearestWR) < 2; // within 2
  if(equallyClear && hintGiven){
    if (steerHint.dot(side()) > 0)
      return side();
    else
      return -side();
  }

  // turn towards the side whose "wing" region is less cluttered
  // (the wing whose nearest obstacle is furthest away)
  if(nearestWL > nearestWR)
    return side();
  else
    return -side();
}

// scan across the obstacle map along a given arc
// (possibly with radius adjustment ramp)
// returns approximate distance to first obstacle found
//
// QQQ 1: this calling sequence does not allow for zero curvature case
// QQQ 2: in library version of this, "map" should be a parameter
// QQQ 3: instead of passing in colors, call virtual annotation function?
// QQQ 4: need flag saying to continue after a hit, for annotation
// QQQ 5: I needed to return both distance-to and position-of the first
//        obstacle. I added returnObstaclePosition but maybe this should
//        return a "scan results object" with a flag for obstacle found,
//        plus distant and position if so.
//
float MapDriver::scanObstacleMap(const Vec3& start,
                      const Vec3& center,
                      const float arcAngle,
                      const int segments,
                      const float endRadiusChange,
                      const Color& beforeColor,
                      const Color& afterColor,
                      Vec3& returnObstaclePosition){
  // "spoke" is initially the vector from center to start,
  // which is then rotated step by step around center
  Vec3 spoke = start - center;
  // determine the angular step per segment
  const float step = arcAngle / segments;
  // store distance to, and position of first obstacle
  float obstacleDistance = 0;
  returnObstaclePosition = Vec3::zero;
  // for spiral "ramps" of changing radius
  const float startRadius = (endRadiusChange == 0) ? 0 : spoke.length();

  // traverse each segment along arc
  float sin=0, cos=0;
  Vec3 oldPoint = start;
  bool obstacleFound = false;
  for(int i = 0; i < segments; i++){
    // rotate "spoke" to next step around circle
    // (sin and cos values get filled in on first call)
    spoke = spoke.rotateAboutGlobalY (step, sin, cos);

    // for spiral "ramps" of changing radius
    const float adjust = ((endRadiusChange == 0) ? 1.0f :
                          interpolate((float)(i+1) / (float)segments, 1.0f,
                          (maxXXX(0, (startRadius + endRadiusChange)) / startRadius)));

    // construct new scan point: center point, offset by rotated
    // spoke (possibly adjusting the radius if endRadiusChange!=0)
    const Vec3 newPoint = center + (spoke * adjust);

    // once an obstacle if found "our work here is done" -- continue
    // to loop only for the sake of annotation (make that optional?)
    if(!obstacleFound){
      // no obstacle found on this scan so far,
      // scan map along current segment (a chord of the arc)
      const Vec3 offset = newPoint - oldPoint;
      const float d2 = offset.length() * 2;

      // when obstacle found: set flag, save distance and position
      if(! map->isPassable (newPoint)){
        obstacleFound = true;
        obstacleDistance = d2 * 0.5f * (i+1);
        returnObstaclePosition = newPoint;
      }
    }
    // save new point for next time around loop
    oldPoint = newPoint;
  }
  // return distance to first obstacle (or zero if none found)
  return obstacleDistance;
}

bool MapDriver::detectImminentCollision(){
  // QQQ  this should be integrated into steerToAvoidObstaclesOnMap
  // QQQ  since it shares so much infrastructure
  // QQQ  less so after changes on 3-16-04
  bool returnFlag = false;
  const float spacing = map->minSpacing() / 2;
  const float maxSide = halfWidth + spacing;
  const float minDistance = curvedSteering ? 2.0f : 2.5f; // meters
  const float predictTime = curvedSteering ? .75f : 1.3f; // seconds
  const float maxForward =
      speed () * combinedLookAheadTime(predictTime, minDistance);
  const Vec3 step = forward () * spacing;
  float s = curvedSteering ? (spacing / 4) : (spacing / 2);

  const float signedRadius = 1 / nonZeroCurvatureQQQ ();
  const Vec3 localCenterOfCurvature = side () * signedRadius;
  const Vec3 center = position() + localCenterOfCurvature;
  const float sign = signedRadius < 0 ? 1.0f : -1.0f;
  const float arcRadius = signedRadius * -sign;
  const float twoPi = 2 * OPENSTEER_M_PI;
  const float circumference = twoPi * arcRadius;
  const Vec3 qqqLift(0, 0.2f, 0);
  Vec3 ignore;

  // scan region ahead of vehicle
  while(s < maxSide){
    const Vec3 sOffset = side() * s;
    const Vec3 lOffset = position () + sOffset;
    const Vec3 rOffset = position () - sOffset;
    const float bevel = 0.3f;
    const float fraction = s / maxSide;
    const float scanDist = (halfLength +
                            interpolate (fraction,
                                         maxForward,
                                         maxForward * bevel));
    const float angle = (scanDist * twoPi * sign) / circumference;
    const int samples = (int)(scanDist / spacing);
    const int L = (curvedSteering ?
                   (int)(scanObstacleMap (lOffset + qqqLift,
                                          center,
                                          angle,
                                          samples,
                                          0,
                                          gMagenta,
                                          gCyan,
                                          ignore)
                          / spacing) : map->scanXZray (lOffset, step, samples));
    const int R = (curvedSteering ?
                   (int)(scanObstacleMap (rOffset + qqqLift,
                                          center,
                                          angle,
                                          samples,
                                          0,
                                          gMagenta,
                                          gCyan,
                                          ignore)
                          / spacing) : map->scanXZray (rOffset, step, samples));

    returnFlag = returnFlag || (L > 0);
    returnFlag = returnFlag || (R > 0);

    // annotation
    if(!curvedSteering){
      const Vec3 d(step * (float)samples);
      annotationLine(lOffset, lOffset + d, gWhite);
      annotationLine(rOffset, rOffset + d, gWhite);
    }

    // increment sideways displacement of scan line
    s += spacing;
  }
  return returnFlag;
}
// see comments at SimpleVehicle::predictFuturePosition, in this instance
// I just need the future position (not a LocalSpace), so I'll keep the
// calling sequence and just conditionalize its body
//
// this should be const, but easier for now to ignore that
Vec3 MapDriver::predictFuturePosition(const float predictionTime) const{
  if(curvedSteering){
    // QQQ this chunk of code is repeated in far too many places,
    // QQQ it has to be moved inside some utility
    // QQQ
    // QQQ and now, worse, I rearranged it to try the "limit arc
    // QQQ angle" trick
    const float signedRadius = 1 / nonZeroCurvatureQQQ();
    const Vec3 localCenterOfCurvature = side() * signedRadius;
    const Vec3 center = position() + localCenterOfCurvature;
    const float sign = signedRadius < 0 ? 1.0f : -1.0f;
    const float arcRadius = signedRadius * -sign;
    const float twoPi = 2 * OPENSTEER_M_PI;
    const float circumference = twoPi * arcRadius;
    const float rawLength = speed() * predictionTime * sign;
    const float arcLength = arcLengthLimit(rawLength,
                                           circumference * 0.25f);
    const float arcAngle = twoPi * arcLength / circumference;

    const Vec3 spoke = position() - center;
    const Vec3 newSpoke = spoke.rotateAboutGlobalY(arcAngle);
    const Vec3 prediction = newSpoke + center;

    return prediction;
  }else{
    return position() + (velocity() * predictionTime);
  }
}
// QQQ experimental fix for arcLength limit in predictFuturePosition
// QQQ and steerToAvoidObstaclesOnMap.
//
// args are the intended arc length (signed!), and the limit which is
// a given (positive!) fraction of the arc's (circle's) circumference
//
float MapDriver::arcLengthLimit(const float length, const float limit) const{
  if(length > 0)
    return minXXX(length, limit);
  else
    return -minXXX(-length, limit);
}
// this is a version of the one in SteerLibrary.h modified for "slow when
// heading off path".  I put it here because the changes were not
// compatible with Pedestrians.cpp.  It needs to be merged back after
// things settle down.
//
// its been modified in other ways too (such as "reduce the offset if
// facing in the wrong direction" and "increase the target offset to
// compensate the fold back") plus I changed the type of "path" from
// Pathway to GCRoute to use methods like indexOfNearestSegment and
// dotSegmentUnitTangents
//
// and now its been modified again for curvature-based prediction
//
Vec3 MapDriver::steerToFollowPath(const int direction, const float predictionTime, GCRoute& path){
  if(curvedSteering)
    return steerToFollowPathCurve(direction, predictionTime, path);
  else
    return steerToFollowPathLinear(direction, predictionTime, path);
}
Vec3 MapDriver::steerToFollowPathLinear(const int direction, const float predictionTime, GCRoute& path){
  // our goal will be offset from our path distance by this amount
  const float pathDistanceOffset = direction * predictionTime * speed();

  // predict our future position
  const Vec3 futurePosition = predictFuturePosition(predictionTime);

  // measure distance along path of our current and predicted positions
  const float nowPathDistance = path.mapPointToPathDistance (position());

  // are we facing in the correction direction?
  const Vec3 pathHeading = mapPointToTangent(path, position()) * static_cast<float>(direction);// path.tangentAt(position()) * (float)direction;
  const bool correctDirection = pathHeading.dot(forward()) > 0;

  // find the point on the path nearest the predicted future position
  // XXX need to improve calling sequence, maybe change to return a
  // XXX special path-defined object which includes two Vec3s and a
  // XXX bool (onPath,tangent (ignored), withinPath)
  float futureOutside;
  const Vec3 onPath = mapPointToPointOnCenterLineAndOutside(path, futurePosition, futureOutside); // path.mapPointToPath (futurePosition,futureOutside);

  // determine if we are currently inside the path tube
  float nowOutside;
  const Vec3 nowOnPath = mapPointToPointOnCenterLineAndOutside(path, position(), nowOutside);  // path.mapPointToPath (position (), nowOutside);

  // no steering is required if our present and future positions are
  // inside the path tube and we are facing in the correct direction
  const float m = -radius();
  const bool whollyInside = (futureOutside < m) && (nowOutside < m);
  if(whollyInside && correctDirection){
    // all is well, return zero steering
    return Vec3::zero;
  }else{
    // otherwise we need to steer towards a target point obtained
    // by adding pathDistanceOffset to our current path position
    // (reduce the offset if facing in the wrong direction)
    const float targetPathDistance = (nowPathDistance +
                                      (pathDistanceOffset *
                                       (correctDirection ? 1 : 0.1f)));
    Vec3 target = path.mapPathDistanceToPoint (targetPathDistance);


    // if we are on one segment and target is on the next segment and
    // the dot of the tangents of the two segments is negative --
    // increase the target offset to compensate the fold back
    const int ip =  static_cast<int>(mapPointToSegmentIndex(path, position())); // path.indexOfNearestSegment (position ());
    const int it =  static_cast<int>(mapPointToSegmentIndex(path, target)); // path.indexOfNearestSegment (target);
    // Because polyline paths have a constant tangent along a segment
    // just set the distance along the segment to @c 0.0f.
    Vec3 const ipTangent = path.mapSegmentDistanceToTangent(ip, 0.0f);
    // Because polyline paths have a constant tangent along a segment
    // just set the distance along the segment to @c 0.0f.
    Vec3 const itTangent = path.mapSegmentDistanceToTangent(it, 0.0f);
    if(((ip + direction) == it) && (itTangent.dot(ipTangent) < -0.1f)){
      const float newTargetPathDistance =
          nowPathDistance + (pathDistanceOffset * 2);
      target = path.mapPathDistanceToPoint(newTargetPathDistance);
    }

    // if we are currently outside head directly in
    // (QQQ new, experimental, makes it turn in more sharply)
    if(nowOutside > 0) return steerForSeek(nowOnPath);

    // steering to seek target on path
    const Vec3 seek = steerForSeek(target).truncateLength(maxForce());

    // return that seek steering -- except when we are heading off
    // the path (currently on path and future position is off path)
    // in which case we put on the brakes.
    if((nowOutside < 0) && (futureOutside > 0))
      return (seek.perpendicularComponent(forward()) - (forward() * maxForce()));
    else
      return seek;
  }
}
// Path following case for curved prediction and incremental steering
// (called from steerToFollowPath for the curvedSteering case)
//
// QQQ this does not handle the case when we AND futurePosition
// QQQ are outside, say when approach the path from far away
//
Vec3 MapDriver::steerToFollowPathCurve(const int direction, const float predictionTime, GCRoute& path){
  // predict our future position (based on current curvature and speed)
  const Vec3 futurePosition = predictFuturePosition(predictionTime);
  // find the point on the path nearest the predicted future position
  float futureOutside;
  const Vec3 onPath =  mapPointToPointOnCenterLineAndOutside(path, futurePosition, futureOutside); // path.mapPointToPath (futurePosition,futureOutside);
  const Vec3 pathHeading =  mapPointAndDirectionToTangent(path, onPath, direction); // path.tangentAt (onPath, direction);
  const Vec3 rawBraking = forward() * maxForce() * -1;
  const Vec3 braking = ((futureOutside < 0) ? Vec3::zero : rawBraking);
  //qqq experimental wrong-way-fixer
  float nowOutside;
  Vec3 nowTangent;
  const Vec3 p = position();
  const Vec3 nowOnPath = path.mapPointToPath(p, nowTangent, nowOutside);
  nowTangent *= (float)direction;
  const float alignedness = nowTangent.dot(forward ());

  // facing the wrong way?
  if(alignedness < 0){
    annotationLine (p, p + (nowTangent * 10), gCyan);

    // if nearly anti-parallel
    if(alignedness < -0.707f){
      const Vec3 towardCenter = nowOnPath - p;
      const Vec3 turn = (towardCenter.dot(side()) > 0 ?
                         side() * maxForce() : side() * maxForce() * -1);
      return (turn + rawBraking);
    }else{
      return (steerTowardHeading(pathHeading).perpendicularComponent(forward()) + braking);
    }
  }

  // is the predicted future position(+radius+margin) inside the path?
  if(futureOutside < -(radius () + 1.0f)){
    // then no steering is required
    return Vec3::zero;
  }else{
    // two cases, if entering a turn (a waypoint between path segments)
    if(isNearWaypoint(path, onPath)  && (futureOutside > 0)){
      // steer to align with next path segment
      return steerTowardHeading(pathHeading) + braking;
    }else{
      // otherwise steer away from the side of the path we
      // are heading for
      const Vec3 pathSide = localRotateForwardToSide(pathHeading);
      const Vec3 towardFP = futurePosition - onPath;
      const float whichSide = (pathSide.dot(towardFP) < 0) ? 1.0f : -1.0f;
      return (side() * maxForce() * whichSide) + braking;
    }
  }
}

GCRoute* MapDriver::makePath(){
  // a few constants based on world size
  const float m = worldSize * 0.4f; // main diamond size
  const float n = worldSize / 8;    // notch size
  const float o = worldSize * 2;    // outside of the sand

  // construction vectors
  const Vec3 p(0,   0, m);
  const Vec3 q(0,   0, m-n);
  const Vec3 r(-m,  0, 0);
  const Vec3 s(2*n, 0, 0);
  const Vec3 t(o,   0, 0);
  const Vec3 u(-o,  0, 0);
  const Vec3 v(n,   0, 0);
  const Vec3 w(0, 0, 0);

  // path vertices
  const Vec3 a(t-p);
  const Vec3 b(s+v-p);
  const Vec3 c(s-q);
  const Vec3 d(s+q);
  const Vec3 e(s-v+p);
  const Vec3 f(p-w);
  const Vec3 g(r-w);
  const Vec3 h(-p-w);
  const Vec3 i(u-p);

  // return Path object
  const int pathPointCount = 9;
  const Vec3 pathPoints[pathPointCount] = {a, b, c, d, e, f, g, h, i};
  const float k = 10.0f;
  const float pathRadii[pathPointCount] = {k, k, k, k, k, k, k, k, k};

  return new GCRoute(pathPointCount, pathPoints, pathRadii, false);
}
TerrainMap* MapDriver::makeMap(){
  return new TerrainMap(Vec3::zero, worldSize, worldSize, (int)worldSize + 1);
}

bool MapDriver::handleExitFromMap(){
  if(demoSelect == 2){
    // for path following, do wrap-around (teleport) and make new map
    const float px = position ().x;
    const float fx = forward ().x;
    const float ws = worldSize * 0.51f; // slightly past edge
    if(((fx > 0) && (px > ws)) || ((fx < 0) && (px < -ws))){
      // bump counters
      lapsStarted++;
      lapsFinished++;

      // set position on other side of the map (set new X coordinate)
      position((((px < 0) ? 1 : -1) *
                   ((worldSize * 0.5f) + (speed() * lookAheadTimePF ()))),
                   position ().y, position ().z);

      // reset bookeeping to detect stuck cycles
      resetStuckCycleDetection ();

      // prevent long streaks due to teleportation
      clearTrailHistory ();

      return true;
    }
  }else{
      // for the non-path-following demos:
      // reset simulation if the vehicle drives through the fence
      if(position().length() > worldDiag) reset();
  }
  return false;
}
float MapDriver::wingSlope(){
  return interpolate(relativeSpeed(), (curvedSteering ? 0.3f : 0.35f), 0.06f);
}
void MapDriver::resetStuckCycleDetection(){
  resetSmoothedPosition(position() + (forward() * -80)); // qqq
}
// QQQ just a stop gap, not quite right
// (say for example we were going around a circle with radius > 10)
bool MapDriver::weAreGoingInCircles(){
  const Vec3 offset = smoothedPosition() - position();
  return offset.length() < 10;
}
float MapDriver::lookAheadTimeOA() const{
  const float minTime = (baseLookAheadTime *
                         (curvedSteering ?
                          interpolate(relativeSpeed(), 0.4f, 0.7f) :
                          0.66f));
  return combinedLookAheadTime(minTime, 3);
}
float MapDriver::lookAheadTimePF() const{
  return combinedLookAheadTime(baseLookAheadTime, 3);
}

// QQQ maybe move to SimpleVehicle ?
// compute a "look ahead time" with two components, one based on
// minimum time to (say) a collision and one based on minimum distance
// arg 1 is "seconds into the future", arg 2 is "meters ahead"
float MapDriver::combinedLookAheadTime(float minTime, float minDistance) const{
  if(speed () == 0) return 0;
  return maxXXX(minTime, minDistance / speed());
}
// is vehicle body inside the path?
// (actually tests if all four corners of the bounbding box are inside)
//
bool MapDriver::bodyInsidePath(){
  if(demoSelect == 2){
    const Vec3 bbSide = side() * halfWidth;
    const Vec3 bbFront = forward() * halfLength;
    return ( /* path->isInsidePath (position () - bbFront + bbSide) */ isInsidePathway(*path, position() - bbFront + bbSide) &&
             /* path->isInsidePath (position () + bbFront + bbSide) */ isInsidePathway(*path, position() + bbFront + bbSide) &&
             /* path->isInsidePath (position () + bbFront - bbSide) */ isInsidePathway(*path, position() + bbFront - bbSide) &&
             /* path->isInsidePath (position () - bbFront - bbSide) */ isInsidePathway(*path, position() - bbFront - bbSide));
  }
  return true;
}
Vec3 MapDriver::convertAbsoluteToIncrementalSteering(const Vec3& absolute, const float elapsedTime){
  const Vec3 curved = convertLinearToCurvedSpaceGlobal(absolute);
  blendIntoAccumulator(elapsedTime * 8.0f, curved, currentSteering);
  return currentSteering;
}
// QQQ new utility 2-25-04 -- may replace inline code elsewhere
//
// Given a location in this vehicle's linear local space, convert it into
// the curved space defined by the vehicle's current path curvature.  For
// example, forward() gets mapped on a point 1 unit along the circle
// centered on the current center of curvature and passing through the
// vehicle's position().
//
Vec3 MapDriver::convertLinearToCurvedSpaceGlobal(const Vec3& linear){
  const Vec3 trimmedLinear = linear.truncateLength(maxForce());

  // ---------- this block imported from steerToAvoidObstaclesOnMap
  const float signedRadius = 1 / (nonZeroCurvatureQQQ() /*QQQ*/ * 1);
  const Vec3 localCenterOfCurvature = side () * signedRadius;
  const Vec3 center = position() + localCenterOfCurvature;
  const float sign = signedRadius < 0 ? 1.0f : -1.0f;
  const float arcLength = trimmedLinear.dot(forward());
  //
  const float arcRadius = signedRadius * -sign;
  const float twoPi = 2 * OPENSTEER_M_PI;
  const float circumference = twoPi * arcRadius;
  const float arcAngle = twoPi * arcLength / circumference;
  // ---------- this block imported from steerToAvoidObstaclesOnMap

  // ---------- this block imported from scanObstacleMap
  // vector from center of curvature to position of vehicle
  const Vec3 initialSpoke = position() - center;
  // rotate by signed arc angle
  const Vec3 spoke = initialSpoke.rotateAboutGlobalY(arcAngle * sign);
  // ---------- this block imported from scanObstacleMap

  const Vec3 fromCenter = -localCenterOfCurvature.normalize();
  const float dRadius = trimmedLinear.dot(fromCenter);
  const float radiusChangeFactor = (dRadius + arcRadius) / arcRadius;
  const Vec3 resultLocation = center + (spoke * radiusChangeFactor);
  // return the vector from vehicle position to the coimputed location
  // of the curved image of the original linear offset
  return resultLocation - position();
}
// approximate value for the Polaris Ranger 6x6: 16 feet, 5 meters
float MapDriver::minimumTurningRadius() const{
  return 5.0f;
}

Vec3 MapDriver::adjustSteeringForMinimumTurningRadius(const Vec3& steering){
  const float maxCurvature = 1 / (minimumTurningRadius() * 1.1f);

  // are we turning more sharply than the minimum turning radius?
  if (absXXX(curvature()) > maxCurvature){
    // remove the tangential (non-thrust) component of the steering
    // force, replace it with a force pointing away from the center
    // of curvature, causing us to "widen out" easing off from the
    // minimum turing radius
    const float signedRadius = 1 / nonZeroCurvatureQQQ ();
    const float sign = signedRadius < 0 ? 1.0f : -1.0f;
    const Vec3 thrust = steering.parallelComponent (forward ());
    const Vec3 trimmed = thrust.truncateLength (maxForce ());
    const Vec3 widenOut = side () * maxForce () * sign;
    return trimmed + widenOut;
  }
  // otherwise just return unmodified input
  return steering;
}
// QQQ This is to work around the bug that scanObstacleMap's current
// QQQ arguments preclude the driving straight [curvature()==0] case.
// QQQ This routine returns the current vehicle path curvature, unless it
// QQQ is *very* close to zero, in which case a small positive number is
// QQQ returned (corresponding to a radius of 100,000 meters).
// QQQ
// QQQ Presumably it would be better to get rid of this routine and
// QQQ redesign the arguments of scanObstacleMap
//
float MapDriver::nonZeroCurvatureQQQ() const{
  const float c = curvature();
  const float minCurvature = 1.0f / 100000.0f; // 100,000 meter radius
  const bool tooSmall = (c < minCurvature) && (c > -minCurvature);
  return (tooSmall ? minCurvature: c);
}
// QQQ ad hoc speed limitation based on path orientation...
// QQQ should be renamed since it is based on more than curvature
//
float MapDriver::maxSpeedForCurvature(){
  float maxRelativeSpeed = 1;

  if(curvedSteering){
    // compute an ad hoc "relative curvature"
    const float absC = absXXX(curvature());
    const float maxC = 1 / minimumTurningRadius();
    const float relativeCurvature = sqrtXXX(clip (absC/maxC, 0, 1));

    // map from full throttle when straight to 10% at max curvature
    const float curveSpeed = interpolate(relativeCurvature,1.0f,0.1f);
    annoteMaxRelSpeedCurve = curveSpeed;

    if(demoSelect != 2){
      maxRelativeSpeed = curveSpeed;
    }else{
      // heading (unit tangent) of the path segment of interest
      const Vec3 pathHeading =  mapPointAndDirectionToTangent(*path, position(), pathFollowDirection); // path->tangentAt (position (), pathFollowDirection);
      // measure how parallel we are to the path
      const float parallelness = pathHeading.dot(forward());

      // determine relative speed for this heading
      const float mw = 0.2f;
      const float headingSpeed = ((parallelness < 0) ? mw : interpolate(parallelness, mw, 1.0f));
      maxRelativeSpeed = minXXX(curveSpeed, headingSpeed);
    }
  }
  annoteMaxRelSpeed = maxRelativeSpeed;
  return maxSpeed() * maxRelativeSpeed;
}
// xxx library candidate
// xxx assumes (but does not check or enforce) heading is unit length
//
Vec3 MapDriver::steerTowardHeading(const Vec3& desiredGlobalHeading){
  const Vec3 headingError = desiredGlobalHeading - forward();
  return headingError.normalize() * maxForce();
}


void MapDriver::clearTrailHistory(){
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
void MapDriver::recordTrailVertex(const float currentTime, const Vec3& pos){
  // we use our own trail mesh
  Trail.lineIndices.push_back(Trail.lineVertices.size() - 1);
  Trail.lineIndices.push_back(Trail.lineVertices.size());
  irr::video::S3DVertex vert;
  vert.Pos = irr::core::vector3df(pos.x,pos.y,pos.z);
  vert.Color = irr::video::SColor(255, 255,0,0);
  Trail.lineVertices.push_back(vert);
}

// SceneNode
void MapDriver::OnRegisterSceneNode(){
  if(IsVisible) irrPointers.SMgr->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}
void MapDriver::render(){
  irrPointers.Driver->setMaterial(Material);
  irrPointers.Driver->setTransform(irr::video::ETS_WORLD, irr::scene::ISceneNode::AbsoluteTransformation);
  Mesh.draw(irrPointers.Driver);
  irrPointers.Driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
  Trail.draw(irrPointers.Driver);
}
const irr::core::aabbox3d<irr::f32>& MapDriver::getBoundingBox() const{
  return Box;
}
irr::u32 MapDriver::getMaterialCount(){
  return 1;
}
irr::video::SMaterial& MapDriver::getMaterial(irr::u32 i){
  return Material;
}
void MapDriver::setMaterial(irr::video::SMaterial newMaterial){
  Material = newMaterial;
}

/**
 * Maps @a point to @a pathway and extracts the radius at the mapping point.
 */
float mapPointToRadius(OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway, OpenSteer::Vec3 const& point){
  PointToRadiusMapping mapping;
  OpenSteer::mapPointToPathAlike(pathway, point, mapping);
  return mapping.vradius;
}
/**
 * Maps @a point to @a pathway and extracts the tangent at the mapping
 * point.
 */
OpenSteer::Vec3 mapPointToTangent( OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway, OpenSteer::Vec3 const& point){
  PointToTangentMapping mapping;
  OpenSteer::mapPointToPathAlike(pathway, point, mapping);
  return mapping._tangent;
}
/**
 * Returns @c true if @a point is inside @a pathway segment @a segmentIndex.
 *
 * On point on the boundary isn't inside the pathway.
 */
bool isInsidePathSegment(OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway,
                         OpenSteer::PolylineSegmentedPathwaySegmentRadii::size_type segmentIndex,
                         OpenSteer::Vec3 const& point){
  assert(pathway.isValid() && "pathway isn't valid.");
  assert(segmentIndex < pathway.segmentCount() && "segmentIndex out of range.");

  float const segmentDistance = pathway.mapPointToSegmentDistance(segmentIndex, point);
  OpenSteer::Vec3 const pointOnSegmentCenterLine = pathway.mapSegmentDistanceToPoint(segmentIndex, segmentDistance);
  float const segmentRadiusAtPoint = pathway.mapSegmentDistanceToRadius(segmentIndex, segmentDistance);

  float const distancePointToPointOnSegmentCenterLine = (point - pointOnSegmentCenterLine).length();

  return distancePointToPointOnSegmentCenterLine < segmentRadiusAtPoint;
}
/**
 * Maps the @a point to @a pathway and extracts the tangent at the mapping
 * point or of the next path segment as indicated by @a direction if the
 * mapping point is near a path defining point (waypoint).
 *
 * @param pathway Pathway to inspect.
 * @param point Point to map to @a pathway.
 * @param direction Follow the path in path direction (@c 1) or in reverse
 *                  direction ( @c -1 ).
 */
OpenSteer::Vec3 mapPointAndDirectionToTangent(OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway, OpenSteer::Vec3 const& point, int direction){
  assert(((1 == direction) || (-1 == direction)) && "direction must be 1 or -1.");
  typedef OpenSteer::PolylineSegmentedPathwaySegmentRadii::size_type size_type;

  PointToSegmentIndexMapping mapping;
  OpenSteer::mapPointToPathAlike(pathway, point, mapping);
  size_type segmentIndex = mapping.segmentIndex;
  size_type nextSegmentIndex = segmentIndex;
  if(0 < direction){
    nextSegmentIndex = OpenSteer::nextSegment( pathway, segmentIndex );
  }else{
    nextSegmentIndex = OpenSteer::previousSegment( pathway, segmentIndex );
  }

  if(isInsidePathSegment( pathway, nextSegmentIndex, point)){
    segmentIndex = nextSegmentIndex;
  }

  // To save calculations to gather the tangent in a sound way the fact is
  // used that a polyline segmented pathway has the same tangent for a
  // whole segment.
  return pathway.mapSegmentDistanceToTangent(segmentIndex, 0.0f) * static_cast<float>(direction);
}
/**
 * Returns @c true if @a point is near a waypoint of @a pathway.
 *
 * It is near if its distance to a waypoint of the path is lesser than the
 * radius of one of the segments that the waypoint belongs to.
 *
 * On point on the boundary isn't inside the pathway.
 */
bool isNearWaypoint(OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway, OpenSteer::Vec3 const& point){
  assert(pathway.isValid() && "pathway must be valid.");

  typedef OpenSteer::PolylineSegmentedPathwaySegmentRadii::size_type size_type;

  size_type pointIndex = 0;

  // Test first waypoint.
  OpenSteer::Vec3 pointPathwayPointVector = point - pathway.point(pointIndex);
  float pointPathwayPointDistance = pointPathwayPointVector.dot(pointPathwayPointVector);
  if(pointPathwayPointDistance < OpenSteer::square(pathway.segmentRadius(pointIndex))){
      return true;
  }

  // Test other waypoints.
  size_type const maxInnerPointIndex = pathway.pointCount() - 2;
  for(pointIndex = 1; pointIndex <= maxInnerPointIndex; ++pointIndex){
    pointPathwayPointVector = point - pathway.point(pointIndex);
    pointPathwayPointDistance = pointPathwayPointVector.dot(pointPathwayPointVector);
    if((pointPathwayPointDistance < OpenSteer::square(pathway.segmentRadius(pointIndex))) ||
       (pointPathwayPointDistance < OpenSteer::square(pathway.segmentRadius(pointIndex - 1)))){
        return true;
    }
  }

  // Test last waypoint.
  pointPathwayPointVector = point - pathway.point(pointIndex);
  pointPathwayPointDistance = pointPathwayPointVector.dot(pointPathwayPointVector);
  if(pointPathwayPointDistance < OpenSteer::square(pathway.segmentRadius(pointIndex - 1))){
    return true;
  }

  return false;
}
/**
 * Maps @a point to @a pathway and returns the mapping point on the pathway
 * boundary and how far outside @a point is from the mapping point.
 */
OpenSteer::Vec3 mapPointToPointOnCenterLineAndOutside( OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway, OpenSteer::Vec3 const& point, float& outside){
  PointToPointOnCenterLineAndOutsideMapping mapping;
  OpenSteer::mapPointToPathAlike(pathway, point, mapping);
  outside = mapping.distancePointToPathBoundary;
  return mapping.pointOnPathCenterLine;
}
/**
 * Maps @a point to @a pathway and returns how far outside @a point is from
 * the mapping point on the path boundary.
 */
float mapPointToOutside(OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway, OpenSteer::Vec3 const& point){
  PointToOutsideMapping mapping;
  OpenSteer::mapPointToPathAlike(pathway, point, mapping);
  return mapping.distancePointToPathBoundary;
}
/**
 * Returns @c true if @a point is inside @a pathway, @c false otherwise.
 * A point on the boundary isn't inside the pathway.
 */
bool isInsidePathway(OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway, OpenSteer::Vec3 const& point){
  return 0.0f > mapPointToOutside(pathway, point);
}
OpenSteer::PolylineSegmentedPathwaySegmentRadii::size_type mapPointToSegmentIndex(OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway,
                                                                                  OpenSteer::Vec3 const& point){
  PointToSegmentIndexMapping mapping;
  OpenSteer::mapPointToPathAlike(pathway, point, mapping);
  return mapping.segmentIndex;
}

