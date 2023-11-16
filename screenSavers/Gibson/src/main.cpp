/* Gibson - Screensaver that pays homage to the Gibson in Hackers */

/*
    Copyright © 2011 John Serafino
    This file is part of The Gibson Screensaver.

    The Gibson Screensaver is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Gibson Screensaver is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with The Gibson Screensaver If not, see <http://www.gnu.org/licenses/>.
*/

//-Wl,-subsystem,windows
// _IRR_WINDOWS_ doesn't work until you include irrlicht.h,
// after which further inclusions of windows.h break everything.
// I've replaced it with my own WINSTINKS
// To this effect, just add -DWINSTINKS to the compiler flags when compiling under Windoze,
// and you'll be fine.
#ifdef WINSTINKS
#include <windows.h>
#endif

#include "main.h"
#include "PathCreator.h"

f32 then, delta;


#ifdef WINSTINKS

INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, \
	    LPSTR lpCmdLine, INT nCmdShow)
{

#else
int main(int argc, char **argv)
{

#endif

	//Ray.init(OGL, 800,600, false, false, false);
	dimension2d<u32> res;
	res = dimension2d<u32>(800,600);//getScreenResolution();

	//Ray.init(OGL, res.Width,res.Height, true, false, false);

	/* initialize window */
    //irrlicht=createDevice(driver, dimension2d<u32>(w,h), 32, fs, shadows, false, &rcv);

	SIrrlichtCreationParameters params;
	params.AntiAlias = 4;
	params.Bits = 32;
	params.DriverType = EDT_OPENGL;
	params.EventReceiver = &rcv;
	params.Fullscreen = true;
	params.Stencilbuffer = false;
	params.WindowSize = res;

	#ifdef _IRR_WINDOWS_
	//#include <windows.h>

		if(__argc > 2)
		{
			if(strcmp(__argv[1], "/p") == 0)
				params.WindowId = reinterpret_cast<void*>(atoi(__argv[2]));
		}

	#endif

	irrlicht = createDeviceEx(params);



    Video=irrlicht->getVideoDriver();
    Scene=irrlicht->getSceneManager();
    Gui=irrlicht->getGUIEnvironment();

    // demand 32 bit textures
    Video->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);

    // 2d image filtering
    Video->getMaterial2D().TextureLayer[0].BilinearFilter=true;
    Video->getMaterial2D().AntiAliasing=video::EAAM_FULL_BASIC;

    /* Window created */

    //irrlicht->getFileSystem()->changeWorkingDirectoryTo("/home/netpipe/Dev/IrrlichtDemos/screenSavers/Gibson");


	Ray.hideCursor();
	Video->setAllowZWriteOnTransparent(true);

	IPostProc* ppRenderer = new CRendererPostProc( Scene, dimension2du( 1024, 1024 ), true, true, SColor( 255u, 0,10,15 ) );
	// When setting up the effect, the parameters are:
	// Input, size of output, effect ID (see CEffectPostProc.h for full list), effect parameters (in this case, blur size)
	CEffectPostProc* ppBlur = new CEffectPostProc( ppRenderer, dimension2du( 1024, 1024 ), PP_BLOOM, 1, 0.0005, 1.5);
	//CEffectPostProc* ppBlur = new CEffectPostProc( ppRenderer, dimension2du( 1024, 1024 ), PP_MOTIONBLUR, 0.1);
	//CEffectPostProc* ppBlur = new CEffectPostProc( ppRenderer, dimension2du( 1024, 1024 ), PP_BLOOM, 3, 0.005, 1.5);
	//CEffectPostProc* ppBlur = new CEffectPostProc( ppRenderer, dimension2du( 1024, 1024 ), PP_INVERT);
	// Change to a better quality - not all shaders will respect these, but they can be used to hint the rendering standard required.
	//ppBlur->setQuality( PPQ_BEST );
	ppBlur->setQuality( PPQ_FAST );
	// Options (worst to best): PPQ_CRUDE, PPQ_FAST, PPQ_DEFAULT, PPQ_GOOD, PPQ_BEST
	// You can also call setOverallQuality( PPQ_WHATEVER ) to change the quality of all effects which are in the chain.

	//Ray.importZipFile("D:\\gibson\\data.zip");

	Ray.setWindowTitle(L"The Gibson version 15");

	Image loading;
	loading.loadImg("loading.png");

	Ray.Render.clearScreen(0,0,0,1);

	loading.draw((res.Width/2) - (loading.w/2), (res.Height/2) - (loading.h/2));

   	Gui->drawAll();
   	Video->endScene();

	Room room;
	FlyCam cam;
	PulseSet pulses;
	srand(time(NULL));

	room.init();
	//cam.init();
	pulses.init(100);

	setAmbient(0.4,0.4,0.4, 1);

	cam.init();
	cam.cam.camera->setFarValue(200);

	int lastFPS = Video->getFPS();
	then = Ray.getTime();

	bool kill = false;
	int fps;

	//PathCreator creator(Video, cam.cam.camera, "path.txt", "camPath");

	while(Ray.running() && !key[KEY_ESCAPE])
	{
		delta = (f32)(Ray.getTime() - then) / 1000;
		then = Ray.getTime();

		Ray.Render.clearScreen(0,10,15,1);

		///*

		if(key[KEY_F10])
			Ray.Render.takeScreenshot();


		cam.empty->updateAbsolutePosition();
		cam.cam.camera->setTarget(cam.empty->getAbsolutePosition());
		room.update();
		pulses.update();


		for(int x=0; x < KEY_KEY_CODES_COUNT; x++)
		{
			if(x != KEY_F10 && key[x])
			{
				kill = true;
				break;
			}
		}
		if(kill)
			break;


		if(leftMouse)
			break;
		else if (rightMouse)
            break;
		/*
		if(leftMouse)
			creator.addPath(); // add path to list
		else if (rightMouse)
			creator.delPath(); // delete path from list
		else if (key[KEY_KEY_C])
			creator.conPath(); // connect path
		else if (key[KEY_KEY_Z])
			creator.save(); // save list to file


       	creator.drawPath();
		Ray.Render.drawScreen();
		Ray.Render.endScene();
		*/




        // The rendering is as normal, except smgr->drawAll(); is replaced with this line:
       	ppBlur->render( NULL ); // NULL = render to screen. Can also take a texture to render to, or no parameter (renders to an internal texture)
       	//*/
       	//Scene->drawAll();

       	Gui->drawAll();
       	Video->endScene();

        irrlicht->sleep(60);
       	/*
        fps = Video->getFPS();

        if (lastFPS != fps)
        {
                core::stringw str = L"The Gibson Version 14 ";
                str += "FPS: ";
                str += fps;

                irrlicht->setWindowCaption(str.c_str());
                lastFPS = fps;
        }
        */
	}

	delete ppBlur;
	delete ppRenderer;


	/*
	cout << cam.cam.camera->getAbsolutePosition().X << ',' \
			<< cam.cam.camera->getAbsolutePosition().Y << ',' \
			<< cam.cam.camera->getAbsolutePosition().Z << '\n';
	cout << cam.empty->getAbsolutePosition().X << ',' \
			<< cam.empty->getAbsolutePosition().Y << ',' \
			<< cam.empty->getAbsolutePosition().Z << '\n';
			*/
	cout << "Made it to end! Attempting Ray.exit...\n";

	Ray.exit();

	cout << "Ray3D is dead!\n";

	exit(0);

	// This shouldn't be necessary
	return 0;
}
