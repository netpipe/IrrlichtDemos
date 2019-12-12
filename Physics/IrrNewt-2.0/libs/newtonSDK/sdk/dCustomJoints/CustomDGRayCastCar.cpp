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

// CustomDGRayCastCar.cpp: implementation of the CustomDGRayCastCar class.
// This raycast vehicle is currently a work in progress by Dave Gravel - 2009.
// Vehicle Raycast and convexCast Version 3.0b
//////////////////////////////////////////////////////////////////////
#include "CustomJointLibraryStdAfx.h"
#include "CustomDGRayCastCar.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define VEHICLE_MAX_TIRE_COUNT 16
#define DefPO (3.141592f / 180.0f)
#define TIRE_VISCUOS_DAMP (0.1f)

CustomDGRayCastCar::CustomDGRayCastCar (int maxTireCount, const dMatrix& cordenateSytem, NewtonBody* carBody)
	:NewtonCustomJoint(2 * maxTireCount, carBody, NULL),
	 m_normalizedLateralForce(),
	 m_normalizedLongitudinalForce ()
{
	dVector com;
	dMatrix tmp;
	dFloat Ixx; 
	dFloat Iyy; 
	dFloat Izz; 
	dMatrix chassisMatrix;

	NewtonBodyGetMassMatrix( m_body0, &m_mass, &Ixx, &Iyy, &Izz );

	m_curSpeed = 0.0f;
	m_tiresCount = 0;
	m_vehicleOnAir = 0;
	m_steerAngle = 0.0f;
	//set default break force as a function of the vehicle weight 
	//2 time the car weight assuming gravity is 10 
	m_maxBrakeForce = 2.0f * m_mass * 10.0f;	
	m_maxSteerAngle = 30.0f * DefPO;
	m_maxSteerRate = 0.075f;
	m_engineSteerDiv = 100.0f;
	m_maxSteerForce = 6000.0f;
	m_maxSteerForceRate = 0.03f;
	m_maxSteerSpeedRestriction = 2.0f;
	

//	m_engineTireTorque = 0.0f;
//	m_maxEngineTorque = 6000.0f;
//	m_maxEngineTorqueRate = 500.0f;
/*
	m_fixDeceleration = 0.9975f;
	m_engineTireTorque = 0.0f;

	m_maxBrakeForce = 350.0f;
	m_tiresRollSide = 0;

	m_engineTorqueDiv = 200.0f; 
	// chassis rotation fix...
	m_chassisRotationLimit = 0.98f;
*/
	// set the chassis matrix at the center of mass
	NewtonBodyGetCentreOfMass( m_body0, &com[0] );
	com.m_w = 1.0f;

	// set the joint reference point at the center of mass of the body
	NewtonBodyGetMatrix (m_body0, &chassisMatrix[0][0]);

	//make sure the system matrix do not have any translations on it
	dMatrix cordenateSytemLocal (cordenateSytem);
	cordenateSytemLocal.m_posit = dVector (0.0f, 0.0f, 0.0f, 1.0f);
	chassisMatrix.m_posit += chassisMatrix.RotateVector (com);
	chassisMatrix = cordenateSytemLocal * chassisMatrix;

	// set the car local coordinate system
	CalculateLocalMatrix ( chassisMatrix, m_localFrame, tmp );

	// allocate space for the tires;
	m_tires = new Tire[maxTireCount];

	// Create a simplified normalized Tire Curve
	// we will use a simple piece wise curve at this time, 
	// but end application user can use advance cubers like the Pacejkas tire model

	dFloat slips[] = {0.0f, 0.3f, 0.5f, 2.0f};
	dFloat normalizedLongitudinalForce[] = {0.0f, 1.0f, 0.9f, 0.7f};
	m_normalizedLongitudinalForce.InitalizeCurve (sizeof (slips) / sizeof (dFloat), slips, normalizedLongitudinalForce);

	dFloat sideSlip[] = {0.1f, 0.4f, 0.5f, 2.0f};
	dFloat normalizedLateralForce[] = {0.0f, 1.0f, 0.6f, 0.4f};
	m_normalizedLateralForce.InitalizeCurve (sizeof (sideSlip) / sizeof (dFloat), sideSlip, normalizedLateralForce);


//	m_aerodynamicDrag = 0.1f; 
//	m_aerodynamicDownForce = 0.1f; 

	// set linear and angular Drag to zero, this joint will handle this by using Aerodynamic Drag;
	dVector drag (0.0f, 0.0f, 0.0f, 0.0f); 
	NewtonBodySetLinearDamping (m_body0, 0.0f);
	NewtonBodySetAngularDamping (m_body0, &drag[0]);

	// register the callback for tire integration
	NewtonUserJointSetFeedbackCollectorCallback (m_joint, IntegrateTires);
}

CustomDGRayCastCar::~CustomDGRayCastCar ()
{
	NewtonWorld *world;
	world = NewtonBodyGetWorld (m_body0);

	for ( int i = 0; i < m_tiresCount; i ++ ) {
		NewtonReleaseCollision ( world, m_tires[i].m_shape );
	}
	if (m_tires) {
		delete[] m_tires;
	}
}

CustomDGRayCastCar::NormalizeForceCurve::NormalizeForceCurve ()
{
	m_count = 0;
	m_maxCount = 8;
	m_nodes = new Node[m_maxCount];
}

CustomDGRayCastCar::NormalizeForceCurve::~NormalizeForceCurve ()
{
	delete m_nodes;
}


void CustomDGRayCastCar::NormalizeForceCurve::InitalizeCurve (int points, dFloat* const steps, dFloat* const values)
{
	if (m_maxCount < points) {
		delete[] m_nodes;
		m_maxCount = points;
		m_nodes = new Node[m_maxCount];
	}

	m_count = points;

	for (int i = 0; i < m_count; i ++) {
		m_nodes[i].m_slipRatio = steps[i];
		m_nodes[i].m_normalizedForce = values[i];
	}
}

