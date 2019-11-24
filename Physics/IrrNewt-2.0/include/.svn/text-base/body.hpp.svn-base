// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#ifndef IRRNEWT_LIBRARY_PHYSICS_BODY_HPP_CODE_ADGSDVVFEFA65162165816816081068ASFASFW
#define IRRNEWT_LIBRARY_PHYSICS_BODY_HPP_CODE_ADGSDVVFEFA65162165816816081068ASFASFW
#pragma once

#include <stddef.h>
#include <irrlicht.h>
#include <Newton.h>

#include "irrtonewt.hpp"

#include "hidden.hpp"

//#include "structures.hpp"
//#include "dll_exports.hpp"

#include "irrnewt_base.hpp"

#include "collision.hpp"


namespace irr{
	namespace newton{

		class IWorld;
		class IMaterial;
		class ICharacterController;
		class IRagDoll;
		class ICollision;
		struct SPosRot;

        namespace Hidden {
      		void defBodyForceAndTorqueCallback (const NewtonBody* body);
			void defBodyForceAndTorqueCallback (newton::IBody* body, const NewtonBody* n_body);

			void defBodyTransformCallback(const NewtonBody* body, const float* matrix);
			void defRagDollForceCallback(const NewtonBody* body);
        }

		//!Class to management rigid bodies
		class IRRNEWT_API IBody:public irr::newton::IrrNewt_Base{
		public:

			//!Body constructor.
			//!Use irr::newton::IWorld::createBody or irr::newton::IWorld::createBodyAuto
			//!to create a body. Don't call this constructor directly
			IBody(
				irr::newton::IWorld* iWorld,
				scene::ISceneNode* node,
				newton::ICollision* collision,
				f32 mass);

			//!default constructor.
			//!don't call it directly. use World::createBody() to create a body
			IBody() {this->init();};

			//!destructor
			virtual ~IBody() {}

			//!Set the new position for the body. This change position of the scene node and the physics node. Use this function instead of irr::scene::ISceneNode::setPosition.
			//!If you use irr::scene::ISceneNode::setPosition to change body position no happens
			//!\param pos new body position in 3 dimension space
			//!\return none
			void setPosition(irr::core::vector3df pos);

			//!Set the new rotation for the body. This change rotation of the scene node and the physics node.
			//!Use this function instead of irr::scene::ISceneNode::setRotation.
			//!If you use irr::scene::ISceneNode::setRotation to change body rotation no happens
			//!\param rot new body rotation on the 3 axis
			//!\return none
			void setRotation(irr::core::vector3df rot);

			//!Set the new scale for the body.
			//!This change scale of the scene node and the body.
			//!Use this function instead of irr::scene::ISceneNode::setScale.
			//!\param scale new body scale
			//!\return none
			//!\warning This method is different depends on the body type:
			//!\warning -if the body type is a convex hull modifier: in this case newton support scale, and the body is simply scaled real time (faster)
			//!\warning -if the body is not a convex hull modifier: this method destroy and re create the body (in this cases newton doesn't support body scaling)
			//!\warning and so can be slow. avoid to use it in this cases. Prefer to scale the node before create the relative body (so before call createBody, createBodyAuto, createTreeBody and so on)
			//!\warning in this case the body is created scaled already
			//!\warning if you need relatime scaling often, use a convex hull modifier
			void setScale(irr::core::vector3df scale);

			//!
			//!set scale only for body and not for node. Prefer to use scale offset instead. See setScale for more information
			//!\param scale the new scale of the body
			//!\return none
			virtual void setScaleBody(irr::core::vector3df scale);

			//!
			//!Set body matrix in irrlicht unit
			//!\param mat the new body matrix in irrlicht units
			void setMatrixIrrlicht(core::matrix4 mat);

			//!
			//!Set body matrix in newton unit
			//!\param mat the new body matrix in newton units
			void setMatrixNewton(core::matrix4 mat);

			//!Set the new rotation only for the newton body(and not for scene node)
			//!This change rotation of the physics node only, but when World::update() is called
			//!the rotation of the scen node is changed indirectly. To avoid that (so to only change the rotation of the physics node
			//!and not the rotation of the scene node) create a character controller (World::createCharacterController()) and call CharacterController::setRotationUpdate()
			//!with false as argument
			//!\param rot new body rotation on the 3 axis
			//!\return none
			void setRotationBody(irr::core::vector3df rot);

			//!Get the rotation of the newton body(and not of the scene node)
			//!This may be different from the rotation of the scene node. See Body::setRotationGeometry() for more information
			//!\return newton body rotation
			//!\see Body::setRotationGeometry()
			irr::core::vector3df getRotationBody();

