#include "CGUIMenu.h"
#include "IGUIEnvironment.h"
namespace irr{
namespace game{
CGUIMenu::CGUIMenu(gui::IGUIEnvironment* environment, core::recti rectangle):gui::IGUIElement(gui::EGUIET_ELEMENT,environment, environment->getRootGUIElement(), -1, rectangle){
    setAlignment(gui::EGUIA_CENTER,gui::EGUIA_CENTER,gui::EGUIA_CENTER,gui::EGUIA_CENTER);
    static const u32 border = 10;
    static const u32 width = 100;
    static const u32 height = 30;
    core::position2di size(width,height);
    //the computer player text
    core::position2di useAIul((rectangle.getWidth()-width)/2,border);
    UseAIText = Environment->addStaticText(L"  Computer",core::recti(useAIul,useAIul+size),false,true,this);
    UseAIText->setOverrideColor(video::SColor(255,255,255,255));
    UseAIText->setTextAlignment(gui::EGUIA_CENTER,gui::EGUIA_CENTER);
    //the check boxes
    static const u32 checkboxWidth = Environment->getSkin()->getSize(gui::EGDS_CHECK_BOX_WIDTH);
    core::position2di checkBoxL(rectangle.getWidth()/2-checkboxWidth-border,useAIul.Y+height+border);
    core::position2di checkBoxR=checkBoxL;
    checkBoxR.X+=border*2+checkboxWidth;
    core::position2di checkboxSize(checkboxWidth,checkboxWidth);
    CheckBoxLeft =  Environment->addCheckBox(false,core::recti(checkBoxL,checkBoxL+checkboxSize),this);
    CheckBoxRight = Environment->addCheckBox(false,core::recti(checkBoxR,checkBoxR+checkboxSize),this);
    //reuse checkBox for score text
    checkBoxL.Y=border;
    checkBoxR.Y=border;
    ScoreLeft = Environment->addStaticText(L"5",core::recti(checkBoxL,checkBoxL+checkboxSize));
    ScoreLeft->setOverrideColor(video::SColor(255,170,170,255));
    ScoreLeft->setAlignment(gui::EGUIA_CENTER,gui::EGUIA_CENTER,gui::EGUIA_UPPERLEFT,gui::EGUIA_UPPERLEFT);
    ScoreRight = Environment->addStaticText(L"5",core::recti(checkBoxR,checkBoxR+checkboxSize));
    ScoreRight->setOverrideColor(video::SColor(255,255,170,170));
    ScoreRight->setAlignment(gui::EGUIA_CENTER,gui::EGUIA_CENTER,gui::EGUIA_UPPERLEFT,gui::EGUIA_UPPERLEFT);
    //the buttons
    core::position2di packPos((rectangle.getWidth()-width)/2,rectangle.getHeight()/2-height);
    ButtonStart = Environment->addButton(core::recti(packPos,packPos+size),this,-1,L"New Game");
    packPos.Y+=border+height;
    ButtonQuit = Environment->addButton(core::recti(packPos,packPos+size),this,-1,L"Quit");
    // message text
    f32 hMiddle = rectangle.getWidth()/2;
    f32 vMiddle = rectangle.getHeight()/2;
    core::position2di ML(hMiddle/2-width/2,vMiddle-height/2);
    core::position2di MR(hMiddle*1.5-width/2,vMiddle-height/2);
    MessageLeft = Environment->addStaticText(L"",core::recti(ML,ML+size));
    MessageRight = Environment->addStaticText(L"",core::recti(MR,MR+size));
    MessageLeft->setOverrideColor(video::SColor(255,255,255,255));
    MessageRight->setOverrideColor(video::SColor(255,255,255,255));
    MessageLeft->setTextAlignment(gui::EGUIA_CENTER,gui::EGUIA_CENTER);
    MessageRight->setTextAlignment(gui::EGUIA_CENTER,gui::EGUIA_CENTER);
    MessageLeft->setAlignment(gui::EGUIA_SCALE,gui::EGUIA_SCALE,gui::EGUIA_CENTER,gui::EGUIA_CENTER);
    MessageRight->setAlignment(gui::EGUIA_SCALE,gui::EGUIA_SCALE,gui::EGUIA_CENTER,gui::EGUIA_CENTER);
}
CGUIMenu::~CGUIMenu(){

}

bool CGUIMenu::OnEvent(const SEvent& event){
    if(event.EventType == EET_GUI_EVENT){
        //s32 id = event.GUIEvent.Caller->getID();
        switch(event.GUIEvent.EventType){
            case gui::EGET_BUTTON_CLICKED:
                break;
            default:
                break;
        }
    }
    return IGUIElement::OnEvent(event);
}
bool CGUIMenu::useAutoControlLeft(){
    return CheckBoxLeft->isChecked();
}
bool CGUIMenu::useAutoControlRight(){
    return CheckBoxRight->isChecked();
}
void CGUIMenu::toGameMode(){
    ButtonStart->setVisible(false);
    ButtonQuit->setVisible(false);
    UseAIText->setVisible(false);
    CheckBoxLeft->setVisible(false);
    CheckBoxRight->setVisible(false);
    ScoreLeft->setVisible(true);
    ScoreRight->setVisible(true);
    MessageLeft->setVisible(false);
    MessageRight->setVisible(false);
}
void CGUIMenu::toMenuMode(){
    ButtonStart->setVisible(true);
    ButtonQuit->setVisible(true);
    UseAIText->setVisible(true);
    CheckBoxLeft->setVisible(true);
    CheckBoxRight->setVisible(true);
    ScoreLeft->setVisible(false);
    ScoreRight->setVisible(false);
    MessageLeft->setVisible(true);
    MessageRight->setVisible(true);
}
void CGUIMenu::setScore(s32 scoreLeft,s32 scoreRight){
    core::stringw L(scoreLeft);
    core::stringw R(scoreRight);
    ScoreLeft->setText(L.c_str());
    ScoreRight->setText(R.c_str());
}
void CGUIMenu::setWinnerMessage(bool leftWin,bool rightWin){
    static const core::stringw win(L"WINNER :D");
    static const core::stringw lose(L"LOSER :(");
    if(leftWin){
        MessageLeft->setText(win.c_str());
    }else{
        MessageLeft->setText(lose.c_str());
    }
    if(rightWin){
        MessageRight->setText(win.c_str());
    }else{
        MessageRight->setText(lose.c_str());
    }
}
}}//
