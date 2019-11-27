#include <cstdlib>
#include <time.h>
#include <irrlicht.h>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace std;

        ISceneNode *cube;
        ISceneNode *cube2;

            ISceneNode *ball;

    ISceneNode *collisionhorisontal1;
    ISceneNode *collisionhorisontal12;
        ISceneNode *collisionvertical;
    ISceneNode *collisionvertical2;


        IrrlichtDevice *device;

    IVideoDriver* driver;
    ISceneManager* smgr;
    IGUIEnvironment* guienv;


        int score=0;
    int hescore=0;
    float maxvelocity= 0.1f;
    bool poop=false;
bool uad =false;

//credits to lilmargin for this codepeice
bool collision(ISceneNode* one, ISceneNode* two) {
   aabbox3d<f32> b1, b2;

   b1 = one->getBoundingBox ();
   b2 = two->getBoundingBox ();

   one->getRelativeTransformation().transformBoxEx( b1 );
   two->getRelativeTransformation().transformBoxEx( b2 );
   return b1.intersectsWithBox( b2 );
}
int random(int HIGH, int LOW)
{
int resultfr;
time_t seconds;

time(&seconds);

srand((unsigned int) seconds);

resultfr = rand() % (HIGH - LOW + 1) + LOW;
float resulttt = resultfr*0.3;
return resulttt;
}
class MyEventReceiver : public IEventReceiver
{
public:
        // This is the one method that we have to implement
        virtual bool OnEvent(const SEvent& event)
        {
                // Remember whether each key is down or up
                if (event.EventType == irr::EET_KEY_INPUT_EVENT)
                        KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

                return false;
        }

        // This is used to check whether a key is being held down
        virtual bool IsKeyDown(EKEY_CODE keyCode) const
        {
                return KeyIsDown[keyCode];
        }

        MyEventReceiver()
        {
                for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
                        KeyIsDown[i] = false;
        }

private:
        // We use this array to store the current state of each key
        bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

