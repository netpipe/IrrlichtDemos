#include "CBall.h"
#include "ISceneManager.h"
#include "IVideoDriver.h"
#include "ISceneNode.h"
#include "IMeshSceneNode.h"
#include "CBallAnimator.h"
namespace irr{
namespace game{
CBall::CBall(IrrlichtDevice* device):Device(device){
    Device->grab();
    Bill = Device->getSceneManager()->addBillboardSceneNode(NULL,core::dimension2df(2.0f,2.0f));
    Bill-> setMaterialTexture(0,Device->getVideoDriver()->getTexture("media/ball1.bmp"));
    Bill-> setMaterialFlag(video::EMF_LIGHTING,false);
    Bill-> setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    Animator=new CBallAnimator();
    Bill->addAnimator(Animator);
}
CBall::~CBall(){
    Animator->drop();
    Bill->remove();
    Device->drop();
}
}}//
