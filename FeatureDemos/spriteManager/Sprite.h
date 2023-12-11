#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <irrlicht.h>

#define MAX_SPRITE_DATA 8

class Sprite  {
public:

    Sprite();
    ~Sprite();

    void setAnchor(float x, float y);

    void setAngle(float angle);

    void setPosition(float x, float y);

    void setScale(float sx, float sy);

    irr::core::position2di getAnchor();

    irr::core::position2di getPosition();

    irr::core::dimension2df getScale();

    float getAngle();

    int getAlpha();

    void setAlpha(int alpha) ;

    irr::video::SColor getColor();

    void setColor(irr::video::SColor SColor);

    void setColor(int r, int g, int b, int alpha);

    void setVisible(bool visible);

    bool isVisible();

    void setMirror(bool m);

    bool isMirror();

    void setIndex(int idx);

    void setEnabled(bool enabled);

    bool isEnabled();

    int getIndex();

    void draw(float x, float y);

    void setData(void *data, int index = 0);

    void* getData(int index = 0);

    bool hitTest(float x, float y);

protected:

    void init(int index, irr::video::S3DVertex *vertices, int tw, int th, int sx, int sy, int sw, int sh);

    void transform(irr::core::matrix4 &mat);

    friend class SpriteManager;

    irr::video::S3DVertex local[4];
    irr::video::S3DVertex *vertices;
    int index;

    int textureWidth; // texture width
    int textureHeight; // texture height
    int width;  // sprite width
    int height; // sprite height

    float ax; // anchor
    float ay;
    float sx; // scale
    float sy;
    float tx; // translate
    float ty;
    float rot; // rotate
    bool shown;
    bool enabled;
    bool mirror; // mirror horizontally

    irr::core::matrix4 matA;
    irr::core::matrix4 matR;
    irr::core::matrix4 matT;
    irr::core::matrix4 matS;
    irr::core::matrix4 matF;

    float sourceX;
    float sourceY;
    float sourceX2;
    float sourceY2;

    irr::video::SColor clr;

    void *data[MAX_SPRITE_DATA];
};


#endif // _SPRITE_H_
