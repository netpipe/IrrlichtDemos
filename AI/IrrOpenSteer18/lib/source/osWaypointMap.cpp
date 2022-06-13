#include "../../../../config.h"
#ifdef OPENSTEER
#include "osWaypointMap.h"

namespace OpenSteer{

  WaypointMap::WaypointMap()
  : isMeshDirty(true), wp_radius(5), wp_height(10){
  }
  WaypointMap::~WaypointMap(){
    for(u32 t = 0; t < lstWaypoints.size(); ++t)
      delete lstWaypoints[t];
    lstWaypoints.clear();
  }

  int WaypointMap::getWaypointID(Waypoint* wp){
    for(u32 t = 0; t < lstWaypoints.size(); ++t)
      if(lstWaypoints[t] == wp) return t;
    return -1;
  }
  int WaypointMap::getWaypointID(core::stringc name){
    for(u32 t = 0; t < lstWaypoints.size(); ++t)
      if(lstWaypoints[t]->getName() == name) return t;
    return -1;
  }
  int WaypointMap::getWaypointID(int id){
    for(u32 t = 0; t < lstWaypoints.size(); ++t)
      if(lstWaypoints[t]->getID() == id) return t;
    return -1;
  }
  Waypoint* WaypointMap::getWaypoint(core::stringc name){
    int idx = getWaypointID(name);
    if(idx >= 0) return lstWaypoints[idx];
    return 0;
  }
  Waypoint* WaypointMap::getWaypoint(int id){
    int idx = getWaypointID(id);
    if(idx >= 0) return lstWaypoints[idx];
    return 0;
  }

  Waypoint* WaypointMap::addWaypoint(Vec3 pos){
    Waypoint* wpRet = new Waypoint(this, pos);
    if(wpRet) lstWaypoints.push_back(wpRet);
    return wpRet;
  }
  void WaypointMap::removeWaypoint(Waypoint* wp){
    int idx = getWaypointID(wp);
    if(idx >= 0){
      for(u32 t = 0; t < lstWaypoints.size(); ++t)
        lstWaypoints[t]->removeReachable(wp);
      delete wp;
      lstWaypoints.erase(idx);
    }
  }
  core::array<Waypoint*>* WaypointMap::getWaypointArray(){
    return &lstWaypoints;
  }

  void WaypointMap::updateDistances(){
    for(u32 t = 0; t < lstWaypoints.size(); ++t)
      lstWaypoints[t]->updateDistances();
  }

  void WaypointMap::render(video::IVideoDriver* driver, bool showConnections, video::SColor colWP, video::SColor colCon){
    static datAnnotation ann;
    static float old_wp_radius, old_wp_height;
    static bool old_showConnections;
    static video::SColor old_colWP, old_colCon;
    if((old_wp_radius != wp_radius) ||
       (old_wp_height != wp_height) ||
       (old_showConnections != showConnections) ||
       (old_colWP != colWP) ||
       (old_colCon != colCon)
    ) isMeshDirty = true;
    if(isMeshDirty){
      ann.lineVertices.clear();
      ann.lineIndices.clear();
      ann.faceVertices.clear();
      ann.faceIndices.clear();
      for(u32 t = 0; t < lstWaypoints.size(); ++t)
        lstWaypoints[t]->addMesh(&ann, showConnections, colWP, colCon, wp_radius, wp_height);
      old_wp_radius = wp_radius;
      old_wp_height = wp_height;
      old_showConnections = showConnections;
      old_colWP = colWP;
      old_colCon = colCon;
    }
    ann.render(driver);
  }

} // namespace OpenSteer
#endif
