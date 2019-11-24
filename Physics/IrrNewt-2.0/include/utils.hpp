// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#ifndef IRRNEWT_FRAMEWORK_UTILS_HPP_416516816AVAVASDAWDW651686SDBRHSTSADFAWQEW
#define IRRNEWT_FRAMEWORK_UTILS_HPP_416516816AVAVASDAWDW651686SDBRHSTSADFAWQEW
#pragma once

#include <irrlicht.h>
#include <Newton.h>
#include <math.h>

#include "irrnewt_base.hpp"
#include "dll_exports.hpp"
#include "structures.hpp"

//#include "joint_upvector.hpp"

namespace irr{
	namespace newton{

		class IWorld;
		class IBody;
		class ICameraBodyFPS;
		struct STire;
		struct SIntersectionPoint;

		struct STriangleInfoData {
			core::vector2d< f32 >*  TCoords[3]; //array of 3 elements, 1 per vertex of the triangle
		    core::triangle3df Triangle;
		};

		struct SMeshInfo {
			scene::IMesh* Mesh;
			core::array<STriangleInfoData> Triangles;
		};

		struct SMeshInfoArray {
			core::array<SMeshInfo> MeshInfos;
		};


		//-------------------------------------------------------------
		class IRRNEWT_API IUtils:public irr::newton::IrrNewt_Base{

			void reserved_destroy() {this->~IUtils();}
			irr::newton::IWorld* world;
			NewtonWorld* n_world;

		public:

			//!constructor. don't call this directly
			IUtils(irr::newton::IWorld* f_world);
			IUtils();

			//!destructor
			virtual ~IUtils() {return;}

			//!Automatic calculate tire width
			irr::f32 calculateTireWidth(scene::ISceneNode* tire_node);

			//!Automatic calculate tire radius
			irr::f32 calculateTireRadius(scene::ISceneNode* tire_node);

			//!Automatic calculate tire width and radius
			inline void calculateTireWidth(STire& tire) {tire.Width=this->calculateTireWidth(tire.Node);}
			inline void calculateTireRadius(STire& tire) {tire.Radius=this->calculateTireRadius(tire.Node);}

			//!Automatic calculate tire width and radius
			inline void calculateTireWidth(SCar& car) {car.TiresWidth=this->calculateTireWidth(car.TireNodeFR);}
			inline void calculateTireRadius(SCar& car) {car.TiresRadius=this->calculateTireRadius(car.TireNodeFR);}

			//!Positiong car tires
			void calculateCarTiresPosition(SCar car,irr::core::vector3df position_offset_front, irr::core::vector3df position_offset_back);

			//!random number generator using Mersenne Twister algorithm
			irr::s32 randomS32();

			//!random number generator using Mersenne Twister algorithm in the half open interval [0,1[
			irr::f64 randomF64HalfOpen();

			//!random number generator using Mersenne Twister algorithm in the open interval ]0,1[
			irr::f64 randomF64Open();

			//!random number generator using Mersenne Twister algorithm in the closed interval [0,1]
			irr::f64 randomF64Closed();

			//generate a random vector using mersenne twinster algorithm
			inline irr::core::vector3df randomVector() {return core::vector3df(
				this->randomS32(),
				this->randomS32(),
				this->randomS32()
				);
			}

			irr::f32 getDistanceFromFloor(newton::IBody* body, irr::newton::SIntersectionPoint& i_point);

			//!get middle point between two nodes. useful in some case to calculate joint pivot
			irr::core::vector3df getMiddlePoint(irr::scene::ISceneNode* node1,
				irr::scene::ISceneNode* node2);

			//!get a body from a node id (you have created the body before)
			irr::newton::IBody* getBodyFromNodeID(irr::s32 id);

			//!get a body from a node (you have created the body before)
			irr::newton::IBody* getBodyFromNode(scene::ISceneNode* node);

			//!approximate a float to first decimal value
			inline void round(irr::f32& value, irr::u32 decimal);

			//!approximate every value of a vector to first decimal value
			inline void round(irr::core::vector3df& value, irr::u32 decimal);

			newton::IBody* launchCube(irr::f32 size);

			//!make the body don't rotate in all aces
			//!uses up vector joints
			void avoidRotationOnAllAxes(irr::newton::IBody* body);

			//!sorts an array of points by distance from another given point
			irr::core::array<SIntersectionPoint> sortPointsByDistance(
				irr::core::array<SIntersectionPoint>& points,
				core::vector3df start_point);

			//!return the closest point from a given point
			irr::newton::SIntersectionPoint getClosestPoint(
				irr::core::array<SIntersectionPoint>& points,
				core::vector3df start_point);

			//!only for backward compatibility
			//!prefer using IWorld::createCameraFPS()
			void updateCameraFPS(
				irr::newton::IBody* cameraBody,
				irr::f32 mouseSensitivity,
				irr::f32 moveSpeed,
				irr::newton::SDirection direction,
				bool verticalMovement,
				bool only_y,
				bool stop_if_not_moving);

			//!Get a polygon with vertices more than 3 and return a vector of polygons all with  vertices
			core::array<newton::SPolygon> getTriangles(newton::SPolygon poly);

			//!Get all vertices from an array of generic polys
			core::array<core::vector3df> getAllVertices(core::array<newton::SPolygon> poly);

			//!Get all vertices from an array of generic triangles
			core::array<core::vector3df> getAllVertices(core::array<core::triangle3df> poly);

			//!Get all values on x,y,z from an array of vector
			core::array<f32> getAsFloats(core::array<core::vector3df> poly);


			//!Get all poyls from a mesh in irrlicht units
			core::array<core::triangle3df> getPolys(
										scene::IMesh* iMeshBase,
										core::vector3df scaleValue,
										float excludeAreaLess,
										irr::u32& skippedFace
										);

			//!Get all poyls from a mesh in newton units
			core::array<core::triangle3df> getPolysNewton(
										scene::IMesh* iMeshBase,
										core::vector3df scaleValue,
										float excludeAreaLess,
										irr::u32& skippedFace
										);

			int getMeshInfoFromMesh(scene::IMesh* mesh);
			inline SMeshInfo getMeshInfo(int pos) {return this->meshInfos.MeshInfos[pos];}

			scene::ISceneNode* buildDefaultRagdoll(
				core::vector3df loc,
				 video::ITexture* texture
				);


		private:

			friend class IWorld;
			friend class ICameraBodyFPS;
			SMeshInfoArray meshInfos;

		};



	}
}

//-----------------INLINE FUNCTIONS-----------------------------------
inline void irr::newton::IUtils::round(irr::f32& value, irr::u32 decimal) {

	irr::f32 save=(irr::f32)pow(10,decimal);

	value*=save;

	if(value>0)
		value=floorf(value);

	else
		value=ceilf(value);

	value/=save;
}

inline void irr::newton::IUtils::round(irr::core::vector3df& value, irr::u32 decimal) {
	this->round(value.X, decimal);
	this->round(value.Y, decimal);
	this->round(value.Z, decimal);
}

#endif
