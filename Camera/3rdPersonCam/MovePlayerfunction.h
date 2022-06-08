#include "EventReceiver.h"
#include "PlayerControlClass.h"
#include "I3rdPersonCam.h"


I3rdPersonCam My3rdCam1;
IPlayerBP MainPlayer;
IPlayerBP BP1,BP2,BP3,BP4,BP5;
WASDMouseBPControl WASD1;

float CamPan=0;
float CamTilt=-80;
float CamZoom=100;

int PlayerSel=1;

void MovePlayer(float ElapsedTime){
if(Keys_Down[KEY_KEY_T]==1){
	PlayerSel+=1;
	if(PlayerSel>5)PlayerSel=1;
	MainPlayer.StandMode();
	
	if(PlayerSel==1){
		MainPlayer.attachMesh(Player1);
		WASD1.attachPlayerBP(&MainPlayer);
		My3rdCam1.setTarget(Player1);
		}
	if(PlayerSel==2){
		MainPlayer.attachMesh(Player2);
		WASD1.attachPlayerBP(&MainPlayer);
		My3rdCam1.setTarget(Player2);
		}
	if(PlayerSel==3){
		MainPlayer.attachMesh(Player3);
		WASD1.attachPlayerBP(&MainPlayer);
		My3rdCam1.setTarget(Player3);
		}
	if(PlayerSel==4){
		MainPlayer.attachMesh(Player4);
		WASD1.attachPlayerBP(&MainPlayer);
		My3rdCam1.setTarget(Player4);
		}
	if(PlayerSel==5){
		MainPlayer.attachMesh(Player5);
		WASD1.attachPlayerBP(&MainPlayer);
		My3rdCam1.setTarget(Player5);
		}
	Keys_Down[KEY_KEY_T]=0;
	My3rdCam1.updatePosition();
	}



// Use data from event receiver to adjust camera orientation
if(MBRIGHT||MBLEFT)
		device->getCursorControl()->setVisible(false);
		if(!MBRIGHT&&!MBLEFT)
			device->getCursorControl()->setVisible(true);

			if(MBRIGHT||MBLEFT&&(MOUSEY!=OLDMPOSY)){
					CamTilt=CamTilt-(MOUSEY-OLDMPOSY)/6.0f;
					if(CamTilt<-89)CamTilt=-89;
					if(CamTilt>89)CamTilt=89;
					if(device!=0)device->getCursorControl()->setPosition(OLDMPOSX,OLDMPOSY);
					}
			if(MBRIGHT&&(MOUSEX!=OLDMPOSX)){
					if(device!=0)device->getCursorControl()->setPosition(OLDMPOSX,OLDMPOSY);
					}
			if(MBLEFT==1&&MBRIGHT==0&&(MOUSEX!=OLDMPOSX)){
					CamPan=CamPan+(MOUSEX-OLDMPOSX)/3.0f;
					if(device!=0)device->getCursorControl()->setPosition(OLDMPOSX,OLDMPOSY);
					}

		if(MWHEEL!=0){
			CamZoom=CamZoom+MWHEEL*5;
			MWHEEL=0;
			//if(CamZoom<5)CamZoom=5;
			//if(CamZoom>400)CamZoom=400;
			}
WASD1.Tick(ElapsedTime);
My3rdCam1.setOrientation(CamPan,CamTilt,CamZoom);
My3rdCam1.updatePosition();

}