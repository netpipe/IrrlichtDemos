
#include "Game.h"
#include "OS.h"

Game::Game(IrrlichtDevice *device)
{
    this->device = device;
//    this->sengine = sengine;
    smgr = device->getSceneManager();
    driver = device->getVideoDriver();
    guienv = device->getGUIEnvironment();

    //guienv->getSkin()->setColor(gui::EGDC_3D_FACE, video::SColor(255, 0,0,0));
    //guienv->getSkin()->setSize(gui::EGDS_TEXT_DISTANCE_X, 20);

    wallsSceneNode = smgr->addAnimatedMeshSceneNode(smgr->getMesh("walls.b3d"));
    wallsSceneNode->setRotation(core::vector3df(-90,0,0));

    wallRTT1 = NULL;
    wallRTT2 = NULL;
    groundRTT1 = NULL;
    groundRTT2 = NULL;

    // render target textures
    initRTTs();

	wallsSceneNode->setMaterialTexture(1, driver->getTexture("wall3.jpg"));
	wallsSceneNode->setMaterialType(video::EMT_DETAIL_MAP);

//    sMusic = NULL;

    tm = NULL;
    showFirstScreen();

    abm = new abm::AttributeBarManager(device);

    device->setEventReceiver(this);
}

Game::~Game()
{
    if (tm) delete tm;
    delete abm;

    wallRTT1->drop();
    wallRTT2->drop();
    groundRTT1->drop();
    groundRTT2->drop();
}

void Game::initRTTs()
{
    // Render Target Textures for splattering

    if (wallRTT1) wallRTT1->drop();
    if (wallRTT2) wallRTT2->drop();
    if (groundRTT1) groundRTT1->drop();
    if (groundRTT2) groundRTT2->drop();

    // CASTLE WALL

    wallRTT1 = driver->addRenderTargetTexture(core::dimension2d<u32>(512,512));
    wallRTT2 = driver->addRenderTargetTexture(core::dimension2d<u32>(512,512));
    //driver->beginScene(true, true, video::SColor(0,0,0,0));
    driver->setRenderTarget(wallRTT1, true,true);
    // render original texture to RTT1
    driver->draw2DImage(driver->getTexture("wall.jpg"), core::position2d<s32>(0,0));
    driver->setRenderTarget(0);
    //driver->endScene();
    // set wall texture to RTT
 ///   wallsSceneNode->getMaterial(0).Textures[0] = wallRTT1;

    // GROUND (GRASS)

    groundRTT1 = driver->addRenderTargetTexture(core::dimension2d<u32>(512,512));
    groundRTT2 = driver->addRenderTargetTexture(core::dimension2d<u32>(512,512));
    //driver->beginScene(true, true, video::SColor(0,0,0,0));
    driver->setRenderTarget(groundRTT1, true,true);
    // render original texture to RTT1
    driver->draw2DImage(driver->getTexture("grass.jpg"), core::position2d<s32>(0,0));
    driver->setRenderTarget(0);
    //driver->endScene();
    // set wall texture to RTT
 ///   wallsSceneNode->getMaterial(1).Textures[0] = groundRTT1;
}

void Game::showFirstScreen()
{
    started = false;

    if (tm)
    {
        delete tm;
        tm = NULL;
    }

    stopMusic();
    for (int i = 0; i < endTexts.size(); i ++)
    {
        endTexts[i]->remove();
    }
    endTexts.clear();

    int hw = driver->getScreenSize().Width / 2;
    int hh = driver->getScreenSize().Height / 2;

    video::ITexture *tex = driver->getTexture("media/intro.png");
    core::dimension2d<u32> dim = tex->getOriginalSize();
    startImg = guienv->addImage(
        tex,
        core::position2d<s32>(hw-dim.Width/2, hh-dim.Height/2),
        true
        );
}

void Game::hideFirstScreen()
{
    started = true;
    startImg->remove();
}

void Game::initNew()
{
    tm = new TargetManager(device);
    scoreText = NULL;
    score = 0;
    shellCount = maxShells;
    gunFlash = NULL;
    finished = false;
    setMusic("sfx/funtime.ogg");

    percentFull = 0;
    abm->add(driver->getTexture("bar.png"), &percentFull, abm::LEFT, abm::BOTTOM);

    initRTTs();
}