			//!Get the position of the newton body(and not of the scene node)
			//!this is equal to the scene node position. I used it for debug purposes.
			//!\return newton body position
			irr::core::vector3df getPositionBody();

			//!Get irrlicht scene node associated to this body
			//!\return irrlicht scene node
			inline irr::scene::ISceneNode* getNode();

			//!set the node related to this body
			//!\return none
			inline void setNode(irr::scene::ISceneNode* node);

			//!
			//!Apply hierarchical transformation to a body
			//!\param mat the matrix
			//!\remarks This function applies the transformation matrix to the body and also applies the appropriate transformation matrix to set of articulated bodies. If the body is in contact with another body the other body is not transformed.
			//!\remarks this function should not be used to transform set of articulated bodies that are connected to a static body. doing so will result in unpredictables results. Think for example moving a chain attached to a ceiling from one place to another, to do that in real life a person first need to disconnect the chain (destroy the joint), move the chain (apply the transformation to the entire chain), the reconnect it in the new position (recreate the joint again).
			//!\remarks this function will set to zero the linear and angular velocity of all bodies that are part of the set of articulated body array.
			void setMatrixRecursive(core::matrix4 mat);

			/*!
			Set the minimum values for velocity of a body that will be considered at rest.
			\param freezeSpeedMag2 magnitude squared of the velocity threshold.
			\param freezeOmegaMag2 magnitude squared of angular velocity threshold.
			\param framesCount number of frames the body velocity and angular will not exceed freezeSpeedMag and freezeOmegaMag.
			\remarks Ideally, a body should be deactivated when it reaches a state of stable equilibrium. However, because of floating point inaccuracy, discrete time step simulation and other factors it is virtually impossible for a body to reach that state in a real-time simulation. Therefore, in the Newton World, a body is considered to be in stable equilibrium when its velocity and angular velocity fall below some threshold for a consecutive number of frames.
			\remarks The default and minimum values for the thresholds is 0.01 for speed and 10 for frames count. These values are tuned for single objects colliding under the influence of gravity. It is possible that for complex configuration of bodies like multiples pendulums, rag dolls, etc. these values may need to be increased. This is because joints have the property that they add a small amount of energy to the system in order to reduce the separation error. This may cause the bodies reach a state of unstable equilibrium. That is, when a body oscillates between two different positions because the energy added to the body is equal to the energy dissipated by the integrator. This is a situation that is hard to predict, and the best solution is to tweak these values for specific cases.
			*/
			void setFreezeTreshold(
				irr::f32 freezeSpeedMag2,
				irr::f32 freezeOmegaMag2,
				irr::s32 framesCount);

			/*!
			Get the minimum values for velocity of a body the will be considered at rest.
			\param freezeSpeedMag2 point the to a f32 to hold the velocity threshold
			\param freezeOmegaMag2 point the to a f32 to hold the angular velocity threshold
			\see IBody::setFreezeTreshold()
			*/
			void getFreezeTreshold(
				irr::f32* freezeSpeedMag2,
				irr::f32* freezeOmegaMag2);

			//!Set moment of inertia of the body
			//!
			//!\param inertia new moments of inertia
			void setMomentOfInertia(irr::core::vector3df inertia);

			//!Set mass of the body
			//!\param mass new mass of the body
			void setMass(irr::f32 mass);

			//!Set mass of the body and automatic calculate its new inertia
			//!\param mass new mass of the body
			inline void setMassAndCalcInertia(irr::f32 mass)
			{
				this->setMass(mass);
				this->calculateMomentOfInertia();
			}

			//!Automatic calculate gravity force
			//!\return none
			inline void addGravityForce()
			{
				this->addForceContinuous(core::vector3df(0,-GRAVITY*this->getMass(),0));
			}

			//!set body material
			//!\param new_material new body material
			//!\return none
			void setMaterial(irr::newton::IMaterial* new_material);

			//!get the matrial of this body
			//!\return the body material
			irr::newton::IMaterial* getMaterial() {return this->material;}

			//!set body angular velocity
			//!\param omega the new angular velocity of the body
			//!\return none
			//!\see irr::newton::Body::addTorque()
			void setOmega(irr::core::vector3df omega);

			//!set body linear velocity
			//!\param velocity the new linear velocity of the body
			//!\see irr::newton::Body::addForce()
			virtual void setVelocity(irr::core::vector3df velocity);

