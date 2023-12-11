#include "JukeBox.h"

void JukeBox::loadTemplate(const std::string &workingDir)
{
    TiXmlDocument doc;
    if (doc.LoadFile((std::string("./") + workingDir + std::string("/VGUI/JukeBox.xml")).c_str()) == false)
    {
        // Template was not loaded
        return;
    }
    TiXmlHandle handle(&doc);

    TiXmlElement *element = handle.FirstChild("JukeBox").ChildElement("Window", 0).Element();
    texture = element->Attribute("texture");
    w = atoi(element->Attribute("width"));
    h = atoi(element->Attribute("height"));

    element = handle.FirstChild("JukeBox").ChildElement("NowPlaying", 0).Element();
    PlayInfoX = atoi(element->Attribute("x"));
    PlayInfoY = atoi(element->Attribute("y"));
    PlayInfoW = atoi(element->Attribute("w"));
    PlayInfoH = atoi(element->Attribute("h"));
    PlayInfoFont = element->Attribute("font");
    PlayInfoFontPoint = atoi(element->Attribute("pt"));

    element = handle.FirstChild("JukeBox").ChildElement("Playlist", 0).Element();
    PlayListX = atoi(element->Attribute("x"));
    PlayListY = atoi(element->Attribute("y"));
    PlayListW = atoi(element->Attribute("w"));
    PlayListH = atoi(element->Attribute("h"));
    PlayListFont = element->Attribute("font");
    PlayListFontPoint = atoi(element->Attribute("pt"));

    element = handle.FirstChild("JukeBox").ChildElement("VolumeSlider", 0).Element();
    VolumeX = atoi(element->Attribute("x"));
    VolumeY = atoi(element->Attribute("y"));
    VolumeW = atoi(element->Attribute("w"));
    VolumeH = atoi(element->Attribute("h"));
    VolumeDirection = atoi(element->Attribute("direction"));
    VolumeBarTex = element->Attribute("barTex");
    VolumeSlideTex = element->Attribute("slideTex");

    element = handle.FirstChild("JukeBox").ChildElement("PlaylistScroller", 0).Element();
    SongScrollX = atoi(element->Attribute("x"));
    SongScrollY = atoi(element->Attribute("y"));
    SongScrollW = atoi(element->Attribute("w"));
    SongScrollH = atoi(element->Attribute("h"));
    SongScrollDirection = atoi(element->Attribute("direction"));
    SongScrollBarTex = element->Attribute("barTex");
    SongScrollSlideTex = element->Attribute("slideTex");

}

void JukeBox::loadConfig()
{
    ifstream jukeconfig("./JukeBox.cfg");
    if (!jukeconfig.is_open())
        return;

    Properties jukeProp;
    jukeProp.load(jukeconfig);

    x = Integer::parseInt(jukeProp.getProperty("x", "0"));
    y = Integer::parseInt(jukeProp.getProperty("y", "0"));
    songDirectory = jukeProp.getProperty("SongDirectory", "/");
    playtype = Integer::parseInt(jukeProp.getProperty("PlayType", "2"));
    volume = Float::parseFloat(jukeProp.getProperty("Volume", "1.0"));
}

