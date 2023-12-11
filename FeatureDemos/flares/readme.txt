Torleif (Pippy) changed Xeniczone's camera flare for irrlicht v1.4.1

Changes: 
  Now node remmbers starting point, so you can set it to move with the camera
  more comments
  setTexture(), not accessing raw array
  force converting primative types so no warnings

To use it:

// #1 copy source and flares.jpg into your source folder (or change "flares.jpg" to where art is)
// #2 put this in your includes with all the other #includes:
#include "CLensFlareSceneNode.h"


// #3 put this when loading your files (after driver init)
ILensFlareSceneNode* flare = new CLensFlareSceneNode(NULL, smgr, -1, vector3df(0,0,0));
flare->getMaterial(0).setTexture(0, driver->getTexture("flares.jpg"));

// #4 Put this in the main while(){ loop }:
flare->render();

// #5 if you want to move with the camera (ie, the sun dosn't change positions) change 'cam' to your camera
flare->setPosition(flare->getStartPosition() + cam.getPosition());



Hint: to get it to align with a sky box use step 5 and play around with the constructors vector3df(0,0,0) untill it lines up


You don't need the comments. They're there in case someone gets to exited with the mouse. 

There probally is a smater way of rendering (by putting it in the scene manager), But I'm too lazy and like to see my code working.

Enjoy!