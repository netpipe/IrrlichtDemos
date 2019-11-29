#ifndef _BMFONT_H_
#define _BMFONT_H_

#include <irrlicht.h>

#define MAX_BMFONT_CHARS 256

class Sprite;
class SpriteManager;
class BmFontChar
{
public:
    BmFontChar(char id, int x, int y, int width, int height, int xoffset, int yoffset, int xadvance, int page, int chnl) :
            id(id),
            x(x),
            y(y),
            width(width),
            height(height),
            xoffset(xoffset),
            yoffset(yoffset),
            xadvance(xadvance),
            page(page),
            chnl(chnl)
    {}

    char id;
    int x;
    int y;
    int width;
    int height;
    int xoffset;
    int yoffset;
    int xadvance;
    int page;
    int chnl;
};

class BmFont : public irr::IReferenceCounted
{
public:

    enum {
        ALIGN_LEFT      = 1 << 1,
        ALIGN_RIGHT     = 1 << 2,
        ALIGN_CENTER    = 1 << 3,
        ALIGN_BOTTOM    = 1 << 4,
        ALIGN_MIDDLE    = 1 << 5,
        ALIGN_TOP       = 1 << 6
    };

    BmFont();
    ~BmFont();

    void setFontSource(irr::core::stringc filename, int ox, int oy);
    bool setup(irr::IrrlichtDevice *device, SpriteManager *sprites);

    void drawText(const irr::core::stringc &text,
                int x, int y,
                int w = 0, int h = 0,
                int align = ALIGN_LEFT | ALIGN_TOP,
                irr::video::SColor color = irr::video::SColor(255,255,255,255),
                float angle = 0);
    Sprite* getCharacterSprite(const char c);

    float getWidth(const irr::core::stringc &text);
    float getHeight(const irr::core::stringc &text);
    void getTextMetrics(const irr::core::stringc &text, float *width = 0, float *height = 0, int w = 0, int h = 0);
    float getLineHeight();
    float getBaseHeight();

    void setScale(float s);

protected:

    irr::core::stringc filename;
    float lineHeight;
    float base;
    float offsetX;
    float offsetY;
    float scale;
    
    float textWidth;
    float textHeight;
    bool textMetricsOnly;

    irr::core::stringc textureName;
    BmFontChar* chars[MAX_BMFONT_CHARS];

    SpriteManager *sprites;
};


#endif // _BMFONT_H_
