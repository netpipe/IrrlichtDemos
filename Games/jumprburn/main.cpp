#include <irrlicht.h>
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace irr;

#pragma comment(lib, "Irrlicht.lib")

// bool keys for reading the keyboard
bool keys[irr::KEY_KEY_CODES_COUNT];

//
class MyEventReceiver : public IEventReceiver
{
	public:
	virtual bool OnEvent(const SEvent& event)
	{
        // deal with keyboard
        if(event.EventType == irr::EET_KEY_INPUT_EVENT){
            keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
            return true;
        }
		return false;
	}
};


bool getKeyState(EKEY_CODE key)
{
  return keys[key];
}

int main()
{
	// let user select driver type
	MyEventReceiver receiver;
	// create device

	IrrlichtDevice *device = createDevice(irr::video::EDT_OPENGL,
		core::dimension2du(800, 600),
		16, false, false, false, &receiver);

	device->setWindowCaption(L"burning");

	scene::ISceneManager *smgr = device->getSceneManager();

	video::IVideoDriver* driver = device->getVideoDriver();

    // load the textures
	video::ITexture* images[7];
    images[0] = driver->getTexture("fire.png");
    images[1] = driver->getTexture("fireman.png");
    driver->makeColorKeyTexture(images[1], core::position2d<s32>(0,0));
    images[2] = driver->getTexture("window.png");
    images[3] = driver->getTexture("water.png");
    images[4] = driver->getTexture("man1.png");
    driver->makeColorKeyTexture(images[4], core::position2d<s32>(0,0));
    images[5] = driver->getTexture("manfall.png");
    driver->makeColorKeyTexture(images[5], core::position2d<s32>(0,0));
    images[6] = driver->getTexture("stretcher.png");
    driver->makeColorKeyTexture(images[6], core::position2d<s32>(0,0));

    // reset keyboard
   //	for(int n=0; n<irr::KEY_KEY_CODES_COUNT; n++) keys[n] = false;

    int startpos = 30;
    int gapx = 200;
    int gapy = 150;

    int windowfires[12];
    for (int n = 0; n < 12; n++) windowfires[n]=0;

    int windowdudes[12];
    for (int n = 0; n < 12; n++) windowdudes[n]=0;

    bool wateractive[30];
    for (int n = 0; n < 30; n++) wateractive[n]=false;
    core::position2d<s32> waterpos[30];

    int stretcherpos = 300;
    int difficulty = 0;


    int score = 0;
    int casualties = 0;
    int maxdudetime = 1000;

    bool gameover = false;

    wchar_t t[64];
    wprintf( t, L"Score: %d, Lives %d",score,10-casualties);
    device->setWindowCaption(t);

    u32 time =device->getTimer()->getTime();
    u32 lastfire = time;
    u32 lastdude = time;


	while(device->run() && gameover == false)
	{
            u32 lasttime = time;
			time = device->getTimer()->getTime();

       //     device->getTimer()->tick();


            // move window dudes
            for (int n=0;n<12; n++) {
                if (windowdudes[n] > 0)
                {
                    windowdudes[n]+=1;
                    // if there's a fire in this window, we jump now!
                    if (windowfires[n] != 0 && windowdudes[n] < maxdudetime )
                    {
                        windowdudes[n] = maxdudetime;
                    }
                }
            }

            // move stretcher
            if ( keys[KEY_KEY_A] && stretcherpos > 0 )    stretcherpos -= 1;
            if ( keys[KEY_KEY_S] && stretcherpos < 672 )  stretcherpos += 1;

           if ( keys[KEY_ESCAPE] ) gameover = true;


            // put out fires,
            core::position2d<s32> p = device->getCursorControl()->getPosition();
            for(int n=0; n<4;n++)
                for(int m=0; m<3;m++)
                {
                    if ( p.X > startpos+(n*gapx) &&  p.X < startpos+(n*gapx) + 128 &&
                         p.Y > startpos+(m*gapy) &&  p.Y < startpos+(m*gapy) + 128)

                    {

                        if ( windowfires[n+(m*4)] > 0)
                        {
                            windowfires[n+(m*4)] = 0;
                            score += 10;
                        }

                         // soak dudes!
                         if (windowdudes[n+(m*4)] > 0 && windowdudes[n+(m*4)] < maxdudetime)
                         {
                            casualties++;
                            score -= 50;
                            windowdudes[n+(m*4)] = 0;
                            wchar_t t[64];
                            wprintf( t, L"Score: %d, Lives %d",score,10-casualties);
                            device->setWindowCaption(t);
                         }
                    }

                    if ( startpos+(m*gapy) + windowdudes[n+(m*4)] - maxdudetime > 450 )
                    {
                        windowdudes[n+(m*4)] = 0;
                        // casualty?
                        windowdudes[n] = 0;
                        if ( (startpos+(n*gapx) > stretcherpos - 32) &&
                              startpos+(n*gapx) < stretcherpos + 160 )
                        {
                            score += 50;
                        }
                        else
                        {

                            casualties ++;
                            score -=50;
                            wchar_t t[64];
                            wprintf( t, L"Score: %d, Lives %d",score,10-casualties);
                            device->setWindowCaption(t);
                        }

                    }
                }


            // new fire?
            if (time - lastfire > 1000 + (rand() % (7000 - difficulty))) {
                windowfires[rand() % 12] = 1;
                lastfire = time;
            }

            // new window dude?
            if (time - lastdude > 2000 + (rand() % (10000 - difficulty))) {
                int x = rand() % 12;
                if (windowdudes[x] == 0 )
                {
                    windowdudes[x]  = 1;
                    lastdude = time;
                }
            }



    //BEGIN SCENE
			driver->beginScene(true, true, video::SColor(0,120,102,136));

            // draw windows
            core::rect<s32> a = core::rect<s32>(0,0,64,64);
            core::rect<s32> b = core::rect<s32>(0,0,128,128);

            for(int n=0; n<4;n++)
                for(int m=0; m<3;m++)
                {
                    driver->draw2DImage( images[2], core::position2d<s32>(startpos+(n*gapx),startpos+(m*gapy)) );
                    // is there fire in this window?
                    if (windowfires[n+(m*4)] > 0)
                    {
                        // draw fire!
                        driver->draw2DImage(images[0], core::position2d<s32>(startpos+(n*gapx)+32,startpos+(m*gapy)+60),
                            a, 0, video::SColor(255,time % 255,time % 255,time % 255), true);
                    }
                }

            for(int n=0; n<4;n++)
                for(int m=0; m<3;m++)
                {
                    // draw dudes
                    if (windowdudes[n+(m*4)] > 0)
                    {
                        // is he falling?
                        if (windowdudes[n+(m*4)] < maxdudetime)
                        {
                            // draw dude!
                            driver->draw2DImage(images[4], core::position2d<s32>(startpos+(n*gapx),startpos+(m*gapy)),
                                b, 0, video::SColor(255,255,255,255), true);
                        }
                        else
                        {
                            // draw falling dude!
                            driver->draw2DImage(images[5], core::position2d<s32>(startpos+(n*gapx),startpos+(m*gapy) + windowdudes[n+(m*4)] - maxdudetime),
                                b, 0, video::SColor(255,255,255,255), true);
                        }
                    }
                }

            // draw stretcher
            a = core::rect<s32>(0,0,128,64);
            driver->draw2DImage(images[6], core::position2d<s32>(stretcherpos, 480),a, 0, video::SColor(255,255,255,255), true);

            // draw water
            for (int n = 1; n<10; n++)
            {
                //
            }

            // draw fireman
            a = core::rect<s32>(0,0,128,128);
            driver->draw2DImage(images[1], core::position2d<s32>(device->getCursorControl()->getPosition().X-64, 500),a, 0, video::SColor(255,255,255,255), true);

        smgr->drawAll();
			driver->endScene();

            while (time < lasttime + 2){
                device->getTimer()->tick();
			     time = device->getTimer()->getTime();
            }

            difficulty = score - 1000;


			if (casualties > 0)
            {
                irr::core::stringw t = "Game over! Score";
                // wprintf( t, L"Game over! Score: %d",score);

                 //t = score;

			    device->setWindowCaption(t.c_str());
device->run();
                u32 lasttime = time;

                while (time < lasttime + 2000){
                       printf ( "out %c" , t.c_str());
                    printf ("the time is %i \n",time);
			         time = device->getTimer()->getTime();
			         device->getTimer()->tick();
                }

                gameover = true;
            }
	}
    device->drop();
	return 0;
}
