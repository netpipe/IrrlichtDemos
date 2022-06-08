#ifndef TXTINTERFACE_H
#define TXTINTERFACE_H

#include <iostream>
using namespace std;

#include <irrlicht.h>
using namespace irr;

#include "TXTBlend.h"

// name of current working directory
core::string<c8> curentdir;

// default colors
video::SColor backcolor(255,180,180,180);
video::SColor buttcolor(255,220,220,220);
video::SColor deepcolor(255,150,150,150);
video::SColor bordcolor(255,0,0,0);

// pointers to some GUI elements
gui::IGUIStaticText* t1nametext, *t2nametext, *t1sizetext, *t2sizetext, *infotext;
gui::IGUIEditBox* namebox;
gui::IGUIComboBox* dirbox, *typebox;


video::ITexture* createBoxTexture(core::dimension2d<s32> size, s32 border,
    video::SColor color, video::SColor bordercolor)
{
    video::ITexture* texture = device->getVideoDriver()
        ->addTexture(size, "boxtexture", video::ECF_A1R5G5B5);

    s16* p = (s16*)texture->lock();

    video::SColor tmp;

    for(s32 j=0; j<size.Height; j++)
        for(s32 i=0; i<size.Width; i++)
            p[j*size.Width+i] = bordercolor.toA1R5G5B5();

    for(s32 j=border; j<size.Height-border; j++)
        for(s32 i=border; i<size.Width-border; i++)
            p[j*size.Width+i] = color.toA1R5G5B5();

    texture->unlock();
    return texture;
}

bool addIconToTexture(video::ITexture* texture, const c8* filename)
{
    video::IImage* image =
        device->getVideoDriver()->createImageFromFile(filename);
    video::ITexture* icontexture =
        device->getVideoDriver()->addTexture("icontexture", image);
    image->drop();

    core::dimension2du size = texture->getSize();
    if(size == icontexture->getSize())
    {
        s16* p = (s16*)texture->lock();
        s16* r = (s16*)icontexture->lock();

        for(s32 j=0; j<size.Height; j++)
            for(s32 i=0; i<size.Width; i++)
            {
                if(r[j*size.Width+i] != -1)
                    p[j*size.Width+i] = r[j*size.Width+i];
            }
        icontexture->unlock();
        device->getVideoDriver()->removeTexture(icontexture);

        texture->unlock();
        return true;
    }

    device->getVideoDriver()->removeTexture(icontexture);
    return false;
}

