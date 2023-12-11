/*
    Spirals by Pierre Cusa (pierzone@hotmail.com)
    june 2008
*/


#include <irrlicht.h>
#include <iostream>
#define _FIRSTCIRCLE 1
#define _CIRCLESTEP 1

#define _MAXCIRCLES 600
#define _X 400
#define _Y 300

#define _MAXIMGS 4

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IrrlichtDevice *device = 0;


//the two following functions are actually the most important thing in the program
bool dot(double angle, int circle, IVideoDriver *idriver, ITexture *dottex) {
     if (dottex == 0)
     idriver->draw2DPolygon  (  position2d<s32>(_X+(sin(angle)*_CIRCLESTEP*circle),
                                                _Y+(cos(angle)*_CIRCLESTEP*circle)), 3);
     else
     idriver->draw2DImage  (  dottex,
                           position2d< s32 >(_X+(sin(angle)*_CIRCLESTEP*circle)-8,
                                             _Y+(cos(angle)*_CIRCLESTEP*circle)-8),
                           rect<s32>(0,0,16,16), 0, SColor(255,255,255,255), true);


}

bool drawspiral(double inangle, IVideoDriver *idriver, ITexture *dottex) {
     double tempangle = 0;
     for (int i = _FIRSTCIRCLE; i <= _MAXCIRCLES; i++) {
         dot (tempangle, i, idriver, dottex);
         tempangle += inangle;
         }
}
//not much, eh?

//----------------------------------



class Evre : public IEventReceiver
{
public:

	Evre(ITexture** intex, double *instep, bool *inPaused, bool *inVisible, IGUIScrollBar* inbar)
	{
		curtex = intex;
		step = instep;
		paused = inPaused;
		UIvisible = inVisible;
		stringc dotfilename = L"";
		speedbar = inbar;
		for (int i=1; i<=_MAXIMGS; i++) {
            dotfilename = "dot";
            dotfilename += i;
            dotfilename += ".bmp";
            dottexs[i-1] = device->getVideoDriver()->getTexture(dotfilename.c_str());
            device->getVideoDriver()->makeColorKeyTexture(dottexs[i-1], position2d< s32 >(0,0));
            }
	curdotfilenumber = 0;
    }

