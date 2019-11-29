#include "BmFont.h"
#include "Util.h"
#include "SpriteManager.h"

using namespace irr;

#define MAX_WORD_LENGTH 256
#define MAX_TEXT_LENGTH 1024

int getBmFontValue(core::list<core::stringc>& list, const core::stringc& name, int defaultValue = 0)
{
    core::list<core::stringc>::Iterator it = list.begin();
    while (it != list.end())
    {
        core::list<core::stringc> valuePair;
        (*it).split(valuePair, "=");
        if (valuePair.size() == 2) {
            if ((*valuePair.begin()) == name)
                return atoi((const char*)((*(valuePair.begin()+1)).c_str()));
        }
        it++;
    }
    return defaultValue;
}

core::stringc getBmFontValueString(core::list<core::stringc>& list, const core::stringc& name, const core::stringc& defaultValue)
{
    core::list<core::stringc>::Iterator it = list.begin();
    while (it != list.end())
    {
        core::list<core::stringc> valuePair;
        (*it).split(valuePair, "=");
        if (valuePair.size() == 2) {
            if ((*valuePair.begin()) == name) {
                core::stringc res = *(valuePair.begin()+1);
                res.replace("\"", "");
                return res;
            }
        }
        it++;
    }
    return defaultValue;
}

BmFont::BmFont()
        : sprites(0), scale(1.0f), textMetricsOnly(false)
{
    for (int i = 0; i<MAX_BMFONT_CHARS; i++)
        chars[i] = 0;
}

BmFont::~BmFont()
{
    for (int i = 0; i<MAX_BMFONT_CHARS; i++)
    {
        if (chars[i])
            delete chars[i];
    }
}

void BmFont::setFontSource(core::stringc f, int ox, int oy)
{
    filename = f;
    offsetX = ox;
    offsetY = oy;
}

bool BmFont::setup(IrrlichtDevice *device, SpriteManager *s)
{
    sprites = s;

    // load font if empty
    io::IFileSystem *fs = device->getFileSystem();
    io::IReadFile *file = fs->createAndOpenFile(filename);
    if (!fs) {
        printf("unable to load %s\n", filename.c_str());
        return false;
    }

    if (!textureName.size()) {

        char buffer[1024];

        while (file->getPos() < file->getSize())
        {
            if (readLine(file, buffer)) {
                core::stringc text(buffer);
                core::list<core::stringc> list;
                text.split(list, " ");

                if (*(list.begin()) == "info")
                {

                } else  if (*(list.begin()) == "common") {
                    lineHeight = getBmFontValue(list, "lineHeight", 0);
                    base = getBmFontValue(list, "base", 0);
                } else  if (*(list.begin()) == "char") {
                    char id = getBmFontValue(list, "id", 0);
                    int x = getBmFontValue(list, "x", 0) + offsetX;
                    int y = getBmFontValue(list, "y", 0) + offsetY;
                    int width = getBmFontValue(list, "width", 0);
                    int height = getBmFontValue(list, "height", 0);
                    int xoffset = getBmFontValue(list, "xoffset", 0);
                    int yoffset = getBmFontValue(list, "yoffset", 0);
                    int xadvance = getBmFontValue(list, "xadvance", 0);
                    int page = getBmFontValue(list, "page", 0);
                    int chnl = getBmFontValue(list, "chnl", 0);
                    if (id < MAX_BMFONT_CHARS)
                        chars[id] = new BmFontChar(id, x, y, width, height, xoffset, yoffset, xadvance, page, chnl);
                } else  if (*(list.begin()) == "page") {
                    core::stringc pagefile = getBmFontValueString(list, "file", "");
                    textureName = "assets/";
                    textureName += pagefile;
                }
            }
        }

        file->drop();

    }

    return true;
}

