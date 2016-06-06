#ifndef _MAP_H
#define _MAP_H

#include "globals.h"
#include "main.h"
#include "sound.h"

class CMap
{
    public:
    CMap(core::stringc filename, ISceneManager* smgr, IVideoDriver* driver, CSound* Sound);
    vector3df getPlayerInformation();
    vector3df getCamPosition();
    vector3df getCamTarget();
    vector3df getScale();
    ISceneNode* getNode();
    IAnimatedMesh* getMesh();
    ITriangleSelector* getSelector();
    core::stringc fulldir;

    void remove();

    private:
//    CSound* Sound;
//    ISound* Atmospheric;
    float PlayerMoveSpeed;
    float PlayerJumpSpeed;
    float PlayerRotSpeed;
    ISceneManager* smgr;
    IVideoDriver* driver;
    std::string model;
    std::string name;
    std::string InternalName;
    ISceneNode* node;
	ISceneNode* skybox;
	IAnimatedMesh* mesh;
	//Physics Object
	list<ILightSceneNode *> lights;
	ITriangleSelector* selector;
	vector3df CamPos;
	vector3df CamTar;
	vector3df Position;
	vector3df Rotation;
	vector3df Scale;
};

#endif
