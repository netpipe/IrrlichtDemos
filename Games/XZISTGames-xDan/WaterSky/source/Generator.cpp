
#include "Generator.h"
#include "ODEMeshAnimator.h"
#include "Actor.h"
#include "SoundAnimator.h"

Generator::Generator(WorldBase *world,
    core::vector3df pos, core::vector3df rot)
    : Object(world)
{
    sceneNode = smgr->addAnimatedMeshSceneNode(smgr->getMesh("generator.b3d"));
    
    // add a static collision mesh
    dAnimator = new ODEMeshAnimator(sceneNode, ((scene::IAnimatedMeshSceneNode *)sceneNode)->getMesh(), dPhysics, 0);
    sceneNode->addAnimator(dAnimator);
    dAnimator->drop();
    
    setPosition(pos);
    setRotation(rot);
    
    // requires fuel by default
    active = false;
    
    addActivatableExclamationMark();
    createSoundAnimator();
    
    // Smoke effects
    psSmoke = smgr->addParticleSystemSceneNode(false, sceneNode, -1,
        core::vector3df(1.2,1.0,0),
        core::vector3df(0,0,0)
        );
    psSmoke->setParticleSize(core::dimension2d<f32>(1,1)*0.5);
    // Fade out
	scene::IParticleAffector *paf = psSmoke->createFadeOutParticleAffector();
	psSmoke->addAffector(paf);
	paf->drop();
	// Particle graphics
    psSmoke->setMaterialFlag(video::EMF_LIGHTING, false);
	psSmoke->setMaterialTexture(0, driver->getTexture("media/graphics/smoke.tga"));
	psSmoke->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA );
	// Emitter
	createSmokeEmitter();
}

Object *Generator::createNew()
{
    return new Generator(this);
}

void Generator::update(f32 dt)
{
    
    if (getRotation() != lastRotation) // recreate particle emitter at new angle if rotated
    {
        createSmokeEmitter();
        lastRotation = getRotation();
    }
    
    psSmoke->setVisible(active);
    
    if (!active)
    {
        Actor *actor = (Actor *)getNearestObject(OBJECT_TYPE_PLAYER);
        if (actor)
        {
            if (getBoundsDistance(actor) < 0.0)
            {
                Object *fuelCan = actor->isCarrying(OBJECT_TYPE_FUELCAN);
                
                if (fuelCan)
                {
                    removeObject(fuelCan);
                    
                    active = true;
                    
                    soundAnimator->play("media/sfx/generatorstart.wav", SOUNDANIM_ENQUEUE);
                    soundAnimator->play("media/sfx/generatorloop.wav", SOUNDANIM_ENQUEUE|SOUNDANIM_LOOP);
                }
            }
        }
    }
}

void Generator::createSmokeEmitter()
{
    psSmoke->setEmitter(0);
    core::vector3df vel = core::vector3df(0,0,1)*0.001;
    core::CMatrix4<f32> mat = sceneNode->getAbsoluteTransformation();
    mat.rotateVect(vel);
    scene::IParticleEmitter *psemSmoke = psSmoke->createPointEmitter(
        vel, // velocity
        14,16, // min,max per second
        video::SColor(0,255,255,255),video::SColor(0,255,255,255), // min,max start colours
        6000,7000, // min/max lifetime
        10 // max angle variation
        );
    psSmoke->setEmitter(psemSmoke);
    psemSmoke->drop();
}