dFloat CustomDGRayCastCar::NormalizeForceCurve::GetValue (dFloat param) const
{
	dFloat force;
	dFloat sign;
	force = 0.0f;

	sign = 1.0f;
	if (param < 0.0f) {
		sign = -1.0f;
	}

	param *= sign;
	force = m_nodes[m_count - 1].m_normalizedForce;
	if (param < m_nodes[m_count - 1].m_slipRatio) {
		for (int i = 1; i < m_count; i ++) {
			if (param < m_nodes[i].m_slipRatio) {
				dFloat df = m_nodes[i].m_normalizedForce - m_nodes[i - 1].m_normalizedForce;
				dFloat ds = m_nodes[i].m_slipRatio - m_nodes[i - 1].m_slipRatio;
				dFloat step = param - m_nodes[i - 1].m_slipRatio;

				force = m_nodes[i - 1].m_normalizedForce + df * step / ds;
				break;
			}
		}
	}
	return force * sign;
}



void CustomDGRayCastCar::AddSingleSuspensionTire (
	void *userData,
	const dVector& localPosition, 
	dFloat mass,
	dFloat radius, 
	dFloat width,
	dFloat friction,
	dFloat suspensionLenght,
	dFloat springConst,
	dFloat springDamper,
	int castMode)
{
	dVector relTirePos = localPosition;
	suspensionLenght = dAbs ( suspensionLenght );

	dMatrix chassisMatrix;
	NewtonBodyGetMatrix (m_body0, &chassisMatrix[0][0]);
	chassisMatrix = chassisMatrix * chassisMatrix;
	relTirePos += chassisMatrix.m_up.Scale ( suspensionLenght );

	m_tires[m_tiresCount].m_harpoint              = m_localFrame.UntransformVector( relTirePos ); 
	m_tires[m_tiresCount].m_localAxis             = dVector (0.0f, 0.0f, 1.0f, 0.0f);
	m_tires[m_tiresCount].m_tireAxelPosit         = dVector (0.0f, 0.0f, 0.0f, 1.0f);
	m_tires[m_tiresCount].m_tireAxelVeloc         = dVector (0.0f, 0.0f, 0.0f, 1.0f);
	m_tires[m_tiresCount].m_lateralPin            = dVector (0.0f, 0.0f, 0.0f, 1.0f);
	m_tires[m_tiresCount].m_longitudinalPin       = dVector (0.0f, 0.0f, 0.0f, 1.0f);
	m_tires[m_tiresCount].m_hitBodyPointVelocity  = dVector (0.0f, 0.0f, 0.0f, 1.0f);
	m_tires[m_tiresCount].m_HitBody               = NULL;
	m_tires[m_tiresCount].m_userData              = userData;
	m_tires[m_tiresCount].m_spinAngle             = 0.0f;
	m_tires[m_tiresCount].m_steerAngle            = 0.0f;	
	m_tires[m_tiresCount].m_tireLoad              = 0.0f;

	m_tires[m_tiresCount].m_posit                 = suspensionLenght;
	m_tires[m_tiresCount].m_tireIsOnAir           = 1;
	m_tires[m_tiresCount].m_tireUseConvexCastMode = castMode; 
	
	m_tires[m_tiresCount].m_springConst           = springConst;
	m_tires[m_tiresCount].m_springDamper          = springDamper;
	m_tires[m_tiresCount].m_suspensionLenght	  = suspensionLenght;	
	m_tires[m_tiresCount].m_angularVelocity       = 0.0f;
	m_tires[m_tiresCount].m_breakForce            = 0.0f;
	m_tires[m_tiresCount].m_torque                = 0.0f;
	m_tires[m_tiresCount].m_groundFriction        = friction;
	m_tires[m_tiresCount].m_tireIsConstrained	  = 0;	

	m_tires[m_tiresCount].m_mass				  = mass;
	m_tires[m_tiresCount].m_width				  = width;
	m_tires[m_tiresCount].m_radius				  = radius;
	m_tires[m_tiresCount].m_Ixx					  = mass * radius * radius / 2.0f;
	m_tires[m_tiresCount].m_IxxInv				  = 1.0f / m_tires[m_tiresCount].m_Ixx;

#define TIRE_SHAPE_SIZE 12
	dVector shapePoints[TIRE_SHAPE_SIZE * 2];
	for ( int i = 0; i < TIRE_SHAPE_SIZE; i ++ ) {
		shapePoints[i].m_x = -width * 0.5f;	
		shapePoints[i].m_y = radius * dCos ( 2.0f * 3.14159265f * dFloat( i )/ dFloat( TIRE_SHAPE_SIZE ) );
		shapePoints[i].m_z = radius * dSin ( 2.0f * 3.14159265f * dFloat( i )/ dFloat( TIRE_SHAPE_SIZE ) );
		shapePoints[i + TIRE_SHAPE_SIZE].m_x = -shapePoints[i].m_x;
		shapePoints[i + TIRE_SHAPE_SIZE].m_y = shapePoints[i].m_y;
		shapePoints[i + TIRE_SHAPE_SIZE].m_z = shapePoints[i].m_z;
	}
	m_tires[m_tiresCount].m_shape = NewtonCreateConvexHull ( m_world, TIRE_SHAPE_SIZE * 2, &shapePoints[0].m_x, sizeof (dVector), 0.0f, 0, NULL );
// NewtonCreateChamferCylinder(m_world,radius,width,NULL); 
// NewtonCreateSphere(m_world,radius,radius,radius,&offmat[0][0]);
// NewtonCreateCone(m_world,radius,width,NULL);
// NewtonCreateCapsule(m_world,radius,width,NULL);
// NewtonCreateChamferCylinder(m_world,radius,width,NULL);
// NewtonCreateCylinder(m_world,radius*2,width*2,NULL);
// NewtonCreateBox(m_world,radius*2,radius*2,radius*2,NULL);
// NewtonCreateConvexHull (m_world, TIRE_SHAPE_SIZE * 2, &shapePoints[0].m_x, sizeof (dVector), 0.0f, NULL);

	m_tiresCount ++;
}


dFloat CustomDGRayCastCar::GetSpeed() const
{
	return m_curSpeed;
}

int CustomDGRayCastCar::GetTiresCount() const
{
	return m_tiresCount;
}

