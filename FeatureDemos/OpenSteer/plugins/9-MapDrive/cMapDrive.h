
#ifdef OPENSTEER
#ifndef C_MapDrive_H
#define C_MapDrive_H

#include <irrlicht.h>
#include "../../cOpenSteerDemo.h"
#include "../../cMeshTool.h"

using namespace OpenSteer;

typedef PolylineSegmentedPathwaySegmentRadii GCRoute;

class TerrainMap{
  private:
    int mapAddress(int i, int j) const{
      return i + (j * resolution);
    }
    std::vector<bool> map;

  public:
    Vec3 center;
    float xSize;
    float zSize;
    int resolution;
    bool outsideValue;

    // constructor
    TerrainMap(const Vec3& c, float x, float z, int r)
    : center(c), xSize(x), zSize(z), resolution(r), outsideValue(false),map(resolution * resolution){
      map.reserve(resolution * resolution);
    }
    // destructor
    ~TerrainMap(){}

    // clear the map (to false)
    void clear(){
      for(int i = 0; i < resolution; i++)
        for(int j = 0; j < resolution; j++)
          setMapBit (i, j, 0);
    }

    // get and set a bit based on 2d integer map index
    bool getMapBit(int i, int j) const{
      return map[mapAddress(i, j)];
    }

    bool setMapBit(int i, int j, bool value){
      return map[mapAddress(i, j)] = value;
    }

    // get a value based on a position in 3d world space
    bool getMapValue(const Vec3& point) const{
      const Vec3 local = point - center;
      const Vec3 localXZ = local.setYtoZero();

      const float hxs = xSize/2;
      const float hzs = zSize/2;

      const float x = localXZ.x;
      const float z = localXZ.z;

      const bool out = (x > +hxs) || (x < -hxs) || (z > +hzs) || (z < -hzs);

      if(out){
        return outsideValue;
      }else{
        const float r = (float)resolution; // prevent VC7.1 warning
        const int i = (int)remapInterval (x, -hxs, hxs, 0.0f, r);
        const int j = (int)remapInterval (z, -hzs, hzs, 0.0f, r);
        return getMapBit(i, j);
      }
    }

    datMesh getDrawMap(){
      datMesh mRet;
      mRet.Visible = true;
      mRet.lineIndices.clear();
      mRet.lineVertices.clear();
      mRet.faceIndices.clear();
      mRet.faceVertices.clear();

      const float xs = xSize / (float)resolution;
      const float zs = zSize / (float)resolution;
      const Vec3 alongRow(xs, 0, 0);
      const Vec3 nextRow(-xSize, 0, zs);
      Vec3 g((xSize - xs) / -2, 0, (zSize - zs) / -2);
      g += center;
      for(int j = 0; j < resolution; j++){
        for(int i = 0; i < resolution; i++){
          if(getMapBit(i, j)){
//            // squares
//            float rockHeight = 0;
//            irr::u32 p1st = mRet.lineVertices.size();
//            irr::video::S3DVertex vert;
//            vert.Pos = irr::core::vector3df(+xs / 2, rockHeight, +zs / 2);
//            mRet.lineVertices.push_back(vert);
//            vert.Pos = irr::core::vector3df(+xs / 2, rockHeight, -zs / 2);
//            mRet.lineVertices.push_back(vert);
//            vert.Pos = irr::core::vector3df(-xs / 2, rockHeight, -zs / 2);
//            mRet.lineVertices.push_back(vert);
//            vert.Pos = irr::core::vector3df(-xs / 2, rockHeight, +zs / 2);
//            mRet.lineVertices.push_back(vert);
//            mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 1);
//            mRet.lineIndices.push_back(p1st + 1); mRet.lineIndices.push_back(p1st + 2);
//            mRet.lineIndices.push_back(p1st + 2); mRet.lineIndices.push_back(p1st + 3);
//            mRet.lineIndices.push_back(p1st + 3); mRet.lineIndices.push_back(p1st + 0);
          }
          g += alongRow;
        }
        g += nextRow;
      }
      return mRet;
    }

    float minSpacing() const{
      return minXXX(xSize, zSize) / (float)resolution;
    }

    // used to detect if vehicle body is on any obstacles
    bool scanLocalXZRectangle(const AbstractLocalSpace& localSpace,
                              float xMin, float xMax,
                              float zMin, float zMax) const{
      const float spacing = minSpacing() / 2;

      for(float x = xMin; x < xMax; x += spacing){
        for(float z = zMin; z < zMax; z += spacing){
          const Vec3 sample(x, 0, z);
          const Vec3 global = localSpace.globalizePosition(sample);
          if(getMapValue (global)) return true;
        }
      }
      return false;
    }

    // Scans along a ray (directed line segment) on the XZ plane, sampling
    // the map for a "true" cell.  Returns the index of the first sample
    // that gets a "hit", or zero if no hits found.
    int scanXZray(const Vec3& origin, const Vec3& sampleSpacing, const int sampleCount) const{
      Vec3 samplePoint (origin);

      for(int i = 1; i <= sampleCount; i++){
        samplePoint += sampleSpacing;
        if (getMapValue (samplePoint)) return i;
      }

      return 0;
    }


