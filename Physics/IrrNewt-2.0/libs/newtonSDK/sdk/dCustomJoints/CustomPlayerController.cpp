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

#include "CustomJointLibraryStdAfx.h"
#include "CustomPlayerController.h"


#define MAX_CONTACTS					16
#define SENSOR_SHAPE_SEGMENTS			32
#define MAX_COLLISIONS_ITERATION		8


#define DEAD_RESIDUAL_HORIZONTAL_VELOC	0.1f		

CustomPlayerController::CustomPlayerController(
	const dMatrix& playerFrameInGlobalSpace,
	const NewtonBody* child, 
	dFloat maxStairStepFactor,
	dFloat cushion)
	:NewtonCustomJoint(6, child, NULL)
{
	dVector com;
	dMatrix matrix;
	NewtonCollision* shape;

	// the minimum cushion is 1.0/64.0f of a unit
	if (cushion < 1.0f/64.0f) {
		cushion = 1.0f/64.0f;
	}

	m_heading = 0.0f;
	m_sideSpeed = 0.0f;
	m_restitution = 0.0f;
	m_forwardSpeed = 0.0f;
//	m_playerState = m_onLand;
	m_playerState = m_onFreeFall;
	m_kinematicCushion = cushion;
	
	SetMaxSlope (30.0f * 3.141592f / 180.0f);

	NewtonBodyGetMatrix (child, &matrix[0][0]);
	NewtonBodyGetCentreOfMass (child, &com[0]);
	com.m_w = 1.0f;

	dMatrix pinMatrix (playerFrameInGlobalSpace);
	pinMatrix.m_posit = matrix.TransformVector(com);
	pinMatrix.m_posit.m_w = 1.0f;
	CalculateLocalMatrix (pinMatrix, m_localMatrix0, m_localMatrix1);


	// register the callback for tire integration
	NewtonUserJointSetFeedbackCollectorCallback (m_joint, KinematicMotion);

	// calculate the dimensions of the Player internal auxiliary shapes
	shape = NewtonBodyGetCollision (child);

	// calculate top and bottom side
	dVector top;
	dVector bottom;
	dVector upDir (m_localMatrix0.m_front);
	NewtonCollisionSupportVertex (shape, &upDir[0], &top[0]);
//	top += upDir.Scale (m_kinematicCushion);

	dVector downDir (m_localMatrix0[0].Scale (-1.0f));
	NewtonCollisionSupportVertex (shape, &downDir[0], &bottom[0]);
//	bottom += downDir.Scale (m_kinematicCushion);

	m_playerHeight = (top - bottom) % upDir;

	// set stairs step high as a percent of the player high
	m_stairHeight = m_playerHeight * maxStairStepFactor;

	// calculate the radius
	dFloat r1;
	dFloat r2;
	dFloat floorRadios;
	dVector radiusVector1;
	dVector radiusVector2;
	NewtonCollisionSupportVertex (shape, &m_localMatrix0[1][0], &radiusVector1[0]);
	NewtonCollisionSupportVertex (shape, &m_localMatrix0[2][0], &radiusVector2[0]);
	r1 = dAbs (radiusVector1 % m_localMatrix0[1]);
	r2 = dAbs (radiusVector2 % m_localMatrix0[2]);
	floorRadios = (r1 > r2 ? r1 : r2);
	m_maxRadius = floorRadios + m_kinematicCushion;

	dVector stairSensorShape[SENSOR_SHAPE_SEGMENTS * 2];
	dVector bodySensorPoints[SENSOR_SHAPE_SEGMENTS * 2];
	dVector floorSensorShape[SENSOR_SHAPE_SEGMENTS * 2];

	dFloat h0; 
	dFloat h1; 
	dFloat startHight; 
	h0 = bottom % pinMatrix[0]; 
	h1 = top % pinMatrix[0] - m_stairHeight; 
	startHight = h0 + m_stairHeight;

	m_loweCap = h0;
	for (int i = 0; i < SENSOR_SHAPE_SEGMENTS; i ++) {

		dFloat x;
		dFloat z;
		dFloat fx;
		dFloat fz;

		x = dCos (2.0f * 3.14159265f * dFloat(i) / dFloat(SENSOR_SHAPE_SEGMENTS));
		z = dSin (2.0f * 3.14159265f * dFloat(i) / dFloat(SENSOR_SHAPE_SEGMENTS));

		fx = floorRadios * x;
		fz = floorRadios * z;

		x = m_maxRadius * x;
		z = m_maxRadius * z;

		dVector point (h0, x, z);
		point = m_localMatrix0.RotateVector (point);
		bodySensorPoints[i].m_x = point.m_x;	
		bodySensorPoints[i].m_y = point.m_y;
		bodySensorPoints[i].m_z = point.m_z;

		point = dVector (h1, x, z);
		point = m_localMatrix0.RotateVector (point);
		bodySensorPoints[i + SENSOR_SHAPE_SEGMENTS].m_x = point.m_x; 
		bodySensorPoints[i + SENSOR_SHAPE_SEGMENTS].m_y = point.m_y;
		bodySensorPoints[i + SENSOR_SHAPE_SEGMENTS].m_z = point.m_z;

		point = dVector (h0, fx, fz);
		point = m_localMatrix0.RotateVector (point);
		floorSensorShape[i].m_x = point.m_x;
		floorSensorShape[i].m_y = point.m_y;
		floorSensorShape[i].m_z = point.m_z;

		point = dVector (h1, fx, fz);
		point = m_localMatrix0.RotateVector (point);
		floorSensorShape[i + SENSOR_SHAPE_SEGMENTS].m_x = point.m_x;
		floorSensorShape[i + SENSOR_SHAPE_SEGMENTS].m_y = point.m_y;
		floorSensorShape[i + SENSOR_SHAPE_SEGMENTS].m_z = point.m_z;


		point = dVector (h0, x, z);
		point = m_localMatrix0.RotateVector (point);
		stairSensorShape[i].m_x = point.m_x;
		stairSensorShape[i].m_y = point.m_y;
		stairSensorShape[i].m_z = point.m_z;

		point = dVector (startHight, x, z);
		point = m_localMatrix0.RotateVector (point);
		stairSensorShape[i + SENSOR_SHAPE_SEGMENTS].m_x = point.m_x;
		stairSensorShape[i + SENSOR_SHAPE_SEGMENTS].m_y = point.m_y;
		stairSensorShape[i + SENSOR_SHAPE_SEGMENTS].m_z = point.m_z;

	}

	m_stairSensorShape = NewtonCreateConvexHull (m_world, SENSOR_SHAPE_SEGMENTS * 2, &stairSensorShape[0].m_x, sizeof (dVector), 0.0f, 0, NULL);
	m_bodySensorShape = NewtonCreateConvexHull (m_world, SENSOR_SHAPE_SEGMENTS * 2, &bodySensorPoints[0].m_x, sizeof (dVector), 0.0f, 0, NULL);
	m_bodyFloorSensorShape = NewtonCreateConvexHull (m_world, SENSOR_SHAPE_SEGMENTS * 2, &floorSensorShape[0].m_x, sizeof (dVector), 0.0f, 0, NULL);
}

