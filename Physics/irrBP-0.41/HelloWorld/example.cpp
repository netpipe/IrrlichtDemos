#include <irrlicht.h>
#include <iostream>
#include <IrrBP.h>
#include "CIrrBPCamera.h"
#include "animator/CIrrBPAnimator.h"
using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace bullet;
using namespace std;

static CIrrBPManager * bulletmgr;
static ISceneManager* smgr;
CIrrBPBoxBody * box;
CIrrBPBoxBody * body;
class Receiver : public IEventReceiver
{
public:
		Receiver();
		virtual bool OnEvent(const SEvent& event);
private:
};
Receiver::Receiver()
{

}

bool Receiver::OnEvent(const irr::SEvent &event)
{

	if(event.EventType == EET_MOUSE_INPUT_EVENT)
	{
		if(event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
		{
			//irr::core::vector3df vettore = box->getRotation();


			box->setRotation(box->getRotation()+irr::core::vector3df(0.0f,60,0.0f));
			//vettore = box->getRotation();
			ISceneNode * node = smgr->addCubeSceneNode(10,0,-1,smgr->getActiveCamera()->getPosition());
			body = bulletmgr->addRigidBox(node,40);
			irr::core::vector3df rot = smgr->getActiveCamera()->getRotation();
			irr::core::matrix4 mat;

			mat.setRotationDegrees(rot);
			irr::core::vector3df forwardDir(irr::core::vector3df(mat[8],mat[9],mat[10]) *120);

			body->getBodyPtr()->setLinearVelocity(irrVectorToBulletVector(forwardDir) * 2);

		}

	}


	return false;
}
int main()
{


	IrrlichtDevice *device =
		createDevice(video::EDT_OPENGL, core::dimension2d<u32>(1280, 720));
	Receiver * recv = new Receiver();
	if (device == 0)
		return 1; // could not create selected driver.

	video::IVideoDriver* driver = device->getVideoDriver();
	 smgr = device->getSceneManager();

//	device->getFileSystem()->addZipFileArchive("map-20kdm2.pk3");

	device->setEventReceiver(recv);

	scene::IAnimatedMesh* mesh = smgr->getMesh("box.3ds");

	scene::IMeshSceneNode* node = 0;

	if (mesh)
		node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);
	node->setMaterialTexture(0,driver->getTexture("box0.jpg"));
	if (node)
				node->setPosition(irr::core::vector3df(20,-0,-0));
//		node->setPosition(irr::core::vector3df(-1350,-130,-1400));

	node->setScale(vector3df(500.0f,1.5f,500.0f));
  node->setMaterialFlag(EMF_LIGHTING,false);
	ICameraSceneNode * cam =  smgr->addCameraSceneNodeFPS(0,100,0.1f);
	cam->setPosition(irr::core::vector3df(000,30,60));


	device->getCursorControl()->setVisible(false);

	//bulletmgr = createMultithreadedBulletManager(device);
	bulletmgr = createBulletManager(device);
	bulletmgr->getWorld()->setGravity(irr::core::vector3df(0,-10,0));
	//bulletmgr->addTrimesh(node,0)->setName("Mappa");
	bulletmgr->addRigidBox(node,0);
	//bulletmgr->addRigidBox(cam,0)->setKinematic(true);
	CIrrBPCamera* camnode =bulletmgr->createCamera(cam,20);// new CIrrBPCamera(cam,20);

	//bulletmgr->addCollisionObjectToBulletQueue(camnode->getCameraBody());

	int xshift,yshift,zshift;
	IMeshSceneNode * Node;
	IMeshSceneNode * Node2;
	Node = smgr->addCubeSceneNode(5,0,-1,irr::core::vector3df(0,-20,0));

	Node->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	Node->setMaterialFlag(EMF_LIGHTING,false);
	Node->setMaterialTexture(0,driver->getTexture("sphere1.jpg"));
	box= bulletmgr->addRigidBox(Node,2);

	box->setRotation(irr::core::vector3df(0,0,0));
	//box->setRotation(box->getRotation() + irr::core::vector3df(0,0,10));

	Node2 = smgr->addCubeSceneNode(5,0,-1,irr::core::vector3df(20,0,-20));
	Node2->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	Node2->setMaterialFlag(EMF_LIGHTING,false);
	Node2->setMaterialTexture(0,driver->getTexture("sphere1.jpg"));
	CIrrBPBoxBody * box2 = bulletmgr->addRigidBox(Node2,0);
	body=box2;
	/*CIrrBPFollowAnimator * anim = new CIrrBPFollowAnimator(cam,vector3df(0,50,0));
	anim->setBody(box2);
	box2->addAnimator(anim);*/
	box->setName("Box1");
	box2->setName("Box1");
	//btTypedConstraint * m_Constraint = new btPoint2PointConstraint(*(box->getBodyPtr()),irrVectorToBulletVector(irr::core::vector3df(0,0,0)));
	//bulletmgr->getWorld()->getBulletWorldPtr()->addConstraint(m_Constraint);

//	bulletmgr->buildP2PConstraint(box);
	//CIrrBPP2PConstraint * constr = bulletmgr->buildP2PConstraint(box);
	//constr->setLimit(PI/4*0.6,PI/4);
	for(int k=0;k<7;k++)
	  for(int i=0;i<100;i++)
        {
		//	if((i+k)%2==0)
				Node2 = smgr->addCubeSceneNode(5,0,-1,irr::core::vector3df(0+(k*10),0 +(i*15),0));
			//else
			//	Node2 = smgr->addSphereSceneNode(5,16,0,-1,irr::core::vector3df(0+(k*10),10 +(i*15),0));
        //Node2->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
        Node2->setMaterialFlag(EMF_LIGHTING,false);
        Node2->setMaterialTexture(0,driver->getTexture("sphere1.jpg"));

        box2 = bulletmgr->addRigidBox(Node2,10);

        }

	int lastFPS = -1;
	int cnt = 0;
	while(device->run())
	{

		//cout<<bulletmgr->getWorld()->isBodyColliding(box);
		if (device->isWindowActive())
		{
			driver->beginScene(true, true, video::SColor(255,200,200,200));
						driver->setTransform(ETS_WORLD,matrix4());
			bulletmgr->stepSimulation();
			smgr->drawAll();

			int fps = driver->getFPS();

			if (lastFPS != fps)
			{

				core::stringw str = L"Irrlicht Engine - Quake 3 Map example [";
				str += driver->getName();
				str += "] FPS:";
				str += fps;

				device->setWindowCaption(str.c_str());
				lastFPS = fps;

			}
			driver->endScene();
		}
		else
			device->yield();
	}
	/*
	In the end, delete the Irrlicht device.
*/
	//delete constr;
	//delete ctw;
	//delete m_Constraint;
	//delete camnode;
	delete recv;
	bulletmgr->drop();
	device->drop();
	return 0;

}

/*
That's it. Compile and play around with the program.
**/
