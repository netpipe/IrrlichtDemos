/*
ARKANOID - Breakoutclone Game 4 Wizzard of OZ4
Autor: Dirk Hoeschen alias RubbelDieK@tz
Email: rubbel@c-base.org
This Game is public domain.
*/

/* Uncomment to use sound */
//#define USE_IRRKLANG

#include <stdio.h>
#include <wchar.h>
#include <irrlicht.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define SDLMIXER
	#ifdef SDLMIXER
	#include "sound.h"
#endif

    int lastFPS = 0;


/*

In the Irrlicht Engine, everything can be found in the namespace
'irr'. So if you want to use a class of the engine, you have to
write an irr:: before the name of the class.
*/
using namespace irr;


#ifdef USE_IRRKLANG
#include <irrklang.h>
using namespace audio;
#pragma comment(lib, "irrKlang.lib")
ISoundEngine* sound;
#endif


/*
There are 5 sub namespaces in the Irrlicht Engine. Take a look
at them, you can read a detailed description of them in the
documentation by clicking on the top menu item 'Namespace List'
or using this link: http://irrlicht.sourceforge.net/docu/namespaces.html.
Like the irr Namespace, we do not want these 5 sub namespaces now,
to keep this example simple. Hence we tell the compiler again
that we do not want always to write their names:
*/
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/*
To be able to use the Irrlicht.DLL file, we need to link with the
Irrlicht.lib. We could set this option in the project settings, but
to make it easy, we use a pragma comment lib:
*/
#pragma comment(lib, "Irrlicht.lib")
core::array<video::ITexture*> textures;
scene::IVolumeLightSceneNode * test;

scene::ISceneNode* batter;
scene::IAnimatedMeshSceneNode* ball;
core::vector3df  bpos;
core::aabbox3df cbox;
IrrlichtDevice * device;
gui::IGUIStaticText* statusText;
gui::IGUIEnvironment * env;
gui::IGUIButton * butt1;
gui::IGUIButton * butt2;
scene::ICameraSceneNode* cam;
video::IVideoDriver* driver;
scene::ISceneManager* smgr;
scene::ISceneNodeAnimator* anim;
scene::ISceneNodeAnimator* anim2;

/* Declare Functions */
void GameMenu();
void StartGame();
void DrawPlayfield();
void InitLevel();
void SoundFx();

/* public variables */
int   speedx = 40;
int   speedz = 200;
int   batterpos = 0;
int   ballsleft = 3;
int   cratesleft = 0;
int   level = 0;
int   screenwidth = 1024;
int   screenheight = 764;
bool  changedir = true; // change balldirectory
bool  gamestart = false;

/* array with the bricks/crates */
core::array<scene::ISceneNode*> crates;
const int numlevels=3;

/* create some fancy levels with 5 different bricks */
int lev[3][27] ={  //level 1
                {0,0,0,1,1,1,0,0,0,
                 0,1,1,1,5,1,1,1,0,
                 0,0,0,1,1,1,0,0,0},
                 //level
                {0,0,1,5,1,5,1,0,0,
                 0,5,1,1,1,1,1,5,0,
                 0,0,2,0,5,0,2,0,0},
                 //level 3
                {1,1,1,1,1,1,1,1,1,
                 1,5,1,5,1,5,1,5,1,
                 1,1,1,1,1,1,1,1,1} };