    MyEventReceiver receiver;
void renderloop(){
        if (cube2)
        {
            vector3df ballps=ball->getPosition();
            vector3df cubeps=cube2->getPosition();
            vector3df maximumps = vector3df(0,23,0);
            vector3df minumumps = vector3df(0,-23,0);
             vector3df newps = vector3df(0,maxvelocity,0);
        if (ballps.Y>cubeps.Y)
            {
        cube2->setPosition(cubeps+newps);
            }
            else  if (ballps.Y<cubeps.Y)
            {
                 cube2->setPosition(cubeps-newps);
            }

         else   if (cubeps.Y>=maximumps.Y)
            {
        cube2->setPosition(vector3df(-39,23,0));
            }
            else if (cubeps.Y<=minumumps.Y)
            {
                cube2->setPosition(vector3df(-39,-23,0));
            }


        }
        if (poop==true)
        {
        ball->setPosition(ball->getPosition()+vector3df(0.5,0.2,0));
        }
        if (poop==false)
        {
       ball->setPosition(ball->getPosition()-vector3df(0.5,0.2,0));
        }
        if (uad==true)
        {
        ball->setPosition(ball->getPosition()+vector3df(0,0.5,0));
//        cout<<"uat = true"<<endl;
        }
        if (uad==false)
        {
        ball->setPosition(ball->getPosition()-vector3df(0,0.5,0));
//        cout<<"uat=false"<<endl;
        }

        if (collision(ball,cube))
        {
            poop=false;
        }
         if (collision(ball,cube2))
        {
            poop=true;
        }
         if (collision(ball,collisionhorisontal1))
        {
            uad=false;
            ball->setPosition(ball->getPosition()-vector3df(random(0,11),random(0,11),0));
//            cout<<"collision ariba"<<endl;
        }
         if (collision(ball,collisionhorisontal12))
        {
            uad=true;
            ball->setPosition(ball->getPosition()+vector3df(random(0,11),random(0,11),0));
//            cout<<"collision abou"<<endl;
        }
        if (collision(ball,collisionvertical2))
        {
            score+=1;
            ball->setPosition(vector3df(random(0,16),0,0));
//            cout<<"collision abou"<<endl;
        }
         if (collision(ball,collisionvertical))
        {
            hescore+=1;
            ball->setPosition(vector3df(random(0,16),0,0));
//            cout<<"collision abou"<<endl;
        }

               if(receiver.IsKeyDown(irr::KEY_KEY_W))
            {
                vector3df oldposy=cube->getPosition();
                vector3df newposy = vector3df(0,maxvelocity,0);
                vector3df posy = vector3df(0,23,0);
                if (oldposy.Y<=posy.Y)
                {
                cube->setPosition(oldposy+=newposy);
                }
            }
            if(receiver.IsKeyDown(irr::KEY_KEY_S))
            {
                vector3df oldposy=cube->getPosition();
                vector3df newposy = vector3df(0,0.1,0);
                  vector3df posy = vector3df(0,-23,0);
                if (oldposy.Y>=posy.Y)
                {
                cube->setPosition(oldposy-=newposy);
                }
            }

            if(receiver.IsKeyDown(irr::KEY_KEY_W)&& collision(ball,cube))
            {
                vector3df oldposy=cube->getPosition();
                vector3df newposy = vector3df(0,0.1,0);
                vector3df posy = vector3df(0,23,0);
                if (oldposy.Y<=posy.Y)
                {
                cube->setPosition(oldposy+=newposy);
                }
                 ball->setPosition(ball->getPosition()-vector3df(random(0,11),random(0,11),0));
            }
//            if(receiver.IsKeyDown(irr::KEY_KEY_S))
//            {
//                vector3df oldposy=cube->getPosition();
//                vector3df newposy = vector3df(0,0.1,0);
//                  vector3df posy = vector3df(0,-23,0);
//                if (oldposy.Y>=posy.Y)
//                {
//                cube->setPosition(oldposy-=newposy);
//                }
//            }
//

}

void main_loop(){

renderloop();
        driver->beginScene(true, true, SColor(0,200,200,200));

        smgr->drawAll();
        guienv->drawAll();

        driver->endScene();
                        core::stringw tmp;
                        tmp += "Your Score = ";
                        tmp += score;
                       // tmp += oldposy.Y;
                        tmp += " And ";
                        //tmp += oldposy.Z;
                        tmp+="Your oponement score is ";
                       // tmp+=oldroty.X;
                        tmp += hescore;
                       // tmp+=oldroty.Y;
                        tmp += " ";
                       // tmp+=oldroty.Z;



                        device->setWindowCaption(tmp.c_str());



}

int main(int argc, char** argv)
{


#ifdef __EMSCRIPTEN__
    device =
        createDevice(EDT_OGLES2, dimension2d<u32>(640, 480), 16,
            false, false, false, &receiver);


#else
	    device =
        createDevice(EDT_OPENGL, dimension2d<u32>(640, 480), 16,
            false, false, false, &receiver);

#endif
    device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");


    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();

        cube=smgr->addCubeSceneNode();
        cube2=smgr->addCubeSceneNode();
     if (cube)
     {
    cube->setScale(vector3df(0.5,1,0.1));
    cube->setPosition(vector3df(39,0,1));
     }
     if (cube2)
     {
        cube2->setScale(vector3df(0.5,1,0.1));
        cube2->setPosition(vector3df(-39,0,1));
     }

    ball=smgr->addSphereSceneNode();

    if (ball)
    {
        ball->setScale(vector3df(0.5,0.5,0.5));
    }

    collisionhorisontal1=smgr->addCubeSceneNode();
    collisionhorisontal12=smgr->addCubeSceneNode();

    if (collisionhorisontal1)
    {
        collisionhorisontal1->setScale(vector3df(10,0.5,0.1));
        collisionhorisontal1->setPosition(vector3df(0,38,0));
    }

    if (collisionhorisontal12)
    {
        collisionhorisontal12->setScale(vector3df(10,0.5,0.1));
        collisionhorisontal12->setPosition(vector3df(0,-38,0));
    }

    collisionvertical=smgr->addCubeSceneNode();
    collisionvertical2=smgr->addCubeSceneNode();

    if (collisionvertical)
    {
        collisionvertical->setScale(vector3df(0.5,10,0.1));
        collisionvertical->setPosition(vector3df(49,0,1));
    }

     if (collisionvertical2)
    {
        collisionvertical2->setScale(vector3df(0.5,10,0.1));
        collisionvertical2->setPosition(vector3df(-49,0,1));
    }

    ICameraSceneNode *cam=smgr->addCameraSceneNode(0, vector3df(0,0,50), vector3df(0,0,0));


#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(main_loop,0,1);
#else
    while(device->run())
    {
renderloop();
        driver->beginScene(true, true, SColor(0,200,200,200));

        smgr->drawAll();
        guienv->drawAll();

        driver->endScene();
                        core::stringw tmp;
                        tmp += "Your Score = ";
                        tmp += score;
                       // tmp += oldposy.Y;
                        tmp += " And ";
                        //tmp += oldposy.Z;
                        tmp+="Your oponement score is ";
                       // tmp+=oldroty.X;
                        tmp += hescore;
                       // tmp+=oldroty.Y;
                        tmp += " ";
                       // tmp+=oldroty.Z;



                        device->setWindowCaption(tmp.c_str());

    }
#endif // __EMSCRIPTEN__

    device->drop();

    return 0;
}

