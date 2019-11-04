#include <irrlicht.h>
#include <iostream>

#include "CGUIExtendedText.h"
#include "CColorTextTag.h"
#include "CFontTextTag.h"
#include "CImageTextTag.h"
#include "CTabTextTag.h"
#include "CHLineTextTag.h"
#include "CAlignTextTag.h"

#include "CGUIPanel.h"

using namespace irr;

int main()
{
    IrrlichtDevice* device = createDevice( video::EDT_OPENGL , core::dimension2du(640,480), 32);

    scene::ISceneManager* manager = device->getSceneManager();
    video::IVideoDriver* driver = device->getVideoDriver();
    gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

    guienv->getSkin()->setColor( gui::EGDC_BUTTON_TEXT, video::SColor(255,255,255,255) );

    device->getFileSystem()->changeWorkingDirectoryTo( "../data/" );

    gui::IGUIWindow* win = guienv->addWindow( core::rect<s32>(50,20,510,520) );

    gui::CGUIPanel* panel = new gui::CGUIPanel( guienv, guienv->getRootGUIElement(), -1, core::rect<s32>(2,20,157,197), false,
            gui::ESBM_AUTOMATIC,
            gui::ESBM_ALWAYS_INVISIBLE );

    panel->drop();

    gui::CGUIExtendedText* text = new gui::CGUIExtendedText( guienv, win, -1, core::rect<s32>(0,0,160,250) );

    gui::CColorTextTag* color = new gui::CColorTextTag();
    color->addDefaultColors();
    text->addTextTag( color );
    color->drop();

    gui::CImageTextTag* img = new gui::CImageTextTag();
    img->addImage( L"warn", driver->getTexture("warning.png"), core::dimension2d<s32>(10,10) );
    img->addImage( L"life", driver->getTexture("heart.tga"), core::dimension2d<s32>(10,10) );
    img->addImage( L"armor", driver->getTexture("armor.png"), core::dimension2d<s32>(10,10) );
    img->addImage( L"armorbig", driver->getTexture("armor.png"), core::dimension2d<s32>(60,45) );
    img->addImage( L"star", driver->getTexture("star.png"), core::dimension2d<s32>(10,10) );
    text->addTextTag( img );
    img->drop();

    gui::CFontTextTag* font = new gui::CFontTextTag();
    font->addFont( L"thick", guienv->getFont("font2.png") );
    text->addTextTag( font );
    font->drop();

    gui::CTabTextTag* tab = new gui::CTabTextTag( L"tab", 40 );
    text->addTextTag( tab );
    tab->drop();

    gui::CHLineTextTag* hline = new gui::CHLineTextTag();
    text->addTextTag( hline );
    hline->drop();

    gui::CAlignTextTag* align = new gui::CAlignTextTag();
    text->addTextTag( align );
    align->drop();

    text->setText( L"#img:armorbig:left#This is the extended text element. It can contain images: #img:warn# as well as #col:red#coloured text#col#."
                   L"You can also change the #font:thick#font to something else#font# if you #col:yellow#would like to do so.#col#"
                   L"Here is an example of the tabulator:\n\n"
                   L"#tab##img:life# 7/35#tab##img:warn#Warning!\n"
                   L"#tab##img:armor# 17#tab##img:star# 5#hline#There are also horizontal lines.They can be coloured as well, and have variable width:"
                   L"#col:orange##hline:2##col#\n"
                   L"#img:armorbig:right##align:center#Text can be centered#align:right##img:star#..or right-aligned #img:star##align#"
                   L"The two big images are \"anchored\" - they are anchored to either side and the text wraps around them."
                   L"#align:right#This text is right-aligned#align:center#And this is centered#align#" );
    if(0){
    text->setText( L"#img:armorbig:left#The leather armor offers good protection for the upper torso and shoulders. Electrical and fire attacks "
                   L"are especially well fend off by its hard leathers.\n\n"
                   L"#align:center##col:blue#Stats:#col##align##hline#\n"
                   L"#tab##img:armor# 17#tab##img:life# 35/35\n"
                   L"#tab##img:star# 26#tab##img:warn# #col:red#Can't use#col#\n" );
    };

    panel->fitChildrenToPanel();

    while ( device->run() )
    {
        driver->beginScene( true, true, video::SColor(0,0,0,0) );

        manager->drawAll();
        guienv->drawAll();

        driver->endScene();

        wchar_t caption [40];

        swprintf( &caption[0], 40, L"FPS: %i", driver->getFPS() );
        device->setWindowCaption( caption );
    }


    device->drop();

    return 0;
}