void BmFont::drawText(const core::stringc &text,
                int x, int y, int w, int h,
                int align,
                video::SColor color,
                float angle)
{
    if (!sprites)
        return;

    float lineHeight = getLineHeight();
    float baseHeight = getBaseHeight();
    float offsetX = 0;
    float offsetY = 0;

    Sprite *lineStart = 0;
    Sprite *word[MAX_WORD_LENGTH];
    int wordIdx = 0;

    Sprite *characters[MAX_TEXT_LENGTH];
    int charIdx = 0;

    textWidth = 0;
    textHeight = 0;

    for (int i = 0; i<text.size(); i++)
    {
        bool isSpace = (text[i] == ' ' || text[i] == '\t' || text[i] == '\n');

        if (isSpace)
            wordIdx = 0;

        if (text[i] == '\n') {
            // save line width
            if (lineStart) {
                lineStart->setData((void*)(int)offsetX, 1);
                lineStart = 0;

                if (textWidth < offsetX)
                    textWidth = offsetX;
            }
            offsetX = 0;
            offsetY += lineHeight;
            continue;
        }

        BmFontChar *c = chars[text[i]];
        if (!c)
            continue;

        float xoffset = c->xoffset * scale;
        float yoffset = c->yoffset * scale;

        if (!isSpace) {
            Sprite *sprite = 0;
            if (!textMetricsOnly) {
                sprite = sprites->addSprite(c->x, c->y, c->width, c->height);
                sprite->setPosition(x + offsetX + xoffset, y + offsetY + yoffset);
                sprite->setColor(color);
                sprite->setScale(scale, scale);
                sprite->setData((void*)c);
            }
            characters[charIdx++] = sprite;
            if (!lineStart)
                lineStart = sprite;
            word[wordIdx++] = sprite;

            // text is too long, adjust the maximum
            if (charIdx == MAX_TEXT_LENGTH)
                break;

        } else {
            // save line width
            if (lineStart) {
                lineStart->setData((void*)(int)offsetX, 1);
                if (textWidth < offsetX)
                    textWidth = offsetX;
            }
        }

        offsetX += (c->xadvance * scale);

        // word is too long
        if (wordIdx == MAX_WORD_LENGTH)
            wordIdx = 0;

        if (offsetX > w && w != 0) {

            lineStart = 0;
            offsetY += lineHeight;
            offsetX = 0;

            // bring down current word
            for(int i = 0; i<wordIdx; i++) {
                Sprite *sprite = 0;
                if (!textMetricsOnly) {
                sprite = word[i];
                    if (!lineStart)
                        lineStart = sprite;
                    BmFontChar *c = (BmFontChar*)sprite->getData();
                    float xoffset = c->xoffset * scale;
                    float yoffset = c->yoffset * scale;
                    sprite->setPosition(x + offsetX + xoffset, y + offsetY + yoffset);
                }
                offsetX += (c->xadvance * scale);
            }
        }
    }

    if (lineStart)
        lineStart->setData((void*)(int)offsetX, 1);

    if (textWidth < offsetX)
        textWidth = offsetX;
    textHeight = offsetY + lineHeight;

    float moveX = 0;
    float moveY = 0;

    if (align & ALIGN_BOTTOM) {
        moveY = h - textHeight;
    }
    if (align & ALIGN_MIDDLE) {
        moveY = (h - textHeight) * 0.5f;
    }

    // do align
    if (!textMetricsOnly) {
        for(int i = 0; i<charIdx; i++) {
            Sprite *s = characters[i];
            int lineWidth = 0; //(int)s->getData(1);  //! was the only reson for adding -fpermissive to project
            s->getData(1); //put here instead
            if (lineWidth) {
                if (align & ALIGN_RIGHT) {
                    moveX = w - lineWidth;
                }
                if (align & ALIGN_CENTER) {
                    moveX = (w - lineWidth) * 0.5f;
                }
            }
            irr::core::position2di pos = s->getPosition();
            s->setPosition(pos.X + moveX, pos.Y + moveY);
        }
    }

    // angle - doesn't work yet -- do some math
    #if 0
    if (angle) {
        for(int i = 0; i<charIdx; i++) {
            Sprite *s = characters[i];
            irr::core::position2di pos = s->getPosition();
            s->setPosition(x, y);
            s->setAnchor(-pos.X, -pos.Y);
            s->setAngle(angle);
        }
    }
    #endif

}

Sprite* BmFont::getCharacterSprite(const char ci)
{
    BmFontChar *c = chars[ci];
    if (!c)
        return 0;
    Sprite *sprite = sprites->addSprite(c->x, c->y, c->width, c->height);
    return sprite;
}

float BmFont::getLineHeight()
{
    return lineHeight * scale;
}

float BmFont::getBaseHeight()
{
    return (lineHeight - base) * scale;
}

void BmFont::getTextMetrics(const irr::core::stringc &text, float *width, float *height, int w, int h)
{
    textMetricsOnly = true;
    drawText(text, 0, 0, w, h);
    if (width)
        *width = textWidth;
    if (height)
        *height = textHeight;
    textMetricsOnly = false;
}

float BmFont::getWidth(const irr::core::stringc &text)
{
    float w = 0;
    getTextMetrics(text, &w, 0);
    return w;
}

float BmFont::getHeight(const irr::core::stringc &text)
{
    float h = 0;
    getTextMetrics(text, 0, &h);
    return h;
}

void BmFont::setScale(float s)
{
    scale = s;
}