bool Game::run()
{
    scene::ISceneManager *smgr = device->getSceneManager();
    video::IVideoDriver *driver = device->getVideoDriver();
    gui::IGUIEnvironment *guienv = device->getGUIEnvironment();
    ITimer *timer = device->getTimer();

    long int startTime = timer->getTime();
    double dt;

    int lastFPS = -1;

    double gameTime = 0;

    while(device->run())
    {
        if (device->isWindowActive())
        {
            if (timer->isStopped())
            {
                timer->start();
            }
            if ( (timer->getTime() - startTime >= 1000.0 / 60.0) && device->isWindowActive())
            {
                dt = (double)(timer->getTime() - startTime) / 1000.0f;

                if (finished || !started)
                {
                    // drawing
                    driver->beginScene(true, true, video::SColor(0,180,197,255));
                    smgr->drawAll();
                    guienv->drawAll();
                    driver->endScene();
                }
                else
                {
                    score += tm->getScoreIncrements();

                    // reduce shells by number of targets (probably only People)
                    // which have terminated themselves without being hit
                    // called before TargetManager update() as that *may* remove the target
                    //shellCount -= tm->takeUnhitCount();
                    //percentFull += tm->takeUnhitCount()*10;

                    percentFull += tm->takeFullnessIncreases();
                    if (percentFull > 100) percentFull = 100;
                    if (percentFull < 0) percentFull = 0;

                    int shellsGained = tm->takeShellsGained();

                    if (shellsGained > 0)
                    {
                        shellCount += shellsGained;
                        if (shellCount >= maxShells)
                        {
                            shellCount = maxShells;
                            // sound for Full Shells
//                            sengine->play2D("sfx/chaching.wav");
                        }
                        else
                        {
                            // sound for gained a few shells
             //               sengine->play2D("sfx/shells.wav");
                        }
                    }

                    // update HUD (score, shell display...)
                    removeHUD();
                    createHUD();

                    // update targets
                    // dt not increased when finished==true, so no new targets created then
                    gameTime += dt;
                }

                // occurs whether finished or not
                if (started) tm->update(gameTime, dt);

                // continue with code for Not Finished...
                if (!finished && started)
                {
                    // start drawing
                    driver->beginScene(true, true, video::SColor(0,180,197,255));

                    // add blood splatters :(
                    std::vector <Splatter> splatters = tm->getNewSplatters();
                    for (int i = 0; i < splatters.size(); i ++)
                    {
                        bloodSplat(splatters[i].pos, splatters[i].texture);
                    }
                    // reset render target
                    driver->setRenderTarget(0, true, true, video::SColor(0,180,197,255));
                    // draw rest
                    smgr->drawAll();
                    guienv->drawAll();
                    // attribute bar
                    abm->render(dt);
                    // crosshair
                    video::ITexture *crosshairTexture = driver->getTexture("crosshair.tga");
                    core::dimension2d<u32> crosshairDimensions = crosshairTexture->getSize();
                    core::position2d<s32> pos = device->getCursorControl()->getPosition();
                    pos.X -= crosshairDimensions.Width / 2;
                    pos.Y -= crosshairDimensions.Height / 2;
                    driver->draw2DImage(crosshairTexture, pos,
                        core::rect<s32>(0,0,crosshairDimensions.Width,crosshairDimensions.Height),
                        NULL,
                        video::SColor(255,255,255,255),
                        true
                        );
                    // end drawing
                    if (!driver->endScene()) {
                    //device->closeDevice();
                    //printf("END sCENE FAILED\n");
                    }

                    removeGunFlash();

                    if ( (percentFull >= 100 && abm->notChanging(0)))
                    {
                        // GAME OVER STUFF
                        finished = true;
                        finishTime = timer->getTime();
                        removeHUD();
                        stopMusic();
                        abm->clear();

                        // scoring
                        int oldScore = loadScore();
                        bool highScore = score > oldScore;
                        if (highScore)
                        {
                            saveScore();
                        }

                        core::stringw str;
                        // New High Score / Game Over, you scored
                        // 10202
                        // Beautiful/Delightful/Shiny/Marvellous/Delectable points
                        // you beat/didn't beat the previous score...
                        int x = driver->getScreenSize().Width/2;
                        int y = driver->getScreenSize().Height/2-50;
                        gui::IGUIFont *font = guienv->getFont("scorefont.xml");
                        core::dimension2d<s32> dim;
                        core::rect<s32> rec;
                        gui::IGUIStaticText *text;
                        const int gap = 70;

                        video::ITexture *tex = driver->getTexture("splatscreen.png");
                        dim = tex->getOriginalSize();
                        endTexts.push_back (guienv->addImage(
                            tex,
                            core::position2d<s32>(x-dim.Width/2, y-dim.Height/2+30),
                            true
                            ));

                        // Game Over TEXTS
                        str = highScore ? L"New High Score!!" : L"Game Over!! You scored";
                        dim = font->getDimension(str.c_str())/2;
                        rec = core::rect<s32>(x-dim.Width,y-dim.Height-gap, x+dim.Width,y+dim.Height-gap);
                        text = guienv->addStaticText(str.c_str(), rec, false,false);//, 0,-1,true);
                        text->setOverrideFont(font);
                        text->setOverrideColor(video::SColor(255,255,100,100));
                        endTexts.push_back(text);

                       // str = score;
                        dim = font->getDimension(str.c_str())/2;
                        rec = core::rect<s32>(x-dim.Width,y-dim.Height, x+dim.Width,y+dim.Height);
                        text = guienv->addStaticText(str.c_str(), rec, false,false);
                        text->setOverrideFont(font);
                        text->setOverrideColor(video::SColor(255,255,100,100));
                        endTexts.push_back(text);

                        const char *words[5] =
                        {
                            "Beautiful",
                            "Delightful",
                            "Shiny",
                            "Marvellous",
                            "Delectable"
                        };
                        str = words[rand()%5];
                        str += " points";
                        dim = font->getDimension(str.c_str())/2;
                        rec = core::rect<s32>(x-dim.Width,y-dim.Height+gap, x+dim.Width,y+dim.Height+gap);
                        text = guienv->addStaticText(str.c_str(), rec, false,false);
                        text->setOverrideFont(font);
                        text->setOverrideColor(video::SColor(255,255,100,100));
                        endTexts.push_back(text);

                        if (highScore)
                        {
                            str = L"you beat the previous score of ";
                            str += oldScore;
                            str += " ";
                            str += words[rand()%5];
                            str += " points :)";
                        }
                        else
                        {
                            str = L"but you didn't beat the high score of ";
                            str += oldScore;
                            str += " ";
                            str += words[rand()%5];
                            str += " points :(";
                        }
                        font = guienv->getFont("smallfont.xml");
                        dim = font->getDimension(str.c_str())/2;
                        rec = core::rect<s32>(x-dim.Width,y-dim.Height+gap*2, x+dim.Width,y+dim.Height+gap*2);
                        text = guienv->addStaticText(str.c_str(), rec, false,false);
                        text->setOverrideFont(font);
                        text->setOverrideColor(video::SColor(255,255,100,100));
                        endTexts.push_back(text);
                    }
                } // END if finished

                startTime = timer->getTime();
            }
            device->sleep(20);

            int fps = driver->getFPS();

            if (lastFPS != fps)
            {
                printf("FPS: %i\n", fps);
                lastFPS = fps;
            }
        }
        else
        {
            device->closeDevice();
            if (!timer->isStopped())
            {
                timer->stop();
            }
        }
    }

    return false;
}

