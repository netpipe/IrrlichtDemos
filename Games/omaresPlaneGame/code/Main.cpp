#include "StdAfx.h"
#include "game.h"



// the game
class CGame Game;

int main(int argc, char* argv[])
{


	// menu game
	Game.menu();
////checks if start on menu is pressed
if(Game.startGame==true)
{
	Game.Init();
	Game.startGame=false;

}
////sounds
/*

AudioDevicePtr audiereDevice;
OutputStreamPtr stream;

    audiereDevice = OpenDevice();
    if (!audiereDevice)
     return 1;

    stream = OpenSound(audiereDevice.get(), "engine1.wav", true);
    if (!stream)
     return 2;

    stream->setRepeat(true);
    stream->setVolume(1.0f); // 50% volume
   // stream->play();
///////////bgm noise

    OutputStreamPtr streambgm;


    streambgm = OpenSound(audiereDevice.get(), "IrrlichtTheme.mp3", true);
    if (!streambgm)
     return 2;

    streambgm->setRepeat(true);
    streambgm->setVolume(1.0f); // 50% volume
   // streambgm->play();

///////////

*/

	// Create scene
	Game.Start();


	// Clean up memory
	Game.Finish();

	return 0;
}
