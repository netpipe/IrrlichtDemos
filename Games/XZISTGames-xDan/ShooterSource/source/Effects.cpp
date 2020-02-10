
#include "Effects.h"
#include "ParabolaAnimator.h"

Effects::Effects(IrrlichtDevice *device)
{
    this->device = device;
    smgr = device->getSceneManager();
    driver = device->getVideoDriver();
}

void Effects::meshSpray(
    scene::IAnimatedMesh **meshes, u16 meshCount, core::vector3df pos,
    core::vector3df vel, core::vector3df acc, core::vector3df rvel)
{
    pos += sn->getPosition();
    if (pos.Y < 0) pos.Y = 0.01;

    for (int i = 0; i < meshCount; i ++)
    {
        scene::IAnimatedMeshSceneNode *n = smgr->addAnimatedMeshSceneNode(meshes[i]);

        core::vector3df rvel;
        rvel.X = rand()%100 * ((rand()%2) ? -1 : 1);
        rvel.Y = rand()%100 * ((rand()%2) ? -1 : 1);
        rvel.Z = rand()%100 * ((rand()%2) ? -1 : 1);

  /*      scene::ISceneNodeAnimator *an = new ParabolaAnimator(
            smgr,
            device->getTimer()->getTime(),
            pos+core::vector3df(rand()%200-100,0,rand()%200-100)/200.0,
            vel,//core::vector3df(rand()%200-100,100.0,rand()%200-100) / 50.0,
            acc,//core::vector3df(0,-5.0,0),
            rvel,
            core::plane3df(0,-1.0,0, 0,1,0)
            );
            */
//        n->addAnimator(an);
//        an->drop();
    }
}

scene::IParticleSystemSceneNode *Effects::particleSpray(
    video::ITexture *texture,
    scene::IAnimatedMesh **gibMeshes, u16 gibMeshCount,
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
    meshSpray(gibMeshes, gibMeshCount, core::vector3df(0,0,0),
        core::vector3df(rand()%200-100,100.0,rand()%200-100) / 50.0,
        core::vector3df(0,-5.0,0),
        rvel);

    scene::IParticleSystemSceneNode *ps = smgr->addParticleSystemSceneNode(
        false, sn, -1, pos
        );

    ps->setParticleSize(core::dimension2d<f32>(particleSize, particleSize));

    // create and set emitter
    scene::IParticleEmitter *em = ps->createBoxEmitter(
        core::aabbox3d<f32>(-0.01,-0.01,-0.01,0.01,0.01,0.01),
        vec,      // velocity
		pps,     // min particles per second
		pps,     // max particles per second
		video::SColor(0,255,255,255), // min colour
        video::SColor(0,255,255,255), // max colour
		lifeTime,   // min lifetime
        lifeTime,   // max lifetime
        40      // max angle
        );
	ps->setEmitter(em);
	em->drop();

	// gravity affector
	scene::IParticleAffector *paf;
	paf = ps->createGravityAffector(core::vector3df(0,-0.01,0), 1000);
	ps->addAffector(paf);
	paf->drop();

	// spray affector. Halts particle production after a set time
	paf = new SprayAffector(ps, smgr, device->getTimer()->getTime() + sprayTime);
	ps->addAffector(paf);
	paf->drop();

	// adjust some material settings
    ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialTexture(0, texture);
	ps->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

	return ps;
}

void Effects::flyText(core::stringw str)
{
    core::vector3df pos = sn->getPosition();

    // type == 0
    video::SColor col = video::SColor(255,255,166,222);;
    gui::IGUIFont *font = device->getGUIEnvironment()->getFont("font1.bmp");
    int time = 500;

    scene::ISceneNode *tn = (scene::ISceneNode *)smgr->addTextSceneNode(
        font,
        str.c_str(), col, NULL, pos);
    tn->addAnimator(smgr->createDeleteAnimator(time));
    tn->addAnimator(
        smgr->createFlyStraightAnimator(
        pos,
        pos
        +core::vector3df(
            ((float)(rand()%20000-10000))/10000.0f*10,
            10,
            ((float)(rand()%20000-10000))/10000.0f*10),
        4000,
        false)
        );
}



