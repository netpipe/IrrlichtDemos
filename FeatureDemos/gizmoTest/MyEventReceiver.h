#pragma once

#include <irrlicht.h> 

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;

#define center_axis 0
#define x_axis 1
#define y_axis 2
#define z_axis 3
#define selectedNode 4

class MyEventReceiver :
   public IEventReceiver
{
public:
   MyEventReceiver(IrrlichtDevice *device);
   ~MyEventReceiver(void);

   virtual bool OnEvent(const SEvent& event);
   ISceneNode* getSelectedNode();
   ISceneManager* getGizmoSceneManager();

private:
	IrrlichtDevice *Device;
	ISceneCollisionManager *CollMan;
        ISceneManager* gizmoSceneManager;
	bool LMouseButtonDown;
        ISceneNode* SelectedNode;
        ISceneNode* GizmoSceneNode;
        scene::ISceneNode* nodeGizmoX;
        scene::ISceneNode* nodeGizmoY;
        scene::ISceneNode* nodeGizmoZ;

        scene::ISceneNode* nodeGizmoXZplane;

        position2di initialCursorPosition;
        position2di initialObjectPosition;
        vector3df distanceToSelectedNode;
        vector3df currentDistanceToSelectedNode;
        gui::IGUIStaticText *posText;
        gui::IGUIStaticText *initialObjectPositionText;
        gui::IGUIStaticText *initialCursorPositionText;
        gui::IGUIStaticText *currentCursorPositionText;
        vector3df gizmoInitScale;

};
