/* A flythrough camera with a predefined path */

/*
    Copyright © 2011 John Serafino
    This file is part of The Gibson Screensaver.

    The Gibson Screensaver is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Gibson Screensaver is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with The Gibson Screensaver If not, see <http://www.gnu.org/licenses/>.
*/

#include "cam.h"
//#include "path.h"

int CAM_MOVEMENT_SPEED;

void FlyCam::init()
{
	// HA! I'm commenting out the comments. Did you know G++ doesn't forbid such idiocy?
	/*
	SKeyMap keyMap[8];
	keyMap[0].Action = EKA_MOVE_FORWARD;
	keyMap[0].KeyCode = KEY_UP;
	keyMap[1].Action = EKA_MOVE_FORWARD;
	keyMap[1].KeyCode = KEY_KEY_W;

	keyMap[2].Action = EKA_MOVE_BACKWARD;
	keyMap[2].KeyCode = KEY_DOWN;
	keyMap[3].Action = EKA_MOVE_BACKWARD;
	keyMap[3].KeyCode = KEY_KEY_S;

	keyMap[4].Action = EKA_STRAFE_LEFT;
	keyMap[4].KeyCode = KEY_LEFT;
	keyMap[5].Action = EKA_STRAFE_LEFT;
	keyMap[5].KeyCode = KEY_KEY_A;

	keyMap[6].Action = EKA_STRAFE_RIGHT;
	keyMap[6].KeyCode = KEY_RIGHT;
	keyMap[7].Action = EKA_STRAFE_RIGHT;
	keyMap[7].KeyCode = KEY_KEY_D;

	cam.camera = Scene->addCameraSceneNodeFPS(0, 100, 0.05, -1, keyMap, 8);


	empty = Scene->addEmptySceneNode(0);
	return;
	*/

	CAM_MOVEMENT_SPEED = 20;

	//cam.init(0,50,0);
	//cam.setRotation(90,0,0);

	//cam.init(0,8,TOWER_DIST);


	/* begin path block */
	/*
	camPath.push_back(core::vector3df(-1.65535,16.4066,27.0081));
	camPath.push_back(core::vector3df(-1.46174,14.418,83.7388));
	camPath.push_back(core::vector3df(-1.53489,11.927,137.328));
	camPath.push_back(core::vector3df(-1.57689,10.6151,168.1));
	camPath.push_back(core::vector3df(2.51616,10.0309,187.574));
	camPath.push_back(core::vector3df(38.6503,8.76926,195.517));
	camPath.push_back(core::vector3df(64.4084,7.43277,196.084));
	camPath.push_back(core::vector3df(94.4781,6.16091,195.639));
	camPath.push_back(core::vector3df(127.04,6.31556,192.858));
	camPath.push_back(core::vector3df(189.092,12.2842,192.678));
	camPath.push_back(core::vector3df(248.532,20.1126,194.397));
	camPath.push_back(core::vector3df(289.983,32.4544,191.674));
	camPath.push_back(core::vector3df(317.428,42.4382,181.697));
	camPath.push_back(core::vector3df(348.218,54.0866,168.817));
	camPath.push_back(core::vector3df(351.262,48.901,151.692));
	camPath.push_back(core::vector3df(358.406,35.5472,109.64));
	camPath.push_back(core::vector3df(361.467,22.4387,65.2222));
	camPath.push_back(core::vector3df(360.426,14.2694,13.4877));
	camPath.push_back(core::vector3df(359.761,9.68527,-49.8711));
	camPath.push_back(core::vector3df(321.92,7.98058,-74.6904));
	camPath.push_back(core::vector3df(290.25,8.60577,-75.626));
	camPath.push_back(core::vector3df(255.362,7.86445,-75.383));
	camPath.push_back(core::vector3df(232.87,7.25111,-75.3222));
	camPath.push_back(core::vector3df(202.876,7.17029,-75.2944));
	camPath.push_back(core::vector3df(142.359,14.3787,-74.8111));
	camPath.push_back(core::vector3df(112.661,16.7787,-74.8154));
	camPath.push_back(core::vector3df(46.8683,18.3976,-76.3296));
	camPath.push_back(core::vector3df(17.0815,20.0776,-74.573));
	camPath.push_back(core::vector3df(-11.0875,20.5345,-75.7801));
	camPath.push_back(core::vector3df(-72.439,19.853,-77.689));
	camPath.push_back(core::vector3df(-121.411,20.0191,-75.2218));
	camPath.push_back(core::vector3df(-186.267,20.2409,-72.9332));
	camPath.push_back(core::vector3df(-232.887,20.4743,-77.8075));
	camPath.push_back(core::vector3df(-240.184,17.7214,-47.5505));
	camPath.push_back(core::vector3df(-240.035,17.4427,-18.0545));
	camPath.push_back(core::vector3df(-210.592,15.5777,-14.4624));
	camPath.push_back(core::vector3df(-210.543,16.4641,22.5998));
	camPath.push_back(core::vector3df(-210.423,16.7619,79.445));
	camPath.push_back(core::vector3df(-209.948,16.8259,98.1388));
	camPath.push_back(core::vector3df(-208.186,17.1775,157.411));
	camPath.push_back(core::vector3df(-206.602,15.6615,195.72));
	camPath.push_back(core::vector3df(-171.683,14.3715,195.852));
	camPath.push_back(core::vector3df(-113.689,14.6884,196.147));
	camPath.push_back(core::vector3df(-81.2432,14.6175,196.682));
	camPath.push_back(core::vector3df(-49.4481,10.4671,195.639));
	camPath.push_back(core::vector3df(-34.0226,11.3448,194.765));
	camPath.push_back(core::vector3df(-29.5613,10.9841,170.144));
	camPath.push_back(core::vector3df(-28.5414,10.749,139.163));
	camPath.push_back(core::vector3df(-27.6715,11.0805,97.9237));
	camPath.push_back(core::vector3df(-27.1759,11.1615,64.5778));
	camPath.push_back(core::vector3df(-26.7453,11.1727,21.0265));
	camPath.push_back(core::vector3df(-1.21105,14.3913,7.50106));
	camPath.push_back(core::vector3df(-1.65535,16.4066,27.0081));
*/

	camPath.push_back(core::vector3df(0.0392192,6.95062,32.1667));
	camPath.push_back(core::vector3df(0.13452,6.5646,55.4633));
	camPath.push_back(core::vector3df(0.189138,6.72459,96.8121));
	camPath.push_back(core::vector3df(-0.139794,7.21804,145.059));
	camPath.push_back(core::vector3df(-0.489681,16.2712,186.951));
	camPath.push_back(core::vector3df(-0.401056,16.7698,219.447));
	camPath.push_back(core::vector3df(-0.291434,17.3865,259.642));
	camPath.push_back(core::vector3df(-0.52368,17.9816,308.385));
	camPath.push_back(core::vector3df(8.69042,17.3182,313.72));
	camPath.push_back(core::vector3df(59.6782,15.8175,314.299));
	camPath.push_back(core::vector3df(60.4089,15.0665,278.267));
	camPath.push_back(core::vector3df(60.5837,15.495,232.322));
	camPath.push_back(core::vector3df(60.7141,15.5494,200.472));
	camPath.push_back(core::vector3df(86.3944,14.8672,195.875));
	camPath.push_back(core::vector3df(131.81,13.8411,196.464));
	camPath.push_back(core::vector3df(159.956,20.7447,194.651));
	camPath.push_back(core::vector3df(216.099,37.5139,192.33));
	camPath.push_back(core::vector3df(258.068,50.0952,180.791));
	camPath.push_back(core::vector3df(266.522,48.8569,150.402));
	camPath.push_back(core::vector3df(270.506,33.0024,93.9888));
	camPath.push_back(core::vector3df(272.414,17.6326,35.1101));
	camPath.push_back(core::vector3df(272.369,17.7922,-8.18755));
	camPath.push_back(core::vector3df(269.593,18.0043,-39.8612));
	camPath.push_back(core::vector3df(229.488,19.8768,-43.4609));
	camPath.push_back(core::vector3df(187.706,19.0659,-44.2737));
	camPath.push_back(core::vector3df(150.242,23.1816,-42.1108));
	camPath.push_back(core::vector3df(149.888,18.2483,-9.74219));
	camPath.push_back(core::vector3df(149.797,22.5784,30.3554));
	camPath.push_back(core::vector3df(149.408,26.4205,71.9536));
	camPath.push_back(core::vector3df(148.835,30.2287,110.622));
	camPath.push_back(core::vector3df(149.819,19.2602,150.008));
	camPath.push_back(core::vector3df(148.399,16.7217,195.691));
	camPath.push_back(core::vector3df(104.66,17.6327,195.331));
	camPath.push_back(core::vector3df(66.8026,14.1855,195.546));
	camPath.push_back(core::vector3df(32.9041,14.0211,195.275));
	camPath.push_back(core::vector3df(31.9278,12.607,135.516));
	camPath.push_back(core::vector3df(30.6969,9.92016,38.4132));
	camPath.push_back(core::vector3df(28.3579,10.8269,16.8861));
	camPath.push_back(core::vector3df(2.34459,9.89253,17.0397));
	camPath.push_back(core::vector3df(0.0392192,6.95062,32.1667));
	camPath.push_back(core::vector3df(0.0392192,6.95062,32.1667));

	/* end path block */

	cam.init(0,0,0);
	empty = Scene->addEmptySceneNode(0);
	//empty->addChild(cam.camera);
	empty->setPosition(camPath[0]);
	cam.camera->setPosition(camPath[0]);

	//cout << camSpline[0].X << ' ' << camSpline[0].Y << ' ' << camSpline[0].Z << '\n';

	//anim = Scene->createFollowSplineAnimator(Ray.getTime(), camSpline, 1.0f, 0.5f, false);
	anim = Scene->createFollowSplineAnimator(Ray.getTime(), camPath, 0.9f, 0.5f, true, true);

	//cam.camera->addAnimator(anim);
	empty->addAnimator(anim);

	anim = Scene->createFollowSplineAnimator(Ray.getTime() + 150, camPath, 0.9f, 0.5f, true, true);

	cam.camera->addAnimator(anim);
	anim->drop();
	anim=0;

	cam.update();

	/*

	//cam.init(109.125,0,0);
	//cam.setRotation(90,0,0);

	movement =
	{
			vector3df(0,8,TOWER_DIST/2),
			vector3df(0,8,TOWER_DIST*8.5),
			vector3df(TOWER_DIST*6,8,TOWER_DIST*8.5),
			vector3df(TOWER_DIST*7.5,20,TOWER_DIST*8.25),
			vector3df(TOWER_DIST*8.25,20,TOWER_DIST*8.25),
			vector3df(TOWER_DIST*8.25,20,TOWER_DIST*9),
			vector3df(TOWER_DIST*8,20,TOWER_DIST*10),
			vector3df(TOWER_DIST*8,TOWER_HEIGHT/2,TOWER_DIST*10),
			vector3df(TOWER_DIST*8,TOWER_HEIGHT,TOWER_DIST*10),
			vector3df(TOWER_DIST*9,TOWER_HEIGHT/2,TOWER_DIST*10.5),
			vector3df(TOWER_DIST*10,8,TOWER_DIST*11)
	};

	rotation =
	{
			vector3df(0,0,0),
			vector3df(0,0,0),
			vector3df(0,90,0),
			vector3df(0,0,0),
			vector3df(0,-45,0),
			vector3df(0,-90,0),
			vector3df(0,0,0),
			vector3df(30,30,0),
			vector3df(45,45,0),
			vector3df(30,30,0),
			vector3df(0,0,0)
	};

	velocity =
	{
			30,
			30,
			30,
			30,
			30,
			30,
			30,
			30,
			30,
			30,
			30
	};


	movementIndex = 0;
	setRot = vector3di(1,1,1);

	prevPos = cam.camera->getAbsolutePosition();
	prevRot = cam.camera->getRotation();
	prevVel = CAM_MOVEMENT_SPEED;
	*/
}