CustomPlayerController::~CustomPlayerController()
{
	NewtonReleaseCollision(m_world, m_bodySensorShape);
	NewtonReleaseCollision(m_world, m_stairSensorShape);
	NewtonReleaseCollision(m_world, m_bodyFloorSensorShape);
	
}


void CustomPlayerController::SetVelocity (dFloat forwardSpeed, dFloat sideSpeed, dFloat heading)
{
	m_heading = heading;
	m_sideSpeed = sideSpeed;
	m_forwardSpeed = forwardSpeed;
}

void CustomPlayerController::GetVelocity (dFloat& forwardSpeed, dFloat& sideSpeed, dFloat& heading) const
{
	heading = m_heading;
	sideSpeed = m_sideSpeed;
	forwardSpeed = m_forwardSpeed;
}


const NewtonCollision* CustomPlayerController::GetSensorShape () const
{
	return m_bodySensorShape;
}

const NewtonCollision* CustomPlayerController::GetStairStepShape () const
{
	return m_stairSensorShape;
}



void CustomPlayerController::SetMaxSlope (dFloat maxSlopeAngleIndRadian)
{
//	
	maxSlopeAngleIndRadian = dAbs(maxSlopeAngleIndRadian);
	if (maxSlopeAngleIndRadian < 10.0f * 3.14159265f / 180.0f) {
		maxSlopeAngleIndRadian = 10.0f * 3.14159265f / 180.0f;
	}
	if (maxSlopeAngleIndRadian > 60.0f * 3.14159265f / 180.0f) {
		maxSlopeAngleIndRadian = 60.0f * 3.14159265f / 180.0f;
	}

	m_maxSlope = dCos (maxSlopeAngleIndRadian);
//	m_maxSlope = 1.0f - dSin (dAbs(maxSlopeAngleIndRadian));
}

dFloat CustomPlayerController::GetMaxSlope () const
{
//	return dSin (1.0f - m_maxSlope);
	return dCos (m_maxSlope);
}

dFloat CustomPlayerController::GetPlayerHeight() const
{
	return  m_playerHeight;
}

dFloat CustomPlayerController::GetPlayerStairHeight() const
{
	return m_stairHeight;
}