void createInterface()
{
    gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
    gui::IGUISkin * skin = guienv->getSkin();

    // set gui skin font
    skin->setFont(device->getGUIEnvironment()->getFont("Media/cityblueprint.bmp"));

    // set gui skin
    video::SColor color;
    for(s32 i=0; i<gui::EGDC_COUNT; ++i)
    {
	   color = skin->getColor((gui::EGUI_DEFAULT_COLOR)i);
	   color.setAlpha(255);
	   skin->setColor((gui::EGUI_DEFAULT_COLOR)i, color);
    }
    skin->setColor(gui::EGDC_3D_HIGH_LIGHT, deepcolor);
    skin->setColor(gui::EGDC_3D_FACE, buttcolor);

    // add some text
    guienv->addStaticText (L"Texture 1:", core::rect<s32>(20,20,110,50), false, false, 0, -1, false);
    guienv->addStaticText (L"Texture 2:", core::rect<s32>(20,60,110,90), false, false, 0, -1, false);
    guienv->addStaticText (L"Save to:", core::rect<s32>(20,110,110,140), false, false, 0, -1, false);
    guienv->addStaticText (L"Output name:", core::rect<s32>(20,150,110,180), false, false, 0, -1, false);

    // add main buttons
    video::ITexture* upmbtext = createBoxTexture(core::dimension2d<s32>(140,30), 2,
        buttcolor, bordcolor);
    video::ITexture* dwnmbtext = createBoxTexture(core::dimension2d<s32>(140,30), 4,
        deepcolor, bordcolor);

    gui::IGUIButton* button = guienv->addButton(core::rect<s32>(20,200, 20+140,200+30), 0, 100, L"Exit");
    button->setImage(upmbtext);
    button->setPressedImage(dwnmbtext);

    button = guienv->addButton(core::rect<s32>(180,200, 180+140,200+30), 0, 101, L"Blend");
    button->setImage(upmbtext);
    button->setPressedImage(dwnmbtext);

    button = guienv->addButton(core::rect<s32>(340,200, 340+140,200+30), 0, 102, L"About");
    button->setImage(upmbtext);
    button->setPressedImage(dwnmbtext);

    // add load buttons
    video::ITexture* upldtext = createBoxTexture(core::dimension2d<s32>(40,30), 2,
        buttcolor, bordcolor);
    addIconToTexture(upldtext, "Media/loadico.bmp");
    video::ITexture* dwnldtex = createBoxTexture(core::dimension2d<s32>(40,30), 4,
        deepcolor, bordcolor);
    addIconToTexture(dwnldtex, "Media/loadico.bmp");

    button = guienv->addButton(core::rect<s32>(120,20, 120+40,20+30), 0, 200);
    button->setImage(upldtext);
    button->setPressedImage(dwnldtex);

    button = guienv->addButton(core::rect<s32>(120,60, 120+40,60+30), 0, 201);
    button->setImage(upldtext);
    button->setPressedImage(dwnldtex);

    // add textureinfo text
    video::ITexture* tntext = createBoxTexture(core::dimension2d<s32>(210,30), 2,
        deepcolor, bordcolor);

    guienv->addImage(tntext, core::position2d<s32>(170,20));
    guienv->addImage(tntext, core::position2d<s32>(170,60));

    video::ITexture* tstext = createBoxTexture(core::dimension2d<s32>(90,30), 2,
        deepcolor, bordcolor);

    guienv->addImage(tstext, core::position2d<s32>(390,20));
    guienv->addImage(tstext, core::position2d<s32>(390,60));

    t1nametext = guienv->addStaticText (L"", core::rect<s32>(170,20,380,50), false, false, 0, -1, false);
    t2nametext = guienv->addStaticText (L"", core::rect<s32>(170,60,380,90), false, false, 0, -1, false);

    t1sizetext = guienv->addStaticText (L"", core::rect<s32>(390,20,480,50), false, false, 0, -1, false);
    t2sizetext = guienv->addStaticText (L"", core::rect<s32>(390,60,480,90), false, false, 0, -1, false);

    // add directory selection
    video::ITexture* drtext = createBoxTexture(core::dimension2d<s32>(360,30), 2,
        deepcolor, bordcolor);
    guienv->addImage(drtext, core::position2d<s32>(120,110));

    dirbox = guienv->addComboBox(core::rect<s32>(122,112, 120+360-2,110+30-2));

    // add name edit dialog
    video::ITexture* edtext = createBoxTexture(core::dimension2d<s32>(260,30), 2,
        deepcolor, bordcolor);
    guienv->addImage(edtext, core::position2d<s32>(120,150));

    namebox = guienv->addEditBox (L"", core::rect<s32>(125,150, 120+260,150+30), false);

    // add filetype selection
    guienv->addImage(tstext, core::position2d<s32>(390,150));

    typebox = guienv->addComboBox(core::rect<s32>(392,152, 390+90-2,150+30-2));
    typebox->addItem(L"bmp");
    typebox->addItem(L"tga");
    typebox->addItem(L"png");

    // ad info dialog
    video::ITexture* intext = createBoxTexture(core::dimension2d<s32>(460,30), 2,
        deepcolor, bordcolor);
    guienv->addImage(intext, core::position2d<s32>(20,250));

    infotext = device->getGUIEnvironment()->addStaticText(L"", core::rect<s32>(20,250, 20+460,250+30), false, false, 0, -1, false);

    // find out current directory
    io::IFileSystem* iosys = device->getFileSystem();
    curentdir = core::string<c8>(iosys->getWorkingDirectory());

    // feed list of directories inside working directory to directory box
    io::IFileList* filelist = iosys->createFileList();

    s32 outi = -1;

    for(s32 i=0; i<filelist->getFileCount(); i++)
    {
        if(filelist->isDirectory(i))
        {
            core::string<wchar_t> dirname(filelist->getFileName(i));
            if(dirname != L"." && dirname != L"..")
            {
                core::string<wchar_t>tmp(".../");
                tmp+=dirname;
                s32 n = dirbox->addItem(tmp.c_str());
                if(dirname == L"Out") outi = n;
            }
        }
    }
    filelist->drop();
    // default directory
    dirbox->setSelected(outi);
}

