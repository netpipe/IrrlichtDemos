#include "../../../../config.h"
#ifdef OPENSTEER
/*! \file osWaypointMap.h
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
#ifndef _OS_WAYPOINT_MAP_H_INCLUDED_
#define _OS_WAYPOINT_MAP_H_INCLUDED_

#include "osWaypoint.h"
#include "IrrOpenSteer.h"

using namespace irr;
//! namespace OpenSteer
namespace OpenSteer{
  class Waypoint;

  //! The WaypointMap class
  class WaypointMap{
    private:
      // list of all waypoints
      core::array<Waypoint*> lstWaypoints;
      // get the list index of the waypoint
      int getWaypointID(Waypoint* wp);
      int getWaypointID(core::stringc name);
      int getWaypointID(int id);

    public:
      // (debug-) render data
      bool isMeshDirty;
      float wp_radius;
      float wp_height;

      WaypointMap();
      ~WaypointMap();

      Waypoint* getWaypoint(core::stringc name);
      Waypoint* getWaypoint(int id);

      //! add a waypoint to the map
      Waypoint* addWaypoint(Vec3 pos);
      //! delete a waypoint from the map
      void removeWaypoint(Waypoint* wp);
      //! get the waypoint array
      core::array<Waypoint*>* getWaypointArray();

      //! update all distances to a waypoint
      void updateDistances();

      //! render the pathway of the waypoints
      void render(video::IVideoDriver* driver, bool showConnections = true, video::SColor colWP = video::SColor(255, 0,200,0), video::SColor colCon = video::SColor(255, 0,0,200));

  };

} // namespace OpenSteer

#endif // _OS_WAYPOINT_MAP_H_INCLUDED_
#endif
