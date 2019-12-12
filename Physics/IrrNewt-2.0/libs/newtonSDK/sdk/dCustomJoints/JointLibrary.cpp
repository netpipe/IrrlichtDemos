/* Copyright (c) <2009> <Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely
*/

//////////////////////////////////////////////////////////////////////
#include "CustomJointLibraryStdAfx.h"
#include "JointLibrary.h"
#include "dMatrix.h"
#include "Custom6DOF.h"
#include "CustomHinge.h"
#include "CustomSlider.h"
#include "NewtonCustomJoint.h"
#include "CustomDGRayCastCar.h"
#include "CustomBallAndSocket.h"
#include "CustomPlayerController.h"
#include "CustomMultiBodyVehicle.h"
#include "CustomKinematicController.h"



// common Joints functions
void CustomDestroyJoint(const NewtonUserJoint *joint)
{
	delete ((NewtonCustomJoint*)joint);
}

NewtonJoint* CustomGetNewtonJoint (const NewtonUserJoint *joint)
{
	return  (NewtonJoint*) (((NewtonCustomJoint*)joint)->GetJoint ());
}



int CustomGetJointID (const NewtonUserJoint *joint)
{
	return ((NewtonCustomJoint*)joint)->GetJointID ();
}

void CustomSetJointID (const NewtonUserJoint *joint, int rttI)
{
	((NewtonCustomJoint*)joint)->SetJointID(rttI);
}

const NewtonBody* CustomGetBody0 (const NewtonUserJoint *joint)
{
	return ((NewtonCustomJoint*)joint)->GetBody0();
}

const NewtonBody* CustomGetBody1 (const NewtonUserJoint *joint)
{
	return ((NewtonCustomJoint*)joint)->GetBody1();
}

int CustomGetBodiesCollisionState (const NewtonUserJoint *joint)
{
	return ((NewtonCustomJoint*)joint)->GetBodiesCollisionState();
}

void CustomSetBodiesCollisionState (const NewtonUserJoint *joint, int state)
{
	((NewtonCustomJoint*)joint)->SetBodiesCollisionState(state);
}

void* CustomGetUserData (const NewtonUserJoint *joint)
{
	return ((NewtonCustomJoint*)joint)->GetUserData();
}

void CustomSetUserData (const NewtonUserJoint *joint, void* userData)
{
	((NewtonCustomJoint*)joint)->SetUserData(userData);
}

void CustomSetSubmitContraintCallback (const NewtonUserJoint *joint, NewtonUserJointSubmitConstraintCallback callback)
{
	((NewtonCustomJoint*)joint)->SetUserSubmintConstraintCallBack(callback);
}

void CustomSetDestructorCallback (const NewtonUserJoint *joint, NewtonUserJointDestructorCallback callback)
{
	((NewtonCustomJoint*)joint)->SetUserDestructorCallBack(callback);
}


// This is the implementation of User Blank Joint.
// we recommend advance use to use the CPP interface to implement a class deriving from NewtonCustomJoint at a CPP level ,
// but I need to provide a mean for languages that cannot interface with CPP code for making theirs own custom joints
class CustomBlankJoint: public NewtonCustomJoint  
{
	public:
		CustomBlankJoint (int maxDof, const NewtonBody* body0, const NewtonBody* body1, BlankJointGetInfo info)
		:NewtonCustomJoint(maxDof, body0, body1)
	{
		m_info = info;
	}

	~CustomBlankJoint ()
	{
	}

	void SubmitConstraints (dFloat timestep, int threadIndex)
	{
	}

	virtual void GetInfo (NewtonJointRecord* info) const
	{
		if (m_info) {
			NewtonCustomJoint::GetInfo(info);
			m_info ((NewtonUserJoint *)this, info);
		}
	}

	BlankJointGetInfo m_info;
};


// Create a blank joint 
NewtonUserJoint* CustomCreateBlankJoint (int maxDof, const NewtonBody* body0, const NewtonBody* body1, BlankJointGetInfo info)
{
	return (NewtonUserJoint *) new CustomBlankJoint (maxDof, body0, body1, info);
}


// this is the implementations of a user 6 dof joint for the C interface
class CustomUser6DOF: public Custom6DOF  
{
	public:
	CustomUser6DOF (const dMatrix& pinsAndPivoChildFrame, const dMatrix& pinsAndPivoParentFrame, const NewtonBody* child, const NewtonBody* parent = NULL)
		:Custom6DOF (pinsAndPivoChildFrame, pinsAndPivoParentFrame, child, parent)
	{
	}
};