    int cellwidth() const{
      return resolution;
    }
    int cellheight() const{
      return resolution;
    }
    bool isPassable(const Vec3& point) const{
      return !getMapValue(point);
    }
};

float mapPointToRadius(OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway, OpenSteer::Vec3 const& point);
OpenSteer::Vec3 mapPointToTangent(OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway, OpenSteer::Vec3 const& point);
bool isInsidePathSegment(OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway, OpenSteer::PolylineSegmentedPathwaySegmentRadii::size_type segmentIndex, OpenSteer::Vec3 const& point);
OpenSteer::Vec3 mapPointAndDirectionToTangent(OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway, OpenSteer::Vec3 const& point, int direction);
bool isNearWaypoint(OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway, OpenSteer::Vec3 const& point);
OpenSteer::Vec3 mapPointToPointOnCenterLineAndOutside( OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway, OpenSteer::Vec3 const& point, float& outside);
float mapPointToOutside(OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway, OpenSteer::Vec3 const& point);
bool isInsidePathway(OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway, OpenSteer::Vec3 const& point);
OpenSteer::PolylineSegmentedPathwaySegmentRadii::size_type mapPointToSegmentIndex(OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway, OpenSteer::Vec3 const& point);

class PointToRadiusMapping : public OpenSteer::DontExtractPathDistance{
  public:
    PointToRadiusMapping(): vradius(0.0f){}

    void setPointOnPathCenterLine(OpenSteer::Vec3 const&){}
    void setPointOnPathBoundary(OpenSteer::Vec3 const&){}
    void radius(float r){vradius = r;}
    void tangent(OpenSteer::Vec3 const&){}
    void setSegmentIndex(OpenSteer::size_t){}
    void setDistancePointToPath(float){}
    void setDistancePointToPathCenterLine(float){}
    void setDistanceOnPath(float){}
    void setDistanceOnSegment(float){}

    float vradius;
};
class PointToTangentMapping : public OpenSteer::DontExtractPathDistance{
  public:
    PointToTangentMapping() : _tangent(OpenSteer::Vec3(0.0f, 0.0f, 0.0f)){}

    void setPointOnPathCenterLine(OpenSteer::Vec3 const&){}
    void setPointOnPathBoundary(OpenSteer::Vec3 const&){}
    void radius(float){}
    void tangent(OpenSteer::Vec3 const& t){ _tangent = t; }
    void setSegmentIndex(OpenSteer::size_t){}
    void setDistancePointToPath(float){}
    void setDistancePointToPathCenterLine(float){}
    void setDistanceOnPath(float){}
    void setDistanceOnSegment(float){}

    OpenSteer::Vec3 _tangent;
};
class PointToPointOnCenterLineAndOutsideMapping : public OpenSteer::DontExtractPathDistance{
  public:
    PointToPointOnCenterLineAndOutsideMapping() : pointOnPathCenterLine(OpenSteer::Vec3(0.0f, 0.0f, 0.0f)), distancePointToPathBoundary(0.0f){}

    void setPointOnPathCenterLine(OpenSteer::Vec3 const& point){ pointOnPathCenterLine = point; }
    void setPointOnPathBoundary(OpenSteer::Vec3 const&){}
    void radius(float){}
    void tangent(OpenSteer::Vec3 const&){}
    void setSegmentIndex(OpenSteer::size_t){}
    void setDistancePointToPath(float d){ distancePointToPathBoundary = d; }
    void setDistancePointToPathCenterLine(float){}
    void setDistanceOnPath(float){}
    void setDistanceOnSegment(float){}

    OpenSteer::Vec3 pointOnPathCenterLine;
    float distancePointToPathBoundary;
};
class PointToOutsideMapping : public OpenSteer::DontExtractPathDistance{
  public:
    PointToOutsideMapping() : distancePointToPathBoundary(0.0f){}

    void setPointOnPathCenterLine(OpenSteer::Vec3 const&){}
    void setPointOnPathBoundary(OpenSteer::Vec3 const&){}
    void radius(float){}
    void tangent(OpenSteer::Vec3 const&){}
    void setSegmentIndex(OpenSteer::size_t){}
    void setDistancePointToPath(float d){ distancePointToPathBoundary = d; }
    void setDistancePointToPathCenterLine(float){}
    void setDistanceOnPath(float){}
    void setDistanceOnSegment(float){}

    float distancePointToPathBoundary;
};
class PointToSegmentIndexMapping : public OpenSteer::DontExtractPathDistance{
  public:
    PointToSegmentIndexMapping() : segmentIndex(0){}

    void setPointOnPathCenterLine(OpenSteer::Vec3 const&){}
    void setPointOnPathBoundary(OpenSteer::Vec3 const&){}
    void radius(float){}
    void tangent(OpenSteer::Vec3 const&){}
    void setSegmentIndex(OpenSteer::size_t i){ segmentIndex = i; }
    void setDistancePointToPath(float){}
    void setDistancePointToPathCenterLine(float){}
    void setDistanceOnPath(float){}
    void setDistanceOnSegment(float){}

