
#include <irrlicht.h>
using namespace irr;
using namespace irr::core;
using namespace irr::scene;


// 1. create IAnimatedMeshSceneNode <PlayerMesh1>
// 2. create IPlayerBP <BipedPlayer1>
// 3. call BipedPlayer1.attachMesh(Player1Mesh)
// 4. create WASDMouseBPControl <WASDCtrlr>
// 5. call WASDCtrlr.attachPlayerBP(&BipedPlayer1)

// 6-11. optional: include I3rdPersonCam.h
// 7.  create I3rdPersonCam object <Cam1>;
// 8.  attach a CameraSceneNode with Cam1.attachCamera()
// 9.  attach to a target with Cam1.setTarget()
// 10.  call Cam1.modOrientation(pan, tilt, zoom) or Cam1.setOrientation(pan, tilt, zool) to position camera
// 11.  call Cam1.updatePosition() to apply new Position data
// 12.  repeat 10 and 11 as needed


//-----example code

// IAnimatedMeshSceneNode *Player1;
// I3rdPersonCam My3rdCam1;
// IPlayerBP MainPlayer;
// WASDMouseBPControl WASD1;

// MainPlayer.attachMesh(Player1);
// WASD1.attachPlayerBP(&MainPlayer);
//My3rdCam1.setTarget(Player1);
//My3rdCam1.setOffset(vector3df(0,20,0));
//My3rdCam1.attachCamera(camnode);

//while(1){
//	WASD1.Tick("TimebetweenFrames");
//  My3rdCam1.modOrientation( "orientation");
//  My3rdCam.updatePosition();
//}
// end example


//Stop
//Walk
//Run
//WalkBack
//--need Swim
//StrafeLeft
//StrafeRight

//stepMotion




float GetHeight(irr::IrrlichtDevice *device,scene::ITriangleSelector *Ground, const vector3df& pos, float maxHeight) { 
	scene::ISceneCollisionManager* colMan = device->getSceneManager()->getSceneCollisionManager(); 
    vector3df yAxis(0,maxHeight,0); 
    line3d<f32> ray(pos, pos-yAxis); 
    vector3df ptoCol(0,0,0); 
    triangle3df triangle; 
    if(colMan->getCollisionPoint(ray, Ground, ptoCol, triangle)) { 
        return ptoCol.Y; 
		} 
		else 
		{ 
        return -maxHeight; 
		} 

} 




class IPlayerBP{
public:
	IAnimatedMeshSceneNode *myMesh;
	IrrlichtDevice *MyDevice;
	scene::ITriangleSelector *MyFloor;
	scene::ITerrainSceneNode *MyFloorTerrain;
	float momentum;
	float gravity;

	bool Forward, Back, Run, Walk, StrafeLeft, StrafeRight, Swim, Crouch, Jump,Idle;

	float WalkSpeed,RunSpeed,SwimSpeed;
	bool CanJump;
	bool Jumping;
	bool JumpLanding;
	vector3df JumpDirection;
	vector3df GroundOffset;
	float forwardAngle;
	int JumpTime, JumpTimer;
	int WALKFrames[2], WalkFrameSpeed;
	int LANDFrames[2],LandFrameSpeed;
	int RUNFrames[2], RunFrameSpeed;
	int CROUCHWALKFrames[2],CrouchWalkframeSpeed;
	int CROUCHWALKBACKFrames[2],CrouchWalkBackframeSpeed;
	int STANDFrames[2],StandFrameSpeed;	
	int SWIMFrames[2], SwimFrameSpeed;
	int JUMPFrames[2], JumpFrameSpeed;
	int CROUCHFrames[2],CrouchFrameSpeed;
	int DIEFrames[2], DieFrameSpeed;
	int WALKBACKFrames[2],WalkBackFrameSpeed;
	
