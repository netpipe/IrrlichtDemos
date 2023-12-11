#include <irrlicht.h>
#include "InputHandler.h"
#include "CCompositeEventReceiver.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class Player : public CCompositeInputEventReceiver
{
	ISceneNode* playerModel;
	ICameraSceneNode* playerCamera;

	f32 delta; //Framerate independant movement FTW
	
	ISceneManager* smgr;
public:
	ICameraSceneNode* cam()
	{
		return playerCamera;
	}

	Player(ISceneManager* mgr, stringw meshName, stringw textureName) : smgr(mgr)
	{
		/*IMesh* mesh = smgr->getMesh(meshName);
		if (!mesh)
		{
			char c[50];
			sprintf(c, "Could not load mesh: %s", meshName);
			throw std::exception::exception(c);
		}*/

		//playerModel = smgr->addMeshSceneNode(mesh);
		//playerModel = smgr->addCubeSceneNode(0.1f);
		//playerModel->setMaterialTexture(0, smgr->getVideoDriver()->getTexture(textureName));
		//playerModel->setRotation(vector3df(0.f,-90.f,0.f));
		playerCamera = smgr->addCameraSceneNode(playerModel, vector3df(0.f, 2.0f, -5.f));
		playerCamera->bindTargetAndRotation(true);
		//playerModel->setScale(vector3df(1.f,1.f,1.f) * 0.2f);
		//playerModel->setPosition(vector3df(0.0f, 5.0f,0.0f));
		//playerModel->setMaterialFlag(EMF_LIGHTING, false);
	}

	bool OnInputEvent(InputHandler* handle, const SEvent& event)
	{
		bool done=false;
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		{
			if (handle->keyDown(KEY_RIGHT))
			{
				//playerModel->setRotation(playerModel->getRotation() + vector3df(0.f, .5f*delta, 0.f));
				playerCamera->setRotation(playerCamera->getRotation() + vector3df(0.f, .5f*delta, 0.f));
				done=true;
			}
			else if (handle->keyDown(KEY_LEFT))
			{
				//playerModel->setRotation(playerModel->getRotation() - vector3df(0.f, .5f*delta, 0.f));
				playerCamera->setRotation(playerCamera->getRotation() - vector3df(0.f, .5f*delta, 0.f));
				done=true;
			}

			if (handle->keyDown(irr::KEY_HOME))
			{
				playerCamera->setRotation(playerCamera->getRotation() - vector3df(.5f*delta, 0.f,0.f));
				done=true;
			}
			else if (handle->keyDown(KEY_END))
			{
				playerCamera->setRotation(playerCamera->getRotation() + vector3df(.5f*delta, 0.f,0.f));
				done=true;
			}

			if (handle->keyDown(irr::KEY_UP))
			{
				//playerModel->setPosition(playerModel->getPosition() + 0.5f*delta * playerModel->getRotation().rotationToDirection());
				playerCamera->setPosition(playerCamera->getPosition() + 0.5f*delta * playerCamera->getRotation().rotationToDirection());
				done=true;
			}
			else if (handle->keyDown(irr::KEY_DOWN))
			{
				//playerModel->setPosition(playerModel->getPosition() - 0.5f*delta * playerModel->getRotation().rotationToDirection());
				playerCamera->setPosition(playerCamera->getPosition() - 0.5f*delta * playerCamera->getRotation().rotationToDirection());
				done=true;
			}

			if (handle->keyDown(irr::KEY_NEXT))
			{
				//playerModel->setPosition(playerModel->getPosition() - 0.5f*delta * playerModel->getRotation().rotationToDirection(vector3df(0.f,1.f,0.f)));
				playerCamera->setPosition(playerCamera->getPosition() - 0.5f*delta * playerCamera->getRotation().rotationToDirection(vector3df(0.f,1.f,0.f)));
				done=true;
			}
			else if (handle->keyDown(irr::KEY_PRIOR))
			{
				//playerModel->setPosition(playerModel->getPosition() + 0.5f*delta * playerModel->getRotation().rotationToDirection(vector3df(0.f,1.f,0.f)));
				playerCamera->setPosition(playerCamera->getPosition() + 0.5f*delta * playerCamera->getRotation().rotationToDirection(vector3df(0.f,1.f,0.f)));
				done=true;
			}
				
			//playerCamera->setTarget(playerCamera->getTarget() - 0.5f*delta * playerCamera->getRotation().rotationToDirection());
		}
		return done;
	}

	void updateDelta(f32 newD)
	{
		delta = newD;
	}
};