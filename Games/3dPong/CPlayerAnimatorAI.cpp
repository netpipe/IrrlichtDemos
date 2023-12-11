#include "ISceneManager.h"
#include "CPlayerAnimatorAI.h"
#include "CPlayerAnimator.h"
#include "ISceneNode.h"
#include "assert.h"
namespace irr{
namespace game{
CPlayerAnimatorAI::CPlayerAnimatorAI(CPlayerAnimator* controllee, CBall* ball):Controllee(controllee),Ball(ball){
    Controllee->grab();
    Ball->grab();
    Enabled = false;
}
CPlayerAnimatorAI::~CPlayerAnimatorAI(){
    Ball->drop();
    Controllee->drop();
}
void CPlayerAnimatorAI::setEnabled(bool enable){
    Enabled=enable;
}
void CPlayerAnimatorAI::animateNode(scene::ISceneNode *node, u32 timeMs){
    if(Enabled){
        core::vector3df paddlePos = node->getPosition();
        core::vector3df ballPosRaw = Ball->Bill->getPosition();
        //get ballPos in paddle corrdinate
        irr::core::matrix4 paddleWorld = node->getAbsoluteTransformation();
        paddleWorld.makeInverse();
        scene::ISceneNode* null = node->getSceneManager()->addEmptySceneNode();
        null->setPosition(ballPosRaw);
        null->updateAbsolutePosition();
        irr::core::matrix4 nullWorld = null->getAbsoluteTransformation();
        irr::core::matrix4 nullMatrix = paddleWorld*nullWorld;
        core::vector3df ballPos = nullMatrix.getTranslation();
        null->remove();
        //no moving when difference is smaller than episolon to prevent jiggling
        const static float episolon = 0.01;
        if(ballPos.X < 0){
            if(fabs(ballPos.X)>=episolon)
                Controllee->ActionDown[CPlayerAnimator::RIGHT] = true;
            Controllee->ActionDown[CPlayerAnimator::LEFT]  = false;
        }else{
            Controllee->ActionDown[CPlayerAnimator::RIGHT] = false;
            if(fabs(ballPos.X)>=episolon)
                Controllee->ActionDown[CPlayerAnimator::LEFT]  = true;
        }
        if(ballPos.Y > 0){
            if(fabs(ballPos.Y)>=episolon)
                Controllee->ActionDown[CPlayerAnimator::UP] = true;
            Controllee->ActionDown[CPlayerAnimator::DOWN]  = false;
        }else{
            Controllee->ActionDown[CPlayerAnimator::UP] = false;
            if(fabs(ballPos.Y)>=episolon)
                Controllee->ActionDown[CPlayerAnimator::DOWN]  = true;
        }
    }
}
scene::ISceneNodeAnimator* CPlayerAnimatorAI::createClone(scene::ISceneNode* node,scene::ISceneManager* newManager){
    assert(false);
    return NULL;
}
}}//end irr::game
