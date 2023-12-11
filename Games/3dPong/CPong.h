#ifndef _CPONG_H_
#define _CPONG_H_
#include "CEventReceiverChainer.h"
#include "irrlicht.h"
#include "CPlayerAnimatorAI.h"
namespace irr{
namespace game{
class CBall;
class CPlayer;
class CGUIMenu;
class CPong: public CEventReceiverChainer{
    public:
        CPong();
        ~CPong();
        virtual bool OnEvent(const SEvent& event);
        virtual void run();
    protected:
        enum state{MENU,GAME,INTER};
        enum {RES_X=800,RES_Y=400};
        enum {WD_MENU,WD_SETTING};
        enum {BT_START,BT_SETTING,BT_QUIT};
        enum {PLAYER_LEFT,PLAYER_RIGHT};
        void gameOver(s32 winner);
        state State;
        scene::IMeshSceneNode* Tunnel;
        irr::IrrlichtDevice* Device;
        scene::ISceneManager* Smgr;
        gui::IGUIEnvironment* Env;
        video::IVideoDriver* Driver;
        CPlayer* P1;
        CPlayer* P2;
        CBall* B1;
        gui::IGUIStaticText* ScoreLeft;
        gui::IGUIStaticText* ScoreRight;
        gui::IGUIButton* ButtonStart;
        gui::IGUIButton* ButtonQuit;
        CPlayerAnimatorAI* AILeft ;
        CPlayerAnimatorAI* AIRight;
        CGUIMenu* GUIMenu;
    private:
        CPong(const CPong& other);
        CPong& operator=(const CPong&other);
};
}}//
#endif