// Generic 6 degree of Freedom Joint
NewtonUserJoint *CreateCustomJoint6DOF (const dFloat* pinsAndPivoChildFrame, const dFloat* pinsAndPivoParentFrame, const NewtonBody* child, const NewtonBody* parent)
{
	return (NewtonUserJoint *) new CustomUser6DOF (*(dMatrix*) pinsAndPivoChildFrame, *(dMatrix*) pinsAndPivoParentFrame, child, parent);
}


void CustomJoint6DOF_SetLinearLimits (NewtonUserJoint* customJoint6DOF, const dFloat* minLinearLimits, const dFloat* maxLinearLimits)
{
	((CustomUser6DOF*)customJoint6DOF)->SetLinearLimits (*(dVector*)minLinearLimits, *(dVector*)maxLinearLimits); 
}

void CustomJoint6DOF_SetAngularLimits (NewtonUserJoint* customJoint6DOF, const dFloat* minAngularLimits, const dFloat* maxAngularLimits)
{
	((CustomUser6DOF*)customJoint6DOF)->SetAngularLimits(*(dVector*)minAngularLimits, *(dVector*)maxAngularLimits); 
}

void CustomJoint6DOF_GetLinearLimits (NewtonUserJoint* customJoint6DOF, dFloat* minLinearLimits, dFloat* maxLinearLimits)
{
	((CustomUser6DOF*)customJoint6DOF)->GetLinearLimits (*(dVector*)minLinearLimits, *(dVector*)maxLinearLimits); 
}

void CustomJoint6DOF_GetAngularLimits (NewtonUserJoint* customJoint6DOF, dFloat* minAngularLimits, dFloat* maxAngularLimits)
{
	((CustomUser6DOF*)customJoint6DOF)->GetAngularLimits(*(dVector*)minAngularLimits, *(dVector*)maxAngularLimits); 
}

void CustomJoint6DOF_SetReverseUniversal (NewtonUserJoint* customJoint6DOF, int order)
{
	((CustomUser6DOF*)customJoint6DOF)->SetReverserUniversal(order);
}



// this is the implementations of a user Ball and Socket joint for a C interface
class CustomUserBallAndSocket: public CustomLimitBallAndSocket  
{
public:
	CustomUserBallAndSocket (const dMatrix& pinsAndPivoChildFrame, const NewtonBody* child, const NewtonBody* parent)
		:CustomLimitBallAndSocket (pinsAndPivoChildFrame, child, parent)
	{
	}
};

// Generic Hinge Joint spin along the first axis on pinsAndPivoChildFrame
NewtonUserJoint *CreateCustomBallAndSocket (const dFloat* pinsAndPivoChildFrame, const NewtonBody* child, const NewtonBody* parent)
{
	return (NewtonUserJoint *) new CustomUserBallAndSocket (*(dMatrix*) pinsAndPivoChildFrame, child, parent);
}

void BallAndSocketSetConeAngle (NewtonUserJoint* ballJoint, dFloat angle)
{
	((CustomUserBallAndSocket*)ballJoint)->SetConeAngle(angle); 
}

void BallAndSocketSetTwistAngle (NewtonUserJoint* ballJoint, dFloat minAngle, dFloat maxAngle)
{
	((CustomUserBallAndSocket*)ballJoint)->SetTwistAngle (minAngle, maxAngle); 
}




// this is the implementations of a user Hinge joint for a C interface
class CustomUserHinge: public CustomHinge  
{
public:
	CustomUserHinge (const dMatrix& pinsAndPivoChildFrame, const NewtonBody* child, const NewtonBody* parent)
		:CustomHinge (pinsAndPivoChildFrame, child, parent)
	{
	}

	dFloat GetJointAngle () const
	{
		//return m_curJointAngle.m_angle;
		return CustomHinge::GetJointAngle();
	}

	void GetPin (dFloat* pin) const
	{
//		dMatrix matrix;
//		NewtonBodyGetMatrix (m_body0, &matrix[0][0]);
//		dVector dir (matrix.RotateVector (m_localMatrix0.m_front));
		dVector dir (CustomHinge::GetPinAxis());
		pin[0] = dir.m_x;
		pin[1] = dir.m_y;
		pin[2] = dir.m_z;
	}

