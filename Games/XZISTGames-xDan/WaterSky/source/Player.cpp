
#include "Player.h"

Player::Player(WorldBase *world, core::vector3df pos)
    : Actor(world)
{
    sceneNode = smgr->addAnimatedMeshSceneNode(smgr->getMesh("twinkle.b3d"));
    sceneNode->setPosition(pos);
    sceneNode->updateAbsolutePosition();
    
    // player light (bobble!)
    
	scene::ILightSceneNode *light = smgr->addLightSceneNode(sceneNode,
        core::vector3df(0,
            sceneNode->getBoundingBox().MaxEdge.Y,
            0.6),
        video::SColorf(), 1/0.4);
	video::SLight &slight = light->getLightData();
	slight.AmbientColor = video::SColorf(0.0,0.0,0.0);
	slight.DiffuseColor = video::SColorf(0.5,0.5,0.5);
	slight.SpecularColor = video::SColorf(0.5,0.5,0.5);
	
	scene::ISceneNode *node = smgr->addBillboardSceneNode(light, core::dimension2d<f32>(0.5,0.5));
	node->setMaterialFlag(video::EMF_LIGHTING, false);
	node->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	node->setMaterialTexture(0, driver->getTexture("bobble.tga"));
    
    defineAnimation(ANIM_IDLE, 0, 41, NULL);
    defineAnimation(ANIM_WALK, 0, 41, "media/sfx/step.wav");
    
    setAnimation(ANIM_IDLE);
    
    createSoundAnimator();
    enableCollisionResponse();
}

Object *Player::createNew()
{
    return new Player(this);
}
