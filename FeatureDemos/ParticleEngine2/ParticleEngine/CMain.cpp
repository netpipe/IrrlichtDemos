/*
  Copyright (C) 2011 Daniel Sudmann

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Daniel Sudmann suddani@googlemail.com
*/
#include "CMain.h"
#include <IParticleSystem.h>
#include <IParticleDrawer.h>
#include <IEmitterRegion.h>
#include <IParticleEmitter.h>
#include <Particle.h>

void addExplosion(const irr::core::vector3df& pos, const irr::core::vector3df& rot, const irr::u32& scale, irr::scene::ISceneManager* smgr, const irr::core::plane3df& p);

class ColorAffector : public irr::scene::particle::IParticleAffector
{
public:
    ColorAffector(const irr::video::SColor& targetColor)
    {
        Target = targetColor;
    }
    void affect(irr::scene::particle::Particle* particle, irr::u32 timeMs, irr::f32 diff)
    {
        irr::f32 lifeTime = particle->LifeTime;//particle->DestroyTimeMs-particle->CreateTimeMs;
        irr::f32 livedTime = particle->TimeLived;//timeMs-particle->CreateTimeMs;

        particle->Color = Target.getInterpolated(particle->StartColor, livedTime/lifeTime);
    }
    irr::video::SColor Target;

};


class ColorAffectorQ : public irr::scene::particle::IParticleAffector
{
public:
    ColorAffectorQ(const irr::video::SColor& targetColor0, const irr::video::SColor& targetColor1)
    {
        Target0 = targetColor0;
        Target1 = targetColor1;
    }
    void affect(irr::scene::particle::Particle* particle, irr::u32 timeMs, irr::f32 diff)
    {
        irr::f32 lifeTime = particle->LifeTime;//particle->DestroyTimeMs-particle->CreateTimeMs;
        irr::f32 livedTime = particle->TimeLived;//timeMs-particle->CreateTimeMs;


        particle->Color = particle->StartColor.getInterpolated_quadratic(Target0, Target1, livedTime/lifeTime);
    }
    irr::video::SColor Target0;
    irr::video::SColor Target1;

};

class GravityAffector : public irr::scene::particle::IParticleAffector
{
public:
    GravityAffector(const irr::core::vector3df& gravity)
    {
        Gravity = gravity;
    }
    void affect(irr::scene::particle::Particle* particle, irr::u32 timeMs, irr::f32 diff)
    {
        particle->Speed += Gravity*diff;
    }
    irr::core::vector3df Gravity;

};

class PlaneCollisionAffector : public irr::scene::particle::IParticleAffector
{
public:
    PlaneCollisionAffector(const irr::core::plane3df& plane)
    {
        Plane = plane;
    }
    void affect(irr::scene::particle::Particle* particle, irr::u32 timeMs, irr::f32 diff)
    {
        irr::core::vector3df collision;
        if (Plane.getIntersectionWithLimitedLine(particle->Position, particle->Position+particle->Speed*diff, collision))
        {
            irr::core::vector3df r = particle->Speed-(2*particle->Speed.dotProduct(Plane.Normal))*Plane.Normal;
            particle->Speed = r*0.5f;
        }
    }
    irr::core::plane3df Plane;
};

CMain::CMain()
{
    //ctor
    srand(5);
    Device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2du(800,600));
    smgr = Device->getSceneManager();
    driver = Device->getVideoDriver();
}

CMain::~CMain()
{
    //dtor
}

