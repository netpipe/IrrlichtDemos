
//! simple pong game by psychophoniac. Created with code::blocks, using IrrlichtEngine for Rendering.


#include <irrlicht.h>

//COMMENT THIS TO PLAY WITH 2 HUMAN PLAYERS, UNCOMMEND TO PLAY AGAINST BOT.
#define USE_NPC


using namespace irr;

class MyEventReceiver : public IEventReceiver  // token from the examples
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

enum STATE
{
    S_NEWROUND = 0,
    S_PLAY,
    S_PAUSE,
    S_RESUME
};

enum PLAYER
{
    PLAYER_ONE = 0,
    PLAYER_TWO,
    PLAYER_COUNT = 2
};

int main(void)
{
    MyEventReceiver* evr = new MyEventReceiver();

    IrrlichtDevice *device =
        createDevice(video::EDT_OPENGL, core::dimension2du(1024 , 768), 32,
                     false, false, false, evr);

    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();
    gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

    scene::IMesh* mesh = smgr->getMesh("media/field_m.obj");
    mesh->setHardwareMappingHint(scene::EHM_STATIC);
    //smgr->getMeshManipulator()->makePlanarTextureMapping(mesh, 0.003f);

    core::aabbox3df fieldBox;

    scene::IMeshSceneNode* fieldM = smgr->addMeshSceneNode(mesh);
    fieldM->setPosition(core::vector3df(0.f, 0.f, 0.f));
    fieldM->setScale(core::vector3df(3, 0, 3));
    video::ITexture* fNormalTex = driver->getTexture("media/rockwall_height.bmp");
    video::ITexture* fDifTex = driver->getTexture("media/rockwall.bmp");
    driver->makeNormalMapTexture(fNormalTex);
    fieldM->setMaterialTexture(1, fNormalTex);
    fieldM->setMaterialTexture(0, fDifTex);
    //fieldM->setMaterialType(video::EMT_PARALLAX_MAP_SOLID);
    fieldBox.addInternalBox(fieldM->getTransformedBoundingBox());

    mesh = smgr->getMesh("media/field_s.obj");
    mesh->setHardwareMappingHint(scene::EHM_STATIC);
    //smgr->getMeshManipulator()->makePlanarTextureMapping(mesh, 0.005f);

    scene::IMeshSceneNode* fieldS[PLAYER_COUNT];
    for (u32 i = 0; i < 2; i++)
    {
        fieldS[i] = smgr->addMeshSceneNode(mesh);
        fieldS[i]->setPosition(core::vector3df( i == 0 ? 0 : - 480*3  ,0,0));
        fieldS[i]->setMaterialTexture(0, fDifTex);
        fieldS[i]->setScale(core::vector3df(3, 2, 3));
        fieldS[i]->setMaterialTexture(1, fNormalTex);
        fieldBox.addInternalBox(fieldS[i]->getTransformedBoundingBox());
    };
    fieldBox.addInternalPoint(core::vector3df(-200, 0, -300) * 4 + fieldBox.MinEdge);
    fieldBox.addInternalPoint(core::vector3df(200, 20, 300)* 4 + fieldBox.MaxEdge);

    mesh = smgr->getMesh("media/bar.obj");
    mesh->setHardwareMappingHint(scene::EHM_STATIC);
    scene::IMeshSceneNode* bar[PLAYER_COUNT];
    for (u32 i = 0; i < 2; i++)
    {
        bar[i] = smgr->addMeshSceneNode(mesh);
        bar[i]->setScale(core::vector3df(1.f, 0.5f, 0.5f));
        bar[i]->setPosition(core::vector3df( 0 , 30 , i == 0 ? 480 : -480) * 3);
        bar[i]->setMaterialTexture(0, driver->getTexture(i == 0 ? "media/greenTex.png" : "media/blueTex.png"));
        bar[i]->setMaterialFlag(video::EMF_LIGHTING, false);
    };

//    smgr->addLightSceneNode(bar[0],core::vector3df(0,50,0),video::SColorf(0.0f, 0.0f, 1.0f) , 100.0f );
//    smgr->addLightSceneNode(bar[1],core::vector3df(0,50,0),video::SColorf(1.0f, 0.0f, 0.0f) , 100.0f );


    scene::ISceneNode* ball = smgr->addSphereSceneNode(25.0f, 64);
    ball->setPosition(core::vector3df(0,30,0));
    ball->setScale(core::vector3df(0.75f, 0.75f, 0.75f));
    ball->setMaterialTexture(0, driver->getTexture("media/ballTex.png"));
    ball->setMaterialFlag(video::EMF_LIGHTING, false);


    gui::IGUIStaticText* scoreText = guienv->addStaticText(L"" , core::rect<s32>(20, 20, 1000, 50),
		false, false, 0, -1,false);
    scoreText->setOverrideFont(guienv->getFont("media/FONT.xml"));
    scoreText->setOverrideColor(video::SColor(0xffff0000));


    //scene::ICameraSceneNode* cam; cam = smgr->addCameraSceneNode(0, core::vector3df(-500, 600, 0), fieldM->getTransformedBoundingBox().getCenter());
    scene::ICameraSceneNode* cam = smgr->addCameraSceneNode();
    cam->setPosition(core::vector3df(-1200 , 1600, 0));
    cam->setTarget(fieldM->getTransformedBoundingBox().getCenter());

    u32 lastFPS = 999999, lastTRI = 999999, lastTime = 0, deltaTime = 0, aktTime = device->getTimer()->getTime(), npc_shootTime = 0;
    f32 barSpeed[PLAYER_COUNT] = { 0.75f, 0.75f }, ballSpeed = 1.5f;

    u32 gameState = S_NEWROUND;
    u32 score[PLAYER_COUNT] = {0,0};
    //srand((u32)device->getTimer()->getTime());
    u32 lastScore = rand() % 2;

    core::vector3df ballRot(0,0,0);

    smgr->setAmbientLight(video::SColor(255, 255, 255, 255));

    while (device->run() && !evr->IsKeyDown( KEY_ESCAPE ))
    {
        if(!device->isWindowActive() || !device->isWindowFocused() || device->isWindowMinimized())
            {
                gameState = S_PAUSE;
            }
        else
            if(gameState == S_PAUSE)
                gameState = S_RESUME;

        aktTime = device->getTimer()->getTime();
        deltaTime = aktTime - lastTime;

        if(gameState == S_PLAY || gameState == S_NEWROUND) // move bars
         {
            if (evr->IsKeyDown(KEY_KEY_W))
            {
                core::vector3df dest(bar[0]->getPosition() + core::vector3df( barSpeed[0] * deltaTime, 0, 0));
                if (!fieldS[0]->getTransformedBoundingBox().intersectsWithBox(bar[0]->getTransformedBoundingBox()))
                    bar[0]->setPosition(dest);
            }
            else if (evr->IsKeyDown(KEY_KEY_S))
            {
                core::vector3df dest(bar[0]->getPosition() + core::vector3df( -barSpeed[0] * deltaTime, 0, 0));
                if (!fieldS[1]->getTransformedBoundingBox().intersectsWithBox(bar[0]->getTransformedBoundingBox()))
                    bar[0]->setPosition(dest);
            };
#ifndef USE_NPC
            if (evr->IsKeyDown(KEY_KEY_I))
            {
                core::vector3df dest(bar[1]->getPosition() + core::vector3df( barSpeed[1] * deltaTime, 0, 0));
                if (!fieldS[0]->getTransformedBoundingBox().intersectsWithBox(bar[1]->getTransformedBoundingBox()))
                    bar[1]->setPosition(dest);
            }
            else if (evr->IsKeyDown(KEY_KEY_K))
            {
                core::vector3df dest(bar[1]->getPosition() + core::vector3df( -barSpeed[1] * deltaTime, 0, 0));
                if (!fieldS[1]->getTransformedBoundingBox().intersectsWithBox(bar[1]->getTransformedBoundingBox()))
                    bar[1]->setPosition(dest);
            };
#endif
#ifdef USE_NPC
    bool towardsNPC = ballRot.Z < 0;
    bool above = ball->getAbsolutePosition().X > bar[1]->getAbsolutePosition().X;

    if (towardsNPC)
    {
        if (above)
        {
            core::vector3df dest(bar[1]->getPosition() + core::vector3df( barSpeed[1] * deltaTime, 0, 0));
            if (!fieldS[0]->getTransformedBoundingBox().intersectsWithBox(bar[1]->getTransformedBoundingBox()))
                bar[1]->setPosition(dest);
        }
        else
        {
            core::vector3df dest(bar[1]->getPosition() + core::vector3df( -barSpeed[1] * deltaTime, 0, 0));
            if (!fieldS[1]->getTransformedBoundingBox().intersectsWithBox(bar[1]->getTransformedBoundingBox()))
                bar[1]->setPosition(dest);
        };
    }
    else
    {
        if (bar[1]->getAbsolutePosition().X != 0)
            if (bar[1]->getAbsolutePosition().X < 0)
            {
                core::vector3df dest(bar[1]->getPosition() + core::vector3df( barSpeed[1] * deltaTime, 0, 0));
                if (!fieldS[0]->getTransformedBoundingBox().intersectsWithBox(bar[1]->getTransformedBoundingBox()))
                    bar[1]->setPosition(dest);
            }
            else
            {
                core::vector3df dest(bar[1]->getPosition() + core::vector3df( -barSpeed[1] * deltaTime, 0, 0));
                if (!fieldS[1]->getTransformedBoundingBox().intersectsWithBox(bar[1]->getTransformedBoundingBox()))
                    bar[1]->setPosition(dest);
            };
    };
#endif
         };

        switch (gameState)
        {
         case S_PLAY :
         {
            core::vector3df bPos = ball->getAbsolutePosition(), bRot = ballRot;
            bRot.normalize();

            if(!fieldBox.isPointInside(bPos)) // someone Scored
            {
                if(bPos.Z > 0)
                    {
                        score[PLAYER_TWO]++;
                        lastScore = PLAYER_TWO;
                        #ifdef USE_NPC
                            npc_shootTime = device->getTimer()->getTime() + 3 * 1000;
                        #endif
                    }
                else
                    {
                        score[PLAYER_ONE]++;
                        lastScore = PLAYER_ONE;
                    };

                gameState = S_NEWROUND;
                break;
            };

            bool collision = false;
            for(u32 i = 0; i < 2 ; i++)
                if(fieldS[i]->getTransformedBoundingBox().intersectsWithBox(ball->getTransformedBoundingBox()))
                    {
                        bRot.X *= -10;
                        core::vector3df nRot = ballRot;
                        nRot.X *= -1;
                        ballRot = nRot ;
                        collision = true;
                        break;
                    };

            for(u32 i = 0; i < 2 ; i++)
                if(bar[i]->getTransformedBoundingBox().intersectsWithBox(ball->getTransformedBoundingBox()))
                    {
//                        bRot.Z *= -1;
//                        core::vector3df nRot = ball->getRotation();
//                        nRot.Z *= -1;
//                        ball->setRotation(nRot);

                        bRot = -ball->getAbsolutePosition() + bar[i]->getAbsolutePosition();
                        bRot -= ballRot - core::vector3df(0,0, i == 0 ? 50 : -50);
                        bRot.normalize();
                        bRot *= -1;
                        ballRot = bRot;
                        collision = true;
                        break;
                    };

            if(collision)
                {
                    ball->setRotation(ballRot);
                    ball->removeAnimators();
                    core::vector3df anRot = ballRot;
                    anRot.rotateXZBy(270.f, core::vector3df(0,0,0));
                    scene::ISceneNodeAnimator* anim = smgr->createRotationAnimator(anRot * (11 *ballSpeed) );
                    ball->addAnimator(anim);
                    anim->drop();
                };


            bPos += (bRot * deltaTime * ballSpeed);
            ball->setPosition(bPos);
         }; break;

         case S_NEWROUND :
         {
            ball->removeAnimators();

            core::vector3df nPos = lastScore == PLAYER_ONE ? bar[PLAYER_ONE]->getAbsolutePosition() : bar[PLAYER_TWO]->getAbsolutePosition();
            nPos = bar[lastScore]->getTransformedBoundingBox().getCenter() + core::vector3df(0, 0, 25.f) * (lastScore == PLAYER_ONE ? -1.25f : 1.25f);
            ball->setPosition(nPos);
#ifndef USE_NPC
            if(lastScore == PLAYER_ONE ?  evr->IsKeyDown(KEY_KEY_D) : evr->IsKeyDown(KEY_KEY_L))
            {
             ballRot = (core::vector3df(0, 0, lastScore == PLAYER_ONE ? - 1 : 1));
             core::vector3df anRot = ballRot;
             anRot.rotateXZBy(270.f, core::vector3df(0,0,0));
             scene::ISceneNodeAnimator* anim = smgr->createRotationAnimator(anRot * (6 *ballSpeed));
             ball->addAnimator(anim);
             anim->drop();

             gameState = S_PLAY;
            };
#else
            if(npc_shootTime < device->getTimer()->getTime())
            {
             npc_shootTime = 0;
             ballRot = (core::vector3df(0, 0, lastScore == PLAYER_ONE ? - 1 : 1));
             core::vector3df anRot = ballRot;
             anRot.rotateXZBy(270.f, core::vector3df(0,0,0));
             scene::ISceneNodeAnimator* anim = smgr->createRotationAnimator(anRot * (6 *ballSpeed));
             ball->addAnimator(anim);
             anim->drop();

             gameState = S_PLAY;
            };

#endif
         }; break;

         case S_PAUSE:
         {
             //device->getTimer()->stop();
             device->yield();
         };break;

         case S_RESUME:
         {
             //device->getTimer()->start();
             gameState = S_PLAY;
         };break;

        default:
            return -1;
        };

        driver->beginScene(true, true, video::SColor(0,0,0,0));

            smgr->drawAll();
            guienv->drawAll();

            driver->setTransform(video::ETS_WORLD, core::matrix4());
            driver->draw3DBox(fieldBox, video::SColor(0, 0, 255, 0));

        driver->endScene();

        u32 FPS = driver->getFPS(), TRI = driver->getPrimitiveCountDrawn();
        if (lastFPS != FPS || lastTRI != TRI)
        {
            core::stringw str = "SCORE (";
                str += score[PLAYER_ONE];
                str += " : ";
                str += score[PLAYER_TWO];
                str += ")";
                str += " :: FPS : ";
                str += FPS;
                str += " :: Primitives: ";
                str += TRI;
            //device->setWindowCaption(str.c_str());

                scoreText->setText(str.c_str());

            lastFPS = FPS;
            lastTRI = TRI;
        };
        lastTime = device->getTimer()->getTime();
    }
    device->drop();

    return 0;
}

