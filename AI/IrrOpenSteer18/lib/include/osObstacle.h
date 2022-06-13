/*! \file osObstacle.h
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
#include "../../../../config.h"
#ifdef OPENSTEER
#ifndef OPENSTEER_OBSTACLE_H
#define OPENSTEER_OBSTACLE_H

#include "osVec3.h"
#include "osLocalSpace.h"
#include "osAbstractVehicle.h"

//! namespace OpenSteer
namespace OpenSteer {
  // Forward declaration.
  class Color;

  //! Obstacle types
  enum E_OBSTACLE_TYPE{
    EOS_UNKNOWN = 0, /*!< Unkown obstacle type */
    EOS_SPHERE, /*!<  Sphere obstacle */
    EOS_BOX, /*!< Box obstacle */
    EOS_RECTANGLE, /*!< Rectangle obstacle */

    EOS_COUNT /*!< count of obsracle types (don't use) */
  };

  //! AbstractObstacle: a pure virtual base class for an abstract shape in space, to be used with obstacle avoidance. (Oops, its not "pure" since I added a concrete method to PathIntersection 11-04-04 -cwr).
  class AbstractObstacle{
    public:

      virtual ~AbstractObstacle() { /* Nothing to do. */ }

      //! get type of the obstacle
      E_OBSTACLE_TYPE getType(){ return EOS_UNKNOWN; }

      //! compute steering for a vehicle to avoid this obstacle, if needed
      virtual Vec3 steerToAvoid(const AbstractVehicle& v, const float minTimeToCollision) const = 0;

      //! PathIntersection object
      /*! used internally to analyze and store information about intersections of vehicle paths and obstacles. */
      class PathIntersection{
        public:
          bool intersect;                   /*!< was an intersection found? */
          float distance;                   /*!< how far was intersection point from vehicle? */
          Vec3 surfacePoint;                /*!< position of intersection */
          Vec3 surfaceNormal;               /*!< unit normal at point of intersection */
          Vec3 steerHint;                   /*!< where to steer away from intersection */
          bool vehicleOutside;              /*!< is the vehicle outside the obstacle? */
          const AbstractObstacle* obstacle; /*!< obstacle the path intersects */

          //! determine steering based on path intersection tests
          Vec3 steerToAvoidIfNeeded(const AbstractVehicle& vehicle, const float minTimeToCollision) const;
      };

      //! find first intersection of a vehicle's path with this obstacle (this must be specialized for each new obstacle shape class)
      virtual void findIntersectionWithVehiclePath(const AbstractVehicle& vehicle, PathIntersection& pi) const = 0 ;

      //! virtual function for drawing
      /*! normally does nothing, can be specialized by derived types to provide graphics for obstacles */
      virtual void draw(const bool filled, const Color& color, const Vec3& viewpoint) const = 0 ;

      //! seenFrom (eversion)
      /*! does this obstacle contrain vehicle to stay inside  it or outside it (or both)?<br>
          "Inside" describes a clear space within a solid  (for example, the interior
          of a room inside its walls).<br>
          "Ouitside" describes a solid chunk in the midst of clearspace. */
      enum seenFromState{outside, inside, both};
      virtual seenFromState seenFrom(void) const = 0;
      //! set seenFrom (eversion)
      virtual void setSeenFrom(seenFromState s) = 0;

      //! render methode
      virtual void render(irr::video::IVideoDriver* driver)=0;
  };
  //! an STL vector of AbstractObstacle pointers
  typedef std::vector<AbstractObstacle*> ObstacleGroup;
  //! an iterator for the ObstacleGroup
  typedef ObstacleGroup::const_iterator ObstacleIterator;
  //! Obstacle is a utility base class providing some shared functionality
  class Obstacle : public AbstractObstacle{
    private:
      seenFromState _seenFrom;

    public:
      Obstacle(void) : _seenFrom(outside){}
      virtual ~Obstacle() { /* Nothing to do. */ }

      //! compute steering for a vehicle to avoid this obstacle, if needed
      Vec3 steerToAvoid(const AbstractVehicle& v, const float minTimeToCollision) const;

      //! static method to apply steerToAvoid to nearest obstacle in an ObstacleGroup
      static Vec3 steerToAvoidObstacles(const AbstractVehicle& vehicle,
                                         const float minTimeToCollision,
                                         const ObstacleGroup& obstacles);

      //! static method to find first vehicle path intersection in an ObstacleGroup
      static void firstPathIntersectionWithObstacleGroup(const AbstractVehicle& vehicle,
                                              const ObstacleGroup& obstacles,
                                              PathIntersection& nearest,
                                              PathIntersection& next);

      //! default do-nothing draw function (derived class can overload this)
      void draw(const bool, const Color&, const Vec3&) const {}

      //! Irrlicht render methode
      void render(irr::video::IVideoDriver* driver){}

      //! set position
      void setPosition(float x, float y, float z){}
      //! set position
      void setPosition(Vec3 p){}
      //! set position
      void setPosition3df(irr::core::vector3df p){}
      //! get position
      Vec3 getPosition(){}
      //! get position
      irr::core::vector3df getPosition3df(){}
      //! set rotation
      void setRotation(Vec3 r){}
      //! set rotation
      void setRotation3df(irr::core::vector3df r){}
      //! get rotation
      Vec3 getRotation(){}
      //! get rotation
      irr::core::vector3df getRotation3df(){}
      //! set size
      void setSize(Vec3 s){}
      //! set size
      void setSize3df(irr::core::vector3df s){}
      //! get size
      Vec3 getSize(){}
      //! get size
      irr::core::vector3df getSize3df(){}
      //! set size
      void setSize(float w, float h, float d){}
      //! set width
      void setWidth(float w){}
      //! get width
      float getWidth(){}
      //! set height
      void setHeight(float h){}
      //! get height
      float getHeight(){}
      //! set depth
      void setDepth(float d){}
      //! get depth
      float getDepth(){}

      //! seenFrom (eversion)
      /*! does this obstacle contrain vehicle to stay inside  it or outside it (or both)?<br>
          "Inside" describes a clear space within a solid  (for example, the interior
          of a room inside its walls).<br>
          "Ouitside" describes a solid chunk in the midst of clearspace. */
      seenFromState seenFrom(void) const {return _seenFrom;}
      //! set seenFrom (eversion)
      void setSeenFrom(seenFromState s) {_seenFrom = s;}

  };

  //! LocalSpaceObstacle: a mixture of LocalSpace and Obstacle methods
  typedef LocalSpaceMixin<Obstacle> LocalSpaceObstacle;

