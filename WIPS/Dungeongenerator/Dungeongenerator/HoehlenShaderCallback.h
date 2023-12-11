// zur Verhinderung doppelter Definition
#ifndef HOEHLENSHADERCALLBACK_H
#define HOEHLENSHADERCALLBACK_H

#include <irrlicht.h>

class CHoehlenShaderCallBack : public irr::video::IShaderConstantSetCallBack
{
public:

	// Initialisierung
	CHoehlenShaderCallBack(irr::IrrlichtDevice* _IrrDevice)
	{
		IrrDevice = _IrrDevice;
		SetzeFarbe(irr::video::SColorf(0.0,0.4,0.8,1.0));					
		// (0.5,0.3,0.1,1.0) Sandhoehle
		// (0.0,0.4,0.8,1.0) Eisblau		
	}

	// Farbe der Hoehle festlegen
	void SetzeFarbe(irr::video::SColorf _Farbe)
	{
		HoehlenFarbe = _Farbe;
		HoehlenHalbFarbe = irr::video::SColorf(0.5*HoehlenFarbe.r,0.5*HoehlenFarbe.g,0.5*HoehlenFarbe.b,0.5*HoehlenFarbe.a);
	}
	// aktuelle Farbe auslesen
	irr::video::SColorf LeseFarbe()
	{
		return HoehlenFarbe;
	}

	// Shader-Konstanten setzen
    virtual void OnSetConstants(irr::video::IMaterialRendererServices* _Services,
                    irr::s32 _UserData)
    {
		// Kamera-Position = Position der Lichtquelle setzen
		irr::core::vector3df pos = IrrDevice->getSceneManager()->getActiveCamera()->getAbsolutePosition();
		_Services->setVertexShaderConstant("mLightPos", reinterpret_cast<irr::f32*>(&pos), 3);

		// Farbe der Hoehle setzen (Haelfte des Farbwertes wird uebergeben, spart eine Multiplikation mit 0.5 im Shader)
		_Services->setPixelShaderConstant("mHalfColor",	reinterpret_cast<irr::f32*>(&HoehlenHalbFarbe), 4);
    }
	
	private:
		irr::IrrlichtDevice* IrrDevice;
		irr::video::SColorf HoehlenFarbe;
		irr::video::SColorf HoehlenHalbFarbe;
};

#endif