	dFloat CalculateJointOmega () const
	{
//		dMatrix matrix0;
//		dMatrix matrix1;
//		dVector omega0(0.0f, 0.0f, 0.0f, 0.0f);
//		dVector omega1(0.0f, 0.0f, 0.0f, 0.0f);
//		CalculateGlobalMatrix (m_localMatrix0, m_localMatrix1, matrix0, matrix1);
//		NewtonBodyGetOmega(m_body0, &omega0[0]);
//		if (m_body1) {
//			NewtonBodyGetOmega(m_body1, &omega1[0]);
//		}
//		return (omega0 - omega1) % matrix0.m_front;;
		return CustomHinge::GetJointOmega ();
	}
};

// Generic Hinge Joint spin along the first axis on pinsAndPivoChildFrame
NewtonUserJoint *CreateCustomHinge (const dFloat* pinsAndPivoChildFrame, const NewtonBody* child, const NewtonBody* parent)
{
	return (NewtonUserJoint *) new CustomUserHinge (*(dMatrix*) pinsAndPivoChildFrame, child, parent);
}

void HingeEnableLimits(NewtonUserJoint* hingeJoint, int state)
{
	((CustomUserHinge*)hingeJoint)->EnableLimits(state ? true : false); 
}

void HingeSetLimis (NewtonUserJoint* hingeJoint, dFloat minAngle, dFloat maxAngle)
{
	((CustomUserHinge*)hingeJoint)->SetLimis (minAngle, maxAngle); 
}

dFloat HingeGetJointAngle (const NewtonUserJoint* hingeJoint)
{
	return ((CustomUserHinge*)hingeJoint)->GetJointAngle();
}

void HingeGetPinAxis (const NewtonUserJoint* hingeJoint, dFloat* pin) 
{
	return ((CustomUserHinge*)hingeJoint)->GetPin (pin);
}

dFloat HingeCalculateJointOmega (const NewtonUserJoint* hingeJoint)
{
	return ((CustomUserHinge*)hingeJoint)->CalculateJointOmega();
}



// this is the implementations of a user Hinge joint for a C interface
class CustomUserSlider: public CustomSlider  
{
public:
	CustomUserSlider (const dMatrix& pinsAndPivoChildFrame, const NewtonBody* child, const NewtonBody* parent)
		:CustomSlider (pinsAndPivoChildFrame, child, parent)
	{
	}
};

// Generic Slider Joint spin along the first axis on pinsAndPivoChildFrame
NewtonUserJoint *CreateCustomSlider (const dFloat* pinsAndPivoChildFrame, const NewtonBody* child, const NewtonBody* parent)
{
	return (NewtonUserJoint *) new CustomUserSlider (*(dMatrix*) pinsAndPivoChildFrame, child, parent);
}

void SliderEnableLimits(NewtonUserJoint* sliderJoint, int state)
{
	((CustomUserSlider*)sliderJoint)->EnableLimits(state ? true : false); 
}

void SliderSetLimis (NewtonUserJoint* sliderJoint, dFloat minAngle, dFloat maxAngle)
{
	((CustomUserSlider*)sliderJoint)->SetLimis (minAngle, maxAngle); 
}


// this is the implementations of a user pick body joint
class CustomUserKinematicController: public CustomKinematicController  
{
public:
	CustomUserKinematicController (const NewtonBody* body, const dVector& attachmentPointInGlobalSpace)
		:CustomKinematicController (body, attachmentPointInGlobalSpace)
	{
	}
};

// Pick Body joint
NewtonUserJoint *CreateCustomKinematicController (const NewtonBody* body, dFloat* handleInGlobalSpace)
{
	return (NewtonUserJoint *) new CustomUserKinematicController (body, *((dVector*)handleInGlobalSpace));
}

void CustomKinematicControllerSetPickMode (const NewtonUserJoint* pick, int mode)
{
	((CustomUserKinematicController*)pick)->SetPickMode (mode);
}


void CustomKinematicControllerSetMaxLinearFriction(const NewtonUserJoint* pick, dFloat accel) 
{
	((CustomUserKinematicController*)pick)->SetMaxLinearFriction(accel); 
}