    OpenSteer::size_t segmentIndex;
};

class MapDriver : public SimpleVehicle, public irr::scene::ISceneNode{
  private:
    datMesh Mesh;
    datMesh Trail;
    irr::core::aabbox3df Box;
    irr::video::SMaterial Material;

  public:
    // map of obstacles
    TerrainMap* map;
    // route for path following (waypoints and legs)
    GCRoute* path;
    // follow the path "upstream or downstream" (+1/-1)
    int pathFollowDirection;
    // master look ahead (prediction) time
    float baseLookAheadTime;
    // vehicle dimentions in meters
    float halfWidth;
    float halfLength;
    // keep track of failure rate (when vehicle is on top of obstacle)
    bool collisionDetected;
    bool collisionLastTime;
    float timeOfLastCollision;
    float sumOfCollisionFreeTimes;
    int countOfCollisionFreeTimes;
    // keep track of average speed
    float totalDistance;
    float totalTime;
    // keep track of path following failure rate
    // (these are probably obsolete now, replaced by stuckOffPathCount)
    float pathFollowTime;
    float pathFollowOffTime;
    // take note when current dt is zero (as in paused) for stat counters
    bool dtZero;
    // state saved for annotation
    Vec3 annotateAvoid;
    bool wingDrawFlagL, wingDrawFlagR;
    // QQQ first pass at detecting "stuck" state
    bool stuck;
    int stuckCount;
    int stuckCycleCount;
    int stuckOffPathCount;
    Vec3 qqqLastNearestObstacle;
    int lapsStarted;
    int lapsFinished;
    // QQQ temporary global QQQoaJustScraping
    // QQQ replace this global flag with a cleaner mechanism
    bool QQQoaJustScraping;
    int hintGivenCount;
    int hintTakenCount;
    // for "curvature-based incremental steering" -- contains the current
    // steering into which new incremental steering is blended
    Vec3 currentSteering;
    // use curved prediction and incremental steering:
    bool curvedSteering;
    bool incrementalSteering;
    // save obstacle avoidance stats for annotation
    // (nearest obstacle in each of the four zones)
    static float savedNearestWR, savedNearestR, savedNearestL, savedNearestWL;
    float annoteMaxRelSpeed, annoteMaxRelSpeedCurve, annoteMaxRelSpeedPath;
    // which of the three demo modes is selected
    static int demoSelect;
    // size of the world (the map actually)
    static float worldSize;
    static float worldDiag;

    // constructor
    MapDriver(irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent);
    ~MapDriver();
    // reset state
    void reset();
    // per frame simulation update
    void update(const float currentTime, const float elapsedTime);
    // draw this character/vehicle into the scene
    void draw();

    void adjustVehicleRadiusForSpeed();
    void collectReliabilityStatistics(const float currentTime, const float elapsedTime);
    Vec3 hintForObstacleAvoidance();
    Vec3 convertAbsoluteToIncrementalSteering(const Vec3& absolute, const float elapsedTime);

    Vec3 steerToAvoidObstaclesOnMap(const float minTimeToCollision, const TerrainMap& map);
    Vec3 steerToAvoidObstaclesOnMap(const float minTimeToCollision, const TerrainMap& map,  const Vec3& steerHint);
    float scanObstacleMap(const Vec3& start, const Vec3& center, const float arcAngle, const int segments, const float endRadiusChange, const Color& beforeColor, const Color& afterColor, Vec3& returnObstaclePosition);
    bool detectImminentCollision();
    Vec3 predictFuturePosition(const float predictionTime) const;
    float arcLengthLimit(const float length, const float limit) const;
    Vec3 steerToFollowPath(const int direction, const float predictionTime, GCRoute& path);
    Vec3 steerToFollowPathLinear(const int direction, const float predictionTime, GCRoute& path);
    Vec3 steerToFollowPathCurve(const int direction, const float predictionTime, GCRoute& path);

    GCRoute* makePath();
    TerrainMap* makeMap();

    bool handleExitFromMap();
    float wingSlope();
    void resetStuckCycleDetection();
    bool weAreGoingInCircles();
    float lookAheadTimePF() const;
    float lookAheadTimeOA() const;

    float combinedLookAheadTime(float minTime, float minDistance) const;
    bool bodyInsidePath();
    float minimumTurningRadius() const;
    Vec3 convertLinearToCurvedSpaceGlobal(const Vec3& linear);

    float nonZeroCurvatureQQQ() const;
    Vec3 adjustSteeringForMinimumTurningRadius(const Vec3& steering);
    Vec3 steerTowardHeading(const Vec3& desiredGlobalHeading);
    float maxSpeedForCurvature();


    void recordTrailVertex(const float currentTime, const Vec3& pos);
    void clearTrailHistory();

    //! SceneNode stuff
    virtual void OnRegisterSceneNode();
    virtual void render();
    virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
    virtual irr::u32 getMaterialCount();
    virtual irr::video::SMaterial& getMaterial(irr::u32 i);
    virtual void setMaterial(irr::video::SMaterial newMaterial);

};


#endif // C_MapDrive_H
#endif