			//!Copy a user defined vector to the torque accumulator
			//!Torque is a "rotation force" or a "angular force" which causes a change in rotational motion
			//!For more information go here http://en.wikipedia.org/wiki/Torque
			//!\param new_torque the new torque of the body
			//!\return none
			//!\see irr::newton::Body::addForce()
			inline void setTorque(irr::core::vector3df new_torque) {this->torque=new_torque;}

			//!set linear damping of the body
			//!\param value new body linear damping
			void setLinearDamping(irr::f32 value);

			//!
			//!set angular damping of the body
			//!\param value new body angular damping
			void setAngularDamping(core::vector3df value);

			/*!
			Specify if the node needs a continuous collision mode.
			Continue collision mode enable allow the engine to predict colliding contact on
			rigid bodies Moving at high speed of subject to strong forces.

			continue collision mode does not prevent rigid bodies from inter penetration instead it prevent bodies from passing trough each others by extrapolating contact points when the bodies normal contact calculation determine the bodies are not colliding.

			Because there is penalty of about 40% to 80% depending of the shape complexity
			of the collision geometry, this feature is set off by default. It is the job of
			the application to determine what bodies need this feature on. Good guidelines
			are: very small objects, and bodies that move a height speed.
			\param value set to true active continuous collision mode, set to false deactive it. Default is false
			*/
			void setContinuousCollisionMode(bool value);

			//!get if continuous collision mode is active
			//!\return true if active false if not
			//!\see IBody::setContinuousCollisionMode()
			bool getContinuousCollisionMode();

			/*!
			From newton documentation: "Gyroscopic forces internal forces generated as a result of an asymmetric tensor. They are a pure mathematical consequence that the physics have to comply in order to agree with the math. As Gyroscopic forces are not real forces but the result of net unbalance of the changing inertia tensor or a rigid body when its angular velocity is measured on a reference frame different than the body’s own. Gyroscopic forces are extremely non linear by nature, therefore a first order implicit integrator will have a extremely hard time at dealing with this kind of forces, however because the fact that they are not real forces they do not make much difference in the outcome of the integration. Fortunately due to the fact that the magnitude of gyroscopic forces is proportional to the unbalance of the inertia tensor, it is possible to disregard the effect of this forces by assuming their inertial tensor is symmetric for the purpose of this calculation. For most cases an ordinary person is not capable to distinguish the motion of a body subject to gyroscopic forces and one that is not, especially when the motion is constrained. Because of this fact gyroscopic force are turned off by default in Newton, however there are cases when the desire effect is precisely to simulate these forces like a spinning top, or the design of a space navigational system, etc. The most important feature of gyroscopic forces is that they make the rigid body to process. "
			\param value true to enable gyroscopic forces mode, false to disable it
			\return none
			*/
			void setCoriolisForcesMode(bool value);

			//!freeze the body
			//!a freezed body is not affected by forces and torques
			//!if you don't use a body, call this function with true as argument to improve performance
			//!\param value set it to true to freeze the body, false to unfreeze it
			void setFreeze(bool value);

			//!Set the auto-activation mode for this body
			//!\param value true for auto activation on (controlled by newton), false for auto activation off (controlled by the application)
			//!\see Body::setFreeze()
			void setAutoFreeze(bool value);

			void setCollision(newton::ICollision* newCollision);

			//!Calculate newton body axis aligned boundgin box
			//!\return the axis aligned bounding box for the body calculate using newton
			irr::core::aabbox3d<irr::f32> calculateAABB();

			//!
			//!get moments of inertia of the body
			irr::core::vector3df getMomentOfInertia();

			//!
			//!get body matrix in irrlicht unit
			inline irr::core::matrix4 getMatrixIrrlicht();

			//!
			//!get body matrix in newton unit
			irr::core::matrix4 getMatrixNewton();


			//!
			//!get body mass
			irr::f32 getMass();

			//!this function call ISceneNode::getPosition() internally. This gets the position of the scene node (equal to position of the rigid body)
			//!\see Body::getPositionGeometry()
			inline irr::core::vector3df getPosition() {return this->node->getPosition();}

			//!get body angular velocity
			//!\return body angular velocity
			//!\see Body::setOmega(), Body::addTorque(), Body::setTorque()
			irr::core::vector3df getOmega();

			//!get body linear velocity
			//!\return body linear velocity
			irr::core::vector3df getVelocity();

			//!get the force wich you set using Body::addForce() or Body::setForce().
			//!This don't get the net applied force
			//!\return body force wich you set
			inline irr::core::vector3df getForce() {return this->force;}

			//!get the torque wich you set using IBody::addTorque() or IBody::setTorque().
			//!This don't get the net applied torque
			//!\return body torque wich you set
			inline irr::core::vector3df getTorque() {return this->torque;}