void CustomKinematicControllerSetMaxAngularFriction(const NewtonUserJoint* pick, dFloat alpha) 
{
	((CustomUserKinematicController*)pick)->SetMaxAngularFriction(alpha);
}

void CustomKinematicControllerSetTargetPosit (const NewtonUserJoint* pick, dFloat* posit) 
{
	((CustomUserKinematicController*)pick)->SetTargetPosit ((*(dVector*)posit));
}

void CustomKinematicControllerSetTargetRotation (const NewtonUserJoint* pick, dFloat* rotation) 
{
	((CustomUserKinematicController*)pick)->SetTargetRotation (*((dQuaternion*) rotation)) ;
}


void CustomKinematicControllerSetTargetMatrix (const NewtonUserJoint* pick, dFloat* matrix) 
{
	((CustomUserKinematicController*)pick)->SetTargetMatrix ((*(dMatrix*) matrix)); 
}

void CustomKinematicControllerGetTargetMatrix (const NewtonUserJoint* pick, dFloat* matrix)
{
	dMatrix& retMatrix = (*(dMatrix*) matrix);
	retMatrix = ((CustomUserKinematicController*)pick)->GetTargetMatrix();
}




// player controller functions 
class PlayerController: public CustomPlayerController 
{
	public: 
//	typedef int (*PlayerCanPuchBody) (NewtonUserJoint *me, const NewtonBody* hitBody);

	PlayerController (const dMatrix& globalFrame, const NewtonBody* child, dFloat maxStairStepFactor, dFloat cushion)
		:CustomPlayerController (globalFrame, child, maxStairStepFactor, cushion)
	{
//		m_canPuchOtherBodies = CanPushThisBodyCallback;
	}

//	virtual bool CanPushBody (const NewtonBody* hitBody) const 
//	{
//		if (m_canPuchOtherBodies) {
//			return m_canPuchOtherBodies ((NewtonUserJoint *)this, hitBody) ? true : false;
//		} 
//		return true;
//	}

//	static int CanPushThisBodyCallback(NewtonUserJoint *me, const NewtonBody* hitBody) 
//	{
//		return 1;
//	}

//	PlayerCanPuchBody m_canPuchOtherBodies;
};


NewtonUserJoint *CreateCustomPlayerController (const dFloat* localFrame, const NewtonBody* player, dFloat maxStairStepFactor, dFloat cushion)
{
	return (NewtonUserJoint *) new CustomPlayerController (*(dMatrix*) localFrame, player, maxStairStepFactor, cushion);
}

const NewtonCollision* CustomPlayerControllerGetSensorShape (const NewtonUserJoint* playerController)
{
	return ((PlayerController*)playerController)->GetSensorShape ();
}

void CustomPlayerControllerSetVelocity (const NewtonUserJoint* playerController, dFloat forwardSpeed, dFloat sideSpeed, dFloat heading)
{
	((PlayerController*)playerController)->SetVelocity (forwardSpeed, sideSpeed, heading);
}

void CustomPlayerControllerGetVisualMaTrix (const NewtonUserJoint* playerController, dFloat* matrix)
{
	((dMatrix&)*matrix) = ((PlayerController*)playerController)->CalculateVisualMatrix();
}


void CustomPlayerControllerSetMaxSlope (const NewtonUserJoint* playerController, dFloat maxSlopeAngleIndRadian)
{
	((PlayerController*)playerController)->SetMaxSlope (maxSlopeAngleIndRadian);
}
dFloat CustomPlayerControllerGetMaxSlope (const NewtonUserJoint* playerController)
{
	return ((PlayerController*)playerController)->GetMaxSlope();
}



/*
void CustomPlayerControllerSetPushActorCallback (NewtonUserJoint* playerController, PlayerCanPuchThisBodyCalback callback)
{
	((PlayerController*)playerController)->m_canPuchOtherBodies = (PlayerController::PlayerCanPuchBody)callback;
}



const NewtonCollision* CustomPlayerControllerGetDynamicsSensorShape (NewtonUserJoint* playerController)
{
	return ((PlayerController*)playerController)->GetDynamicsSensorShape ();
}
*/

