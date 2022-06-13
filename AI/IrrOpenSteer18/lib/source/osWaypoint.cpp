#include "../../../../config.h"
#ifdef OPENSTEER
#include "osWaypoint.h"

namespace OpenSteer{

  Waypoint::Waypoint(WaypointMap* wpm, Vec3 pos, int id, core::stringc name)
  : wpMap(wpm), ID(id), Name(name), _position(pos){
    wpm->isMeshDirty = true;
  }

  int Waypoint::getID(){
    return ID;
  }
  void Waypoint::setID(int id){
    ID = id;
  }
  core::stringc Waypoint::getName(){
    return Name;
  }
  void Waypoint::setName(core::stringc name){
    Name = name;
  }

  int Waypoint::getWaypointID(Waypoint* wp){
    for(u32 t = 0; t < lstReachables.size(); ++t)
      if(lstReachables[t]._waypoint == wp) return t;
    return -1;
  }
  int Waypoint::getWaypointID(core::stringc name){
    for(u32 t = 0; t < lstReachables.size(); ++t)
      if(lstReachables[t]._waypoint->getName() == name) return t;
    return -1;
  }
  int Waypoint::getWaypointID(int id){
    for(u32 t = 0; t < lstReachables.size(); ++t)
      if(lstReachables[t]._waypoint->getID() == id) return t;
    return -1;
  }

  void Waypoint::setPosition(Vec3 p){
    _position = p;
    wpMap->updateDistances();
    wpMap->isMeshDirty = true;
  }
  Vec3 Waypoint::getPosition(){
    return _position;
  }
  void Waypoint::updateDistances(){
    for(u32 t = 0; t < lstReachables.size(); ++t)
      lstReachables[t]._distance = (u32)distance(_position, lstReachables[t]._waypoint->getPosition());
  }

  void Waypoint::addReachable(Waypoint* wp){
    int wid = getWaypointID(wp);
    if(wid < 0){
      WP_Reachable rn;
      rn._waypoint = wp;
      rn._distance = (u32)distance(_position, wp->getPosition());
      lstReachables.push_back(rn);
      wpMap->isMeshDirty = true;
    }
  }
  void Waypoint::addReachable(core::stringc name){
    Waypoint* wp = wpMap->getWaypoint(name);
    if(wp) addReachable(wp);
  }
  void Waypoint::addReachable(int id){
    Waypoint* wp = wpMap->getWaypoint(id);
    if(wp) addReachable(wp);
  }
  void Waypoint::removeReachable(Waypoint* wp){
    int wid = getWaypointID(wp);
    if(wid >= 0){
      lstReachables.erase(wid);
      wpMap->isMeshDirty = true;
    }
  }
  void Waypoint::removeReachable(core::stringc name){
    Waypoint* wp = wpMap->getWaypoint(name);
    if(wp) removeReachable(wp);
  }
  void Waypoint::removeReachable(int id){
    Waypoint* wp = wpMap->getWaypoint(id);
    if(wp) removeReachable(wp);
  }

  core::array<WP_Reachable>* Waypoint::getReachableArray(){
    return &lstReachables;
  }

