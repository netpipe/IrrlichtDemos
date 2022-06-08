//I3rdPersonCam.h                          3rd Person camera for Irrlicht 3D 1.1   
// version 0.1

#include <irrlicht.h>
using namespace irr;
using namespace irr::core;
using namespace irr::scene;
// 1.  create I3rdPersonCam object;
// 2.  attach a CameraSceneNode with attachCamera()
// 3.  Set a target with setTarget(_
// 4.  call modOrientation(pan, tilt, zoom) or setOrientation(pan, tilt, zoom) to position camera
// 5.  call updatePosition() to apply new Position data
// 6.  repeat 4 and 5 as needed



class I3rdPersonCam{

public:
	float camPan;
	float camTilt;
	float camZoom;
	float maxZoom;
	float camSpeed;
	ICameraSceneNode* cam;
	ISceneNode* camTarget;
	vector3df offset;
	
	I3rdPersonCam(){
		camPan=0;
		camTilt=45;
		camZoom=14;
		maxZoom=250;
		camSpeed=1;
		}

	void setTarget(ISceneNode *newTarget){
		camTarget=newTarget;
		}

	void setOffset(vector3df newOffset){
		offset=newOffset;
		}

	void attachCamera(ICameraSceneNode *camtoattach){
		cam=camtoattach;
		}

	void modOrientation(float pan1,float tilt1,float zoom1){
		camPan=camPan+pan1;
		camTilt=camTilt+tilt1;
		camZoom=camZoom+zoom1;
		if(camPan>360)camPan=camPan-360;
		if(camPan<-360)camPan=camPan+360;
		if(camTilt>89)camTilt=89;
		if(camTilt<-89)camTilt=-89;
		if(camZoom>maxZoom)camZoom=maxZoom;
		if(camZoom<0)camZoom=0;
		}

	void setOrientation(float pan1,float tilt1,float zoom1){
		camPan=pan1;
		camTilt=tilt1;
		camZoom=zoom1;
		}


void updatePosition()
{
vector3df CPosVector;
vector3df NewCamLocation;
vector3df Target1;
//CamPan==0 places camera behind Model--- CamPan range 0-360
//camTilt inputs should be between -89 and +89
CPosVector.X=cos((camPan+180)*PI/180)*sin((camTilt+90)*PI/180);
CPosVector.Y=cos((camTilt+90)*PI/180);
CPosVector.Z=sin((camPan+180)*PI/180)*sin((camTilt+90)*PI/180);

matrix4 m2;
m2.setRotationDegrees(camTarget->getRotation());
m2.transformVect(CPosVector);

Target1=camTarget->getPosition()+offset;
NewCamLocation.X=Target1.X+CPosVector.X*camZoom;
NewCamLocation.Y=Target1.Y+CPosVector.Y*camZoom;
NewCamLocation.Z=Target1.Z+CPosVector.Z*camZoom;

cam->setPosition(NewCamLocation);
cam->setUpVector(vector3df(0,1,0));
cam->setTarget(vector3df(Target1));
cam->updateAbsolutePosition();
}

};