CustomDGRayCastCar::Tire& CustomDGRayCastCar::GetTire (int index) const
{
	return m_tires[index];
}


void CustomDGRayCastCar::SetTireBrake (int index, dFloat torque)
{
	m_tires[index].m_breakForce = dAbs (torque * m_maxBrakeForce);
}



dMatrix CustomDGRayCastCar::CalculateSuspensionMatrix (int tireIndex, dFloat distance) const
{
	const Tire& tire = m_tires[tireIndex];
	dMatrix matrix;
	// calculate the steering angle matrix for the axis of rotation
	matrix.m_front = tire.m_localAxis;
	matrix.m_up    = dVector (0.0f, 1.0f, 0.0f, 0.0f);
	matrix.m_right = dVector (-tire.m_localAxis.m_z, 0.0f, tire.m_localAxis.m_x, 0.0f);
	matrix.m_posit = tire.m_harpoint - m_localFrame.m_up.Scale ( distance );
	return matrix;
}

dMatrix CustomDGRayCastCar::CalculateTireMatrix (int tireIndex) const
{
	const Tire& tire = m_tires[tireIndex];
	// calculate the rotation angle matrix
	dMatrix angleMatrix ( dPitchMatrix( tire.m_spinAngle ) );
	// get the tire body matrix
	dMatrix bodyMatrix;
	NewtonBodyGetMatrix (m_body0, &bodyMatrix[0][0]);
	return angleMatrix * CalculateSuspensionMatrix (tireIndex, tire.m_posit) * m_localFrame * bodyMatrix;
}

const dMatrix& CustomDGRayCastCar::GetChassisMatrixLocal () const
{
	return m_localFrame;
}


dFloat CustomDGRayCastCar::GetTireParametricPosition (int index) const
{
	return m_tires[index].m_posit / m_tires[index].m_suspensionLenght;
}


dFloat CustomDGRayCastCar::GenerateTiresSteerForce (dFloat value)
{
	dFloat speed, relspeed;
	relspeed = dAbs ( GetSpeed() ); 
	speed = relspeed;
	speed *= m_maxSteerForceRate;
	dFloat value2 = value;
	if ( speed > m_maxSteerSpeedRestriction ) {
		speed = m_maxSteerSpeedRestriction; 
	}
	if ( value2 > 0.0f ) {
		value2 = -( m_maxSteerForce * speed ) * ( 1.0f - relspeed / m_engineSteerDiv );
	} else 
		if ( value < 0.0f ) {
			value2 = ( m_maxSteerForce * speed ) * ( 1.0f - relspeed / m_engineSteerDiv );
		} else {
			value2 = 0.0f;
		}
		return value2;
}

dFloat CustomDGRayCastCar::GenerateTiresSteerAngle (dFloat value)
{
	if ( value > 0.0f ) {
		m_steerAngle += m_maxSteerRate;
		if ( m_steerAngle > m_maxSteerAngle ) {
			m_steerAngle = m_maxSteerAngle;
		}
	} else {
		if ( value < 0.0f ) {
			m_steerAngle -= m_maxSteerRate;
			if ( m_steerAngle < -m_maxSteerAngle ) {
				m_steerAngle = -m_maxSteerAngle;
			}
		} else {
			if ( m_steerAngle > 0.0f ) {
				m_steerAngle -= m_maxSteerRate;
				if ( m_steerAngle < 0.0f ) {
					m_steerAngle = 0.0f;
				}
			} else 
				if ( m_steerAngle < 0.0f ) {
					m_steerAngle += m_maxSteerRate;
					if ( m_steerAngle > 0.0f ) {
						m_steerAngle = 0.0f;
					}
				} 
		}
	}

	return m_steerAngle;
}


dFloat CustomDGRayCastCar::GenerateEngineTorque (dFloat value)
{
	// this function generates Engine Torque using the transmision and tire angular speed
	/// for now just engine torque is fixed
	return -value * 500.0f;

/*
	dFloat speed;
	speed = dAbs( GetSpeed() );
	if ( value > 0.0f ) {
		m_engineTireTorque += m_maxEngineTorqueRate; 
		if ( m_engineTireTorque > m_maxEngineTorque ) {
			m_engineTireTorque = m_maxEngineTorque;
		}
	} else {
		if ( value < 0.0f ) {
			m_engineTireTorque -= m_maxEngineTorqueRate; 
			if ( m_engineTireTorque < -m_maxEngineTorque ) {
				m_engineTireTorque = -m_maxEngineTorque;
			}
		} else {
			if ( m_engineTireTorque > 0.0f ) {
				m_engineTireTorque -= m_maxEngineTorqueRate;
				if ( m_engineTireTorque < 0.0f ) {
					m_engineTireTorque = 0.0f;
				}
			} else {
				if ( m_engineTireTorque < 0.0f ) {
					m_engineTireTorque += m_maxEngineTorqueRate;
					if ( m_engineTireTorque > 0.0f ) {
						m_engineTireTorque = 0.0f;
					}
				} 
			}
		}
	}

	return -m_engineTireTorque;
*/
}


void CustomDGRayCastCar::SetTireTorque (int index, dFloat torque)
{
	m_tires[index].m_torque = torque;
}



void CustomDGRayCastCar::SetTireSteerAngleForce (int index, dFloat angle, dFloat turnforce)
{
	m_tires[index].m_steerAngle = angle;
	m_tires[index].m_localAxis.m_z = dCos (angle);
	m_tires[index].m_localAxis.m_x = dSin (angle);

//	if (m_tiresRollSide==0) {
//		m_tires[index].m_turnforce = turnforce;
//	} else {
//		m_tires[index].m_turnforce = -turnforce;
//	}
}


#if 0
void CustomDGRayCastCar::SetVarChassisRotationLimit (dFloat value)
{
	m_chassisRotationLimit = value;
}

void CustomDGRayCastCar::SetVarFixDeceleration (dFloat value)
{
	m_fixDeceleration = value;
}

void CustomDGRayCastCar::SetVarMaxSteerAngle (dFloat value)
{
	m_maxSteerAngle = value * DefPO;
}