bool CMain::OnEvent(const irr::SEvent& event)
{
    if (event.EventType == irr::EET_KEY_INPUT_EVENT)
    {
        if (event.KeyInput.Key == irr::KEY_KEY_P && event.KeyInput.PressedDown)
        {
            irr::core::list<irr::scene::ISceneNode*> nodes = smgr->getRootSceneNode()->getChildren();
            irr::core::list<irr::scene::ISceneNode*>::Iterator it = nodes.begin();
            while(it != nodes.end())
            {
                irr::scene::ISceneNode* node = *it;
                if (node->getID() == 6)
                {
                    irr::scene::IParticleSystem* s = (irr::scene::IParticleSystem*)node;
                    s->Paused = !s->Paused;

                    if (s->Paused)
                        s->removeAnimators();
                    else
                    {
                        irr::scene::ISceneNodeAnimator* anim = smgr->createDeleteAnimator(3000);
                        s->addAnimator(anim);
                        anim->drop();
                    }

                }
                ++it;
            }
        }
    }
    else if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
    {
        //printf("MouseInput\n");
        if (event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN)
        {
            irr::core::line3d<irr::f32> ray;
            ray.setLine(smgr->getActiveCamera()->getAbsolutePosition(), smgr->getActiveCamera()->getAbsolutePosition()+(smgr->getActiveCamera()->getTarget()-smgr->getActiveCamera()->getAbsolutePosition())*1000.f);
            ray.end = ray.start+ray.getVector()*1000.f;
            irr::core::vector3df pos;
            irr::core::triangle3df triangle;
            irr::scene::ISceneNode* node=0;
            irr::core::plane3df pp(0,0,0,0,1,0);
            if (smgr->getSceneCollisionManager()->getCollisionPoint(ray, Selector, pos, triangle, node))
            {
                irr::core::vector3df n = triangle.getNormal();
                n.normalize();
                irr::core::plane3df p(pos, n);
                addExplosion(pos+n, n.getHorizontalAngle()+irr::core::vector3df(90,0,0), 1.f, smgr, p);
            }
            else if (pp.getIntersectionWithLine(ray.start, ray.getVector(), pos))
            {
                addExplosion(pos+irr::core::vector3df(0,1,0), irr::core::vector3df(0,0,0), 1.f, smgr, pp);
            }
        }
        else if (event.MouseInput.Event == irr::EMIE_MOUSE_MOVED)
        {
            MousePos.X = event.MouseInput.X;
            MousePos.Y = event.MouseInput.Y;
        }
    }
    return false;
}
void addIrr(const irr::core::vector3df& pos, irr::scene::ISceneManager* smgr);
void addMy(const irr::core::vector3df& pos, irr::scene::ISceneManager* smgr);
void CMain::run(void)
{
    irr::scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0, 100, 0.1);
    //irr::scene::ICameraSceneNode* cam = smgr->addCameraSceneNode();
    cam->setPosition(irr::core::vector3df(15.f));
    cam->setTarget(irr::core::vector3df(0.f));

    Device->getFileSystem()->addFileArchive("map-20kdm2.pk3");

    irr::scene::ISceneNode* node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("20kdm2.bsp"));
    node->setScale(irr::core::vector3df(0.1));
    node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    Selector = smgr->createTriangleSelector(smgr->getMesh("20kdm2.bsp")->getMesh(0), node);
    node->setTriangleSelector(Selector);

    irr::scene::IMetaTriangleSelector* meta = smgr->createMetaTriangleSelector();
    meta->addTriangleSelector(Selector);
    Selector = meta;

    for (int x=0; x<20; x++)
    {
        for (int y=0; y<20; y++)
        {
            //addMy(irr::core::vector3df(x*10,0,y*10), smgr);
        }
    }

    addMy(irr::core::vector3df(0,0,0), smgr);


    Device->setEventReceiver(this);

    irr::s32 fps = driver->getFPS();

    while (Device->run())
    {
        if (fps != driver->getFPS())
        {
            fps = driver->getFPS();
            irr::core::stringw c = L"FPS: ";
            c.append(irr::core::stringw(fps));
            Device->setWindowCaption(c.c_str());
        }
        driver->beginScene(true, true, irr::video::SColor(255,30,30,30));
        smgr->drawAll();
        driver->endScene();
    }
    Device->closeDevice();
    Device->drop();
}

int main(int argc, char* argv[])
{
    CMain m;
    m.run();

    //printf("MeshBuffers: %i\n", CParticleDrawer::BufferPool->size());
    //printf("Particles: %i\n", CParticleDrawer::ParticlePool.size());
    return 0;
}