/*
// MultiBody Vehicle interface
NewtonUserJoint *CreateCustomMultiBodyVehicle (const dFloat* frontDir, const dFloat* upDir, const NewtonBody* carBody)
{
	return (NewtonUserJoint *) new CustomMultiBodyVehicle (*((dVector*) frontDir), *((dVector*) upDir), carBody);
}

#if 0
int CustomMultiBodyVehicleAddTire (NewtonUserJoint *car, const void* userData, const dFloat* localPosition, 
								  dFloat mass, dFloat radius, dFloat width,
								  dFloat suspensionLength, dFloat springConst, dFloat springDamper)
{
	dVector posit (localPosition[0], localPosition[1], localPosition[2], 0.0f);
	return ((CustomMultiBodyVehicle*)car)->AddSingleSuspensionTire ((void*)userData, &posit[0], 
																	mass, radius, width, suspensionLength, springConst, springDamper);
}

int CustomMultiBodyVehicleAddSlipDifferencial (NewtonUserJoint *car, int leftTireIndex, int rightToreIndex, dFloat maxFriction)
{
	return ((CustomMultiBodyVehicle*)car)->AddSlipDifferencial(leftTireIndex, rightToreIndex, maxFriction);
}


int CustomMultiBodyVehicleGetTiresCount(NewtonUserJoint *car)
{
	return ((CustomMultiBodyVehicle*)car)->GetTiresCount();
}

const NewtonBody* CustomMultiBodyVehicleGetTireBody(NewtonUserJoint *car, int tireIndex)
{
	return ((CustomMultiBodyVehicle*)car)->GetTireBody(tireIndex);
}

dFloat CustomMultiBodyVehicleGetSpeed(NewtonUserJoint *car)
{
	return ((CustomMultiBodyVehicle*)car)->GetSpeed();
}

void CustomMultiBodyVehicleApplyTorque (NewtonUserJoint *car, int tireIndex, dFloat torque)
{
	((CustomMultiBodyVehicle*)car)->ApplyTireTorque(tireIndex, torque);
}

void CustomMultiBodyVehicleApplySteering (NewtonUserJoint *car, int tireIndex, dFloat angle)
{
	((CustomMultiBodyVehicle*)car)->ApplyTireSteerAngle(tireIndex, angle);
}

void CustomMultiBodyVehicleApplyBrake (NewtonUserJoint *car, int tireIndex, dFloat brakeTorque)
{
	((CustomMultiBodyVehicle*)car)->ApplyTireBrake (tireIndex, brakeTorque);
}

void CustomMultiBodyVehicleApplyTireRollingDrag (NewtonUserJoint *car, int tireIndex, dFloat angularDampingCoef)
{
	((CustomMultiBodyVehicle*)car)->ApplyTireRollingDrag (tireIndex, angularDampingCoef);
}

dFloat CustomMultiBodyVehicleGetTireSteerAngle (NewtonUserJoint *car, int tireIndex)
{
	return ((CustomMultiBodyVehicle*)car)->GetSetTireSteerAngle (tireIndex);
}
*/


// this is a thin shell over the Ray Cast car class, the use can add functionality to the C interface by adding functions to this class
#define CUSTOM_VEHICLE_JOINT_ID		0x56f4bd4e
class CustomUserDGRayCastCar: public CustomDGRayCastCar  
{
	public:
	CustomUserDGRayCastCar (int maxTireCount, const dMatrix& chassisMatrixInLocalSpace, NewtonBody* carBody)
		:CustomDGRayCastCar(maxTireCount, chassisMatrixInLocalSpace, carBody)
	{
		m_tireTransformCallback = NULL;
		SetJointID (CUSTOM_VEHICLE_JOINT_ID);
		m_chainTranformCallback = NewtonBodyGetTransformCallback(carBody);
		NewtonBodySetTransformCallback(carBody, CarTransformCallback);
	}

	void SetTireTransformCallback (DGRaycastVehicleTireTransformCallback callback)
	{
		m_tireTransformCallback = callback;
	}

	static void CarTransformCallback (const NewtonBody* body, const dFloat* matrix, int threadIndex)
	{
		NewtonJoint* joint;
		CustomUserDGRayCastCar* car;

		car = NULL;
		for (joint = NewtonBodyGetFirstJoint(body); joint; joint = NewtonBodyGetNextJoint(body, joint)) {
			car = (CustomUserDGRayCastCar*) NewtonJointGetUserData(joint);
			if (car->GetJointID() == CUSTOM_VEHICLE_JOINT_ID) {
				break;
			}
		}

		car->m_chainTranformCallback (body, matrix, threadIndex);
		if (car->m_tireTransformCallback) {
			car->m_tireTransformCallback ((NewtonUserJoint *)car);
		}
	}
	