void JukeBox::init(const std::string &workingDir)
{
    x = y = 0;
    songDirectory = std::string("./") + workingDir + std::string("/Sounds/Music/");
    sound_factory = SoundFactory::getInstance();
    text_engine = Font::getInstance();
    texture_factory = TextureFactory::getInstance();

    playtype=2;
    visibleOffset=0;

    visible = false;
    hasFocus = false;
    resizeable = false;

    loadTemplate(workingDir);
    loadConfig();

    dir = opendir(songDirectory.c_str());

    TextureFactory::getInstance()->loadTexture(texture);

    text_engine->loadFont(PlayInfoFont);
    text_engine->genPointSize(PlayInfoFont, PlayInfoFontPoint);
    text_engine->genPointSize(PlayListFont, PlayListFontPoint);

    volumeSlider.setX(x + VolumeX).setY(y + VolumeY).setW(VolumeW).setH(VolumeH).setDirection(VolumeDirection).setBarTexture(VolumeBarTex).setSliderTexture(VolumeSlideTex).setBarMin(0).setBarMax(100).setCurrentValue(int(volume * 100));
    songScroller.setX(x + SongScrollX).setY(y + SongScrollY).setW(SongScrollW).setH(SongScrollH).setDirection(SongScrollDirection).setBarTexture(SongScrollBarTex).setSliderTexture(SongScrollSlideTex).setBarMin(0).setCurrentValue(0);

    firstStart = true;

    songlist = glGenLists(1);
    volume = 1.0;
    jukeSource = 0;

    recompileSongList = false;
}

void JukeBox::shutdown()
{
    ofstream temp("./JukeBox.cfg");
    temp << "x = " << x << "\n";
    temp << "y = " << y << "\n";
    temp << "SongDirectory = " << songDirectory << "\n";
    temp << "PlayType = " << playtype << "\n";
    temp << "Volume = " << volume << "\n";
    temp.close();
}

void JukeBox::update()
{
    struct dirent *de;
    files.clear();
    selectBox.clear();
    std::string temp;
    std::string::size_type val;
    unsigned int iterator=0;
    while ((de = readdir(dir)))
    {
        temp = de->d_name;
        val = temp.find(".ogg", 0);
        if (val != std::string::npos)
        {
            files.push_back(temp);
            GUI::Button temp;
            selectBox.push_back(temp);
            selectBox[iterator].setX(x + (int)PlayListX);
            selectBox[iterator].setY(y + (int)PlayListY - Font::getInstance()->getFontHeight(PlayListFont, PlayListFontPoint) + (iterator*15));
            selectBox[iterator].setW(295);
            selectBox[iterator].setH(15);
            iterator++;
        }
    }
    unsigned long int fontHeight = Font::getInstance()->getFontHeight(PlayListFont, PlayListFontPoint);
    MaximumVisibleSongs = int(float(PlayListH + fontHeight) / fontHeight);
    if (selectBox.size() > MaximumVisibleSongs)
        songScroller.setBarMax(selectBox.size() - MaximumVisibleSongs);
    else
        songScroller.setBarMax(0);
    songScroller.updateSlider();
    volumeSlider.updateSlider();
    recompileSongList = true;
}

void JukeBox::setPlayType(unsigned short type)
{
    playtype=type;
}