  void Waypoint::addMesh(datAnnotation* ann, bool showConnections, video::SColor colWP, video::SColor colCon, float radius, float height){
    /*! 8-Eck
       5_4
      6/ \ 3
      |   |
      7\_/ 2
       0 1
    */
    // 1st level
    { video::S3DVertex v(core::vector3df(-0.333, 0, -1.000),  // Position
      core::vector3df(0,1,0), colWP, core::vector2df(1,1));
      v.Pos *= radius;
      v.Pos += _position.vector3df();
      ann->lineVertices.push_back(v);    // Index = 0
    }
    { video::S3DVertex v(core::vector3df(0.333, 0, -1.000),  // Position
      core::vector3df(0,1,0), colWP, core::vector2df(1,1));
      v.Pos *= radius;
      v.Pos += _position.vector3df();
      ann->lineVertices.push_back(v);    // Index = 1
    }
    { video::S3DVertex v(core::vector3df(1.000, 0, -0.333),  // Position
      core::vector3df(0,1,0), colWP, core::vector2df(1,1));
      v.Pos *= radius;
      v.Pos += _position.vector3df();
      ann->lineVertices.push_back(v);    // Index = 2
    }
    { video::S3DVertex v(core::vector3df(1.000, 0,  0.333),  // Position
      core::vector3df(0,1,0), colWP, core::vector2df(1,1));
      v.Pos *= radius;
      v.Pos += _position.vector3df();
      ann->lineVertices.push_back(v);    // Index = 3
    }
    { video::S3DVertex v(core::vector3df(0.333, 0,  1.000),  // Position
      core::vector3df(0,1,0), colWP, core::vector2df(1,1));
      v.Pos *= radius;
      v.Pos += _position.vector3df();
      ann->lineVertices.push_back(v);    // Index = 4
    }
    { video::S3DVertex v(core::vector3df(-0.333, 0,  1.000),  // Position
      core::vector3df(0,1,0), colWP, core::vector2df(1,1));
      v.Pos *= radius;
      v.Pos += _position.vector3df();
      ann->lineVertices.push_back(v);    // Index = 5
    }
    { video::S3DVertex v(core::vector3df(-1.000, 0,  0.333),  // Position
      core::vector3df(0,1,0), colWP, core::vector2df(1,1));
      v.Pos *= radius;
      v.Pos += _position.vector3df();
      ann->lineVertices.push_back(v);    // Index = 6
    }
    { video::S3DVertex v(core::vector3df(-1.000, 0, -0.333),  // Position
      core::vector3df(0,1,0), colWP, core::vector2df(1,1));
      v.Pos *= radius;
      v.Pos += _position.vector3df();
      ann->lineVertices.push_back(v);    // Index = 7
    }
    ann->lineIndices.push_back(0); ann->lineIndices.push_back(1);
    ann->lineIndices.push_back(1); ann->lineIndices.push_back(2);
    ann->lineIndices.push_back(2); ann->lineIndices.push_back(3);
    ann->lineIndices.push_back(3); ann->lineIndices.push_back(4);
    ann->lineIndices.push_back(4); ann->lineIndices.push_back(5);
    ann->lineIndices.push_back(5); ann->lineIndices.push_back(6);
    ann->lineIndices.push_back(6); ann->lineIndices.push_back(7);
    ann->lineIndices.push_back(7); ann->lineIndices.push_back(0);
    // 2nd level
    { video::S3DVertex v(core::vector3df(-0.333, 0, -1.000),  // Position
      core::vector3df(0,1,0), colWP, core::vector2df(1,1));
      v.Pos *= radius;
      v.Pos += _position.vector3df();
      v.Pos.Y += height;
      ann->lineVertices.push_back(v);    // Index = 8
    }
    { video::S3DVertex v(core::vector3df(0.333, 0, -1.000),  // Position
      core::vector3df(0,1,0), colWP, core::vector2df(1,1));
      v.Pos *= radius;
      v.Pos += _position.vector3df();
      v.Pos.Y += height;
      ann->lineVertices.push_back(v);    // Index = 9
    }
    { video::S3DVertex v(core::vector3df(1.000, 0, -0.333),  // Position
      core::vector3df(0,1,0), colWP, core::vector2df(1,1));
      v.Pos *= radius;
      v.Pos += _position.vector3df();
      v.Pos.Y += height;
      ann->lineVertices.push_back(v);    // Index = 10
    }
    { video::S3DVertex v(core::vector3df(1.000, 0,  0.333),  // Position
      core::vector3df(0,1,0), colWP, core::vector2df(1,1));
      v.Pos *= radius;
      v.Pos += _position.vector3df();
      v.Pos.Y += height;
      ann->lineVertices.push_back(v);    // Index = 11
    }
    { video::S3DVertex v(core::vector3df(0.333, 0,  1.000),  // Position
      core::vector3df(0,1,0), colWP, core::vector2df(1,1));
      v.Pos *= radius;
      v.Pos += _position.vector3df();
      v.Pos.Y += height;
      ann->lineVertices.push_back(v);    // Index = 12
    }
    { video::S3DVertex v(core::vector3df(-0.333, 0,  1.000),  // Position
      core::vector3df(0,1,0), colWP, core::vector2df(1,1));
      v.Pos *= radius;
      v.Pos += _position.vector3df();
      v.Pos.Y += height;
      ann->lineVertices.push_back(v);    // Index = 13
    }
    { video::S3DVertex v(core::vector3df(-1.000, 0,  0.333),  // Position
      core::vector3df(0,1,0), colWP, core::vector2df(1,1));
      v.Pos *= radius;
      v.Pos += _position.vector3df();
      v.Pos.Y += height;
      ann->lineVertices.push_back(v);    // Index = 14
    }
    { video::S3DVertex v(core::vector3df(-1.000, 0, -0.333),  // Position
      core::vector3df(0,1,0), colWP, core::vector2df(1,1));
      v.Pos *= radius;
      v.Pos += _position.vector3df();
      v.Pos.Y += height;
      ann->lineVertices.push_back(v);    // Index = 15
    }
    ann->lineIndices.push_back(8); ann->lineIndices.push_back(9);
    ann->lineIndices.push_back(9); ann->lineIndices.push_back(10);
    ann->lineIndices.push_back(10); ann->lineIndices.push_back(11);
    ann->lineIndices.push_back(11); ann->lineIndices.push_back(12);
    ann->lineIndices.push_back(12); ann->lineIndices.push_back(13);
    ann->lineIndices.push_back(13); ann->lineIndices.push_back(14);
    ann->lineIndices.push_back(14); ann->lineIndices.push_back(15);
    ann->lineIndices.push_back(15); ann->lineIndices.push_back(8);
    // up/down lines
    ann->lineIndices.push_back(0); ann->lineIndices.push_back(8);
    ann->lineIndices.push_back(1); ann->lineIndices.push_back(9);
    ann->lineIndices.push_back(2); ann->lineIndices.push_back(10);
    ann->lineIndices.push_back(3); ann->lineIndices.push_back(11);
    ann->lineIndices.push_back(4); ann->lineIndices.push_back(12);
    ann->lineIndices.push_back(5); ann->lineIndices.push_back(13);
    ann->lineIndices.push_back(6); ann->lineIndices.push_back(14);
    ann->lineIndices.push_back(7); ann->lineIndices.push_back(15);
    // Connection-Lines
    if(showConnections){
      for(u32 t = 0; t < lstReachables.size(); ++t){
        int v1 = ann->lineVertices.size();
        { video::S3DVertex v(_position.vector3df(),  // Position
          core::vector3df(0,1,0), video::SColor(255, 255,0,0), core::vector2df(1,1));
          v.Pos.Y += (height * 0.5);
          ann->lineVertices.push_back(v);    // Index = 15
        }
        { video::S3DVertex v(lstReachables[t]._waypoint->getPosition().vector3df(),  // Position
          core::vector3df(0,1,0), colCon, core::vector2df(1,1));
          v.Pos.Y += (height * 0.5);
          ann->lineVertices.push_back(v);    // Index = 15
        }
        ann->lineIndices.push_back(v1); ann->lineIndices.push_back(v1 + 1);
      }
    }
  }

} // namespace OpenSteer
#endif