void CustomDGRayCastCar::SetVarMaxSteerRate (dFloat value)
{
	m_maxSteerRate = value;
}

void CustomDGRayCastCar::SetVarMaxSteerForceRate (dFloat value)
{
	m_maxSteerForceRate = value;
}

void CustomDGRayCastCar::SetVarMaxSteerForce (dFloat value)
{
	m_maxSteerForce = value;
}

void CustomDGRayCastCar::SetVarMaxSteerSpeedRestriction (dFloat value)
{
	m_maxSteerSpeedRestriction = value;
}

void CustomDGRayCastCar::SetVarMaxBrakeForce (dFloat value)
{
	m_maxBrakeForce = value;
}

void CustomDGRayCastCar::SetVarMaxTorque (dFloat value)
{
	m_maxTorque = value;
}

void CustomDGRayCastCar::SetVarMaxTorqueRate (dFloat value)
{
	m_maxTorqueRate = value;
}

void CustomDGRayCastCar::SetVarEngineSteerDiv (dFloat value)
{
	m_engineSteerDiv = value;
}



int CustomDGRayCastCar::GetVehicleOnAir() const
{
	return m_vehicleOnAir;
}

int CustomDGRayCastCar::GetTireOnAir(int index) const
{
	const Tire& tire = m_tires[index];	
	return tire.m_tireIsOnAir;
}

const NewtonCollision* CustomDGRayCastCar::GetTiresShape (int tireIndex) const
{
	const Tire& tire = m_tires[tireIndex];	
	return tire.m_shape;
}

void CustomDGRayCastCar::GetInfo (NewtonJointRecord* info) const
{
}

//this function is to be overloaded by a derive class
void CustomDGRayCastCar::SetSteering (dFloat angle)
{
}

//this function is to be overloaded by a derive class
void CustomDGRayCastCar::SetBrake (dFloat torque)
{
}

//this function is to be overloaded by a derive class
void CustomDGRayCastCar::SetTorque (dFloat torque)
{
}


void CustomDGRayCastCar::SetTireMaxRPS (int tireIndex, dFloat maxTireRPS)
{
	m_tires[tireIndex].m_maxTireRPS = maxTireRPS;
}

void CustomDGRayCastCar::SetVarTireSuspenssionHardLimit (int index, dFloat value)
{
	m_tires[index].m_suspenssionHardLimit = value;
}

void CustomDGRayCastCar::SetVarTireFriction (int index, dFloat value)
{
	m_tires[index].m_groundFriction = value;
}







dFloat CustomDGRayCastCar::GenerateTiresBrake (dFloat value)
{
	return value * m_maxBrakeForce; 
}




void CustomDGRayCastCar::SetVarTireMovePointForceFront (int index, dFloat distance)
{
	m_tires[index].m_MovePointForceFront = distance;
}

void CustomDGRayCastCar::SetVarTireMovePointForceRight (int index, dFloat distance)
{
	m_tires[index].m_MovePointForceRight = distance;
}

void CustomDGRayCastCar::SetVarTireMovePointForceUp (int index, dFloat distance)
{
	m_tires[index].m_MovePointForceUp = distance;
}




dFloat CustomDGRayCastCar::ApplySuspenssionLimit (Tire& tire)
{ 
	// This add a hard suspension limit. 
	// At very high speed the vehicle mass can make get the suspension limit faster.
	// This solution avoid this problem and push up the suspension.
	dFloat distance;
	distance = tire.m_suspensionLenght - tire.m_posit;
	if (distance>=tire.m_suspensionLenght){
		// The tire is inside the vehicle chassis.
		// Normally at this place the tire can't give torque on the wheel because the tire touch the chassis.
		// Set the tire speed to zero here is a bad idea.
		// Because the spring coming very bumpy and make the chassis go on any direction.
		// tire.m_tireSpeed = 0.0f;
		tire.m_torque = 0.0f;
		distance = (tire.m_suspensionLenght - tire.m_posit) + tire.m_suspenssionHardLimit;
	}
	return distance;
}

void CustomDGRayCastCar::ApplyOmegaCorrection ()
{ 
	m_chassisOmega = m_chassisOmega.Scale( m_chassisRotationLimit );
	NewtonBodySetOmega( m_body0, &m_chassisOmega[0] );
}

void CustomDGRayCastCar::ApplyChassisForceAndTorque (const dVector& vForce, const dVector& vPoint)
{
	NewtonBodyAddForce( m_body0, &vForce[0] );
	ApplyChassisTorque( vForce, vPoint ); 
}

void CustomDGRayCastCar::ApplyDeceleration (Tire& tire)
{
	if ( dAbs( tire.m_torque ) < 1.0e-3f ){
		dVector cvel = m_chassisVelocity;   
		cvel = cvel.Scale( m_fixDeceleration );
		cvel.m_y = m_chassisVelocity.m_y;
		NewtonBodySetVelocity( m_body0, &cvel.m_x );
	}	
}

void CustomDGRayCastCar::ApplyChassisTorque (const dVector& vForce, const dVector& vPoint)
{
	dVector Torque;
	dMatrix M;
	dVector com;
	NewtonBodyGetCentreOfMass( m_body0, &com[0] );
	NewtonBodyGetMatrix( m_body0, &M[0][0] );
	Torque = ( vPoint - M.TransformVector( dVector( com.m_x, com.m_y, com.m_z, 1.0f ) ) ) * vForce;
	NewtonBodyAddTorque( m_body0, &Torque[0] );
}