void JukeBox::tick(Input& input, const float &camPosX, const float &camPosY, const float &camPosZ)
{
    if ((sound_factory->hasFinished(jukeSource)) && (files.size()>0) && (sound_factory->isActive()))
    {
        if (playtype == 1) // Linear
        {
            sound_factory->stopSound(jukeSource);
            if (!firstStart)
                sound_factory->unloadSound(jukeSource);
            if ((currentTrackNum < files.size()-1) && (!firstStart))
                currentTrackNum++;
            else
            {
                currentTrackNum=0;
                firstStart = false;
            }
            jukeSource = sound_factory->loadSound((std::string)"Music/" + files[currentTrackNum]);
            sound_factory->playSound(jukeSource, false, volume, 0.0f);
            startTime = time(NULL);
            currentTrack = files[currentTrackNum];

            unsigned int timing = (long)sound_factory->getLength(jukeSource);

            std::stringstream temp2;
            if (timing > 60)
            {
                temp2 << timing / 60 << ":";
                timing -= 60 * (timing/60);
            }
            else
            {
                temp2 << "0:";
            }
            if (timing < 10)
                temp2 << "0" << timing;
            else
                temp2 << timing;
            totaltime = temp2.str();
        }
        else if(playtype == 2) // Random
        {
            sound_factory->stopSound(jukeSource);
            if (!firstStart)
            {
                sound_factory->unloadSound(jukeSource);
            }
            else
            {
                firstStart = false;
            }
            srand(SDL_GetTicks());
            currentTrackNum = (unsigned int)((rand() / ((double)RAND_MAX +1)) * files.size());
            jukeSource = sound_factory->loadSound((std::string)"Music/" + files[currentTrackNum]);
            sound_factory->playSound(jukeSource, false, volume, 0.0f);
            startTime = time(NULL);
            currentTrack = files[currentTrackNum];

            unsigned int timing = (long)sound_factory->getLength(jukeSource);

            std::stringstream temp2;
            if (timing > 60)
            {
                temp2 << timing / 60 << ":";
                timing -= 60 * (timing/60);
            }
            else
            {
                temp2 << "0:";
            }
            if (timing < 10)
                temp2 << "0" << timing;
            else
                temp2 << timing;
            totaltime = temp2.str();
        }
    }

    sound_factory->setPosition(jukeSource, camPosX, camPosY, camPosZ);

    if (!visible)
        return;

    if (input.getMouseDown(1) || input.getMouseScrollNoPop(1) || input.getMouseScrollNoPop(2))
    {
        int mousex = input.getMouseX();
        int mousey = input.getMouseY();

        if (mousex > (x + PlayListX) && mousex < (x + PlayListX + PlayListW) && mousey > (y + PlayListY - Font::getInstance()->getFontHeight(PlayListFont, PlayListFontPoint)) && mousey < (y + PlayListY + PlayListH))
        {
            for (unsigned int i = 0; i < MaximumVisibleSongs && i < selectBox.size() && sound_factory->isActive(); ++i)
            {
                selectBox[visibleOffset+i].parseInput(input);
                if (selectBox[visibleOffset+i].isPressed())
                {
                    sound_factory->stopSound(jukeSource);
                    if (!firstStart)
                    {
                        sound_factory->unloadSound(jukeSource);
                    }
                    else
                    {
                        firstStart = false;
                    }
                    currentTrackNum = visibleOffset+i;
                    jukeSource = sound_factory->loadSound((std::string)"Music/" + files[currentTrackNum]);
                    sound_factory->playSound(jukeSource, false, volume, 0.0f);
                    startTime = time (NULL);
                    currentTrack = files[currentTrackNum];

                    unsigned int timing = (long)sound_factory->getLength(jukeSource);

                    std::stringstream temp2;
                    if (timing > 60)
                    {
                        temp2 << timing / 60 << ":";
                        timing -= 60 * (timing/60);
                    }
                    else
                    {
                        temp2 << "0:";
                    }
                    if (timing < 10)
                        temp2 << "0" << timing;
                    else
                        temp2 << timing;
                    totaltime = temp2.str();

                    break;
                }
            }
        }

        scrollBox.parseInput(input);
        if (scrollBox.wheelUp() && visibleOffset != 0)
        {
            --visibleOffset;
            recompileSongList = true;
            songScroller.setCurrentValue(visibleOffset);
        }
        else if (scrollBox.wheelDown() && unsigned(visibleOffset) + MaximumVisibleSongs < selectBox.size())
        {
            ++visibleOffset;
            recompileSongList = true;
            songScroller.setCurrentValue(visibleOffset);
        }

        if (!volumeSlider.hasInput())
        {
            songScroller.parseInput(input);
            if (unsigned(songScroller.getCurrentValue()) != visibleOffset)
            {
                visibleOffset = songScroller.getCurrentValue();
                recompileSongList = true;
            }
        }

        if (!songScroller.hasInput())
        {
            volumeSlider.parseInput(input);
            if (volume != (float(volumeSlider.getCurrentValue())/100))
            {
                recompileSongList = true;
                volume = float(volumeSlider.getCurrentValue()) / 100;
            }
        }

        if (!volumeSlider.hasInput() && !songScroller.hasInput())
        {
            parseInput(input);
            if (isDragging)
            {
                recompileSongList = true;

                songScroller.setX(x + SongScrollX);
                songScroller.setY(y + SongScrollY);

                volumeSlider.setX(x + VolumeX);
                volumeSlider.setY(y + VolumeY);

                scrollBox.setDimensions(unsigned(int(x + PlayListX)), unsigned(int(y + PlayListY - 15)), PlayListW, PlayListH);
            }
        }

        if (recompileSongList)
        {
            unsigned int shiftAmount = 15*visibleOffset;
            for (unsigned int i = 0; i < selectBox.size(); ++i)
            {
                selectBox[i].setY(y + int(PlayListY + (15 * i) - shiftAmount) - 15);
                selectBox[i].setX(x + int(PlayListX));
            }
        }
    }
    else
    {
        // They still need to parse their inputs.
        for (unsigned int i = 0; i < MaximumVisibleSongs && i < selectBox.size(); ++i)
        {
            selectBox[visibleOffset+i].parseInput(input);
        }
        scrollBox.parseInput(input);
        songScroller.parseInput(input);
        parseInput(input);
    }

    if (sound_factory->isActive() && files.size() > 0)
    {
        unsigned int timing;
        timing = time(NULL) - startTime;
        if (timing == elapsedTime)
            return;

        elapsedTime = timing;

        sound_factory->setVolume(jukeSource, volume);

        std::stringstream temp;

        if (timing > 60)
        {
            unsigned int tempTiming = timing / 60;
            temp << tempTiming;
            timing -= 60 * (tempTiming);
        }
        else
        {
            temp << "0:";
        }
        if (timing < 10)
            temp << "0" << timing;
        else
            temp << timing;
        playtime = temp.str();
    }
    else
    {
        playtime = "0:0";
        totaltime = "0:0";
    }
}

