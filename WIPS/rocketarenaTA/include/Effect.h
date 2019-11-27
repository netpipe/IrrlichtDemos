/*

Effect - header file

*/

#ifndef __Effect_h
#define __Effect_h

#include <irrlicht.h>

using namespace irr;


class cEffect {
public:
	
	// constructor
	cEffect();
	
	// destructor
	~cEffect();
	
	// Initialization
	void Initialize();
	
	// create fire effect
	void createFireEffect(IrrlichtDevice *device);
	
	// create rocket waste gas
	void createRocketWasteGas(IrrlichtDevice *device);
	
	// create explosion effect
	void createExplosionEffect(IrrlichtDevice *device);
	
	const scene::IParticleSystemSceneNode *getPS();
	
	const scene::IParticleEmitter *getEM();
	
	// create an menu effect
	scene::IParticleSystemSceneNode *createMenuEffect(IrrlichtDevice *device, bool right);
	
	
private:
	
	scene::IParticleSystemSceneNode *particleRocketWasteGas;
	scene::IParticleEmitter *emitterRocketWasteGas;
	
};

#endif