	NewtonSetTransform m_chainTranformCallback;
	DGRaycastVehicleTireTransformCallback m_tireTransformCallback;
};


// BEGIN k00m (Dave Gravel simple raycast world vehicle)
NewtonUserJoint *DGRaycastVehicleCreate (int maxTireCount, const dFloat* cordenateSytem, NewtonBody* carBody)
{
	return (NewtonUserJoint *) new CustomUserDGRayCastCar (maxTireCount, *((dMatrix*) cordenateSytem), carBody);
}

void DGRaycastVehicleAddTire (NewtonUserJoint *car, void *userData, const dFloat* localPosition, dFloat mass, dFloat radius, dFloat width, dFloat friction, dFloat suspensionLength, dFloat springConst, dFloat springDamper, int castMode)
{
	((CustomUserDGRayCastCar*)car)->AddSingleSuspensionTire(userData,localPosition,mass,radius,width,friction,suspensionLength,springConst,springDamper,castMode);
}

void DGRaycastVehicleSetTireTransformCallback (NewtonUserJoint *car, DGRaycastVehicleTireTransformCallback callback)
{
	((CustomUserDGRayCastCar*)car)->SetTireTransformCallback (callback);
}

int DGRaycastVehicleGetTiresCount(NewtonUserJoint *car)
{
	return ((CustomUserDGRayCastCar*)car)->GetTiresCount();
}

void* DGRaycastVehicleGetTiresUserData(NewtonUserJoint *car, int tireIndex)
{
	return ((CustomUserDGRayCastCar*)car)->GetTire(tireIndex).m_userData;
}

void DGRaycastVehicleGetTireMatrix(NewtonUserJoint *car, int tire, dFloat* tireMatrix)
{
	(*(dMatrix*)tireMatrix) = ((CustomUserDGRayCastCar*)car)->CalculateTireMatrix(tire);
}


