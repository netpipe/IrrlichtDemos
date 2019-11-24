// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#ifndef RAFFAELE_MANCUSO_IRRNEWT_LIBRARY_PHYSICS_WORLD_HPP_CODE_5468465284961ADGASEGD5165126ASDGDSAGE516216816ADSGFADGFASF
#define RAFFAELE_MANCUSO_IRRNEWT_LIBRARY_PHYSICS_WORLD_HPP_CODE_5468465284961ADGASEGD5165126ASDGDSAGE516216816ADSGFADGFASF
#pragma once

#include <irrlicht.h>
#include <Newton.h>

#include "dll_exports.hpp"
#include "irrtonewt.hpp"
#include "irrnewt_base.hpp"
#include "structures.hpp"


#include "utils.hpp"
#include "collision_manager.hpp"
#include "collision.hpp"


namespace irr{
	namespace newton{

		namespace Hidden{
			void defBodyLeaveWorldEvent(const NewtonBody* n_body, int);
		}

	//	class IUtils;

		class IBody;
		class ITreeBody;
		class IMaterial;
		class ICharacterController;
		class IVehicleSimple;
		class IJointUpVector;
		class IJointHinge;
		class IJointUniversal;
		class IJointCorkscrew;
		class IJointBall;
		class IJointSlider;
		class IJoint;
		class IRagDoll;
		class ICar;
		class ICameraBodyFPS;
		class IVehicleTire;
		class IInternalBehaviour;
		struct SCar;
		struct SBody;
		struct SGenericJoint;



		//!internal behaviour of the wrapper
		class IInternalBehaviour:public irr::newton::IrrNewt_Base {

			virtual ~IInternalBehaviour() {}
			void reserved_destroy() {this->~IInternalBehaviour();}

		public:

			friend class IWorld;
			IInternalBehaviour():treeTriangleMinArea(1.0e-6f) {}

			//!This is the minimum area that a triangle MUST have to be a part of a tree collision.
			//!If in the mesh there are triangles with an area less than this values, they will
			//! be ignored during the collision creation, and won't become triangles of the collision
			//! only for tree collisions
			irr::f32 treeTriangleMinArea;

		}; //IInternalBehaviour



		//!Class to management newton world
		class IRRNEWT_API IWorld:public irr::newton::IrrNewt_Base{
		public:

			//!constructor. don't call this directly
			IWorld(irr::IrrlichtDevice* Device);

			//!destrructor
			virtual ~IWorld();

			//!update physics simulation. Call this every loop
			//!\return none
			void update();
			void update(irr::f32 time_step);

			//!draw all debug infos. call this between smgr->drawAll() and driver->endScene()
			//!\return none
			void drawAllDebugInfos();

			//!create a rigid body from a scene node
			//!\param body the body data
			//!\return the new body
			irr::newton::IBody*  createBody(SBodyFromNode body);

			//!create a rigid body from a scene node and a collision object
			irr::newton::IBody*  createBody(
				scene::ISceneNode* node,
				newton::ICollision* collision);

			//!create lots of rigid bodies from an array of nodes
			//!all the bodies will be created with the same creation data
			//!\param nodes the array of nodes
			//!\param data the common data for all the bodies
			//!\return an array of bodies with the same size of the array of nodes
			irr::core::array<irr::newton::IBody*> createBodies(
				irr::core::array<irr::scene::ISceneNode*> nodes,
				irr::newton::SBodyFromNode data);

			//!load a tree body from a file
			//!\param node the graphic node related to the tree body
			//!\param file path and name of the file wich contain the tree body
			//!\return the body
			irr::newton::ITreeBody* deserializeTreeBody(
				irr::scene::ISceneNode* node,
				irr::core::stringc file
				);

			//!create a new material
			//!\return the material
			irr::newton::IMaterial* createMaterial();

			//!create a collision object from a node a body type and eventually a mesh (for tree collision)
			irr::newton::ICollision* createCollision(irr::newton::SBodyFromNode data);

			//!create a collision object from a newton collision object
			irr::newton::ICollision* createCollision(NewtonCollision* nCollision);

	        //!create a unique compund collision object from lots of collision objects
			irr::newton::ICollision* createCollision( core::array<irr::newton::ICollision*> collision);

			//!create a camera FPS
			//!the node in data (data.Node) would be a simple camera (addCameraSceneNode and not addCameraSceneNodeFPS)
			//!arraow keys move the camera, mouse rotate it. You can change the keys for the camera
			//!by access the related functions in ICameraBodyFPS
			irr::newton::ICameraBodyFPS* createCameraFPS(
				irr::newton::SBodyFromNode data
				);