void CustomDGRayCastCar::ApplyTiresTorqueVisual (Tire& tire, dFloat timestep, int threadIndex)
{
	dFloat timestepInv;
	// get the simulation time
	timestepInv = 1.0f / timestep;
	dVector tireRadius (tire.m_contactPoint - tire.m_tireAxelPosit);
	dFloat tireLinearSpeed;
	dFloat tireContactSpeed;
	tireLinearSpeed = tire.m_tireAxelVeloc % tire.m_longitudinalPin;	
	tireContactSpeed = (tire.m_lateralPin * tireRadius) % tire.m_longitudinalPin;
	//check if any engine torque or brake torque is applied to the tire
	if (dAbs(tire.m_torque) < 1.0e-3f){
		//tire is coasting, calculate the tire zero slip angular velocity
		// this is the velocity that satisfy the constraint equation
		// V % dir + W * R % dir = 0
		// where V is the tire Axel velocity
		// W is the tire local angular velocity
		// R is the tire radius
		// dir is the longitudinal direction of of the tire.		
		// this checkup is suposed to fix a infinit division by zero...
		if ( dAbs(tireContactSpeed)  > 1.0e-3) { 
			tire.m_angularVelocity = - (tireLinearSpeed) / (tireContactSpeed);
		}
		tire.m_spinAngle = dMod (tire.m_spinAngle + tire.m_angularVelocity * timestep, 3.14159265f * 2.0f);
	} else {
		// tire is under some power, need to do the free body integration to apply the net torque
		dFloat nettorque = tire.m_angularVelocity;
		// this checkup is suposed to fix a infinit division by zero...
		if ( dAbs(tireContactSpeed)  > 1.0e-3) { 
			nettorque = - (tireLinearSpeed) / (tireContactSpeed);
		} 
		//tire.m_angularVelocity = - tireLinearSpeed / tireContactSpeed;
		dFloat torque;
		torque = tire.m_torque - nettorque - tire.m_angularVelocity * tire.m_Ixx * 0.1f;
		tire.m_angularVelocity  += torque * tire.m_IxxInv * timestep;
		tire.m_spinAngle = dMod (tire.m_spinAngle + tire.m_angularVelocity * timestep, 3.14159265f * 2.0f); 
	}
}
#endif


unsigned CustomDGRayCastCar::ConvexCastPrefilter (const NewtonBody* body, const NewtonCollision* collision, void* userData)
{
	NewtonBody* me;
	me = (NewtonBody*) userData;
	// do no cast myself
	return ( me != body );
}


void CustomDGRayCastCar::CalculateTireCollision (Tire& tire, const dMatrix& suspensionMatrix, int threadIndex) const
{
	int floorcontact = 0;
	tire.m_HitBody = NULL;

	tire.m_posit = tire.m_suspensionLenght;
	if ( tire.m_tireUseConvexCastMode ) {
		dFloat hitParam;
		NewtonWorldConvexCastReturnInfo info;

//		tire.m_rayDestination = tire.m_suspensionMatrix.TransformVector (m_localFrame.m_up.Scale ( -tire.m_suspensionLenght ));   
		dVector rayDestination (suspensionMatrix.TransformVector (m_localFrame.m_up.Scale ( -tire.m_suspensionLenght)));   
		if ( NewtonWorldConvexCast ( m_world, &suspensionMatrix[0][0], &rayDestination[0], tire.m_shape, &hitParam, (void*)m_body0, ConvexCastPrefilter, &info, 1, threadIndex ) ){
			tire.m_posit = hitParam * tire.m_suspensionLenght;
			tire.m_contactPoint = info.m_point;
			tire.m_contactNormal = info.m_normal;
			tire.m_HitBody = (NewtonBody*)info.m_hitBody; 
			floorcontact = 1;
		}
	} else {
		struct RayCastInfo
		{
			RayCastInfo (const NewtonBody* body)
			{
				m_param = 1.0f;
				m_me = body;
				m_hitBody = NULL;
				m_contactID = 0;
				m_normal = dVector (0.0f, 0.0f, 0.0f, 1.0f);
			}
			static dFloat RayCast (const NewtonBody* body, const dFloat* normal, int collisionID, void* userData, dFloat intersetParam)
			{
				RayCastInfo& caster = *( (RayCastInfo*) userData ); 
				// if this body is not the vehicle, see if a close hit
				if ( body != caster.m_me ) {
					if ( intersetParam < caster.m_param) {
						// this is a close hit, record the information. 
						caster.m_param = intersetParam;
						caster.m_hitBody = body;
						caster.m_contactID = collisionID;
						caster.m_normal = dVector (normal[0], normal[1], normal[2], 1.0f);
					} 
				}
				return intersetParam;
			}
			dFloat m_param;
			dVector m_normal;
			const NewtonBody* m_me;
			const NewtonBody* m_hitBody;
			int m_contactID;
		};
		RayCastInfo info (m_body0);

_ASSERTE (0);
		// extend the ray by the radius of the tire
		dFloat dist ( tire.m_suspensionLenght + tire.m_radius );
		dVector rayDestination (suspensionMatrix.TransformVector (m_localFrame.m_up.Scale ( -dist )));	
		// cast a ray to the world ConvexCastPrefilter
		NewtonWorldRayCast( m_world, &suspensionMatrix.m_posit[0], &rayDestination[0], RayCastInfo::RayCast, &info, &ConvexCastPrefilter );
		// if the ray hit something, it means the tire has some traction
		if ( info.m_hitBody ) {
			dFloat intesectionDist;
			tire.m_HitBody = (NewtonBody*)info.m_hitBody; 
			tire.m_contactPoint = suspensionMatrix.m_posit + (rayDestination - suspensionMatrix.m_posit ).Scale ( info.m_param ); 
			tire.m_contactNormal = info.m_normal;  

			// TO DO: get the material properties for tire frictions on different roads 
			intesectionDist = ( dist * info.m_param - tire.m_radius );
			if ( intesectionDist < 0.0f ) {
				intesectionDist = 0.0f;
			} else if ( intesectionDist > tire.m_suspensionLenght ) {
				intesectionDist = tire.m_suspensionLenght;
			}
			tire.m_posit = intesectionDist;
		}
	}
}