class TXTEventReceiver : public IEventReceiver
{

public:

	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();

			switch(event.GUIEvent.EventType)
			{
                case gui::EGET_BUTTON_CLICKED:
                    if(id == 100){device->closeDevice(); return true;}

                    if(id == 101)
                    {
                        if(!tb->texture1Loaded())
                        {
                            infotext->remove();
                            infotext = device->getGUIEnvironment()->addStaticText (L"Error ...please load firsth texture!", core::rect<s32>(25,250, 20+460-5,250+30), false, false, 0, -1, false);
                            return true;
                        }

                        if(!tb->texture2Loaded())
                        {
                            infotext->remove();
                            infotext = device->getGUIEnvironment()->addStaticText (L"Error ...please load second texture!", core::rect<s32>(25,250, 20+460-5,250+30), false, false, 0, -1, false);
                            return true;
                        }

                        if(!tb->texturesSizeIsEqual())
                        {
                            infotext->remove();
                            infotext = device->getGUIEnvironment()->addStaticText (L"Error ...size of loaded textures do not match!", core::rect<s32>(25,250, 20+460-5,250+30), false, false, 0, -1, false);
                            return true;
                        }

                        // get filename to write
                        core::string<wchar_t> name(namebox->getText());
                        if(name == L"")
                        {
                            infotext->remove();
                            infotext = device->getGUIEnvironment()->addStaticText (L"Error ...please type output name!", core::rect<s32>(25,250, 20+460-5,250+30), false, false, 0, -1, false);
                            return true;
                        }

                        core::string<wchar_t> type(typebox->getItem(typebox->getSelected()));
                        core::string<wchar_t> dir(dirbox->getItem(dirbox->getSelected()));
                        s32 i = dir.findLast('/');
                        dir = dir.subString(i+1, dir.size() - i);

                        device->getFileSystem()->changeWorkingDirectoryTo(curentdir.c_str());

                        c8* charstr = new c8[dir.size()+1];
                        wcstombs(charstr, dir.c_str(), dir.size()+1);
                        device->getFileSystem()->changeWorkingDirectoryTo(charstr);
                        delete charstr;

                        charstr = new c8[name.size()+1];
                        wcstombs(charstr, name.c_str(), name.size()+1);
                        tb->setFileName(charstr);
                        delete charstr;

                        charstr = new c8[type.size()+1];
                        wcstombs(charstr, type.c_str(), type.size()+1);
                        tb->writeFile(charstr);
                        delete charstr;

                        infotext->remove();
                        infotext = device->getGUIEnvironment()->addStaticText (L"Blending successful.", core::rect<s32>(25,250, 20+460-5,250+30), false, false, 0, -1, false);

                        return true;
                    }

                    if(id == 102)
                    {
                        gui::IGUIWindow* window;
                        window = device->getGUIEnvironment()->addWindow(core::rect<s32>(20,20,480,280), true, L"About TXTBlend");

                        device->getGUIEnvironment()->addImage(device->getVideoDriver()->getTexture("Media/txtblend.bmp"),
                            core::position2d<s32>(278,19), false, window );

                        device->getGUIEnvironment()->addStaticText (L"version: 1.0", core::rect<s32>(20,40, 250,60), false, false, window, -1, false);
                        device->getGUIEnvironment()->addStaticText (L"author: Michal Svantner", core::rect<s32>(20,60, 250,80), false, false, window, -1, false);
                        device->getGUIEnvironment()->addStaticText (L"date: 15.03.2007", core::rect<s32>(20,80, 250,100), false, false, window, -1, false);
                        device->getGUIEnvironment()->addStaticText (L"Created thanks to:", core::rect<s32>(20,110, 260,130), false, false, window, -1, false);
                        device->getGUIEnvironment()->addStaticText (L" Irrlich   http://irrlicht.sourceforge.net", core::rect<s32>(20,130, 260,150), false, false, window, -1, false);
                        device->getGUIEnvironment()->addStaticText (L" Dev-C++   http://www.bloodshed.net", core::rect<s32>(20,150, 260,170), false, false, window, -1, false);
                        device->getGUIEnvironment()->addStaticText (L"Published under GPU General Public License.", core::rect<s32>(20,210, 440,230), false, false, window, -1, false);
                        return true;
                    }

                    if(id == 200)
                    {
                        device->getGUIEnvironment()->addFileOpenDialog(L"Please choose a file.", true, 0, 300);
                        return true;
                    }

                    if(id == 201)
                    {
                        device->getGUIEnvironment()->addFileOpenDialog(L"Please choose a file.", true, 0, 301);
                        return true;
                    }
                    break;

                case gui::EGET_FILE_SELECTED:
                    if(id == 300)
                    {
                        gui::IGUIFileOpenDialog* dialog = (gui::IGUIFileOpenDialog*)event.GUIEvent.Caller;

                        core::stringc name(dialog->getFilename());
                        s32 i = name.findLast('/');
                        name = name.subString(i+1, name.size() - i);
                        core::string<wchar_t> text;

                        if(tb->loadImage1( name.c_str() ) )
                        {
                            text += "\"";
                            text += name.c_str();
                            text += "\" loaded as texture 1.";
                            t1nametext->remove();
                            t1nametext = device->getGUIEnvironment()->addStaticText
                                (core::string<wchar_t>(name.c_str()).c_str(), core::rect<s32>(175,20,380,50), false, false, 0, -1, false);

                            core::string<wchar_t> size(tb->getTexture1Size().Width);
                            size += L" x ";
                            size += core::string<wchar_t>(tb->getTexture1Size().Height);
                            t1sizetext->remove();
                            t1sizetext = device->getGUIEnvironment()->addStaticText
                                (size.c_str(), core::rect<s32>(395,20,480,50), false, false, 0, -1, false);
                        }
                        else
                        {
                            text += "Error ...\"";
                            text += name.c_str();
                            text += "\" can not be loaded!";
                        }

                        infotext->remove();
                        infotext = device->getGUIEnvironment()->addStaticText (text.c_str(), core::rect<s32>(25,250, 20+460-5,250+30), false, false, 0, -1, false);
                    }

                    if(id == 301)
                    {
                        gui::IGUIFileOpenDialog* dialog = (gui::IGUIFileOpenDialog*)event.GUIEvent.Caller;

                        core::stringc name(dialog->getFilename());
                        s32 i = name.findLast('/');
                        name = name.subString(i+1, name.size() - i);
                        core::string<wchar_t> text;

                        if(tb->loadImage2( name.c_str() ) )
                        {
                            text += "\"";
                            text += name.c_str();
                            text += "\" loaded as texture 2.";
                            t2nametext->remove();
                            t2nametext = device->getGUIEnvironment()->addStaticText
                                (core::string<wchar_t>(name.c_str()).c_str(), core::rect<s32>(175,60,380,90), false, false, 0, -1, false);

                            core::string<wchar_t> size(tb->getTexture2Size().Width);
                            size += L" x ";
                            size += core::string<wchar_t>(tb->getTexture2Size().Height);
                            t2sizetext->remove();
                            t2sizetext = device->getGUIEnvironment()->addStaticText
                                (size.c_str(), core::rect<s32>(395,60,480,90), false, false, 0, -1, false);
                        }
                        else
                        {
                            text += "Error ...\"";
                            text += name.c_str();
                            text += "\" can not be loaded!";
                        }

                        infotext->remove();
                        infotext = device->getGUIEnvironment()->addStaticText (text.c_str(), core::rect<s32>(25,250, 20+460-5,250+30), false, false, 0, -1, false);
                    }
			}
		}

		return false;
	}
};
#endif