void addIrr(const irr::core::vector3df& pos, irr::scene::ISceneManager* smgr)
{
    irr::scene::IParticleSystemSceneNode* system = smgr->addParticleSystemSceneNode(false);
    system->setPosition(pos);
    system->updateAbsolutePosition();

    irr::scene::IParticleEmitter* emitter = system->createPointEmitter(irr::core::vector3df(0,0.01,0), 200, 400, irr::video::SColor(255,200,180,0), irr::video::SColor(255,200,180,0), 2000, 5000, 45);
    system->setEmitter(emitter);
    emitter->drop();

    irr::scene::IParticleAffector* affector = system->createGravityAffector(irr::core::vector3df(0,-0.001,0), 5000);
    system->addAffector(affector);
    affector->drop();

    for (irr::u32 i=0; i<system->getMaterialCount(); ++i)
    {
        system->getMaterial(i).MaterialType = irr::video::EMT_ONETEXTURE_BLEND;
        system->getMaterial(i).setTexture(0, smgr->getVideoDriver()->getTexture("fig7_alpha.png"));
        system->getMaterial(i).setFlag(irr::video::EMF_LIGHTING, false);
        system->getMaterial(i).setFlag(irr::video::EMF_ZWRITE_ENABLE, false);
        system->getMaterial(i).MaterialTypeParam = irr::video::pack_textureBlendFunc (irr::video::EBF_SRC_ALPHA, irr::video::EBF_ONE_MINUS_SRC_ALPHA, irr::video::EMFN_MODULATE_1X, irr::video::EAS_VERTEX_COLOR | irr::video::EAS_TEXTURE );
    }
}

void addMy(const irr::core::vector3df& pos, irr::scene::ISceneManager* smgr)
{
    irr::scene::ISceneNode* node = smgr->addEmptySceneNode();

    //smgr->addAnimatedMeshSceneNode(smgr->getMesh("dwarf.x"), node)->setFrameLoop(0,0);

    irr::core::array<irr::core::vector3df> spline;
    spline.push_back(irr::core::vector3df(0,0,0));
    spline.push_back(irr::core::vector3df(0,0,200));

    irr::scene::ISceneNodeAnimator* rootNodeAnim = smgr->createRotationAnimator(irr::core::vector3df(0.2,0,0));
    node->addAnimator(rootNodeAnim);
    rootNodeAnim->drop();

    rootNodeAnim = smgr->createFollowSplineAnimator(0, spline, 0.3f);
    //node->addAnimator(rootNodeAnim);
    rootNodeAnim->drop();

    irr::scene::IParticleSystem* system = irr::scene::createParticleSystem(node, smgr, 6);
    system->setLocalParticles(true);
    system->setPosition(pos);
    //system->setRotation(irr::core::vector3df(45,0,0));
    system->updateAbsolutePosition();

    irr::scene::particle::IParticleDrawer* drawer = system->addParticleDrawer();

    drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.0f,0.0f), irr::core::vector2df(0.25f,0.0f), irr::core::vector2df(0.0f,0.25f), irr::core::vector2df(0.25f,0.25f)));
    drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.25f,0.25f), irr::core::vector2df(0.5f,0.25f), irr::core::vector2df(0.25f,0.5f), irr::core::vector2df(0.5f,0.5f)));
    drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.25f,0.0f), irr::core::vector2df(0.5f,0.0f), irr::core::vector2df(0.25f,0.25f), irr::core::vector2df(0.5f,0.25f)));
    drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.0f,0.25f), irr::core::vector2df(0.25f,0.25f), irr::core::vector2df(0.0f,0.5f), irr::core::vector2df(0.25f,0.5f)));

    irr::scene::particle::IParticleEmitter* emitter = drawer->addStandardEmitter(irr::core::vector3df(0,0,0), irr::core::vector3df(0,0,0), irr::core::vector3df(0,0,0), irr::core::vector3di(0, 0, 0), 1000, 2000, 0, 1000, 5000, irr::core::vector2df(1,1), irr::core::vector2df(2,2), irr::core::vector2df(0), irr::video::SColor(255,200,180,0), irr::video::SColor(255,200,180,0));
    IEmitterRegion* region = new CMeshRegion(smgr->getMesh("dwarf.x"), false);
    emitter->setRegion(region);
    region->drop();


    irr::scene::particle::IParticleAffector* affector = new GravityAffector(irr::core::vector3df(0,-90,0));
    //drawer->addAffector(affector);
    affector->drop();

    irr::video::SMaterial overrideMaterial;
    overrideMaterial.MaterialType = irr::video::EMT_ONETEXTURE_BLEND;
    overrideMaterial.setTexture(0, smgr->getVideoDriver()->getTexture("fire.bmp"));
    overrideMaterial.setFlag(irr::video::EMF_LIGHTING, false);
    overrideMaterial.setFlag(irr::video::EMF_ZWRITE_ENABLE, false);
    overrideMaterial.MaterialTypeParam = irr::video::pack_textureBlendFunc (irr::video::EBF_SRC_ALPHA, irr::video::EBF_ONE_MINUS_SRC_ALPHA, irr::video::EMFN_MODULATE_1X, irr::video::EAS_VERTEX_COLOR | irr::video::EAS_TEXTURE );
    system->setOverrideMaterial(overrideMaterial);

    irr::scene::ISceneNodeAnimator* anim = smgr->createFlyStraightAnimator(irr::core::vector3df(), irr::core::vector3df(0,0,500), 2000, true, true);
    //system->addAnimator(anim);
    anim->drop();
    anim = smgr->createRotationAnimator(irr::core::vector3df(0.1f,0.1f,0));
    //system->addAnimator(anim);
    anim->drop();


    system->drop();
}

