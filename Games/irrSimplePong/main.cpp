
#include "irrlicht.h"
#include <stdio.h>
#include <cmath>

///NOTE HEIGHT AND WIDTH SHOULD BE EVEN NUMBERS, OR PADDLES WONT REACH THE END OF MOVEMENT AREA.
#define ZDISTANCE 60
#define WIDTH 80
#define HEIGHT 60

///This handles keyboard input
class MyEventReceiver : public irr::IEventReceiver
{
public:
    // This is the one method that we have to implement
    virtual bool OnEvent(const irr::SEvent& event)
    {
        // Remember whether each key is down or up
        if(event.EventType == irr::EET_KEY_INPUT_EVENT)
        {
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
        }

        return false;
    }

    // This is used to check whether a key is being held down
    virtual bool isKeyDown(irr::EKEY_CODE keyCode) const
    {
        return KeyIsDown[keyCode];
    }

    MyEventReceiver()
    {
        for (irr::u32 i=0; i<irr::KEY_KEY_CODES_COUNT; ++i)
        {
            KeyIsDown[i] = false;
        }

    }

private:
    // We use this array to store the current state of each key
    bool KeyIsDown[irr::KEY_KEY_CODES_COUNT];

};

void moveBall(irr::scene::ISceneNode * ballnode, irr::scene::ISceneNode * paddle1, irr::scene::ISceneNode * paddle2, irr::core::vector2df & vel, float height, float width, int & score1, int & score2);
void movePaddleUp(irr::scene::ISceneNode * paddle, float height);
void movePaddleDown(irr::scene::ISceneNode * paddle, float height);

int main()
{
    irr::IrrlichtDevice * dvc=irr::createDevice(irr::video::EDT_OPENGL,irr::core::dimension2di(640,480),32,false,true,false,0);
    irr::video::IVideoDriver * driver = dvc->getVideoDriver();
    irr::scene::ISceneManager * smgr = dvc->getSceneManager();

    /// Create and set our input receiver
    MyEventReceiver rcv;
    dvc->setEventReceiver(&rcv);

    ///Set up font to draw scores with
    irr::gui::IGUIFont* font = dvc->getGUIEnvironment()->getBuiltInFont();

    ///camera to see the scene
    smgr->addCameraSceneNode();

    ///ADD boundaries
    irr::scene::ISceneNode * boundarie;

    ///Left side
    boundarie = smgr->addCubeSceneNode(1,0,-1,irr::core::vector3df(-1*WIDTH/2,0,ZDISTANCE));
    boundarie->setScale(irr::core::vector3df(1,HEIGHT,1));
    boundarie->setMaterialFlag(irr::video::EMF_LIGHTING,false);

    ///Right side
    boundarie = smgr->addCubeSceneNode(1,0,-1,irr::core::vector3df(WIDTH/2,0,ZDISTANCE));
    boundarie->setScale(irr::core::vector3df(1,HEIGHT,1));
    boundarie->setMaterialFlag(irr::video::EMF_LIGHTING,false);

    ///Top
    boundarie = smgr->addCubeSceneNode(1,0,-1,irr::core::vector3df(0,(HEIGHT/2)+0.5f,ZDISTANCE));
    boundarie->setScale(irr::core::vector3df(WIDTH+1,1,1));
    boundarie->setMaterialFlag(irr::video::EMF_LIGHTING,false);

    ///Bottom
    boundarie = smgr->addCubeSceneNode(1,0,-1,irr::core::vector3df(0,(-1*HEIGHT/2)-0.5f,ZDISTANCE));
    boundarie->setScale(irr::core::vector3df(WIDTH+1,1,1));
    boundarie->setMaterialFlag(irr::video::EMF_LIGHTING,false);


    ///Add paddles
    irr::scene::ISceneNode * lpaddle;
    irr::scene::ISceneNode * rpaddle;

    lpaddle = smgr->addCubeSceneNode(1,0,-1,irr::core::vector3df(WIDTH/2*-1+2,0,ZDISTANCE));
    lpaddle->setScale(irr::core::vector3df(1,8,1));
    lpaddle->setMaterialFlag(irr::video::EMF_LIGHTING,false);

    rpaddle = smgr->addCubeSceneNode(1,0,-1,irr::core::vector3df(WIDTH/2-2,0,ZDISTANCE));
    rpaddle->setScale(irr::core::vector3df(1,8,1));
    rpaddle->setMaterialFlag(irr::video::EMF_LIGHTING,false);

    ///Add the ball of steel
    irr::scene::ISceneNode * ball;
    ball = smgr->addCubeSceneNode(1,0,-1,irr::core::vector3df(0,0,ZDISTANCE));
    ball->setMaterialFlag(irr::video::EMF_LIGHTING,false);


    ///ball velocity
    irr::core::vector2df vel(0.3,-0.1);

    ///timer
    unsigned lnext_move=0;
    unsigned rnext_move=0;
    irr::ITimer * timer = dvc->getTimer();

    ///scores
    int lscore, rscore;
    lscore=rscore=0;

    while(dvc->run())
    {
        if(rcv.isKeyDown(irr::KEY_KEY_W))
        {
            if(timer->getTime()>=lnext_move)
            {
                ///MOVE UP AND INCREASE NEXT MOVE TIME
                lnext_move=timer->getTime()+50;///half a second
                movePaddleUp(lpaddle,HEIGHT);
            }
        }
        else if(rcv.isKeyDown(irr::KEY_KEY_S))
        {
            if(timer->getTime()>=lnext_move)
            {
                ///MOVE UP AND INCREASE NEXT MOVE TIME
                lnext_move=timer->getTime()+50;///half a second
                movePaddleDown(lpaddle,HEIGHT);
            }
        }
        else
        {
            lnext_move=0;
        }

        if(rcv.isKeyDown(irr::KEY_UP))
        {
            if(timer->getTime()>=rnext_move)
            {
                ///MOVE UP AND INCREASE NEXT MOVE TIME
                rnext_move=timer->getTime()+50;///half a second
                movePaddleUp(rpaddle,HEIGHT);
            }
        }

        else if(rcv.isKeyDown(irr::KEY_DOWN))
        {
            if(timer->getTime()>=rnext_move)
            {
                ///MOVE UP AND INCREASE NEXT MOVE TIME
                rnext_move=timer->getTime()+50;///half a second
                movePaddleDown(rpaddle,HEIGHT);
            }
        }
        else
        {
            rnext_move=0;
        }

        moveBall(ball, lpaddle, rpaddle, vel, HEIGHT, WIDTH, lscore, rscore);


        driver->beginScene(true, true, irr::video::SColor(255,0,0,0));

        ///draw scores
    //    font->draw(irr::core::stringw(lscore),
     //                                   irr::core::rect<irr::s32>(10,5,30,25),
     //                                   irr::video::SColor(255,255,255,255));

     //   font->draw(irr::core::stringw(rscore),
     //                                   irr::core::rect<irr::s32>(625,5,640,25),
     //                                   irr::video::SColor(255,255,255,255));



        smgr->drawAll();
        driver->endScene();
        dvc->sleep(10);
    }

    return 0;
}

