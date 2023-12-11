// zur Verhinderung doppelter Definition
#ifndef GANGSHADERCALLBACK_H
#define GANGSHADERCALLBACK_H

#include <irrlicht.h>

class CGangShaderCallBack : public irr::video::IShaderConstantSetCallBack
{
public:

	// Initialisierung
	CGangShaderCallBack(irr::IrrlichtDevice* _IrrDevice)
	{
		IrrDevice = _IrrDevice;
	}

	// Shader-Konstanten setzen
    virtual void OnSetConstants(irr::video::IMaterialRendererServices* _Services,
                    irr::s32 _UserData)
    {
		// Kamera-Position = Position der Lichtquelle setzen
		irr::core::vector3df pos = IrrDevice->getSceneManager()->getActiveCamera()->getAbsolutePosition();
		_Services->setVertexShaderConstant("mLightPos", reinterpret_cast<irr::f32*>(&pos), 3);
    }
	
	private:
		irr::IrrlichtDevice* IrrDevice;
};

#endif