/*
void CustomPlayerController::GetInfo (NewtonJointRecord* info) const
{
}

unsigned CustomPlayerController::ConvexDynamicCastPrefilter(const NewtonBody* body, const NewtonCollision* collision, void* userData)
{
	// for now just collide with static bodies
	dFloat mass; 
	dFloat Ixx; 
	dFloat Iyy; 
	dFloat Izz; 
	CastFilterData* CastFilterData;

	CastFilterData = (CastFilterData*) userData;
	for (int i =0; i < CastFilterData->m_count;i ++){
		if (CastFilterData->m_filter[i] == body) {
			return 0;
		}
	}
	NewtonBodyGetMassMatrix(body, &mass, &Ixx, &Iyy, &Izz);
	return (mass > 0.0f);
}


dFloat CustomPlayerController::FindFloorCallback(const NewtonBody* body, const dFloat* hitNormal, int collisionID, void* userData, dFloat intersetParam)
{
	dFloat param;

	param = 1.0f;
	FindFloorData& data = *((FindFloorData*)userData);
	if (body != data.m_me) {
		param = intersetParam;
		if (param < data.m_param) {
			data.m_param = param;
			data.m_normal.m_x = hitNormal[0];
			data.m_normal.m_y = hitNormal[1];
			data.m_normal.m_z = hitNormal[2];
			data.m_hitBody = body;
		}
	}

	return param;
}
*/

unsigned CustomPlayerController::ConvexStaticCastPrefilter(const NewtonBody* body, const NewtonCollision* collision, void* userData)
{
// for now just collide with static bodies
//	dFloat mass; 
//	dFloat Ixx; 
//	dFloat Iyy; 
//	dFloat Izz; 
//	CastFilterData* CastFilterData;
//	CastFilterData = (CastFilterData*) userData;
//	for (int i =0; i < CastFilterData->m_count;i ++){
//		if (CastFilterData->m_filter[i] == body) {
//			return 0;
//		}
//	}
//	NewtonBodyGetMassMatrix(body, &mass, &Ixx, &Iyy, &Izz);
//	return (mass == 0.0f);
//	return 1;

	dFloat mass; 
	dFloat Ixx; 
	dFloat Iyy; 
	dFloat Izz; 
	NewtonBodyGetMassMatrix(body, &mass, &Ixx, &Iyy, &Izz);
	return (mass == 0.0f);
}

unsigned CustomPlayerController::ConvexAllBodyCastPrefilter(const NewtonBody* body, const NewtonCollision* collision, void* userData)
{
	CastFilterData* filterData;

	filterData = (CastFilterData*) userData;
	for (int i =0; i < filterData->m_count;i ++){
		if (filterData->m_filter[i] == body) {
			return 0;
		}
	}
	return 1;
}




void CustomPlayerController::KinematicMotion (const NewtonJoint* userJoint, dFloat timestep, int threadIndex)
{
	CustomPlayerController* joint;  

	// get the pointer to the joint class
	joint = (CustomPlayerController*) NewtonJointGetUserData (userJoint);  
	joint->KinematicMotion(timestep, threadIndex);
}


dMatrix CustomPlayerController::CalculateVisualMatrix () const
{
	dMatrix bodyMatrix;

	// Get the global matrices of each rigid body.
	NewtonBodyGetMatrix(m_body0, &bodyMatrix[0][0]);

/*
	if (!m_isInJumpState) {
//		dFloat hitParam;
//		CastFilterData staticCastFilterData (m_body0);
//		NewtonWorldConvexCastReturnInfo info;
		dVector upDir (bodyMatrix.RotateVector (m_localMatrix0.m_front));
//		bodyMatrix.m_posit += upDir.Scale (m_stairHeight);
//		dVector target (bodyMatrix.m_posit - upDir.Scale(m_stairHeight * 2.0f));

		FindFloorData floor (m_body0);
		dVector p0 (bodyMatrix.m_posit + upDir.Scale (m_loweCap + m_stairHeight));
		dVector p1 (p0 - upDir.Scale (2.0f * m_stairHeight));
		NewtonWorldRayCast (m_world, &p0[0], &p1[0], FindFloorCallback, &floor, NULL);
		if (floor.m_hitBody) {
			bodyMatrix.m_posit -= upDir.Scale (m_stairHeight * (2.0f * floor.m_param - 1.0f));
		}
	}
*/
	return bodyMatrix;
}





