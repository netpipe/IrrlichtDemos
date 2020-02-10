
#include "carrots.h"
#include <vector>
#include "rabbit.h"
//#include <irrKlang.h>
#include "stringaling.h"
#include "text.h"
#include "misc.h"

//using namespace audio;

extern IrrlichtDevice *device;
//extern ISoundEngine *sengine;
extern scene::ISceneManager *smgr;
extern gui::IGUIEnvironment *guienv;
extern video::IVideoDriver *driver;

const f32 carrotGapZ = 4.0;
const f32 carrotMaxGapX = 3.0;
const f32 carrotStartHeight = -2.0;
const f32 carrotHeightRange = 10.0;

int carrotsCollected;

std::vector <scene::IAnimatedMeshSceneNode *> carrots;

//scene::IAnimatedMeshSceneNode *iconCarrot = NULL;
gui::IGUIImage *carrotImg = NULL;
gui::IGUIStaticText *iconText = NULL;

void resetCarrotsCollected()
{
    carrotsCollected = 0;
}

void addCarrotDisplay()
{
    /*
    iconCarrot = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/carrotsmall.obj"), smgr->getActiveCamera());
    iconCarrot->setMaterialFlag(video::EMF_ZBUFFER, false);

    smgr->getActiveCamera()->setPosition(core::vector3df(0,0,0));
    smgr->getActiveCamera()->setRotation(core::vector3df(0,0,0));
    smgr->getActiveCamera()->setTarget(core::vector3df(0,0,1));
    smgr->getActiveCamera()->updateAbsolutePosition();

    core::line3df ray = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(
        core::position2d<s32>(150,110),
        smgr->getActiveCamera()
        );

    iconCarrot->updateAbsolutePosition();

    iconCarrot->setPosition(ray.start + ray.getVector().normalize()*80.0); // put it some distance away
    iconCarrot->updateAbsolutePosition();

    core::position2d<s32> pos = smgr->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(
        iconCarrot->getAbsolutePosition(),
        smgr->getActiveCamera()
        );
    printf("POS: %i,%i\n", pos.X,pos.Y);

    //iconCarrot->setPosition(core::vector3df(-70,48,60));
    iconCarrot->setRotation(core::vector3df(20,0,0));
    */

    video::ITexture *tex = driver->getTexture("media/carrot.png");
    core::dimension2d<u32> dim = tex->getSize();
    carrotImg = guienv->addImage(
        tex,
        core::position2d<s32>(30,30),
        true
        );
}

void setCarrotDisplayCount(int c)
{
    if (iconText)
    {
        iconText->remove();
    }

    iconText = staticText(stringify(c).c_str(), 120,65, 3, "media/blinkfont.xml");
}

void removeCarrotDisplay()
{
    //if (iconCarrot)
    //{
    //    iconCarrot->remove();
        if (iconText) iconText->remove();
    //    iconCarrot = NULL;
        iconText = NULL;
    //}
    //else
    //{
    //    printf("Error: removeCarrotDisplay()\n");
    //}

    if (carrotImg) carrotImg->remove();
    carrotImg = NULL;
}

void addCarrots(scene::ISceneNode *platform)
{
    if (rand()%1000 < 800)
    {
        int carrotCount = 2+rand()%8;

        f32 carrotHeight = carrotStartHeight+5.0;
        f32 scale = 1.0;
        core::vector3df rot = core::vector3df(0,0,0);

        if (rand()%100 < 5)
        {
            carrotCount = 1;
            carrotHeight = frandU()*20 + 10;
            scale = 4.0;
            rot = core::vector3df(frandU()*20,frandU()*360,0);
        }

        for (int i = 0; i < carrotCount; i ++)
        {
            scene::IAnimatedMeshSceneNode *node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/carrotsmall.obj"),
                platform);
            node->setPosition(core::vector3df(
                frandS()*carrotMaxGapX,
                carrotHeight,//+node->getBoundingBox().getExtent().Y/2.0,
                (f32)i*carrotGapZ-(f32)carrotCount)*carrotGapZ/2.0
                );
            node->setScale(core::vector3df(scale,scale,scale));
            node->setRotation(rot);
            node->grab();
            carrots.push_back(node);
        }
    }
}

void starShower(scene::ISceneNode *parent, core::vector3df pos, char *imageFile, video::E_MATERIAL_TYPE eType)
{
    scene::IParticleSystemSceneNode *ps = smgr->addParticleSystemSceneNode(
        false, parent, -1, pos
        );

    //ps->setParticleSize(core::dimension2d<f32>(0.4f, 0.4f));

    // create and set emitter
    scene::IParticleEmitter *em = ps->createBoxEmitter(
        core::aabbox3d<f32>(-1,-1,-1,1,1,1),
        core::vector3df(0,1.0,0)*0.05,
		10,20,
		video::SColor(255,255,255,255), video::SColor(255,255,255,255),
		// min, max lifetime of particles
		2000,2000,
		// max angle
        360);
	ps->setEmitter(em);
	em->drop();

	//scene::IParticleAffector *ef = ps->createGravityAffector(core::vector3df(0,-1.0,0)*0.05, 1000);
	//ps->addAffector(ef);
	//ef->drop();

	// adjust some material settings
    ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialTexture(0, driver->getTexture(imageFile));
	ps->setMaterialType(eType);
}

void updateCarrots()
{
    for (int i = 0; i < carrots.size(); i ++)
    {
        bool rabbitGotCarrot = rabbitHitNode(carrots[i]);

        // remove any that have gone "off screen"
        if (carrots[i]->getParent() == NULL || rabbitGotCarrot)
        {
            if (rabbitGotCarrot)
            {
                if (carrots[i]->getScale().X > 1.01)
                {
//                    sengine->play2D("media/boom.wav");
                    carrotsCollected += 100;
                }
                else
                {
  //                  sengine->play2D("media/pop0.wav");
                    carrotsCollected ++;
                }
                starShower(carrots[i]->getParent(), carrots[i]->getPosition(),
                    "media/star.png", video::EMT_TRANSPARENT_ADD_COLOR);
                carrots[i]->remove();
            }
            carrots[i]->drop();
            carrots.erase(carrots.begin()+i);
            i --;
            continue;
        }
    }

    setCarrotDisplayCount(carrotsCollected);
}

void rotateCarrotIcon()
{
    //if (iconCarrot)
    //{
        //iconCarrot->setRotation(iconCarrot->getRotation()+core::vector3df(0,2,0));
    //}
}

void removeCarrots()
{
    for (int i = 0; i < carrots.size(); i ++)
    {
        carrots[i]->drop();
    }
    carrots.clear();
}

