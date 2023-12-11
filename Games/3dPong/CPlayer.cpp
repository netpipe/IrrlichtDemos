#include "CPlayer.h"
#include "ISceneManager.h"
#include "assert.h"
namespace irr{
namespace game{
CPlayer::CPlayer(IrrlichtDevice* device):Device(device){
    Device->grab();
    scene::ISceneManager* Smgr=device->getSceneManager();
    Camera=Smgr->addCameraSceneNode();
    Paddle=Smgr->addMeshSceneNode(Smgr->getMesh("media/paddle.b3d"));
    Paddle->setScale(core::vector3df(PADDLE_SIZE,PADDLE_SIZE,1));
    Paddle->setMaterialFlag(video::EMF_LIGHTING,false);
    Paddle->setMaterialFlag(video::EMF_BACK_FACE_CULLING,false);
    Paddle->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
    Camera->setPosition(core::vector3df(0,0,15));
    Camera->setParent(Paddle);
    Light = Smgr->addLightSceneNode(Paddle,core::vector3df(2,2,10),video::SColorf(0.3,0.3,0.3),20);
    Animator = new CPlayerAnimator();
    Paddle->addAnimator(Animator);
}
CPlayer::~CPlayer(){
    Animator->drop();
    Light->remove();
    Camera->remove();
    Paddle->remove();
    Device->drop();
}
void CPlayer::setTunnelSize(f32 x,f32 y){
    Animator->MaxX = x-PADDLE_SIZE;
    Animator->MinX = -Animator->MaxX;
    Animator->MaxY = y-PADDLE_SIZE;
    Animator->MinY = -Animator->MaxY;
}
void CPlayer::setControlEnabled(bool enable){
    Animator->ControlEnabled= enable;
}
void CPlayer::drawAll(){
    core::dimension2du res = Device->getVideoDriver()->getCurrentRenderTargetSize();
    Camera->setAspectRatio(float(res.Width)/res.Height/2);
    Camera->setTarget(core::vector3df());
    scene::ISceneManager* Smgr = Device->getSceneManager();
    Smgr->setActiveCamera(Camera);
    Smgr->drawAll();
}
bool CPlayer::isBehind(const core::vector3df point){
    irr::core::matrix4 paddleWorld = Paddle->getAbsoluteTransformation();
    paddleWorld.makeInverse();
    scene::ISceneNode* null = Paddle->getSceneManager()->addEmptySceneNode();
    null->setPosition(point);
    null->updateAbsolutePosition();
    irr::core::matrix4 nullWorld = null->getAbsoluteTransformation();
    irr::core::matrix4 nullMatrix = paddleWorld*nullWorld;
    core::vector3df pos = nullMatrix.getTranslation();
    null->remove();
    if(pos.Z >= 10){
        return true;
    }else{
        return false;
    }
}
}}//end irr::game