void CustomDGRayCastCar::ApplyTireFrictionVelocitySiding (Tire& tire, const dMatrix& chassisMatrix, const dVector& tireAxelVeloc, const dVector& tireAxelPosit, dFloat timestep, dFloat invTimestep)
{
	dFloat invMag2;
	dFloat frictionCircleMag;
	dFloat lateralFrictionForceMag;
	dFloat longitudinalFrictionForceMag;
//	dFloat tireContactSpeed;
//	dFloat tireRelativeSpeed;
//	dFloat lateralForceMagnitud;

	// calculate relative velocity at the tire center
	dVector tireAxelRelativeVelocity (tireAxelVeloc - tire.m_hitBodyPointVelocity); 

	// now calculate relative velocity a velocity at contact point
	dVector tireAngularVelocity ( tire.m_lateralPin.Scale (tire.m_angularVelocity));
	dVector tireRadius (tire.m_contactPoint - tireAxelPosit);
	dVector tireContactVelocity ( tireAngularVelocity * tireRadius);	
	dVector tireContactRelativeVelocity ( tireAxelRelativeVelocity + tireContactVelocity ); 


	// Apply brake, need some little fix here.
	// The fix is need to generate axial force when the brake is apply when the vehicle turn from the steer or on sliding.
	if ( dAbs( tire.m_breakForce ) > 1.0e-3f ) {
		_ASSERTE (0);
//		tire.m_isBraking = 1;
//		tire.m_torque = 0.0f;
//		tire.m_turnforce = tire.m_turnforce * 0.5f;
//		tire.m_breakForce /= timestep;
//		NewtonUserJointAddLinearRow ( m_joint, &tireAxelPosit[0], &tireAxelPosit[0], &chassisMatrix.m_front.m_x  );
//		NewtonUserJointSetRowMaximumFriction( m_joint, tire.m_breakForce );
//		NewtonUserJointSetRowMinimumFriction( m_joint, -tire.m_breakForce );
	} 

	tire.m_breakForce = 0.0f;  

	//submit constrained for applying side forces.
	frictionCircleMag = tire.m_tireLoad * tire.m_groundFriction;
	lateralFrictionForceMag = frictionCircleMag;
	longitudinalFrictionForceMag = tire.m_tireLoad;
	invMag2 = frictionCircleMag / dSqrt ( lateralFrictionForceMag * lateralFrictionForceMag + longitudinalFrictionForceMag * longitudinalFrictionForceMag );

	lateralFrictionForceMag *= invMag2;
	longitudinalFrictionForceMag = invMag2;
	NewtonUserJointAddLinearRow (m_joint, &tireAxelPosit[0], &tireAxelPosit[0], &tire.m_lateralPin[0]);
	NewtonUserJointSetRowMaximumFriction (m_joint,  lateralFrictionForceMag);
	NewtonUserJointSetRowMinimumFriction (m_joint, -lateralFrictionForceMag);

	// save the tire contact longitudinal velocity for integration after the solver
//	tire.m_currentSlipVeloc = tireAxelRelativeVelocity % tire.m_longitudinalPin;
}

void CustomDGRayCastCar::IntegrateTires (const NewtonJoint* userJoint, dFloat timestep, int threadIndex)
{
	CustomDGRayCastCar* joint;  

	// get the pointer to the joint class
	joint = (CustomDGRayCastCar*) NewtonJointGetUserData (userJoint);  
	joint->IntegrateTires(timestep, threadIndex);
}

void CustomDGRayCastCar::IntegrateTires (dFloat timestep, int threadIndex)
{
	dMatrix bodyMatrix;  

	// get the vehicle global matrix, and use it in several calculations
	NewtonBodyGetMatrix (m_body0, &bodyMatrix[0][0]);
	dMatrix chassisMatrix (m_localFrame * bodyMatrix);

	// get the chassis instantaneous linear and angular velocity in the local space of the chassis
	dVector bodyOmega;
	dVector bodyVelocity;
	
	NewtonBodyGetVelocity (m_body0, &bodyVelocity[0]);
	NewtonBodyGetOmega (m_body0, &bodyOmega[0]);

	// set the current vehicle speed
	m_curSpeed = bodyMatrix.m_front % bodyVelocity;

	for (int i = 0; i < m_tiresCount; i ++ ) {
		Tire& tire = m_tires[i];
/*
		if (tire.m_tireIsConstrained) {
			// the torqued generate by the tire can no be larger than the external torque on the tire 
			// when this happens ther tire is spinning unde contrained rotation 

			// V % dir + W * R % dir = 0
			// where V is the tire Axel velocity
			// W is the tire local angular velocity
			// R is the tire radius
			// dir is the longitudinal direction of of the tire.

			dFloat contactRadius;
			dFloat axelLinealSpeed;
			dVector tireAxelPosit (chassisMatrix.TransformVector (tire.m_harpoint - m_localFrame.m_up.Scale (tire.m_posit)));
			dVector tireAxelVeloc = bodyVelocity + bodyOmega * (tireAxelPosit - chassisMatrix.m_posit) - tire.m_hitBodyPointVelocity; 
			axelLinealSpeed = tireAxelVeloc % chassisMatrix.m_front;


			dVector tireRadius (tire.m_contactPoint - tire.m_tireAxelPosit);
			contactRadius = (tire.m_lateralPin * tireRadius) % tire.m_longitudinalPin;
			tire.m_angularVelocity = - axelLinealSpeed / contactRadius ;

		} else if (tire.m_tireIsOnAir) {
			if (tire.m_breakForce > 1.0e-3f) {
				tire.m_angularVelocity = 0.0f;
			} else {
				//the tire is on air, need to be integrate net toque and apply a drag coneficenct

	//			dFloat nettorque = tire.m_angularVelocity;
	//			// this checkup is suposed to fix a infinit division by zero...
	//			if ( dAbs(tireContactSpeed)  > 1.0e-3) { 
	//				nettorque = - (tireLinearSpeed) / (tireContactSpeed);
	//			} 
				//tire.m_angularVelocity = - tireLinearSpeed / tireContactSpeed;
				dFloat torque;
				torque = tire.m_torque - tire.m_angularVelocity * tire.m_Ixx * TIRE_VISCUOS_DAMP;
				tire.m_angularVelocity += torque * tire.m_IxxInv * timestep;
			}
		} else {
			// there is a next torque on the tire
			dFloat torque;
			torque = tire.m_torque - tire.m_angularVelocity * tire.m_Ixx * TIRE_VISCUOS_DAMP;
			tire.m_angularVelocity += torque * tire.m_IxxInv * timestep;
		}
*/
		if (tire.m_tireIsOnAir) {
			if (tire.m_breakForce > 1.0e-3f) {
				tire.m_angularVelocity = 0.0f;
			} else {
				//the tire is on air, need to be integrate net toque and apply a drag coeficient
				dFloat torque;
				torque = tire.m_torque - tire.m_angularVelocity * tire.m_Ixx * TIRE_VISCUOS_DAMP;
				tire.m_angularVelocity += torque * tire.m_IxxInv * timestep;
			}
		} else if (tire.m_tireIsConstrained) {
			// the torqued generate by the tire can no be larger than the external torque on the tire 
			// when this happens there tire is spinning under constrained rotation 

			// V % dir + W * R % dir = 0
			// where V is the tire Axel velocity
			// W is the tire local angular velocity
			// R is the tire radius
			// dir is the longitudinal direction of of the tire.

			dFloat contactRadius;
			dFloat axelLinealSpeed;
			dVector tireAxelPosit (chassisMatrix.TransformVector (tire.m_harpoint - m_localFrame.m_up.Scale (tire.m_posit)));
			dVector tireAxelVeloc = bodyVelocity + bodyOmega * (tireAxelPosit - chassisMatrix.m_posit) - tire.m_hitBodyPointVelocity; 
			axelLinealSpeed = tireAxelVeloc % chassisMatrix.m_front;


			dVector tireRadius (tire.m_contactPoint - tire.m_tireAxelPosit);
			contactRadius = (tire.m_lateralPin * tireRadius) % tire.m_longitudinalPin;
			tire.m_angularVelocity = - axelLinealSpeed / contactRadius ;

		} else {

			// there is a next torque on the tire
			dFloat torque;
			torque = tire.m_torque - tire.m_angularVelocity * tire.m_Ixx * TIRE_VISCUOS_DAMP;
			tire.m_angularVelocity += torque * tire.m_IxxInv * timestep;
		}


		// spin the tire by the angular velocity
		tire.m_spinAngle = dMod (tire.m_spinAngle + tire.m_angularVelocity * timestep, 3.14159265f * 2.0f);

		// reset the tire torque
		tire.m_torque = 0.0f;
		tire.m_breakForce = 0.0f;  
	}
}


