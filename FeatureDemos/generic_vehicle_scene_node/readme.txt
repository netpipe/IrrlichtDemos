Updated: 21st May, 2012

A real car pitches when accelerates or brakes. It also rolls to one side on turning a corner. 
To add more realism in controlling the vehicle, new functions are added:

	void pitchBody(f32 degs);
	void rollBody(f32 degs);
	void rotate(scene::ISceneNode* node, const core::vector3df& rot);
	void resetPitch();
	
After setting up the vehicle, press 'A', 'D' to steer it and it will roll to one side.
Press 'W', 'S' to change speed and the vehicle will pitch accordingly.

Changed files:
main.cpp
vehicle_scene_node.h
vehicle_scene_node.cpp

See the first post for an updated archive file.


To compile
==========
Change parameters to point to the proper directories:

main.cpp:
#define IRRLICHT_DIR "/home/smso/downloads/irrlicht/irrlicht-svn/"

makefile:
IRRLICHT_DIR = /home/smso/downloads/irrlicht/irrlicht-svn


To start the editor
===================
Run:
./r


Use of the editor
=================
1. Load a body mesh in the scene.
2. Change the body rotation so that it is in the z-direction (as indicated by the blue line).
3. Load a tire mesh.
4. Roughly scale the tires to match with the body.
5. Adjust the tire offsets and tire rotations accordingly.
6. Repeat steps 4 and 5 to refine.
7. Save the params.


