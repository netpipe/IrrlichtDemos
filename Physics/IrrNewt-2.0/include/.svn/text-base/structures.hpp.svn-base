// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#ifndef IRRNEWT_FRAMEWORK_STRUCTURES_HPP_416516
#define IRRNEWT_FRAMEWORK_STRUCTURES_HPP_416516
#pragma once

#include <irrlicht.h>
#include <Newton.h>
#include "pos_rot.hpp"
#include "irrtonewt.hpp"

namespace irr{
	namespace newton{

		class IBody;


		//---------------------------------------------------------------------------------------
		//ENUMS
		//--------------------------------------------------------------------------------------

		//!enum to define a fps camera type
		enum E_CAMERA_FPS_TYPE {
			//!rotate on x and y. move on x, y and z
			ECFT_NORMAL,
			//!rotate on x and y, move on x and z (speed of movement depend on camera x rotation)
			ECFT_NO_VERTICAL,
			//!rotate on x and y, move on x and z (speed of movement doesn't depend on camera x rotation)
			ECFT_ONLY_Y
		};


		//!use hardware mode structure. Specifing if use maximum resource of the hardware or as less resource as possible
		enum E_USE_HARDWARE_MODE{
			EUHM_MINIMUM,
			EUHM_MEDIUM,
			EUHM_MAXIMUM
		};

		//!specifing the optimization mode (faster, normal or more precise?)
		enum E_OPTIMIZATION_MODE{

			//!Fast and not precise
			EOM_FAST,

			//!Quite fast and quite precise
			EOM_MEDIUM,

			//!Precise and slow
			EOM_PRECISE
		};


		//!How to automatic calculate offset
		enum E_CALCULATE_OFFSET {

			//!Don't calculate offset automatically and use ONLY
			//!the user specified offset in SBodyFromNode::BodyOffsetFromNode
			ECO_NONE,

			//!Try to calculate the offset from the node and add
			//!the result to the user specified offset in SBodyFromNode::BodyOffsetFromNode
			ECO_NODE,


			//!Try to calculate the offset from the mesh and add
			//!the result to the user specified offset in SBodyFromNode::BodyOffsetFromNode
			//!If there is no mesh this option is the same as ECO_NODE
			//!this is the default option
			ECO_MESH
		};

		//!HOw to automatically calculate the collision size (only for primitive bodies)
		enum E_CALCULATE_SIZE {

			//!Calculate the collision size from node and add the result to SBodyFromNode::Size
			ECS_NODE,

			//!Calculate shape size from mesh and add the result to SBodyFromNode::Size
			//!If there is no mesh this is the same as ECS_NODE. This is the defualt option
			ECS_MESH,

			//!Don't calculate size automatically and use only the size sfecified in SBodyFromNode::Size
			ECS_NONE
		};

		//!type of the body
		enum E_BODY_TYPE {

			//!autodetect appropiate body type
			EBT_AUTODETECT = 0,

			//!body with primitive box collision
			EBT_PRIMITIVE_BOX,

			//!body with primitive ellipsoid collision
			EBT_PRIMITIVE_ELLIPSOID,

			//!body with primitive cylinder collision
			EBT_PRIMITIVE_CYLINDER,

			//!body with primitive chamfer cylinder collision
			EBT_PRIMITIVE_CHAMFER_CYLINDER,

			//!body with primitive capsule collision
			EBT_PRIMITIVE_CAPSULE,

			//!body with primitive cone collision
			EBT_PRIMITIVE_CONE,

			//!Body with convex hull collision. The shape of the body is the same as the shape of the node.
			//!This is slower than a primitive collision but more precise if the node is not a primitive
			//!a convex collision body can have no infinite (0) mass, so it can move, be affected by force, exc..
			//!differently from a tree body
			EBT_CONVEX_HULL,

			//!The same as EBT_CONVEX_HULL, but this make real-time scaling ( IBody::setScale() and IBody::setScaleBody() ) faster
			EBT_CONVEX_HULL_MODIFIER,

			//!Create a transparent collision primitive.
			//!\remarks Some times the application needs to create helper rigid bodies that will never collide with other bodies, for example the neck of a rag doll, or an internal part of an articulated structure. This can be done by using the material system but it too much work and it will increase unnecessarily the material count, and therefore the project complexity. The Null collision is a collision object that satisfy all this conditions without having to change the engine philosophy.
			EBT_NULL,

			//!body with optimized tree collision
			//!The shape of the body is the same as the shape of the node.
			//!A body with a tree collision must have infinite mass. So it can't move, can't be affectd by force, exc..
			//!But it will be optimized (faster than convex hull). Use this for static node like level
			//!Highly reccomanded to use this body type for oct tree nodes
			EBT_TREE,

			//!body with optimized tree collision (see EBT_TREE) but create using addTerrainSceneNode() irrlicht functionù
			//!for irrlicht terrain created frm a heighmap (addTerrainSceneNode) you can't use EBT_TREE. Use this type instead
			EBT_TREE_TERRAIN,

			//!a body wich contained mutiple collision objects
			EBT_COMPOUND_COLLISION,

