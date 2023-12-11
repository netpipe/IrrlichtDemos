#pragma once

#include <BulletDynamics/Vehicle/btRaycastVehicle.h>

class CarHandlingDemo {
public:

    CarHandlingDemo();

    void initPhysics();

    void exitPhysics();

    virtual ~CarHandlingDemo();

    virtual void stepSimulation(float deltaTime);

    virtual bool mouseMoveCallback(float x, float y)
    {
	return false;
    }

    virtual bool mouseButtonCallback(int button, int state, float x, float y)
    {
	return false;
    }

    virtual void physicsDebugDraw(int debugFlags);

    virtual void resetCamera()
    {
	float dist = 5 * 8;
	float pitch = -45;
	float yaw = 32;
	float targetPos[3] = { -0.33, -0.72, 4.5 };
    }

private:
    btRaycastVehicle* vehicle;

    btAlignedObjectArray<btCollisionShape*> collisionShapes;

    btRigidBody* createGroundRigidBodyFromShape(btCollisionShape* groundShape);

    btRigidBody* createChassisRigidBodyFromShape(btCollisionShape* chassisShape);

    void addWheels(
		   btVector3* halfExtents,
		   btRaycastVehicle* vehicle,
		   btRaycastVehicle::btVehicleTuning tuning);
};
