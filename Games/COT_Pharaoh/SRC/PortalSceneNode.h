
#include <irrlicht.h>
//#include <audiere.h>
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace io;
using namespace video;
using namespace gui;
//using namespace audiere;
using namespace std;

class PortalSceneNode {

      public:
             PortalSceneNode();
             PortalSceneNode(IrrlichtDevice* dev, vector3df pos, vector3df targetPos, vector3df targetTarg);
             ITriangleSelector* getTriangleSelector();
             void render(ICameraSceneNode* camera);
             void togglePortalActive(IMetaTriangleSelector* metaSelector);
             vector3df getPosition();
             ISceneNodeAnimatorCollisionResponse* checkCameraInside(ICameraSceneNode* camera, IMetaTriangleSelector* metaSelector, ISceneNodeAnimatorCollisionResponse* camAnim);

      private:
              IrrlichtDevice* device;
              ISceneManager* smgr;
              IVideoDriver* driver;
              IAnimatedMesh* mesh;
              ISceneNode* portal, * portalInner;
              ITexture* rt;
              array<ITexture*> portalInnerTextures, portalInnerTexturesBackwards;
              ICameraSceneNode* rttCamera;
//             AudioDevicePtr soundDevice;
//              OutputStreamPtr portalHum;
              bool portalActive;

};
