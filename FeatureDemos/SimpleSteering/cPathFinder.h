#ifndef CPATHFINDER_H
#define CPATHFINDER_H

#include "stdDefs.h"

class cPathFinder{
  private:

  public:
    cPathFinder();
    virtual ~cPathFinder();

    void getRoute(int idStart, int idDest, int idEnemy);
};

extern array<datWaypoint> Waypoints;
extern array<datEnemy> Enemies;

#endif // CPATHFINDER_H
