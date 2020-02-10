
#include "text.h"
#include <vector>

extern IrrlichtDevice *device;
extern video::IVideoDriver *driver;
extern gui::IGUIEnvironment *guienv;

void loadFonts()
{
    guienv->getFont("media/blinkfont.xml");
    guienv->getFont("media/shoutfont.xml");
    guienv->getFont("media/bigfont.xml");
}

const f32 blinkTimeGap = 0.5;

struct Text
{
    gui::IGUIStaticText *text;
    bool blink;
    double ttl;
    double age;
    double ageLastBlink;
};

std::vector <Text> texts;

void updateTexts(double dt)
{
    for (int i = 0; i < texts.size(); i ++)
    {
        texts[i].age += dt;

        if (texts[i].ttl > 0)
        {
            if (texts[i].age > texts[i].ttl)
            {
                texts[i].text->remove();
                texts.erase(texts.begin()+i);
                i --;
                continue;
            }
        }

        if (texts[i].blink)
        {
            if (texts[i].age > texts[i].ageLastBlink+blinkTimeGap)
            {
                texts[i].ageLastBlink = texts[i].age;
                texts[i].text->setVisible(!texts[i].text->isVisible());
            }
        }
    }
}

void removeText(Text *text)
{
    for (int i = 0; i < texts.size(); i ++)
    {
        if (&texts[i] == text)
        {
            texts[i].text->remove();
            texts.erase(texts.begin()+i);
            return;
        }
    }
    printf("Warning: removeText: text not found.\n");
}

void removeAllTimedTexts()
{
    for (int i = 0; i < texts.size(); i ++)
    {
        if (texts[i].ttl > 0)
        {
            texts[i].text->remove();
            texts.erase(texts.begin()+i);
            i --;
        }
    }
}

void removeAllTexts()
{
    for (int i = 0; i < texts.size(); i ++)
    {
        texts[i].text->remove();
        texts.erase(texts.begin()+i);
        i --;
    }
}

int screenHW()
{
    return (driver->getScreenSize()/2).Width;
}

int screenHH()
{
    return (driver->getScreenSize()/2).Height;
}

gui::IGUIStaticText *staticText(core::stringw str, int x, int y, int flag, char *fontFile)
{
    gui::IGUIFont *font = guienv->getFont(fontFile);

    if (!font)
    {
        printf("Error, could not load font: %s\n", fontFile);
        return NULL;
    }

    core::dimension2d<u32> dim = font->getDimension(str.c_str());
    core::rect<s32> rec;

    if (flag == 0)
    {
        rec = core::rect<s32>(x,y, x+dim.Width,y+dim.Height);
    }
    else if (flag == 4)
    {
        rec = core::rect<s32>(x-dim.Width/2,y-dim.Height/2, x+dim.Width/2,y+dim.Height/2);
    }
    else if (flag == 3)
    {
        rec = core::rect<s32>(x,y-dim.Height/2, x+dim.Width,y+dim.Height/2);
    }
    else
    {
        printf("Error: staticText: invalid flag (%i)\n", flag);
    }

    gui::IGUIStaticText *text = guienv->addStaticText(str.c_str(), rec, false,false);
    text->setOverrideFont(font);
    text->setOverrideColor(video::SColor(255,255,77,0));

    return text;
}

Text *normalMessage(core::stringw str, int y, char *fontFile)
{
    gui::IGUIFont *font = guienv->getFont(fontFile);
    if (!font)
    {
        printf("Error: font not found.\n");
        return NULL;
    }

    core::dimension2d<u32> dim = font->getDimension(str.c_str());
    core::rect<s32> rec = core::rect<s32>(screenHW()-dim.Width/2,y-dim.Height/2,
        screenHW()+dim.Width/2,y+dim.Height/2);

    gui::IGUIStaticText *itext = guienv->addStaticText(str.c_str(), rec, false,false);
    itext->setOverrideFont(font);
    itext->setOverrideColor(video::SColor(255,255,77,0));

    Text text =
    {
        itext,  // text node
        false,   // blink?
        0,    // time to live
        0.0f,   // current age
        0.0f    // age at last blink
    };
    texts.push_back(text);

    return &texts[texts.size()-1];
}

Text *blinkMessage(core::stringw str, int y, double ttl)
{
    Text *text = normalMessage(str, y, "media/blinkfont.xml");
    text->blink = true;
    text->ttl = ttl;
    return text;
}

void shoutMessage(core::stringw str, int y)
{
    Text *text = normalMessage(str, y, "media/shoutfont.xml");
    text->ttl = 2;
}