class MyEventReceiver : public IEventReceiver
{
public:
	virtual bool OnEvent(const SEvent& event)
	{
	if (!device)
		return false;
    /* Gui Event */
    if (!gamestart) {
 	  if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();
            switch(event.GUIEvent.EventType)
			{
			case EGET_BUTTON_CLICKED:
				if (id == 101)
				{
					device->closeDevice();
					exit(1);
					return true;
				}
				if (id == 102)
				{
                    // switch off menu
                    butt1->setVisible(false);
                    butt2->setVisible(false);
                    // static camera
                 	cam->removeAnimator(anim2);
                 	cam->setTarget(core::vector3df(0,-20,0));
                 	cam->setPosition(core::vector3df(10,50,-90));
                    speedz = 200; speedx = 40;
                    ball->setPosition(core::vector3df(0,-9,-45));
                    ball->removeAnimator(anim);
                    device->getCursorControl()->setVisible(false);
                    level = 0;
                    InitLevel();
				    changedir = true;
				    gamestart = true;
					return true;
				}
				break;
			}

       }
     } else {
   	  /* mouse events */
	  if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
		{
             core::vector3df v = batter->getPosition();
			 v.X = ((event.MouseInput.X-(screenwidth/2))/(screenwidth/100));
			 if ((v.X) >=42.5) { v.X = 42.5; }
			 if ((v.X) <=-42.5) { v.X = -42.5; }
			 batter->setPosition(v);
			 batterpos = int(v.X);
			 return true;
		}
     }
   	/* key events */
    if (event.EventType == irr::EET_KEY_INPUT_EVENT && event.KeyInput.PressedDown == false)
       {
	 	if (event.KeyInput.Key == KEY_F9) {
	 	   video::IImage* image = device->getVideoDriver()->createScreenShot();
           if (image)
           {
		     device->getVideoDriver()->writeImageToFile(image, "screenshot.bmp");
		     image->drop();
           	 return true;
           }
	 	}
	 	if (event.KeyInput.Key == KEY_KEY_Q)
	 	{
		 	device->closeDevice();
          	 return false;
	  }
	}
	return false;
	}
};