void JukeBox::draw(const int &sw, const int &sh)
{
    if (!visible)
        return;

    if (recompileSongList)
    {
        glDeleteLists(songlist, 1);
        glNewList(songlist, GL_COMPILE_AND_EXECUTE);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, sw, sh, 0.0, -1.0, 2000);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glDisable(GL_CULL_FACE);

        glTranslatef(x, y, 0.0f);

        glPushMatrix();
        volumeSlider.draw();
        glPopMatrix();

        glColor3f(1.0f, 1.0f, 1.0f);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        texture_factory->applyTexture(texture);
        glPushMatrix();
        glBegin(GL_QUADS);
        {
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(0.0f, 0.0f);
            glTexCoord2f(1.0, 1.0f);
            glVertex2f((float)w, 0.0f);
            glTexCoord2f(1.0, 0.0);
            glVertex2f((float)w, (float)h);
            glTexCoord2f(0.0f, 0.0);
            glVertex2f(0.0f, (float)h);
        }
        glEnd();
        glPopMatrix();

        glPushMatrix();
        songScroller.draw();
        glPopMatrix();

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);

        glPushMatrix();
        glTranslatef(PlayListX, PlayListY, 0.0f);
        glPushMatrix();
        glColor3f(1.0f, 1.0f, 1.0f);
        for (unsigned int i = 0; i < MaximumVisibleSongs && i < files.size(); i++)
        {
            text_engine->drawText(files[visibleOffset+i], PlayListFont, PlayListFontPoint, PlayListW);
            glTranslatef(0.0f, 15.0f, 0.0);
        }
        glEndList();
        recompileSongList = false;
    }
    else
        glCallList(songlist);

    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(PlayInfoX, PlayInfoY, 0.0f);
    glPushMatrix();
    glColor3f(0.6f, 0.8f, 0.6f);
    std::stringstream temp;
    temp << currentTrack << "\n" << playtime << " / " << totaltime;
    text_engine->drawText(temp.str(), PlayInfoFont, PlayInfoFontPoint, PlayInfoW);
    glPopMatrix();
    glPopMatrix();

    glEnable(GL_CULL_FACE);
}