			//!create a character control from a body
			//!\param body the body
			//!\param erase delete the body and mantain only the CharacterController? If true you will not be able more to use the body pointer passed the function
			irr::newton::ICharacterController* createCharacterController(
				irr::newton::IBody* body,bool erase=true);

			//!creates a simple vehicle
			//!\param chassis the body wich rappresent the chassis
			//!\param up_dir the vector opposite to the gravity. Generally (0,1,0). Used to simulate suspension
			//!\return the vehicle
			irr::newton::IVehicleSimple* createVehicleSimple(
				irr::newton::IBody* chassis,
				irr::core::vector3df up_dir=irr::core::vector3df(0,1,0));

			//!create a car, a simple vehicle with 4 tires. The back tires move, the forward tires rotate
			irr::newton::ICar* createCar(irr::newton::SCar car);

			//!
			//!delete all bodies, materials  destroy world
			//!\see IWorld::clear()
			void destroyWorld();

			//!
			//!delete all bodies and materials but don't destroy world
			void clear();


			//!create a up vector joint
			//!\param jointData joint data wich is used to create the joint
			irr::newton::IJointUpVector* createJoint(irr::newton::SJointUpVector jointData);

			//!create a up vector joint
			//!\param jointData joint data wich is used to create the joint
			irr::newton::IJointHinge* createJoint(irr::newton::SJointHinge jointData);

			//!create a up vector joint
			//!\param jointData joint data wich is used to create the joint
			irr::newton::IJointUniversal* createJoint(irr::newton::SJointUniversal jointData);

			//!create a up vector joint
			//!\param jointData joint data wich is used to create the joint
			irr::newton::IJointCorkscrew* createJoint(irr::newton::SJointCorkscrew jointData);

			//!create a up vector joint
			//!\param jointData joint data wich is used to create the joint
			irr::newton::IJointSlider* createJoint(irr::newton::SJointSlider jointData);

			//!create a up vector joint
			//!\param jointData joint data wich is used to create the joint
			irr::newton::IJointBall* createJoint(irr::newton::SJointBall jointData);

			//!create a generic joint
			irr::newton::IJoint* createJoint(irr::newton::SGenericJoint c);

			//!create a rag doll
			irr::newton::IRagDoll* createRagDoll(irr::newton::SRagDoll data);

			//!NOT IMPLEMENTED JET
			irr::newton::IRagDoll* createRagDoll();

			//!Get the collision manager to collision checking and ray casting
			//!\return the collision manager
			irr::newton::ICollisionManager* getCollisionManager();

			//!Get a pointer to a class wich contail some tolls functions
			//!\return a tools class
			irr::newton::IUtils* getUtils();
			irr::newton::IInternalBehaviour* getInternalBehaviour();

			/*!
			Set the solver precision mode
			\param mode the solver precision mode
			*/
			void setSolverModel(irr::newton::E_OPTIMIZATION_MODE mode);

			/*!
			Set coulomb model of friction
			\param mode coulomb model of friction
			*/
			void setFrictionModel(irr::newton::E_OPTIMIZATION_MODE mode);

			/*!
			Set the current platform hardware architecture
			This function allows the application to configure the Newton to take advantage for specific hardware architecture in the same platform.
			\param mode the platform architecture
			*/
			void setPlatformArchitecture(irr::newton::E_USE_HARDWARE_MODE mode);

			/*!
			Set the minimum frame rate at which the simulation can run
			\param minimum_frame_rate minimum frame rate of the simulation in frame per second. This value is clamped between 60fps and 1000fps
			\remarks the default minimum frame rate of the simulation is 60 frame per second. When the simulation falls below the specified minimum frame, Newton will perform sub steps in order to meet the desired minimum FPS.
			*/
			void setMinimumFrameRate(irr::f32 minimum_frame_rate);

			/*!
			Return the correct time step for this simulation update.
			\return correct update timestep
			\remarks This application can used this function to get the current simulation time step.
			*/
			irr::f32 getTimeStep();

			/*!
			Return the current Newton library version number.
			\return the decimal three digit value x.xx the first digit: mayor version number (interface changes among other things) the second digit: is mayor patch number (new features, and bug fixes) third digit: is minor bug fixed patch.
			*/
			irr::s32 getNewtonVersion();

			//!get an array of all bodies in the world
			irr::core::array<irr::newton::IBody*> getAllBodies();

			//!get an array of all bodies with primitive collision in the world
			irr::core::array<irr::newton::IBody*> getAllPrimitiveCollisionBodies();

