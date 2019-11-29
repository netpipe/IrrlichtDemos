#ifndef _PLAY_H_
#define _PLAY_H_

#include "CStateSys.h"
#include <irrlicht.h>
	using namespace irr;
	using namespace scene;

#include "commonStates.h"

class CSceneState : public CState
{
	public:
		CSceneState(CStateManager* man = NULL, CState* par = NULL)
			: CState(man, par)
		{
			device = manager->getDevice();
			ISceneManager* smgr = device->getSceneManager();

			manager->setColor(video::SColor(255, 128, 0, 0));
			manager->setRenderScene(true);

			smgr->addCameraSceneNode(0, core::vector3df(0,30,-40), core::vector3df(0,5,0));

			IAnimatedMesh* mesh = smgr->getMesh("../../media/sydney.md2");

			if(mesh == false)
				return;

			IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);

			if(node != NULL)
			{
				node->setMaterialFlag(video::EMF_LIGHTING, false);
				node->setMD2Animation(scene::EMAT_STAND);
				node->setMaterialTexture(0, device->getVideoDriver()->getTexture("../../media/sydney.bmp"));
			}
		}

		~CSceneState(void)
		{
			sydney->drop();
			cam->drop();
		}

		bool onEvent(const SEvent& event)
		{
			if(CState::onEvent(event) == true)
				return true;

			if(event.EventType == irr::EET_KEY_INPUT_EVENT
				&& event.KeyInput.PressedDown == false)
			{
				if(event.KeyInput.Key == irr::KEY_ESCAPE)
					CQuitState* quitState = new CQuitState(manager, this);
			}

			return false;
		}

	private:
		IAnimatedMeshSceneNode* sydney;
		ICameraSceneNode* cam;

		IGUIEnvironment* guiEnv;
		IrrlichtDevice* device;
};

#endif