	bool OnEvent(const SEvent& event)
	{
		// check if user presses a key
		if (event.EventType == EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
		{
			switch (event.KeyInput.Key)
			{
			case irr::KEY_UP: // increment speed
				*step += 0.000001;
				speedbar->setPos((*step*-1000000)+50);
				return true;
			case irr::KEY_DOWN: // decrement speed
				*step -= 0.000001;
				speedbar->setPos((*step*-1000000)+50);
				return true;
			case irr::KEY_SPACE: // toggle pause
			     *paused = !(*paused);
				 return true;
			case irr::KEY_KEY_Q: // cycle dot image
			     if (curdotfilenumber >= _MAXIMGS-1)
			        curdotfilenumber = 0;
                 else
                     curdotfilenumber++;
                 *curtex = dottexs[curdotfilenumber];
				 return true;
			case irr::KEY_KEY_H: // hide/show user interface
			     *UIvisible = !*UIvisible;
				 return true;
			case irr::KEY_ESCAPE: // quit
			     device -> closeDevice();
				 return true;
			}
        }
        //check if user touched the interface
		if (event.EventType == EET_GUI_EVENT)
		{
		 s32 id = event.GUIEvent.Caller->getID();
         IGUIEnvironment* env = device->getGUIEnvironment();
         switch(event.GUIEvent.EventType)
			{

			case EGET_SCROLL_BAR_CHANGED:
					*step = (speedbar->getPos()-50)*-0.000001;
				return true;

			case EGET_BUTTON_CLICKED:

				if (id == 101)// change dot image
				{
			     if (curdotfilenumber >= _MAXIMGS-1)
			        curdotfilenumber = 0;
                 else
                     curdotfilenumber++;
                 *curtex = dottexs[curdotfilenumber];
				 return true;
				}

        }
        }

		return false;
	}

private:
	ITexture *dottexs[_MAXIMGS];
	ITexture **curtex;
	int curdotfilenumber;
    double *step;
    bool *paused;
    bool *UIvisible;
    IGUIScrollBar* speedbar;
};


int main(int argc, char *argv[]) {

    bool doFullScreen = false;
    if (argc == 2)
             if (stringc("-fullscreen") == argv[1])
                doFullScreen = true;


    double angle = 0;
    double curstep = 0.0000;
    bool isPaused = 0;
    bool UIvisible = 1;
    double waittime = 1;

    device = createDevice(EDT_OPENGL, dimension2d<s32>(800,600), 16, doFullScreen);
    if (device==0)
       return 1;
    IVideoDriver *driver = device-> getVideoDriver();
    ISceneManager *smgr = device -> getSceneManager();
    IGUIEnvironment *env = device -> getGUIEnvironment();

    // ----------------------- User Interface starts here
    IGUISkin* skin = env->getSkin();
	IGUIFont* font = env->getFont("fonthaettenschweiler.bmp");
	if (font)
		skin->setFont(font);
	skin->setColor(EGDC_BUTTON_TEXT, SColor (255, 0, 200, 0));

    device -> setWindowCaption(L"spirals");

    IGUITab *bgtab = env->addTab(rect<s32>(550, 10, 800, 580), 0, -1);
    bgtab->setBackgroundColor  (  SColor(100,0,0,50));
    bgtab->setDrawBackground(true);

    IGUIScrollBar* speedbar = env->addScrollBar(false, rect<s32>(770, 10, 794, 580), 0, 113);
	speedbar->setMax(100);
	speedbar->setPos(50);

    stringw tempstr = L"Angle : ";
    tempstr += angle;
	IGUIStaticText* angleDisplay = env->addStaticText  ( tempstr.c_str(),
                                                         rect< s32 >(560, 40, 750, 60),
                                                          1, 1, 0, -1, 1);
    tempstr = L"Speed : ";
    tempstr += (curstep*1000000);
	IGUIStaticText* speedDisplay = env->addStaticText  ( tempstr.c_str(),
                                                         rect< s32 >(560, 80, 750, 100),
                                                          1, 1, 0, -1, 1);
    tempstr = L"Spiral : ";
    tempstr += "1";
	IGUIStaticText* factorDisplay = env->addStaticText  ( tempstr.c_str(),
                                                         rect< s32 >(560, 120, 750, 140),
                                                          1, 1, 0, -1, 1);

    IGUIStaticText* quickhelp = env->addStaticText  ( L"Quick help :\n\n   up/down keys or scrollbar : change speed\n\n   Q : change dot type\n\n   Spacebar : pause\n\n   H : hide interface",
                                                         rect< s32 >(560, 160, 750, 360),
                                                          1, 1, 0, -1, 1);


    env->addButton(rect<s32>(560, 450, 700, 480), 0, 101, L"Change dot type(Q)");
    //------------------------- User Interface ends here


    smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));
    ITexture *dottex = driver->getTexture("dot1.bmp");
    driver->makeColorKeyTexture(dottex, position2d< s32 >(0,0));

    Evre theevre(&dottex, &curstep, &isPaused, &UIvisible, speedbar);
	device->setEventReceiver(&theevre);

    int lastFPS = -1;
    u32 lasttime = device->getTimer()->getTime();
    while(device->run()) {
                         driver->beginScene(true, true, SColor(0,0,00,00));
                         smgr->drawAll();
                         drawspiral (angle, driver, dottex);
                         if (UIvisible == true) {
                            tempstr = L"Angle : ";
                            tempstr += angle;
                            angleDisplay->setText(tempstr.c_str());
                            tempstr = L"Speed : ";
                            tempstr += curstep*1000000;
                            speedDisplay->setText(tempstr.c_str());
                            tempstr = L"Spiral : ";
                            if (angle == 0)
                                tempstr += "1";
                            else if (angle > PI)
                               tempstr += (PI/((PI*2)-angle))*2;
                            else
                               tempstr += (PI/angle)*2;
                            factorDisplay->setText(tempstr.c_str());
                            env->drawAll();
                         }

                         u32 thistime = device->getTimer()->getTime();
                         waittime = (thistime - lasttime);
                         if (isPaused==0)
                            angle += curstep * waittime;
                         lasttime = thistime;
                         if (angle > PI*2)
                            angle = 0;
                         if (angle < 0)
                            angle = PI*2;
                         driver->endScene();

  		                 int fps = driver->getFPS();
                         if (lastFPS != fps)
                     		{
                     			core::stringw str = L"Spirals [";
                    			str += driver->getName();
                    			str += "] FPS:";
                    			str += fps;

                    			device->setWindowCaption(str.c_str());
                    			lastFPS = fps;
                    		}
                }
    device -> drop();
    return 1;
}