/*
const NewtonCollision* DGRayCarTireShape(NewtonUserJoint *car, int tireIndex) 
{
  return ((CustomUserDGRayCastCar*)car)->GetTiresShape(tireIndex); 
}

void DGRayCarGetChassisMatrixLocal(NewtonUserJoint *car, dFloat* chassisMatrix)
{
  (*(dMatrix*)chassisMatrix) = ((CustomUserDGRayCastCar*)car)->GetChassisMatrixLocal();
}


void DGRayCarSuspensionMatrix(NewtonUserJoint *car, int tire, dFloat param, dFloat* SuspensionMatrix)
{
  (*(dMatrix*)SuspensionMatrix) = ((CustomUserDGRayCastCar*)car)->CalculateSuspensionMatrix(tire,param);
}

void DGRaycastVehicleSetCustomTireBrake (NewtonUserJoint *car, int index, dFloat torque)
{
  ((CustomUserDGRayCastCar*)car)->SetCustomTireBrake(index,torque);
}

void DGRaycastVehicleSetCustomTireTorque (NewtonUserJoint *car, int index, dFloat torque)
{
  ((CustomUserDGRayCastCar*)car)->SetCustomTireTorque(index,torque);
}

void DGRaycastVehicleSetCustomTireSteerAngleForce (NewtonUserJoint *car, int index, dFloat angle, dFloat turnforce)
{
  ((CustomUserDGRayCastCar*)car)->SetCustomTireSteerAngleForce(index,angle,turnforce);
}

dFloat DGRaycastVehicleGetSpeed(NewtonUserJoint *car)
{
  return ((CustomUserDGRayCastCar*)car)->GetSpeed();
}

dFloat DGRaycastVehicleGenerateTiresBrake (NewtonUserJoint *car, dFloat value)
{
  return ((CustomUserDGRayCastCar*)car)->GenerateTiresBrake ( value );
}

dFloat DGRaycastVehicleGenerateTiresTorque (NewtonUserJoint *car, dFloat value)
{
  return ((CustomUserDGRayCastCar*)car)->GenerateTiresTorque( value );
}

dFloat DGRaycastVehicleGenerateTiresSteerForce (NewtonUserJoint *car, dFloat value)
{
  return ((CustomUserDGRayCastCar*)car)->GenerateTiresSteerForce( value );
}

dFloat DGRaycastVehicleGenerateTiresSteerAngle (NewtonUserJoint *car, dFloat value)
{
  return ((CustomUserDGRayCastCar*)car)->GenerateTiresSteerAngle( value );
}

void DGRaycastVehicleSetVarTireMovePointForceFront (NewtonUserJoint *car, int index, dFloat distance)
{
  ((CustomUserDGRayCastCar*)car)->SetVarTireMovePointForceFront( index, distance );
}

void DGRaycastVehicleSetVarTireMovePointForceRight (NewtonUserJoint *car, int index, dFloat distance)
{
  ((CustomUserDGRayCastCar*)car)->SetVarTireMovePointForceRight( index, distance );
}

void DGRaycastVehicleSetVarTireMovePointForceUp (NewtonUserJoint *car, int index, dFloat distance)
{
  ((CustomUserDGRayCastCar*)car)->SetVarTireMovePointForceUp( index, distance );
}

void DGRaycastVehicleSetVarFixDeceleration (NewtonUserJoint *car, dFloat value)
{
  ((CustomUserDGRayCastCar*)car)->SetVarFixDeceleration( value );
}

void DGRaycastVehicleSetVarChassisRotationLimit (NewtonUserJoint *car, dFloat value)
{
  ((CustomUserDGRayCastCar*)car)->SetVarChassisRotationLimit( value );
}

void DGRaycastVehicleSetVarMaxSteerAngle (NewtonUserJoint *car, dFloat value)
{
  ((CustomUserDGRayCastCar*)car)->SetVarMaxSteerAngle( value );
}

void DGRaycastVehicleSetVarMaxSteerRate (NewtonUserJoint *car, dFloat value)
{
  ((CustomUserDGRayCastCar*)car)->SetVarMaxSteerRate( value );
}

void DGRaycastVehicleSetVarMaxSteerForceRate (NewtonUserJoint *car, dFloat value)
{
  ((CustomUserDGRayCastCar*)car)->SetVarMaxSteerForceRate( value );
}

void DGRaycastVehicleSetVarMaxSteerForce (NewtonUserJoint *car, dFloat value)
{
  ((CustomUserDGRayCastCar*)car)->SetVarMaxSteerForce( value );
}

void DGRaycastVehicleSetVarMaxSteerSpeedRestriction (NewtonUserJoint *car, dFloat value)
{
  ((CustomUserDGRayCastCar*)car)->SetVarMaxSteerSpeedRestriction( value );
}

void DGRaycastVehicleSetVarMaxBrakeForce (NewtonUserJoint *car, dFloat value)
{
  ((CustomUserDGRayCastCar*)car)->SetVarMaxBrakeForce( value );
}

void DGRaycastVehicleSetVarMaxTorque (NewtonUserJoint *car, dFloat value)
{
  ((CustomUserDGRayCastCar*)car)->SetVarMaxTorque( value );
}

void DGRaycastVehicleSetVarMaxTorqueRate (NewtonUserJoint *car, dFloat value)
{
  ((CustomUserDGRayCastCar*)car)->SetVarMaxTorqueRate( value );
}

void DGRaycastVehicleSetVarEngineSteerDiv (NewtonUserJoint *car, dFloat value)
{
  ((CustomUserDGRayCastCar*)car)->SetVarEngineSteerDiv( value );
}

void DGRaycastVehicleSetVarTireSuspenssionHardLimit (NewtonUserJoint *car, int index, dFloat value)
{
  ((CustomUserDGRayCastCar*)car)->SetVarTireSuspenssionHardLimit( index, value );
}

void DGRaycastVehicleSetVarTireFriction (NewtonUserJoint *car, int index, dFloat value)
{
  ((CustomUserDGRayCastCar*)car)->SetVarTireFriction( index, value );
}

int DGRaycastVehicleGetVehicleOnAir(NewtonUserJoint *car)
{
  return ((CustomUserDGRayCastCar*)car)->GetVehicleOnAir();
}

int DGRaycastVehicleGetTireOnAir(NewtonUserJoint *car, int index)
{
  return ((CustomUserDGRayCastCar*)car)->GetTireOnAir( index );
}


void DGRaycastVehicleDestroy (NewtonUserJoint *car)
{
  delete car;
}
#endif



*/



