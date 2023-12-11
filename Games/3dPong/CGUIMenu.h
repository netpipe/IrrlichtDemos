#ifndef _CGUIMENU_H_
#define _CGUIMENU_H_
#include "IGUIElement.h"
#include "IGUIButton.h"
#include "IGUICheckBox.h"
#include "IGUIStaticText.h"
namespace irr{
namespace game{
class CGUIMenu:public gui::IGUIElement{
    public:
        enum{MSG_NICHTS,MSG_START,MSG_QUIT};
        CGUIMenu(gui::IGUIEnvironment* environment,core::recti rectangle);
        ~CGUIMenu();
        virtual bool OnEvent(const SEvent& event);
        virtual bool useAutoControlLeft();
        virtual bool useAutoControlRight();
        virtual void toMenuMode();
        virtual void toGameMode();
        virtual void setScore(s32 scoreLeft,s32 scoreRight);
        virtual void setWinnerMessage(bool leftWin,bool rightWin);
        gui::IGUIButton* ButtonStart;
        gui::IGUIButton* ButtonQuit;
    protected:
        gui::IGUIStaticText* UseAIText;
        gui::IGUICheckBox* CheckBoxLeft;
        gui::IGUICheckBox* CheckBoxRight;
        gui::IGUIStaticText* ScoreLeft;
        gui::IGUIStaticText* ScoreRight;
        gui::IGUIStaticText* MessageLeft;
        gui::IGUIStaticText* MessageRight;
    private:
        CGUIMenu(const CGUIMenu& other);
        CGUIMenu& operator=(const CGUIMenu& other);
};
}}//
#endif
