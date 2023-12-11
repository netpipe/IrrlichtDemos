#include "CPlayerAnimator.h"
#include "assert.h"
namespace irr{
namespace game{
CPlayerAnimator::CPlayerAnimator(){
    LastTime=0;
    allKeyUp();
    ControlEnabled = true;
}
void CPlayerAnimator::allKeyUp(){
    for(int i=0;i<COUNT;++i)
        ActionDown[Action(i)]=false;
}
bool CPlayerAnimator::OnEvent(const SEvent& event){
    if(event.EventType==EET_KEY_INPUT_EVENT){
        s32 key = event.KeyInput.Key;
        if(KeyToAction.find(key)){
            ActionDown[KeyToAction[key]]=event.KeyInput.PressedDown;
        }
    }
    return false;
}
void CPlayerAnimator::animateNode(scene::ISceneNode *node, u32 timeMs){
    const static float moveSpeed=8.0f;
    core::vector3df pos = node->getPosition();
    if(!LastTime){
        LastTime = timeMs;
        return;
    }
	f32 frameDelta = float((timeMs - LastTime))/1000.0f;
	LastTime = timeMs;
	if(ControlEnabled){
        float charAngle =  node->getRotation().Y;
        if(!(ActionDown[UP] && ActionDown[DOWN])){
            if(ActionDown[UP]){
                pos.Y += frameDelta*moveSpeed;
                if(pos.Y>MaxY)
                    pos.Y=MaxY;
            }else if(ActionDown[DOWN]){
                pos.Y -= frameDelta*moveSpeed;
                if(pos.Y<MinY)
                    pos.Y=MinY;
            }
        }
        if(!(ActionDown[LEFT] && ActionDown[RIGHT])){
            if(ActionDown[LEFT]){
                pos.X += cos((charAngle)*core::DEGTORAD)*frameDelta*moveSpeed;
            }else if(ActionDown[RIGHT]){
                pos.X -= cos((charAngle)*core::DEGTORAD)*frameDelta*moveSpeed;
            }
            if(pos.X>MaxX)
                pos.X=MaxX;
            if(pos.X<MinX)
                pos.X=MinX;
        }
        node->setPosition(pos);
	}
    return;
}
scene::ISceneNodeAnimator* CPlayerAnimator::createClone(scene::ISceneNode* node,scene::ISceneManager* newManager){
    assert(false);
    return NULL;
}
}}//end irr::game