void moveBall(irr::scene::ISceneNode * ballnode, irr::scene::ISceneNode * paddle1, irr::scene::ISceneNode * paddle2, irr::core::vector2df & vel, float height, float width, int & score1, int & score2)
{
    ///Reset ball and add points to player
    if(ballnode->getAbsolutePosition().X+vel.X>=width/2)
    {
        vel.X*=-1;
        ballnode->setPosition(irr::core::vector3df(0,0,ZDISTANCE));
        ballnode->updateAbsolutePosition();
        score1+=1;
    }

    ///Reset ball and add points to player
    if(ballnode->getAbsolutePosition().X+vel.X<=width/2*-1)
    {
        vel.X*=-1;
        ballnode->setPosition(irr::core::vector3df(0,0,ZDISTANCE));
        ballnode->updateAbsolutePosition();
        score2+=1;
    }

    ///Invert velocity if ball collides with paddle
    if(ballnode->getTransformedBoundingBox().intersectsWithBox(paddle1->getTransformedBoundingBox())
            ||ballnode->getTransformedBoundingBox().intersectsWithBox(paddle2->getTransformedBoundingBox()))
    {
        vel.X*=-1;
    }

    ///Invert if collides with boundaries
    if(abs(ballnode->getAbsolutePosition().Y+vel.Y)>=(height/2))
        vel.Y*=-1;

    ///Move the ball
    ballnode->setPosition(ballnode->getAbsolutePosition()+irr::core::vector3df(vel.X, vel.Y,0));
}

void movePaddleUp(irr::scene::ISceneNode * paddle, float height)
{
    if(paddle->getAbsolutePosition().Y+(paddle->getScale().Y/2) +1 <= height/2)
        paddle->setPosition(paddle->getAbsolutePosition()+irr::core::vector3df(0,1,0));
}

void movePaddleDown(irr::scene::ISceneNode * paddle, float height)
{
    if(paddle->getAbsolutePosition().Y-(paddle->getScale().Y/2)-1 >= (height/2)*-1)
        paddle->setPosition(paddle->getAbsolutePosition()+irr::core::vector3df(0,-1,0));
}