void Game::removeHUD()
{
    for (int i = 0; i < shellImages.size(); i ++)
    {
        shellImages[i]->remove();
    }
    shellImages.clear();
    if (scoreText)
    {
        scoreText->remove();
        scoreText = NULL;
    }
}

/*
 * Assumes HUD is either not present or has just been removed with removeHUD()
 */
void Game::createHUD()
{
    // score text
    core::stringw str = L"SCORE ";
    str += score;
    int x = 40, y = 20;
    gui::IGUIFont *font = guienv->getFont("scorefont.xml");
    core::dimension2d<u32> dim = font->getDimension(str.c_str());
    core::rect<s32> rec = core::rect<s32>(x,y, x+dim.Width,y+dim.Height);
    scoreText = guienv->addStaticText(str.c_str(), rec, false,false);
    scoreText->setOverrideFont(font);
    scoreText->setOverrideColor(video::SColor(255,255,100,100));

    /*
     * Shell display
     */
    video::ITexture *tex = driver->getTexture("shell.png");
    video::ITexture *tex2 = driver->getTexture("shell2.png");

    const int margin = 20;
    int tx = tex->getOriginalSize().Width;
    int ty = tex->getOriginalSize().Height;

    x = driver->getScreenSize().Width - margin;
    y = driver->getScreenSize().Height - ty - margin;

    for (int i = 0; i < maxShells; i ++)
    {
        x -= tx;
        shellImages.push_back(
            guienv->addImage(
            i < shellCount ? tex : tex2,
            core::position2d<s32>(x,y),
            true)
        );
    }
}