void CustomDGRayCastCar::SubmitConstraints (dFloat timestep, int threadIndex)
{
	int constraintIndex;
	dFloat invTimestep;
	dMatrix bodyMatrix;  

	// get the simulation time
	invTimestep = 1.0f / timestep ;

	// get the vehicle global matrix, and use it in several calculations
	NewtonBodyGetMatrix (m_body0, &bodyMatrix[0][0]);
	dMatrix chassisMatrix (m_localFrame * bodyMatrix);

	// get the chassis instantaneous linear and angular velocity in the local space of the chassis
	dVector bodyOmega;
	dVector bodyVelocity;
	
	NewtonBodyGetVelocity (m_body0, &bodyVelocity[0]);
	NewtonBodyGetOmega (m_body0, &bodyOmega[0]);

	// all tire is on air check
	m_vehicleOnAir = 0;
	constraintIndex = 0;
	for ( int i = 0; i < m_tiresCount; i ++ ) {

		Tire& tire = m_tires[i];
		tire.m_tireIsOnAir = 1;
		tire.m_tireIsConstrained = 0;	
		tire.m_tireForceAcc = dVector(0.0f, 0.0f, 0.0f, 0.0f);

		// calculate all suspension matrices in global space and tire collision
		dMatrix suspensionMatrix (CalculateSuspensionMatrix (i, 0.0f) * chassisMatrix);

		// calculate the tire collision
		CalculateTireCollision (tire, suspensionMatrix, threadIndex);

		// calculate the linear velocity of the tire at the ground contact
		tire.m_tireAxelPosit = chassisMatrix.TransformVector( tire.m_harpoint - m_localFrame.m_up.Scale (tire.m_posit));
		tire.m_tireAxelVeloc = bodyVelocity + bodyOmega * (tire.m_tireAxelPosit - chassisMatrix.m_posit); 
		tire.m_lateralPin = ( chassisMatrix.RotateVector ( tire.m_localAxis ) );
		tire.m_longitudinalPin = ( chassisMatrix.m_up * tire.m_lateralPin );

		if (tire.m_posit < tire.m_suspensionLenght )  {
			dFloat distance;
			dFloat sideSlipCoef;
			dFloat slipRatioCoef;
			dFloat tireForceMag;
			dFloat tireTorqueMag;
			dFloat suspensionSpeed;
			dFloat axelLinealSpeed;
			dFloat tireRotationSpeed;
			dFloat frictionCircleMag;
			dFloat longitudinalForceMag;
			dFloat lateralFrictionForceMag;


			tire.m_tireIsOnAir = 0;
			tire.m_hitBodyPointVelocity = dVector (0.0f, 0.0f, 0.0f, 1.0f);
			if (tire.m_HitBody){
				dMatrix matrix;
				dVector com;
				dVector omega;

				NewtonBodyGetOmega (tire.m_HitBody, &omega[0]);
				NewtonBodyGetMatrix (tire.m_HitBody, &matrix[0][0]);
				NewtonBodyGetCentreOfMass (tire.m_HitBody, &com[0]);
				NewtonBodyGetVelocity (tire.m_HitBody, &tire.m_hitBodyPointVelocity[0]);
				tire.m_hitBodyPointVelocity += (tire.m_contactPoint - matrix.TransformVector (com)) * omega;
			} 

			// calculate the relative velocity
			dVector relVeloc (tire.m_tireAxelVeloc - tire.m_hitBodyPointVelocity);
			suspensionSpeed = - (relVeloc % chassisMatrix.m_up);

			// now calculate the tire load at the contact point
			// Tire suspension distance and hard limit.
			distance = tire.m_suspensionLenght - tire.m_posit;
			_ASSERTE (distance <= tire.m_suspensionLenght);
			tire.m_tireLoad = - NewtonCalculateSpringDamperAcceleration (timestep, tire.m_springConst, distance, tire.m_springDamper, suspensionSpeed );
			if ( tire.m_tireLoad < 0.0f ) {
				// since the tire is not a body with real mass it can only push the chassis.
				tire.m_tireLoad = 0.0f;
			} 

			//this suspension is applying a normalize force to the car chassis, need to scales by the mass of the car
			tire.m_tireLoad *= (m_mass * 0.5f);

			tire.m_tireIsConstrained = (dAbs (tire.m_torque) < 0.3f);

			// convert the tire load force magnitude to a torque and force.
			// accumulate the force doe to the suspension spring and damper
			tire.m_tireForceAcc += chassisMatrix.m_up.Scale (tire.m_tireLoad);

			// calculate relative velocity at the tire center
			dVector tireAxelRelativeVelocity (tire.m_tireAxelVeloc - tire.m_hitBodyPointVelocity); 

			// axle linear speed
			axelLinealSpeed = tireAxelRelativeVelocity % chassisMatrix.m_front;

			// calculate tire rotation velocity at the tire radio
			dVector tireAngularVelocity (tire.m_lateralPin.Scale (tire.m_angularVelocity));
			dVector tireRadius (tire.m_contactPoint - tire.m_tireAxelPosit);
			dVector tireRotationalVelocityAtContact (tireAngularVelocity * tireRadius);	

			longitudinalForceMag = 0.0f;
//			if (!tire.m_tireIsConstrained) {
				
				// calculate slip ratio and max longitudinal force
				tireRotationSpeed = tireRotationalVelocityAtContact % tire.m_longitudinalPin;
				slipRatioCoef = (dAbs (axelLinealSpeed) > 1.e-3f) ? ((-tireRotationSpeed - axelLinealSpeed) / dAbs (axelLinealSpeed)) : 0.0f;

				// calculate the formal longitudinal force the tire apply to the chassis
				longitudinalForceMag = m_normalizedLongitudinalForce.GetValue (slipRatioCoef) * tire.m_tireLoad * tire.m_groundFriction;
//			} 

		
			// now calculate relative velocity a velocity at contact point
			dVector tireContactRelativeVelocity (tireAxelRelativeVelocity + tireRotationalVelocityAtContact); 

			// calculate the sideslip as the angle between the tire lateral speed and longitudila speed 
			sideSlipCoef = dAtan2 (dAbs (tireContactRelativeVelocity % tire.m_lateralPin), dAbs (axelLinealSpeed));
			lateralFrictionForceMag = m_normalizedLateralForce.GetValue (sideSlipCoef) * tire.m_tireLoad * tire.m_groundFriction;

			// Apply brake, need some little fix here.
			// The fix is need to generate axial force when the brake is apply when the vehicle turn from the steer or on sliding.
			if ( tire.m_breakForce > 1.0e-3f ) {
				// row constrained force is save for later determine the dynamic state of this tire 
  				tire.m_isBrakingForceIndex = constraintIndex;
				constraintIndex ++;

				frictionCircleMag = tire.m_tireLoad * tire.m_groundFriction;
				if (tire.m_breakForce > frictionCircleMag) {
					tire.m_breakForce = frictionCircleMag;
				}

				//NewtonUserJointAddLinearRow ( m_joint, &tire.m_tireAxelPosit[0], &tire.m_tireAxelPosit[0], &chassisMatrix.m_front.m_x  );
				NewtonUserJointAddLinearRow (m_joint, &tire.m_tireAxelPosit[0], &tire.m_tireAxelPosit[0], &tire.m_longitudinalPin.m_x);
				NewtonUserJointSetRowMaximumFriction( m_joint, tire.m_breakForce);
				NewtonUserJointSetRowMinimumFriction( m_joint, -tire.m_breakForce);

				// there is a longitudinal force that will reduce the lateral force, we need to recalculate the lateral force
				tireForceMag = lateralFrictionForceMag * lateralFrictionForceMag + tire.m_breakForce * tire.m_breakForce;
				if (tireForceMag > (frictionCircleMag * frictionCircleMag)) {
  					lateralFrictionForceMag *= 0.25f * frictionCircleMag / dSqrt (tireForceMag);
				}
			} 


			//project the longitudinal and lateral forces over the circle of friction for this tire; 
			frictionCircleMag = tire.m_tireLoad * tire.m_groundFriction;
			tireForceMag = lateralFrictionForceMag * lateralFrictionForceMag + longitudinalForceMag * longitudinalForceMag;
			if (tireForceMag > (frictionCircleMag * frictionCircleMag)) {
				dFloat invMag2;
				invMag2 = frictionCircleMag / dSqrt (tireForceMag);
				longitudinalForceMag *= invMag2;
				lateralFrictionForceMag *= invMag2;
			}

			// submit this constraint for calculation of side slip forces
			lateralFrictionForceMag = dAbs (lateralFrictionForceMag);
			tire.m_lateralForceIndex = constraintIndex;
			constraintIndex ++;
			NewtonUserJointAddLinearRow (m_joint, &tire.m_tireAxelPosit[0], &tire.m_tireAxelPosit[0], &tire.m_lateralPin[0]);
			NewtonUserJointSetRowMaximumFriction (m_joint,  lateralFrictionForceMag);
			NewtonUserJointSetRowMinimumFriction (m_joint, -lateralFrictionForceMag);

			// accumulate the longitudinal force
			dVector tireForce (tire.m_longitudinalPin.Scale (longitudinalForceMag));
			tire.m_tireForceAcc += tireForce;

			// now we apply the combined tire force generated by this tire, to the car chassis
			dVector torque ((tire.m_tireAxelPosit - chassisMatrix.m_posit) * tire.m_tireForceAcc);
			NewtonBodyAddForce (m_body0, &tire.m_tireForceAcc[0]);
			NewtonBodyAddTorque( m_body0, &torque[0] );


			// calculate the net torque on the tire
			tireTorqueMag = -((tireRadius * tireForce) % tire.m_lateralPin);
			if (dAbs (tireTorqueMag) > dAbs (tire.m_torque)) {
				// the tire reaction force can no be larger than the applied engine torque 
				// when this happens the net torque is zero and the tire is constrained to the vehicle linear motion
				tire.m_tireIsConstrained = 1;
				tireTorqueMag = tire.m_torque;
			}

			tire.m_torque -= tireTorqueMag;
		} 	
	}
}



