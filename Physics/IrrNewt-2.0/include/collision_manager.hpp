// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#ifndef IRRNEWT_FRAMEWORK_COLLISION_MANAGER_HPP_ASDBASEGAEF52616816AVEAWEWASFAS
#define IRRNEWT_FRAMEWORK_COLLISION_MANAGER_HPP_ASDBASEGAEF52616816AVEAWEWASFAS
#pragma once

#include <Newton.h>
#include "dll_exports.hpp"
#include <irrlicht.h>
#include "irrnewt_base.hpp"
#include "intersection_point.hpp"
#include "stdio.h" //NULL declaration

namespace irr{
	namespace newton{

		//struct SIntersectionPoint;
		class IBody;

		//!Class to check collision and raycasting
		class IRRNEWT_API ICollisionManager:public irr::newton::IrrNewt_Base {
		public:

			//!constructor. don't call it directly
			ICollisionManager(NewtonWorld* newton_world);
			ICollisionManager();

			//!intersects a line with a body and get the first intersection point
			//!\param body the body with the line intersects
			//!\param line the line
			//!\param out the returned intersection point. After call this function this parameters will contain the intersection point
			//!\return true if a collision is cecked,false if not
			//!\see irr::newton::IntersectionPoint
			bool getCollisionPoint(
				irr::newton::IBody* body,
				irr::core::line3d<irr::f32> line,
				irr::newton::SIntersectionPoint& out);

			//!intersects a line with all bodies and get all intersection points
			//!\param line the line
			//!\param intersect_points an array of intersection points. After call this function this array will contain all intersection points. An empty array indicates a non collision cecked
			//!\return none
			//!\see irr::newton::IntersectionPoint
			void getCollisionPoints(
				irr::core::line3d<irr::f32> line,
				irr::core::array<SIntersectionPoint>* intersect_points);

			//!intersects a line with all bodies and get the first intersection point checked with the first body
			//!\param line the line
			//!\return the returned intersection point. If returned_intersection_point.body==NULL no intersection point has been checked
			//!\see irr::newton::IntersectionPoint
			irr::newton::SIntersectionPoint getCollisionFirstPoint(
				irr::core::line3d<irr::f32> line);

			//!like getCollisionFirstPoint, more precise but also a bit slower
			irr::newton::SIntersectionPoint getCollisionFirstPointEx(
				irr::core::line3d<irr::f32> line);

			//!like getCollisionFirstPoint, more precise but also a bit slower
			irr::newton::SIntersectionPoint getCollisionFirstPointEx(
				irr::core::line3d<irr::f32> line,core::array<NewtonBody*> excluded);

			/*!
			Calculate the closest points between two disjoint collision primitive.
			\param body_a pointer to the first body
			\param body_b pointer to the second body
			\param contactA the returned closest point to body_a
			\param contactB the returned closest point to body_b
			\param normal the returned separating vector normal.
			\return true if the two bodies are disjoint and the closest point could be found, false if the point is inside the primitive.
			\remarks the current implementation of this function does not work on collision trees, or user define collision. This is a Newton limitation
			\remarks This function can be used as a low-level building block for a stand-alone collision system. Applications that have already there own physics system, and only want and quick and fast collision solution, can use Newton advanced collision engine as the low level collision detection part. To do this the application only needs to initialize Newton, create the collision primitives at application discretion, and just call this function when the objects are in close proximity. Applications using Newton as a collision system only, are responsible for implementing their own broad phase collision determination, based on any high level tree structure. Also the application should implement their own trivial aabb test, before calling this function .
			*/
			bool getClosestPoint(
				irr::newton::IBody* body_a,
				irr::newton::IBody* body_b,
				core::vector3df& contactA,
				core::vector3df& contactB,
				core::vector3df& normal);

			/*!
			Calculate the closest point between a point and collision primitive.
			\param body the body
			\param point the point (must be extern to the body)
			\param contact the returned closest point to body from point
			\param normal the returned separating vector normal.
			\return true if the two bodies are disjoint and the closest point could be found, false if the point is inside the primitive.
			\remarks the current implementation of this function does not work on collision trees, or user define collision. This is a Newton limitation
			\remarks This function can be used as a low-level building block for a stand-alone collision system. Applications that have already there own physics system, and only want and quick and fast collision solution, can use Newton advanced collision engine as the low level collision detection part. To do this the application only needs to initialize Newton, create the collision primitives at application discretion, and just call this function when the objects are in close proximity. Applications using Newton as a collision system only, are responsible for implementing their own broad phase collision determination, based on any high level tree structure. Also the application should implement their own trivial aabb test, before calling this function .
			*/
			bool getClosestPoint(
				irr::newton::IBody* body,
				core::vector3df point,
				core::vector3df& contact,
				core::vector3df& normal);

			/*!
			Calculate contact points between two collision primitive.
			\param body_a the first body
			\param body_b the second body
			\param maxSize size of maximum number of elements in contacts, normals, and penetrations.
			\param contacts the returned collision contacts points
			\param normals the returned collision contact normals
			\param penetrations the returned collision penetrations at each contact.
			\return true if collision is cheked, false if the two bodies is not colliding
			\remarks This function clean contacts,normals and penetrations array before push in them the related informations
			\remarks This function can be used as a low-level building block for a stand-alone collision system. Applications that have already there own physics system, and only want and quick and fast collision solution, can use Newton advanced collision engine as the low level collision detection part. To do this the application only needs to initialize Newton, create the collision primitives at application discretion, and just call this function when the objects are in close proximity. Applications using Newton as a collision system only, are responsible for implementing their own broad phase collision determination, based on any high level tree structure. Also the application should implement their own trivial aabb test, before calling this function .
			*/
			bool collide(
				irr::newton::IBody* body_a,
				irr::newton::IBody* body_b,
				irr::u32 maxSize,
				irr::core::array<irr::core::vector3df>& contacts,
				irr::core::array<irr::core::vector3df>& normals,
				irr::core::array<irr::f32>& penetrations);


		private:

			friend class IWorld;
			NewtonWorld* n_world;
			void reserved_destroy() {}


		};

	}
}

//---------INLINE FUNCTIONS---------------------------

#endif