			//!get net applied force since last IWorld::update() call
			inline irr::core::vector3df getNetAppliedForce() {return this->newton_net_applied_force;}

			//!get net applied torque since last IWorld::update() call
			inline irr::core::vector3df getNetAppliedTorque() {return this->newton_net_applied_torque;}

			//!get body matrix
			//!\return body matrix
			irr::core::matrix4 getNewtonMatrix();

			//!is the body moving because a velocity move it?
			//!\return true if the body is affected by velocity, false if not
			inline bool isAffectedByVelocity() {return this->getVelocity()!=core::vector3df(0,0,0);}

			//!is the body moving because a force move it?
			//!\return true if the body is affected by forces, false if not
			inline bool isAffectedByForce() {return this->getForce()!=core::vector3df(0,0,0);}

			//!get shape size on the 3 axis
			//!\return shape size
			inline irr::core::vector3df getShapeSize() {return this->collision->getSize();}

			//!get newton collision
			//!\return newton collision
			inline NewtonCollision* getNewtonCollision();

			//!get newton body
			//!\return newton body
			inline NewtonBody* getNewtonBody();

			//!if the body is a camera IrrNewt set for it a special newton callback to set the camera target properly. If you don't want this special callback, use this function. Only if your body is a irrlicht camera
			virtual void restoreCallbacks();

			//!add a force in a point expressed in global space
			//!\param force force intensity and direction to apply
			//!\param point point in global space where apply the force
			//!\return none
			void addGlobalForce(core::vector3df force, core::vector3df point);

			//!add a force in a point expressed in local space
			//!\param localForce force intensity and direction to apply local to the body
			//!\param localPoint point in local space where apply the force
			//!\return none
			void addLocalForce (core::vector3df localForce, core::vector3df localPoint);


			//!get IrrNewt world
			//!\return IrrNewt world
			inline irr::newton::IWorld* getWorld();

			inline NewtonWorld* getNewtonWorld();


			//!get the data setted using IBody::setUserData()
			inline void* getUserData() {return this->user_data;}

			//!set user data. To get this use IBody::getUserData()
			inline void setUserData(void* userData) {this->user_data=userData;}

			//!add a force to the body
			//!you can add a force to a body at any time, not only when the body is created
			//!This function add the force 1 time
			//!\param force the force to add
			//!\return none
			//!\see Body::setForce()
			virtual void addForce(irr::core::vector3df force);

			//!copy a user defined vector to the force accumulator
			//!\param new_force the new force of the body
			//!\return none
			//!\see irr::newton::Body::addForce()
			virtual void setForce(irr::core::vector3df new_force) {this->force=new_force;}

			//!add a force recursive to the body
			//!you can add a force to a body at any time, not only when the body is created
			//!This function add the force any time that World::update() function is called
			//!so the force is added always. useful for simulate gravity (in this case you should be use a force like (0,-9.8*body->getMass(),0)
			void addForceContinuous(irr::core::vector3df force);


			//!add a torque to the body
			//!This add a user defined vetor to the torque accumulator
			//!to know what torque is, see Body::setTorque()
			//!\param torque the vector to add to the torque accumulator
			//!\return none
			void addTorque(irr::core::vector3df torque);

			//!Add an impulse to a specific point on a body
			//!\param delta_velocity a vector containing the desired change in velocity to point impulse_center.
			//!\param impulse_center a vector containing the center of the impulse in global space.
			void addImpulse(
				irr::core::vector3df delta_velocity,
				irr::core::vector3df impulse_center);

			/*!
			Set the relative position of the center of mass of a rigid body

			From newton documentation "This function can be used to set the relative offset of the center
			of mass of a rigid body. when a rigid body is created the center of mass is set the
			the point c(0, 0, 0), and normally this is the best setting for a rigid body.
			However the are situations in which and object does not have symmetry or simple some
			kind of special effect is desired, and this origin need to be changed."
			\param centre a vector containing the relative offset of the center of mass of the body.
			*/

			void setCentreOfMass(irr::core::vector3df centre);

			//!Get the relative position of the center of mass of a rigid body
			//!see Body::setCentreOfMass()
			//\return relative position of the center of mass of a rigid body
			//\see Body::setCentreOfMass()
			irr::core::vector3df getCentreOfMass();

			//!draw debug info
			//!draw the geometry of the body as a wire frame rappresentation
			//!\return none
			//!\warning It is very importat that you call this function between smgr->drawAll()
			//!and driver->endScene(), or you don't see the wire frame rappresentation
			void drawDebugInfo();

