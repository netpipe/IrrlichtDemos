#include "Player.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

Player::Player(ISceneManager* mgr)
{
    smgr=mgr;

    mesh=smgr->getMesh("../media/bandido.b3d");
    node=smgr->addAnimatedMeshSceneNode(mesh);

    node->setFrameLoop(70,90);
    node->setAnimationSpeed(30);
    node->setScale(vector3df(1.5,1.5,1.5));

    node->setMaterialFlag(EMF_NORMALIZE_NORMALS,true);

    meshOuro=smgr->getMesh("../media/ouro.b3d");
    nodeOuro=smgr->addAnimatedMeshSceneNode(meshOuro);

    nodeOuro->setMaterialFlag(EMF_NORMALIZE_NORMALS,true);
    nodeOuro->setMaterialFlag(EMF_LIGHTING,false);

    nodeOuro->setAnimationSpeed(30);

    nodeOuro->setScale(vector3df(1.5,1.5,1.5));

    shadowMesh=smgr->getMesh("../media/sombra.b3d");
    shadowNode=smgr->addMeshSceneNode(shadowMesh,node);
    shadowNode->setPosition(vector3df(0,0.02,0));
    shadowNode->setScale(vector3df(0.5,0.5,0.7));
    shadowNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);

    currentAction=PWAIT;

    walkSpeed = 0.08;

    cam=smgr->addCameraSceneNode();
    cam->setPosition(node->getPosition()+vector3df(0,10,-5));
    cam->setTarget(node->getPosition());
}

ISceneNode* Player::getNode()
{
    return node;
}

vector3df Player::getPosition()
{
    return node->getPosition();
}

void Player::setAnimation(ANIM anim)
{
    if(anim == PLAYER_ANIM_WAIT)
    {
        node->setFrameLoop(41,41);
        nodeOuro->setVisible(false);
    }

    if(anim == PLAYER_ANIM_WAIT_GOLD)
    {
        if(node->getStartFrame() != 71)
        {
            node->setFrameLoop(71,71);
            nodeOuro->setFrameLoop(71,71);
            nodeOuro->setVisible(true);
        }
    }

    if(anim == PLAYER_ANIM_WALK)
    {
        if(node->getStartFrame() != 40)
        {
            node->setFrameLoop(40,60);
            nodeOuro->setVisible(false);
        }
    }

    if(anim == PLAYER_ANIM_WALK_GOLD)
    {
        if(node->getStartFrame() != 70)
        {
            node->setFrameLoop(70,90);
            nodeOuro->setFrameLoop(70,90);
            nodeOuro->setVisible(true);
        }
    }
}

void Player::setPosition(vector3df pos)
{
    node->setPosition(pos);
    nodeOuro->setPosition(pos);
    cam->setPosition(node->getPosition()+vector3df(0,10,-5));
    cam->setTarget(node->getPosition());
}

void Player::walk(PWALK walkDir, bool ouroNoSaco)
{
    if(walkDir == PWALK_L)
    {
        if(currentAction != PWALK_L)
        {
            if(ouroNoSaco)
                setAnimation(PLAYER_ANIM_WALK_GOLD);
            else
                setAnimation(PLAYER_ANIM_WALK);
            node->setRotation(vector3df(0,90,0));
        }

        node->setPosition(node->getPosition()+vector3df(-walkSpeed,0,0));
    }
    else if(walkDir == PWALK_R)
    {
        if(currentAction != PWALK_R)
        {
            if(ouroNoSaco)
                setAnimation(PLAYER_ANIM_WALK_GOLD);
            else
                setAnimation(PLAYER_ANIM_WALK);
            node->setRotation(vector3df(0,-90,0));
        }
        node->setPosition(node->getPosition()+vector3df(walkSpeed,0,0));
    }
    else if(walkDir == PWALK_U)
    {
        if(currentAction != PWALK_U)
        {
            if(ouroNoSaco)
                setAnimation(PLAYER_ANIM_WALK_GOLD);
            else
                setAnimation(PLAYER_ANIM_WALK);
            node->setRotation(vector3df(0,180,0));
        }
        node->setPosition(node->getPosition()+vector3df(0,0,walkSpeed));
    }
    else if(walkDir == PWALK_D)
    {
        if(currentAction != PWALK_D)
        {
            if(ouroNoSaco)
                setAnimation(PLAYER_ANIM_WALK_GOLD);
            else
                setAnimation(PLAYER_ANIM_WALK);
            node->setRotation(vector3df(0,0,0));
        }
        node->setPosition(node->getPosition()+vector3df(0,0,-walkSpeed));
    }

    cam->setPosition(node->getPosition()+vector3df(0,10,-5));
    cam->setTarget(node->getPosition());

    nodeOuro->setPosition(node->getPosition());
    nodeOuro->setRotation(node->getRotation());
}