void CustomPlayerController::SubmitConstraints (dFloat timestep, int threadIndex)
{
	dFloat mag; 
	dFloat angle; 
	dFloat invIxx;
	dFloat invIyy;
	dFloat invIzz;
	dFloat invMass;
	dMatrix matrix0;
	dMatrix matrix1;
	dVector velocity;

	// save the gravity before collision force are applied
	NewtonBodyGetForceAcc (m_body0, &m_gravity[0]);
	NewtonBodyGetInvMass(m_body0, &invMass, &invIxx, &invIyy, &invIzz);
	m_gravity = m_gravity.Scale (invMass);


	// calculate the position of the pivot point and the Jacobian direction vectors, in global space. 
	CalculateGlobalMatrix (m_localMatrix0, m_localMatrix1, matrix0, matrix1);

	// if the body ha rotated by some amount, the there will be a plane of rotation
	dVector lateralDir (matrix0.m_front * matrix1.m_front);
	mag = lateralDir % lateralDir;
	if (mag > 1.0e-6f) {
		// if the side vector is not zero, it means the body has rotated
		mag = dSqrt (mag);
		lateralDir = lateralDir.Scale (1.0f / mag);
		angle = dAsin (mag);

		// add an angular constraint to correct the error angle
		NewtonUserJointAddAngularRow (m_joint, angle, &lateralDir[0]);

		// in theory only one correction is needed, but this produces instability as the body may move sideway.
		// a lateral correction prevent this from happening.
		dVector frontDir (lateralDir * matrix1.m_front);
		NewtonUserJointAddAngularRow (m_joint, 0.0f, &frontDir[0]);
	} else {
		// if the angle error is very small then two angular correction along the plane axis do the trick
		NewtonUserJointAddAngularRow (m_joint, 0.0f, &matrix0.m_up[0]);
		NewtonUserJointAddAngularRow (m_joint, 0.0f, &matrix0.m_right[0]);
	}



	// get linear velocity
	if (m_playerState == m_onLand) {
//		dFloat mag2;
		NewtonBodyGetVelocity(m_body0, &velocity[0]);

		// Get the global matrices of each rigid body.
		const dVector& frontDir = matrix0.m_up;
		const dVector& strafeDir = matrix0.m_right;
		const dVector& upDir = matrix0.m_front;
		dVector desiredVeloc (frontDir.Scale (m_forwardSpeed) + upDir.Scale (velocity % upDir) + strafeDir.Scale (-m_sideSpeed));
	//	dFloat maxSpeed = dAbs (m_forwardSpeed) > dAbs (m_sideSpeed) ? dAbs (m_forwardSpeed) : dAbs (m_sideSpeed);	
	//	mag2 = desiredVeloc % desiredVeloc;
	//	if (mag2 > 1.0e-6f) {
	//		desiredVeloc = desiredVeloc.Scale (maxSpeed /dSqrt (mag2));
	//	}

		NewtonBodySetVelocity(m_body0, &desiredVeloc[0]);
	}
}


int CustomPlayerController::PreProcessContacts (NewtonWorldConvexCastReturnInfo* const contacts, int count, const dVector& upDir) const
{
	// flatten contact prune contacts
	for (int i = 0; i < count; i ++) {
		dVector normal (contacts[i].m_normal);
//		dVector point (contacts[i].m_point);
		normal -= upDir.Scale (upDir % normal);
		normal = normal.Scale (1.0f / dSqrt (normal % normal));
		contacts[i].m_normal[0] = normal.m_x;
		contacts[i].m_normal[1] = normal.m_y;
		contacts[i].m_normal[2] = normal.m_z;
//		contacts[i].m_normal[3] = - (normal % point);
	}

	// prune contact with the same normal
	for (int i = 0; i < (count - 1); i ++) {
		const dVector& normal0 = *((dVector*) (&contacts[i].m_normal[0])); 
		for (int j = i + 1; j < count; j ++) {
			const dVector& normal1 = *((dVector*) (&contacts[j].m_normal[0])); 
			dFloat val = normal0 % normal1;
			if (val > 0.9995f) {
				count --;
				contacts[j] = contacts[count];
				j --;
			}
		}
	}
	return count;
}

