#include "IMetaTriangleSelector.h"
#include "CPlayerAnimatorAI.h"
#include "CPong.h"
#include "CPlayer.h"
#include "CBall.h"
#include "CGUIMenu.h"
namespace irr{
namespace game{
CPong::CPong(){
    Device = createDevice(video::EDT_OPENGL, core::dimension2d<irr::u32>(RES_X,RES_Y), 32, false, false, false, 0);
    Env = Device->getGUIEnvironment();
    Driver = Device->getVideoDriver();
    Smgr=Device->getSceneManager();

    Smgr->addLightSceneNode(NULL,core::vector3df(),video::SColorf(0.3,0.3,0.3),30);
    Device->setEventReceiver(this);
    // create tunnel
    const static float tunnelSize = 10.0f;
    Tunnel = Smgr->addMeshSceneNode(Smgr->getMesh("media/tunnel.b3d"));
    Tunnel->setMaterialFlag(video::EMF_LIGHTING,false);
    Tunnel->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    Tunnel->setScale(core::vector3df(tunnelSize,tunnelSize,tunnelSize));
    // create player 1
    P1 = new CPlayer(Device);
    P1->Paddle->setPosition(core::vector3df(0.0f,0.0f,9.5f));
    P1->Paddle->setRotation(core::vector3df(0,0,0));
    P1->Animator->KeyToAction[KEY_KEY_W]=CPlayerAnimator::UP;
    P1->Animator->KeyToAction[KEY_KEY_S]=CPlayerAnimator::DOWN;
    P1->Animator->KeyToAction[KEY_KEY_A]=CPlayerAnimator::LEFT;
    P1->Animator->KeyToAction[KEY_KEY_D]=CPlayerAnimator::RIGHT;
    P1->setTunnelSize(tunnelSize,tunnelSize);
    // create player 2
    P2= new CPlayer(Device);
    P2->Paddle->setPosition(core::vector3df(0.0f,0.0f,-9.5f));
    P2->Paddle->setRotation(core::vector3df(0,180,0));
    P2->Animator->KeyToAction[KEY_UP]=CPlayerAnimator::UP;
    P2->Animator->KeyToAction[KEY_DOWN]=CPlayerAnimator::DOWN;
    P2->Animator->KeyToAction[KEY_LEFT]=CPlayerAnimator::LEFT;
    P2->Animator->KeyToAction[KEY_RIGHT]=CPlayerAnimator::RIGHT;
    P2->setTunnelSize(tunnelSize,tunnelSize);
    //create ball and the selectors it needs to bounce
    B1 = new CBall(Device);
    scene::IMetaTriangleSelector* selector = Smgr->createMetaTriangleSelector();
    scene::ITriangleSelector* tunnelSelector = Smgr->createTriangleSelector(Tunnel->getMesh(),Tunnel);
    scene::ITriangleSelector* paddle1Selector = Smgr->createTriangleSelector(P1->Paddle->getMesh(),P1->Paddle);
    scene::ITriangleSelector* paddle2Selector = Smgr->createTriangleSelector(P2->Paddle->getMesh(),P2->Paddle);
    selector->addTriangleSelector(tunnelSelector);
    selector->addTriangleSelector(paddle1Selector);
    selector->addTriangleSelector(paddle2Selector);
    tunnelSelector->drop();
    paddle1Selector->drop();
    paddle2Selector->drop();
    B1->Animator->setTriangleSelector(selector);
    //add the computer control
    AILeft  = new CPlayerAnimatorAI(P1->Animator,B1);
    P1->Paddle->addAnimator(AILeft);
    AIRight = new CPlayerAnimatorAI(P2->Animator,B1);
    P2->Paddle->addAnimator(AIRight);
    //add gui stuff
    Env->getSkin()->setFont(Env->getFont("media/font.png"));
    GUIMenu = new CGUIMenu(Env,core::recti(0,0,RES_X,RES_Y));
    GUIMenu->drop();
    GUIMenu->toMenuMode();
    //set the state
    State=MENU;
}
bool CPong::OnEvent(const SEvent& event){
    if(event.EventType==EET_GUI_EVENT){
        if(event.GUIEvent.EventType==gui::EGET_BUTTON_CLICKED){
            if(event.GUIEvent.Caller == GUIMenu->ButtonQuit){
                Device->closeDevice();
            }else if(event.GUIEvent.Caller == GUIMenu->ButtonStart){
                GUIMenu->toGameMode();
                if(GUIMenu->useAutoControlLeft()){
                    AILeft->setEnabled(true);
                }else{
                    addReceiver(P1->Animator);
                }
                if(GUIMenu->useAutoControlRight()){
                    AIRight->setEnabled(true);
                }else{
                    addReceiver(P2->Animator);
                }
                P1->Life = 5;
                P2->Life = 5;
                GUIMenu->setScore(P1->Life,P2->Life);
                B1->Animator->Velocity=core::vector3df(8,-1,2);
                State=INTER;
            }
        }
    }
    if(CEventReceiverChainer::OnEvent(event))
        return true;

    return false;
}
void CPong::run(){
    scene::ISceneNode* sky = Smgr->addEmptySceneNode();
    while(Device->run()){
        core::dimension2du res = Driver->getCurrentRenderTargetSize();
        Device->getVideoDriver()->beginScene(true, true);
        Driver->setViewPort(core::recti(0,0,res.Width/2,res.Height));
        sky->remove();
        sky = Smgr->addSkyDomeSceneNode(Driver->getTexture("media/skydome1.jpg"));
        P1->drawAll();
        Driver->setViewPort(core::recti(res.Width/2,0,res.Width,res.Height));
        sky->remove();
        sky = Smgr->addSkyDomeSceneNode(Driver->getTexture("media/skydome2.jpg"));
        P2->drawAll();
        Driver->setViewPort(core::recti(0,0,res.Width,res.Height));
        Env->drawAll();
        Driver->endScene();
        if(B1->Animator->Velocity.Z>0){
            B1->Bill->setMaterialTexture(0,Driver->getTexture("media/ball2.png"));
        }else{
            B1->Bill->setMaterialTexture(0,Driver->getTexture("media/ball1.png"));
        }
        switch(State){
        case GAME:
            if(P1->isBehind(B1->Bill->getPosition())){
                P1->Life-=1;
                GUIMenu->setScore(P1->Life,P2->Life);
                if(P1->Life!=0){
                    B1->Animator->Velocity=core::vector3df(8.0f,-1.0f,-2.0f);
                    State=INTER;
                }else{
                    gameOver(PLAYER_LEFT);
                }
            }else if(P2->isBehind(B1->Bill->getPosition())){
                P2->Life-=1;
                GUIMenu->setScore(P1->Life,P2->Life);
                if(P2->Life!=0){
                    B1->Animator->Velocity=core::vector3df(8.0f,-1.0f,2.0f);
                    State=INTER;
                }else{
                    gameOver(PLAYER_LEFT);
                }
            }
            break;
        case MENU:
            break;
        case INTER:
            B1->Bill->setPosition(core::vector3df(0,0,0));
            State = GAME;
            break;
        default:
            break;
        }
                        Device->sleep(1);
    }

}
CPong::~CPong(){
    AILeft->drop();
    AIRight->drop();
    GUIMenu->remove();
    Tunnel->remove();
    B1->drop();
    P1->drop();
    P2->drop();
    Device->drop();
}
void CPong::gameOver(s32 winner){
    if(winner==PLAYER_LEFT){
        GUIMenu->setWinnerMessage(true,false);
    }else{
        GUIMenu->setWinnerMessage(false,true);
    }
    AILeft->setEnabled(false);
    AIRight->setEnabled(false);
    P1->Animator->allKeyUp();
    P2->Animator->allKeyUp();
    removeReceiver(P1->Animator);
    removeReceiver(P2->Animator);
    GUIMenu->toMenuMode();
    State=MENU;
}
}}//end irr::game;
