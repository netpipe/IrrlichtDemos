#ifndef _CPLAYERANIMATORAI_H_
#define _CPLAYERANIMATORAI_H_
#include "CPlayerAnimator.h"
#include "CBall.h"
namespace irr{
namespace game{
class CPlayerAnimatorAI: public scene::ISceneNodeAnimator{
    public:
        CPlayerAnimatorAI(CPlayerAnimator* controllee, CBall* ball);
        ~CPlayerAnimatorAI();
        void setEnabled(bool enabled);
        void animateNode(scene::ISceneNode *node, u32 timeMs);
    protected:
        CPlayerAnimator* Controllee;
        CBall* Ball;
        bool Enabled;
    private:
        CPlayerAnimatorAI(const CPlayerAnimatorAI& other);
        CPlayerAnimatorAI& operator=(const CPlayerAnimatorAI& other);
        virtual scene::ISceneNodeAnimator* createClone(scene::ISceneNode* node,scene::ISceneManager* newManager=0);
};
}}//end irr::game
#endif