dVector CustomPlayerController::CalculateVelocity (
	const dVector& veloc, 
	dFloat timestep, 
	const dVector& upDir, 
	dFloat elevation,
	int threadIndex) const
{
	// see if it hit and obstacle
	
	dFloat mag2;

	dVector verticalVeloc (upDir.Scale (veloc % upDir)) ;
	dVector velocity (veloc - verticalVeloc);
	mag2 = velocity % velocity;

/*
static int xxx1;
xxx1 ++;
if (xxx1 >= 170){
xxx1 *=1;
dMatrix bodyMatrix;

//NewtonBodyGetVelocity(m_body0, &veloc[0]);
NewtonBodyGetMatrix(m_body0, &bodyMatrix[0][0]);
static dVector xxxx(bodyMatrix.m_posit);
dVector x (bodyMatrix.m_posit - xxxx);
//dTrace (("%d %f %f %f\n", xxx, bodyMatrix.m_posit.m_x, bodyMatrix.m_posit.m_y, bodyMatrix.m_posit.m_z));
dTrace (("step(%d %f %f %f)  v(%f %f)\n", xxx1, x.m_x, x.m_y, x.m_z, veloc.m_x, veloc.m_z));
xxxx = bodyMatrix.m_posit;
}
*/

	if (mag2 > 0.0f) {
		int contactCount;
		int prevContactCount;
		int positionHasChanged;
		dFloat hitParam;
		dFloat paddingTimestep;
		dMatrix bodyMatrix;
		CastFilterData castFilterData (m_body0);
		NewtonWorldConvexCastReturnInfo info[MAX_CONTACTS];
		NewtonWorldConvexCastReturnInfo prevInfo[MAX_CONTACTS];


		NewtonBodyGetMatrix(m_body0, &bodyMatrix[0][0]);
		castFilterData.m_count = 1;
		bodyMatrix.m_posit += upDir.Scale (elevation);

		dVector translationStep (velocity.Scale (timestep));
		dVector paddingStep (velocity.Scale (m_kinematicCushion / dSqrt (mag2)));
		dVector destination (bodyMatrix.m_posit + translationStep + paddingStep);

		paddingTimestep = (paddingStep % velocity) / (velocity % velocity);

		prevContactCount = 0;
		positionHasChanged = 0;
		contactCount = NewtonWorldConvexCast (m_world, &bodyMatrix[0][0], &destination[0], m_bodySensorShape, &hitParam, &castFilterData, ConvexStaticCastPrefilter, info, sizeof (info) / sizeof (info[0]), threadIndex);
		contactCount = PreProcessContacts (info, contactCount, upDir);
		for (int iterations = 0; contactCount && (iterations < MAX_COLLISIONS_ITERATION); iterations ++) {
			int flag;
			int count;
			dFloat time;

			// calculate the travel time, and subtract from time remaining
			time = hitParam * ((translationStep + paddingStep) % velocity) / (velocity % velocity) - paddingTimestep;

			// teleport the body to the intersection point
			positionHasChanged = 1;
			bodyMatrix.m_posit += velocity.Scale (time);

			dFloat speed[MAX_CONTACTS * 2];
			dFloat bounceSpeed[MAX_CONTACTS * 2];
			dVector bounceNormal[MAX_CONTACTS * 2];
			dVector auxBounceVeloc (0.0f, 0.0f, 0.0f, 0.0f);

			count = 0;
			for (int i = 0; i < contactCount; i ++) {
				dFloat reboundVeloc;						

				speed[count] = 0.0f;
				
				bounceNormal[count] = info[i].m_normal;
				reboundVeloc = -(velocity % bounceNormal[count]) * (1.0f + m_restitution);
				bounceSpeed[count] = (reboundVeloc > 0.0f) ? reboundVeloc : 0.0f; 
				count ++;
			}

			for (int i = 0; i < prevContactCount; i ++) {
				dFloat reboundVeloc;						

				speed[count] = 0.0f;
				bounceNormal[count] = prevInfo[i].m_normal;
				reboundVeloc = (velocity % bounceNormal[count]) * (1.0f + m_restitution);
				bounceSpeed[count] = (reboundVeloc > 0.0f) ? reboundVeloc : 0.0f; 
				count ++;
			}

			dFloat residual;
			residual = 10.0f;
			for (int i = 0; (i < 16) && (residual > 1.0e-3f); i ++) {
				residual = 0.0f;
				for (int k = 0; k < count; k ++) {
					dFloat v;
					dFloat x;

					v = bounceSpeed[k] - bounceNormal[k] % auxBounceVeloc;
					x = speed[k] + v;

					if (x < 0.0f) {
						v = 0.0f;
						x = 0.0f;
					}

					if (dAbs (v) > residual) {
						residual = dAbs (v);
					}

					auxBounceVeloc += bounceNormal[k].Scale (x - speed[k]);
					speed[k] = x;
				}
			}

			flag = 0;
			for (int i = 0; i < count; i ++) {
				flag = (speed[i] > 0.0f);
				velocity += bounceNormal[i].Scale (speed[i]);
			}

			prevContactCount = contactCount;
			contactCount = 0;			
			if (flag) {
				memcpy (prevInfo, info, prevContactCount * sizeof (NewtonWorldConvexCastReturnInfo));
				mag2 = velocity % velocity;
				if (mag2 > 1.0e-6f) {
					translationStep = velocity.Scale (timestep);
					paddingStep = velocity.Scale (m_kinematicCushion / dSqrt (mag2));
					dVector destination (bodyMatrix.m_posit + translationStep + paddingStep);
					castFilterData.m_count = 1;

					contactCount = NewtonWorldConvexCast (m_world, &bodyMatrix[0][0], &destination[0], m_bodySensorShape, &hitParam, &castFilterData, ConvexStaticCastPrefilter, info, sizeof (info) / sizeof (info[0]), threadIndex);
					contactCount = PreProcessContacts (info, contactCount, upDir);
				}
			}
		}

		if (positionHasChanged) {
			bodyMatrix.m_posit -= upDir.Scale (elevation);
			NewtonBodySetMatrix (m_body0, &bodyMatrix[0][0]);
		}
		// prevent the body form drifting form residual velocity when it is in a corner
		mag2 = velocity % velocity;
		if (mag2 < (DEAD_RESIDUAL_HORIZONTAL_VELOC * DEAD_RESIDUAL_HORIZONTAL_VELOC)) {
			velocity = dVector (0.0f, 0.0f, 0.0f, 0.0f) ;
		}
	}

	return (velocity + verticalVeloc);
}

