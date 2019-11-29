
#include <irrlicht.h>

#include <SpriteManager.h>
#include <BmFont.h>
#include <ParticleSystem.h>

using namespace irr;
using namespace core;

int main(int argc, char** argv)
{
    int w = 400;
    int h = 400;

    IrrlichtDevice *device =
        createDevice( video::EDT_OPENGL, dimension2d<u32>(w, h), 16,
                      false, false, false, 0);

    if (!device)
        return 1;

    device->setWindowCaption(L"Irrlicht");

    video::IVideoDriver *driver = device->getVideoDriver();
    SpriteManager *sprites = new SpriteManager;
    sprites->setup(driver, driver->getTexture("assets/texture.png"));

    BmFont *fonts = new BmFont;
    fonts->setFontSource("assets/pixel1.fnt", 256, 128);
    fonts->setup(device, sprites);

    ParticleSystem *ps = new ParticleSystem(0, 200);
    ps->setSprite(sprites, 32 * 8, 8 * 8,8,8);
    ps->setPosition(w >> 1, h >> 1);
    ps->setEmissionRate(100);
    ps->setParticleLife(1.0f);
    ps->setDirection(360);
    ps->setLifetime(3);
    ps->setSpread(30);
    ps->setSpeed(100);
    ps->setGravity(150);
    ps->setSpin(100, 30, 2.0f);
    ps->setSize(1.5,2.0,1.0);
    video::SColor color1(255,255,255,255);
    video::SColor color2(0,255,0,0);
    ps->setColor((unsigned char*)&color1, (unsigned char*)&color2);

    // draw
    u32 prevTime = 0;
    while(device->run()) {

        // get elapsed time
        u32 currTime = device->getTimer()->getTime();
        if (prevTime == 0)
            prevTime = currTime;
        u32 elapsed = currTime - prevTime;
        prevTime = currTime;

        driver->beginScene(true, true, video::SColor(0,0,0,0));
        sprites->clear();

        // add sprite from tile (0,0) size(8,8)
        Sprite *spr = sprites->addSprite(0,0,8,8);
        spr->setPosition(20, 20);
        spr->setScale(4.0f, 4.0f);
fonts->setScale(10);
        // fonts
        fonts->drawText("the quick brown fox\njumped over\n the lazy dog", 0, 0, w, h, BmFont::ALIGN_CENTER | BmFont::ALIGN_MIDDLE);
        fonts->drawText("the quick brown fox\njumped over\n the lazy dog", 0, 0, w, h, BmFont::ALIGN_RIGHT | BmFont::ALIGN_TOP);
        fonts->drawText("the quick brown fox\njumped over\n the lazy dog", 0, 0, w, h, BmFont::ALIGN_LEFT| BmFont::ALIGN_BOTTOM);

        // particles
        float secs = (float)elapsed / 1000.0f;
        ps->update(secs);
        ps->draw(0,0,0,0,0,0,0);
        if (ps->isEmpty())
            ps->start();

        sprites->drawAll();

        driver->endScene();
    }

    device->drop();

    return 0;
}

