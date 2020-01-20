IPhone Touch Events for Irrlicht

These files add iPhone Touch events to the Irrlicht engine. The main changes are adding Touch event types to IEventReceiver.h and ObjectiveC methods to IrrPhoneView class in CIrrDeviceIPhone.m. 

Main files:

irrlicht.h		- Adds include for ITouchControl.h
IEventReceiver.h	- Touch Event Types
ITouchControl.h		- Touch control modeled after the mouse control interface

CIrrDeviceIPhone.h	- Adds CTouchControl to the CIrrDeviceIPhone class
CIrrDeviceIPhone.m	- Adds touch methods to IrrPhoneView class 
CIrrDeviceIPhone.cpp    - Adds methods to support CTouchControl and to receive and post the Touch events


Demostration files:

ISceneNodeAnimatorCameraTouch.h
SceneNodeAnimatorCameraTouch.h
SceneNodeAnimatorCameraTouch.cpp

The SceneNodeAnimatorCameraTouch files are a sample for testing the Touch events. 


Thanks,
John Hatch (Rockfish)
john@rockfishnw.com

