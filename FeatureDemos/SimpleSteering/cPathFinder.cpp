#include "cPathFinder.h"

cPathFinder::cPathFinder(){
  //ctor
}
cPathFinder::~cPathFinder(){
  //dtor
}

void cPathFinder::getRoute(int idStart, int idDest, int idEnemy){
  // path finding based on AStar (A*)
  core::array<datAStar> lstOpen, lstClosed;
  datAStar wpCur;
  lstOpen.clear();
  lstClosed.clear();

  // add start point to open list
  wpCur.idWaypoint = idStart;
  wpCur.idParent = idStart;
  wpCur.costG = 0; // complete way cost in path
  wpCur.costH = 0; // distance to target
  wpCur.costF = 0; // intersection cost (costG + costH)
  lstOpen.push_back(wpCur);
  // search shortest path
  bool pFound = false;
  while((lstOpen.size() > 0) && !pFound){
    // get first waypoint from open list
    wpCur = lstOpen[0];
    // move waypoint from open list to closed list
    lstClosed.push_front(wpCur);
    lstOpen.erase(0);
    // add all now reachable waypoints to open list

    for(unsigned int t = 0; t < Waypoints[wpCur.idWaypoint].next.size(); t++){
      datAStar wpNew;
      wpNew.idWaypoint = Waypoints[wpCur.idWaypoint].next[t].ID; // new ID from current reachable way point
      wpNew.idParent = wpCur.idWaypoint; // parent ID for new is current ID
      wpNew.costH = (long)Waypoints[wpNew.idWaypoint].pos.getDistanceFrom(Waypoints[idDest].pos);
      wpNew.costG = wpCur.costG + Waypoints[wpCur.idWaypoint].next[t].Distanz;
      wpNew.costF = wpNew.costG + wpNew.costH;
      // add to open list (position in array depends on costF)
      int pInsert = 0, pExist = -1;
      // is new in closed list !?!?!
      for(unsigned int r = 0; r < lstClosed.size(); r++){
        if(lstClosed[r].idWaypoint == wpNew.idWaypoint){
          pInsert = -1;
          break;
        }
      }
      if(pInsert >= 0){
        // check for position and existance in open list
        for(unsigned int r = 0; r < lstOpen.size(); r++){
          // way point already in open list !!!
          if(lstOpen[r].idWaypoint == wpNew.idWaypoint) pExist = r;
          // position for insert
          if(lstOpen[r].costF < wpNew.costF) pInsert = r + 1;
        }
        if(pExist >= 0){
          // erase existing in open list if it has more costF !!!
          if(lstOpen[pExist].costF > wpNew.costF){
            lstOpen.erase(pExist);
          }else{
            // do not add to open list
            pInsert = -1;
          }
        }
        // add as new to open list
        if(pInsert >= 0){
          // is new waypoint = destination, then path was found !!!
          if(wpNew.idWaypoint == idDest){
            pFound = true;
            lstClosed.push_front(wpNew);
          }else{
            if(pInsert >= lstOpen.size()){
              lstOpen.push_back(wpNew);
            }else{
              lstOpen.insert(wpNew, pInsert);
            }
          }
        }
      }
    }
  }
  Enemies[idEnemy].route.clear();
  if(pFound){
    // path found
    // walk back to starting point and add waypoints to path
    datAStar wpNew = lstClosed[0];
    do{
      Enemies[idEnemy].route.push_front(wpNew.idWaypoint);
      // find parent and set it as new (current)
      for(unsigned int r = 0; r < lstClosed.size(); r++){
        if(lstClosed[r].idWaypoint == wpNew.idParent){
          wpNew = lstClosed[r];
          break;
        }
      }
    }while(wpNew.idWaypoint != idStart);
  }
}
