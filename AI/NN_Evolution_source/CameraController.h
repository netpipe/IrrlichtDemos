
enum E_CAMERA_TYPE
{
	ECT_BIRDSEYE,// Called "Top Down" in the GUI
	ECT_FOLLOW
};

#define BIRDSEYE_CAMERA_POS		core::vector3df(4.f,6.f,0)
#define BIRDSEYE_CAMERA_TARGET	core::vector3df(0,0,0)
#define FOLLOW_CAMERA_OFFSET	core::vector3df(1.5,1.5,0)
#define INTERPOLATION_SPEED		0.995

class CameraController
{
	scene::ICameraSceneNode *camera;
	E_CAMERA_TYPE cameraType;
	
	core::vector3df nextCameraPos;
	core::vector3df nextCameraTarget;
	
	Vehicle *followVehicle;
	
public:
	CameraController()
	{
		// A camera to view the world
		camera = engine.getSceneManager()->addCameraSceneNode();
		camera->setNearValue(0.01);
		camera->setFOV(core::PI/2.5);
		
		// Default camera is a top down, "bird's eye" view of the scene
		camera->setPosition(BIRDSEYE_CAMERA_POS);
		camera->setTarget(BIRDSEYE_CAMERA_TARGET);
		
		setCameraType(ECT_BIRDSEYE);
		
		setFollowVehicle(NULL);
	}
	
	void setCameraType(E_CAMERA_TYPE type)
	{
		cameraType = type;
		
		switch (type)
		{
		case ECT_BIRDSEYE:
			nextCameraPos = BIRDSEYE_CAMERA_POS;
			nextCameraTarget = BIRDSEYE_CAMERA_TARGET;
			break;
		case ECT_FOLLOW:
			break;
		default:
			return;
		}
	}
	
	void setFollowVehicle(Vehicle *vehicle)
	{
		followVehicle = vehicle;
	}
	
	// called "draw" as it updates positions ready for drawing
	// actual drawing is done by Irrlicht
	void draw(float dt)
	{
		if (cameraType == ECT_FOLLOW)
		{
			if (followVehicle)
			{
				nextCameraTarget = followVehicle->getPosition();
				nextCameraPos = followVehicle->getPosition() + FOLLOW_CAMERA_OFFSET;
			}
			else
			{
				// If there are no vehicles to follow, then use the top down camera type
				// (note: doesn't actually change the camera type with setCameraType. This way
				// it will resume following a vehicle as soon as there is one to follow)
				nextCameraPos = BIRDSEYE_CAMERA_POS;
				nextCameraTarget = BIRDSEYE_CAMERA_TARGET;
			}
		}
		
		core::vector3df cameraPos = camera->getPosition();
		core::vector3df cameraTarget = camera->getTarget();
		
		// Now interpolate the camera towards the desired position and target...
		// (this gives a smooth transition rather than an abrupt switch)
		if (cameraPos != nextCameraPos
				|| cameraTarget != nextCameraTarget)
		{
			float interpolateAmount = 1.f-INTERPOLATION_SPEED*dt;
			
			camera->setPosition( cameraPos.getInterpolated(nextCameraPos, interpolateAmount) );
			camera->setTarget( cameraTarget.getInterpolated(nextCameraTarget, interpolateAmount) );
		}
	}
};
