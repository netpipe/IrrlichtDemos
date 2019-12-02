/*
  CGTASceneNode: GTA world scene node for Irrlicht

  Copyright (C) 2008 David Kment

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.


  v0.1, developed and tested with Irrlicht 1.4.2

  Notes:
    - When creating an instance of CGTASceneNode, gameId specifies which game world it should load:
        1: GTA3
        2: GTAVC
        3: GTASA
    - the following lines show how CGTASceneNode is configured:
        device->getSceneManager()->getParameters()->setAttribute("GTA_TEXTURES_PATH", "textures/");
        device->getSceneManager()->getParameters()->setAttribute("GTA_MODELS_PATH", "models/");
        device->getSceneManager()->getParameters()->setAttribute("GTA_LOD_MESHES_ONLY", true);
        device->getSceneManager()->getParameters()->setAttribute("GTA_WINDOWS_PATH_FORMAT", false);
    - GTA_WINDOWS_PATH_FORMAT must be used when under windows, set to false for unix or ZIP files.
    - GTA_LOD_MESHES_ONLY draws only low detail meshes. speeds up rendering big time.
    - from GTA_TEXTURES_PATH the extracted TXD textures are loaded.
      in there must be a subdirectory for each TXD file, and in there the containing textures as
      TGA, JPG or PNG. JPG is not recommended because it has no alpha channel.
    - from GTA_MODELS_PATH the DFF models are loaded.
    - The DAT/IDE/IPL files must be in the subfolder 'data', because subsequent file paths are
      hardcoded in the DAT files themselves.

*/

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

    CGTASceneNode(IrrlichtDevice *device, int gameId, ISceneNode* parent, ISceneManager* mgr, s32 id,
        const core::vector3df& position = core::vector3df(0,0,0),
        const core::vector3df& rotation = core::vector3df(0,0,0),
        const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));
    virtual ~CGTASceneNode();
    virtual void OnRegisterSceneNode();
    virtual void render();
    virtual const core::aabbox3d<f32>& getBoundingBox() const;

    void initialise();

private:

    class ObjectDefinition
    {
        public:
            ObjectDefinition(ISceneManager *sceneManager);
            ~ObjectDefinition();
            void setName(stringc name);
            void setTextureDir(stringc texturedir);
            irr::scene::IAnimatedMesh *getMesh();
            void loadFromDFFFile();

        private:

            stringc Name;
            stringc TextureDir;
            IAnimatedMesh *Mesh;

            ISceneManager *SceneManager;
    };

    class ObjectInstance
    {
        public:
            ObjectInstance(ISceneManager *sceneManager);
            ~ObjectInstance();

            void init();
            void setName(stringc name);
            void setID(int id);
            int getID();
            void setPosition(float x, float y, float z);
            void setRotation(float x, float y, float z);
            void setScale(float x, float y, float z);
            void setMesh(IAnimatedMesh *mesh);

        private:

            stringc Name;
            int Id;
            int State;

            ISceneManager *SceneManager;

            IAnimatedMesh *Mesh;
            IMeshSceneNode *Node;

            float PositionX, PositionY, PositionZ;
            float RotationX, RotationY, RotationZ;
            float ScaleX, ScaleY, ScaleZ;
    };

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

void quaternionsToAngles(float in_x, float in_y, float in_z, float in_w, float out_x, float out_y, float out_z);
bool startsWith(stringc *haystack, c8 *needle);

#endif /* _CGTASCENENODE_H_ */
