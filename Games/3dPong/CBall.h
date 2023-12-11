#ifndef _CBALL_H_
#define _CBALL_H_
#include "IrrlichtDevice.h"
#include "ILightSceneNode.h"
#include "IBillboardSceneNode.h"
#include "CBallAnimator.h"
namespace irr{
namespace game{
class CBall: public IReferenceCounted{
    public:
        explicit CBall(IrrlichtDevice* device);
        ~CBall();
        CBallAnimator* Animator;
        scene::IBillboardSceneNode* Bill;
    protected:
        IrrlichtDevice* Device;
        scene::ILightSceneNode* Light;
    private:
        CBall(const CBall& other);
        CBall& operator=(const CBall&other);
};
}}//
#endif