int CustomPlayerController::FindFloor (const dMatrix& origin, const dVector& dest, const dVector upDir, NewtonCollision* shape, dFloat& hitParam, dVector& normal, int threadIndex) const
{
	int contacts;
	CastFilterData castFilterData (m_body0);
	NewtonWorldConvexCastReturnInfo info[MAX_CONTACTS];

	contacts = NewtonWorldConvexCast (m_world, &origin[0][0], &dest[0], shape, &hitParam, &castFilterData, ConvexAllBodyCastPrefilter, info, 8, threadIndex);
	if (contacts) {
		int bestContact = 0;
		dFloat bestValue = upDir % info[0].m_normalOnHitPoint;
		for (int i = 1; i < contacts; i ++) {
			dFloat value = upDir % info[i].m_normalOnHitPoint;
			if (value > bestValue) {
				bestContact = i;
				bestValue = value;

			}
		}
		normal = info[bestContact].m_normalOnHitPoint;
		contacts = 1;
	}
	return contacts;
}


void CustomPlayerController::PlayerOnFreeFall (dFloat timestep, int threadIndex)
{
	dFloat dist;
	dFloat hitParam;
	dVector velocity;
	dMatrix bodyMatrix;

	// Get the global matrices of each rigid body.
	NewtonBodyGetMatrix(m_body0, &bodyMatrix[0][0]);
	dVector posit (bodyMatrix.m_posit);

	dVector upDir (bodyMatrix.RotateVector (m_localMatrix0.m_front));
	dVector frontDir (bodyMatrix.RotateVector (m_localMatrix0.m_up));

	// make sure the body velocity will no penetrates other bodies
	NewtonBodyGetVelocity (m_body0, &velocity[0]);

	velocity = CalculateVelocity (velocity, timestep, upDir, m_stairHeight, threadIndex);

	// player of in free fall look ahead for the land
	dist = upDir % velocity.Scale (timestep);

	bodyMatrix.m_posit -= upDir.Scale (m_kinematicCushion);
	dVector floorNormal;
	dVector target (bodyMatrix.m_posit + upDir.Scale (dist));
	if (FindFloor (bodyMatrix, target, upDir, m_bodyFloorSensorShape, hitParam, floorNormal, threadIndex)) {
		// player is about to land, snap position to the ground
		bodyMatrix.m_posit = posit + upDir.Scale (dist * hitParam);
		NewtonBodySetMatrix (m_body0, &bodyMatrix[0][0]);

		if ((floorNormal % upDir) < m_maxSlope) {
			m_playerState = m_onIlligalRamp;
		} else {
			m_playerState = m_onLand;
			velocity = velocity - upDir.Scale (upDir % velocity);
		}
	}
	NewtonBodySetVelocity(m_body0, &velocity[0]);
}



