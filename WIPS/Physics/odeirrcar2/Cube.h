#pragma once
#include "includes.h"
#include <time.h>
//#include <Windows.h> // проблема при портировании
class Cube
{

	IAnimatedMeshSceneNode* node;
	IrrlichtDevice* device;
	ISceneNode *pNode;
	CIrrOdeWorld *worldNode;

public:

	Cube(IrrlichtDevice* dev,CIrrOdeWorld *world, vector3df pos = vector3df(0,10,0), vector3df size = vector3df(1,1,1))
	{
		/*
		device = dev;
		worldNode = world;

		//node = device->getSceneManager()->addCubeSceneNode();
		//node->setScale(size);
		//node->setPosition(pos);
		scene::ISceneManager* smgr = device->getSceneManager();










	IAnimatedMesh *Mesh=smgr->getMesh("models/box.3ds");
	IAnimatedMeshSceneNode *node = smgr->addAnimatedMeshSceneNode(Mesh,worldNode);
	node->setMaterialTexture(0,device->getVideoDriver()->getTexture("models/pillar.jpg"));
	node->setScale(vector3df(9.0f,5.0f,8.0f));
	node->setPosition(vector3df(20,20,20));
	node->setMaterialFlag(EMF_LIGHTING,false);

    CIrrOdeGeomBox *bx=reinterpret_cast<CIrrOdeGeomBox *>(smgr->addSceneNode(
          CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_GEOM_BOX_NAME),
          node));
    bx->getSurfaceParameters(0)->setBounce(1.0f);
    bx->getSurfaceParameters(0)->setModeBounce(true);
    bx->drop();


	    //сперва создаём body (тело) как дитя worldNode (узла физического мира)
    CIrrOdeBody *pBody=reinterpret_cast<CIrrOdeBody *>(
          smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(
          IRR_ODE_BODY_NAME),worldNode));
    pBody->setPosition(vector3df(0.0f,15.0f,0.0f));

    //далее загружаем модель в AnimatedMeshSceneNode
    //как дитя body (тела)
    Mesh=smgr->getMesh("models/sphere.3ds");
    node=smgr->addAnimatedMeshSceneNode(Mesh,pBody);
    node->setMaterialTexture(0,device->getVideoDriver()->getTexture("models/pillar.jpg"));
    node->setMaterialFlag(EMF_LIGHTING,false);

    //и создаем геометрию сферы как дитя AnimatedMeshSceneNode
    CIrrOdeGeomSphere *pSphere=reinterpret_cast<CIrrOdeGeomSphere *>
          (smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(
          IRR_ODE_GEOM_SPHERE_NAME),node));
    pSphere->setMassTotal(0.5f);
    pSphere->getSurfaceParameters(0)->setBounce(1.0f);
    pSphere->getSurfaceParameters(0)->setModeBounce(true);
    pSphere->drop();










	CIrrOdeManager::getSharedInstance()->initODE();
	worldNode->initPhysics();


	*/

	}

	void generateCube()
	{
		srand(time(0));
			node->setScale(vector3df(rand()%10,rand()%17,rand()%11));

			node->setPosition(vector3df(rand()%200,10,rand()%240));
	}
};
