#include <cstdio>
    #include "./AgAudio2/CAudioDevice.h"
    agEngine::audio::CAudioDevice* adevice;

int main ( int argc, char** argv )
{

 agEngine::audio::CAudioDevice* adevice = new agEngine::audio::CAudioDevice();

 agEngine::audio::CAudioSource *test = adevice->createAudioSource( adevice->createAudioStream("./bling.ogg",1));

 adevice->addAudioSource( test);

test->play();

while (1){
 // adevice->playAll();
}

		system("PAUSE");
	return 0;
}


