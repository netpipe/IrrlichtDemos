          /// EVENTS: ( From: "libraries\MainLibs\myEvents.h" )

            // A Keyboard event: (exit)
            if(receiver.IsKeyDown(irr::KEY_ESCAPE)){ quit = true; };


            // A Keyboard event: (pause music + pause device)
            if(receiver.IsKeyDown(irr::KEY_SPACE) && canClickAgain[0] == 1){
            canClickAgain[0] = 0;
            for(int i=0; i<5; i++){ doingCamTransOnce[i]=true; };

            if(INTERNALS_SOUNDS){ soundClick->play(); }; // Audiere
            // sound.Play(); // SFML
            if(music && music->isPlaying()){ /// Confirm if music exists (else crashes!)
            if(INTERNALS_SOUNDS){ soundClaps->stop(); soundClaps->reset(); soundOoo->play(); };  // Booooo

            music->stop(); // Pause music
            paused = true;
            texts[12]->setText(L"Intermission\nThe show will continue in a few moments..."); // PAUSED == break, intermission
            // timer->stop(); // Pause irr device
            } else if(music){  /// Confirm if music exists (else crashes!)
            if(INTERNALS_SOUNDS){ soundClaps->play(); soundOoo->stop(); soundOoo->reset(); };  // Claps
            music->play();  // Start music
            paused = false;
            texts[12]->setText(L"");
            // timer->start(); // Start irr device
            };  // Audiere
            // if(music.GetStatus() == sf::Sound::Playing){ music.Stop(); } else { music.Play(); }  // SFML
            };
            if(receiver.IsKeyUp(irr::KEY_SPACE)){ canClickAgain[0] = 1; };
            // if(music && music->isPlaying()){ texts[12]->setText(L"");  }; /// BUG: To avoid the fails at "play loop mode"










            // A Keyboard event: (play mode music)
            if(receiver.IsKeyDown(irr::KEY_KEY_P) && canClickAgain[2] == 1){
            canClickAgain[2] = 0;
            if(INTERNALS_SOUNDS){ soundClick->play(); }; // Audiere
            // music.Play(); // SFML
            playMode += 1; if(playMode>4){ playMode = 1; }; // Play Modes: 1 Single, 2 Single Loop, 3 List loop, 4 List Random
            stringw textt = "";
            if(playMode == 1){ textt = "Play Mode: Single (P)";      };
            if(playMode == 2){ textt = "Play Mode: Single Loop (P)"; };
            if(playMode == 3){ textt = "Play Mode: List Loop (P)";   };
            if(playMode == 4){ textt = "Play Mode: List Random (P)"; };
            texts[8]->setText(textt.c_str());
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_P)){ canClickAgain[2] = 1; };






            /// GUI ListBox events ------------------------------------------------------------------------------------------ START


            // A GUI ListBox event: (select a music)
            if(ItemSelected_GUIListBox){

            // if(texts[20]!=NULL){ texts[20]->setText(L"Please Standby\nThe show will continue in a moment..."); };

            // if(!music){ std::cout << "music = NULL" << std::endl; };
            // std::cout << "musicListBox->getItemCount() = " << musicListBox->getItemCount() << std::endl; /// Empty returns 0
            // std::cout << "musicListBox->getSelected() = " << musicListBox->getSelected() << std::endl;   /// Empty returns -1
            // std::cout << "musicList.size() = " << musicList.size() << std::endl;                         /// Empty returns 0

            if(musicListBox->getItemCount()>0 && musicListBox->getSelected()>-1 && musicList.size()>0){ // void crashes if musicListBox has no items
            if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            int s = musicListBox->getSelected();
            musicID = s;
            /// -----------------------------------------------------------------------------------
            /// Reset to default camera:
            // CAMERA_SELECTED = 1;
            /// The call to "TEXT TO TEXTURE" function is here:
            /// -----------------------------------------------------------------------------------
            /// And write some text to texture too:
            /// doTextToTexture(nodeFlyingPlackard, "Irrlicht Music Player");
            doTextToTexture(nodeFlyingPlackard, musicListNamesOnly[musicID].c_str());
            /// -----------------------------------------------------------------------------------
            /// Coloring ListBox:
            int c = musicListBox->getItemCount();
            for(int i=0; i<c; i++){ musicListBox->setItemOverrideColor(i, video::SColor(200,150,150,200)); }; // Colors to default
            musicListBox->setItemOverrideColor(musicID, video::SColor(255,210,235,255));
            /// -----------------------------------------------------------------------------------
            // musicListBox->setItemHeight(10);
            // musicListBox->setItemOverrideColor(s, video::SColor(255,255,0,0)); // Add selected item a new color
            ItemSelected_GUIListBox = false;
            playNextSong = true;
            };

            // if(texts[20]!=NULL){ texts[20]->setText(L""); };

            };



            if(DEBUG_MODE==0){

             // A Keyboard event: (Get ListBox previous item)
            if(receiver.IsKeyDown(irr::KEY_UP) && canClickAgain[20] == 1){
            canClickAgain[20] = 0;
            int totbox = musicListBox->getItemCount();
            if(totbox>0){ /// Void crashes if musicListBox is empty!
            if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            int s = musicListBox->getSelected(); s-=1;
            if(s <= totbox && s <= (int)musicList.size()-1){
            musicListBox->setSelected(s); ItemSelected_GUIListBox = true; /// Select until last list item - and - Trigger: Load new music + lyric
            } else {
            musicListBox->setSelected(totbox-1); ItemSelected_GUIListBox = true; /// Goes to last list item - and - Trigger: Load new music + lyric
            };
            };
            };
            if(receiver.IsKeyUp(irr::KEY_UP)){ canClickAgain[20] = 1; };

            // A Keyboard event: (Get ListBox next item)
            if(receiver.IsKeyDown(irr::KEY_DOWN) && canClickAgain[21] == 1){
            canClickAgain[21] = 0;
            int totbox = musicListBox->getItemCount();
            if(totbox>0){ /// Void crashes if musicListBox is empty!
            if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            int s = musicListBox->getSelected(); s+=1;
            if(s <= totbox && s <= (int)musicList.size()-1){
            musicListBox->setSelected(s); ItemSelected_GUIListBox = true; /// Select until last list item - and - Trigger: Load new music + lyric
            } else {
            musicListBox->setSelected(0); ItemSelected_GUIListBox = true; /// Goes to the first list item - and - Trigger: Load new music + lyric
            };
            };
            };
            if(receiver.IsKeyUp(irr::KEY_DOWN)){ canClickAgain[21] = 1; };

            };


            // A Keyboard event: (Get ListBox current item)
            if(receiver.IsKeyDown(irr::KEY_RETURN) && canClickAgain[26] == 1){
            canClickAgain[26] = 0;
            int totbox = musicListBox->getItemCount();
            if(totbox>0){ /// Void crashes if musicListBox is empty!
            if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            signed int s = musicListBox->getSelected();
            if(s <= totbox && s <= (int)musicList.size()-1){ musicListBox->setSelected(s); ItemSelected_GUIListBox = true; }; /// Triggers: Load new music + lyrics
            };
            };
            if(receiver.IsKeyUp(irr::KEY_RETURN)){ canClickAgain[26] = 1; };







            /// GUI ListBox events ------------------------------------------------------------------------------------------ END



            // A Keyboard event: (Show FileOpenDialog)
            if(receiver.IsKeyDown(irr::KEY_KEY_D) && canClickAgain[30] == 1){
            canClickAgain[30] = 0;
            if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            if(canCreateNewFOD == true){ /// <-- To allow only 1 dialog instance!
            canCreateNewFOD = false;
            /// addFileOpenDialog --> http://irrlicht.sourceforge.net/docu/classirr_1_1gui_1_1_i_g_u_i_environment.html#aaac7c786a19c5cac51a550ce77cd972a
            musicFileDialog = guienv->addFileOpenDialog(L"", false, 0, -1, false, 0);
            };
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_D)){ canClickAgain[30] = 1; };

            // FileOpenDialog is done!...
            if(DirSelected_GUIFileOpenDialog == true){
            /// Fill ListBox:
            if(musicListBox){ fillListBox(); };
            // Reset:
            DirSelected_GUIFileOpenDialog = false;
            canCreateNewFOD = true;
            };



            // if(music && !music->isPlaying()){ } //  && playOneSong
            // if(!playSongNow && (music && !music->isPlaying()) && (playMode == 3 || playMode == 4)){ playSongNow = true; }
            /// TO-DO work still on progres...

            /// Play Mode: 1 - Single
            if(playMode == 1 && playNextSong && paused == false){
            if(musicID<0 || musicID>(int)musicList.size()-1){ musicID = 0; }; // Just in case...
            // music.Play(); // SFML
            if(music){ music->setRepeat(false); }; // Audiere
            playNextSong = false;
            musicListBox->setSelected(musicID);    // Update ListBox

            std::string mm = musicList[musicID];            // Get song name (full path) + GUI Buttons array starts at 100
            std::string nn = musicLyricsListPath[musicID];  // Get lyrics (full path) + GUI Buttons array starts at 100
            load_MUSIC(mm);
            load_LYRICS(nn);
            if(music){ music->play(); }; // Audiere
            if(music && INTERNALS_SOUNDS){ soundClaps->play(); };
            };



            /// Play Mode: 2 - Single Loop
            /*
            if(playMode == 2 && playNextSong && paused == false && music){
            if(musicID<0 || musicID>musicList.size()-1){ musicID = 0; }; // Just in case...
            // music.Play(); // SFML
            if(music){ music->setRepeat(true); }; // Audiere
            playNextSong = false;
            musicListBox->setSelected(musicID);   // Update ListBox

            std::string mm = musicList[musicID];            // Get song name (full path) + GUI Buttons array starts at 100
            std::string nn = musicLyricsListPath[musicID];  // Get lyrics (full path) + GUI Buttons array starts at 100
            loadMyMusic(mm); load_LYRICS(nn); if(music){ music->play(); }; // Audiere
            if(music && INTERNALS_SOUNDS){ soundClaps->play(); };
            };
            */

            /// Play Mode: 2 - Single Loop
            if(playMode == 2 && playNextSong && paused == false){ /// music is selected
            if(musicID<0 || musicID>(int)musicList.size()-1){ musicID = 0; }; // Just in case...
            // music.Play(); // SFML
            if(music){ music->setRepeat(true); }; // Audiere
            playNextSong = false;
            musicListBox->setSelected(musicID);   // Update ListBox

            // std::string mm = musicList[musicID];            // Get song name (full path) + GUI Buttons array starts at 100
            // std::string nn = musicLyricsListPath[musicID];  // Get lyrics (full path) + GUI Buttons array starts at 100
            // loadMyMusic(mm); load_LYRICS(nn); if(music){ music->play(); }; // Audiere
            // if(music && INTERNALS_SOUNDS){ soundClaps->play(); };
            };


            /// Play Mode: 3 - List Loop
            if(playMode == 3 && playNextSong && paused == false){
            // musicID += 1;
            // if(ClickedGUIButton == false){ musicID += 1; }
            if(musicID<0 || musicID>(int)musicList.size()-1){ musicID = 0; }; // Just in case...
            // music.Play(); // SFML
            if(music){ music->setRepeat(false); }; // Audiere
            playNextSong = false;
            musicListBox->setSelected(musicID);    // Update ListBox

            std::string mm = musicList[musicID];            // Get song name (full path) + GUI Buttons array starts at 100
            std::string nn = musicLyricsListPath[musicID];  // Get lyrics (full path) + GUI Buttons array starts at 100
            load_MUSIC(mm); load_LYRICS(nn); if(music){ music->play(); }; // Audiere
            if(music && INTERNALS_SOUNDS){ soundClaps->play(); };
            };


            /// Play Mode: 4 - List Random
            if(playMode == 4 && playNextSong && paused == false){
            int oldMusicID = musicID;
            musicID = rand()%musicList.size();
            if(musicID == oldMusicID){ musicID += 1; };
            if(musicID<0 || musicID>(int)musicList.size()-1){ musicID = 0; }; // Just in case...
            // music.Play(); // SFML
            if(music){ music->setRepeat(false); }; // Audiere
            playNextSong = false;
            musicListBox->setSelected(musicID);    // Update ListBox

            std::string mm = musicList[musicID];            // Get song name (full path) + GUI Buttons array starts at 100
            std::string nn = musicLyricsListPath[musicID];  // Get lyrics (full path) + GUI Buttons array starts at 100
            load_MUSIC(mm); load_LYRICS(nn); if(music){ music->play(); }; // Audiere
            if(music && INTERNALS_SOUNDS){ soundClaps->play(); };
            };



            // if((music.GetStatus() == sf::Sound::Playing) && (playMode == 3 || playMode == 4)){ // SFML
            if((music && !music->isPlaying()) && (playMode == 3 || playMode == 4) && paused == false){ // Audiere
            if(musicList.size()>0){
            playNextSong = true;
            musicID += 1;
            if(musicID<0 || musicID>(int)musicList.size()-1){ musicID = 0; }; // Just in case...
            /// -----------------------------------------------------------------------------------
            /// Reset to default camera:
            // CAMERA_SELECTED = 1;
            /// The call to "TEXT TO TEXTURE" function is here:
            /// -----------------------------------------------------------------------------------
            /// And write some text to texture too:
            /// doTextToTexture(nodeFlyingPlackard, "Irrlicht Music Player");
            doTextToTexture(nodeFlyingPlackard, musicListNamesOnly[musicID].c_str());
            /// -----------------------------------------------------------------------------------
            /// Coloring ListBox:
            int c = musicListBox->getItemCount();
            for(int i=0; i<c; i++){ musicListBox->setItemOverrideColor(i, video::SColor(200,150,150,200)); }; // Colors to default
            musicListBox->setItemOverrideColor(musicID, video::SColor(255,210,235,255));
            /// -----------------------------------------------------------------------------------
            };
            };




            /// 3D music volume: (todo)
            /*
            if(nodeBand && music){
            f32 px = camera[CAMERA_SELECTED]->getPosition().X;
            f32 pz = camera[CAMERA_SELECTED]->getPosition().Z;
            f32 sx = nodeBand->getPosition().X;
            f32 sz = nodeBand->getPosition().Z;
            // f32 distance = sqrt(pow(std::abs(px-sx),2) + pow(std::abs(pz-sz),2));
            // irr::f32 distance = sqrt(pow(abs(px-sx),2) + pow(abs(pz-sz),2));
            f32 distance = sqrt(pow(std::abs((int)px-(int)sx),2));
            music->setVolume(abs((int)distance-1000)/1000.0);
            };
            */

            /// Render to textute for nodeStageFloor:
            /*
            // set render target texture
            driver->setRenderTarget(rttr, true, true, video::SColor(0,0,0,255));
            smgr->setActiveCamera(camera[2]);
            // draw whole scene into render buffer
            smgr->drawAll();
            // set back old render target (The buffer might have been distorted, so clear it)
            driver->setRenderTarget(0, true, true, 0);
            smgr->setActiveCamera(camera[CAMERA_SELECTED]);
            */




            /// Animate all scene? (if music is playing)
            doAnimations = false;
            if(music && music->isPlaying()){ doAnimations = true; }; // Audiere




            // A Keyboard or Mouse event: (Show/Hide Menu)
            if(receiver.IsKeyDown(irr::KEY_KEY_M) && canClickAgain[3] == 1){
            if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            canClickAgain[3] = 0;
            static int wintohide = 1;
            bool doOnce = true;

            if(wintohide == 1 && doOnce == true){
            win1->setVisible(true);         // Left window visibility: visible
            musicListBox->setVisible(true); // ListBox visibility: visible
            texts[6]->setVisible(true);
            win2->setVisible(false);
            texts[4]->setVisible(false);
            texts[5]->setVisible(false);
            texts[26]->setVisible(false);
            texts[27]->setVisible(false);
            texts[28]->setVisible(false);
            musicPositionScrollbar->setVisible(false);
            musicVolumeScrollbar->setVisible(false);
            musicPitchScrollbar->setVisible(false);
            // Finished:
            wintohide = 2;
            doOnce = false;
            };

            if(wintohide == 2 && doOnce == true){
            win1->setVisible(false);         // Left window visibility: hidden
            musicListBox->setVisible(false); // ListBox visibility: visible
            texts[6]->setVisible(false);
            win2->setVisible(true);
            texts[4]->setVisible(true);
            texts[5]->setVisible(true);
            texts[26]->setVisible(true);
            texts[27]->setVisible(true);
            texts[28]->setVisible(true);
            musicPositionScrollbar->setVisible(true);
            musicVolumeScrollbar->setVisible(true);
            musicPitchScrollbar->setVisible(true);
            // Finished:
            wintohide = 3;
            doOnce = false;
            };

            if(wintohide == 3 && doOnce == true){
            win1->setVisible(false);         // Left window visibility: visible
            musicListBox->setVisible(false); // ListBox visibility: visible
            texts[6]->setVisible(false);
            win2->setVisible(false);
            texts[4]->setVisible(false);
            texts[5]->setVisible(false);
            texts[26]->setVisible(false);
            texts[27]->setVisible(false);
            texts[28]->setVisible(false);
            musicPositionScrollbar->setVisible(false);
            musicVolumeScrollbar->setVisible(false);
            musicPitchScrollbar->setVisible(false);
            // Finished:
            wintohide = 4;
            doOnce = false;
            };

            if(wintohide == 4 && doOnce == true){
            win1->setVisible(true);         // Left window visibility: visible
            musicListBox->setVisible(true); // ListBox visibility: visible
            texts[6]->setVisible(true);
            win2->setVisible(true);
            texts[4]->setVisible(true);
            texts[5]->setVisible(true);
            texts[26]->setVisible(true);
            texts[27]->setVisible(true);
            texts[28]->setVisible(true);
            musicPositionScrollbar->setVisible(true);
            musicVolumeScrollbar->setVisible(true);
            musicPitchScrollbar->setVisible(true);
            // Finished:
            wintohide = 1;
            doOnce = false;
            };

            };
            if(receiver.IsKeyUp(irr::KEY_KEY_M)){ canClickAgain[3] = 1; };



            // A Keyboard or Mouse event: (Show/Hide Infos)
            if(receiver.IsKeyDown(irr::KEY_KEY_I) && canClickAgain[14] == 1){
            if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            canClickAgain[14] = 0;
            infos+=1; // 0 = Music Infos, 1= Lyrics file content, 2 = Help, 3 = About, 4 = Nothing
            if(infos<0 || infos>5){ infos = 0; };
            if(infos==0){ texts[15]->setText(L"Display Infos: System (I)"); };
            if(infos==1){ texts[15]->setText(L"Display Infos: Lyrics (I)"); };
            if(infos==2){ texts[15]->setText(L"Display Infos: Help (I)");   };
            if(infos==3){ texts[15]->setText(L"Display Infos: Keys (I)");   };
            if(infos==4){ texts[15]->setText(L"Display Infos: About (I)");  };
            if(infos==5){ texts[15]->setText(L"Display Infos: None (I)");   };
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_I)){ canClickAgain[14] = 1; };



            // A Keyboard or Mouse event: (Select the Band on stage)
            if(receiver.IsKeyDown(irr::KEY_KEY_B) && canClickAgain[4] == 1){
            if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            canClickAgain[4] = 0;
            texts[14]->setOverrideColor(video::SColor(255,210,235,255));
            // cout << "Band: " << BAND_STYLE << endl;
            // changingBands = false; /// NOT launch moving loop
            BAND_SELECTED += 1;
            if(BAND_SELECTED<=0){ BAND_SELECTED = 0; }; // Just in case
            if(BAND_SELECTED >7){ BAND_SELECTED = 0; }; // No more bands, restart.
            load_BANDMESH(); /// Loading Band...
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_B)){ canClickAgain[4] = 1; };





            /// Moving Smooth Band entering on stage:
            if(!nodeBand){
            BAND_SELECTED = 0;
            load_BANDMESH(); /// Loading Band...
            };

            if(nodeBand){
            f32 bX = nodeBand->getPosition().X;
            f32 bY = nodeBand->getPosition().Y;
            f32 bZ = nodeBand->getPosition().Z;
            if(bX > 0){
            if(STENCIL_SHADOWS==true && texts[20]!=NULL){ texts[20]->setText(L"Generating shadows..."); };
            nodeBand->setPosition(vector3df(bX -= 0.70*frameDeltaTime, bY, bZ));
            };
            if(bX < 0){
            bX = 0.0f;
            if(STENCIL_SHADOWS==true && texts[20]!=NULL){ texts[20]->setText(L"Generating shadows..."); };
            nodeBand->setPosition(vector3df(bX, bY, bZ));
            nodeBand->addShadowVolumeSceneNode(); /// Activates STENCIL_SHADOWS only on this node (BAND) and only AFTER placed in its right position!
            };
            if(bX == 0){
            if(STENCIL_SHADOWS==true && texts[20]!=NULL){ texts[20]->setText(L""); };
            };
            };



             // A Keyboard event: (Change Lyrics Font Size)
            if(receiver.IsKeyDown(irr::KEY_KEY_K) && canClickAgain[16] == 1){
            canClickAgain[16] = 0;
            texts[18]->setOverrideColor(video::SColor(255,210,235,255));
            if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            FONT_SIZE_SELECTED += 1;
            if(FONT_SIZE_SELECTED<1 || FONT_SIZE_SELECTED>6){ FONT_SIZE_SELECTED = 1; };
            if(FONT_SIZE_SELECTED == 1){ FONT_SIZE = L"1"; };
            if(FONT_SIZE_SELECTED == 2){ FONT_SIZE = L"2"; };
            if(FONT_SIZE_SELECTED == 3){ FONT_SIZE = L"3"; };
            if(FONT_SIZE_SELECTED == 4){ FONT_SIZE = L"4"; };
            if(FONT_SIZE_SELECTED == 5){ FONT_SIZE = L"5"; };
            if(FONT_SIZE_SELECTED == 6){ FONT_SIZE = L"6"; };
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_K)){ canClickAgain[16] = 1; };

            stringw fontls = L"Lyrics Size: "; fontls+=FONT_SIZE.c_str(); fontls+=L" (K)";
            texts[18]->setText(fontls.c_str()); /// Set Lyrics Font Size




            // A Keyboard event: (Change Lyrics Colors)
            if(receiver.IsKeyDown(irr::KEY_KEY_L) && canClickAgain[19] == 1){
            canClickAgain[19] = 0;
            texts[19]->setOverrideColor(video::SColor(255,210,235,255));
            if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            FONT_COLOR_SELECTED += 1;
            if(FONT_COLOR_SELECTED<0 || FONT_COLOR_SELECTED>5){ FONT_COLOR_SELECTED = 0; };
            if(FONT_COLOR_SELECTED == 0){ FONT_COLOR = L"White";  };
            if(FONT_COLOR_SELECTED == 1){ FONT_COLOR = L"Red";    };
            if(FONT_COLOR_SELECTED == 2){ FONT_COLOR = L"Green";  };
            if(FONT_COLOR_SELECTED == 3){ FONT_COLOR = L"Blue";   };
            if(FONT_COLOR_SELECTED == 4){ FONT_COLOR = L"Yellow"; };
            if(FONT_COLOR_SELECTED == 5){ FONT_COLOR = L"Purple"; };
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_L)){ canClickAgain[19] = 1; };

            stringw fontlc = L"Lyrics Color: "; fontlc+=FONT_COLOR.c_str(); fontlc+=L" (L)";
            texts[19]->setText(fontlc.c_str()); /// Set Lyrics Font Color

            /// Lyrics Font final calculations:
            if(FONT_COLOR_SELECTED == 0){ FONT_COLOR = L"White";  FONT_COLOR_RED = 255; FONT_COLOR_GREEN = 255; FONT_COLOR_BLUE = 255; };
            if(FONT_COLOR_SELECTED == 1){ FONT_COLOR = L"Red";    FONT_COLOR_RED = 255; FONT_COLOR_GREEN = 0;   FONT_COLOR_BLUE = 0;   };
            if(FONT_COLOR_SELECTED == 2){ FONT_COLOR = L"Green";  FONT_COLOR_RED = 0;   FONT_COLOR_GREEN = 255; FONT_COLOR_BLUE = 0;   };
            if(FONT_COLOR_SELECTED == 3){ FONT_COLOR = L"Blue";   FONT_COLOR_RED = 0;   FONT_COLOR_GREEN = 0;   FONT_COLOR_BLUE = 255; };
            if(FONT_COLOR_SELECTED == 4){ FONT_COLOR = L"Yellow"; FONT_COLOR_RED = 255; FONT_COLOR_GREEN = 255; FONT_COLOR_BLUE = 0;   };
            if(FONT_COLOR_SELECTED == 5){ FONT_COLOR = L"Purple"; FONT_COLOR_RED = 200; FONT_COLOR_GREEN = 100; FONT_COLOR_BLUE = 255; };

            /// Apply lyrics font styles:
            texts[7]->setOverrideFont(font_lyrics[FONT_SIZE_SELECTED]); // Set the new lyrics font size
            /// texts[7]->setOverrideColor(video::SColor(255, FONT_COLOR_RED, FONT_COLOR_GREEN, FONT_COLOR_BLUE)); // Set the new lyrics font color





            /// Pitch Keys:
            /// ------------------------------------------->>>

            // A Keyboard event: (Pitch - reduce)
            if(receiver.IsKeyDown(irr::KEY_KEY_7) && canClickAgain[24] == 1){
            canClickAgain[24] = 0;
            userDragging_pitchScroolbar = true;
            /// if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            f32 sp = musicPitchScrollbar->getPos();
            sp -= 1;
            if(sp<=0 ){ sp = 0;  };
            if(sp>=20){ sp = 20; };
            musicPitchScrollbar->setPos(sp);
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_7)){ canClickAgain[24] = 1; };

            // A Keyboard event: (Pitch - increase)
            if(receiver.IsKeyDown(irr::KEY_KEY_8) && canClickAgain[25] == 1){
            canClickAgain[25] = 0;
            userDragging_pitchScroolbar = true;
            /// if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            f32 sp = musicPitchScrollbar->getPos();
            sp += 1;
            if(sp<=0 ){ sp = 0;  };
            if(sp>=20){ sp = 20; };
            musicPitchScrollbar->setPos(sp);
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_8)){ canClickAgain[25] = 1; };


            /// Pitch Scrollbar Controller:
            /// ------------------------------------------->>>

            if(userDragging_pitchScroolbar == true){
            f32 sp = musicPitchScrollbar->getPos();
            // musicVolumeScrollbar->setPos(sp);
            MUSIC_PITCH = sp/10;
            if(MUSIC_PITCH <= 0.1){ MUSIC_PITCH = 0.1; };
            if(MUSIC_PITCH >= 2.0){ MUSIC_PITCH = 2.0; };
            if(music){ music->setPitchShift(MUSIC_PITCH); }; /// Apply to the music

            stringw pitchtxt = L""; for(float i=0.0; i<MUSIC_PITCH; i+=0.1){ pitchtxt += L"|"; };
            /// Decimal places restriction:
            std::stringstream pssin; pssin.precision(1); pssin << std::fixed << MUSIC_PITCH; std::string pssout = pssin.str();
            /// Display texts:
            if(pssout=="1.0"){ pitchtxt += L" 1.0 Normal"; } else { pitchtxt += L" "; pitchtxt += pssout.c_str(); };
            texts[27]->setText(pitchtxt.c_str());
            userDragging_pitchScroolbar = false;
            };

            /// ------------------------------------------->>>






            /// Volume Keys:
            /// ------------------------------------------->>>

            /// Compares mouse position and ListBox and FileDialog positions: (ELSE: mouse wheel event works at Volume Bar and on that GUI elements simultaneously!)
            core::position2di mousepos = device->getCursorControl()->getPosition();

            if(musicListBox && musicListBox->isVisible()){
            core::rect<int> listboxpos = musicListBox->getRelativePosition();
            if((mousepos.X<listboxpos.UpperLeftCorner.X || mousepos.X>listboxpos.LowerRightCorner.X) || (mousepos.Y<listboxpos.UpperLeftCorner.Y || mousepos.Y>listboxpos.LowerRightCorner.Y)){
            mouseIsOutsideThisArea = true;   // Mouse is outside ListBox area!
            } else {
            mouseIsOutsideThisArea = false;  // Mouse is inside ListBox area!
            userMouseWheel = "";             // Reset this one too!
            };
            };
            if(musicListBox && !musicListBox->isVisible()){
            mouseIsOutsideThisArea = true;   /// ListBox is hidden, so mouse is outside the forbidden area!
            };

            if(musicFileDialog && canCreateNewFOD == false){
            core::rect<int> fdialogpos = musicFileDialog->getRelativePosition();
            if((mousepos.X<fdialogpos.UpperLeftCorner.X || mousepos.X>fdialogpos.LowerRightCorner.X) || (mousepos.Y<fdialogpos.UpperLeftCorner.Y || mousepos.Y>fdialogpos.LowerRightCorner.Y)){
            mouseIsOutsideThisArea = true;   // Mouse is outside FileDialog area!
            } else {
            mouseIsOutsideThisArea = false;  // Mouse is inside FileDialog area!
            userMouseWheel = "";            // Reset this one too!
            };
            };
            if(musicFileDialog && canCreateNewFOD == true){
            mouseIsOutsideThisArea = true;   /// FileDialog is hidden, so mouse is outside the forbidden area!
            };


            // cout << "musicListBox: " << win->isVisible() << endl;



            // A Keyboard event: (Volume - reduce)
            if((receiver.IsKeyDown(irr::KEY_KEY_9) || (userMouseWheel == "down" && mouseIsOutsideThisArea)) && canClickAgain[22] == 1){
            canClickAgain[22] = 0;
            userDragging_volumeScroolbar = 1;
            userMouseWheel = "";
            /// if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            f32 sp = musicVolumeScrollbar->getPos();
            sp -= 1;
            if(sp<=0 ){ sp = 0;  };
            if(sp>=20){ sp = 20; };
            musicVolumeScrollbar->setPos(sp);
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_9)){ canClickAgain[22] = 1; };

            // A Keyboard event: (Volume - increase)
            if((receiver.IsKeyDown(irr::KEY_KEY_0) || (userMouseWheel == "up" && mouseIsOutsideThisArea)) && canClickAgain[23] == 1){
            canClickAgain[23] = 0;
            userDragging_volumeScroolbar = 1;
            userMouseWheel = "";
            /// if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            f32 sp = musicVolumeScrollbar->getPos();
            sp += 1;
            if(sp<=0 ){ sp = 0;  };
            if(sp>=20){ sp = 20; };
            musicVolumeScrollbar->setPos(sp);
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_0)){ canClickAgain[23] = 1; };

            /// Volume Scrollbar Controller:
            /// ------------------------------------------->>>

            if(userDragging_volumeScroolbar == 1){
            f32 sp = musicVolumeScrollbar->getPos();
            // musicVolumeScrollbar->setPos(sp);
            GLOBAL_VOLUME = sp/20;
            if(GLOBAL_VOLUME<=0.0){ GLOBAL_VOLUME = 0.0; };
            if(GLOBAL_VOLUME>=1.0){ GLOBAL_VOLUME = 1.0; };
            if(music     ){ music->setVolume(GLOBAL_VOLUME);        }; /// Apply to the music
            if(soundClick){ soundClick->setVolume(GLOBAL_VOLUME/2); }; /// Apply to audio
            if(soundIntro){ soundIntro->setVolume(GLOBAL_VOLUME/2); }; /// Apply to audio
            if(soundOoo  ){ soundOoo->setVolume(GLOBAL_VOLUME/3);   }; /// Apply to audio
            if(soundClaps){ soundClaps->setVolume(GLOBAL_VOLUME/2); }; /// Apply to audio
            stringw volt = L""; for(float i=0.0; i<GLOBAL_VOLUME; i+=0.05){ volt += L"|"; };
            /// Decimal places restriction:
            std::stringstream vssin; vssin.precision(2); vssin << std::fixed << GLOBAL_VOLUME; std::string vssout = vssin.str();
            /// Display texts:
            if(vssout=="0.00"){ volt += L"Mute";  } else { volt += L""; };
            if(vssout=="0.05"){ volt += L" 5%";   } else { volt += L""; };
            if(vssout=="0.10"){ volt += L" 10%";  } else { volt += L""; };
            if(vssout=="0.15"){ volt += L" 15%";  } else { volt += L""; };
            if(vssout=="0.20"){ volt += L" 20%";  } else { volt += L""; };
            if(vssout=="0.25"){ volt += L" 25%";  } else { volt += L""; };
            if(vssout=="0.30"){ volt += L" 30%";  } else { volt += L""; };
            if(vssout=="0.35"){ volt += L" 35%";  } else { volt += L""; };
            if(vssout=="0.40"){ volt += L" 40%";  } else { volt += L""; };
            if(vssout=="0.45"){ volt += L" 45%";  } else { volt += L""; };
            if(vssout=="0.50"){ volt += L" 50%";  } else { volt += L""; };
            if(vssout=="0.55"){ volt += L" 55%";  } else { volt += L""; };
            if(vssout=="0.60"){ volt += L" 60%";  } else { volt += L""; };
            if(vssout=="0.65"){ volt += L" 65%";  } else { volt += L""; };
            if(vssout=="0.70"){ volt += L" 70%";  } else { volt += L""; };
            if(vssout=="0.75"){ volt += L" 75%";  } else { volt += L""; };
            if(vssout=="0.80"){ volt += L" 80%";  } else { volt += L""; };
            if(vssout=="0.85"){ volt += L" 85%";  } else { volt += L""; };
            if(vssout=="0.90"){ volt += L" 90%";  } else { volt += L""; };
            if(vssout=="0.95"){ volt += L" 95%";  } else { volt += L""; };
            if(vssout=="1.00"){ volt += L" 100%"; } else { volt += L""; };
            texts[26]->setText(volt.c_str());
            userDragging_volumeScroolbar = 0;
            };

            /// ------------------------------------------->>>










            // A Keyboard event: (choose view camera)
            if(receiver.IsKeyDown(irr::KEY_KEY_C) && canClickAgain[5] == 1){
            canClickAgain[5] = 0;
            if(INTERNALS_SOUNDS){ soundClick->play(); }  // Audiere
            CAMERA_SELECTED += 1;
            if(CAMERA_SELECTED <1 || CAMERA_SELECTED >10){ CAMERA_SELECTED = 1; };
            stringw txtcam = L"";
            if(CAMERA_SELECTED==1){ randomCamera = true;  txtcam += L"Camera: Auto (C)"; };
            if(CAMERA_SELECTED >1){ randomCamera = false; txtcam += L"Camera: "; txtcam += CAMERA_SELECTED; txtcam += L" (C)"; };
            texts[1]->setText(txtcam.c_str()); /// Set Camera
            doCameraTransition = true;
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_C)){ canClickAgain[5]  = 1; };



            // A Keyboard event: (choose your sound)
            // texts[11] = guienv->addStaticText(L"Audios: 1, 2, 3", core::rect<s32>(10,sy,200,SCREEN_HEIGHT), false, false, win); // border? wordwrap?

            if(receiver.IsKeyDown(irr::KEY_KEY_1) && canClickAgain[12] == 1){
            canClickAgain[12] = 0;
            if(INTERNALS_SOUNDS){ soundClaps->play(); soundOoo->stop(); soundOoo->reset(); };  // Claps
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_1)){ canClickAgain[12] = 1; };

            if(receiver.IsKeyDown(irr::KEY_KEY_2) && canClickAgain[13] == 1){
            canClickAgain[13] = 0;
            if(INTERNALS_SOUNDS){ soundClaps->stop(); soundClaps->reset(); soundOoo->play(); };  // Booooo
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_2)){ canClickAgain[13] = 1; };



            stringw spectxt = L"Sounds: ";
            if(receiver.IsKeyDown(irr::KEY_KEY_O) && canClickAgain[29] == 1){
            canClickAgain[29] = 0;
            texts[23]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue
            if(INTERNALS_SOUNDS){ soundClick->play(); }  // Audiere
            if(INTERNALS_SOUNDS){ INTERNALS_SOUNDS = false; } else { INTERNALS_SOUNDS = true; };
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_O)){ canClickAgain[29] = 1; };
            // Apply text:
            if(INTERNALS_SOUNDS == false){ spectxt += "Off (O)"; } else { spectxt += "On (O)"; };
            texts[23]->setText(spectxt.c_str());



            stringw txtclock = L"Clock: ";
            if(receiver.IsKeyDown(irr::KEY_KEY_H) && canClickAgain[31] == 1){
            canClickAgain[31] = 0;
            texts[24]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue
            if(INTERNALS_SOUNDS){ soundClick->play(); }  // Audiere
            if(CLOCK_VISIBLE){ CLOCK_VISIBLE = false; } else { CLOCK_VISIBLE = true; };
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_H)){ canClickAgain[31] = 1; };
            // Apply text:
            if(CLOCK_VISIBLE == false){ txtclock += "Off (H)"; } else { txtclock += "On (H)"; };
            texts[24]->setText(txtclock.c_str());








            /*
            // A Keyboard event: (check for updates)
            if(receiver.IsKeyDown(irr::KEY_F1) && canClickAgain[15] == 1){
            canClickAgain[15] = 0;
            if(INTERNALS_SOUNDS){ soundClick->play(); }  // Audiere
            system("cmd /c start http://sourceforge.net/projects/irrlichtmp");
            };
            if(receiver.IsKeyUp(irr::KEY_F1)){ canClickAgain[15] = 1; };

            // A Keyboard event: (visit author webpage)
            if(receiver.IsKeyDown(irr::KEY_F2) && canClickAgain[27] == 1){
            canClickAgain[27] = 0;
            if(INTERNALS_SOUNDS){ soundClick->play(); }  // Audiere
            system("cmd /c start http://sites.google.com/site/jorgerosaportfolio");
            };
            if(receiver.IsKeyUp(irr::KEY_F2)){ canClickAgain[27] = 1; };
            */


            // A Keyboard event: (Show type)
            if(receiver.IsKeyDown(irr::KEY_KEY_S) && canClickAgain[28] == 1){
            canClickAgain[28] = 0;
            texts[22]->setOverrideColor(video::SColor(255,210,235,255));
            if(INTERNALS_SOUNDS){ soundClick->play(); }  // Audiere
            SHOW_TYPE += 1;
            if(SHOW_TYPE<0 || SHOW_TYPE>2){ SHOW_TYPE = 0; };
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_S)){ canClickAgain[28] = 1; };

            stringw txt = L"Show: ";
            if(SHOW_TYPE == 0){ txt += L"Slow (S)";   };
            if(SHOW_TYPE == 1){ txt += L"Normal (S)"; };
            if(SHOW_TYPE == 2){ txt += L"Fast (S)";   };
            texts[22]->setText(txt.c_str());




            // A Keyboard event: (cameras ZOOM)
            /*
            if(receiver.IsKeyDown(irr::KEY_KEY_Z)){ // Zoom out
            f32 newFOV1 = camera[CAMERA_SELECTED]->getFOV();
            // newFOV1 = core::min_(newFOV1 + DEGTORAD, core::PI * .5f);
            newFOV1 = core::min_(newFOV1 + DEGTORAD, core::PI * 1.0f);
            if(newFOV1 >=1.5f){ newFOV1 = 1.5f; };
            camera[CAMERA_SELECTED]->setFOV(newFOV1);
            };
            if(receiver.IsKeyDown(irr::KEY_KEY_X)){ // Zoom in
            f32 newFOV1 = camera[CAMERA_SELECTED]->getFOV();
            // newFOV1 = core::max_(newFOV1 - DEGTORAD, core::PI * .0125f);
            newFOV1 = core::max_(newFOV1 - DEGTORAD, core::PI * 0.0250f);
            if(newFOV1 <=0.3f){ newFOV1 = 0.3f; };
            camera[CAMERA_SELECTED]->setFOV(newFOV1);
            };
            */




            static int previous_cam = 999999999;
            static int rcam = 999999999;
            static int keepcam = 0; // Keep random camera a bit, before change it.
            static int keepcam_duration = 12000; // For some cameras stay longer time than others
            if(SHOW_TYPE==0){ keepcam_duration = 12000; };
            if(SHOW_TYPE==1){ keepcam_duration =  8000; };
            if(SHOW_TYPE==2){ keepcam_duration =  5000; };
            keepcam += 1*frameDeltaTime; // keep random camera an minimum ammount of time

            if(doAnimations && randomCamera && keepcam>=keepcam_duration){
            // Reset cameras out of random loop:
            for(int i=0; i<5; i++){ doingCamTransOnce[i]=true; };
            // Random camera:
            rcam = (rand()%10)+1; // 1 to 11
            if(previous_cam == rcam) { /// Avoid new same camera again
            keepcam = 999999999;
            } else {
            previous_cam = rcam;
            CAMERA_SELECTED = 1;
            doCameraTransition = true;
            if(rcam == 1 ){ CAMERA_SELECTED = 1;  keepcam = 0; doCameraTransition = true; };
            if(rcam == 2 && SHOW_TYPE != 0){ CAMERA_SELECTED = 2; keepcam = 0; doCameraTransition = true; }; /// Disabled in Show:Slow
            if(rcam == 3 ){ CAMERA_SELECTED = 3;  keepcam = 0; doCameraTransition = true; };
            if(rcam == 4 ){ CAMERA_SELECTED = 4;  keepcam = 0; doCameraTransition = true; };
            if(rcam == 5 ){ CAMERA_SELECTED = 5;  keepcam = 0; doCameraTransition = true; };
            if(rcam == 6 && SHOW_TYPE != 0){ CAMERA_SELECTED = 6; doCameraTransition = true; }; /// Disabled in Show:Slow
            if(rcam == 7 && SHOW_TYPE == 0){ CAMERA_SELECTED = 7; keepcam = +9000; doCameraTransition = true; }; /// The camera from behind bands (less time)
            if(rcam == 7 && SHOW_TYPE == 1){ CAMERA_SELECTED = 7; keepcam = +5000; doCameraTransition = true; }; /// The camera from behind bands (less time)
            if(rcam == 7 && SHOW_TYPE == 2){ CAMERA_SELECTED = 7; keepcam = +2000; doCameraTransition = true; }; /// The camera from behind bands (less time)
            if(rcam == 8 ){ CAMERA_SELECTED = 8;  keepcam = 0; doCameraTransition = true; };
            if(rcam == 9 ){ CAMERA_SELECTED = 9;  keepcam = 0; doCameraTransition = true; };
            if(rcam == 10){ CAMERA_SELECTED = 10; keepcam = -3000; doCameraTransition = true; }; /// The follow path camera (more time)
            // if(rcam >= 10 && rcam < 16){ CAMERA_SELECTED = 1;  keepcam = 0; keepcam_duration = 2000; }; /// More chances for camera 1
            // if(rcam >= 16 && rcam < 21){ CAMERA_SELECTED = 5;  keepcam = 0; keepcam_duration = 2000; }; /// More chances for camera 5
            // if(rcam >= 21 && rcam < 25){ CAMERA_SELECTED = 10; keepcam = 0; keepcam_duration = 6000; }; /// More chances for camera 10
            newFOV1 = (double)((rand())/0.90f);
            if(newFOV1 >= 0.90f){ newFOV1 = 0.90f; };
            if(newFOV1 <= 0.60f){ newFOV1 = 0.60f; };
            camera[CAMERA_SELECTED]->setFOV(newFOV1);
            };
            };





            /// If music exists, show is not slow mode, and music >5 sec to end, do EFFECTS:
            /// Cameras effects: (postrender effects)
            if(doAnimations && SHOW_TYPE != 0 && songCurrentTime<(songFullTime-5)){
            /// Random Camera effects ---> TODO: Sync this stuff, with song beats
            if(doEFFrand){
            int df = 0;
            if(SHOW_TYPE==1){ df = rand()%1000; }; // 1500; // 800
            if(SHOW_TYPE==2){ df = rand()%800;  }; // 1500; // 800
            if(df == 100){ waitEFF = 0; doEFF = true; } else { doEFF = false; };
            };
            waitEFF += 0.1*frameDeltaTime;
            // if(doEFF && waitEFF <100){
            if(doEFF && waitEFF == 1){ /// Random effect
            doEFF = true;
            doEFFrand = false;
            ppeffectsEnable = true;
            ppEffectsRand = rand()%ppeCount;
            };
            if(doEFF && waitEFF > 1 && waitEFF < 70){ /// Keep the effect for a while
            doEFF = true;
            doEFFrand = false;
            ppeffectsEnable = true;
            // ppEffectsRand = rand()%ppeCount;
            };
            if(doEFF && waitEFF >= 70){ /// Remove effects
            waitEFF = 0;
            doEFF = false;
            doEFFrand = true;
            ppeffectsEnable = false;
            // ppEffectsRand = rand()%ppeCount;
            };
            } else { /// Remove effects
            waitEFF = 0;
            doEFF = false;
            doEFFrand = true;
            ppeffectsEnable = false;
            // ppEffectsRand = rand()%ppeCount;
            };


            /// If MUTED, always do grey effect:
            if(GLOBAL_VOLUME<=0.0){
            doEFF = true;           /// For effect
            doEFFrand = false;      /// For effect
            ppeffectsEnable = true; /// For effect
            ppEffectsRand = 0;      /// 0 == Do only grey effect now
            };





            /// Cameras FAST ZOOMS in/out: Waiting...
            zoomwait += 1 * frameDeltaTime;

            if(zoomwait >= zoomwaited){ zoomwaited = rand()%20000+1000; zoomwait = 0.0; dozoomrand = true; };
            /// Cameras FAST ZOOMS in/out: Do it!
            if(doAnimations){

            if(dozoomrand){
            int direction = rand()%2; // 0 to 1
            if(direction == 0){ doZoomingIn = true;  };
            if(direction == 1){ doZoomingIn = false; };
            dozoomrand = false;
            };
            if(doZoomingIn && zoomwait<800){
            f32 fovShowSpeed = 0.00030f;
            if(SHOW_TYPE ==0){ fovShowSpeed = 0.00001f; };
            if(SHOW_TYPE ==1){ fovShowSpeed = 0.00030f; };
            if(SHOW_TYPE ==2){ fovShowSpeed = 0.00090f; };
            /// Random Camera zooms ---> TO-DO: Sync this stuff, with song beats
            camRatio1 = ((4.0-0.0)*((irr::f32)rand()/RAND_MAX))+1.0; // from (0.0 to 4.0)
            camRatio2 = ((3.0-0.0)*((irr::f32)rand()/RAND_MAX))+1.0; // from (0.0 to 3.0)
            newFOV1 -= fovShowSpeed * frameDeltaTime;
            if(newFOV1 >= 1.25f){ newFOV1 = 1.25f; doZoomingIn = true;  };
            if(newFOV1 <= 0.55f){ newFOV1 = 0.55f; doZoomingIn = false; };
            camera[CAMERA_SELECTED]->setFOV(newFOV1);
            camera[CAMERA_SELECTED]->setAspectRatio(camRatio1/camRatio2); // Sets the aspect ratio (irr camera default ratio is: 4.0f/3.0f)
            };
            if(!doZoomingIn && zoomwait<800){
            f32 fovShowSpeed = 0.00030f;
            if(SHOW_TYPE ==0){ fovShowSpeed = 0.00001f; };
            if(SHOW_TYPE ==1){ fovShowSpeed = 0.00030f; };
            if(SHOW_TYPE ==2){ fovShowSpeed = 0.00090f; };
            camRatio1 = ((4.0-0.0)*((irr::f32)rand()/RAND_MAX))+1.0; // from (0.0 to 4.0)
            camRatio2 = ((3.0-0.0)*((irr::f32)rand()/RAND_MAX))+1.0; // from (0.0 to 3.0)
            newFOV1 += fovShowSpeed * frameDeltaTime;
            if(newFOV1 >= 1.25f){ newFOV1 = 1.25f; doZoomingIn = true;  };
            if(newFOV1 <= 0.55f){ newFOV1 = 0.55f; doZoomingIn = false; };
            camera[CAMERA_SELECTED]->setFOV(newFOV1);
            camera[CAMERA_SELECTED]->setAspectRatio(camRatio1/camRatio2); // Sets the aspect ratio (irr camera default ratio is: 4.0f/3.0f)
            };

            /// Cameras CONSTANT SLOW ZOOMS in/out:
            if(doZoomingIn2){
            f32 fovShowSpeed = 0.00012f;
            int fovmultiplier = 2;
            if(SHOW_TYPE ==0){ fovShowSpeed = 0.00001f; fovmultiplier = 10; };
            if(SHOW_TYPE ==1){ fovShowSpeed = 0.00012f; fovmultiplier = 1;  };
            if(SHOW_TYPE ==2){ fovShowSpeed = 0.00015f; fovmultiplier = 2;  };
            /// Random Camera zooms ---> TO-DO: Sync this stuff, with song beats
            camRatio1 = ((4.0-0.0)*((irr::f32)rand()/RAND_MAX))+1.0; // from (0.0 to 4.0)
            camRatio2 = ((3.0-0.0)*((irr::f32)rand()/RAND_MAX))+1.0; // from (0.0 to 3.0)
            newFOV2 = (fovShowSpeed*frameDeltaTime);
            newFOV1 -= newFOV2*fovmultiplier;
            if(newFOV1 >= 1.25f){ newFOV1 = 1.25f; doZoomingIn2 = true;  };
            if(newFOV1 <= 0.55f){ newFOV1 = 0.55f; doZoomingIn2 = false; };
            camera[CAMERA_SELECTED]->setFOV(newFOV1);
            camera[CAMERA_SELECTED]->setAspectRatio(camRatio1/camRatio2); // Sets the aspect ratio (irr camera default ratio is: 4.0f/3.0f)
            // cout << "Zooming In: " << newFOV1 << endl;
            };
            if(!doZoomingIn2){
            f32 fovShowSpeed = 0.00012f;
            int fovmultiplier = 2;
            if(SHOW_TYPE ==0){ fovShowSpeed = 0.00001f; fovmultiplier = 10; };
            if(SHOW_TYPE ==1){ fovShowSpeed = 0.00012f; fovmultiplier = 1;  };
            if(SHOW_TYPE ==2){ fovShowSpeed = 0.00015f; fovmultiplier = 2;  };
            camRatio1 = ((4.0-0.0)*((irr::f32)rand()/RAND_MAX))+1.0; // from (0.0 to 4.0)
            camRatio2 = ((3.0-0.0)*((irr::f32)rand()/RAND_MAX))+1.0; // from (0.0 to 3.0)
            newFOV2 = (fovShowSpeed*frameDeltaTime);
            newFOV1 += newFOV2*fovmultiplier;
            if(newFOV1 >= 1.25f){ newFOV1 = 1.25f; doZoomingIn2 = true;  };
            if(newFOV1 <= 0.55f){ newFOV1 = 0.55f; doZoomingIn2 = false; };
            camera[CAMERA_SELECTED]->setFOV(newFOV1);
            camera[CAMERA_SELECTED]->setAspectRatio(camRatio1/camRatio2); // Sets the aspect ratio (irr camera default ratio is: 4.0f/3.0f)
            // cout << "Zooming Out: " << newFOV1 << endl;
            };

            }; // END: if(doAnimations)

            /// No music, then set FOV (zoom) to it default value: (0.80f)
            if(!doAnimations && newFOV1!=0.80f){
            f32 fovShowSpeed = 0.00004f;
            int fovmultiplier = 2;
            camRatio1 = ((4.0-0.0)*((irr::f32)rand()/RAND_MAX))+1.0; // from (0.0 to 4.0)
            camRatio2 = ((3.0-0.0)*((irr::f32)rand()/RAND_MAX))+1.0; // from (0.0 to 3.0)
            newFOV2 = (fovShowSpeed*frameDeltaTime);
            if(newFOV1 > 0.80f){ newFOV1 -= (newFOV2*fovmultiplier); };
            if(newFOV1 < 0.80f){ newFOV1 += (newFOV2*fovmultiplier); };
            if(newFOV1 >= 1.25f){ newFOV1 = 1.25f; };
            if(newFOV1 <= 0.55f){ newFOV1 = 0.55f; };
            camera[CAMERA_SELECTED]->setFOV(newFOV1);
            camera[CAMERA_SELECTED]->setAspectRatio(camRatio1/camRatio2); // Sets the aspect ratio (irr camera default ratio is: 4.0f/3.0f)
            };


            /*
            /// Animate textures (reflexes.jpg)
            static f32 luzinhas_rotY = 0.0f;
            nodeStage->getMaterial(0).getTextureMatrix(1).setTextureRotationCenter( luzinhas_rotY );
            // nodeStage->getMaterial(0).getTextureMatrix(0).setRotationDegrees(irr::core::vector3d<f32>(luzinhas_rotY,luzinhas_rotY,luzinhas_rotY));
            luzinhas_rotY += 0.05 * frameDeltaTime;
            */




            if(doAnimations){
            /// Animate textures --> http://www.irrlicht3d.org/wiki/index.php?n=Main.HowToUseTexturesAndMaterials
            // 1) Animated "irrlich words": (Scrolling Texture)
            // int mc = nodeStagePlackard->getMaterialCount();
            // std::cout << "Material count: " << mc << std::endl;
            // for(int i=0; i<5; i++){
            f32 fspeed = 0;
            if(SHOW_TYPE == 0){ fspeed = 0.00002; };
            if(SHOW_TYPE == 1){ fspeed = 0.00003; };
            if(SHOW_TYPE == 2){ fspeed = 0.00006; };
            nodeStagePlackard->getMaterial(0).getTextureMatrix(0).setTextureTranslate(uu*1,vv);  /// Irrlicht characters ---> Range is clipped between 0 and 1 with the default initial value of 0.5.
            nodeFlyingPlackard->getMaterial(1).getTextureMatrix(0).setTextureTranslate(uu*3,vv); /// Plackard ---> Range is clipped between 0 and 1 with the default initial value of 0.5.
            uu += fspeed * frameDeltaTime; // vv += 0.0005;
            if(uu>1.0 || vv>1.0){ uu = 0.0; vv = 0.0; };
            // };

            // 2) Animated "disco spot lights ball": (Scrolling Texture all over the scene)
            int db = rand()%800; if(db==100){ doBall = true; } else { doBall = false; };
            if(doBall){ ballDirection = rand()%2; };

            if(ballDirection==0){
            for(u32 i=0; i<nodeStagePlackard->getMaterialCount(); i++){
            nodeStagePlackard->getMaterial(i).getTextureMatrix(1).setTextureTranslate(uuu,vvv); // Moving ball textures
            // nodeStagePlackard->getMaterial(i).getTextureMatrix(1).setTextureScaleCenter(0.001,0.001);
            };

            uuu += 0.0001 * frameDeltaTime;
            vvv += 0.0001 * frameDeltaTime;
            if(uuu>1.0 || vvv>1.0){ uuu = 0.0; vvv = 0.0; };
            };
            if(ballDirection==1){
            for(u32 i=0; i<nodeStagePlackard->getMaterialCount(); i++){
            nodeStagePlackard->getMaterial(i).getTextureMatrix(1).setTextureRotationCenter(radians); // Rotating ball textures
            };
            radians += 0.003; if(radians>360){ radians=0; };
            };


            // 3A) Animated TV: (Scaling Texture && Rotation)
            // rot += 0.01;
            static bool scale_decrease = true;
            if(scale_decrease==true){
            if(SHOW_TYPE == 0){ zx += 0.00003 * frameDeltaTime; zy += 0.00003 * frameDeltaTime; };
            if(SHOW_TYPE == 1){ zx += 0.00050 * frameDeltaTime; zy += 0.00050 * frameDeltaTime; };
            if(SHOW_TYPE == 2){ zx += 0.00100 * frameDeltaTime; zy += 0.00100 * frameDeltaTime; };
            };
            if(scale_decrease==false){
            if(SHOW_TYPE == 0){ zx -= 0.00003 * frameDeltaTime; zy -= 0.00003 * frameDeltaTime; };
            if(SHOW_TYPE == 1){ zx -= 0.00050 * frameDeltaTime; zy -= 0.00050 * frameDeltaTime; };
            if(SHOW_TYPE == 2){ zx -= 0.00100 * frameDeltaTime; zy -= 0.00100 * frameDeltaTime; };
            };
            if(zx>1.0 || zy>1.0){ zx = 1.0; zy = 1.0; scale_decrease = false; };
            if(zx<0.8 || zy<0.8){ zx = 0.8; zy = 0.8; scale_decrease = true;  };
            for(u32 i=0; i < nodeTVback1->getMaterialCount(); i++){
            nodeTVback1->getMaterial(i).getTextureMatrix(0).setTextureScaleCenter(zx,zx); // from 0 to 1 you're zooming with 0.5 means to 2x;
            // nodeTVback1->getMaterial(i).getTextureMatrix(0).setTextureRotationCenter(rot); // radians
            };

            /// The TVS images:
            nodeTVback1->setVisible(true);
            nodeTVback2->setVisible(true);
            nodeTVback3->setVisible(true);

            } else {

            // 3B) Animated TV: (Scaling Texture && Rotation) = ZERO
            // for(u32 i=0; i<nodeTVback1->getMaterialCount(); i++){
            // nodeTVback1->getMaterial(i).getTextureMatrix(0).setTextureScaleCenter(1,1); // from 0 to 1 you're zooming with 0.5 means to 2x;
            /// nodeTVback1->getMaterial(i).getTextureMatrix(0).setTextureRotationCenter(rot); // radians
            // };

            /// Plackard with texts centered:
            uu = 0.0;
            nodeFlyingPlackard->getMaterial(1).getTextureMatrix(0).setTextureTranslate(uu,vv);

            /// The TV images:
            nodeTVback1->setVisible(false);
            nodeTVback2->setVisible(false);
            nodeTVback3->setVisible(false);

            };




            if(CLOCK_VISIBLE == true){
            /// Show Clock:
            nBody->setVisible(true);        // Clock body
            nodeClock->setVisibility(true); // Clock pointers
            nodeTVback1->setVisible(false); // TV images
            nodeTVback2->setVisible(false); // TV images
            nodeTVback3->setVisible(false); // TV images
            } else {
            /// Hide Clock:
            nBody->setVisible(false);        // Clock body
            nodeClock->setVisibility(false); // Clock pointers
            nodeTVback1->setVisible(true);   // TV images
            nodeTVback2->setVisible(true);   // TV images
            nodeTVback3->setVisible(true);   // TV images
            };



            if(!doAnimations){
            /// Hide TV and Clock:
            nBody->setVisible(false);        // Clock body
            nodeClock->setVisibility(false); // Clock pointers
            nodeTVback1->setVisible(false);  // TV images
            nodeTVback2->setVisible(false);  // TV images
            nodeTVback3->setVisible(false);  // TV images
            };










            /// Constant camera rotation on Y axis:
            rotcamera_X = camera[CAMERA_SELECTED]->getRotation().X;
            // rotcamera_Y = camera[CAMERA_SELECTED]->getRotation().Y;
            // rotcamera_Z = camera[CAMERA_SELECTED]->getRotation().Z;

            // rotcamera_X = 0.2;
            rotcamera_Y = 0.2;
            rotcamera_Z = 0.2;

            f32 rotspeed = 0.000010;
            if(SHOW_TYPE == 0){ rotspeed = 0.000005; };
            if(SHOW_TYPE == 1){ rotspeed = 0.000010; };
            if(SHOW_TYPE == 2){ rotspeed = 0.000010; };


            if(rotcameratoleft == true ){ rotcamera_X -= rotspeed * frameDeltaTime; };
            if(rotcameratoleft == false){ rotcamera_X += rotspeed * frameDeltaTime; };
            if(rotcamera_X < -0.2){ rotcamera_X = -0.2; rotcameratoleft = false;    };
            if(rotcamera_X >  0.2){ rotcamera_X =  0.2; rotcameratoleft = true;     };
            vector3df calc1 = vector3df(rotcamera_X, rotcamera_Y, rotcamera_Z);
            camera[CAMERA_SELECTED]->setRotation(vector3df(calc1));
            /*
            if(rotcameratoleft == true ){ rotcamera_Y -= rotspeed * frameDeltaTime; };
            if(rotcameratoleft == false){ rotcamera_Y += rotspeed * frameDeltaTime; };
            if(rotcamera_Y < -0.2){ rotcamera_Y = -0.2; rotcameratoleft = false;    };
            if(rotcamera_Y >  0.2){ rotcamera_Y =  0.2; rotcameratoleft = true;     };
            vector3df calc2 = vector3df(rotcamera_X, rotcamera_Y, rotcamera_Z);
            camera[CAMERA_SELECTED]->setRotation(vector3df(calc2));

            if(rotcameratoleft == true ){ rotcamera_Z -= rotspeed * frameDeltaTime; };
            if(rotcameratoleft == false){ rotcamera_Z += rotspeed * frameDeltaTime; };
            if(rotcamera_Z < -0.2){ rotcamera_Z = -0.2; rotcameratoleft = false;    };
            if(rotcamera_Z >  0.2){ rotcamera_Z =  0.2; rotcameratoleft = true;     };
            vector3df calc3 = vector3df(rotcamera_X, rotcamera_Y, rotcamera_Z);
            camera[CAMERA_SELECTED]->setRotation(vector3df(calc3));
            */

            /// HELP -> http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=4680
            // Get transformation matrix of node:
            irr::core::matrix4 mx;

            mx.setRotationDegrees(vector3df(rotcamera_X, rotcamera_Y, rotcamera_Z));
            // Transform forward vector of camera:
            vector3df frv = vector3df(cameraTarget);
            mx.transformVect(frv);
            // Transform upvector of camera:
            /// vector3df upv = vector3df(0.0, 1.00, 0.25); /// Set the limits in each AXIs

            static irr::f32 Zdegrees = 0.00;
            static irr::f32 Zmax     = 0.20;
            static irr::f32 Zspeed   = 0.00003;
            static bool Zincrease    = false;

            if(SHOW_TYPE==0){ Zmax = 0.20; Zspeed = ((0.00006-0.00003)*((irr::f32)rand()/RAND_MAX))+0.00003; }; // Random speed float - Between 0.00003 & 0.00006;
            if(SHOW_TYPE==1){ Zmax = 0.25; Zspeed = ((0.00012-0.00006)*((irr::f32)rand()/RAND_MAX))+0.00006; }; // Random speed float - Between 0.00006 & 0.00012
            if(SHOW_TYPE==2){ Zmax = 0.35; Zspeed = ((0.00042-0.00012)*((irr::f32)rand()/RAND_MAX))+0.00012; }; // Random speed float - Between 0.00012 & 0.00042

            if(Zincrease==true ){ Zdegrees += Zspeed * frameDeltaTime; };
            if(Zincrease==false){ Zdegrees -= Zspeed * frameDeltaTime; };
            if(Zdegrees>=  Zmax){ Zdegrees=  Zmax; Zincrease=false; };
            if(Zdegrees<= -Zmax){ Zdegrees= -Zmax; Zincrease=true;  };

            vector3df upv = vector3df(0.0, 1.00, Zdegrees); /// Set the max angle limits for each AXIs
            mx.transformVect(upv);
            // set camera:
            camera[CAMERA_SELECTED]->setUpVector(upv); // Set up vector of camera
            camera[CAMERA_SELECTED]->setTarget(cameraTarget+frv); // Set target of camera (look at point) (The "offset" corrects this)
            camera[CAMERA_SELECTED]->updateAbsolutePosition();








            /// texts[28]->setText(L"");
            stringw taa = L"";
            // Display texts song info:
            if(music){ // (Audiere) Var: doAnimations, cant be here!!!
            // if(music.GetStatus() == sf::Sound::Playing){ // SMFL
            // L"Music: "; selectedMusicInfos += musicListNamesOnly[musicID].c_str(); // Get song name (full path) + GUI Buttons array starts at 100
            stringw lyricstxt = L"";
            if(thisMusicHasLyrics){ lyricstxt += L"Lyrics: Yes"; } else { lyricstxt += L"Lyrics: No"; };
            /// Apply all music infos texts:
            texts[28]->setText(lyricstxt.c_str());



            /// Changing from frames, to seconds:
            songFullTime = music->getLength()/frate;   // (current samples/rate) Retrieves number in seconds, ex: 1756785
            songCurrentTime = music->getPosition()/frate; // (last samples/rate) Retrieves number in seconds, ex: 1756785


            /// Position Scrollbar Controller:
            /// ------------------------------------------->>>

            if(userDragging_positionScroolbar == 1){
            int sp = musicPositionScrollbar->getPos();
            musicPositionScrollbar->setPos(sp);
            music->setPosition(sp);
            userDragging_positionScroolbar = 0;
            lyricsFadeout = true;
            for(int i=0; i<5; i++){ doingCamTransOnce[i]=true; };
            doEFF = true;           /// For effect
            doEFFrand = false;      /// For effect
            ppeffectsEnable = true; /// For effect
            ppEffectsRand = 0;      /// 0 == Do only grey effect now
            };
            musicPositionScrollbar->setPos(music->getPosition());

            /// ------------------------------------------->>>



            int mil1 = songFullTime-(songFullTime-songCurrentTime); /// Gets the time is seconds (Not in milliseconds!)
            int mil2 = songFullTime;                   /// Gets the time is seconds (Not in milliseconds!)

            // int milliseconds1 =  mil1;
            int seconds1 =  mil1%60;
            int minutes1 = (mil1/60)%60;
            int hours1   = (mil1/60/60)%24;
            /// int day1     =  mil1/60/60/24;

            int seconds2 =  mil2%60;
            int minutes2 = (mil2/60)%60;
            int hours2   = (mil2/60/60)%24;
            /// int day2     =  mil2/60/60/24;


            /*

            int mil11 = songFullTime-(songFullTime-songCurrentTime); /// Gets the time is seconds (Not in milliseconds!)
            int mil22 = songFullTime;                   /// Gets the time is seconds (Not in milliseconds!)

            int seconds11 =  mil11%600;
            int minutes11 = (mil11/60)%60;
            int hours11   = (mil11/60/60)%24;
            int day11     =  mil11/60/60/24;

            int seconds22 =  mil22%60;
            int minutes22 = (mil22/60)%60;
            int hours22   = (mil22/60/60)%24;
            int day22     =  mil22/60/60/24;
            */



            /// Changing from frames, to seconds:
            // songFullTime = music->getLength()/frate;   // Retrieves number in seconds, ex: 1756785
            // songCurrentTime = music->getPosition()/frate; // Retrieves number in seconds, ex: 1756785

            // 1000 / (sample rate / number of samples) = milliseconds
            // Retrieves number in seconds, ex: 1756785

            /// https://www.audio-forums.com/threads/samples-to-milliseconds-conversion.9552/


            // float a1 = ((mil1)/frate)*1000;

            /// For instance 128 samples in seconds = 128/44100 = 0,0029 seconds = 2,9ms (That's assuming your samplerate is 44100)
            /// Frame length (in ms) = (samples per frame / sample rate (in hz)) * 1000

            // int tot = samples*a1;

            // float a1 = (music->getPosition()/frate)/1000;
            // double b1 = 1000/a1;
            // cout <<  a1 << endl;





            // Prepare to display (first, we need to convert INT to string):
            std::string h1 = convertIntToString(hours1);
            std::string m1 = convertIntToString(minutes1);
            std::string s1 = convertIntToString(seconds1);
            if(h1.size()<=1){h1="0"+h1;}; if(m1.size()<=1){m1="0"+m1;}; if(s1.size()<=1){s1="0"+s1;};
            taa = h1.c_str(); taa+=":"; taa+=m1.c_str(); taa+=":"; taa+=s1.c_str(); // Ready formated time first line.

            // Prepare to display (first, we need to convert INT to string):
            stringw tbb;
            std::string h2 = convertIntToString(hours2);
            std::string m2 = convertIntToString(minutes2);
            std::string s2 = convertIntToString(seconds2);
            if(h2.size()<=1){h2="0"+h2;}; if(m2.size()<=1){m2="0"+m2;}; if(s2.size()<=1){s2="0"+s2;};
            tbb = h2.c_str(); tbb+=":"; tbb+=m2.c_str(); tbb+=":"; tbb+=s2.c_str(); // Ready formated time second line.

            /// Diplay song times: (add the 2 time lines created above)
            // stringw tss = L"Position: "; tss+=taa; tss+=L" - "; tss+=tbb;
            stringw tss = L""; tss+=taa; tss+=L" - "; tss+=tbb;
            texts[5]->setText(tss.c_str());



            /// Always select camera N1, until 3 seconds of the START of the song:
            if(songCurrentTime<=5 && randomCamera==true){
            CAMERA_SELECTED = 1;
            keepcam = 0;
            if(doingCamTransOnce[1]==true){ doCameraTransition=true; doingCamTransOnce[1]=false; };
            };

            /// Always select camera N5, between 10 a 5 seconds BEFORE the song ENDS:
            if(songCurrentTime>=(songFullTime-15) && randomCamera==true){
            CAMERA_SELECTED = 5;
            keepcam = 0;
            if(doingCamTransOnce[5]==true){ doCameraTransition=true; doingCamTransOnce[5]=false; };
            };

            /// Always select camera N1, 5 seconds BEFORE the song ENDS:
            if(songCurrentTime>=(songFullTime-5) && randomCamera==true){
            CAMERA_SELECTED = 1;
            keepcam = 0;
            if(doingCamTransOnce[1]==true){ doCameraTransition=true; doingCamTransOnce[1]=false; };
            };





            /// Do clapping, at 1 second BEFORE the song ENDS:
            if(INTERNALS_SOUNDS && (songCurrentTime>=(songFullTime-1))){ // At 1 seconds BEFORE the song ENDS, do applause.
            soundClaps->play();
            };

            }; /// Ends: if(music)



            // Get current time, from the above code.
            wstring currentTime = taa.c_str();

            /// Diplay lyrics:

            for(int r=0; r<linesCounter; ++r){
            wstring starTime = arrayLYRICS[r][0];
            wstring endTime  = arrayLYRICS[r][1];

            if(currentTime >= starTime && currentTime < endTime){ /// TIME MATCH! - Comparison between 2 wstring (MUST be without wstring.c_str())
            fulllines = arrayLYRICS[r][2];
            lyricsFadeout = false;
            };

            if(currentTime >= endTime){  /// Comparison between 2 wstring (MUST be without wstring.c_str())
            lyricsFadeout = true;
            };

            };

            /// Display lyrics string:
            texts[7]->setOverrideColor(video::SColor(255, FONT_COLOR_RED, FONT_COLOR_GREEN, FONT_COLOR_BLUE)); // Set the new lyrics font color
            texts[7]->setText(fulllines.c_str());

            if(lyricsFadeout){
            lirFadeOut -= 0.25*frameDeltaTime;
            texts[7]->setOverrideColor(video::SColor(lirFadeOut, FONT_COLOR_RED, FONT_COLOR_GREEN, FONT_COLOR_BLUE));
            if(lirFadeOut<=10){
            lyricsFadeout = false;
            lirFadeOut = 255;
            fulllines = L"";
            };
            };







            if(!doAnimations){
            lyricsFadeout = true;
            };





            /// TEXTS infos are: 0 = Music Infos, 1= Lyrics file content, 2 = Help, 3 = About, 4 = Nothing
            stringw stags;

            if(infos == 0){
            // stags += L"\n\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"SYSTEM\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"Irrlicht v."; stags += device->getVersion(); stags += L"\n";
            stags += L"Video Driver: "; stags += driver->getName(); stags += L"\n";
            stags += L"Audiere v."; stags += audiere::GetVersion(); stags += L"\n"; // Get audiere version
            stags += L"Audio Device: "; if(audiereAudioDevice){ stags += audiereAudioDevice->getName(); }; stags += L"\n\n";   // Get audiere device
            // stags += L"Midi Device: ";  if(audiereMidiDevice) { stags += audiereMidiDevice->getName();  }; stags += L"\n\n"; // Get audiere device
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"3D SCENE\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            FRAMES_PER_SECOND = driver->getFPS();
            stags += L"FPS: "; stags += FRAMES_PER_SECOND; stags += L" (";
            stringw vsync;
            if(VSYNC==true){ vsync = L"On"; } else { vsync = L"Off"; };
            stags += L"Vertical Sync: "; stags += vsync; stags += L")\n";
            stags += L"Stage Lights: "; stags += stagelights; stags += L"\n";
            // int dlc = driver->getDynamicLightCount();
            // stags += L"Dynamic Lights: "; stags += dlc; stags += L"\n";
            stringw effect;
            if(ppeffectsEnable == true){ effect = ppEffectName[ppEffectsRand].c_str(); } else { effect = L"None"; };
            stags += L"Post Effect: "; stags += effect.c_str(); stags += L" \n";
            stags += L"Camera: "; stags += CAMERA_SELECTED; stags += L" \n";
            /// Decimal places restriction:
            irr::f32 float1 = camera[CAMERA_SELECTED]->getFOV();
            std::stringstream ssin1; ssin1.precision(2); ssin1 << std::fixed << float1; std::string ssout1 = ssin1.str(); /// Float into String
            stags += L"Camera FOV: "; stags += ssout1.c_str(); stags += L" \n";
            irr::f32 float2 = Zdegrees;
            std::stringstream ssin2; ssin2.precision(2); ssin2 << std::fixed << float2; std::string ssout2 = ssin2.str();   /// Float into String
            stags += L"Camera Z Degrees: "; stags += ssout2.c_str(); stags += L" \n\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"MUSIC INFO\n"; // stags += ta; stags += "---"; stags += tb; stags += " \n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"Channels: "; stags += fchannels; stags += L"\n";
            stags += L"Rate: "; stags += frate; stags += L" Hz\n"; // Get audiere version
            stags += L"Format: "; stags += musicExtensionsOnly[musicID].c_str(); stags += L"\n\n";
            for(int i=0; i<(int)tags.size(); i++){ stags += tags[i].c_str(); }; // Get all tags
            };
            if(infos == 1){
            // stags += L"\n\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"LYRICS FILE\n"; // Nothing (Hidden)
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += fullLoadedTexts.c_str(); // Full text from .lyrics file
            };
            if(infos == 2){
            // stags += L"\n\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"LYRICS AND SETTINGS HELP\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"(1) Musics and lyrics:\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"The lyrics file is a simply text file and must have the same name of the music file.\n";
            stags += L"The \".TXT\" or \".SRT\" and \".MP3\" (or any other type of music file) must be inside the same directory.\n\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"(2) Defining user settings in the lyrics file:\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"- \"Band:\"             -- options are -->  \"Man\", \"Woman\", \"Pop\", \"Traditional\", \"Rock\", \"Dj\", \"Drummers\", \"Santa\".\n";
            stags += L"- \"Show:\"            -- options are -->  \"Slow\", \"Normal\", \"Fast\".\n";
            stags += L"- \"LyricsColor:\"   -- options are -->  \"White\", \"Red\", \"Green\", \"Blue\", \"Yellow\", \"Purple\".\n";
            stags += L"- \"LyricsSize:\"    -- options are -->  \"1\" to \"6\".\n";
            stags += L"- \"Sounds:\"         -- options are -->  \"On\", \"Off\".\n";
            stags += L"- \"Clock:\"             -- options are -->  \"On\", \"Off\".\n\n";
            stags += L"Examples:\n\n";
            stags += L"<Band:Pop>\n";
            stags += L"<Show:Fast>\n";
            stags += L"<LyricsColor:Yellow>\n";
            stags += L"<LyricsSize:2>\n";
            stags += L"<Clock:On>\n";
            stags += L"<Sounds:On>\n";
            };
            if(infos == 3){
            // stags += L"\n\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"KEYS (RESUME)\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"Music:\n";
            stags += L"Play/Select Music: Enter, Arrow Up, Arrow Down\n";
            stags += L"Play/Pause Music: Space Bar\n";
            stags += L"Music Volume: 9, 0, Mouse Wheel\n";
            stags += L"Music Pitch: 7, 8\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"Lyrics:\n";
            stags += L"Lyrics Color: L\n";
            stags += L"Lyrics Size: K\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            };
            if(infos == 4){
            // stags += L"\n\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"ABOUT\n"; // stags += ta; stags += "---"; stags += tb; stags += " \n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"IMP - Irrlicht Music Player\n";
            stags += L"Developer: Jorge Rosa - Portugal\n";
            stags += L"Email: jorge.bigarte@gmail.com\n";
            stags += L"Portfolio: http://sites.google.com/site/jorgerosaportfolio\n";
            stags += L"Social: http://www.facebook.com/jorge.bigarte\n";
            stags += L"Project: https://sourceforge.net/projects/irrlichtmp\n";
            stags += L"Developed to ASCR with love!\n\n";
            };
            if(infos == 5){
            stags += L""; // Nothing (Hidden)
            };
            texts[6]->setText(stags.c_str());






            // TEXTS - Band:
            if(BAND_SELECTED == 0){ BAND_STYLE = L"Man";         };
            if(BAND_SELECTED == 1){ BAND_STYLE = L"Woman";       };
            if(BAND_SELECTED == 2){ BAND_STYLE = L"Pop";         };
            if(BAND_SELECTED == 3){ BAND_STYLE = L"Traditional"; };
            if(BAND_SELECTED == 4){ BAND_STYLE = L"Rock";        };
            if(BAND_SELECTED == 5){ BAND_STYLE = L"Dj";          };
            if(BAND_SELECTED == 6){ BAND_STYLE = L"Drummers";    };
            if(BAND_SELECTED == 7){ BAND_STYLE = L"Santa";       };

            stringw btxt;
            btxt += L"Band: "; btxt += BAND_STYLE.c_str(); btxt += L" (B)";
            texts[14]->setText(btxt.c_str()); // border? wordwrap?

            // int i = musicID;
            // guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, video::SColor(255, time%255, time%255, 255));

            // 1 Single, 2 Single Loop, 3 List loop, 4 List Random
            // playMode+=1; if(playMode>4){ playMode = 1; }




            // If music exists, animate the SINGERS:
            if(doAnimations || (music && music->isPlaying())){
            /// Animate singers:
            if(nodeBand){ animation_BP(nodeBand); };
            /// Animate staff:
            animation_SEC(nodeStaff1);
            animation_SEC(nodeStaff2);
            } else {
            if(nodeBand){ animation_BS(nodeBand); };
            };





            // if(INTERNALS_SOUNDS){ soundClaps->play(); }  // Audiere
            // if(doAnimations){
            for(int x=0; x<48; x++){

            /// Anim - Spectators clapping:
            if(soundClaps->isPlaying()){
            animation5(nodeSpectators[x]);
            };

            /// Anim - Spectators dancing:
            if(music){
            if(!soundClaps->isPlaying() && music->isPlaying() && doAnimations && !soundOoo->isPlaying()){
            animation4(nodeSpectators[x]);
            // animation_BALLOONS_UP(nodeBalloons[x]);
            };
            };

            /// Anim - Spectators standby:
            if(!soundClaps->isPlaying() && !doAnimations && !soundOoo->isPlaying()){
            animation6(nodeSpectators[x]);
            // animation_BALLOONS_RESET(nodeBalloons[x]);
            };

            /// Update Spectators to always face BAND:
            if(nodeBand){
            vector3df nodePos = nodeSpectators[x]->getPosition();
            vector3df targetPos = nodeBand->getPosition();
            vector3df diff = -(targetPos - nodePos); // Normal calculation is: vector3df diff = targetPos - nodePos;
            nodeSpectators[x]->setRotation(diff.getHorizontalAngle());
            nodeSpectators[x]->updateAbsolutePosition();
            };
            };





            /// Anim - Free a BALLOON:
            if(doAnimations){
            if(doingOneBalloon == false){ ballon_id = rand()%48; };
            if(nodeBalloons[ballon_id]->getPosition().Y <= 650){
            doingOneBalloon = true;
            animation_BALLOONS_UP(nodeBalloons[ballon_id]);
            };
            if(nodeBalloons[ballon_id]->getPosition().Y > 650){
            doingOneBalloon = false;
            for(int x=0; x<48; x++){ animation_BALLOONS_RESET(nodeBalloons[x]); };
            };
            } else {
            doingOneBalloon = false;
            for(int x=0; x<48; x++){ animation_BALLOONS_RESET(nodeBalloons[x]); };
            };



            /// COURTAINS do...
            open_or_close_COURTAINS();




            /// #1) Calculations for the next EMITTERS quantities:
            static int cqt_90  = 0;
            static int cqt_100 = 0;
            static int cqt_150 = 0;
            static int cqt_200 = 0;
            if(doAnimations){
            cqt_90 +=0.150*frameDeltaTime; if(cqt_90 >=90 ){ cqt_90  = 90;  }; /// <-- Force a smooth gradually start
            cqt_100+=0.150*frameDeltaTime; if(cqt_100>=100){ cqt_100 = 100; }; /// <-- Force a smooth gradually start
            cqt_150+=0.150*frameDeltaTime; if(cqt_150>=150){ cqt_150 = 150; }; /// <-- Force a smooth gradually start
            cqt_200+=0.150*frameDeltaTime; if(cqt_200>=200){ cqt_200 = 200; }; /// <-- Force a smooth gradually start
            } else {
            cqt_90  = 0;
            cqt_100 = 0;
            cqt_150 = 0;
            cqt_200 = 0;
            };
            /// #2) Do constant EMITTERS:
            if(doAnimations){
            em2->setMinParticlesPerSecond(cqt_100);  em2->setMaxParticlesPerSecond(cqt_200);  /// falling stars
            em3->setMinParticlesPerSecond(cqt_90 );  em3->setMaxParticlesPerSecond(cqt_150);  /// right fire
            em4->setMinParticlesPerSecond(cqt_90 );  em4->setMaxParticlesPerSecond(cqt_150);  /// left fire
            em7->setMinParticlesPerSecond(cqt_100);  em7->setMaxParticlesPerSecond(cqt_200);  /// falling flames right
            em8->setMinParticlesPerSecond(cqt_100);  em8->setMaxParticlesPerSecond(cqt_200);  /// falling flames left
            em60->setMinParticlesPerSecond(cqt_90); em60->setMaxParticlesPerSecond(cqt_100); /// disco ball n1 - lights reflexes
            em61->setMinParticlesPerSecond(cqt_90); em61->setMaxParticlesPerSecond(cqt_100); /// disco ball n2 - lights reflexes
            em62->setMinParticlesPerSecond(cqt_90); em62->setMaxParticlesPerSecond(cqt_100); /// disco ball n3 - lights reflexes
            em63->setMinParticlesPerSecond(cqt_90); em63->setMaxParticlesPerSecond(cqt_100); /// disco ball n4 - lights reflexes
            } else {
            em2->setMinParticlesPerSecond(0);   em2->setMaxParticlesPerSecond(0);  /// falling stars
            em3->setMinParticlesPerSecond(0);   em3->setMaxParticlesPerSecond(0);  /// right fire
            em4->setMinParticlesPerSecond(0);   em4->setMaxParticlesPerSecond(0);  /// left fire
            em7->setMinParticlesPerSecond(0);   em7->setMaxParticlesPerSecond(0);  /// falling flames right
            em8->setMinParticlesPerSecond(0);   em8->setMaxParticlesPerSecond(0);  /// falling flames left
            em60->setMinParticlesPerSecond(0);	em60->setMaxParticlesPerSecond(0); /// disco ball n1 - lights reflexes
            em61->setMinParticlesPerSecond(0);	em61->setMaxParticlesPerSecond(0); /// disco ball n2 - lights reflexes
            em62->setMinParticlesPerSecond(0);	em62->setMaxParticlesPerSecond(0); /// disco ball n3 - lights reflexes
            em63->setMinParticlesPerSecond(0);	em63->setMaxParticlesPerSecond(0); /// disco ball n4 - lights reflexes
            };




            /// #1) Calculations for the next EMITTERS quantities:
            static int fqt_100   = 0;
            static int fqt_150   = 0;
            static int emitThis  = 0;
            static int emit_wait = 0;
            static bool finalflames = true;

            /// #2) Do random EMITTERS:
            if(doAnimations){
            fqt_100+=0.150*frameDeltaTime; if(fqt_100>=100){ fqt_100 = 100; }; /// <-- Force a smooth gradually start
            fqt_150+=0.150*frameDeltaTime; if(fqt_150>=150){ fqt_150 = 150; }; /// <-- Force a smooth gradually start
            emit_wait += (0.1*frameDeltaTime);
            /// Emitter duration finished:
            if(emit_wait>200 && emit_wait<=900){
            em30->setMinParticlesPerSecond(0); em30->setMaxParticlesPerSecond(0);
            em31->setMinParticlesPerSecond(0); em31->setMaxParticlesPerSecond(0);
            em32->setMinParticlesPerSecond(0); em32->setMaxParticlesPerSecond(0);
            em33->setMinParticlesPerSecond(0); em33->setMaxParticlesPerSecond(0);
            emitThis = 0; /// Reset
            };
            if(emit_wait>900){
            emitThis = (rand()%8)+1; // from (1 to 7)
            if(emitThis==1){ emit_wait=0; fqt_100=0; fqt_150=0; };
            if(emitThis==2){ emit_wait=0; fqt_100=0; fqt_150=0; };
            if(emitThis==3){ emit_wait=0; fqt_100=0; fqt_150=0; };
            if(emitThis==4){ emit_wait=0; fqt_100=0; fqt_150=0; };
            if(emitThis==5){ emit_wait=0; fqt_100=0; fqt_150=0; };
            if(emitThis==6){ emit_wait=0; fqt_100=0; fqt_150=0; };
            if(emitThis==7){ emit_wait=0; fqt_100=0; fqt_150=0; };
            };
             /// Do small stage flames, at 4 seconds BEFORE the song ENDS:
            if(songCurrentTime>=(songFullTime-4) && finalflames == true){ emitThis=7; emit_wait=0; fqt_100=0; fqt_150=0; finalflames=false; };
            };

            if(emitThis == 1){
            em30->setMinParticlesPerSecond(fqt_100); em30->setMaxParticlesPerSecond(fqt_150); /// small stage flames N1 going up
            };
            if(emitThis == 2){
            em31->setMinParticlesPerSecond(fqt_100); em31->setMaxParticlesPerSecond(fqt_150); /// small stage flames N2 going up
            };
            if(emitThis == 3){
            em32->setMinParticlesPerSecond(fqt_100); em32->setMaxParticlesPerSecond(fqt_150); /// small stage flames N3 going up
            };
            if(emitThis == 4){
            em33->setMinParticlesPerSecond(fqt_100); em33->setMaxParticlesPerSecond(fqt_150); /// small stage flames N4 going up
            };
            if(emitThis == 5){
            em30->setMinParticlesPerSecond(fqt_100); em30->setMaxParticlesPerSecond(fqt_150); /// small stage flames N1 going up
            em33->setMinParticlesPerSecond(fqt_100); em33->setMaxParticlesPerSecond(fqt_150);
            };
            if(emitThis == 6){
            em31->setMinParticlesPerSecond(fqt_100); em31->setMaxParticlesPerSecond(fqt_150); /// small stage flames N2 going up
            em32->setMinParticlesPerSecond(fqt_100); em32->setMaxParticlesPerSecond(fqt_150); /// small stage flames N3 going up
            };
            if(emitThis == 7){
            em30->setMinParticlesPerSecond(fqt_100); em30->setMaxParticlesPerSecond(fqt_150); /// small stage flames N1 going up
            em31->setMinParticlesPerSecond(fqt_100); em31->setMaxParticlesPerSecond(fqt_150); /// small stage flames N2 going up
            em32->setMinParticlesPerSecond(fqt_100); em32->setMaxParticlesPerSecond(fqt_150); /// small stage flames N3 going up
            em33->setMinParticlesPerSecond(fqt_100); em33->setMaxParticlesPerSecond(fqt_150); /// small stage flames N4 going up
            };

            if(!doAnimations){ // doAnimations = false
            em30->setMinParticlesPerSecond(0); em30->setMaxParticlesPerSecond(0); /// small stage flames N1 going up
            em31->setMinParticlesPerSecond(0); em31->setMaxParticlesPerSecond(0); /// small stage flames N2 going up
            em32->setMinParticlesPerSecond(0); em32->setMaxParticlesPerSecond(0); /// small stage flames N3 going up
            em33->setMinParticlesPerSecond(0); em33->setMaxParticlesPerSecond(0); /// small stage flames N4 going up
            emitThis  = 0; /// Reset
            emit_wait = 0; /// Reset
            finalflames = true; /// Reset
            };








            /// #1) Calculations for the next EMITTERS quantities:
            static int smqt_150 = 0;
            static int smqt_250 = 0;
            /// If music exists, and song > 40 seconds, and <= 40 seconds BEFORE the song ENDS:
            if(doAnimations && songFullTime>=000040 && (songCurrentTime>=(songFullTime-40))){
            smqt_150+=0.150*frameDeltaTime; if(smqt_150>=150){ smqt_150 = 150; }; /// <-- Force a smooth gradually start
            smqt_250+=0.150*frameDeltaTime; if(smqt_250>=250){ smqt_250 = 250; }; /// <-- Force a smooth gradually start
            } else {
            smqt_150-=0.001*frameDeltaTime; if(smqt_150<=0){ smqt_150 = 0; };     /// <-- Force a smooth gradually ending
            smqt_250-=0.001*frameDeltaTime; if(smqt_250<=0){ smqt_250 = 0; };     /// <-- Force a smooth gradually ending
            };
            /// #2) Do Smoke Machines EMITTERS:
            em50->setMinParticlesPerSecond(smqt_150); em50->setMaxParticlesPerSecond(smqt_250); /// smoke machine N1 - left
            em51->setMinParticlesPerSecond(smqt_150); em51->setMaxParticlesPerSecond(smqt_250); /// smoke machine N2 - right



            /// Do fireworks, at 2 seconds BEFORE the song ENDS:
            if(doAnimations && songCurrentTime>=(songFullTime-2)){ // At 1 seconds BEFORE the song ENDS, do applause.
            em5->setMinParticlesPerSecond(5000); em5->setMaxParticlesPerSecond(5000); // centered fireworks
            } else {
            em5->setMinParticlesPerSecond(0); em5->setMaxParticlesPerSecond(0);    // centered fireworks
            };











            /// If music exists, and music >5 sec to end, do STROBE LIGHT: (Main Flash White Light)
            static irr::f32 strobeRepeat = 0.0;
            if(doAnimations && (SHOW_TYPE == 1 || SHOW_TYPE == 2) && songCurrentTime<(songFullTime-6)){
            static irr::f32 wait = 0.0;
            int k = 0;
            if(SHOW_TYPE == 1){ k = rand()%1500; }; // from (0 to 1999) - More frequency
            if(SHOW_TYPE == 2){ k = rand()%3000; }; // from (0 to 3999) - Less frequency
            if(k==1000 && strobeRepeat==0.0){ strobeRepeat = 200.0; };
            light_ML1->setVisible(true); /// Turn ON main colored light
            light_FL1->setVisible(false);
            if(strobeRepeat>0.0) {
            if(wait< 5.0){ light_ML1->setVisible(false); light_FL1->setVisible(false); };
            if(wait>=5.0){ light_ML1->setVisible(false); light_FL1->setVisible(true);  };
            wait += (0.15*frameDeltaTime); if(wait>=10.0){ wait = 0.0; };
            strobeRepeat -= (0.20*frameDeltaTime); if(strobeRepeat<=0.0){ strobeRepeat = 0.0; };
            };
            } else {
            strobeRepeat = 0.0;
            light_ML1->setVisible(true);
            light_FL1->setVisible(false);
            };


            /// If NOT music exists, hide these lights:
            if(!doAnimations){
            light_ML1->setVisible(false);
            light_FL1->setVisible(false);
            };




            /// 1) Do spot light rotations: (left - right - left ... Y axis)
            /*
            static float rotX = 160.0;
            static float rotY = 0.0;
            static bool rotYIncrease = false;
            if(rotYIncrease==false){ rotY-=(0.015*frameDeltaTime); };
            if(rotYIncrease==true ){ rotY+=(0.015*frameDeltaTime); };
            if(rotY<= -10){ rotY= -10; rotYIncrease = true;  };
            if(rotY>=  10){ rotY=  10; rotYIncrease = false; };
            // rotX+=(0.150*frameDeltaTime);
            // std::cout << "rotY: " << rotY << std::endl;
            /// 2) Do spot light rotations: (up - down ... X axis)
            if(doAnimations){
            rotX+=(0.150*frameDeltaTime);
            } else {
            rotX = 160.0;
            };
            /// 3) Apply to the light both rotations:
            light_SP1->setRotation(vector3df(rotX,rotY,0.0)); // X90 = Middle Stage & Y90 = left, Y-90 = right
            */




            /// Show lights?...
            if(doAnimations){
            nodeFocusFloor1->setVisible(true);
            nodeFocusFloor2->setVisible(true);
            nodeFocusFloor3->setVisible(true);
            nodeFocusSmokeLight1->setVisible(true);
            nodeFocusSmokeLight2->setVisible(true);
            nodeFocusSmokeLight3->setVisible(true);
            light_SP1->setVisible(false);
            smgr->setAmbientLight(video::SColorf(0.50,0.50,0.50,0.0)); // Ambient Light. SColorf == r,g,b,a
            } else {
            nodeFocusFloor1->setVisible(false);
            nodeFocusFloor2->setVisible(false);
            nodeFocusFloor3->setVisible(false);
            nodeFocusSmokeLight1->setVisible(false);
            nodeFocusSmokeLight2->setVisible(false);
            nodeFocusSmokeLight3->setVisible(false);
            light_SP1->setVisible(true);
            smgr->setAmbientLight(video::SColorf(0.50,0.50,0.50,0.0)); // Ambient Light. SColorf == r,g,b,a
            };


            /// If music exists, and music >5 sec to end, do LASER LIGHTS:
            if(doAnimations && songCurrentTime<(songFullTime-6)){
             /// Show/Hide left LASER LIGHTS:
            if(SHOW_TYPE == 0){ for(int i=0; i<8; i++){ if(nodeLaserA[i]){ nodeLaserA[i]->setVisible(false); }; }; };
            if(SHOW_TYPE == 1){ for(int i=0; i<8; i++){ if(nodeLaserA[i]){ nodeLaserA[i]->setVisible(true);  }; }; };
            if(SHOW_TYPE == 2){ for(int i=0; i<8; i++){ if(nodeLaserA[i]){ nodeLaserA[i]->setVisible(true);  }; }; };
            /// Show/Hide right LASER LIGHTS:
            if(SHOW_TYPE == 0){ for(int i=0; i<8; i++){ if(nodeLaserB[i]){ nodeLaserB[i]->setVisible(false); }; }; };
            if(SHOW_TYPE == 1){ for(int i=0; i<8; i++){ if(nodeLaserB[i]){ nodeLaserB[i]->setVisible(true);  }; }; };
            if(SHOW_TYPE == 2){ for(int i=0; i<8; i++){ if(nodeLaserB[i]){ nodeLaserB[i]->setVisible(true);  }; }; };
            } else {
            /// Hide all LASER LIGHTS:
            for(int i=0; i<8; i++){ if(nodeLaserA[i]){ nodeLaserA[i]->setVisible(false); }; };
            for(int i=0; i<8; i++){ if(nodeLaserB[i]){ nodeLaserB[i]->setVisible(false); }; };
            };



            /// If music exists, do COLUMNS FAKE LIGHTS:
            if(doAnimations){
            irr::f32 wait = 40;
            if(SHOW_TYPE==0){ wait = 200; };
            if(SHOW_TYPE==1){ wait = 60;  };
            if(SHOW_TYPE==2){ wait = 40;  };
			if(columns_light_speed > wait){
            /// Flashing lights:
			// Reset all lights to hidden:
			for(int i=0; i<48; i++){ light[i]->setVisible(false); };  // Hides all fake lights

            // Choose random column to visible:
            int k = rand()%20; // from (0 to 19)

            if(k == 0){ /// 1st column lights on:
			for ( int i=0; i<6; i++   ){ stagelights = i+1; light[i]->setVisible(true); }; };
            if(k == 1){ /// 2nd column lights on:
			for ( int i=6; i<12; i++  ){ stagelights = i+1; light[i]->setVisible(true); }; };
            if(k == 2){ /// 3rd column lights on:
            for ( int i=12; i<18; i++ ){ stagelights = i+1; light[i]->setVisible(true); }; };
            if(k == 3){ /// 4th column lights on:
            for ( int i=18; i<24; i++ ){ stagelights = i+1; light[i]->setVisible(true); }; };
            if(k == 4){ /// 5th column lights on:
            for ( int i=24; i<30; i++ ){ stagelights = i+1; light[i]->setVisible(true); }; };
            if(k == 5){ /// 6th column lights on:
            for ( int i=30; i<36; i++ ){ stagelights = i+1; light[i]->setVisible(true); }; };
            if(k == 6){ /// 7th column lights on:
            for ( int i=36; i<42; i++ ){ stagelights = i+1; light[i]->setVisible(true); }; };
            if(k == 7){ /// 8th column lights on:
            for ( int i=42; i<48; i++ ){ stagelights = i+1; light[i]->setVisible(true); }; };
            if(k == 8){ /// all left columns lights on:
            for ( int i=0; i<24; i++  ){ stagelights = i+1; light[i]->setVisible(true); }; };
            if(k == 9){ /// all right columns lights on:
            for ( int i=24; i<48; i++ ){ stagelights = i+1; light[i]->setVisible(true); }; };
            if(k == 10){ /// all left and right columns lights on:
            for ( int i=0; i<48; i++  ){ stagelights = i+1; light[i]->setVisible(true); }; };
            if(k >= 11){ /// only eight random lights at a time, are on:
            int l1 = rand()%48; // from (0 to 47)
            int l2 = rand()%48; // from (0 to 47)
            int l3 = rand()%48; // from (0 to 47)
            int l4 = rand()%48; // from (0 to 47)
            int l5 = rand()%48; // from (0 to 47)
            int l6 = rand()%48; // from (0 to 47)
            int l7 = rand()%48; // from (0 to 47)
            int l8 = rand()%48; // from (0 to 47)
            light[l1]->setVisible(true);
            light[l2]->setVisible(true);
            light[l3]->setVisible(true);
            light[l4]->setVisible(true);
            light[l5]->setVisible(true);
            light[l6]->setVisible(true);
            light[l7]->setVisible(true);
            light[l8]->setVisible(true);
            stagelights = 8;
            };
			columns_light_speed = 0;
			};
			/// The speed of the lights:
            columns_light_speed += (SPEED_LIGHTS*frameDeltaTime);
            } else {
            /// All lights are off:
            for(int i=0; i<48; i++){ light[i]->setVisible(false); }; // Hides all the fake lights
            stagelights = 0;
            };






            /// If music exists, do MAIN COLORED LIGHT:
            if(doAnimations){
            irr::f32 wait = 50.0f;
            if(SHOW_TYPE == 0){ wait = 100.0f; };
            if(SHOW_TYPE == 1){ wait = 60.0f;  };
            if(SHOW_TYPE == 2){ wait = 50.0f;  };

            /// Main Colored light:
            if(delay > wait){
            delay = 0.0f;
            /// Animated light: ( Change to a random color... )
            irr::f32 red   = ((1.0-0.0)*((float)rand()/RAND_MAX))+0.0; // from (0.0 to 1.0)
            irr::f32 green = ((1.0-0.0)*((float)rand()/RAND_MAX))+0.0; // from (0.0 to 1.0)
            irr::f32 blue  = ((1.0-0.0)*((float)rand()/RAND_MAX))+0.0; // from (0.0 to 1.0)
            data_ML1.AmbientColor  = video::SColorf(red,green,blue,0.0);
            data_ML1.SpecularColor = video::SColorf(red,green,blue,0.0);
            data_ML1.DiffuseColor  = video::SColorf(red,green,blue,0.0);
            light_ML1->setLightData(data_ML1); // Apply data to the light
            /// Transform SColorf to SColor and apply it to the shadows: (Great, but... NOT great visually)
            // video::SColor scolor = data_ML1.AmbientColor.toSColor();
            // smgr->setShadowColor(video::SColor(30,scolor.getRed(),scolor.getGreen(),scolor.getBlue())); // a (0 = transparent | 255 = opaque), r, g, b
            /// Animated light: ( ... OR... Change to a pre-defined color... )
            static bool do_my_color = false;
            irr::f32 pred   = 0.0; // from (0.0 to 1.0)
            irr::f32 pgreen = 0.0; // from (0.0 to 1.0)
            irr::f32 pblue  = 0.0; // from (0.0 to 1.0)
            int fixed_color = rand()%50; // from (0 to 49)
            if(fixed_color== 5){ pred = 1.0; pgreen = 0.0; pblue = 0.0; do_my_color = true; }; // red
            if(fixed_color==10){ pred = 0.0; pgreen = 1.0; pblue = 0.0; do_my_color = true; }; // green
            if(fixed_color==15){ pred = 0.0; pgreen = 0.0; pblue = 1.0; do_my_color = true; }; // blue
            if(fixed_color==20){ pred = 1.0; pgreen = 1.0; pblue = 0.0; do_my_color = true; }; // yellow
            if(fixed_color==25){ pred = 1.0; pgreen = 0.0; pblue = 1.0; do_my_color = true; }; // pink
            if(fixed_color==30){ pred = 1.0; pgreen = 0.5; pblue = 0.0; do_my_color = true; }; // orange
            if(fixed_color==35){ pred = 0.0; pgreen = 1.0; pblue = 1.0; do_my_color = true; }; // cyan
            if(fixed_color==40){ pred = 1.0; pgreen = 1.0; pblue = 1.0; do_my_color = true; }; // white
            if(do_my_color == true){
            do_my_color = false;
            data_ML1.AmbientColor  = video::SColorf(pred,pgreen,pblue,0.0);
            data_ML1.SpecularColor = video::SColorf(pred,pgreen,pblue,0.0);
            data_ML1.DiffuseColor  = video::SColorf(pred,pgreen,pblue,0.0);
            light_ML1->setLightData(data_ML1); // Apply data to the light
            };
            };
            delay += (0.1*frameDeltaTime);
            } else { // doAnimations = false (set defaults)
            delay = 0.0f;
            };





            /// Set ambient lights colors, 5 seconds BEFORE the song ENDS:
            if(songCurrentTime>=(songFullTime-5)){
            /// All fake lights are on, between 5 and 2 seconds BEFORE the song ENDS:
            for(int i=0; i<48; i++){ light[i]->setVisible(true); }; stagelights = 48;
            /// Animated light: ( Change color to white )
            data_ML1.AmbientColor  = video::SColorf(1.0,1.0,1.0,0.0);
            data_ML1.SpecularColor = video::SColorf(1.0,1.0,1.0,0.0);
            data_ML1.DiffuseColor  = video::SColorf(1.0,1.0,1.0,0.0);
            light_ML1->setLightData(data_ML1); // Apply data to the light
            /// Ambient light:
            smgr->setAmbientLight(video::SColorf(1.0,1.0,1.0,0.0)); // Ambient Light. SColorf == r,g,b,a
            };







            /*
            /// Add Colors to the bill lights:
            for(int i=0; i<48; i++){ // Loop trought lights (6 Lights X 8 columns = 48 Lights)
            light[i]->setVisible(true);
            int mc = light[i]->getMaterialCount();
            for(int x=0; i<mc; x++){
            light[i]->getMaterial(x).Shininess = 0.0f; // Set size of specular highlights
            light[i]->getMaterial(x).ColorMaterial = 0;
            light[i]->getMaterial(x).ColorMask = 0;
            light[i]->getMaterial(x).AmbientColor  = video::SColor(0,0,0,0); // video::SColor(0,0.0,0.0,0.0)
            light[i]->getMaterial(x).SpecularColor = video::SColor(0,0,0,0); // video::SColor(0,0.0,0.0,0.0)
            light[i]->getMaterial(x).DiffuseColor  = video::SColor(0,0,0,0);
            light[i]->getMaterial(x).EmissiveColor = video::SColor(150,255,255,150);
            };
            };
            */








            /// If music exists, do PHOTO FLASHES:
            flashes_light_speed += 1.5*frameDeltaTime;

            flashwait += 1.5*frameDeltaTime;
            if(flashwait>=40){
            for(int i=0; i<48; i++){ flash[i]->setVisible(false); }; // Hides all photo flashes
            flashwait = 0;
            };

            static irr::f32 flwait = 0;
            if(doAnimations){
            if(SHOW_TYPE == 0){ flwait = 3000.0; };
            if(SHOW_TYPE == 1){ flwait = 1000.0; };
            if(SHOW_TYPE == 2){ flwait =  500.0; };
            if(flashes_light_speed > flwait){
            flashes_light_speed = 0;
            randomflash = rand()%48; // from (0 to 47)
            flash[randomflash]->setVisible(true); // flash visible
            };
            };

            /// This consumes LOTS of CPU, so its disabled...
            /// if(flash[randomflash]->isVisible()){ animation8(nodeSpectators[randomflash]); };








            /// Animate the logotipo...
            /*
            static int logoChange = 0;
            logoChange += 0.1*frameDeltaTime;
            if(logoChange>=100 ){ image[0] };
            if(logoChange>=200 ){ image[1] };
            if(logoChange>=300 ){ image[2] };
            if(logoChange>=1000){ logoChange = 0; };
            std::cout << "logoChange: " << logoChange << std::endl;
            */



            /// OnResize window, do...
            int sw = driver->getScreenSize().Width;
            int sh = driver->getScreenSize().Height;
            driver->OnResize(core::dimension2d<irr::u32>(sw,sh));

            /// Update globals:
            SCREEN_WIDTH  = sw;
            SCREEN_HEIGHT = sh;

            /// OnResize main widow, update camera ratio:
            camera[CAMERA_SELECTED]->setAspectRatio((float)sw/(float)sh);

            /// OnResize main widow, update listbox position: (Songs list)
            musicListBox->setRelativePosition(core::rect<s32>(10,300,165,sh-20));
            /// Keep constant focus REMOVE on Listbox: (So its own ListBox keys events dont mess with other IMP keys!)
            guienv->removeFocus(musicListBox); // Opposite is: guienv->setFocus(musicListBox);

            /// OnResize main widow, update 2D Graphics positions: (Logotipo)
            image[0]->setRelativePosition(core::position2d<int>(sw-145,10));
            // image[1]->setRelativePosition(core::position2d<int>(sw-145,10));
            // image[2]->setRelativePosition(core::position2d<int>(sw-145,10));

            /// OnResize main widow, update Display Infos: Music Tags, Help, About, Nothing.
            rect<int> post6 = texts[6]->getRelativePosition();
            texts[6]->setRelativePosition(core::rect<s32>(post6.UpperLeftCorner.X, post6.UpperLeftCorner.Y, sw, sh));

            /// OnResize main widow, update text infos to new height:
            // rect<int> post14 = texts[14]->getRelativePosition();
            // texts[14]->setRelativePosition(core::rect<s32>(post14.UpperLeftCorner.X, post14.UpperLeftCorner.Y, sw, sh));


            /// OnResize main widow, update text position: (Lyrics texts)
            int fts = texts[7]->getTextHeight(); /// Get current lyrics font size (height)
            texts[7]->setRelativePosition(core::rect<s32>(50,(sh-fts)-15,sw-50,sh-10));

            /// OnResize main widow, update text position: (PAUSED texts: Show intermission)
            texts[12]->setRelativePosition(core::rect<s32>(0,0,sw,sh));

            /// OnResize main widow, update text position: (Loading texts: Please standby, Show will continue in moments)
            texts[20]->setRelativePosition(core::rect<s32>(0,0,sw,sh));

            /// OnResize main widow, update text position: (Loading texts: Loading music, Loading lyrics)
            texts[25]->setRelativePosition(core::rect<s32>(0,sh-130,sw,sh));


            /// OnResize main widow, keep win2 always in the visible screen area:
            core::rect<s32> posW2 = win2->getRelativePosition();
            // if(posW2.UpperLeftCorner.X  < 185){ posW2.UpperLeftCorner.X = 185; }; <-- Only if we want it to collide with the win1
            if(posW2.UpperLeftCorner.X  < 5){ posW2.UpperLeftCorner.X = 5; };
            if(posW2.UpperLeftCorner.Y  < 5){ posW2.UpperLeftCorner.Y = 5; };
            if(posW2.UpperLeftCorner.X  > (int)SCREEN_WIDTH-400-5 ){ posW2.UpperLeftCorner.X = SCREEN_WIDTH-400-5; };
            if(posW2.UpperLeftCorner.Y  > (int)SCREEN_HEIGHT-65-5 ){ posW2.UpperLeftCorner.Y = SCREEN_HEIGHT-65-5; };
            win2->setRelativePosition(core::rect<s32>(posW2.UpperLeftCorner.X, posW2.UpperLeftCorner.Y, posW2.UpperLeftCorner.X+400, posW2.UpperLeftCorner.Y+65));
            win2->getParent()->bringToFront(win2);

            /// OnResize main widow, update internal window:
            win1->setRelativePosition(core::rect<s32>(5,5,180,sh-5));
            win1->getParent()->bringToFront(win1); /// <-- This way its ListBox child, is clickable (else may fail clicking on the last item in the list)


            /// OnResize main widow, update text position: (Music timmings texts)
            /// int mts = texts[4]->getTextWidth(); /// Get current lyrics font size (height) - Ok
            /// rect<int> post4 = texts[4]->getRelativePosition(); // First we get the "father" text... - Ok
            /// texts[5]->setRelativePosition(core::rect<s32>(post4.UpperLeftCorner.X + mts + 152, post4.UpperLeftCorner.Y,sw-50,sh-10)); // New position (00:00:00)  - Ok
            /// musicPositionScrollbar->setRelativePosition(position2di(post4.UpperLeftCorner.X + mts + 0, post4.UpperLeftCorner.Y + 2));         // New position (Scrollbar) - Ok
            // texts[5]->setRelativePosition(core::rect<s32>(660,15,sw-50,sh-10)); // New position (00:00:00)
            // musicPositionScrollbar->setRelativePosition(position2di(240, 17));  // New position (Scrollbar)

