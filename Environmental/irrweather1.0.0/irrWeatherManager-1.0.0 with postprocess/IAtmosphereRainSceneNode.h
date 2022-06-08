//! The class was written by Gawaboumga (Copyright (C) 20013-2014 Youri Hubaut).

#ifndef __IAtmosphereRainSceneNode_H_INCLUDED__
#define __IAtmosphereRainSceneNode_H_INCLUDED__

#include <irr/irrlicht.h>

enum E_WEATHER_EFFECT
{
	EWE_RAIN,
    EWE_SNOW,
    EWE_NONE
};

namespace irr
{

class IAtmosphereRainSceneNode
{
    public:
        //! Constructor.
        IAtmosphereRainSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr,
            video::ITexture* rain, video::ITexture* snow);

        //! Destructor.
		~IAtmosphereRainSceneNode();

		//! Changes the inclemency's effect.
		void changeInclemency(E_WEATHER_EFFECT effect);

	private:

	    scene::ISceneNode* Parent;
	    scene::ISceneManager* smgr;
	    video::IVideoDriver* Driver;

	    scene::IParticleSystemSceneNode* ps;
	    scene::IParticleEmitter* em;

	    video::ITexture* Rain;
	    video::ITexture* Snow;

};

} // namespace

#endif // __IAtmosphereRainSceneNode_H_INCLUDED__
