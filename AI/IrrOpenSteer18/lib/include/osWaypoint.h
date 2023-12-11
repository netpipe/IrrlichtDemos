#ifdef OPENSTEER
/*! \file osWaypoint.h
    ----------------------------------------------------------------------------<br>
    Irrlicht implementation by A.Buschhüter<br>
    http://abusoft.g0dsoft.com<br>
    abusoft@g0dsoft.com<br>
    ----------------------------------------------------------------------------<br>
    OpenSteer -- Steering Behaviors for Autonomous Characters<br>
    <br>
    Copyright (c) 2002-2005, Sony Computer Entertainment America<br>
    Original author: Craig Reynolds <craig_reynolds@playstation.sony.com><br>
    <br>
    Permission is hereby granted, free of charge, to any person obtaining a<br>
    copy of this software and associated documentation files (the "Software"),<br>
    to deal in the Software without restriction, including without limitation<br>
    the rights to use, copy, modify, merge, publish, distribute, sublicense,<br>
    and/or sell copies of the Software, and to permit persons to whom the<br>
    Software is furnished to do so, subject to the following conditions:<br>
    <br>
    The above copyright notice and this permission notice shall be included in<br>
    all copies or substantial portions of the Software.<br>
    <br>
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR<br>
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,<br>
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL<br>
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER<br>
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING<br>
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER<br>
    DEALINGS IN THE SOFTWARE.<br>
    ----------------------------------------------------------------------------<br>
*/
#ifndef _OS_WAYPOINT_H_INCLUDED_
#define _OS_WAYPOINT_H_INCLUDED_

#include "IrrOpenSteer.h"

using namespace irr;
//! namespace OpenSteer
namespace OpenSteer{
  struct datAnnotation;
  class WaypointMap;
  class Waypoint;
  struct WP_Reachable{
    Waypoint* _waypoint;
    u32 _distance;
  };
  struct AS_Data{
    Waypoint* Parent;
    u32 F;
    u32 G;
    u32 H;
  };

  //! The Waypoint class
  class Waypoint{
    private:
      int ID;
      core::stringc Name;
      WaypointMap* wpMap;
      Vec3 _position;
      core::array<WP_Reachable> lstReachables;
      AS_Data datAStar;

      // get the list index of the waypoint
      int getWaypointID(Waypoint* wp);
      int getWaypointID(core::stringc name);
      int getWaypointID(int id);

    public:
      //! constructor
      Waypoint(WaypointMap* wpm, Vec3 pos, int id = -1, core::stringc name = "");

      int getID();
      void setID(int id);
      core::stringc getName();
      void setName(core::stringc name);

      //! set the position
      void setPosition(Vec3 p);
      //! get the position
      Vec3 getPosition();
      //! update the distance to a waypoint
      void updateDistances();

      //! add a reachable waypoint to the waypoint
      void addReachable(Waypoint* wp);
      void addReachable(core::stringc name);
      void addReachable(int id);
      //! delete a reachable waypoint from the waypoint
      void removeReachable(Waypoint* wp);
      void removeReachable(core::stringc name);
      void removeReachable(int id);
      //! get the "reachable waypoints" array
      core::array<WP_Reachable>* getReachableArray();

      //! add render mesh
      void addMesh(datAnnotation* ann, bool showConnections, video::SColor colWP, video::SColor colCon, float radius, float height);

  };

} // namespace OpenSteer

#endif // _OS_WAYPOINT_H_INCLUDED_
#endif
