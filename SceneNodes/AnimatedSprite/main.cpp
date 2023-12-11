// Animated Sprite Example
// shows howe to create sprite, add and play animations
// created by Michal Švantner
// 04.09.2007

#include <irrlicht.h>
using namespace irr;

#include "AnimSprite.h"

int main()
{
    IrrlichtDevice * Device = createDevice(video::EDT_OPENGL,
        core::dimension2d<u32>(640, 480), 32, false, false, false, 0);

    Device->setWindowCaption(L"Animated Sprite Example, by arras, 04.09.2007");

    video::IVideoDriver* driver = Device->getVideoDriver();
    scene::ISceneManager* smgr = Device->getSceneManager();

	// firsth load texture with animation frames
	video::IImage *image;
    image = driver->createImageFromFile("texture.png");
    video::ITexture *texture = driver->addTexture("texture", image);
    image->drop();

    // create sprite
    // texture will be divided in to 4x2=8 frames
    // frames are based on 0 index so frames 0,1,2,3,4,5,6,7 will be created
    AnimSprite sprite1(Device, texture , 4, 2);

    // set firsth animation
    // it will play frames 0 - 3, one frame per 1000 miliseconds looping itself
    sprite1.addAnimation(0,3,1000, true, false);

    // set second animation
    // it will reverse play frames 2 - 7, one frame per 500 miliseconds
    // as you see animations can share frames
    sprite1.addAnimation(2,7,500, false, true);

    // set firsth animation to play
    // change paramter to 1 to play second animation
    sprite1.setAnimation(0);

    // set position of sprite
    sprite1.setPosition(236,208);

    // create another sprite as a coppy of firsth sprite
    AnimSprite sprite2(sprite1);

    // set it to play second animation
    sprite2.setAnimation(1);

    // set its position
    sprite2.setPosition(340,208);



	while(Device->run())
    {
        driver->beginScene(true, true, video::SColor(255,150,150,150));

		smgr->drawAll();

		// animate sprites
		sprite1.play();
		sprite2.play();

		driver->endScene();
    }
    Device->drop();

    return 0;
}
