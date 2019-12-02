int WINAPI charactersFunc()
{
	//handles everything with other characters includes players and npc's
	//new characters will be added from the network thread
	//

	// time
	f32 time = 0;
	f32 oldtime;
	// time between frames
	f32 deltaT;
	printf("characters thread started");
	while(runloop){
		// calculate time between frames
		oldtime = time;
		time = (f32)timer->getTime() / 1000.0f;
		deltaT = -1*(time - oldtime);
		//loop through all charsj
		for(int x=0; x<charCount;x++)
		{
			//if the slot is in use
			if(charDat[x].id>0&&charDat[x].loaded&&!charDat[x].buildModel)
			{
				//movement
				if(charDat[x].mov1)
				{
					printf("forward");
					vector3df playerrotation = charDat[x].playerModel->getRotation();
					int rotationtotal=playerrotation.X+playerrotation.Y+playerrotation.Z;
					vector3df newposition;
					newposition.Z=sin(playerrotation.Y*PI/180)*charDat[x].speed*deltaT;//px*speed;
					newposition.X=-cos(playerrotation.Y*PI/180)*charDat[x].speed*deltaT;//py*speed;
					newposition.Y=0;//pz*speed;
					charDat[x].playerModel->setPosition(charDat[x].playerModel->getPosition()+newposition);
				}
				if(charDat[x].mov2)
				{
					printf("left");
					vector3df playerrotation = charDat[x].playerModel->getRotation();
					int rotationtotal=playerrotation.X+playerrotation.Y+playerrotation.Z;
					vector3df newposition;
					newposition.Z=sin((playerrotation.Y+270)*PI/180)*charDat[x].speed*deltaT;//px*speed;
					newposition.X=-cos((playerrotation.Y+270)*PI/180)*charDat[x].speed*deltaT;//py*speed;
					newposition.Y=0;//pz*speed;
					charDat[x].playerModel->setPosition(charDat[x].playerModel->getPosition()+newposition);
				}
				if(charDat[x].mov3)
				{
					printf("right");
					vector3df playerrotation = charDat[x].playerModel->getRotation();
					int rotationtotal=playerrotation.X+playerrotation.Y+playerrotation.Z;
					vector3df newposition;
					newposition.Z=sin((playerrotation.Y+180)*PI/180)*charDat[x].speed*deltaT;//px*speed;
					newposition.X=-cos((playerrotation.Y+180)*PI/180)*charDat[x].speed*deltaT;//py*speed;
					newposition.Y=0;//pz*speed;
					charDat[x].playerModel->setPosition(charDat[x].playerModel->getPosition()+newposition);
				}
				if(charDat[x].mov4)
				{
					printf("back");
					vector3df playerrotation = charDat[x].playerModel->getRotation();
					int rotationtotal=playerrotation.X+playerrotation.Y+playerrotation.Z;
					vector3df newposition;
					newposition.Z=sin((playerrotation.Y+90)*PI/180)*charDat[x].speed*deltaT;//px*speed;
					newposition.X=-cos((playerrotation.Y+90)*PI/180)*charDat[x].speed*deltaT;//py*speed;
					newposition.Y=0;//pz*speed;
					charDat[x].playerModel->setPosition(charDat[x].playerModel->getPosition()+newposition);
				}
				if(charDat[x].rotationDir!=0)
				{
					printf("rotate");
					if(charDat[x].rotationDir==1)
					{
						charDat[x].rotation+=charDat[x].rotateSpeed*deltaT;
					}else if(charDat[x].rotationDir==2)
					{
						charDat[x].rotation-=charDat[x].rotateSpeed*deltaT;
					}
					if(charDat[x].rotation<0)
					{
						charDat[x].rotation=360+charDat[x].rotation;
					}else if(charDat[x].rotation>360)
					{
						charDat[x].rotation=charDat[x].rotation-360;
					}
					charDat[x].playerModel->setRotation(vector3df(0,charDat[x].rotation,0));
				}
				//server wants to reset position
				/*if(charDat[x].changePosition==1)
				{
					//need maps to be done
				}
				//server wants to change animations
				if(charDat[x].changeAction==1)
				{
					//need animation file and complete model
				}*/
			}
		}
	}
	return 0;
}