			//!not a valid body type. This indicates how many body types contain this enum
			EBT_COUNT
		};

		//---------------------------------------------------------------------------------------
		//STRUCTURES
		//---------------------------------------------------------------------------------------

		//!structure for camera FPS. contain moving direction
		struct SDirection {
			SDirection(bool up,bool down,bool left,bool right):Up(up),Down(down),Left(left),Right(right) {}
			SDirection():Up(false),Down(false),Left(false),Right(false) {}

			bool Up/*!if true move up*/, Down/*!if true move down*/, Left/*!if true strafe left*/, Right/*!if true strafe right*/;

			inline bool isMoving() {return (this->Up||this->Down||this->Left||this->Right); }
		};

		//!structure to hold a single polygon data of any type (quad,triangle, general polygon of n sides
		struct SPolygon {

			//!all vertices wich constitute the polygon in global space
			irr::core::array<irr::core::vector3df> Vertices;

		};

		//"structure contain data to create a body from a node
		struct SBodyFromNode{
			SBodyFromNode():Type(EBT_AUTODETECT),Node(NULL),Mesh(NULL),
				Mass(1.0f),TerrainLOD(3),BodyScaleOffsetFromNode(1,1,1),
				CalculateOffsetMode(newton::ECO_MESH),
				CalculateSizeMode(newton::ECS_MESH)  {}

			//!if true irrnewt tries to calculate automatically position offset from the body
			//!useful if in your model file the model is not centered at the origin (for example
			//!with feet at the origin)
			E_CALCULATE_OFFSET CalculateOffsetMode;

			//!GHoe calculate primitive shape dimensions
			E_CALCULATE_SIZE CalculateSizeMode;

			//!body type
			E_BODY_TYPE Type;

			//!related node
			irr::scene::ISceneNode* Node;

			//!node mesh (you must specify this for EBT_TREE and EBT_CONVEX_HULL bodies. For all other bodies type if you specify this IrrNewt calculate body dimensions from mesh, if this is NULL IrrNewt calculates body dimension directly from node)
			irr::scene::IMesh* Mesh;

			//!body mass. you can change it in real time using IBody::setMass()
			irr::f32 Mass;

			//!body position and rotation offset from node.
			//!\warning don't use this for scale offset
			irr::core::matrix4 BodyOffsetFromNode;

			//!body scale offset from node
			irr::core::vector3df BodyScaleOffsetFromNode;

			//!terrain reserved. Only if the body type is EBT_TREE_TERRAIN. Specifing level of detail of the terrain to take collision polygons
			irr::u32 TerrainLOD;

			//!Size of collision shape. Only for primitives bodies
			core::vector3df Size;
		};

		//! general joint
		struct SJoint {
			SJoint():ParentBody(NULL) {}

			//!the parent body
			irr::newton::IBody* ParentBody;
		};

		//!up vector joint
		struct SJointUpVector:SJoint{
			SJointUpVector():PinDir(core::vector3df(0.0f,1.0f,0.0f)),SJoint() {}

			//!the aligning vector
			irr::core::vector3df PinDir;
		};

		struct SJointHinge:SJoint{
			SJointHinge():PinDir(core::vector3df(0.0f,1.0f,0.0f)),SJoint() {}

			//!joint pivot
			irr::core::vector3df Pivot;

			//!aligning vector
			irr::core::vector3df PinDir;

			//!the body affected by the joint
			irr::newton::IBody* ChildBody;
		};

		struct SJointUniversal:SJoint{
			SJointUniversal():SJoint() {}

			//!joint pivot
			irr::core::vector3df Pivot;

			//!pin vector 1
			irr::core::vector3df Pin0;

			//!pin vector 2
			irr::core::vector3df Pin1;

			//!the body affected by the joint
			irr::newton::IBody* ChildBody;
		};

		//corksrew is a hinge+slider
		struct SJointCorkscrew:SJoint{
			SJointCorkscrew():SJoint() {}

			//!joint position
			irr::core::vector3df Pivot;

			//!pin vector
			irr::core::vector3df PinDir;

			//!the body affected by the joint
			irr::newton::IBody* ChildBody;
		};

		struct SJointSlider:SJoint{
			SJointSlider():PinDir(core::vector3df(0.0f,1.0f,0.0f)),SJoint() {}

			//!joint pivot
			irr::core::vector3df Pivot;

			//!pin vector
			irr::core::vector3df PinDir;

			//!the body affected by the joint
			irr::newton::IBody* ChildBody;
		};

		struct SJointBall:SJoint{
			SJointBall():SJoint() {}

			//!joint position
			irr::core::vector3df Pivot;

			//!the body affected by the joint
			irr::newton::IBody* ChildBody;
		};

		enum E_JOINT_TYPE {
			EJT_UPVECTOR = 0,
			EJT_HINGE,
			EJT_UNIVERSAL,
			EJT_CORKSCREW,
			EJT_SLIDER,
			EJT_BALL
		};

		struct SGenericJoint{
			E_JOINT_TYPE Type;
			core::vector3df Pivot;
			irr::newton::IBody* ParentBody;
			irr::newton::IBody* ChildBody;
			core::vector3df Pin0;
			core::vector3df Pin1;
		};
		//end of joints