void addExplosion(const irr::core::vector3df& pos, const irr::core::vector3df& rot, const irr::u32& scale, irr::scene::ISceneManager* smgr, const irr::core::plane3df& p)
{
    printf("Add Explosion\n");
    irr::scene::IParticleSystem* system = irr::scene::createParticleSystem(smgr->getRootSceneNode(), smgr, 6);
    system->setPosition(pos);
    system->setRotation(rot);
    system->setScale(irr::core::vector3df(scale));
    system->updateAbsolutePosition();

    ///Spark Trails
    irr::scene::particle::IParticleDrawer* drawer = system->addTrailParticleDrawer();

    irr::scene::particle::SParticleUV uv(irr::core::vector2df(0.75f,0.75f), irr::core::vector2df(1.0f,0.75f), irr::core::vector2df(0.75f,1.0f), irr::core::vector2df(1.0f,1.0f));
    uv.turnRight();
    drawer->addUVCoords(uv);

    irr::scene::particle::IParticleEmitter* emitter = drawer->addStandardEmitter(irr::core::vector3df(0,0,0), irr::core::vector3df(0,0,0), irr::core::vector3df(0,15,0), irr::core::vector3di(180, 0, 180), 0, 500, 30, 500, 1200, irr::core::vector2df(1.0), irr::core::vector2df(1.5), irr::core::vector2df(0,0), irr::video::SColor(255,255,255,0), irr::video::SColor(255,255,255,0));

    irr::scene::particle::IParticleAffector* affector = new ColorAffector(irr::video::SColor(0,100,50,0));
    drawer->addAffector(affector);
    affector->drop();

    ///Smoke Trails
    drawer = system->addTrailParticleDrawer();

    uv = irr::scene::particle::SParticleUV(irr::core::vector2df(0.0f,0.5f), irr::core::vector2df(0.5f, 0.5f), irr::core::vector2df(0.0f,0.5f+0.125f), irr::core::vector2df(0.5f,0.5f+0.125f));
    uv.turnRight();
    drawer->addUVCoords(uv);
    uv = irr::scene::particle::SParticleUV(irr::core::vector2df(0.0f,0.5f+0.125f), irr::core::vector2df(0.5f, 0.5f+0.125f), irr::core::vector2df(0.0f,0.5f+0.125f+0.125f), irr::core::vector2df(0.5f,0.5f+0.125f+0.125f));
    uv.turnRight();
    drawer->addUVCoords(uv);
    uv = irr::scene::particle::SParticleUV(irr::core::vector2df(0.0f,0.5f+0.125f+0.125f), irr::core::vector2df(0.5f, 0.5f+0.125f+0.125f), irr::core::vector2df(0.0f,0.5f+0.125f+0.125f+0.125f), irr::core::vector2df(0.5f,0.5f+0.125f+0.125f+0.125f));
    uv.turnRight();
    drawer->addUVCoords(uv);

    emitter = drawer->addStandardEmitter(irr::core::vector3df(0,0,0), irr::core::vector3df(0,0,0), irr::core::vector3df(0,10,0), irr::core::vector3di(180, 0, 180), 0, 500, 10, 400, 600, irr::core::vector2df(0.25,1.0), irr::core::vector2df(0.25,1.0), irr::core::vector2df(2.f), irr::video::SColor(255,255,128,50), irr::video::SColor(255,255,128,50));

    affector = new ColorAffectorQ(irr::video::SColor(128,128,128,50), irr::video::SColor(0,0,0,0));
    drawer->addAffector(affector);
    affector->drop();

    ///Debris
    drawer = system->addParticleDrawer();

    drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.5f,0.5f), irr::core::vector2df(0.5f+0.0833f,0.5f), irr::core::vector2df(0.5f,0.5f+0.0833f), irr::core::vector2df(0.5f+0.0833f,0.5f+0.0833f)));
    drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.5f+0.0833f,0.5f), irr::core::vector2df(0.5f+0.0833f+0.0833f,0.5f), irr::core::vector2df(0.5f+0.0833f,0.5f+0.0833f), irr::core::vector2df(0.5f+0.0833f+0.0833f,0.5f+0.0833f)));
    drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.5f+0.0833f+0.0833f,0.5f), irr::core::vector2df(0.5f+0.0833f+0.0833f+0.0833f,0.5f), irr::core::vector2df(0.5f+0.0833f+0.0833f,0.5f+0.0833f), irr::core::vector2df(0.5f+0.0833f+0.0833f+0.0833f,0.5f+0.0833f)));
    drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.5f,0.5f+0.0833f), irr::core::vector2df(0.5f+0.0833f,0.5f+0.0833f), irr::core::vector2df(0.5f,0.5f+0.0833f+0.0833f), irr::core::vector2df(0.5f+0.0833f,0.5f+0.0833f+0.0833f)));
    drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.5f,0.5f+0.0833f+0.0833f), irr::core::vector2df(0.5f+0.0833f,0.5f+0.0833f+0.0833f), irr::core::vector2df(0.5f,0.5f+0.0833f+0.0833f+0.0833f), irr::core::vector2df(0.5f+0.0833f,0.5f+0.0833f+0.0833f+0.0833f)));

    emitter = drawer->addStandardEmitter(irr::core::vector3df(0,0,0), irr::core::vector3df(1,0,0), irr::core::vector3df(0,12,0), irr::core::vector3di(45, 0, 45), 0, 400, 10, 2000, 3000, irr::core::vector2df(0.2,0.2), irr::core::vector2df(0.5,0.5), irr::core::vector2df(0,0), irr::video::SColor(255,255,100,0), irr::video::SColor(255,255,100,0));

    affector = new ColorAffectorQ(irr::video::SColor(255,50,50,50), irr::video::SColor(255,50,50,50));
    drawer->addAffector(affector);
    affector->drop();
    affector = new GravityAffector(irr::core::vector3df(0.f,-15.0f,0.f));
    drawer->addAffector(affector);
    affector->drop();
    affector = new PlaneCollisionAffector(p);
    drawer->addAffector(affector);
    affector->drop();

    ///Wave Emitter
    drawer = system->addOrientedParticleDrawer();
    drawer->getMaterial().BackfaceCulling = false;

    drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.75f,0.5f), irr::core::vector2df(1.0f,0.5f), irr::core::vector2df(0.75f,0.75f), irr::core::vector2df(1.0f,0.75f)));

    emitter = drawer->addStandardEmitter(irr::core::vector3df(0,0,0), irr::core::vector3df(0,0,0), irr::core::vector3df(0,0,0), irr::core::vector3di(0), 20, 50, 1, 500, 500, irr::core::vector2df(2.5,2.5), irr::core::vector2df(2.5,2.5), irr::core::vector2df(15), irr::video::SColor(255,200,100,0), irr::video::SColor(255,200,128,0));

    affector = new ColorAffector(irr::video::SColor(0,100,10,0));
    drawer->addAffector(affector);
    affector->drop();

    ///Fire/Smoke
    drawer = system->addParticleDrawer();

    drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.0f,0.0f), irr::core::vector2df(0.25f,0.0f), irr::core::vector2df(0.0f,0.25f), irr::core::vector2df(0.25f,0.25f)));
    drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.25f,0.25f), irr::core::vector2df(0.5f,0.25f), irr::core::vector2df(0.25f,0.5f), irr::core::vector2df(0.5f,0.5f)));
    drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.25f,0.0f), irr::core::vector2df(0.5f,0.0f), irr::core::vector2df(0.25f,0.25f), irr::core::vector2df(0.5f,0.25f)));
    drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.0f,0.25f), irr::core::vector2df(0.25f,0.25f), irr::core::vector2df(0.0f,0.5f), irr::core::vector2df(0.25f,0.5f)));

    emitter = drawer->addStandardEmitter(irr::core::vector3df(0,0,0), irr::core::vector3df(1,0,0), irr::core::vector3df(2,0,0), irr::core::vector3di(0, 180, 0), 200, 400, 50, 1000, 1200, irr::core::vector2df(1,1), irr::core::vector2df(2,2), irr::core::vector2df(2), irr::video::SColor(255,200,180,0), irr::video::SColor(255,200,180,0));

    affector = new ColorAffectorQ(irr::video::SColor(255, 100, 0, 0), irr::video::SColor(0,0,0,0));
    drawer->addAffector(affector);
    affector->drop();
    affector = new GravityAffector(irr::core::vector3df(0.f,10.0f,0.f));
    drawer->addAffector(affector);
    affector->drop();

    ///Spark Emitter
    drawer = system->addParticleDrawer();

    drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.5f,0.75f), irr::core::vector2df(0.75f,0.75f), irr::core::vector2df(0.5f,1.0f), irr::core::vector2df(0.75f,1.0f)));

    emitter = drawer->addStandardEmitter(irr::core::vector3df(0,0,0), irr::core::vector3df(0,0,0), irr::core::vector3df(0,6,0), irr::core::vector3di(90, 0, 90), 20, 50, 10, 1000, 1200, irr::core::vector2df(1.5, 1.5), irr::core::vector2df(2.0, 2.0), irr::core::vector2df(1,1), irr::video::SColor(255,255,128,0), irr::video::SColor(255,255,128,0));

    affector = new GravityAffector(irr::core::vector3df(0.f,0.8f,0.f));
    drawer->addAffector(affector);
    affector->drop();
    affector = new ColorAffector(irr::video::SColor(0,255,128,0));
    drawer->addAffector(affector);
    affector->drop();

    ///Flash Emitter
    drawer = system->addParticleDrawer();

    drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.5,0), irr::core::vector2df(0.75,0), irr::core::vector2df(0.5,0.25), irr::core::vector2df(0.75f,0.25f)));
    drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.5+0.25f,0), irr::core::vector2df(0.75+0.25f,0), irr::core::vector2df(0.5+0.25f,0.25), irr::core::vector2df(0.75f+0.25f,0.25f)));

    emitter = drawer->addStandardEmitter(irr::core::vector3df(0,0,0), irr::core::vector3df(0,0,0), irr::core::vector3df(0,0,0), irr::core::vector3di(0), 20, 50, 5, 200, 300, irr::core::vector2df(1,1), irr::core::vector2df(1,1), irr::core::vector2df(3), irr::video::SColor(255,255,255,100), irr::video::SColor(255,255, 255,100));

    affector = new ColorAffector(irr::video::SColor(0,255, 255,100));
    drawer->addAffector(affector);
    affector->drop();

    irr::video::SMaterial overrideMaterial;
    overrideMaterial.MaterialType = irr::video::EMT_ONETEXTURE_BLEND;
    overrideMaterial.setTexture(0, smgr->getVideoDriver()->getTexture("fig7_alpha.png"));
    overrideMaterial.setFlag(irr::video::EMF_LIGHTING, false);
    overrideMaterial.setFlag(irr::video::EMF_ZWRITE_ENABLE, false);
    overrideMaterial.setFlag(irr::video::EMF_BACK_FACE_CULLING, false);
    overrideMaterial.MaterialTypeParam = irr::video::pack_textureBlendFunc (irr::video::EBF_SRC_ALPHA, irr::video::EBF_ONE_MINUS_SRC_ALPHA, irr::video::EMFN_MODULATE_1X, irr::video::EAS_VERTEX_COLOR | irr::video::EAS_TEXTURE );
    system->setOverrideMaterial(overrideMaterial);
/*
    for (irr::u32 i=0; i<system->getMaterialCount(); ++i)
    {
        system->getMaterial(i).MaterialType = irr::video::EMT_ONETEXTURE_BLEND;
        system->getMaterial(i).setTexture(0, smgr->getVideoDriver()->getTexture("fig7_alpha.png"));
        system->getMaterial(i).setFlag(irr::video::EMF_LIGHTING, false);
        system->getMaterial(i).setFlag(irr::video::EMF_ZWRITE_ENABLE, false);
        system->getMaterial(i).MaterialTypeParam = irr::video::pack_texureBlendFunc (irr::video::EBF_SRC_ALPHA, irr::video::EBF_ONE_MINUS_SRC_ALPHA, irr::video::EMFN_MODULATE_1X, irr::video::EAS_VERTEX_COLOR | irr::video::EAS_TEXTURE );
    }
*/

    irr::scene::ISceneNodeAnimator* anim = smgr->createDeleteAnimator(3000);
    system->addAnimator(anim);
    anim->drop();

    system->drop();
}