void CustomPlayerController::PlayerOnRamp (dFloat timestep, int threadIndex)
{
	dFloat hitParam;
	dVector velocity;
	dMatrix bodyMatrix;
//	CastFilterData castFilterData (m_body0);
//	NewtonWorldConvexCastReturnInfo info[MAX_CONTACTS];

	// Get the global matrices of each rigid body.
	NewtonBodyGetMatrix(m_body0, &bodyMatrix[0][0]);
	dVector posit (bodyMatrix.m_posit);

	dVector upDir (bodyMatrix.RotateVector (m_localMatrix0.m_front));
	dVector frontDir (bodyMatrix.RotateVector (m_localMatrix0.m_up));

	// make sure the body velocity will no penetrates other bodies
	NewtonBodyGetVelocity (m_body0, &velocity[0]);

	
//	apply free fall gravity force to the body along the ramp
//	castFilterData.m_count = 1;
	bodyMatrix.m_posit = posit + upDir.Scale (m_stairHeight);
	dVector target (bodyMatrix.m_posit - upDir.Scale(m_stairHeight * 2.0f));
	dVector floorNormal;
//	if (NewtonWorldConvexCast (m_world, &bodyMatrix[0][0], &target[0], m_bodyFloorSensorShape, &hitParam, &castFilterData, ConvexStaticCastPrefilter, info, sizeof (info) / sizeof (info[0]), threadIndex)) {
	if (FindFloor (bodyMatrix, target, upDir, m_bodyFloorSensorShape, hitParam, floorNormal, threadIndex)) {
//		velocity -= m_gravity.Scale (timestep);
		velocity += m_gravity.Scale (timestep);

//		dVector	floorNormal (info[0].m_normalOnHitPoint[0], info[0].m_normalOnHitPoint[1], info[0].m_normalOnHitPoint[2], 0.0f);
//		dVector	gravityForce (m_gravity - floorNormal.Scale (floorNormal % m_gravity));
//		velocity = velocity - floorNormal.Scale (floorNormal % velocity) + gravityForce.Scale (timestep);
		velocity = velocity - floorNormal.Scale (floorNormal % velocity);
	}

	velocity = CalculateVelocity (velocity, timestep, upDir, m_stairHeight, threadIndex);

	// if the player did not change state, then make sure it is still landed on a floor
	dVector step (velocity.Scale (timestep));
	bodyMatrix.m_posit = posit + step + upDir.Scale (m_stairHeight - m_kinematicCushion);
	target = bodyMatrix.m_posit - upDir.Scale(m_stairHeight * 2.0f);

//	castFilterData.m_count = 1;
//	if (NewtonWorldConvexCast (m_world, &bodyMatrix[0][0], &target[0], m_bodyFloorSensorShape, &hitParam, &castFilterData, ConvexAllBodyCastPrefilter, info, 1, threadIndex)) {
	if (FindFloor (bodyMatrix, target, upDir, m_bodyFloorSensorShape, hitParam, floorNormal, threadIndex)) {
		if (hitParam > 1.0e-3f) {

			int isFuturePosiInRamp;

			bodyMatrix.m_posit = bodyMatrix.m_posit + (target - bodyMatrix.m_posit).Scale (hitParam) - step + upDir.Scale (m_kinematicCushion);
			NewtonBodySetMatrix (m_body0, &bodyMatrix[0][0]);

//			dVector	floorNormal (info[0].m_normalOnHitPoint[0], info[0].m_normalOnHitPoint[1], info[0].m_normalOnHitPoint[2], 0.0f);

			isFuturePosiInRamp = (floorNormal % upDir) < m_maxSlope;
			if (!isFuturePosiInRamp) {
				m_playerState = m_onLand;
//			} else {
//				// apply free fall gravity force to the body along the ramp
//				dFloat hitParam1;
//				NewtonWorldConvexCastReturnInfo info1;
//				bodyMatrix.m_posit = posit + upDir.Scale (m_stairHeight - m_kinematicCushion);
//				dVector target (bodyMatrix.m_posit - upDir.Scale(m_stairHeight * 2.0f));
//				NewtonWorldConvexCast (m_world, &bodyMatrix[0][0], &target[0], m_bodyFloorSensorShape, &hitParam1, &castFilterData, ConvexAllBodyCastPrefilter, &info1, 1, threadIndex);
//				if (hitParam < hitParam1) {
//					// the player hit the edge of a forbidden ramp
//					//floorNormal -= upDir.Scale (floorNormal % upDir);
//					//_ASSERTE ((floorNormal % floorNormal) > 1.0e-3f);
//					//floorNormal = floorNormal.Scale (1.0f / dSqrt (floorNormal % floorNormal));
//					velocity -= floorNormal.Scale (velocity % floorNormal);
//				}
			}
		}
	} else {
		m_playerState = m_onFreeFall;
	}

	NewtonBodySetVelocity(m_body0, &velocity[0]);
}



