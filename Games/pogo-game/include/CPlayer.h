#ifndef CPLAYER_H
#define CPLAYER_H

#include "irrlicht.h"
#include "Utils.h"
#include "IWindGenerator.h"
using namespace irr;




class CPlayer
{
//stops animations from looping,
class animCallback: public scene::IAnimationEndCallBack
    {
    public:

        virtual void OnAnimationEnd(scene::IAnimatedMeshSceneNode* node)
        {
            node->setFrameLoop (0, 0);
        }
    };
public:
    CPlayer(f32 mmass,scene::ISceneManager* mgr,scene::ICameraSceneNode* mcam,btDiscreteDynamicsWorld*mdynamicsWorld,s32 mat,int windPower);
    ~CPlayer();

    void update(f32 gravity,f32 time,f32 dt);
    void applyForce(core::vector3df appForce,bool overide);
    core::vector3df pos_now;
    core::vector3df getVel();
    void showJumpCombo()
    {
        jumpComboNode->setFrameLoop (0, 200);
        coinAirNode->setFrameLoop (0, 0);
        specialCoinNode->setFrameLoop (0, 0);
    };
    void showCoinAirCombo()
    {
        coinAirNode->setFrameLoop (0, 200);
        jumpComboNode->setFrameLoop (0, 0);
        specialCoinNode->setFrameLoop (0, 0);
    };
    void showSpecialCoinCombo()
    {
        specialCoinNode->setFrameLoop (0, 200);
        coinAirNode->setFrameLoop (0,0);
        jumpComboNode->setFrameLoop (0, 0);
    };
protected:
private:
    //Players body
    btRigidBody* body;
    btDiscreteDynamicsWorld* dynamicsWorld;

    f32 mass;

    //HUD nodes
    scene::IAnimatedMeshSceneNode* jumpComboNode;
    scene::IAnimatedMeshSceneNode* coinAirNode;
    scene::IAnimatedMeshSceneNode* specialCoinNode;
    scene::IAnimatedMeshSceneNode* flagNode;

    scene::ICameraSceneNode* cam;
    scene::IWindGenerator* windGen;

    core::vector3df lastAngle;


};

#endif // CPLAYER_H
