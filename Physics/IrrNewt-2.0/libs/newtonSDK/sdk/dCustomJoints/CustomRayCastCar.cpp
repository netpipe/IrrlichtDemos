// CustomRayCastCar.cpp: implementation of the CustomRayCastCar class.
//
//////////////////////////////////////////////////////////////////////
#include "CustomJointLibraryStdAfx.h"
#include "CustomRayCastCar.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef USE_MASTER_KOOM_VARIANCE




CustomRayCastCar::CustomRayCastCar(int maxTireCount, const dMatrix& cordenateSytem, NewtonBody* carBody, const dVector& gravity)
	:NewtonCustomJoint(0, carBody, NULL), m_gravity (gravity)
{
	dVector com;
	dMatrix tmp;

	m_gravity.m_y = -12.0f;
	m_tiresRollSide = 0;

	// set the chassis matrix at the center of mass
	NewtonBodyGetCentreOfMass(m_body0, &com[0]);
	com.m_w = 1.0f;

	// set the joint reference point at the center of mass of the body
	dMatrix chassisMatrix (cordenateSytem);
	chassisMatrix.m_posit += chassisMatrix.RotateVector(com);

	CalculateLocalMatrix (chassisMatrix, m_localFrame, tmp);

	// allocate space for the tires;
	m_tiresCount = 0;
	m_tires = new Tire[maxTireCount];

	m_curSpeed = 0.0f;
	m_aerodynamicDrag = 0.1f; 
	m_aerodynamicDownForce = 0.1f; 

	dFloat Ixx; 
	dFloat Iyy; 
	dFloat Izz; 
	NewtonBodyGetMassMatrix(m_body0, &m_mass, &Ixx, &Iyy, &Izz);

	// register the callback for tire integration
//	NewtonUserJointSetFeedbackCollectorCallback (m_joint, IntegrateTires);
}


CustomRayCastCar::~CustomRayCastCar()
{
	NewtonWorld *world;

	world = NewtonBodyGetWorld (m_body0);
	for (int i = 0; i < m_tiresCount; i ++) {
		NewtonReleaseCollision (world, m_tires[i].m_shape);
	}

	if(m_tires) {
		delete[] m_tires;
	}
}


int CustomRayCastCar::GetTiresCount() const
{
	return m_tiresCount;
}




void CustomRayCastCar::GetInfo (NewtonJointRecord* info) const
{
}

//this function is to be overloaded by a derive class
void CustomRayCastCar::SetSteering (dFloat angle)
{
}

//this function is to be overloaded by a derive class
void CustomRayCastCar::SetBrake (dFloat torque)
{
}

//this function is to be overloaded by a derive class
void CustomRayCastCar::SetTorque (dFloat torque)
{
}

dFloat CustomRayCastCar::GetSpeed() const
{
	return m_curSpeed;
}


void CustomRayCastCar::SetTireMaxRPS (int tireIndex, dFloat maxTireRPS)
{
	m_tires[tireIndex].m_maxTireRPS = maxTireRPS;
}

CustomRayCastCar::Tire& CustomRayCastCar::GetTire (int index) const
{
	return m_tires[index];
}

dFloat CustomRayCastCar::GetParametricPosition (int index) const
{
	return m_tires[index].m_posit / m_tires[index].m_suspensionLength;
}

void CustomRayCastCar::SetTireSteerAngle (int index, dFloat angle, dFloat turnforce)
{
   m_tires[index].m_steerAngle = angle;
   m_tires[index].m_localAxis.m_z = dCos (angle);
   m_tires[index].m_localAxis.m_x = dSin (angle);
	if (m_tiresRollSide==0) {
	  m_tires[index].m_turnforce = turnforce;
	} else {
	  m_tires[index].m_turnforce = -turnforce;
	}
}

void CustomRayCastCar::SetTireTorque (int index, dFloat torque)
{
//torque=-600.0f;
	m_tires[index].m_torque = torque;
}

void CustomRayCastCar::SetTireBrake (int index, dFloat torque)
{
	m_tires[index].m_breakTorque = torque;
}


void CustomRayCastCar::AddSingleSuspensionTire (
	void *userData,
	const dVector& localPosition, 
	dFloat mass,
	dFloat radius, 
	dFloat width,
	dFloat suspensionLength,
	dFloat springConst,
	dFloat springDamper,
	int castMode)
{
	// calculate the tire local base pose matrix
	dMatrix bodyMatrix;
	m_tires[m_tiresCount].m_contactPoint = dVector (0.0f, 0.0f, 0.0f, 1.0f);
	m_tires[m_tiresCount].m_tireAxelPosit = dVector (0.0f, 0.0f, 0.0f, 1.0f);
	m_tires[m_tiresCount].m_localAxelPosit = dVector (0.0f, 0.0f, 0.0f, 1.0f);
	m_tires[m_tiresCount].m_tireAxelVeloc = dVector (0.0f, 0.0f, 0.0f, 1.0f);
	m_tires[m_tiresCount].m_torque = 0.0f;
	m_tires[m_tiresCount].m_turnforce = 0.0f;
	m_tires[m_tiresCount].m_harpoint = m_localFrame.UntransformVector(localPosition);              
	m_tires[m_tiresCount].m_localAxis = m_localFrame.UnrotateVector(dVector (0.0f, 0.0f, 1.0f, 0.0f));
	m_tires[m_tiresCount].m_localAxis.m_w = 0.0f;
	m_tires[m_tiresCount].m_userData = userData;
	m_tires[m_tiresCount].m_angularVelocity = 0.0f;
	m_tires[m_tiresCount].m_spinAngle = 0.0f;
	m_tires[m_tiresCount].m_steerAngle = 0.0f;
	
	m_tires[m_tiresCount].m_posit = suspensionLength;
	m_tires[m_tiresCount].m_suspensionLength = suspensionLength;
	m_tires[m_tiresCount].m_tireLoad = 0.0f;
	m_tires[m_tiresCount].m_breakTorque = 0.0f;
	m_tires[m_tiresCount].m_localSuspentionSpeed = 0.0f;
	
	m_tires[m_tiresCount].m_springConst = springConst;
	m_tires[m_tiresCount].m_springDamper = springDamper;
	m_tires[m_tiresCount].m_groundFriction = 1.0f;

	m_tires[m_tiresCount].m_tireUseConvexCastMode = castMode; 
//	m_tires[m_tiresCount].m_tireJacobianRowIndex = -1;

	// make a convex shape to represent the tire collision
	#define TIRE_SHAPE_SIZE 12
	dVector shapePoints[TIRE_SHAPE_SIZE * 2];
	for (int i = 0; i < TIRE_SHAPE_SIZE; i ++) {
		shapePoints[i].m_x = -width * 0.5f;	
		shapePoints[i].m_y = radius * dCos (2.0f * 3.1416 * dFloat(i)/ dFloat(TIRE_SHAPE_SIZE));
		shapePoints[i].m_z = radius * dSin (2.0f * 3.1416 * dFloat(i)/ dFloat(TIRE_SHAPE_SIZE));
		shapePoints[i + TIRE_SHAPE_SIZE].m_x = -shapePoints[i].m_x;
		shapePoints[i + TIRE_SHAPE_SIZE].m_y = shapePoints[i].m_y;
		shapePoints[i + TIRE_SHAPE_SIZE].m_z = shapePoints[i].m_z;
	}
	m_tires[m_tiresCount].m_shape = NewtonCreateConvexHull (m_world, TIRE_SHAPE_SIZE * 2, &shapePoints[0].m_x, sizeof (dVector), 0.0f, NULL);

	// calculate the tire geometrical parameters
	m_tires[m_tiresCount].m_radius = radius;
//	m_tires[m_tiresCount].m_radiusInv  = 1.0f / m_tires[m_tiresCount].m_radius;
	m_tires[m_tiresCount].m_mass = mass;	
	m_tires[m_tiresCount].m_massInv = 1.0f / m_tires[m_tiresCount].m_mass;	
	m_tires[m_tiresCount].m_Ixx = mass * radius * radius / 2.0f;
	m_tires[m_tiresCount].m_IxxInv = 1.0f / m_tires[m_tiresCount].m_Ixx;
	SetTireMaxRPS (m_tiresCount, 150.0f / radius);

	m_tiresCount ++;
}


const dMatrix& CustomRayCastCar::GetChassisMatrixLocal () const
{
	return m_localFrame;
}

dMatrix CustomRayCastCar::CalculateSuspensionMatrix (int tireIndex, dFloat distance) const
{
	const Tire& tire = m_tires[tireIndex];

	dMatrix matrix;
	// calculate the steering angle matrix for the axis of rotation
	matrix.m_front = tire.m_localAxis;
	matrix.m_up    = dVector (0.0f, 1.0f, 0.0f, 0.0f);
	matrix.m_right = dVector (-tire.m_localAxis.m_z, 0.0f, tire.m_localAxis.m_x, 0.0f);
	matrix.m_posit = tire.m_harpoint - m_localFrame.m_up.Scale (distance);
	return matrix;
}

