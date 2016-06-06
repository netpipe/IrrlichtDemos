#ifndef _CPLAYER_H_
#define _CPLAYER_H_
#include "IrrlichtDevice.h"
#include "IMeshSceneNode.h"
#include "ICameraSceneNode.h"
#include "CPlayerAnimator.h"
#include "ILightSceneNode.h"
#include "IReferenceCounted.h"
namespace irr{
namespace game{
class CPlayer: public IReferenceCounted{
    public:
        explicit CPlayer(IrrlichtDevice* device);
        ~CPlayer();
        //!
        void setControlEnabled(bool);
        //! This switchs the camera and draws to the current viewport
        void drawAll();
        //! Full fledge collision detection is not needed, a simple bound check is good enough
        void setTunnelSize(f32 x,f32 y);
        //! Used to check and see if a ball has past the paddle
        bool isBehind(const core::vector3df point);

        scene::IMeshSceneNode* Paddle;
        scene::ICameraSceneNode* Camera;
        CPlayerAnimator* Animator;
        s32 Life;
    protected:
        enum{PADDLE_SIZE=2};
        IrrlichtDevice* Device;
        scene::ILightSceneNode* Light;
    private:
        CPlayer(const CPlayer& other);
        CPlayer& operator=(const CPlayer& other);
};
}}//end irr::game
#endif