void FlyCam::update()
{
/*

	prevVel = CAM_MOVEMENT_SPEED;

	prevPos = cam.camera->getAbsolutePosition();

	if(movementIndex == INDEX_LENGTH)
		movementIndex = 0;


	if(CAM_MOVEMENT_SPEED > velocity[movementIndex])
	{
		CAM_MOVEMENT_SPEED -= (delta * CAM_MOVEMENT_SPEED * SPEEDUP_DAMP);

		if(prevVel > velocity[movementIndex] && CAM_MOVEMENT_SPEED < velocity[movementIndex])
		{
			CAM_MOVEMENT_SPEED = velocity[movementIndex];
		}
	}
	if(CAM_MOVEMENT_SPEED < velocity[movementIndex])
	{
		CAM_MOVEMENT_SPEED += (delta * CAM_MOVEMENT_SPEED * SPEEDUP_DAMP);

		if(prevVel < velocity[movementIndex] && CAM_MOVEMENT_SPEED > velocity[movementIndex])
		{
			CAM_MOVEMENT_SPEED = velocity[movementIndex];
		}
	}


	if(movement[movementIndex].X > cam.x)
	{
		cam.translateGlobal(delta * CAM_MOVEMENT_SPEED,0,0);
		cam.update();

		if(prevPos.X < movement[movementIndex].X && cam.x > movement[movementIndex].X)
		{
			cam.setPosition(movement[movementIndex].X,cam.y,cam.z);
		}
	}
	if(movement[movementIndex].X < cam.x)
	{
		cam.translateGlobal(-(delta * CAM_MOVEMENT_SPEED),0,0);
		cam.update();

		if(prevPos.X > movement[movementIndex].X && cam.x < movement[movementIndex].X)
		{
			cam.setPosition(movement[movementIndex].X,cam.y,cam.z);
		}
	}


	if(movement[movementIndex].Y > cam.y)
	{
		cam.translateGlobal(0,delta * CAM_MOVEMENT_SPEED,0);
		cam.update();

		if(prevPos.Y < movement[movementIndex].Y && cam.y > movement[movementIndex].Y)
		{
			cam.setPosition(cam.x,movement[movementIndex].Y,cam.z);
		}
	}
	if(movement[movementIndex].Y < cam.y)
	{
		cam.translateGlobal(0,-(delta * CAM_MOVEMENT_SPEED),0);
		cam.update();

		if(prevPos.Y > movement[movementIndex].Y && cam.y < movement[movementIndex].Y)
		{
			cam.setPosition(cam.x,movement[movementIndex].Y,cam.z);
		}
	}

	if(movement[movementIndex].Z > cam.z)
	{
		cam.translateGlobal(0,0,delta * CAM_MOVEMENT_SPEED);
		cam.update();

		if(prevPos.Z < movement[movementIndex].Z && cam.z > movement[movementIndex].Z)
		{
			cam.setPosition(cam.x,cam.y,movement[movementIndex].Z);
		}
	}
	if(movement[movementIndex].Z < cam.z)
	{
		cam.translateGlobal(0,0,-(delta * CAM_MOVEMENT_SPEED));
		cam.update();

		if(prevPos.Z > movement[movementIndex].Z && cam.z < movement[movementIndex].Z)
		{
			cam.setPosition(cam.x,cam.y,movement[movementIndex].Z);
		}
	}

	if(rotation[movementIndex].X > cam.rx)
	{
		cam.rotateGlobal(delta * CAM_ROTATION_SPEED,0,0);

		if((rotation[movementIndex].X - cam.rx) < INDEX_PRECISION)
			cam.rotateGlobal(rotation[movementIndex].X - cam.rx,0,0);
	}
	if(rotation[movementIndex].X < cam.rx)
	{
		cam.rotateGlobal(-(delta * CAM_ROTATION_SPEED), 0,0);

		if((cam.rx - rotation[movementIndex].X) < INDEX_PRECISION)
			cam.rotateGlobal(cam.rx - rotation[movementIndex].X,0,0);
	}

	prevRot = cam.camera->getRotation();

	if(rotation[movementIndex].X > cam.rx)
	{
		cam.rotateGlobal(delta * CAM_ROTATION_SPEED,0,0);
		cam.update();

		setRot.X = 0;

		if(prevRot.X < rotation[movementIndex].X && cam.rx > rotation[movementIndex].X)
		{
			cam.setRotation(rotation[movementIndex].X,cam.ry,cam.rz);
			setRot.X = 1;
		}
	}
	if(rotation[movementIndex].X < cam.rx)
	{
		cam.rotateGlobal(-(delta * CAM_ROTATION_SPEED),0,0);
		cam.update();

		setRot.X = 0;

		if(prevRot.X > rotation[movementIndex].X && cam.rx < rotation[movementIndex].X)
		{
			cam.setRotation(rotation[movementIndex].X,cam.ry,cam.rz);
			setRot.X = 1;
		}
	}

	if(rotation[movementIndex].Y > cam.ry)
	{
		cam.rotateGlobal(0,delta * CAM_ROTATION_SPEED,0);
		cam.update();

		setRot.Y = 0;

		if(prevRot.Y < rotation[movementIndex].Y && cam.ry > rotation[movementIndex].Y)
		{
			cam.setRotation(cam.rx,rotation[movementIndex].Y,cam.rz);
			setRot.Y = 1;
		}
	}
	if(rotation[movementIndex].Y < cam.ry)
	{
		cam.rotateGlobal(0,-(delta * CAM_ROTATION_SPEED),0);
		cam.update();

		setRot.Y = 0;

		if(prevRot.Y > rotation[movementIndex].Y && cam.ry < rotation[movementIndex].Y)
		{
			cam.setRotation(cam.rx,rotation[movementIndex].Y,cam.rz);
			setRot.Y = 1;
		}
	}

	if(rotation[movementIndex].Z > cam.rz)
	{
		cam.rotateGlobal(0,0,delta * CAM_ROTATION_SPEED);
		cam.update();

		setRot.Z = 0;

		if(prevRot.Z < rotation[movementIndex].Z && cam.rz > rotation[movementIndex].Z)
		{
			cam.setRotation(cam.rx,cam.ry,rotation[movementIndex].Z);
			setRot.Z = 1;
		}
	}
	if(rotation[movementIndex].Z < cam.rz)
	{
		cam.rotateGlobal(0,0,-(delta * CAM_ROTATION_SPEED));
		cam.update();

		setRot.Z = 0;

		if(prevRot.Z > rotation[movementIndex].Z && cam.rz < rotation[movementIndex].Z)
		{
			cam.setRotation(cam.rx,cam.ry,rotation[movementIndex].Z);
			setRot.Z = 1;
		}
	}



	cam.update();
	if(movement[movementIndex] == cam.camera->getAbsolutePosition() && \
			cam.camera->getRotation() == rotation[movementIndex] && \
			velocity[movementIndex] == CAM_MOVEMENT_SPEED)
	{
		movementIndex++;

		cout << "Camera-> finished tranformation index # " << movementIndex << "\n";
	}

	cam.update();
	*/
}