		//!a vehicle tire structure
		struct STire{
			STire():Pin(irr::core::vector3df(0,0,1.0f)),Mass(1.0f),
				SuspensionShock(30.0f),SuspensionSpring(200.0f),SuspensionLength(1.2f),
				BodyOffsetFromSceneNode(irr::newton::SPosRot()),
				Node(NULL),Width(UNDEFINED_PARAMETER),Radius(UNDEFINED_PARAMETER) {}

			//!the rotation axis of the tire, in the space of the tire.
			irr::core::vector3df Pin;

			//!tire mass
			irr::f32 Mass;

			//!width of the tire, must be smaller than the tire radius
			//!if you don't specify it, IrrNewt calculates it automatically
			irr::f32 Width;

			//!radius of the tire
			//!if you don't specify it, IrrNewt calculates it automatically
			irr::f32 Radius;

			//!parametrized damping constant for a spring, mass, damper system. A value of one corresponds to a critically damped system.
			irr::f32 SuspensionShock;

			//!parametrized spring constant for a spring, mass, damper system. A value of one corresponds to a critically damped system.
			irr::f32 SuspensionSpring;

			//!distance from the tire set position to the upper stop on the vehicle body frame. The total suspension length is twice that.
			irr::f32 SuspensionLength;

			//!the node related to the tire
			irr::scene::ISceneNode* Node;

			//!the position and rotation tire offset from node
			irr::newton::SPosRot BodyOffsetFromSceneNode;
		};//STire

		//!a structure specific for cars
		struct SCar {
			SCar():TiresPin(irr::core::vector3df(0,0,1.0f)),UpVector(core::vector3df(0,1,0)),
				TiresMass(1.0f),
				TiresOffsetFromChassisFront(core::vector3df()),
				TiresOffsetFromChassisBack(core::vector3df()),
				SuspensionShock(30.0f),SuspensionSpring(200.0f),SuspensionLength(1.2f),
				TiresBodyOffsetFromSceneNode(irr::newton::SPosRot()),ChassisBody(NULL),
				TireNodeFR(NULL),TireNodeFL(NULL),TireNodeBR(NULL),TireNodeBL(NULL),
				TiresWidth(UNDEFINED_PARAMETER),TiresRadius(UNDEFINED_PARAMETER){}

			//!chassis body
			irr::newton::IBody* ChassisBody;

			//!the front right tire node
			irr::scene::ISceneNode* TireNodeFR;

			//!the front left tire node
			irr::scene::ISceneNode* TireNodeFL;

			//!the back right tire node
			irr::scene::ISceneNode* TireNodeBR;

			//!the back left tire node
			irr::scene::ISceneNode* TireNodeBL;

			//!the rotation axis of the tires, in the space of the tires
			irr::core::vector3df TiresPin;

			//!the mass of the tires
			irr::f32 TiresMass;

			//!see STire
			irr::f32 TiresWidth;

			//!see STire
			irr::f32 TiresRadius;

			//!see STire
			irr::f32 SuspensionShock;

			//!see STire
			irr::f32 SuspensionSpring;

			//!see STire
			irr::f32 SuspensionLength;

			//!position and rotation offset of the tires from the related nodes
			irr::newton::SPosRot TiresBodyOffsetFromSceneNode;

			//!vector inverse of gravity vector
			irr::core::vector3df UpVector;

			//!position offset of the tires from chassis
			irr::core::vector3df TiresOffsetFromChassisFront;
			irr::core::vector3df TiresOffsetFromChassisBack;


		};//SCar

		//ragdoll
		//!structure to store rag doll data
		struct SRagDoll{
			SRagDoll():bonesMass(10.0f) {}

			//!This is the main ragdoll node
			//!Here are how ragdoll must be build. Each bone MUST be parent of another bone
			//!(example chest is parent of arms and legs, legs are parents of feet, exc..)
			//!but 1 bone ONLY MUST have no parents (generally the chest or the head)
			//!This is the node with no parents
			scene::ISceneNode* mainNode;

			//!This is the initial mass of all the bones
			f32 bonesMass;
		};


		//---------------------------------------------------
		//INTERNAL USE
		//-----------------------------------------------------

		//!internal use only
		enum E_CLASS_TYPE{
			ECT_UNKNOW,
			ECT_BODY,
			ECT_TREE_BODY,
			ECT_MATERIAL,
			ECT_VEHICLE_SIMPLE,
			ECT_VEHICLE_TIRE,
			ECT_CAR,
			ECT_WORLD,
			ECT_BODY_CONTROLLER,
			ECT_CAMERA_FPS_BODY,
			ECT_COLLISION_MANAGER,
			ECT_JOINT,
			ECT_UTILS,
			ECT_COUNT
		};

		//!internal use only
		struct SClassType{
			SClassType():Type(ECT_UNKNOW) {}
			E_CLASS_TYPE Type;
		};
		//---------------------------------------------------
		//END OF INTERNAL USE
		//-----------------------------------------------------

	}
}

#endif
