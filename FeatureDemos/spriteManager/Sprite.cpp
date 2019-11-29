#include "Sprite.h"

using namespace irr;
using namespace core;
using namespace video;

Sprite::Sprite()
        : index(0),
        vertices(0),
        ax(0),
        ay(0),
        sx(1.0f),
        sy(1.0f),
        tx(0),
        ty(0),
        rot(0),
        shown(true),
        enabled(true),
        mirror(false)
{
    clr.set(255,255,255,255);
}

Sprite::~Sprite()
{}

void Sprite::init(int idx, S3DVertex *vtx,
                  int texWidth, int texHeight,
                  int sourceX, int sourceY, int sourceWidth, int sourceHeight)
{
    index = idx;
    vertices = vtx;
    clr.set(255,255,255,255);

    width = sourceWidth;
    height = sourceHeight;
    textureWidth = texWidth;
    textureHeight = texHeight;

    float ftW = 1.0f / texWidth;
    float ftH = 1.0f / texHeight;

    this->sourceX = (float)sourceX * ftW;
    this->sourceY = (float)sourceY * ftH;
    this->sourceX2 = (float)(sourceWidth + sourceX) * ftW;
    this->sourceY2 = (float)(sourceHeight + sourceY) * ftH;

    local[0].Pos = vector3df(0, 0, 0);
    local[1].Pos = vector3df(width, 0, 0);
    local[2].Pos = vector3df(width, height, 0);
    local[3].Pos = vector3df(0, height, 0);

    local[0].Color = clr;
    local[1].Color = clr;
    local[2].Color = clr;
    local[3].Color = clr;

    setEnabled(true);
    setVisible(true);
    setMirror(false);
    setAnchor(0, 0);
    setScale(1, 1);
    setAngle(0);

    for (int i = 0; i<MAX_SPRITE_DATA; i++)
        data[i] = 0;
}

void Sprite::transform(matrix4 &mat)
{
    if (vertices)
    {
        matrix4 matFinal;
        matFinal = mat;		// world pos
        if (matF.isIdentity()) {
            matF *= matT;	// pos
            matF *= matR;	// rotation
            matF *= matS;	// scale
            matF *= matA;	// anchor pos
        }
        matFinal *= matF;
        for (int i = 0; i<4; i++)
        {
            vertices[i] = local[i];
            matFinal.transformVect(vertices[i].Pos);
        }
    }
}

void Sprite::setAnchor(float x, float y)
{
    ax = x;
    ay = y;
    matA.makeIdentity();
    matA.setTranslation(vector3df(-ax,-ay,0));
    matF.makeIdentity();
}

void Sprite::setAngle(float a)
{
    rot = a;
    matR.makeIdentity();
    matR.setRotationDegrees(vector3df(0,0,rot));
    matF.makeIdentity();
}

void Sprite::setPosition(float x, float y)
{
    tx = x;
    ty = y;
    matT.makeIdentity();
    matT.setTranslation(vector3df(tx,ty,0));
    matF.makeIdentity();
}

void Sprite::setScale(float x, float y)
{
    sx = x;
    sy = y;
    matS.makeIdentity();
    matS.setScale(vector3df(sx, sy, 1.0f));
    matF.makeIdentity();
}

position2di Sprite::getAnchor()
{
    position2di p = position2di(ax, ay);
    return p;
}

position2di Sprite::getPosition()
{
    position2di p = position2di(tx, ty);
    return p;
}

dimension2df Sprite::getScale()
{
    dimension2df d = dimension2df(sx, sy);
    return d;
}

float Sprite::getAngle()
{
    return rot;
}

int Sprite::getAlpha()
{
    return clr.getAlpha();
}

void Sprite::setAlpha(int alpha)
{
    clr.setAlpha(alpha);
    setColor(clr);
}

SColor Sprite::getColor()
{
    return clr;
}

void Sprite::setColor(SColor c)
{
    clr = c;
    for (int i = 0; i<4; i++)
        local[i].Color = clr;
}

void Sprite::setColor(int r, int g, int b, int alpha)
{
    clr.set(alpha,r,g,b);
    for (int i = 0; i<4; i++)
        local[i].Color = clr;
}

void Sprite::setVisible(bool visible)
{
    shown = visible;
}

bool Sprite::isVisible()
{
    return shown;
}

void Sprite::setMirror(bool m)
{
    mirror = m;

    if (m) {
        local[0].TCoords = vector2df(sourceX2, sourceY);
        local[1].TCoords = vector2df(sourceX, sourceY);
        local[2].TCoords = vector2df(sourceX, sourceY2);
        local[3].TCoords = vector2df(sourceX2, sourceY2);
    } else {
        local[0].TCoords = vector2df(sourceX, sourceY);
        local[1].TCoords = vector2df(sourceX2, sourceY);
        local[2].TCoords = vector2df(sourceX2, sourceY2);
        local[3].TCoords = vector2df(sourceX, sourceY2);
    }
}

bool Sprite::isMirror()
{
    return mirror;
}

void Sprite::setEnabled(bool e)
{
    enabled = e;
}

bool Sprite::isEnabled()
{
    return enabled;
}

void Sprite::setIndex(int idx)
{
    index = idx;
}

int Sprite::getIndex()
{
    return index;
}

void Sprite::draw(float x, float y)
{
    printf("draw this sprite's manager instead\n");
}

void Sprite::setData(void *d, int index)
{
    if (index >= MAX_SPRITE_DATA || index < 0)
        return;
    data[index] = d;
}

void* Sprite::getData(int index)
{
    if (index >= MAX_SPRITE_DATA || index < 0)
        return 0;
    return data[index];
}

bool Sprite::hitTest(float x, float y)
{
    vector3df p(x,y,0);
    triangle3df t1(vertices[0].Pos, vertices[1].Pos, vertices[2].Pos);
    if (t1.isPointInsideFast(p))
        return true;
    triangle3df t2(vertices[0].Pos, vertices[2].Pos, vertices[3].Pos);
    if (t2.isPointInsideFast(p))
        return true;
    return false;
}