void main_loop(){
	/* let the game begin	*/

device->run();
		/* start the scene*/
		driver->beginScene(true, true, SColor(255,100,101,140));
		smgr->drawAll();
		env->drawAll();
		driver->endScene();
	    if (!gamestart) { // game not started, show information
            int fps = driver->getFPS();
            if (lastFPS != fps)
            {
  			wchar_t tmp[256];
			swprintf(tmp, 255, L"%ls fps: %d", driver->getName(),	driver->getFPS());
			statusText->setText(tmp);
    		lastFPS = fps;
            }
	    } else { // game started, show level and ballsleft
  			wchar_t tmp[256];
			swprintf(tmp, 255, L"Level: %d  Balls: %d", level+1, ballsleft);
            statusText->setText(tmp);
	    }

        bpos = ball->getAbsolutePosition();

        /* check boundaries */
  		if (bpos.X > 50) { speedx = -abs(speedx); bpos.X=50.0f; changedir=true; }
  		if (bpos.X < -50) { speedx = abs(speedx); bpos.X=-50.0f; changedir=true; }
  		if (bpos.Z > 50)  { speedz = -200;  changedir=true; bpos.Z = 50.0f; }
  		else if (bpos.Z <= -60) { // Z -60 is behind the batter
		   bpos.Z = -49.0f;
		   changedir=true;
  	       if (!gamestart) {
  		     speedz = 200;
  		   } else {
              ballsleft--;
              ball->removeAnimator(anim);
   		      bpos.Z = -45.0f; bpos.X = 0;
              speedz = 200; speedx = 40;
             if (ballsleft<1) // Game over
    	     {
               gamestart=false;
               ballsleft = 3;
               level = 0;
               InitLevel();
               // show menu
               butt1->setVisible(true);
               butt2->setVisible(true);
               device->getCursorControl()->setVisible(true);
               // animate camera
               cam->setTarget(core::vector3df(0,0,0));
	           anim2 = smgr->createFlyCircleAnimator(core::vector3df(0,30,-100), 30.0f);
	           cam->addAnimator(anim2);
    	     }
  		   }
  		} else if ((bpos.Z > -3) || (bpos.Z < 33))	{
            /* check collision to crate */
           int n;
           for (n=0; n<=26; ++n) {
              cbox = crates[n]->getTransformedBoundingBox(); // get the bounding box
              core::vector3df  bmin = cbox.MinEdge; core::vector3df  bmax = cbox.MaxEdge;
              // blow the box up a little bit, because of the size of the ball
              cbox.MinEdge = core::vector3df(bmin.X-1,bmin.Y,bmin.Z-1);
              cbox.MaxEdge = core::vector3df(bmax.X+1,bmax.Y,bmax.Z+1);
              if (cbox.isPointInside(bpos)) { // is the Ball inside the box
  	 	         if (crates[n]->isVisible()) { // destroy only visible crates
                    /* from which direction the ball hits? not very elegant, but fast.  */
                    f32 bdir[4] = { fabs(bpos.X-bmax.X), fabs(bpos.X-bmin.X),
                                  fabs(bpos.Z-bmax.Z), fabs(bpos.Z-bmin.Z) };
                    int i = 0; int k = 0;
                    for (i=0; i<4; ++i) {
                        if (bdir[i]<bdir[k]) { k=i; } // get the nearest side
                    }
                    if ((k%2)==0) { i=1; } else { i=-1; } // vector + or -
                    if (k<2){ // change X-Speed
                       speedx = (i * abs(speedx)) + (1 + ( rand() % 5 ));
                    } else { // change Z-Speed
                       speedz = (i * abs(speedz)) + (1 + ( rand() % 15 ));
                    }
                    changedir=true;

                          	 	           test->setPosition(core::vector3df((bpos.X,bpos.Y,bpos.Z)));


   	 	            if (lev[level][n]<5) { // crate 5 is undestroyable
      	 	           crates[n]->setVisible(false); // disable crate
      	 	           cratesleft--;
      	 	           /* all crates destroyed? */
      	 	            if (cratesleft<=0) {
      	 	              level++;
                      	  if (level>=numlevels) { // last level = restart
                      	      level=0;
                      	  }
                      	  InitLevel();
                  	      // ball to startposition
                          ball->removeAnimator(anim);
                          bpos.Z = -45.0f; bpos.X = 0;
                          speedz = 200; speedx = 40;
      	 	            }
   	 	             }
  	 	          }
              }
           }
       }

       if ((bpos.Z <= -50) && (bpos.Z >= -54)) {
            /* is the ball hitting the batter? */
  		    int bvector = int(bpos.X) - batterpos;
     		if ((bvector <= 8) && (bvector >= -8)) {
                 speedz = 200;
                 speedx = (bvector*40); // recalculate the x-speed-vector
                 bpos.Z = -49.0f;
                 /* move into new direction */
                 changedir=true;
     		}

        }

       if (changedir) { // switch ballanimation
           ball->removeAnimator(anim);
           ball->setPosition(core::vector3df(bpos.X,-9.0f,bpos.Z));
            /* fly with the ball to another direction */
            anim = smgr->createFlyStraightAnimator(core::vector3df(bpos.X,-9.0f,bpos.Z),
                                                   core::vector3df((bpos.X+speedx),-9.0f,(bpos.Z+speedz)), 2500, true);
            ball->addAnimator(anim);
            SoundFx();
            changedir = false;

       }
	//  }
	//}

}
/*
This is the main method.
*/
int main()
{
	#ifdef USE_IRRKLANG
   // start the sound engine with default parameters
	sound = createIrrKlangDevice();

	if (sound)
	{	// play some sound stream, looped
	    sound->play2D("./media/getout.ogg", true);
	}
	#endif
	/*
	The most important function of the engine is the 'createDevice'
	function. The Irrlicht Device can be created with it, which is the
	root object for doing everything with the engine.
	*/
    #ifdef __EMSCRIPTEN__
	device = createDevice( video::EDT_OGLES2, dimension2d<u32>(screenwidth, screenheight), 32,
			false, true, true);
			#else
				device = createDevice( video::EDT_OPENGL, dimension2d<u32>(screenwidth, screenheight), 32,
			false, true, true);
			#endif



	device->setWindowCaption(L"WOS 06 Game Demo - Irrlicht Engine");

	/*
	Get a pointer to the video driver, the SceneManager and the
	graphical user interface environment, so that
	we do not always have to write device->getVideoDriver(),
	device->getSceneManager() and device->getGUIEnvironment().
	*/
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();

	MyEventReceiver receiver;
 	device->setEventReceiver(&receiver);

	env = device->getGUIEnvironment();

    /* initialize the playfield */
    DrawPlayfield();

    /* show the menu */
	GameMenu();



    #ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(main_loop,0,1);
#else
    while(driver)
	{
	main_loop();
}
#endif

	/*
	After we are finished, we have to delete the Irrlicht Device
	created before with createDevice().
	*/
	device->drop();

	return 0;
}

