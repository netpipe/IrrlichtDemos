#ifndef CPROGRESS_BAR_H
#define CPROGRESS_BAR_H

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;



class CProgressBar {

    IrrlichtDevice* device;
    IVideoDriver* driver;
    IGUIEnvironment* guienv;
    // GUI Skin
    IGUISkin* skin;
    IGUIStaticText* text;
    ITexture* backgroundImageTexture = NULL;
    // IGUIFont* font;

    // Percentage of loading bar.
    stringw DisplayText;
    s32 Percent;
    s32 PercentNum;
    s32 MaxNum; // The far right X pos of the border

    // Visible?
    bool isVisiblee;
    bool isDone;

    // Draw texts?
    bool Text;



    // Draw border?
    bool Border;
    rect<s32> BorderRect;
    rect<s32> AbsoluteBorderRect;

    // Loading bar rectangle
    rect<s32> LoadRect;


public:


    CProgressBar(IrrlichtDevice* in_d, video::IVideoDriver* in_driver, IGUIEnvironment* in_guienv, ITexture* in_backgroundImageTexture, const rect<s32>& rectangle,
        bool vis,s32 TotalNum): device(in_d), Percent(0), Text(true), DisplayText(L""), Border(true), isVisiblee(vis),
        AbsoluteBorderRect(rectangle), BorderRect(rectangle), MaxNum(TotalNum),isDone(false){

        // driver = device->getVideoDriver();
        driver = in_driver;
        // guienv = device->getGUIEnvironment();
        guienv = in_guienv;
        skin = guienv->getSkin();

        if(backgroundImageTexture != NULL){ driver->removeTexture(backgroundImageTexture); backgroundImageTexture = NULL; };
        backgroundImageTexture = in_backgroundImageTexture;
    };


    ~CProgressBar(){};


    void draw();

    void setVisible(bool t_f){ isVisiblee = t_f; };

    void setText(stringw t){ DisplayText = t; };
    stringw getText(){ return DisplayText; };

    void setPercent(s32 p){ Percent = p; };
    s32 getPercent(){ return Percent; };

    bool isComplete(){ return isDone; };

};

#endif

