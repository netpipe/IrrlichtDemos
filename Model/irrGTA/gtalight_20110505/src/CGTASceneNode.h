// Copyright (C) 2007-2011 David Kment
// For license see license.txt

#ifndef _CGTASCENENODE_H_
#define _CGTASCENENODE_H_

#include "IrrlichtDevice.h"
#include "ISceneNode.h"
#include "IMeshSceneNode.h"
#include "IAnimatedMesh.h"
#include "SMeshBuffer.h"

#include <vector>

using namespace std;
using namespace irr;
using namespace scene;
using namespace core;

#define GAMEID_GTA3 1
#define GAMEID_GTAVC 2
#define GAMEID_GTASA 3

#define MAXOBJECTIDS_GTA3 3200
#define MAXOBJECTIDS_GTAVC 4800
#define MAXOBJECTIDS_GTASA 18700

#define FORMAT_DAT 1
#define FORMAT_IPL 2
#define FORMAT_IDE 3

#define IPL_MODE_INST 1
#define IPL_MODE_CULL 2
#define IPL_MODE_PICK 3

#define IDE_MODE_OBJS 1
#define IDE_MODE_TOBJ 2
#define IDE_MODE_PATH 3
#define IDE_MODE_2DFX 4

class CGTASceneNode : public ISceneNode
{
public:

    class ObjectDefinition
    {
        public:
            ObjectDefinition(ISceneManager *sceneManager);
            ~ObjectDefinition();
            float getDrawDistance();
            stringc getTextureDir();
            void setDrawDistance(float distance);
            void setName(stringc name);
            void setTextureDir(stringc texturedir);

        private:

            stringc Name;
            stringc TextureDir;
            float DrawDistance;

            ISceneManager *SceneManager;
    };

    class ObjectInstance
    {
        public:
            ObjectInstance(ISceneManager *sceneManager);
            ~ObjectInstance();

            void init();
            void update();
            int getID();
            ObjectInstance *getInstance();
            float getDrawDistance();
            core::vector3df getPosition();
            core::vector3df getAbsolutePosition();
            stringc getName();
            bool getRender();
            bool isLODMesh();
            void setDrawDistance(float distance);
            void setName(stringc name);
            void setTextureDir(stringc texturedir);
            void setID(int id);
            void setRender(bool render);
            void setPosition(core::vector3df position);
            void setRotation(core::vector3df rotation);
            void setScale(core::vector3df scale);
            void setLodObject(ObjectInstance &object);

        private:

            void loadFromDFFFile();

            int Id;
            float DrawDistance;
            bool IsLODMesh;
            bool Render;
            stringc Name;
            stringc TextureDir;

            ISceneManager *SceneManager;

            IAnimatedMesh *Mesh;
            IMeshSceneNode *Node;
            ObjectInstance *LodObject;

            core::vector3df Position, Rotation, Scale, AbsolutePosition;
    };

    CGTASceneNode(IrrlichtDevice *device, int gameId, ISceneNode* parent, ISceneManager* mgr, s32 id,
        const core::vector3df& position = core::vector3df(0,0,0),
        const core::vector3df& rotation = core::vector3df(0,0,0),
        const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));
    virtual ~CGTASceneNode();
    virtual void OnRegisterSceneNode();
    virtual void render();
    virtual const core::aabbox3d<f32>& getBoundingBox() const;

    void initialise();
    std::vector<CGTASceneNode::ObjectInstance> *getObjectInstances();

private:
    void loadDAT(stringc inStr);
    void loadIPL(stringc inStr);
    void loadIDE(stringc inStr);
    void processFile(stringc inFileName, int mode);
    void processDAT(stringc inStr);
    void processIPL(stringc inStr);
    void processIDE(stringc inStr);

    int GameId;
    int IdeMode;
    int IplMode;

    IrrlichtDevice *Device;
	ILogger* Logger;
    core::aabbox3d<f32> Box;

    std::vector<ObjectDefinition> Definitions;
    std::vector<ObjectInstance> Instances;
};

bool startsWith(stringc *haystack, const char *needle);

#endif /* _CGTASCENENODE_H_ */
