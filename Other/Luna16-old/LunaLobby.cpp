
#include "Luna.h"

int Luna::lobby() {
    #ifndef NDEBUG
	// falsify the quit flags
	events.quit = false;
	events.login = false;
	// Flush the gui factory
	guienv->clear();
	smgr->clear();
    device->setEventReceiver ( &events );



	smgr->loadScene ( "./data/models/maps/lobby.irr" );
	//smgr->addCameraSceneNode(0, vector3df(20,100, 0), vector3df(0,100,0), -1);
	//smgr->addCameraSceneNodeFPS();

	ITexture *loginWindow = driver->getTexture ( "./data/textures/lobby/loginBg.png" );

	int buttonPosX = ( resolution[0] / 2 ) - 60;
	int buttonPosY = ( resolution[1] / 100 ) * 80;

	IGUIEditBox *uname = guienv->addEditBox ( L"", rect<s32> ( buttonPosX - 30, buttonPosY - 100, buttonPosX + 150, buttonPosY - 80 ), true, 0, -1 );
	IGUIEditBox *pword = guienv->addEditBox ( L"", rect<s32> ( buttonPosX - 30, buttonPosY - 60, buttonPosX + 150, buttonPosY - 40 ), true, 0, -1 );

	pword->setPasswordBox ( true, L'*' );

	// Load the button textures
	ITexture *menuButtonNormal = driver->getTexture ( "./data/textures/lobby/lobbyButtonNormal.png" );
	ITexture *menuButtonDepressed = driver->getTexture ( "./data/textures/lobby/lobbyButtonDepressed.png" );

	IGUIButton *exitButton = guienv->addButton ( rect<s32> ( buttonPosX, buttonPosY + 50, buttonPosX + 120, buttonPosY + 70 ), 0, 101, L"Exit" );
	exitButton->setImage ( menuButtonNormal );
	exitButton->setPressedImage ( menuButtonDepressed );

	IGUIButton *loginButton = guienv->addButton ( rect<s32> ( buttonPosX, buttonPosY + 0, buttonPosX + 120, buttonPosY + 20 ), 0, 102, L"Login" );
	loginButton->setImage ( menuButtonNormal );
	loginButton->setPressedImage ( menuButtonDepressed );

	IGUIButton *optionsButton = guienv->addButton ( rect<s32> ( buttonPosX, buttonPosY + 25, buttonPosX + 120, buttonPosY + 45 ), 0, 103, L"Options" );
	optionsButton->setImage ( menuButtonNormal );
	optionsButton->setPressedImage ( menuButtonDepressed );

	// Developers login box
#ifndef NDEBUG
	IGUIButton *devButton = guienv->addButton ( rect<s32> ( buttonPosX, buttonPosY - 25, buttonPosX + 120, buttonPosY-10 ), 0, 901, L"Dev Login" );
	devButton->setImage ( menuButtonNormal );
	devButton->setPressedImage ( menuButtonDepressed );
#endif

	// Do the text
	guienv->addStaticText ( L"Account Login.", rect<s32> ( buttonPosX - 60, buttonPosY - 148, buttonPosX + 120, buttonPosY - 138 ) );
	guienv->addStaticText ( L"Username:", rect<s32> ( buttonPosX - 30, buttonPosY - 110, buttonPosX + 120, buttonPosY - 100 ) );
	guienv->addStaticText ( L"Password:", rect<s32> ( buttonPosX - 30, buttonPosY - 70, buttonPosX + 120, buttonPosY - 60 ) );

	// Error message buttons
	IGUIButton *conOK;
	IGUIStaticText *conMessage;

	IGUIButton *wrongPWOK;
	IGUIStaticText *wrongPWMessage;

	// Error message flags
	bool conRefOK = false;
	bool wrongPassOK = false;

	while ( device->run() && !events.quit && !events.devLogin )
     {
		if ( conRefOK && events.conRefOK ) {
			conRefOK = events.conRefOK = false;
			conOK->remove();
			conMessage->remove();
			loginButton->setEnabled ( true );
			optionsButton->setEnabled ( true );
			exitButton->setEnabled ( true );
			uname->setEnabled ( true );
			pword->setEnabled ( true );
		}
		if ( wrongPassOK && events.wrongPassOK ) {
			events.wrongPassOK = false;
			wrongPWOK->remove();
			wrongPWMessage->remove();
			loginButton->setEnabled ( true );
			optionsButton->setEnabled ( true );
			exitButton->setEnabled ( true );
			uname->setEnabled ( true );
			pword->setEnabled ( true );
		}



		if ( events.login ) {
			int ret = doLogin ( std::wstring ( uname->getText() ), std::wstring ( pword->getText() ) );
			if ( ret == 0  ){
                events.quit=true;
			    //break;
			   return 1;
			}
			else if ( ret == 1 ) {
				wrongPWMessage = guienv->addStaticText ( L"Incorrect username or password.", rect<s32> ( 0, 0, 130, 10 ) );
				wrongPWOK = guienv->addButton ( rect<s32> ( buttonPosX, resolution[1] / 2, buttonPosX + 120, ( resolution[1]/2 ) + 20 ), 0, 105, L"Wrong Password" );
				wrongPWOK->setImage ( menuButtonNormal );
				wrongPWOK->setPressedImage ( menuButtonDepressed );

				loginButton->setEnabled ( false );
				optionsButton->setEnabled ( false );
				exitButton->setEnabled ( false );

				uname->setEnabled ( false );    pword->setEnabled ( false );

				events.login = false;
				wrongPassOK = true;
			}
			else if ( ret < 0 ) {
				conMessage = guienv->addStaticText ( L"Could not connect to the server, please see the status page", rect<s32> ( 0, 0, 130, 10 ) );
				conOK = guienv->addButton ( rect<s32> ( buttonPosX, resolution[1] / 2, buttonPosX + 120, ( resolution[1]/2 ) + 20 ), 0, 104, L"no server connection" );
				conOK->setImage ( menuButtonNormal );
				conOK->setPressedImage ( menuButtonDepressed );
				loginButton->setEnabled ( false );
				optionsButton->setEnabled ( false );
				exitButton->setEnabled ( false );
				uname->setEnabled ( false );
				pword->setEnabled ( false );
				events.login = false;
				conRefOK = true;
			}
		}
		driver->beginScene ( true, true, SColor ( 0, 0, 0, 0 ) );
		smgr->drawAll();
            driver->draw2DImage ( loginWindow, core::position2d<s32> ( buttonPosX - 65, buttonPosY - 150 ) );
		guienv->drawAll();
		driver->endScene();

        device->sleep ( 10, true );



	}

	return 0;
	#endif
}