dMatrix CustomRayCastCar::CalculateTireMatrix (int tireIndex) const
{
	const Tire& tire = m_tires[tireIndex];

	// calculate the rotation angle matrix
	dMatrix angleMatrix (dPitchMatrix(tire.m_spinAngle));

	// get the tire body matrix
	dMatrix bodyMatrix;
	NewtonBodyGetMatrix(m_body0, &bodyMatrix[0][0]);
	return angleMatrix * CalculateSuspensionMatrix (tireIndex, tire.m_posit) * m_localFrame * bodyMatrix;

}


unsigned CustomRayCastCar::ConvexCastPrefilter(const NewtonBody* body, const NewtonCollision* collision, void* userData)
{
	NewtonBody* me;
	me = (NewtonBody*) userData;
	// do no cast myself
	return (me != body);
}


void CustomRayCastCar::CalculateTireCollision (Tire& tire, const dMatrix& suspensionMatrixInGlobalSpace) const
{
  // make a data structure to collect the information returned by the ray cast
  struct RayCastInfo
  {
    RayCastInfo(const NewtonBody* body)
	{
	  m_param = 1.0f;
	  m_me = body;
	  m_hitBody = NULL;
	}
	static dFloat RayCast (const NewtonBody* body, const dFloat* normal, int collisionID, void* userData, dFloat intersetParam)
	{
	  RayCastInfo& caster = *((RayCastInfo*) userData); 
	  // if this body is not the vehicle, see if a close hit
	  if (body != caster.m_me) {
	    if (intersetParam < caster.m_param) {
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
  // extend the ray by the radius of the tire
  dFloat dist (tire.m_suspensionLength + tire.m_radius);
  dVector destination (suspensionMatrixInGlobalSpace.TransformVector(m_localFrame.m_up.Scale (-dist)));	
  // cast a ray to the world ConvexCastPrefilter
  NewtonWorldRayCast(m_world, &suspensionMatrixInGlobalSpace.m_posit[0], &destination[0], RayCastInfo::RayCast, &info, &ConvexCastPrefilter);
  // if the ray hit something, it means the tire has some traction
  if (info.m_hitBody) {
    dFloat intesectionDist;

  tire.m_contactPoint = suspensionMatrixInGlobalSpace.m_posit + (destination - suspensionMatrixInGlobalSpace.m_posit).Scale (info.m_param); 
  tire.m_contactNormal = info.m_normal;
  // TO DO: get the material properties for tire frictions on different roads 

  intesectionDist = dist * info.m_param - tire.m_radius;
  if (intesectionDist > tire.m_suspensionLength) {
    intesectionDist = tire.m_suspensionLength;
  } else if (intesectionDist < dFloat (0.0f)) {
    intesectionDist = dFloat (0.0f);
  }
  tire.m_posit = intesectionDist;
  switch (info.m_contactID)
  {
    case 0:
	{
	  // normal ground friction
	  tire.m_groundFriction = 1.0f;
	  break;
	}
	default:
	{
	  // default ground friction
	  tire.m_groundFriction = 1.0f;
	  break;
	}
   }
  } else {
    tire.m_posit = tire.m_suspensionLength;
	tire.m_groundFriction = 0.0f;
  }
}

void CustomRayCastCar::SubmitConstrainst(dFloat timestep, int threadIndex)
{
	dFloat invTimestep;
	dMatrix bodyMatrix;
	dMatrix suspensionMatrices[VEHICLE_MAX_TIRE_COUNT];  

	// get the simulation time
	invTimestep = 1.0f / timestep ;

	// get the vehicle global matrix, and use it in several calculations
	NewtonBodyGetMatrix(m_body0, &bodyMatrix[0][0]);
	dMatrix chassisMatrix (m_localFrame * bodyMatrix);  

	// calculate all suspension matrices in global space and tire collision
	for (int i = 0; i < m_tiresCount; i ++) {
		Tire& tire = m_tires[i];
		dMatrix suspensionMatrix = suspensionMatrices[i];

		// calculate this suspension matrix and save it for future used
		suspensionMatrix = CalculateSuspensionMatrix (i, 0.0f) * chassisMatrix;

		// calculate the tire collision
		CalculateTireCollision (tire, suspensionMatrix);
	}


	// calculate all suspension forces due to spring and damper
	dVector m_chassisForce (0.0f, 0.0f, 0.0f, 0.0f);
	dVector m_chassisTorque (0.0f, 0.0f, 0.0f, 0.0f);

	// get the chassis instantaneous linear and angular velocity in the local space of the chassis
	int longitidunalForceIndex;
	longitidunalForceIndex = 0;
	NewtonBodyGetVelocity(m_body0, &m_chassisVelocity[0]);
	NewtonBodyGetOmega(m_body0, &m_chassisOmega[0]);
	for (int i = 0; i < m_tiresCount; i ++) {
		Tire& tire = m_tires[i];
		// calculate the linear velocity of the tire at the ground contact
		tire.m_tireAxelPosit = (chassisMatrix.TransformVector(tire.m_harpoint - m_localFrame.m_up.Scale (tire.m_posit)));
	    tire.m_localAxelPosit = (tire.m_tireAxelPosit - chassisMatrix.m_posit);
		tire.m_tireAxelVeloc = (m_chassisVelocity + m_chassisOmega * tire.m_localAxelPosit); 


	    dVector lateralPin (chassisMatrix.RotateVector (tire.m_localAxis));
	    dVector longitudinalPin (chassisMatrix.m_up * lateralPin);
	    tire.m_longitudinalDir = longitudinalPin;
	    tire.m_lateralDir = lateralPin; 
		if (tire.m_posit < tire.m_suspensionLength)  {
			dFloat speed;
            // TO DO: need to calculate the velocity if the other body at the point

			// for now assume the ground is a static body
			dVector hitBodyVeloc (0, 0, 0, 0);

			// calculate the relative velocity
			dVector relVeloc (tire.m_tireAxelVeloc - hitBodyVeloc);
			speed = -(relVeloc % chassisMatrix.m_up);

			// now calculate the tire load at the contact point
			tire.m_tireLoad = - NewtonCalculateSpringDamperAcceleration (timestep, tire.m_springConst, tire.m_suspensionLength - tire.m_posit, tire.m_springDamper, speed)*0.5f;

			if (tire.m_tireLoad < 0.0f) {
				// since the tire is not a body with real mass it can only push the chassis.
				tire.m_tireLoad = 0.0f;
			} else {
				//this suspension is applying a normalize force to the car chassis, need to scales by the mass of the car
				tire.m_tireLoad *= m_mass;

				// apply the tire model to these wheel
			}

			// convert the tire load force magnitude to a torque and force.
			dVector tireForce (chassisMatrix.m_up.Scale (tire.m_tireLoad));

			// accumulate the force and torque form this suspension
			m_chassisForce = tireForce;

            m_chassisForce += chassisMatrix.m_front.Scale(tire.m_torque*-40);
			if (dAbs(m_curSpeed)!=0.0f) {
              m_chassisForce += chassisMatrix.m_right.Scale(tire.m_turnforce*60);
			}
			if (tire.m_groundFriction!=0) {
			  ApplyTractionAndSteer(m_chassisForce,tire.m_tireAxelPosit); 
			  ApplyTireFrictionModel(chassisMatrix, timestep);
			}
		} else {
			//tire is on the air  not force applied to the vehicle.
			tire.m_tireLoad = dFloat (0.0f);
//			tire.m_tireJacobianRowIndex = -1;
			dFloat torque;
			torque = tire.m_torque - tire.m_angularVelocity * tire.m_Ixx * 0.1f;
			tire.m_angularVelocity  += torque * tire.m_IxxInv * timestep;
		}
		ApplyTiresTorqueVisual(tire,timestep,threadIndex);
	}
	// set the current vehicle speed
	m_curSpeed = bodyMatrix[0] % m_chassisVelocity;
	if (m_curSpeed>0) { 
      m_tiresRollSide = 0; 
	} else {
      m_tiresRollSide = 1;
	}
}

// I get a problem with the visual.
// If the tire get so much presure from the side the visual can disappear.
void CustomRayCastCar::ApplyTiresTorqueVisual(Tire& tire, dFloat timestep, int threadIndex)
{
	dFloat timestepInv;
	// get the simulation time
	timestepInv = 1.0f / timestep;
    dVector tireRadius (tire.m_contactPoint - tire.m_tireAxelPosit);
    //check if any engine torque or brake torque is applied to the tire
	if (dAbs(tire.m_torque) < 1.0e-3f){
      //tire is coasting, calculate the tire zero slip angular velocity
	  // this is the velocity that satisfy the constraint equation
	  // V % dir + W * R % dir = 0
	  // where V is the tire Axel velocity
	  // W is the tire local angular velocity
	  // R is the tire radius
	  // dir is the longitudinal direction of of the tire.		
	  dFloat tireLinearSpeed;
	  dFloat tireContactSpeed;
	  tireLinearSpeed = tire.m_tireAxelVeloc % tire.m_longitudinalDir;
	  tireContactSpeed = (tire.m_lateralDir * tireRadius) % tire.m_longitudinalDir;
	  tire.m_angularVelocity = - tireLinearSpeed / tireContactSpeed;
	} else {
	  // tire is under some power, need to do the free body integration to apply the net torque
	  dFloat tireLinearSpeed;
	  dFloat tireContactSpeed;
	  tireLinearSpeed = tire.m_tireAxelVeloc % tire.m_longitudinalDir;
	  tireContactSpeed = (tire.m_lateralDir * tireRadius) % tire.m_longitudinalDir;
	  dFloat nettorque = - tireLinearSpeed / tireContactSpeed;
	  //tire.m_angularVelocity = - tireLinearSpeed / tireContactSpeed;

	  dFloat torque;
	  torque = tire.m_torque - nettorque - tire.m_angularVelocity * tire.m_Ixx * 0.1f;
	  tire.m_angularVelocity  += torque * tire.m_IxxInv * timestep;
	}
	// integrate tire angular velocity and rotation
	tire.m_spinAngle = dMod (tire.m_spinAngle + tire.m_angularVelocity * timestep, 3.1416f * 2.0f); 
	// reset tire torque to zero after integration; 
	tire.m_torque = 0.0f;
}


void CustomRayCastCar::ApplyTireFrictionModel(const dMatrix& chassisMatrix, dFloat timestep)
{
  ApplyVelocityCorrection(chassisMatrix);
  ApplyOmegaCorrection();

  NewtonBodySetVelocity(m_body0,&m_chassisVelocity[0]);
  NewtonBodySetOmega(m_body0,&m_chassisOmega[0]);
}

void CustomRayCastCar::ApplyOmegaCorrection()
{
  // can surely need a fix to make lose friction when the vehicle turn from steer for simulate rotational effect.
  // this is only a quick hack in waitting a better solution.
  m_chassisOmega = m_chassisOmega.Scale(0.975f);
}

// friction need a fix to make lose friction on the torque tires at pos tires.
// because currently the friction is only lose in directional mode by the hack.
void CustomRayCastCar::ApplyVelocityCorrection(const dMatrix& chassisMatrix)
{
  dFloat vl = 0;
  dVector vlo = m_chassisVelocity;
  dVector vDir = chassisMatrix.m_right;
  dVector vt = dVector(0.0f, 0.0f, 0.0f, 0.0f);
  //
  dFloat speed;
  // add some lose friction.
  // can surely need a fix to make lose friction when the vehicle turn from steer.
  // this is only a quick hack in waitting a better solution.
  speed = dAbs (GetSpeed())*0.03f;
  if (speed>0.295f) {
    speed = 0.295f;
  }
  vl = (vlo % vDir.Scale((0.3f-speed)));
  //
  vt.m_x = (vlo.m_x) - vl * vDir.m_x;
  vt.m_y = (vlo.m_y) - vl * vDir.m_y;
  vt.m_z = (vlo.m_z) - vl * vDir.m_z;

  m_chassisVelocity = vt; 
}

void CustomRayCastCar::ApplyTractionAndSteer(const dVector& vForce, const dVector& vPoint)
{
  dVector Torque;
  dMatrix M;
  dVector com;
  NewtonBodyGetCentreOfMass(m_body0,&com[0]);
  NewtonBodyGetMatrix(m_body0,&M[0][0]);
  Torque = ( vPoint - M.TransformVector( dVector(com.m_x, com.m_y, com.m_z, 1.0f ))) * vForce;
  NewtonBodyAddForce(m_body0,&vForce[0]);
  NewtonBodyAddTorque(m_body0,&Torque[0]);
}







#else

CustomRayCastCar::CustomRayCastCar(int maxTireCount, const dMatrix& cordenateSytem, NewtonBody* carBody, const dVector& gravity)
	:NewtonCustomJoint(3 * maxTireCount, carBody, NULL), m_gravity (gravity)
{
	dVector com;
	dMatrix tmp;

	// set the chassis matrix at the center of mass
	NewtonBodyGetCentreOfMass(m_body0, &com[0]);
	com.m_w = 1.0f;

	// set the joint reference point at the center of mass of the body
	dMatrix chassisMatrix (cordenateSytem);
	chassisMatrix.m_posit += chassisMatrix.RotateVector(com);

	CalculateLocalMatrix (chassisMatrix, m_localFrame, tmp);

	// allocate space for the tires;
	m_tiresCount = 0;
	m_tires = new Tire[maxTireCount];

	m_curSpeed = 0.0f;
	m_aerodynamicDrag = 0.1f; 
	m_aerodynamicDownForce = 0.1f; 

	dFloat Ixx; 
	dFloat Iyy; 
	dFloat Izz; 
	NewtonBodyGetMassMatrix(m_body0, &m_mass, &Ixx, &Iyy, &Izz);

	// register the callback for tire integration
//	NewtonUserJointSetFeedbackCollectorCallback (m_joint, IntegrateTires);
}


CustomRayCastCar::~CustomRayCastCar()
{
	NewtonWorld *world;

	world = NewtonBodyGetWorld (m_body0);
	for (int i = 0; i < m_tiresCount; i ++) {
		NewtonReleaseCollision (world, m_tires[i].m_shape);
	}

	if(m_tires) {
		delete[] m_tires;
	}
}


int CustomRayCastCar::GetTiresCount() const
{
	return m_tiresCount;
}




void CustomRayCastCar::GetInfo (NewtonJointRecord* info) const
{
}

//this function is to be overloaded by a derive class
void CustomRayCastCar::SetSteering (dFloat angle)
{
}

//this function is to be overloaded by a derive class
void CustomRayCastCar::SetBrake (dFloat torque)
{
}

//this function is to be overloaded by a derive class
void CustomRayCastCar::SetTorque (dFloat torque)
{
}

dFloat CustomRayCastCar::GetSpeed() const
{
	return m_curSpeed;
}


void CustomRayCastCar::SetTireMaxRPS (int tireIndex, dFloat maxTireRPS)
{
	m_tires[tireIndex].m_maxTireRPS = maxTireRPS;
}

CustomRayCastCar::Tire& CustomRayCastCar::GetTire (int index) const
{
	return m_tires[index];
}

dFloat CustomRayCastCar::GetParametricPosition (int index) const
{
	return m_tires[index].m_posit / m_tires[index].m_suspensionLength;
}

void CustomRayCastCar::SetTireSteerAngle (int index, dFloat angle)
{
	m_tires[index].m_steerAngle = angle;
	m_tires[index].m_localAxis.m_z = dCos (angle);
	m_tires[index].m_localAxis.m_x = dSin (angle);
}

void CustomRayCastCar::SetTireTorque (int index, dFloat torque)
{
//torque=-600.0f;
	m_tires[index].m_torque = torque;
}

void CustomRayCastCar::SetTireBrake (int index, dFloat torque)
{
	m_tires[index].m_breakTorque = torque;
}


void CustomRayCastCar::AddSingleSuspensionTire (
	void *userData,
	const dVector& localPosition, 
	dFloat mass,
	dFloat radius, 
	dFloat width,
	dFloat suspensionLength,
	dFloat springConst,
	dFloat springDamper,
	int castMode)
{
	// calculate the tire local base pose matrix
	dMatrix bodyMatrix;
	m_tires[m_tiresCount].m_torque = 0.0f;
	m_tires[m_tiresCount].m_harpoint = m_localFrame.UntransformVector(localPosition);              
	m_tires[m_tiresCount].m_localAxis = m_localFrame.UnrotateVector(dVector (0.0f, 0.0f, 1.0f, 0.0f));
	m_tires[m_tiresCount].m_localAxis.m_w = 0.0f;
	m_tires[m_tiresCount].m_userData = userData;
	m_tires[m_tiresCount].m_angularVelocity = 0.0f;
	m_tires[m_tiresCount].m_spinAngle = 0.0f;
	m_tires[m_tiresCount].m_steerAngle = 0.0f;
	
	m_tires[m_tiresCount].m_posit = suspensionLength;
	m_tires[m_tiresCount].m_suspensionLength = suspensionLength;
	m_tires[m_tiresCount].m_tireLoad = 0.0f;
	m_tires[m_tiresCount].m_breakTorque = 0.0f;
	m_tires[m_tiresCount].m_localSuspentionSpeed = 0.0f;
	
	m_tires[m_tiresCount].m_springConst = springConst;
	m_tires[m_tiresCount].m_springDamper = springDamper;
	m_tires[m_tiresCount].m_groundFriction = 1.0f;

	m_tires[m_tiresCount].m_tireUseConvexCastMode = castMode; 
//	m_tires[m_tiresCount].m_tireJacobianRowIndex = -1;

	// make a convex shape to represent the tire collision
	#define TIRE_SHAPE_SIZE 12
	dVector shapePoints[TIRE_SHAPE_SIZE * 2];
	for (int i = 0; i < TIRE_SHAPE_SIZE; i ++) {
		shapePoints[i].m_x = -width * 0.5f;	
		shapePoints[i].m_y = radius * dCos (2.0f * 3.141592 * dFloat(i)/ dFloat(TIRE_SHAPE_SIZE));
		shapePoints[i].m_z = radius * dSin (2.0f * 3.141592 * dFloat(i)/ dFloat(TIRE_SHAPE_SIZE));
		shapePoints[i + TIRE_SHAPE_SIZE].m_x = -shapePoints[i].m_x;
		shapePoints[i + TIRE_SHAPE_SIZE].m_y = shapePoints[i].m_y;
		shapePoints[i + TIRE_SHAPE_SIZE].m_z = shapePoints[i].m_z;
	}
	m_tires[m_tiresCount].m_shape = NewtonCreateConvexHull (m_world, TIRE_SHAPE_SIZE * 2, &shapePoints[0].m_x, sizeof (dVector), 0.0f, NULL);

	// calculate the tire geometrical parameters
	m_tires[m_tiresCount].m_radius = radius;
//	m_tires[m_tiresCount].m_radiusInv  = 1.0f / m_tires[m_tiresCount].m_radius;
	m_tires[m_tiresCount].m_mass = mass;	
	m_tires[m_tiresCount].m_massInv = 1.0f / m_tires[m_tiresCount].m_mass;	
	m_tires[m_tiresCount].m_Ixx = mass * radius * radius / 2.0f;
	m_tires[m_tiresCount].m_IxxInv = 1.0f / m_tires[m_tiresCount].m_Ixx;
	SetTireMaxRPS (m_tiresCount, 150.0f / radius);

	m_tiresCount ++;
}


const dMatrix& CustomRayCastCar::GetChassisMatrixLocal () const
{
	return m_localFrame;
}

dMatrix CustomRayCastCar::CalculateSuspensionMatrix (int tireIndex, dFloat distance) const
{
	const Tire& tire = m_tires[tireIndex];

	dMatrix matrix;
	// calculate the steering angle matrix for the axis of rotation
	matrix.m_front = tire.m_localAxis;
	matrix.m_up    = dVector (0.0f, 1.0f, 0.0f, 0.0f);
	matrix.m_right = dVector (-tire.m_localAxis.m_z, 0.0f, tire.m_localAxis.m_x, 0.0f);
	matrix.m_posit = tire.m_harpoint - m_localFrame.m_up.Scale (distance);
	return matrix;
}

dMatrix CustomRayCastCar::CalculateTireMatrix (int tireIndex) const
{
	const Tire& tire = m_tires[tireIndex];

	// calculate the rotation angle matrix
	dMatrix angleMatrix (dPitchMatrix(tire.m_spinAngle));

	// get the tire body matrix
	dMatrix bodyMatrix;
	NewtonBodyGetMatrix(m_body0, &bodyMatrix[0][0]);
	return angleMatrix * CalculateSuspensionMatrix (tireIndex, tire.m_posit) * m_localFrame * bodyMatrix;
}


unsigned CustomRayCastCar::ConvexCastPrefilter(const NewtonBody* body, const NewtonCollision* collision, void* userData)
{
	NewtonBody* me;

	// for now just collide with static data.m_bodies
	dFloat mass; 
	dFloat Ixx; 
	dFloat Iyy; 
	dFloat Izz; 
	NewtonBodyGetMassMatrix(body, &mass, &Ixx, &Iyy, &Izz);
	if (mass > 0.0f) {
		return 0;
	}


	me = (NewtonBody*) userData;
	// do no cast myself
	return (me != body);
}


void CustomRayCastCar::CalculateTireCollision (Tire& tire, const dMatrix& suspensionMatrixInGlobalSpace) const
{
	if (tire.m_tireUseConvexCastMode) { 
		NewtonWorldConvexCastReturnInfo info;
		dVector destination (suspensionMatrixInGlobalSpace.TransformVector(m_localFrame.m_up.Scale (-tire.m_suspensionLength)));

		// cast the convex shape along the suspension length
		dFloat hitParam;
		if (NewtonWorldConvexCast (m_world, &suspensionMatrixInGlobalSpace[0][0], &destination[0], tire.m_shape, &hitParam, (void*)m_body0, ConvexCastPrefilter, &info, 1)) {
			// the ray hit something
			tire.m_posit = info.m_intersectionParam * tire.m_suspensionLength;
			tire.m_contactPoint = info.m_point;
			tire.m_contactNormal = info.m_normal;
			// TO DO: get the material properties for tire frictions on different roads 
			//dTrace (("%f\n", info.m_intersectionParam));

			switch (info.m_contactID)
			{
				case 0:
				{
					// normal ground friction
					tire.m_groundFriction = 1.0f;
					break;
				}

				default:
				{
					// default ground friction
					tire.m_groundFriction = 1.0f;
					break;
				}
			}

		} else {
			// if the ray do not hit anything the tire in airborne
			tire.m_posit = tire.m_suspensionLength;
			tire.m_groundFriction = 0.0f;
		}

	} else {
		// make a data structure to collect the information returned by the ray cast
		struct RayCastInfo
		{
			RayCastInfo(const NewtonBody* body)
			{
				m_param = 1.0f;
				m_me = body;
				m_hitBody = NULL;
			}


			static dFloat RayCast (const NewtonBody* body, const dFloat* normal, int collisionID, void* userData, dFloat intersetParam)
			{
				RayCastInfo& caster = *((RayCastInfo*) userData); 

				// if this body is not the vehicle, see if a close hit
				if (body != caster.m_me) {
					if (intersetParam < caster.m_param) {
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

		// extend the ray by the radius of the tire
		dFloat dist (tire.m_suspensionLength + tire.m_radius);
		dVector destination (suspensionMatrixInGlobalSpace.TransformVector(m_localFrame.m_up.Scale (-dist)));	

		// cast a ray to the world
		NewtonWorldRayCast(m_world, &suspensionMatrixInGlobalSpace.m_posit[0], &destination[0], RayCastInfo::RayCast, &info, ConvexCastPrefilter);

		// if the ray hit something, it means the tire has some traction
		if (info.m_hitBody) {
			dFloat intesectionDist;


			tire.m_contactPoint = suspensionMatrixInGlobalSpace.m_posit + (destination - suspensionMatrixInGlobalSpace.m_posit).Scale (info.m_param); 
			tire.m_contactNormal = info.m_normal;
			// TO DO: get the material properties for tire frictions on different roads 

			intesectionDist = dist * info.m_param - tire.m_radius;
			if (intesectionDist > tire.m_suspensionLength) {
				intesectionDist = tire.m_suspensionLength;
			} else if (intesectionDist < dFloat (0.0f)) {
				intesectionDist = dFloat (0.0f);
			}
			tire.m_posit = intesectionDist;
			switch (info.m_contactID)
			{
				case 0:
				{
					// normal ground friction
					tire.m_groundFriction = 1.0f;
					break;
				}

				default:
				{
					// default ground friction
					tire.m_groundFriction = 1.0f;
					break;
				}
			}

		} else {
			tire.m_posit = tire.m_suspensionLength;
			tire.m_groundFriction = 0.0f;
		}
	}
}

#if 0

void CustomRayCastCar::SubmitConstrainst(dFloat timestep, int threadIndex)
{
//	dFloat invTimestep;
	dMatrix bodyMatrix;
	dMatrix suspensionMatrices[VEHICLE_MAX_TIRE_COUNT];  

	// get the simulation time
//	invTimestep = 1.0f / timestep ;

	// get the vehicle global matrix, and use it in several calculations
	NewtonBodyGetMatrix(m_body0, &bodyMatrix[0][0]);
	dMatrix chassisMatrix (m_localFrame * bodyMatrix);  

	// calculate all suspension matrices in global space and tire collision
	for (int i = 0; i < m_tiresCount; i ++) {
		Tire& tire = m_tires[i];
		dMatrix suspensionMatrix = suspensionMatrices[i];

		// calculate this suspension matrix and save it for future used
		suspensionMatrix = CalculateSuspensionMatrix (i, 0.0f) * chassisMatrix;

		// calculate the tire collision
		CalculateTireCollision (tire, suspensionMatrix);
	}

	// force and torque accumulators

	// calculate and accumulate the external forces
	ApplySuspensionForces (chassisMatrix, timestep);

	// calculate and accumulate the internals forces
	ApplyTireForces (chassisMatrix, timestep);

	// set the current vehicle speed
//	m_curSpeed = bodyMatrix[0] % globalVeloc;
}



#define SOLVER_ERROR_2					 (1.0e-2f) 
#define TIRE_FRICTION					 (1.2f)
//#define TIRE_FRICTION					 (1.0f)

#define AXEL_RELAXATION					(1.0e-3f)
#define CONTACT_PATCH_RELAXATION		(1.0e-2f)

#define VEHICLE_MAX_DOF					 (VEHICLE_MAX_TIRE_COUNT * 8)


class RayCastCarPhysicsData
{
	public:

	class BodyProxy
	{
		public:
		dMatrix m_invInertia;
		dVector m_force;
		dVector m_torque;
		dVector m_com;
		dFloat m_invMass;
		dFloat m_padd[3];
	};

	class Jacobian
	{
		public:
		dVector m_linear;
		dVector m_angular;
	};

	class JacobianPair
	{
		public:
		Jacobian m_jacobian_IM0;
		Jacobian m_jacobian_IM1;
	};

	BodyProxy m_bodies[VEHICLE_MAX_TIRE_COUNT + 2];
	Jacobian m_y[VEHICLE_MAX_DOF];
	JacobianPair m_Jt[VEHICLE_MAX_DOF];
	JacobianPair m_JinvMass[VEHICLE_MAX_DOF];

	dFloat m_accel[VEHICLE_MAX_DOF];
	dFloat m_force[VEHICLE_MAX_DOF];
	dFloat m_deltaForce[VEHICLE_MAX_DOF];
	dFloat m_deltaAccel[VEHICLE_MAX_DOF];

	dFloat m_diagDamp[VEHICLE_MAX_DOF];
	dFloat m_activeRow[VEHICLE_MAX_DOF];
	dFloat m_invDJMinvJt[VEHICLE_MAX_DOF];
	dFloat m_relaxation[VEHICLE_MAX_DOF];
	dFloat m_minForceLimit[VEHICLE_MAX_DOF];
	dFloat m_maxForceLimit[VEHICLE_MAX_DOF];

	int m_body0[VEHICLE_MAX_DOF];
	int m_body1[VEHICLE_MAX_DOF];
};

void CustomRayCastCar::ApplySuspensionForces (const dMatrix& chassisMatrix, dFloat timestep) const
{
	dVector globalVeloc;
	dVector globalOmega;
	dVector torque (0.0f, 0.0f, 0.0f, 0.0f);
	dVector force (0.0f, 0.0f, 0.0f, 0.0f);
	
	// get the chassis instantaneous linear and angular velocity in the local space of the chassis
	NewtonBodyGetOmega(m_body0, &globalOmega[0]);
	NewtonBodyGetVelocity(m_body0, &globalVeloc[0]);

	// add aerodynamic forces
	dVector verticalVeloc (chassisMatrix.m_up.Scale (chassisMatrix.m_up % globalVeloc));
	dVector horizontalVeloc (globalVeloc - verticalVeloc);

	force -= horizontalVeloc.Scale (m_aerodynamicDrag * m_mass);
	force -= chassisMatrix.m_up.Scale (m_aerodynamicDownForce * m_mass * dSqrt (horizontalVeloc % horizontalVeloc));

	// calculate all suspension forces due to spring and damper
	for (int i = 0; i < m_tiresCount; i ++) {
		
		Tire& tire = m_tires[i];

		tire.m_tireLoad = dFloat (0.0f);
		if (tire.m_posit < tire.m_suspensionLength) {

			// calculate the linear velocity of the tire at the ground contact
			dVector tireAxelPosit (chassisMatrix.TransformVector(tire.m_harpoint - m_localFrame.m_up.Scale (tire.m_posit)));
			dVector localAxelPosit (tireAxelPosit - chassisMatrix.m_posit);
			dVector tireAxelVeloc (globalVeloc + globalOmega * localAxelPosit); 		

			// TO DO: need to calculate the velocity if the other body at the point
			// for now assume the ground is a static body
			dVector hitBodyVeloc (0, 0, 0, 0);

			// calculate the relative velocity
			dVector relVeloc (tireAxelVeloc - hitBodyVeloc);
			tire.m_localSuspentionSpeed = -(relVeloc % chassisMatrix.m_up);

			// now calculate the tire load at the contact patch
			tire.m_tireLoad = - NewtonCalculateSpringDamperAcceleration (timestep, tire.m_springConst, tire.m_suspensionLength - tire.m_posit, tire.m_springDamper, tire.m_localSuspentionSpeed)  * m_mass;
			if (tire.m_tireLoad < 0.0f) {
				tire.m_tireLoad = 0.0f;
			}

			// convert the tire load force magnitude to a torque and force.
			dVector tireForce (chassisMatrix.m_up.Scale (tire.m_tireLoad));

			// accumulate the force and torque form this suspension
			force += tireForce;
			torque += localAxelPosit * tireForce;
		}
	}

	// apply the suspention force to body
	NewtonBodyAddForce(m_body0, &force[0]);
	NewtonBodyAddTorque(m_body0, &torque[0]);
}



void CustomRayCastCar::ApplyTireForces (const dMatrix& chassisMatrix, dFloat timestep) const
{
	int rows;
	int activeTiresRows;
	dFloat invTimestep;
	dVector bodyVeloc;
	dVector bodyOmega;
	RayCastCarPhysicsData data;

	// initialize workld bod physics data
	data.m_bodies[0].m_force = dVector (0.0f, 0.0f, 0.0f, 0.0f); 
	data.m_bodies[0].m_torque = dVector (0.0f, 0.0f, 0.0f, 0.0f); 
	data.m_bodies[0].m_invInertia = GetZeroMatrix();
	data.m_bodies[0].m_invMass = 0.0f;

	// initialize chassis physics data
	dMatrix tmpMatrix (GetIdentityMatrix());;
	NewtonBodyGetForceAcc (m_body0, &data.m_bodies[1].m_force.m_x);
	NewtonBodyGetTorqueAcc (m_body0, &data.m_bodies[1].m_torque.m_x);
	NewtonBodyGetInvMass(GetBody0(), &data.m_bodies[1].m_invMass, &tmpMatrix[0][0], &tmpMatrix[1][1], &tmpMatrix[2][2]);
	data.m_bodies[1].m_invInertia = chassisMatrix.Transpose() * tmpMatrix;


	NewtonBodyGetOmega(m_body0, &bodyOmega.m_x);
	NewtonBodyGetVelocity(m_body0, &bodyVeloc.m_x);

	// add Jacobian between chassis and tires
	rows = 0;
	for (int i = 0; i < m_tiresCount; i ++) {
		Tire& tire = m_tires[i];

		// calculate the tire pin directions 
		dVector rightDir (chassisMatrix.RotateVector (tire.m_localAxis));
		dVector frontDir (chassisMatrix.m_up * rightDir);
		tire.m_longitudinalDir = frontDir;
		tire.m_lateralDir = rightDir; 

		// save tire dynamic data			
		data.m_bodies[i + 2].m_com = chassisMatrix.TransformVector (tire.m_harpoint - m_localFrame.m_up.Scale (tire.m_posit));
		data.m_bodies[i + 2].m_invMass = tire.m_massInv;
		data.m_bodies[i + 2].m_force = m_gravity.Scale(tire.m_mass);
		data.m_bodies[i + 2].m_torque = tire.m_lateralDir.Scale (tire.m_torque);
		data.m_bodies[i + 2].m_invInertia = GetIdentityMatrix();
		data.m_bodies[i + 2].m_invInertia[0][0] *= tire.m_IxxInv;
		data.m_bodies[i + 2].m_invInertia[1][1] *= tire.m_IxxInv;
		data.m_bodies[i + 2].m_invInertia[2][2] *= tire.m_IxxInv;  // assume for now spherical inerrtial fo tires

		// set the tire longitudinal jacobian
		{
			data.m_body0[rows] = 1;
			data.m_body1[rows] = i + 2;

			RayCastCarPhysicsData::Jacobian &jacobian0 = data.m_Jt[rows].m_jacobian_IM0; 
			jacobian0.m_linear = frontDir;
			jacobian0.m_angular = (data.m_bodies[i + 2].m_com - chassisMatrix.m_posit) * jacobian0.m_linear;

			RayCastCarPhysicsData::Jacobian &jacobian1 = data.m_Jt[rows].m_jacobian_IM1; 
			jacobian1.m_linear = frontDir.Scale (-1.0f);
			jacobian1.m_angular = dVector (0.0f, 0.0f, 0.0f, 0.0f);

			data.m_accel[rows] = 0.0f;
			data.m_relaxation[rows] = AXEL_RELAXATION;
			data.m_minForceLimit[rows] = -1.0e10f;
			data.m_maxForceLimit[rows] =  1.0e10f;
			rows ++;
		}

		// set the tire lateral jacobian
		{
			data.m_body0[rows] = 1;
			data.m_body1[rows] = i + 2;

			RayCastCarPhysicsData::Jacobian &jacobian0 = data.m_Jt[rows].m_jacobian_IM0; 
			jacobian0.m_linear = rightDir;
			jacobian0.m_angular = (data.m_bodies[i + 2].m_com - chassisMatrix.m_posit) * jacobian0.m_linear;

			RayCastCarPhysicsData::Jacobian &jacobian1 = data.m_Jt[rows].m_jacobian_IM1; 
			jacobian1.m_linear = rightDir.Scale (-1.0f);
			jacobian1.m_angular = dVector (0.0f, 0.0f, 0.0f, 0.0f);

			data.m_accel[rows] = 0.0f;
			data.m_relaxation[rows] = AXEL_RELAXATION;
			data.m_minForceLimit[rows] = -1.0e10f;
			data.m_maxForceLimit[rows] =  1.0e10f;
			rows ++;
		}


		// longitudinal twist tire jacobian
		{
			data.m_body0[rows] = 1;
			data.m_body1[rows] = i + 2;

			RayCastCarPhysicsData::Jacobian &jacobian0 = data.m_Jt[rows].m_jacobian_IM0; 
			jacobian0.m_linear = dVector (0.0f, 0.0f, 0.0f, 0.0f);
			jacobian0.m_angular = frontDir;

			RayCastCarPhysicsData::Jacobian &jacobian1 = data.m_Jt[rows].m_jacobian_IM1; 
			jacobian1.m_linear = dVector (0.0f, 0.0f, 0.0f, 0.0f);
			jacobian1.m_angular = frontDir.Scale (-1.0f);

			data.m_accel[rows] = 0.0f;
			data.m_relaxation[rows] = AXEL_RELAXATION;
			data.m_minForceLimit[rows] = -1.0e10f;
			data.m_maxForceLimit[rows] =  1.0e10f;
			rows ++;
		}

		// Lateral twist tire jacobian
		{
			data.m_body0[rows] = 1;
			data.m_body1[rows] = i + 2;

			RayCastCarPhysicsData::Jacobian &jacobian0 = data.m_Jt[rows].m_jacobian_IM0; 
			jacobian0.m_linear = dVector (0.0f, 0.0f, 0.0f, 0.0f);
			jacobian0.m_angular = chassisMatrix.m_up;

			RayCastCarPhysicsData::Jacobian &jacobian1 = data.m_Jt[rows].m_jacobian_IM1; 
			jacobian1.m_linear = dVector (0.0f, 0.0f, 0.0f, 0.0f);
			jacobian1.m_angular = chassisMatrix.m_up.Scale (-1.0f);

			data.m_accel[rows] = 0.0f;
			data.m_relaxation[rows] = AXEL_RELAXATION;
			data.m_minForceLimit[rows] = -1.0e10f;
			data.m_maxForceLimit[rows] =  1.0e10f;
			rows ++;
		}
		_ASSERTE (rows < VEHICLE_MAX_DOF);
	}


	// add Jacobian between body tires and floor or other data.m_bodies
	activeTiresRows = rows;
	invTimestep = 1.0f / timestep;
	for (int i = 0; i < m_tiresCount; i ++) {
		Tire& tire = m_tires[i];

		dVector tireOmega (bodyOmega + tire.m_lateralDir.Scale (tire.m_angularVelocity));
		dVector tireVeloc (bodyVeloc + bodyOmega * (data.m_bodies[i + 2].m_com - chassisMatrix.m_posit));
		dVector tireContactVeloc (tireVeloc + tireOmega * (tire.m_contactPoint - data.m_bodies[i + 2].m_com));

		if (tire.m_tireLoad > 0.0f) {

			dVector surfaceVeloc (0.0f, 0.0f, 0.0f, 0.0f);

			dVector relativeVeloc (surfaceVeloc - tireContactVeloc);

			// calculate the tire longitudinal forces derivatives
			{
				data.m_body0[rows] = 0;
				data.m_body1[rows] = i + 2;

				RayCastCarPhysicsData::Jacobian &jacobian0 = data.m_Jt[rows].m_jacobian_IM0; 
				jacobian0.m_linear = dVector (0.0f, 0.0f, 0.0f, 0.0f);
				jacobian0.m_angular = dVector (0.0f, 0.0f, 0.0f, 0.0f);

				RayCastCarPhysicsData::Jacobian &jacobian1 = data.m_Jt[rows].m_jacobian_IM1; 
				jacobian1.m_linear = tire.m_longitudinalDir;
				jacobian1.m_angular = (tire.m_contactPoint - data.m_bodies[i + 2].m_com) * jacobian1.m_linear;

				data.m_relaxation[rows] = CONTACT_PATCH_RELAXATION;
				data.m_accel[rows] = (relativeVeloc % tire.m_longitudinalDir) * invTimestep;
				data.m_minForceLimit[rows] = - tire.m_tireLoad * TIRE_FRICTION;
				data.m_maxForceLimit[rows] =   tire.m_tireLoad * TIRE_FRICTION;
				rows ++;
			}

			// calculate the tire side forces derivatives
			{
				data.m_body0[rows] = 0;
				data.m_body1[rows] = i + 2;

				RayCastCarPhysicsData::Jacobian &jacobian0 = data.m_Jt[rows].m_jacobian_IM0; 
				jacobian0.m_linear = dVector (0.0f, 0.0f, 0.0f, 0.0f);
				jacobian0.m_angular = dVector (0.0f, 0.0f, 0.0f, 0.0f);

				RayCastCarPhysicsData::Jacobian &jacobian1 = data.m_Jt[rows].m_jacobian_IM1; 
				jacobian1.m_linear = tire.m_lateralDir;
				jacobian1.m_angular = (tire.m_contactPoint - data.m_bodies[i + 2].m_com) * jacobian1.m_linear;

				data.m_relaxation[rows] = CONTACT_PATCH_RELAXATION;
				data.m_accel[rows] = (relativeVeloc % tire.m_lateralDir) * invTimestep;
				data.m_minForceLimit[rows] = - tire.m_tireLoad * TIRE_FRICTION;
				data.m_maxForceLimit[rows] =   tire.m_tireLoad * TIRE_FRICTION;
				rows ++;
			}
		} 


		if ((tire.m_breakTorque > 0.0f) || (dAbs (tire.m_angularVelocity) > tire.m_maxTireRPS)) {

			if (tire.m_breakTorque > 0.0f) {
				data.m_accel[rows] = tire.m_angularVelocity * invTimestep;
				data.m_minForceLimit[rows] = -tire.m_breakTorque;
				data.m_maxForceLimit[rows] =  tire.m_breakTorque;
			} else {
				if (tire.m_angularVelocity > 0.0f) {
					data.m_accel[rows] = (tire.m_angularVelocity - tire.m_maxTireRPS) * invTimestep;
				} else {
					data.m_accel[rows] = (tire.m_angularVelocity + tire.m_maxTireRPS) * invTimestep;
				}
				data.m_minForceLimit[rows] = -1.0e10f;
				data.m_maxForceLimit[rows] =  1.0e10f;;
			}
				
		} else {
			dFloat val;
			data.m_accel[rows] = tire.m_angularVelocity * invTimestep * 0.3f;
			val = dAbs (tire.m_angularVelocity);
			data.m_minForceLimit[rows] = -10.0f * tire.m_Ixx;
			data.m_maxForceLimit[rows] =  10.0f * tire.m_Ixx;
		}
		data.m_body0[rows] = 1;
		data.m_body1[rows] = i + 2;
		RayCastCarPhysicsData::Jacobian &jacobian0 = data.m_Jt[rows].m_jacobian_IM0; 
		RayCastCarPhysicsData::Jacobian &jacobian1 = data.m_Jt[rows].m_jacobian_IM1; 
		jacobian0.m_linear = dVector (0.0f, 0.0f, 0.0f, 0.0f);
		jacobian0.m_angular = tire.m_lateralDir;
		jacobian1.m_linear = dVector (0.0f, 0.0f, 0.0f, 0.0f);
		jacobian1.m_angular = tire.m_lateralDir.Scale (-1.0f);
		data.m_relaxation[rows] = AXEL_RELAXATION;
		rows ++;
	}

	// calculate the relative accelerations
	for (int i = 0; i < rows; i ++) {
		int j0;
		int j1;
		dFloat accel;
		dFloat diagonal;

		j0 = data.m_body0[i];
		j1 = data.m_body1[i];

		RayCastCarPhysicsData::Jacobian &jacobian0 = data.m_Jt[i].m_jacobian_IM0; 
		RayCastCarPhysicsData::Jacobian &jacobian1 = data.m_Jt[i].m_jacobian_IM1; 
		RayCastCarPhysicsData::Jacobian &jacobianInvM0 = data.m_JinvMass[i].m_jacobian_IM0; 
		RayCastCarPhysicsData::Jacobian &jacobianInvM1 = data.m_JinvMass[i].m_jacobian_IM1; 

		jacobianInvM0.m_linear = jacobian0.m_linear.Scale (data.m_bodies[j0].m_invMass);
		jacobianInvM0.m_angular = data.m_bodies[j0].m_invInertia.UnrotateVector (jacobian0.m_angular);
		jacobianInvM1.m_linear = jacobian1.m_linear.Scale (data.m_bodies[j1].m_invMass);
		jacobianInvM1.m_angular = data.m_bodies[j1].m_invInertia.UnrotateVector (jacobian1.m_angular);

		accel  = jacobianInvM0.m_linear  % data.m_bodies[j0].m_force;
		accel += jacobianInvM0.m_angular % data.m_bodies[j0].m_torque;
		accel += jacobianInvM1.m_linear  % data.m_bodies[j1].m_force;
		accel += jacobianInvM1.m_angular % data.m_bodies[j1].m_torque;
		// remembet adding centripletal force to teh accelration here
//			accel += ceneteripelts()

		diagonal  = jacobianInvM0.m_linear % jacobian0.m_linear;
		diagonal += jacobianInvM0.m_angular % jacobianInvM0.m_angular;
		diagonal += jacobianInvM1.m_linear % jacobian1.m_linear;
		diagonal += jacobianInvM1.m_angular % jacobianInvM1.m_angular;

		data.m_force[i] = dFloat (0.0f);
		data.m_accel[i] -= accel;
		data.m_diagDamp[i] = diagonal * data.m_relaxation[i];
		data.m_invDJMinvJt[i] = 1.0f / (diagonal + data.m_diagDamp[i]);
	}

	{
		dFloat akNum;
		dFloat accNorm2;

		akNum = dFloat (0.0f);
		accNorm2 = 0.0f;
		for (int i = 0; i < rows; i ++) {
			dFloat bk;
			dFloat val;
			val = data.m_accel[i];
			bk = val * data.m_invDJMinvJt[i];
			akNum += val * bk;
			data.m_deltaForce[i] = bk;
			data.m_activeRow[i] = 1.0f;
			val = val * val;
			if (val > accNorm2) accNorm2 = val;
		}

		for (int i = 0; (i < rows) && (accNorm2 > SOLVER_ERROR_2); i ++) {
			int clampedForce;
			dFloat ak;
			dFloat akDen;
			dFloat clipVal;
			for (int j = 0; j < rows; j ++) {
				data.m_y[j].m_linear[0] = 0.0f;
				data.m_y[j].m_linear[1] = 0.0f;
				data.m_y[j].m_linear[2] = 0.0f;
				data.m_y[j].m_linear[3] = 0.0f;
				data.m_y[j].m_angular[0] = 0.0f;
				data.m_y[j].m_angular[1] = 0.0f;
				data.m_y[j].m_angular[2] = 0.0f;
				data.m_y[j].m_angular[3] = 0.0f;
			}

			for (int j = 0; j < rows; j ++) {
				int i0;
				int i1;
				dFloat val;

				i0 = data.m_body0[j];
				i1 = data.m_body1[j];

				val = data.m_deltaForce[j]; 
				data.m_y[i0].m_linear  += data.m_Jt[j].m_jacobian_IM0.m_linear.Scale (val);
				data.m_y[i0].m_angular += data.m_Jt[j].m_jacobian_IM0.m_angular.Scale (val);

				data.m_y[i1].m_linear  += data.m_Jt[j].m_jacobian_IM1.m_linear.Scale (val);
				data.m_y[i1].m_angular += data.m_Jt[j].m_jacobian_IM1.m_angular.Scale (val);
			}

			akDen = dFloat (0.0f);
			for (int j = 0; j < rows; j ++) {
				int i0;
				int i1;
				dFloat acc;

				i0 = data.m_body0[j];
				i1 = data.m_body1[j];
				
				acc  = ((data.m_JinvMass[j].m_jacobian_IM0.m_linear % data.m_y[i0].m_linear) + (data.m_JinvMass[j].m_jacobian_IM0.m_angular % data.m_y[i0].m_angular));
				acc += ((data.m_JinvMass[j].m_jacobian_IM1.m_linear % data.m_y[i1].m_linear) + (data.m_JinvMass[j].m_jacobian_IM1.m_angular % data.m_y[i1].m_angular));
				acc += data.m_deltaForce[j] * data.m_diagDamp[j];

				data.m_deltaAccel[j] = acc;
				akDen += (acc * data.m_deltaForce[j]);
			}
			

//			_ASSERTE (akDen >= dFloat (0.0f));
			akDen = (akDen > 1.0e-16f) ? akDen : 1.0e-16f;
			_ASSERTE (dAbs (akDen) >= 1.0e-16f);
			ak = akNum / akDen;

			clipVal = 0.0f;
			clampedForce = -1;
			for (int j = activeTiresRows; j < rows; j ++) {

				if (data.m_activeRow[j]) {
					dFloat force;
					force = data.m_force[j] + ak * data.m_deltaForce[j];
					if (force < data.m_minForceLimit[j]) {
						_ASSERTE (((data.m_minForceLimit[j] - data.m_force[j]) / data.m_deltaForce[j]) <= ak);
						ak = (data.m_minForceLimit[j] - data.m_force[j]) / data.m_deltaForce[j];
						clampedForce = j;
						clipVal = data.m_minForceLimit[j];

					} else if (force > data.m_maxForceLimit[j]) {
						_ASSERTE (((data.m_maxForceLimit[j] - data.m_force[j]) / data.m_deltaForce[j]) <= ak);
						ak = (data.m_maxForceLimit[j] - data.m_force[j]) / data.m_deltaForce[j];
						clampedForce = j;
						clipVal = data.m_maxForceLimit[j];
					}
				}
			}

			if (clampedForce != -1) {

				akNum = dFloat (0.0f);
				accNorm2 = dFloat (0.0f);
				data.m_activeRow[clampedForce] = dFloat (0.0f);
				for (int j = 0; j < rows; j ++) {
					dFloat val;
					data.m_force[j] += ak * data.m_deltaForce[j];
					data.m_accel[j] -= ak * data.m_deltaAccel[j];

					val = data.m_accel[j] * data.m_invDJMinvJt[j] * data.m_activeRow[j];
					data.m_deltaForce[j] = val;
					akNum += val * data.m_accel[j];
					val = data.m_accel[j] * data.m_accel[j] * data.m_activeRow[j];
					if (val > accNorm2) accNorm2 = val;
				}
				data.m_force[clampedForce] = clipVal;
				i = -1;

			} else {
				accNorm2 = dFloat (0.0f);
				for (int j = 0; j < rows; j ++) {
					dFloat val;
					data.m_force[j] += ak * data.m_deltaForce[j];
					data.m_accel[j] -= ak * data.m_deltaAccel[j];
					val = data.m_accel[j] * data.m_accel[j] * data.m_activeRow[j];
					if (val > accNorm2) accNorm2 = val;
				}

				if (accNorm2 > SOLVER_ERROR_2) {
					dFloat akDen;

					akDen = (akNum > 1.0e-17f) ? akNum : 1.0e-17f;;
					akNum = 0.0f;
					for (int j = 0; j < rows; j ++) {
						dFloat val;
						val = data.m_accel[j] * data.m_invDJMinvJt[j] * data.m_activeRow[j];
						data.m_deltaAccel[j] = val;
						akNum += data.m_accel[j] * val;
					}

					ak = akNum / akDen;
					for (int j = 0; j < rows; j ++) {
						data.m_deltaForce[j] = data.m_deltaAccel[j] + ak * data.m_deltaForce[j];
					}
				}
			}
		}
	}

	for (int i = 0; i < rows; i ++) {
		int i0;
		int i1;
		dFloat force;

		i0 = data.m_body0[i];
		i1 = data.m_body1[i];
		force = data.m_force[i]; 

		RayCastCarPhysicsData::Jacobian &jacobian0 = data.m_Jt[i].m_jacobian_IM0; 
		RayCastCarPhysicsData::Jacobian &jacobian1 = data.m_Jt[i].m_jacobian_IM1; 

		data.m_bodies[i0].m_force += jacobian0.m_linear.Scale (force);
		data.m_bodies[i0].m_torque += jacobian0.m_angular.Scale (force);

		data.m_bodies[i1].m_force += jacobian1.m_linear.Scale (force);
		data.m_bodies[i1].m_torque += jacobian1.m_angular.Scale (force);
	}

	for (int i = 0; i < m_tiresCount; i ++) {
		Tire& tire = m_tires[i];

		tire.m_torque = 0.0f;
		tire.m_breakTorque = 0.0f;
		tire.m_angularVelocity += (data.m_bodies[i + 2].m_torque % tire.m_lateralDir) * tire.m_IxxInv * timestep;

//if (tire.m_angularVelocity >  100.0) tire.m_angularVelocity = 100.0f;
//if (tire.m_angularVelocity < -100.0) tire.m_angularVelocity = -100.0f;

		tire.m_spinAngle = dMod (tire.m_spinAngle + tire.m_angularVelocity * timestep, 3.141592f * 2.0f); 
	}

	// apply the net force and torque to the car chassis.
	NewtonBodySetForce(m_body0, &data.m_bodies[1].m_force.m_x);
	NewtonBodySetTorque(m_body0, &data.m_bodies[1].m_torque.m_x);
}


#else

void CustomRayCastCar::SubmitConstrainst(dFloat timestep, int threadIndex)
{
	dFloat invTimestep;
	dMatrix bodyMatrix;
	dMatrix suspensionMatrices[VEHICLE_MAX_TIRE_COUNT];  

	// get the simulation time
	invTimestep = 1.0f / timestep ;

	// get the vehicle global matrix, and use it in several calculations
	NewtonBodyGetMatrix(m_body0, &bodyMatrix[0][0]);
	dMatrix chassisMatrix (m_localFrame * bodyMatrix);  

	// calculate all suspension matrices in global space and tire collision
	for (int i = 0; i < m_tiresCount; i ++) {
		Tire& tire = m_tires[i];
		dMatrix suspensionMatrix = suspensionMatrices[i];

		// calculate this suspension matrix and save it for future used
		suspensionMatrix = CalculateSuspensionMatrix (i, 0.0f) * chassisMatrix;

		// calculate the tire collision
		CalculateTireCollision (tire, suspensionMatrix);
	}


	// calculate all suspension forces due to spring and damper
	dVector globalVeloc;
	dVector globalOmega;
	dVector force (0.0f, 0.0f, 0.0f, 0.0f);
	dVector torque (0.0f, 0.0f, 0.0f, 0.0f);

	// get the chassis instantaneous linear and angular velocity in the local space of the chassis
	int longitidunalForceIndex;
	longitidunalForceIndex = 0;
	NewtonBodyGetVelocity(m_body0, &globalVeloc[0]);
	NewtonBodyGetOmega(m_body0, &globalOmega[0]);
	for (int i = 0; i < m_tiresCount; i ++) {
		Tire& tire = m_tires[i];

		if (tire.m_posit < tire.m_suspensionLength) {
			dFloat speed;

			// calculate the linear velocity of the tire at the ground contact
			dVector tireAxelPosit (chassisMatrix.TransformVector(tire.m_harpoint - m_localFrame.m_up.Scale (tire.m_posit)));
			dVector localAxelPosit (tireAxelPosit - chassisMatrix.m_posit);
			dVector tireAxelVeloc (globalVeloc + globalOmega * localAxelPosit); 		

			// TO DO: need to calculate the velocity if the other body at the point
			// for now assume the ground is a static body
			dVector hitBodyVeloc (0, 0, 0, 0);

			// calculate the relative velocity
			dVector relVeloc (tireAxelVeloc - hitBodyVeloc);
			speed = -(relVeloc % chassisMatrix.m_up);

			// now calculate the tire load at the contact point
			tire.m_tireLoad = - NewtonCalculateSpringDamperAcceleration (timestep, tire.m_springConst, tire.m_suspensionLength - tire.m_posit, tire.m_springDamper, speed);

			if (tire.m_tireLoad < 0.0f) {
				// since the tire is not a body with real mass it can only push the chassis.
				tire.m_tireLoad = 0.0f;
			} else {
				//this suspension is applying a normalize force to the car chassis, need to scales by the mass of the car
				tire.m_tireLoad *= m_mass;

				// apply the tire model to these wheel
				ApplyTireFrictionModel(tire, chassisMatrix, tireAxelVeloc, tireAxelPosit, timestep, invTimestep, longitidunalForceIndex);
			}

			// convert the tire load force magnitude to a torque and force.
			dVector tireForce (chassisMatrix.m_up.Scale (tire.m_tireLoad));

			// accumulate the force and torque form this suspension
			force += tireForce;
			torque += localAxelPosit * tireForce;
		} else {
			//tire is on the air  not force applied to the vehicle.
			tire.m_tireLoad = dFloat (0.0f);
//			tire.m_tireJacobianRowIndex = -1;
		}
	}


	// add aerodynamic forces
	dVector verticalVeloc (chassisMatrix.m_up.Scale (chassisMatrix.m_up % globalVeloc));
	dVector horizontalVeloc (globalVeloc - verticalVeloc);
	force -= horizontalVeloc.Scale (m_aerodynamicDrag * m_mass);
	force -= chassisMatrix.m_up.Scale (m_aerodynamicDownForce * m_mass * dSqrt (horizontalVeloc % horizontalVeloc));

	// apply the net suspension force and torque to the car chassis.
	NewtonBodyAddForce(m_body0, &force[0]);
	NewtonBodyAddTorque(m_body0, &torque[0]);

	// set the current vehicle speed
	m_curSpeed = bodyMatrix[0] % globalVeloc;
}


void CustomRayCastCar::ApplyTireFrictionModel(
	Tire& tire, 
	const dMatrix& chassisMatrix,
	const dVector& tireAxelVeloc,
	const dVector& tireAxelPosit,
	dFloat timestep,
	dFloat invTimestep,
	int& longitudinalForceIndex)
{
	// calculate the point of side ping vector
	dVector lateralPin (chassisMatrix.RotateVector (tire.m_localAxis));
	dVector longitudinalPin (chassisMatrix.m_up * lateralPin);
	tire.m_longitudinalDir = longitudinalPin;
	tire.m_lateralDir = lateralPin; 

	// TO DO: need to subtract the velocity at the contact point of the hit body
	// for now assume the ground is a static body
	dVector hitBodyContactVeloc (0, 0, 0, 0);

	// calculate relative velocity at the tire center
	dVector tireAxelRelativeVelocity (tireAxelVeloc - hitBodyContactVeloc); 

	// now calculate relative velocity a velocity at contact point

	dVector tireAngularVelocity (lateralPin.Scale (tire.m_angularVelocity));
	dVector tireRadius (tire.m_contactPoint - tireAxelPosit);
	dVector tireContactVelocity (tireAngularVelocity * tireRadius);	
	dVector tireContactRelativeVelocity (tireAxelRelativeVelocity + tireContactVelocity); 
	tire.m_tireRadius = tireRadius;


	if (1) {
		// the tire is in coasting mode
		//		dFloat tireContactSpeed;
		//		dFloat tireRelativeSpeed;
		//		dFloat lateralForceMagnitud;

		//these tire is coasting, so the lateral friction dominates the behaviors  

		dFloat invMag2;
		dFloat frictionCircleMag;
		dFloat lateralFrictionForceMag;
		dFloat longitudinalAcceleration;
		dFloat longitudinalFrictionForceMag;

		frictionCircleMag = tire.m_tireLoad * tire.m_groundFriction;
		lateralFrictionForceMag = frictionCircleMag;
//		longitudinalFrictionForceMag = tire.m_tireLoad * tire.m_fixedRollingFriction;
		longitudinalFrictionForceMag = tire.m_tireLoad * 1.0f;

		invMag2 = frictionCircleMag / dSqrt (lateralFrictionForceMag * lateralFrictionForceMag + longitudinalFrictionForceMag * longitudinalFrictionForceMag);
		lateralFrictionForceMag *= invMag2;
		longitudinalFrictionForceMag = invMag2;

		//longitudinalFrictionForceMag = 1000;
		NewtonUserJointAddLinearRow (m_joint, &tireAxelPosit[0], &tireAxelPosit[0], &lateralPin[0]);
		NewtonUserJointSetRowMaximumFriction(m_joint,  lateralFrictionForceMag);
		NewtonUserJointSetRowMinimumFriction(m_joint, -lateralFrictionForceMag);
		longitudinalForceIndex ++; 

		// apply longitudinal friction force and acceleration
		longitudinalAcceleration = -(tireContactRelativeVelocity % longitudinalPin) * invTimestep;
		NewtonUserJointAddLinearRow (m_joint, &tireAxelPosit[0], &tireAxelPosit[0], &longitudinalPin[0]);
		NewtonUserJointSetRowMaximumFriction (m_joint,  longitudinalFrictionForceMag);
		NewtonUserJointSetRowMinimumFriction (m_joint, -longitudinalFrictionForceMag);
		NewtonUserJointSetRowAcceleration (m_joint, longitudinalAcceleration);
//		tire.m_tireJacobianRowIndex = longitudinalForceIndex;
		longitudinalForceIndex ++;



		// save the tire contact longitudinal velocity for integration after the solver
		tire.m_currentSlipVeloc = tireAxelRelativeVelocity % longitudinalPin;
	} else {
		_ASSERTE (0);
	}
}


/*
void CustomRayCastCar::IntegrateTires (dFloat timestep, int threadIndex)
{
	dFloat timestepInv;

	// get the simulation time
	timestepInv = 1.0f / timestep;

	dMatrix bodyMatrix;
	NewtonBodyGetMatrix(m_body0, &bodyMatrix[0][0]);
	dMatrix chassisMatrix (m_localFrame * bodyMatrix);  

	dVector globalVeloc;
	dVector globalOmega;
	NewtonBodyGetVelocity(m_body0, &globalVeloc[0]);
	NewtonBodyGetOmega(m_body0, &globalOmega[0]);

	// get the tire longitudinal to apply free force diagram for tire integration
	for (int i = 0; i < m_tiresCount; i ++) {
		Tire& tire = m_tires[i];

		if (tire.m_tireJacobianRowIndex == -1){
			dFloat torque;
			//this is free rolling tire 
			//just apply just integrate the torque and apply some angular damp
			torque = tire.m_torque - tire.m_angularVelocity * tire.m_Ixx * 0.1f;
			tire.m_angularVelocity  += torque * tire.m_IxxInv * timestep;


		} else {
			//check if any engine torque or brake torque is applied to the tire
			if (dAbs(tire.m_torque) < 1.0e-3f){
				//tire is coasting, calculate the tire zero slip angular velocity
				// this is the velocity that satisfy the constraint equation
				// V % dir + W * R % dir = 0
				// where V is the tire Axel velocity
				// W is the tire local angular velocity
				// R is the tire radius
				// dir is the longitudinal direction of of the tire.

				dVector tireAxelPosit (chassisMatrix.TransformVector(tire.m_harpoint - chassisMatrix.m_up.Scale (tire.m_posit)));
				dVector localAxelPosit (tireAxelPosit - chassisMatrix.m_posit);
				dVector tireAxelVeloc (globalVeloc + globalOmega * localAxelPosit); 		

				dFloat tireLinearSpeed;
				dFloat tireContactSpeed;
				tireLinearSpeed = tireAxelVeloc % tire.m_longitudinalDir;
				tireContactSpeed = (tire.m_lateralDir * tire.m_tireRadius) % tire.m_longitudinalDir;
				tire.m_angularVelocity = - tireLinearSpeed / tireContactSpeed ;
			} else {
				// tire is under some power, need to do the free body integration to apply the net torque
				int tireforceIndex;
				dFloat netTorque;
				dFloat tireFrictionTorque;
				tireforceIndex = tire.m_tireJacobianRowIndex;
				tireFrictionTorque = tire.m_radius * NewtonUserJointGetRowForce (m_joint, tireforceIndex);
				netTorque = tire.m_torque - tireFrictionTorque - tire.m_angularVelocity * tire.m_Ixx * 0.1f;


				tire.m_angularVelocity  += netTorque * tire.m_IxxInv * timestep;

			}
		}

		// integrate tire angular velocity and rotation
		tire.m_spinAngle = dMod (tire.m_spinAngle + tire.m_angularVelocity * timestep, 3.141592f * 2.0f); 

		// reset tire torque to zero after integration; 
		tire.m_torque = 0.0f;
	}
}
*/


#endif
#endif



