void CustomPlayerController::PlayerOnLand (dFloat timestep, int threadIndex)
{
	dFloat hitParam;
	dVector velocity;
	dMatrix bodyMatrix;

	// Get the global matrices of each rigid body.
	NewtonBodyGetMatrix(m_body0, &bodyMatrix[0][0]);
	dVector posit (bodyMatrix.m_posit);

	dVector upDir (bodyMatrix.RotateVector (m_localMatrix0.m_front));
	dVector frontDir (bodyMatrix.RotateVector (m_localMatrix0.m_up));

	// enlarge the time step to no overshot too much

	// make sure the body velocity will no penetrates other bodies
	NewtonBodyGetVelocity (m_body0, &velocity[0]);

	// subtract the Gravity contribution
	velocity -= m_gravity.Scale (timestep);
	velocity = CalculateVelocity (velocity, timestep, upDir, m_stairHeight, threadIndex);

	// if the player did not change state, then make sure it is still landed on a floor
	dVector step (velocity.Scale (timestep));
	bodyMatrix.m_posit = posit + step + upDir.Scale (m_stairHeight - m_kinematicCushion);
	dVector target (bodyMatrix.m_posit - upDir.Scale(m_stairHeight * 2.0f));

	dVector floorNormal;
	if (FindFloor (bodyMatrix, target, upDir, m_bodyFloorSensorShape, hitParam, floorNormal, threadIndex)) {
		if (hitParam > 1.0e-3f) {
			int isFuturePosiInRamp;
			isFuturePosiInRamp = (floorNormal % upDir) < m_maxSlope;

			if (isFuturePosiInRamp) {
				// apply free fall gravity force to the body along the ramp
				dFloat hitParam1;
				dVector floorNormal1;

				bodyMatrix.m_posit = posit + upDir.Scale (m_stairHeight - m_kinematicCushion);
				target = bodyMatrix.m_posit - upDir.Scale(m_stairHeight * 2.0f);
				FindFloor (bodyMatrix, target, upDir, m_bodyFloorSensorShape, hitParam1, floorNormal1, threadIndex);
				if (hitParam < hitParam1) {

					int iterations;
					int contactCount = 1;
					// the player hit the edge of a forbidden ramp
					dVector savedVeloc (velocity);
					for (iterations = 0; contactCount && (iterations < MAX_COLLISIONS_ITERATION); iterations ++) {
						dFloat projectVel;
						floorNormal -= upDir.Scale (floorNormal % upDir);
						_ASSERTE ((floorNormal % floorNormal) > 1.0e-3f);
						floorNormal = floorNormal.Scale (1.0f / dSqrt (floorNormal % floorNormal));

						projectVel = velocity % floorNormal;
						velocity -= floorNormal.Scale (projectVel);

						step = velocity.Scale (timestep);
						bodyMatrix.m_posit = posit + step + upDir.Scale (m_stairHeight - m_kinematicCushion);
						target = bodyMatrix.m_posit - upDir.Scale(m_stairHeight * 2.0f);

						if (FindFloor (bodyMatrix, target, upDir, m_bodyFloorSensorShape, hitParam, floorNormal, threadIndex)) {
							contactCount = (floorNormal % upDir) < m_maxSlope;
						}
					}

					if (iterations >= MAX_COLLISIONS_ITERATION) {
						dVector veloc1 (CalculateVelocity (savedVeloc, timestep, upDir, 0.0f, threadIndex));
						dVector err (veloc1 - velocity);
						if ((err % err) < 1.0e-6f) {
							m_playerState = m_onIlligalRamp;
						} else {
							velocity = veloc1;
							hitParam = 0.0f;
							step = dVector (0.0f, 0.0f, 0.0f, 0.0f);
							bodyMatrix.m_posit = posit - upDir.Scale (m_kinematicCushion);
						}
					}

				} else {
					m_playerState = m_onIlligalRamp;
				}
			}

			bodyMatrix.m_posit = bodyMatrix.m_posit + (target - bodyMatrix.m_posit).Scale (hitParam) + upDir.Scale (m_kinematicCushion);
			bodyMatrix.m_posit -= step;
			NewtonBodySetMatrix (m_body0, &bodyMatrix[0][0]);
		}
	} else {
		m_playerState = m_onFreeFall;
	}

	NewtonBodySetVelocity(m_body0, &velocity[0]);
}



void CustomPlayerController::KinematicMotion (dFloat timestep, int threadIndex)
{
	dFloat turnOmega;
	dFloat turnAngle;
	dMatrix bodyMatrix;

	// handle angular velocity and heading
	NewtonBodyGetMatrix(m_body0, &bodyMatrix[0][0]);

	dVector upDir (bodyMatrix.RotateVector (m_localMatrix0.m_front));
	dVector frontDir (bodyMatrix.RotateVector (m_localMatrix0.m_up));

	dVector heading (0.0f, dCos (m_heading), dSin (m_heading), 0.0f);
	heading = m_localMatrix0.RotateVector(heading);
	turnAngle = (frontDir * heading) % upDir;
	turnAngle = dAsin (min (max (turnAngle, -1.0f), 1.0f)); 
	turnOmega = turnAngle / timestep;

	dVector omega (upDir.Scale (turnOmega));
	NewtonBodySetOmega(m_body0, &omega.m_x);

	switch (m_playerState) 
	{
		case m_onFreeFall:
			PlayerOnFreeFall (timestep, threadIndex);
			break;

		case m_onLand:
			PlayerOnLand (timestep, threadIndex);
			break;

		case m_onIlligalRamp:
			PlayerOnRamp (timestep, threadIndex);
			break;
	}
}
