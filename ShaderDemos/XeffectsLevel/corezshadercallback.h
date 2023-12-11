#ifndef __CORE_ZSHADER_CALLBACK_H_INCLUDED__
#define __CORE_ZSHADER_CALLBACK_H_INCLUDED__

#include <irrlicht.h>


class CoreZShaderCallback : public irr::video::IShaderConstantSetCallBack
{
public:
	virtual void OnSetConstants(irr::video::IMaterialRendererServices * services, irr::s32 userData)
	{
		irr::video::IVideoDriver * driver = services->getVideoDriver();
		irr::scene::ISceneManager * smgr = this->device->getSceneManager();

		services->setVertexShaderConstant("model", driver->getTransform(irr::video::ETS_WORLD).pointer(), 16);
		services->setVertexShaderConstant("view", driver->getTransform(irr::video::ETS_VIEW).pointer(), 16);
		services->setVertexShaderConstant("projection", driver->getTransform(irr::video::ETS_PROJECTION).pointer(), 16);
	}

	CoreZShaderCallback(irr::IrrlichtDevice * dev) : device(dev) {
		this->device->grab();
	}

	~CoreZShaderCallback() { this->device->drop(); }

private:

	irr::IrrlichtDevice * device;
};


#endif