/*
The Arkanoid Game Functions
*/
void GameMenu()  // Init Game Menu
{
	if (!device) return;

	IGUISkin* skin = env->getSkin();
	IGUIFont* font = env->getFont("./media/fontoffi.png");
	if (font)
		skin->setFont(font);

	skin->setColor(EGDC_BUTTON_TEXT,video::SColor(255,255,220,100));

	env->addImage(driver->getTexture("./media/woslogo.tga"),
		core::position2d<s32>(5,5));

	// Statustext
	const int lwidth = 450;
	const int lheight = 50;
	core::dimension2du size = device->getVideoDriver()->getScreenSize();
	core::rect<int> pos(10, size.Height-lheight-10, 10+lwidth, size.Height-10);
	env->addImage(pos);

	statusText = env->addStaticText(L"Irrlicht WOS 06 Demo", pos, true,false);
	statusText->setOverrideColor(video::SColor(255,205,200,200));

	/* Buttons	*/
	int lastFPS = -1;
    int halbe = (screenwidth/2);
	butt1 = env->addButton(rect<s32>(halbe-100,210,halbe+100,250), 0, 102, L"New Game");
	butt2 = env->addButton(rect<s32>(halbe-100,290,halbe+100,330), 0, 101, L"Quit");

    InitLevel();

	cam = smgr->addCameraSceneNode();
	cam->setTarget(core::vector3df(0,0,0));
	anim2 = smgr->createFlyCircleAnimator(core::vector3df(0,30,-100), 30.0f);
	cam->addAnimator(anim2);

}

void StartGame()
{
}

void InitLevel()  // Draw Playfield
{
	video::IVideoDriver* driver = device->getVideoDriver();

	 test = smgr->addVolumeLightSceneNode(0, -1,
				32,                              // Subdivisions on U axis
				32,                              // Subdivisions on V axis
				video::SColor(0, 255, 255, 255), // foot color
				video::SColor(0, 0, 0, 0));      // tail color

	if (test)
	{
		test->setScale(core::vector3df(5.0f, 5.0f, 5.0f));
		test->setPosition(core::vector3df(0,0,0));




		for (s32 g=7; g > 0; --g)
		{
			core::stringc tmp("../../media/");
			tmp += "portal";
			tmp += g;
			tmp += ".bmp";
			video::ITexture* t = driver->getTexture( tmp.c_str() );
			textures.push_back(t);
		}

	}


		// create texture animator
		scene::ISceneNodeAnimator* glow = smgr->createTextureAnimator(textures, 100);

//		test->setPosition(core::vector3df((bpos.X,bpos.Y,bpos.Z)));

		// add the animator
		test->addAnimator(glow);


		// drop the animator because it was created with a create() function
		glow->drop();

    int n;
    cratesleft=0;
    for (n=0; n<=26; ++n) {
        crates[n]->setVisible(true);
        if ((lev[level][n]>0) && (lev[level][n]<5)) {  cratesleft++; }
        if (lev[level][n]==1) {
    	  crates[n]->setMaterialTexture(0, driver->getTexture("./media/crate.jpg"));
        } else if (lev[level][n]==2) {
    	  crates[n]->setMaterialTexture(0, driver->getTexture("./media/streifen.jpg"));
        } else if (lev[level][n]==3) {
    	  crates[n]->setMaterialTexture(0, driver->getTexture("./media/crate.jpg"));
        } else if (lev[level][n]==4) {
    	  crates[n]->setMaterialTexture(0, driver->getTexture("./media/crate.jpg"));
        } else if (lev[level][n]==5) {
          crates[n]->setVisible(true);
    	  crates[n]->setMaterialTexture(0, driver->getTexture("./media/metall.jpg"));
        } else {
          crates[n]->setVisible(false);
        }
    }
}