	IPlayerBP(){
		gravity=9.8*12;
		momentum=0;
		JumpTime=5.6f;
		forwardAngle=0;
		WalkSpeed=80;
		RunSpeed=180;
		SwimSpeed=20;
		WALKFrames[0]=320;
		WALKFrames[1]=367;
	    RUNFrames[0]=320;
		RUNFrames[1]=367;
	    STANDFrames[0]=0;
		STANDFrames[1]=319;
	    SWIMFrames[0]=320;
		SWIMFrames[1]=367;
		JUMPFrames[0]=520;
		JUMPFrames[1]=550;
		LANDFrames[0]=550;
		LANDFrames[1]=570;
		CROUCHFrames[0]=1072;
		CROUCHFrames[1]=1080;
		DIEFrames[0]=0;
		DIEFrames[1]=1;
		CROUCHWALKFrames[0]=1225;
		CROUCHWALKFrames[1]=1265;
		CROUCHWALKBACKFrames[0]=1225;
		CROUCHWALKBACKFrames[1]=1265;
		WALKBACKFrames[0]=320;
		WALKBACKFrames[1]=367;
		WalkBackFrameSpeed=30;
		WalkFrameSpeed=30;
		RunFrameSpeed=60;
		SwimFrameSpeed=80;
		LandFrameSpeed=45;
		JumpFrameSpeed=30;
		StandFrameSpeed=10;
		CrouchFrameSpeed=10;
		CrouchWalkframeSpeed=20;
		CrouchWalkBackframeSpeed=20;
		DieFrameSpeed=10;
		JumpTimer=0;

				Forward= Back= Run= Walk= StrafeLeft= StrafeRight= Swim= Crouch=Jump=Idle=false;


	}


void NULLifyFlags(){
		Forward= Back= Run= Walk= StrafeLeft= StrafeRight= Swim=Idle=JumpLanding=false;
		}

void SetGravity(float UnitsPerSecond){
	gravity=UnitsPerSecond;
	}
void SetGroundOffset(vector3df GO){
		GroundOffset=GO;
}
void SetDevice(IrrlichtDevice *dev1){
		MyDevice=dev1;
	}

void SetGround(IrrlichtDevice *dev1,ITerrainSceneNode *ground1){
	if(ground1){
	MyDevice=dev1;
	if(ground1==MyFloorTerrain)return;
	MyFloorTerrain=ground1;
	MyFloor	= MyDevice->getSceneManager()->createTerrainTriangleSelector(ground1, 0);
	}
	}

void SetJumpForce(int frc){
	JumpTime=frc;
}

void EndJumpMode(){
		NULLifyFlags();
		JumpLanding=true;  
		myMesh->setLoopMode(0);
		myMesh->setFrameLoop(LANDFrames[0],LANDFrames[1]);
		myMesh->setAnimationSpeed(LandFrameSpeed);
		}

void ApplyGravity(int msec){
	vector3df pos1;
	pos1=myMesh->getPosition();
		pos1.Y=pos1.Y+100000;
		
		float THeight=GetHeight(MyDevice,MyFloor,pos1,110000);
		float XHeight=GetHeight(MyDevice,MyFloor,vector3df(pos1.X-1,pos1.Y,pos1.Z),110000);
		float ZHeight=GetHeight(MyDevice,MyFloor,vector3df(pos1.X,pos1.Y,pos1.Z-1),110000);
		pos1.Y=pos1.Y-100000;
		momentum=momentum+gravity*msec/1000.0f;
		pos1.Y-=momentum;
		
		if(Jump){
		pos1.Y+=gravity*JumpTime*msec/1000.0f;
		pos1=pos1+JumpDirection*msec/1000.0f;
		}

		if(pos1.Y<(THeight+GroundOffset.Y)){
			pos1.Y=THeight+GroundOffset.Y;
			CanJump=true;
			if(Jump)EndJumpMode();
			Jump=false;
			momentum=0;
			//these lines should prevent climbing steep slopes
			// they do but you can jump up steep slopes with repeated jumping....needs fixed
			if(THeight<XHeight-1)pos1.X=pos1.X+WalkSpeed/10.0f;
			if(THeight>XHeight+1)pos1.X=pos1.X-WalkSpeed/10.0f;
			if(THeight<ZHeight-1)pos1.Z=pos1.Z+WalkSpeed/10.0f;
			if(THeight>ZHeight+1)pos1.Z=pos1.Z-WalkSpeed/10.0f;
			}
		myMesh->setPosition(pos1);
		myMesh->updateAbsolutePosition();	
	if(JumpLanding&&(myMesh->getFrameNr()==LANDFrames[1])){
	JumpLanding=false;
	Jump=false;
	NULLifyFlags();
		}
	}

void SetWalkSpeed(float speed){
	WalkSpeed=speed;
}

void SetRunSpeed(float speed){
	RunSpeed=speed;
}
void SetSwimSpeed(float speed){
	SwimSpeed=speed;
}




void Turn(float tdegrees){
	vector3df r1;
	r1=myMesh->getRotation();
	r1.Y+=tdegrees;
	myMesh->setRotation(r1);
	myMesh->updateAbsolutePosition();
	}	

void attachMesh(IAnimatedMeshSceneNode *m1){
	myMesh=m1;
	}



//Set Frames Functions
void SetCrouchFrames(int FirstFrame,int LastFrame,int speed){
		CROUCHFrames[0]=FirstFrame;
		CROUCHFrames[1]=LastFrame;
		CrouchFrameSpeed=speed;
	}

void SetJumpLandFrames(int FirstFrame,int LastFrame,int speed){
		LANDFrames[0]=FirstFrame;
		LANDFrames[1]=LastFrame;
		LandFrameSpeed=speed;
	}

void SetCrouchWalkFrames(int FirstFrame,int LastFrame,int speed){
		CROUCHWALKFrames[0]=FirstFrame;
		CROUCHWALKFrames[1]=LastFrame;
		CrouchWalkframeSpeed=speed;
	}

void SetCrouchWalkBackFrames(int FirstFrame,int LastFrame,int speed){
		CROUCHWALKBACKFrames[0]=FirstFrame;
		CROUCHWALKBACKFrames[1]=LastFrame;
		CrouchWalkBackframeSpeed=speed;
	}
void SetDieFrames(int FirstFrame,int LastFrame,int speed){
		DIEFrames[0]=FirstFrame;
		DIEFrames[1]=LastFrame;
		DieFrameSpeed=speed;
	}
void SetWalkFrames(int FirstFrame,int LastFrame,int speed){
		WALKFrames[0]=FirstFrame;
		WALKFrames[1]=LastFrame;
		WalkFrameSpeed=speed;
	}

void SetRunFrames(int FirstFrame,int LastFrame,int speed){
		RUNFrames[0]=FirstFrame;
		RUNFrames[1]=LastFrame;
		RunFrameSpeed=speed;
	}

void SetStandFrames(int FirstFrame,int LastFrame,int speed){
		STANDFrames[0]=FirstFrame;
		STANDFrames[1]=LastFrame;
		StandFrameSpeed=speed;
	}

void SetSwimFrames(int FirstFrame,int LastFrame,int speed){
		SWIMFrames[0]=FirstFrame;
		SWIMFrames[1]=LastFrame;
		SwimFrameSpeed=speed;
	}

void SetJumpFrames(int FirstFrame,int LastFrame,int speed){
		JUMPFrames[0]=FirstFrame;
		JUMPFrames[1]=LastFrame;
		JumpFrameSpeed=speed;
	}

void SetWalkBackFrames(int FirstFrame,int LastFrame,int speed){
		WALKBACKFrames[0]=FirstFrame;
		WALKBACKFrames[1]=LastFrame;
		WalkBackFrameSpeed=speed;
	}
//end set Frames functions


void Die(){
		NULLifyFlags();
		myMesh->setLoopMode(0);
		myMesh->setFrameLoop(DIEFrames[0],DIEFrames[1]);
		myMesh->setAnimationSpeed(DieFrameSpeed);
		}

void StandMode(){
	NULLifyFlags();
	Crouch=false;
	myMesh->setFrameLoop(STANDFrames[0],STANDFrames[1]);
	myMesh->setAnimationSpeed(StandFrameSpeed);
}

void IdleMode(){
	if(Idle||JumpLanding||Jump)return;
	NULLifyFlags();
	Idle=true;
	Crouch=false;
	myMesh->setLoopMode(1);
	myMesh->setFrameLoop(STANDFrames[0],STANDFrames[1]);
	myMesh->setAnimationSpeed(StandFrameSpeed);
}


void Freeze(){
		NULLifyFlags();
		myMesh->setAnimationSpeed(0);


}



// new modified functions begin here 

void JumpMode(){
	if(CanJump){
		CanJump=false;
		NULLifyFlags();
		Jump=true;
		Crouch=false;
		myMesh->setLoopMode(false);
		myMesh->setFrameLoop(JUMPFrames[0],JUMPFrames[1]);
		myMesh->setAnimationSpeed(JumpFrameSpeed);
		}
	}

void RunForwardMode(){
		if((Run&&Forward)||Jump)return;
		NULLifyFlags();
		Run=Forward=true;
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(RUNFrames[0],RUNFrames[1]);
		myMesh->setAnimationSpeed(RunFrameSpeed);
		}
void RunBackMode(){
		if((Run&&Back)||Jumping)return;
		NULLifyFlags();
		Run=Back=true;
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(RUNFrames[0],RUNFrames[1]);
		myMesh->setAnimationSpeed(RunFrameSpeed);
		}




void WalkForwardMode(){	
		if((Walk&&Forward)||Jump)return;
		NULLifyFlags();
		Walk=Forward=true;
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(WALKFrames[0],WALKFrames[1]);
		myMesh->setAnimationSpeed(WalkFrameSpeed);
		}

void WalkBackMode(){
		if((Back)||Jump)return;
		NULLifyFlags();
	
		Back=true;
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(WALKBACKFrames[0],WALKBACKFrames[1]);
		myMesh->setAnimationSpeed(WalkBackFrameSpeed);
		}

void WalkForwardCrouchMode(){	
		if((Walk&&Forward&&Crouch)||Jump)return;
		NULLifyFlags();
		Walk=Forward=Crouch=true;
		myMesh->setLoopMode(true);
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(CROUCHWALKFrames[0],CROUCHWALKFrames[1]);
		myMesh->setAnimationSpeed(CrouchWalkframeSpeed);
		}


void WalkBackCrouchMode(){
		if((Walk&&Back&&Crouch)||Jump)return;
		NULLifyFlags();
		Walk=Back=Crouch=true;
		myMesh->setLoopMode(true);
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(CROUCHWALKBACKFrames[0],CROUCHWALKBACKFrames[1]);
		myMesh->setAnimationSpeed(CrouchWalkBackframeSpeed);
		}

void StrafeLeftMode(){
		if((StrafeLeft)||Jump)return;
		NULLifyFlags();
		StrafeLeft=true;
		myMesh->setLoopMode(true);
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(WALKFrames[0],WALKFrames[1]);
		myMesh->setAnimationSpeed(WalkFrameSpeed);
		}
void StrafeRightMode(){
		if((StrafeRight)||Jump)return;
		NULLifyFlags();
		StrafeRight=true;
		myMesh->setLoopMode(true);
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(WALKFrames[0],WALKFrames[1]);
		myMesh->setAnimationSpeed(WalkFrameSpeed);
		}

void StrafeLeftCrouchMode(){
		if((StrafeLeft&&Crouch)||Jump)return;
		NULLifyFlags();
		StrafeLeft=Crouch=true;
		myMesh->setLoopMode(true);
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(CROUCHWALKFrames[0],CROUCHWALKFrames[1]);
		myMesh->setAnimationSpeed(CrouchWalkframeSpeed);
		}
void StrafeRightCrouchMode(){
		if((StrafeRight&&Crouch)||Jump)return;
		NULLifyFlags();
		StrafeRight=Crouch=true;
		myMesh->setLoopMode(true);
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(CROUCHWALKFrames[0],CROUCHWALKFrames[1]);
		myMesh->setAnimationSpeed(CrouchWalkframeSpeed);
		}

void StrafeLeftForwardMode(){
		if((StrafeLeft&&Walk&&Forward)||Jump)return;
		NULLifyFlags();
		StrafeLeft=Forward=Walk=true;
		myMesh->setLoopMode(true);
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(WALKFrames[0],WALKFrames[1]);
		myMesh->setAnimationSpeed(WalkFrameSpeed);
		}
void StrafeRightForwardMode(){
		if((StrafeRight&&Walk&&Forward)||Jump)return;
		NULLifyFlags();
		StrafeRight=Forward=Walk=true;
		myMesh->setLoopMode(true);
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(WALKFrames[0],WALKFrames[1]);
		myMesh->setAnimationSpeed(WalkFrameSpeed);
		}

void StrafeLeftBackMode(){
		if((StrafeLeft&&Back)||Jump)return;
		NULLifyFlags();
		StrafeLeft=Back=true;
		myMesh->setLoopMode(true);
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(WALKBACKFrames[0],WALKBACKFrames[1]);
		myMesh->setAnimationSpeed(WalkBackFrameSpeed);
		}
void StrafeRightBackMode(){
		if((StrafeRight&&Back)||Jump)return;
		NULLifyFlags();
		StrafeRight=Back=true;
		myMesh->setLoopMode(true);
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(WALKBACKFrames[0],WALKBACKFrames[1]);
		myMesh->setAnimationSpeed(WalkBackFrameSpeed);
		}





void StrafeLeftForwardCrouchMode(){
		if((StrafeLeft&&Forward&&Walk&&Crouch)||Jump)return;
		NULLifyFlags();
		StrafeLeft=Forward=Walk=Crouch=true;
		myMesh->setLoopMode(true);
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(CROUCHWALKFrames[0],CROUCHWALKFrames[1]);
		myMesh->setAnimationSpeed(CrouchWalkframeSpeed);
		}
void StrafeRightForwardCrouchMode(){
		if((StrafeRight&&Forward&&Walk&&Crouch)||Jump)return;
		NULLifyFlags();
		StrafeRight=Forward=Walk=Crouch=true;
		myMesh->setLoopMode(true);
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(CROUCHWALKFrames[0],CROUCHWALKFrames[1]);
		myMesh->setAnimationSpeed(CrouchWalkframeSpeed);
		}

void StrafeLeftBackCrouchMode(){
		if((StrafeLeft&&Back&&Crouch)||Jump)return;
		NULLifyFlags();
		StrafeLeft=Back=Crouch=true;
		myMesh->setLoopMode(true);
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(CROUCHWALKBACKFrames[0],CROUCHWALKBACKFrames[1]);
		myMesh->setAnimationSpeed(CrouchWalkBackframeSpeed);
		}
void StrafeRightBackCrouchMode(){
		if((StrafeRight&&Back&&Crouch)||Jump)return;
		NULLifyFlags();
		StrafeRight=Back=Crouch=true;
		myMesh->setLoopMode(true);
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(CROUCHWALKBACKFrames[0],CROUCHWALKBACKFrames[1]);
		myMesh->setAnimationSpeed(CrouchWalkBackframeSpeed);
		}


void CrouchMode(){
	if((Crouch)||Jump)return;
		NULLifyFlags();
		Crouch=true;
		myMesh->setLoopMode(0);
		myMesh->setFrameLoop(CROUCHFrames[0],CROUCHFrames[1]);
		myMesh->setAnimationSpeed(CrouchFrameSpeed);
		}





//SwimFunctions


void SwimBackMode(){
		if((Swim&&Back)||Jump)return;
		NULLifyFlags();
		Swim=Back=true;
		myMesh->setLoopMode(true);
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(SWIMFrames[0],SWIMFrames[1]);
		myMesh->setAnimationSpeed(SwimFrameSpeed);
		}



void SwimForwardMode(){	
		if((Swim&&Forward)||Jump)return;
		NULLifyFlags();
		Swim=Forward=true;
		myMesh->setLoopMode(true);
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(SWIMFrames[0],SWIMFrames[1]);
		myMesh->setAnimationSpeed(SwimFrameSpeed);
		}


void SwimStrafeLeftMode(){
		if((Swim&&StrafeLeft)||Jump)return;
		NULLifyFlags();
		StrafeLeft=Swim=true;
		myMesh->setLoopMode(true);
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(SWIMFrames[0],SWIMFrames[1]);
		myMesh->setAnimationSpeed(SwimFrameSpeed);;
		}
void SwimStrafeRightMode(){
		if((Swim&&StrafeRight)||Jump)return;
		NULLifyFlags();
		StrafeRight=Swim=true;
		myMesh->setLoopMode(true);
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(SWIMFrames[0],SWIMFrames[1]);
		myMesh->setAnimationSpeed(SwimFrameSpeed);
		}

void SwimStrafeLeftBackMode(){
		if((Swim&&StrafeLeft&&Back)||Jump)return;
		NULLifyFlags();
		StrafeLeft=Swim=Back=true;
		myMesh->setLoopMode(true);
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(SWIMFrames[0],SWIMFrames[1]);
		myMesh->setAnimationSpeed(SwimFrameSpeed);
		}
void SwimStrafeRightBackMode(){
		if((Swim&&StrafeRight&&Back)||Jump)return;
		NULLifyFlags();
		StrafeRight=Swim=Back=true;
		myMesh->setLoopMode(true);
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(SWIMFrames[0],SWIMFrames[1]);
		myMesh->setAnimationSpeed(SwimFrameSpeed);
		}





void SwimStrafeLeftForwardMode(){
		if((Swim&&StrafeLeft&&Walk&&Forward)||Jump)return;
		NULLifyFlags();
		StrafeLeft=Forward=Swim=true;
		myMesh->setLoopMode(true);
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(SWIMFrames[0],SWIMFrames[1]);
		myMesh->setAnimationSpeed(SwimFrameSpeed);
		}
void SwimStrafeRightForwardMode(){
		if((Swim&&StrafeRight&&Walk&&Forward)||Jump)return;
		NULLifyFlags();
		StrafeRight=Forward=Swim=true;
		myMesh->setLoopMode(true);
		myMesh->setLoopMode(1);
		myMesh->setFrameLoop(SWIMFrames[0],SWIMFrames[1]);
		myMesh->setAnimationSpeed(SwimFrameSpeed);
		}



// end animation mode functions

void stepMotion(int milliseconds){
		float sTime=milliseconds/1000.0f;
		vector3df pos1=myMesh->getPosition();
		vector3df tmpv;
		vector3df rot1;
		if(!Jump)JumpDirection=(vector3df(0,0,0));
		rot1= myMesh->getRotation();
		forwardAngle=rot1.Y;
		if(Walk&&Forward){
			vector3df pos1=myMesh->getPosition();
			if(!Crouch){
			tmpv.X=+float(cos(forwardAngle*PI/180)*WalkSpeed);
			tmpv.Z=-float(sin(forwardAngle*PI/180)*WalkSpeed);}
			if(Crouch){
			tmpv.X=+float(cos(forwardAngle*PI/180)*WalkSpeed/4.0f);
			tmpv.Z=-float(sin(forwardAngle*PI/180)*WalkSpeed/4.0f);}

			JumpDirection=tmpv;

			myMesh->setPosition(core::vector3df(pos1.X+tmpv.X*sTime,pos1.Y,pos1.Z+tmpv.Z*sTime));
			myMesh->updateAbsolutePosition();
			}

		if(!Run&&Back){
			vector3df pos1=myMesh->getPosition();
			if(!Crouch){
			tmpv.X=-float(cos(forwardAngle*PI/180)*WalkSpeed);
			tmpv.Z=+float(sin(forwardAngle*PI/180)*WalkSpeed);}
			if(Crouch){
			tmpv.X=-float(cos(forwardAngle*PI/180)*WalkSpeed/4.0f);
			tmpv.Z=+float(sin(forwardAngle*PI/180)*WalkSpeed/4.0f);}

			JumpDirection=tmpv;

			myMesh->setPosition(core::vector3df(pos1.X+tmpv.X*sTime,pos1.Y,pos1.Z+tmpv.Z*sTime));
			myMesh->updateAbsolutePosition();
			}
		if(Run&&Forward){
			vector3df pos1=myMesh->getPosition();
			tmpv.X=+float(cos(forwardAngle*PI/180)*RunSpeed);
			tmpv.Z=-float(sin(forwardAngle*PI/180)*RunSpeed);
			JumpDirection=tmpv;
			myMesh->setPosition(core::vector3df(pos1.X+tmpv.X*sTime,pos1.Y,pos1.Z+tmpv.Z*sTime));
			myMesh->updateAbsolutePosition();}

		if(StrafeLeft){
			vector3df pos1=myMesh->getPosition();
			if(!Crouch){
			tmpv.X=-float(cos(forwardAngle*PI/180+3.1416/2)*WalkSpeed);
			tmpv.Z=float(sin(forwardAngle*PI/180+3.1416/2)*WalkSpeed);}
			if(Crouch){
			tmpv.X=-float(cos(forwardAngle*PI/180+3.1416/2)*WalkSpeed/4.0f);
			tmpv.Z=float(sin(forwardAngle*PI/180+3.1416/2)*WalkSpeed/4.0f);}
			JumpDirection=tmpv;
			myMesh->setPosition(core::vector3df(pos1.X+tmpv.X*sTime,pos1.Y,pos1.Z+tmpv.Z*sTime));
			myMesh->updateAbsolutePosition();}

		if(StrafeRight){
			vector3df pos1=myMesh->getPosition();
			if(!Crouch){
			tmpv.X=float(cos(forwardAngle*PI/180+3.1416/2)*WalkSpeed);
			tmpv.Z=-float(sin(forwardAngle*PI/180+3.1416/2)*WalkSpeed);}
			if(Crouch){
			tmpv.X=float(cos(forwardAngle*PI/180+3.1416/2)*WalkSpeed/4.0f);
			tmpv.Z=-float(sin(forwardAngle*PI/180+3.1416/2)*WalkSpeed/4.0f);}
			JumpDirection=tmpv;
			myMesh->setPosition(core::vector3df(pos1.X+tmpv.X*sTime,pos1.Y,pos1.Z+tmpv.Z*sTime));
			myMesh->updateAbsolutePosition();
			}
		ApplyGravity(milliseconds);
		}
};