// --------------------------- Obstacles -------------------------------------------------
  //! SphereObstacle a simple ball-shaped obstacle
  class SphereObstacle : public Obstacle{
    private:
      float _radius; /*!< The radius */
      Vec3 _position; /*!< The center */
      //! Debug render struff
      irr::core::array<irr::video::S3DVertex> lstVertices;
      irr::core::array<irr::u16> lstIndices;
      void createMesh();

    public:

      //! get type of the obstacle
      E_OBSTACLE_TYPE getType(){ return EOS_SPHERE; }

      //! constructor
      SphereObstacle(float r, Vec3 c) : _radius(r), _position (c) {}
      //! constructor
      SphereObstacle(void) : _radius(1), _position (Vec3::zero) {}
      virtual ~SphereObstacle() { /* Nothing to do. */ }

      //! find first intersection of a vehicle's path with this obstacle
      void findIntersectionWithVehiclePath(const AbstractVehicle& vehicle, PathIntersection& pi) const;

      //! set position
      void position(Vec3 p);
      //! get position
      Vec3 position();
      //! set position
      void position3df(irr::core::vector3df p);
      //! get position
      irr::core::vector3df position3df();
      //! set radius
      void radius(float r);
      //! get radius
      float radius();

      //! render methode
      virtual void render(irr::video::IVideoDriver* driver);

  };
  //! BoxObstacle: a box-shaped (cuboid) obstacle of a given height, width, depth, position and orientation.
  /*! The box is centered on and aligned with a local space. */
  class BoxObstacle : public LocalSpaceObstacle{
    private:
      float _width;  /*!< width  of box centered on local X (side)    axis */
      float _height; /*!< height of box centered on local Y (up)      axis */
      float _depth;  /*!< depth  of box centered on local Z (forward) axis */
      //! Debug render struff
      irr::core::array<irr::video::S3DVertex> lstVertices;
      irr::core::array<irr::u16> lstIndices;
      void createMesh();

    public:
      //! get type of the obstacle
      E_OBSTACLE_TYPE getType(){ return EOS_BOX; }

      //! constructor
      /*! \param w Width
          \param h Height
          \param d Depth */
      BoxObstacle(float w, float h, float d) : _width(w), _height(h), _depth(d) {}
      //! constructor
      BoxObstacle(void) :  _width(1.0f), _height(1.0f), _depth(1.0f) {}
      virtual ~BoxObstacle() { /* Nothing to do. */ }

      //! set size
      void size(Vec3 s);
      //! get size
      Vec3 size();
      //! set size
      void size3df(irr::core::vector3df s);
      //! get size
      irr::core::vector3df size3df();
      //! get width
      float width();
      //! get height
      float height();
      //! get depth
      float depth();
      //! set width
      void width(float w);
      //! set height
      void height(float h);
      //! set depth
      void depth(float d);

      //! find first intersection of a vehicle's path with this obstacle
      void findIntersectionWithVehiclePath(const AbstractVehicle& vehicle, PathIntersection& pi) const;

      //! render methode
      virtual void render(irr::video::IVideoDriver* driver);

  };
  //! PlaneObstacle: a planar obstacle of a given position and orientation.
  /*! The plane is defined as the XY (aka side/up) plane of a local space.
      The +Z (forward) half-space is considered "outside" the obstacle.<br>
      This is also the base class for several other obstacles which represent
      2d shapes (rectangle, triangle, ...) arbitarily oriented and positioned
      in 2d space.  They specialize this class via xyPointInsideShape which
      tests if a given point on the XZ plane is inside the obstacle's shape. */
  class PlaneObstacle : public LocalSpaceObstacle{
    private:
      irr::core::array<irr::video::S3DVertex> lstVertices;
      irr::core::array<irr::u16> lstIndices;
      void createMesh();

    public:
      //! get type of the obstacle
      E_OBSTACLE_TYPE getType(){ return EOS_UNKNOWN; }
      //! constructor
      PlaneObstacle(void){}
      //! constructor
      /*! \param s Side vector
          \param u Up vector
          \param f Forward vector
          \param p Position vector */
      PlaneObstacle(const Vec3& s, const Vec3& u, const Vec3& f, const Vec3& p)
      : LocalSpaceObstacle(s, u, f, p){}

      //! find first intersection of a vehicle's path with this obstacle
      void findIntersectionWithVehiclePath(const AbstractVehicle& vehicle, PathIntersection& pi) const;
      //! determines if a given point on XY plane is inside obstacle shape
      virtual bool xyPointInsideShape (const Vec3& p, float r) const{
        return true; // always true for PlaneObstacle
      }
      //! Irrlicht render methode
      virtual void render(irr::video::IVideoDriver* driver);

  };
  //! RectangleObstacle: a rectangular obstacle of a given height, width, position and orientation.
  /*! It is a rectangle centered on the XY (aka side/up) plane of a local space. */
  class RectangleObstacle : public PlaneObstacle{
    private:
      float _width;  /*!< width  of rectangle centered on local X (side) axis */
      float _height; /*!< height of rectangle centered on local Y (up)   axis */
      irr::core::array<irr::video::S3DVertex> lstVertices;
      irr::core::array<irr::u16> lstIndices;
      void createMesh();

    public:

      //! get type of the obstacle
      E_OBSTACLE_TYPE getType(){ return EOS_RECTANGLE; }
      //! constructor
      RectangleObstacle (float w, float h) : _width(w), _height(h) {}
      //! constructor
      RectangleObstacle (void) :  _width(1.0f), _height(1.0f) {}
      //! constructor
      /*! \param w Width
          \param h Height
          \param s Side vector
          \param u Up vector
          \param f Forward vector
          \param p Position vector
          \param sf SeenFrom state */
      RectangleObstacle (float w, float h, const Vec3& s,
                         const Vec3& u, const Vec3& f, const Vec3& p,
                         seenFromState sf)
      : PlaneObstacle( s, u, f, p ), _width(w), _height(h){
        setSeenFrom (sf);
      }
      virtual ~RectangleObstacle() { /* Nothing to do. */ }

      //! determines if a given point on XY plane is inside obstacle shape
      bool xyPointInsideShape (const Vec3& point, float _radius) const;
      //! get width
      float width();
      //! get height
      float height();
      //! set width
      void width(float w);
      //! set height
      void height(float h);

      //! Irrlicht render methode
      virtual void render(irr::video::IVideoDriver* driver);

  };

} // namespace OpenSteer

#endif // OPENSTEER_OBSTACLE_H
#endif
