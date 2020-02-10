
#include "UFO.h"

UFO::UFO(IrrlichtDevice *devicesengine) : Target(device)
{
    sn = smgr->addAnimatedMeshSceneNode(smgr->getMesh("ufo.x"));
    //((scene::IAnimatedMeshSceneNode *)sn)->setAnimationSpeed(100);
    sn->setRotation(core::vector3df(90,0,0));

    //f32 scale = 0.5;
    //sn->setScale(core::vector3df(scale,scale,scale));
    bbExtent = sn->getBoundingBox().getExtent();

    // choose start position
    int side = rand()%2;
    core::vector3df positions[2] =
    {
        core::vector3df(-1,0,0),
        core::vector3df(1,0,0)
    };
    core::vector3df pos = positions[side];
    pos.X *= 9;
    pos.Y = 0.2;//2 + rand()%4;
    pos.Z = 1 + rand()%5;
    sn->setPosition(pos);

    // movement
    vel.X = side ? -1 : 1;
    vel.X *= 1.0;

    for (int i = 0; i < sn->getMaterialCount(); i ++)
    {
    ///    video::ITexture *texture = sn->getMaterial(i).Textures[0];
    ///    if (texture)
    ///    {
      ///      if (texture->getName() == "ufo_glass.tga")
      ///      {
      ///          sn->getMaterial(i).MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
      ///      }
    ///    }
    }
}

void UFO::secondUpdate(double dt)
{
    core::vector3df pos = sn->getPosition();

    if (fabs(pos.X) > 10)
    {
        removable = true;
    }
}

void UFO::hitScore()
{
    score += 500;
    shellsGiven += 100;
}

void UFO::hitEffects(core::line3df ray)
{
    inactive = true;
    removable = true;
}
