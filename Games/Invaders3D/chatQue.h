/*
chatQue is copyright g0dsoft 2006
Version: 0.2.3
Code by: Mark Laprairie
Contact: webmaster@g0dsoft.com
Url: www.g0dsoft.com

chatQue.h
ChatQue is a object for simulating half-life style chat display
capable of colors, special effects and any custom font.

Example usage:
ChatQue* chatQue;
IGUIFont* guiFont;
guiFont = gdevice->getGUIEnvironment()->getFont("Media/Fonts/haettenschweiler.bmp");
chatQue = new ChatQue();
//gdevice is a pointer to the current irrlicht device.
chatQue->initialise(gdevice,core::rect<s32>(10,100,gWIDTH-100,gHEIGHT-100),guiFont,8000,500);
chatQue->setDebug(true);
chatQue->setDefaultStyle(CQS_PLAIN); //This line isnt needed
    chatQue->addMessage( L"This is a test of the chatQue object," );
    chatQue->addMessage( L"for use with irrlicht." );
    chatQue->addMessage( L"Lots of great features including- Word wrrrrrapping! Do, do do, do the word wrap...auto matic happy! Aint life easy?  word wrap" );
    chatQue->addMessage( L"Rainbow Text",CQS_RAINBOW);
    chatQue->addMessage( L"Pulse Text",SColor(255,255,255,0),SColor(0,0,0,0), CQS_PULSE );
    chatQue->addMessage( L"Shadow Text ... Also text can fade off slowly :D",CQS_SHADOW);
    chatQue->addMessage( L"Text can be any color!", SColor(255,100,255,25));
//Just befor you call driver->endScene();
chatQue->draw();

ToDo:
     -Word Wrapping that preserves whole words.
*/

#ifndef __CHATQUE_H__
#define __CHATQUE_H__

#define CHATQUE_STRINGLENGTH 200

#include <irrlicht.h>
#include <list>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

enum chatQueStyle{
    CQS_USEDEFAULT,
    CQS_PLAIN,
    CQS_SHADOW,
    CQS_RAINBOW,
    CQS_MULTIRAINBOW,
    CQS_PULSE,
    CQS_BACKGROUND,
    CQS_CROPBACKGROUND,
    /*!! Warning:
    The following styles are for internal
    use only,  using them may cause your
    program to crash.
    !!*/
    CQS_INTERNALUSEONLY_PULSE,
    CQS_INTERNALUSEONLY_FADE,
    CQS_INTERNALUSEONLY_FADE2
};

class ChatQue {
public:
    //Constructor
    ChatQue();
    ChatQue(IrrlichtDevice * device,const core::rect<s32> drawArea,IGUIFont* font = NULL,u32 textLife = 5000, u32 fadeTime = 500.00, bool setWordWrap = true);

    ~ChatQue();
    void drop();
    //Main Functions
    /* Initialise
    device       - Irrlicht device pointer
    drawArea     - Drawing surface rectangle
    Font         - Loaded font to use
    textLife     - how long the text should remain on the screen
                   to disable this feature, give a negative number.
    fadeTime     - How long should the text fade off for (0 for no fade)
    wordWrap     - If the text is longer then the width of the draw area,
                   should the text continue on anouther line?
    */
    void initialise(IrrlichtDevice * device,const core::rect<s32> drawArea,IGUIFont* font = NULL,u32 textLife = 5000, u32 fadeTime = 500, bool setWordWrap = true);
    /* addMessage
    text         - Irrlicht stringw,  wchar_t* or const wchar_t*
                   refrencing the text you want displayed
    color1       - The main color for your text
    color2       - The special color, used for styles
                   ex. if you apply CQS_SHADOW as the style this
                   will be the color for the background shadow.
    chatQueStyle - The style the text is to be renderd with
                   defaults to a style set with setDefaultStyle
                   or if not set,  CQS_PLAIN
    */
    void addMessage(core::stringw text,SColor color1 = SColor(255,255,255,255),SColor color2 = SColor(255,0,0,0),chatQueStyle useStyle = CQS_USEDEFAULT);

    void addMessage(wchar_t* text,SColor color1 = SColor(255,255,255,255),SColor color2 = SColor(255,0,0,0),chatQueStyle useStyle = CQS_USEDEFAULT);
    void addMessage(const wchar_t* text,SColor color1 = SColor(255,255,255,255),SColor color2 = SColor(255,0,0,0),chatQueStyle useStyle =  CQS_USEDEFAULT);
    void addMessage(wchar_t* text, chatQueStyle useStyle);
    void addMessage(const wchar_t* text, chatQueStyle useStyle);
    void addMessage(core::stringw text, chatQueStyle useStyle);

    void draw();

    void setFadeTime(u32 fadeTime = 1000);
    void setFont(irr::gui::IGUIFont* font);
    void setMaxLines(u16 maxLines);
    void setVisible(bool setVisible = true);
    void setLife(u32 setLife = 1000);
    void setWordWrap(bool setWordWrap = true);
    void setDrawArea(const core::rect<s32> areaArea);
    void setDebug(bool setDebug = true);
    void setDefaultStyle(chatQueStyle useStyle);
    bool getVisible();
    bool getDebug();
    u32 getFadeTime();
    irr::gui::IGUIFont* getFont();
    chatQueStyle getDefaultStyle();

private:
    struct chatQueMessage{
        wchar_t message[CHATQUE_STRINGLENGTH];
        chatQueStyle Style;
        SColor color1;
        SColor color2;
        SColor color1Fade;
        SColor color2Fade;
        u32 created;
        u32 fade;
        bool chatQue_memoryManaged;
    };
    //Helper functions
    void chatQue_calculateRect(const core::rect<s32> drawArea);
    void chatQue_calculateFontDimensions(irr::gui::IGUIFont* font);
    void chatQue_calculateMaxLines();

    //Style and Font
    chatQueStyle chatQue_defaultStyle;
    IGUIFont* chatQue_font;
    //Rect replacment
    u32 m_xWidth;
    u32 m_yHeight;
    u32 m_y;
    u32 m_x;
    //Font height
    u16 m_fontHeight;

    IrrlichtDevice* chatQue_device;
    bool chatQue_visible;
    bool chatQue_debug;
    bool chatQue_wordWrap;
    bool chatQue_init;
    u32 chatQue_fadeTime;
    u32 chatQue_life;
    u16 chatQue_maxLines;
    std::list<chatQueMessage> chatQue_list;
};

#endif //chatque.h