			//!get an array of all bodies with tree collision in the world
			irr::core::array<irr::newton::ITreeBody*> getAllTreeCollisionBodies();

			//!get an array of all materials
			irr::core::array<irr::newton::IMaterial*> getAllMaterials();

			//!get an array of all vehicles (except vehicles created using createCar())
			irr::core::array<irr::newton::IVehicleSimple*> getAllSimpleVehicles();

			//!get an array of all car vehicles (only vehicles created using createCar())
			irr::core::array<irr::newton::ICar*> getAllCars();

			//!get an array of all vehicle tires
			irr::core::array<irr::newton::IVehicleTire*> getAllVehicleTires();

			//!Set the size of the world
			//!\param size new world size
			//!\return none
			void setWorldSize(irr::core::aabbox3d<irr::f32> size);

			//!call a user defined function when a body leave world
			//!\param new_leave_world_event_callback pointer to a function that takes a irr::newton::IBody pointer (contatin the body eich leave the world) and return void
			//!\return none
			void setBodyLeaveWorldEvent( void (*new_leave_world_event_callback) (irr::newton::IBody* body));

			//!set the update frma rate
			inline void setFrameRate(irr::u32 frame_rate) {this->update_fps = frame_rate;}

			//!get the update frma rate
			inline irr::f32 getFrameRate() {return this->update_fps;}

			//!Get the time elapsed from the last World::update() call
			//!\return the elapsed time
			inline irr::f32 getTimeElapsed() const {return this->time_elapsed;}

			//!get newton world
			inline NewtonWorld* getNewtonWorld() const {return this->world;}

			//!get newton default group id
			inline int getNewtonDefaultMaterialGroupID() const {return this->group_material_ID;}

			//!get irrlicht device
			inline irr::IrrlichtDevice* getIrrlichtDevice() const {return this->device;}



			//---------------------------------------------------------------------------------------------
			//---------------------------------------------------------------------------------------------
			//OBSOLETE FUNCTIONS
			//---------------------------------------------------------------------------------------------
			//---------------------------------------------------------------------------------------------


			//---------------
			//IrrNewt 0.11
			//---------------


			//!Obsolete. Use IWorld::createBody(SBody body)
			irr::newton::IBody*  createBody(
				irr::scene::ISceneNode* node,
				irr::scene::IMesh* mesh = NULL,
				irr::newton::E_BODY_TYPE shape_type = newton::EBT_PRIMITIVE_BOX,
				irr::f32 mass = 1.0f,
				irr::core::matrix4 body_offset_from_scene_node = irr::core::matrix4(),
				core::vector3df body_scale_offset = irr::core::vector3df(1,1,1),
				E_CALCULATE_OFFSET autoCalcPositionOffset = newton::ECO_MESH,
				E_CALCULATE_SIZE calcSizeMode = newton::ECS_NODE);


			//!Obsolete. Use IWorld::createBody(SBody body)
			irr::newton::IBody* createBodyAuto(
				irr::scene::ISceneNode* node,
				irr::scene::IMesh* mesh = NULL,
				irr::f32 mass = 1.0f,
				irr::u32 terrain_lod = 3,
				irr::core::matrix4 body_offset_from_scene_node = irr::core::matrix4(),
				irr::core::vector3df scale_offset = irr::core::vector3df(),
				E_CALCULATE_OFFSET autoCalcPositionOffset = newton::ECO_MESH,
				E_CALCULATE_SIZE calcSizeMode = newton::ECS_NODE
				);

			//!Obsolete. Use IWorld::createBody(SBody body)
			irr::newton::ITreeBody*  createTreeBody(
				irr::scene::ISceneNode* node,
				irr::scene::IMesh* iMeshBase);

			//!Obsolete. Use IWorld::createBody(SBody body)
			irr::newton::ITreeBody*  createTerrainTreeBody(
				irr::scene::ITerrainSceneNode* node,
				irr::s32 LOD=3
				);



		private:
			friend void irr::newton::Hidden::defBodyLeaveWorldEvent(const NewtonBody* n_body, int);

			virtual void reserved_destroy();
			void Initialize();

			//attributes

			void (*body_leave_world_event_callback) (irr::newton::IBody* body);

			irr::newton::ICollisionManager collision_manager;
			irr::newton::IUtils utils;
			irr::newton::IInternalBehaviour behaviour;

			irr::IrrlichtDevice* device;

			irr::f32 accumulated_time;
			irr::f32 update_fps;
			irr::f32 time_elapsed;

			NewtonWorld* world;
			int group_material_ID;

		};//class ARES_API World{

	}
}

#endif
