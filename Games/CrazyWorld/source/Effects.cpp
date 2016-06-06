
#include "Effects.h"

Effects::Effects(IrrlichtDevice *device)
{
    this->device = device;
    smgr = device->getSceneManager();
    driver = device->getVideoDriver();
}


scene::IParticleSystemSceneNode *Effects::particleSpray(
    video::ITexture *texture,
    core::vector3df pos, core::vector3df vec,
    u16 pps,
    u32 sprayTime,
    u32 lifeTime,
    f32 particleSize
    )
{
    
    core::vector3df rvel;
    rvel.X = rand()%100 * ((rand()%2) ? -1 : 1);
    rvel.Y = rand()%100 * ((rand()%2) ? -1 : 1);
    rvel.Z = rand()%100 * ((rand()%2) ? -1 : 1);
    
    scene::IParticleSystemSceneNode *ps = smgr->addParticleSystemSceneNode(false, 0);
	ps->setPosition(pos);
    
    ps->setParticleSize(core::dimension2d<f32>(particleSize, particleSize));
    
    // create and set emitter
    scene::IParticleEmitter *em = ps->createBoxEmitter(
        core::aabbox3d<f32>(-0.01,-0.01,-0.01,0.01,0.01,0.01),
        vec,      // velocity
		pps,     // min particles per second
		pps,     // max particles per second
		video::SColor(0,255,255,255), // min colour
        video::SColor(0,255,255,255), // max colour
		lifeTime*0.1,   // min lifetime
        lifeTime*1.2,   // max lifetime
        25      // max angle
        );
	ps->setEmitter(em);
	em->drop();
	
	// spray affector. Halts particle production after a set time
	scene::IParticleAffector *paf = new SprayAffector(ps, smgr, device->getTimer()->getTime() + sprayTime);
	ps->addAffector(paf);
	//paf = ps->createGravityAffector();
	//ps->addAffector(paf);
	paf->drop();
	
	// adjust some material settings
    ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialTexture(0, texture);
	ps->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	
	return ps;
}



