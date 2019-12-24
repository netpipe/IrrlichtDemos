

    /// ##########################################################################################
    /// ############# AUDIO: (USING AUDIERE LIBRARY) #############################################
    /// ##########################################################################################

    int fchannels = 0;
    int frate = 0;
    audiere::SampleFormat fformat;
    audiere::SampleSource* src;

    /// Changing from frames, to seconds:
    int songFullTime = 0; // Retrieves number in seconds, ex: 1756785 --> Music Total Length
    int songCurrentTime = 0; // Retrieves number in seconds, ex: 1756785 --> Music Position (Now its 0)


    void load_MUSIC(std::string musicPath){

    if(texts[20]!=NULL){ texts[20]->setText(L"Please Standby\nThe show will continue in a moment..."); };
    CAMERA_SELECTED = 1; // 1
    fulllines = L"";
    for(int i=0; i<5; i++){ doingCamTransOnce[i]=true; };
    // closed_COURTAINS();

    /// Prepare the render with an GREYSCALE effect:
    // driver->beginScene(false, false, 0);
    driver->beginScene(true, true, 0);
    texts[25]->setText(L"Loading Music...");
    texts[7]->setText(L"");
    if(ppEffects[0]){ ppEffects[0]->render(NULL); };
    driver->setMaterial(driver->getMaterial2D()); // In case buffer is distorted
    guienv->drawAll();  /// Draw GUI
    drawMoreStuff();
    driver->endScene();
    texts[25]->setText(L"");
    texts[7]->setText(L"");

    /// Take screenshot and convert to texture:
    screenshot = driver->createScreenShot();
    if(screenshot!=NULL){ /// Voiding crashes, when menu window is hidden
    if(backgroundImageTexture != NULL){ driver->removeTexture(backgroundImageTexture); backgroundImageTexture = NULL; };
    if(backgroundImageTexture == NULL){ backgroundImageTexture = driver->addTexture("screenshot", screenshot); };
    screenshot->drop();
    screenshot = NULL;
    };

    /// Loading Bar for Lyrics Only: #1 (Initialize)
	LoadBarForMusic = new CLoadingBar(device, driver, guienv, backgroundImageTexture, rect<s32>(300, SCREEN_HEIGHT-50, SCREEN_WIDTH-300, SCREEN_HEIGHT-50+15), true, 7);  // posX, posY, bar width, bar height
    LoadBarForMusic->setPercent(5); // 0 to 100
    LoadBarForMusic->draw();
    // LoadBarForMusic->setVisible(true); // for the last "LoadBar" only

    // audiereAudioDevice = NULL; // clear device
    // music = NULL;         // clear music
    tags.clear();            // clears all items in array:  std::vector<std::string> tags
    if(music){ if(music->isPlaying()){ music->stop(); }; }; // clear music
    music = NULL;            // clear music
    bool error = false;

    // blah = OpenSampleSource(...)
    // Calculate length in seconds here ....
    // music = OpenSound(device, blah);

    // audiereAudioDevice = audiere::OpenDevice();
    if(!audiereAudioDevice){ std::cout << "ERROR: Audiere: Cant open audio device! \n" << std::endl; };
    music = audiere::OpenSound(audiereAudioDevice.get(), musicPath.c_str(), true); // Load music into memory? true / false

    /// May be a midi file: (TODO)
    // if(music == NULL){ audiere::OpenFile(audiereMidiDevice.get(), musicPath.c_str(), false); }
    // std::vector<FileFormatDesc> fff = audiere::GetSupportedFileFormats();

    if(!music){ error = true; std::cout << "ERROR: Audiere - Cant open the audio file!" << std::endl; };  /// Void CRASH for many reasons!
    if(music && music->getLength()<=0){ error = true; music = NULL; std::cout << "ERROR: Audiere - File not supported!" << std::endl; }; /// Void CRASH when file format is supported, but not the codec!

    /// Coloring ListBox:
    if(musicListBox && error){
    int c = musicListBox->getItemCount();
    for(int i=0; i<c; i++){ musicListBox->setItemOverrideColor(i, video::SColor(200,150,150,200)); }; // Colors to default
    musicListBox->setItemOverrideColor(musicListBox->getSelected(), video::SColor(255,255,0,0)); /// ERROR -> paint this item in red
    };

    LoadBarForMusic->setPercent(50); // 0 to 100
    LoadBarForMusic->draw();

    // All is ready, lets go...
    if(audiereAudioDevice && music && music->getLength()>0){

    // music->setRepeat(false);        // true, false or int.
    music->setVolume(GLOBAL_VOLUME);   // volume 0.0 = silence, 1.0 = maximum. Default is 1.0.
    music->setPitchShift(MUSIC_PITCH); // shift can range from 0.5 to 2.0. Default is 1.0.
    music->setPan(0.0);                // pan -1.0 = left, 0.0 = center (default), 1.0 = right
    // music->play();
    /// music->setReverb(x); <-- Would be GREAT for a real live concert, right!?... ;)
    // music->reset();
    // music->error();
    // music->isMuted();
    // music->isPlaying();
    // music->getPosition();
    // music->getLength();
    // music->read(64, buffer);
    // music->read();
    // music->openStream();
    // music = audiereAudioDevice->openBuffer(outSound, largest, sfinfo.channels, sfinfo.samplerate * 2, SF_ML16);

    LoadBarForMusic->setPercent(60); // 0 to 100
    LoadBarForMusic->draw();

    // audiere::MIDIDevice *audiereMIDIDevice = NULL;
    std::cout << "" << std::endl;
    std::cout << "------------------------------------------------------------------------------" << std::endl;
    std::cout << "Loaded a new Music" << std::endl;
    std::cout << "------------------------------------------------------------------------------" << std::endl;
    src = audiere::OpenSampleSource(musicPath.c_str());

    LoadBarForMusic->setPercent(90); // 0 to 100
    LoadBarForMusic->draw();

    std::cout << " " << std::endl;
    std::cout << "Audiere Device: " << audiereAudioDevice->getName() << std::endl;

    if(src){
    if(src->getTagCount()>0){
    std::cout << " " << std::endl;
    std::cout << "Music Tags: " << std::endl;
    tags.push_back("Music Tags: \n");
    };
    for(int i=0; i<src->getTagCount(); i++){
    std::cout << "Tag: " << i << ") Key: " << src->getTagKey(i) << " -> " << src->getTagValue(i) << std::endl;
    std::string tg = src->getTagKey(i); tg += ": "; tg += src->getTagValue(i); tg += "\n";
    tags.push_back(tg); // Add to array: Songs tags
    };
    if(src->getTagCount()>0){ tags.push_back("\n"); };
    };

    // Needed to get song length to seconds:
    if(src->isSeekable()){
    src->getFormat(fchannels, frate, fformat);
    std::cout << " " << std::endl;
    std::cout << "Music Info: " << std::endl;
    std::cout << "Channels: " << fchannels << " - Frame Rate: " << frate << " - Format: " << fformat << std::endl;
    };

    std::cout << "Music total frames: " << music->getLength() << std::endl;
    std::cout << "Music path: " << musicPath << std::endl;
    std::cout << " " << std::endl;

    /// Changing from frames, to seconds:
    songFullTime = music->getLength()/frate;      // Retrieves number in seconds, ex: 1756785 --> Music Total Length
    songCurrentTime = music->getPosition()/frate; // Retrieves number in seconds, ex: 1756785 --> Music Position (Now its 0)

    /// Music Scrollbar Controller:
    if(musicPositionScrollbar){
    musicPositionScrollbar->setMin(0);
    musicPositionScrollbar->setMax(music->getLength());
    musicPositionScrollbar->setPos(0);
    musicPositionScrollbar->setSmallStep(music->getLength()/50);
    musicPositionScrollbar->setLargeStep(music->getLength());
    };

    /// if(INTERNALS_SOUNDS){ soundClaps->play(); };

    LoadBarForMusic->setPercent(99); // 0 to 100
    LoadBarForMusic->draw();

    };

    /// Loading Bar:
    LoadBarForMusic->setPercent(100);   // Last One: Must be 100
    LoadBarForMusic->draw();
    LoadBarForMusic->setVisible(false); // Last One: Must hide LoadBar
    delete LoadBarForMusic; // Clear
    LoadBarForMusic = NULL; // Clear

    if(texts[20]!=NULL){ texts[20]->setText(L""); };

    };



    /// ##########################################################################################
    /// ############# AUDIO: (USING AUDIERE LIBRARY) - END #######################################
    /// ##########################################################################################