bool Game::OnEvent(const SEvent &e)
{
    bool canRestart = device->getTimer()->getTime()-finishTime > 3000;

    if (e.EventType == EET_KEY_INPUT_EVENT)
    {
        if (e.KeyInput.PressedDown)
        {
            switch (e.KeyInput.Key)
            {
            case KEY_ESCAPE:

                if (started)
                {
                    removeHUD();
                    showFirstScreen();
                }
                else
                {
                    device->closeDevice();
                }
                return true;

            default:
                if (started)
                {
                    if (finished && canRestart)
                    {
                        restart();
                        return true;
                    }
                }
                else
                {
                    start();
                    return true;
                }
                return false;
            }
        }
    }
    else if (e.EventType == EET_MOUSE_INPUT_EVENT)
    {
        if (e.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
        {
            if (started)
            {
                if (finished)
                {
                    if (canRestart)
                    {
                        // start a new game
                        restart();
                    }
                }
                else
                {
                    shoot();
                }
            }
            else
            {
                start();
            }
            return true;
        }
    }
    return false;
}

void Game::restart()
{
    delete tm;
    for (int i = 0; i < endTexts.size(); i ++)
    {
        endTexts[i]->remove();
    }
    endTexts.clear();
    initNew();
}

void Game::start()
{
    hideFirstScreen();
    initNew();
}

void Game::createGunFlash()
{
    removeGunFlash(); // probably not needed

    video::ITexture *tex = driver->getTexture("bang.tga");
    int tx = tex->getOriginalSize().Width;
    int ty = tex->getOriginalSize().Height;

    int x = driver->getScreenSize().Width/2 - tx/2;
    int y = driver->getScreenSize().Height - ty;

    gunFlash = guienv->addImage(
        tex,
        core::position2d<s32>(x,y),
        true);
}

void Game::removeGunFlash()
{
    if (gunFlash)
    {
        gunFlash->remove();
        gunFlash = NULL;
    }
}

void Game::shoot()
{
    if (shellCount <= 0)
    {
//        sengine->play2D("sfx/shotgunempty.wav");
        //if (percentFull < 50) percentFull = 80; // run out of shells so end the game
        return;
    }

    createGunFlash();
//    sengine->play2D("sfx/shotgun2.wav");
    // pass mouse ray to TargetManager
    core::position2d<s32> mousePos = device->getCursorControl()->getPosition();
    core::line3df ray = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(mousePos, smgr->getActiveCamera());

    if (tm->shootTargets(ray)) // if hit a target
    {
    }

    shellCount --;

    if (shellCount < 0)
    {
        shellCount = 0;
    }

}

void Game::bloodSplat(core::vector3df hitPos, video::ITexture *texture)
{
    int w = texture->getOriginalSize().Width;
    int h = texture->getOriginalSize().Height;

    video::ITexture *RTT1, *RTT2;
    int material;
    int x,y;

    if (hitPos.Y > 0.001)   // Splatter on wall
    {
        // swap RTTs
        video::ITexture *tempRTT = wallRTT1;
        wallRTT1 = wallRTT2;
        wallRTT2 = tempRTT;

        RTT1 = wallRTT1;
        RTT2 = wallRTT2;
        material = 0;

        x = (int)(hitPos.X*512.0/16.0)+256-w/2;
        y = 512-(int)(hitPos.Y*512.0/12.0)-h/2;
    }
    else                    // Splatter on ground
    {
        // swap RTTs
        video::ITexture *tempRTT = groundRTT1;
        groundRTT1 = groundRTT2;
        groundRTT2 = tempRTT;

        RTT1 = groundRTT1;
        RTT2 = groundRTT2;
        material = 1;

        x = 512 - (int)((hitPos.X+11.0)*512.0/22.0)-w/2;
        y = 512 - (int)((hitPos.Z+4.0)*512.0/22.0)-h/2;
    }

    // rendering
    driver->setRenderTarget(RTT1, false,false);
    // draw old render
    driver->draw2DImage(RTT2, core::position2d<s32>(0,0));
    // render blood splat with transparency
    driver->draw2DImage(texture,
        core::rect<s32>(x,y,x+w,y+h),
        core::rect<s32>(0,0,w,h),
        0,0,true);

    // set to new RTT
///    wallsSceneNode->getMaterial(material).Textures[0] = RTT1;
}

void Game::stopMusic()
{/*
    if (sMusic)
    {
        sMusic->stop();
        sMusic->drop();
        sMusic = NULL;
    }
    */
}
void Game::setMusic(char *fileName)
{/*
    stopMusic();
    if (fileName)
    {
        sMusic = sengine->play2D(fileName, true, false, true, audio::ESM_NO_STREAMING);
        sMusic->setVolume(0.25);
    }
     */
}

int Game::loadScore()
{
    int temp;
    FILE *f = fopen(SCORE_FILE, "rb");
    if (f)
    {
        fread(&temp, sizeof(int), 1, f);
        fclose(f);
        return temp;
    }
    return 0;
}

void Game::saveScore()
{
    FILE *f = fopen(SCORE_FILE, "wb");
    if (f)
    {
        fwrite(&score, sizeof(int), 1, f);
        fclose(f);
    }
    else
    {
        printf("Warning: error saving high score.\n");
    }
}