class WASDMouseBPControl{
public:
	IPlayerBP *MyBP;
	int Key_For_Walk;
	int Key_For_Back;
	int Key_For_Left;
	int Key_For_Right;
	int Key_For_Jump;
	int Key_For_Crouch;
	int Key_For_Run;
	bool Crouch_Key_Down;

WASDMouseBPControl(){
	Key_For_Walk=KEY_KEY_W;
	Key_For_Back=KEY_KEY_S;
	Key_For_Left=KEY_KEY_A;
	Key_For_Right=KEY_KEY_D;
	Key_For_Jump=KEY_SPACE;
	Key_For_Crouch=KEY_KEY_C;
	Key_For_Run=KEY_SHIFT;
	}

void SetWalkKey(int key1){
	Key_For_Walk=key1;
}
void SetBackKey(int key1){
	Key_For_Back=key1;
}
void SetLeftKey(int key1){
	Key_For_Left=key1;
}
void SetRightKey(int key1){
	Key_For_Right=key1;
}
void SetJumpKey(int key1){
	Key_For_Jump=key1;
}
void SetCrouchKey(int key1){
	Key_For_Crouch=key1;
}
void SetRunKey(int key1){
	Key_For_Run=key1;
}
void attachPlayerBP(IPlayerBP *BPtoAttach){
		MyBP=BPtoAttach;
		}
	
void Tick(int msecs){
		
		if(!Crouch_Key_Down&&Keys_Down[Key_For_Crouch]){
			if(MyBP->Crouch==true){	
				MyBP->StandMode();
				}
				else{
					MyBP->CrouchMode();
					}
				Crouch_Key_Down=true;
			}

		if(!Keys_Down[Key_For_Crouch])Crouch_Key_Down=false;

		if(MBRIGHT||MBLEFT) //if either mouse buttons pressed then hide cursor
		device->getCursorControl()->setVisible(false);
		
		if(!MBRIGHT&&!MBLEFT) //if no mouse buttons pressed then show cursor
			device->getCursorControl()->setVisible(true);

		if((!Keys_Down[Key_For_Left]&&!Keys_Down[Key_For_Right])&&(MyBP->StrafeLeft||MyBP->StrafeRight))
			MyBP->NULLifyFlags();

		// if NO controls pressed..
		if(!Keys_Down[Key_For_Walk]&&!Keys_Down[Key_For_Back]&&!Keys_Down[Key_For_Left]&&!Keys_Down[Key_For_Right]&&!Keys_Down[Key_For_Crouch]&&(!MBRIGHT||!MBLEFT)){
			if(MyBP->Crouch&&!(MyBP->myMesh->getFrameNr()>=MyBP->CROUCHFrames[0]&&MyBP->myMesh->getFrameNr()<=MyBP->CROUCHFrames[1]))
			MyBP->Freeze();
			if(!MyBP->Crouch&&!MyBP->Jump)
				MyBP->IdleMode();
			}
		
		// if Walking controls pressed...
		if((Keys_Down[Key_For_Walk]||(MBRIGHT&&MBLEFT))&&!(Keys_Down[Key_For_Back])&&!(Keys_Down[Key_For_Left])&&!(Keys_Down[Key_For_Right])){
			if(MyBP->Crouch==false){
				if(Keys_Down[Key_For_Run]){
					MyBP->RunForwardMode();}
				else {MyBP->WalkForwardMode();}
				}
			else{MyBP->WalkForwardCrouchMode();}
			}

		// if Walk Back Controls pressed...
		if(Keys_Down[Key_For_Back]&&!(MBLEFT&&MBRIGHT)&&!(Keys_Down[Key_For_Walk])&&!(Keys_Down[Key_For_Left])&&!(Keys_Down[Key_For_Right])){
			if(MyBP->Crouch==false){
				MyBP->WalkBackMode();}
			else{MyBP->WalkBackCrouchMode();}
			}
	
		// if Strafe Right AND/OR Forward
	if((Keys_Down[Key_For_Right]&&MBRIGHT)&&!Keys_Down[Key_For_Left]&&!Keys_Down[Key_For_Back]){// add strafe key	// Strafe controls
			if(MyBP->Crouch==false){
				if(Keys_Down[Key_For_Walk]||(MBLEFT&&MBRIGHT)){
					MyBP->StrafeRightForwardMode();}
				else{
					MyBP->Walk=false;
					MyBP->StrafeRightMode();}}
			else{
				if(Keys_Down[Key_For_Walk]||(MBLEFT&&MBRIGHT)){
					MyBP->StrafeRightForwardCrouchMode();}
				else{
					MyBP->Walk=false;
					MyBP->StrafeRightCrouchMode();}
				}
			}

		//if Strafe Left AND/OR forward
		if((Keys_Down[Key_For_Left]&&MBRIGHT)&&!Keys_Down[Key_For_Right]&&!Keys_Down[Key_For_Back]){// add strafe key	// Strafe controls
			if(MyBP->Crouch==false){
				if(Keys_Down[Key_For_Walk]||(MBLEFT&&MBRIGHT)){
					MyBP->StrafeLeftForwardMode();}
				else{
					MyBP->Walk=false;
					MyBP->StrafeLeftMode();}}
			else{
				if(Keys_Down[Key_For_Walk]||(MBLEFT&&MBRIGHT)){
					MyBP->StrafeLeftForwardCrouchMode();}
				else{
					MyBP->Walk=false;
					MyBP->StrafeLeftCrouchMode();}
				}
			}

		
		// if Strafe Right AND/OR Back
		if((Keys_Down[Key_For_Right]&&!Keys_Down[Key_For_Left]&&MBRIGHT)&&!Keys_Down[Key_For_Walk]&&!(MBLEFT&&MBRIGHT)){// add strafe key	// Strafe controls
			if(MyBP->Crouch==false){
				if(Keys_Down[Key_For_Back]){
					MyBP->StrafeRightBackMode();}
				else{
					MyBP->Back=false;
					MyBP->StrafeRightMode();}
				}
			else{
				if(Keys_Down[Key_For_Back]){
					MyBP->StrafeRightBackCrouchMode();}
				else{
					MyBP->Back=false;
					MyBP->StrafeRightCrouchMode();}
				}
			}

		//if Strafe Left AND/OR Back
		if((Keys_Down[Key_For_Left]&&MBRIGHT)&&!Keys_Down[Key_For_Right]&&!Keys_Down[Key_For_Walk]&&!(MBLEFT&&MBRIGHT)){// add strafe key	// Strafe controls
			if(MyBP->Crouch==false){
				if(Keys_Down[Key_For_Back]){
					MyBP->StrafeLeftBackMode();}
				else{
					MyBP->Back=false;
					MyBP->StrafeLeftMode();}
				}
			else{
				if(Keys_Down[Key_For_Back]){
					MyBP->StrafeLeftBackCrouchMode();}
				else{
					MyBP->Back=false;
					MyBP->StrafeLeftCrouchMode();}
				}
			}

		



		if(MBRIGHT||MBLEFT&&(MOUSEY!=OLDMPOSY)){
			if(device!=0)device->getCursorControl()->setPosition(OLDMPOSX,OLDMPOSY);
				}

		if(MBRIGHT&&(MOUSEX!=OLDMPOSX)){
			MyBP->Turn(((MOUSEX-OLDMPOSX)/120.0f)*57);
			if(device!=0)device->getCursorControl()->setPosition(OLDMPOSX,OLDMPOSY);
			}

		if(MBLEFT==1&&MBRIGHT==0&&(MOUSEX!=OLDMPOSX)){
			if(device!=0)device->getCursorControl()->setPosition(OLDMPOSX,OLDMPOSY);
			}

		if(Keys_Down[Key_For_Left]==1&&MBRIGHT==0){
			MyBP->Turn(-7.695);
			}

		if(Keys_Down[Key_For_Right]==1&&MBRIGHT==0){
			MyBP->Turn(7.695);
			}
		if(Keys_Down[Key_For_Jump]){
			MyBP->JumpMode();
			}
		if(MWHEEL!=0){
			}
	MyBP->stepMotion(msecs);


	}

};