void DrawPlayfield()  // Draw Playfield
{
	/*
	add Skybox
	*/
	smgr->addSkyBoxSceneNode(
	driver->getTexture("./media/ba-top.jpg"),
	driver->getTexture("./media/ba-bottom.jpg"),
	driver->getTexture("./media/ba-left.jpg"),
	driver->getTexture("./media/ba-right.jpg"),
	driver->getTexture("./media/ba-front.jpg"),
	driver->getTexture("./media/ba-back.jpg"));

	/*
	Bottom
	*/
    scene::ISceneNode* bottom = 0;
	bottom = smgr->addCubeSceneNode(1);
	bottom->setPosition(core::vector3df(0,-12,0));
	bottom->setScale(core::vector3df(110,5,110));
	bottom->setMaterialTexture(0, driver->getTexture("./media/boden.jpg"));
	bottom->setMaterialFlag(video::EMF_LIGHTING, true);

	/*
	Sides
	*/
    scene::ISceneNode* side = 0;
	side = smgr->addCubeSceneNode(1);
	side->setPosition(core::vector3df(-52.5,-7,0));
	side->setScale(core::vector3df(5,5,110));
	side->setMaterialTexture(0, driver->getTexture("./media/wall.jpg"));
	side->setMaterialFlag(video::EMF_LIGHTING, true);

	side = smgr->addCubeSceneNode(1);
	side->setPosition(core::vector3df(52.5,-7,0));
	side->setScale(core::vector3df(5,5,110));
	side->setMaterialTexture(0, driver->getTexture("./media/wall.jpg"));
	side->setMaterialFlag(video::EMF_LIGHTING, true);

	side = smgr->addCubeSceneNode(1);
	side->setPosition(core::vector3df(0,-7,52.5));
	side->setScale(core::vector3df(100,5,5));
	side->setMaterialTexture(0, driver->getTexture("./media/wall.jpg"));
	side->setMaterialFlag(video::EMF_LIGHTING, true);

	/*
	Get ball
	*/
	IAnimatedMesh* bb = smgr->getMesh("./media/ball.x");
	ball = smgr->addAnimatedMeshSceneNode(bb);

	ball->setMaterialTexture(0, driver->getTexture("./media/ball.jpg"));

	// make the model a little bit smaler and normalize its normals
	// because of this for correct lighting
	ball->setScale(core::vector3df(1,1,1));
	ball->setMaterialFlag(video::EMF_LIGHTING, false);

	// add shadow
	ball->addShadowVolumeSceneNode();
	smgr->setShadowColor(video::SColor(60,0,0,0));

    // rotate the ball
    anim2 = smgr->createRotationAnimator(core::vector3df(2.0f,5.3f,1.0f));
    ball->addAnimator(anim2);

    // add 2 lights
	bottom = smgr->addLightSceneNode(0, core::vector3df(20,30,40),
		video::SColorf(1.0f, 0.5f, 0.7f, 1.0f), 100.0f);

	bottom = smgr->addLightSceneNode(0, core::vector3df(20,20,-80),
		video::SColorf(1.0f, 0.8f, 0.2f, 1.0f), 200.0f);

	/* 	add the batter 	*/
	batter = smgr->addCubeSceneNode(10);
	batter->setPosition(core::vector3df(0,-7,-52.5));
	batter->setMaterialTexture(0, driver->getTexture("./media/bronze.jpg"));
	batter->setScale(core::vector3df(1.5,0.5,0.2));
	batter->setMaterialFlag(video::EMF_LIGHTING, true);


    /* add the crates / bricks to the scene */
    int cx; int cz;
    scene::ISceneNode* crate = 0;
    for (cz=0; cz<=30; cz=cz+15)
     {
      for (cx=-40; cx<=40; cx=cx+10)
      {
    	crate = smgr->addCubeSceneNode(6);
		crates.push_back(crate);
	    crate->setPosition(core::vector3df(cx,-6,cz));
        crate->setMaterialFlag(video::EMF_LIGHTING, true);
      }
    }

}

void SoundFx() {

	#ifdef USE_IRRKLANG
	if (sound) {
       sound->play2D("./media/klack.wav",false);
	}
	#endif
	#ifdef SDLMIXER
mainplay("./media/klack.wav");
#endif
}