			//!This set rigid body position and rotation as the scene node position\\rotation
			//!usually you don't need to call it
			//!\return none
			void setNodeMatrixToBody();

			//!This function auto calculate body moments of inertia and centre of mass and set them.
			//!You can calculate moments of inertia by yourself or leave newton to calculate it for you
			//!If you use this function for calculate moments of inertia, every time you call Body::setMass()
			//!and change body mass you need to call this function immediately afterwards to recalculate moments of inertia.
			//! You can retrieve automatic calculated moments of inerta and mass origin by call getMomentOfInerta() and getCentreOfMass()
			//!\return none
			void calculateMomentOfInertia();

			//!get node matrix offset from body. Retrieve only for position and rotation offset
			inline core::matrix4 getOffsetMatrix() {return this->applied_offset;}

			//!get all polygons of the body
			irr::core::array<irr::newton::SPolygon> getAllPoligons();

			//!remove the body and the related node
			void remove();

			//!remove the body but not the related node
			void removeBody();

			//void setParent(irr::scene::ISceneNode* new_parent) {this->parent=new_parent;}
			//irr::scene::ISceneNode* getParent() {return this->parent;}

			//!\param setting true for setting force (NewtonBodySetForce) false for adding (NewtonBodyAddForce)
			inline void setForceMode(bool setting) {this->setting_force = setting;}

			//!get for mode
			//!\return true if the forces are setted (NewtonBodySetForce) (default), false false if the forces are added (NewtonBodyAddForce)
			inline bool getForceMode() {return this->setting_force;}

			//!get the type of the collision object attached to the body
			inline newton::E_BODY_TYPE getType() {return this->collision->getCreationData().Type;}

		protected:

			//callbacks
			friend void
				irr::newton::Hidden::defBodyForceAndTorqueCallback (const NewtonBody* body, float, int);
			friend void
				irr::newton::Hidden::defBodyTransformCallback(const NewtonBody* body, const float* matrix, int);
			//friend void
				//irr::newton::Hidden::defRagDollForceCallback(const NewtonBody* body, int);
			friend void
				irr::newton::Hidden::defBodyForceAndTorqueCallback(newton::IBody* body, const NewtonBody* n_body, int);


			friend class IRagDoll;


			/*
			void CreateCollision(
				irr::core::vector3df& box_size,
				irr::core::matrix4 offset,
				irr::scene::IMesh* mesh_for_convex);

			void CreateCompundCollision(core::array<NewtonCollision*> collisions);

			void createConvexHullCollision(scene::IMesh* mesh,core::matrix4 offset);

			void createConvexHullCollision(core::array< core::triangle3d<f32> > polys,
				irr::core::matrix4 offset);

  */

            void Create( newton::IWorld* world, f32 mass);

			void init();

			//reserved
			inline virtual void reserved_destroy() {this->~IBody();}

			//attributes
			//IMPORTANT! IF YOU ADD AN ATTRIBUTE, MAKE SURE THAT YOU COPY IT IN CHARACTER CONTROLLING
			irr::core::matrix4 applied_offset;

			bool camera;
			bool setting_force;

			irr::newton::IMaterial* material;

			newton::ICollision* collision;
			NewtonBody* body;

			scene::ISceneNode* node;
			void* user_data;

			irr::core::vector3df force;
			irr::core::vector3df force_continuos;
			irr::core::vector3df torque;
			irr::core::vector3df box_size;

			irr::core::vector3df newton_net_applied_force;
			irr::core::vector3df newton_net_applied_torque;

		};//class ARES_API SceneNode {

	}//newton
}//irr

//--------------INLINE FUNCTIONS-----------------------
inline irr::scene::ISceneNode* irr::newton::IBody::getNode() {
	return this->node;
}

inline NewtonCollision* irr::newton::IBody::getNewtonCollision() {
	return this->collision->getNewtonCollision();
}

inline NewtonBody* irr::newton::IBody::getNewtonBody() {
	return this->body;
}

inline void irr::newton::IBody::setNode(irr::scene::ISceneNode* node) {
	this->node = node;
}

inline irr::newton::IWorld* irr::newton::IBody::getWorld() {
	return (irr::newton::IWorld*)NewtonWorldGetUserData(NewtonBodyGetWorld(this->body));
}

inline NewtonWorld* irr::newton::IBody::getNewtonWorld() {
	return NewtonBodyGetWorld(this->body);
}

inline irr::core::matrix4 irr::newton::IBody::getMatrixIrrlicht() {
	core::matrix4 mat;
	mat=this->getMatrixNewton();
	mat.setTranslation(mat.getTranslation()*NewtonToIrr);
	return mat;
}

#endif
