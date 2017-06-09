/*
 * Copyright 2011
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <string>

#include "SoundManager.h"
#include "Serializer.h"
#include "Deserializer.h"

//cAudio::IAudioManager* manager=0;

SoundManager::SoundManager(void) {
	initialize();
}
;
SoundManager::~SoundManager(void) {
	finalize();
}
;

SoundManager* SoundManager::soundmanager = 0;
cAudio::IAudioManager* SoundManager::manager = 0;
cAudio::IAudioSource* SoundManager::menumusic = 0;
cAudio::IAudioSource* SoundManager::gamemusic = 0;

		cAudio::IAudioDeviceList* pDeviceList;

float SoundManager::musicvolume;
float SoundManager::fxvolume;

void SoundManager::save() {
	writeTo(new Serializer("./config/sound.xml"));
}


bool SoundManager::initialize() {
	//cAudio::IAudioManager* manager=cAudio::createAudioManager(false);
	manager = cAudio::createAudioManager(false);
	SoundManager::soundmanager = this;
	if (manager) {
		Deserializer* d = new Deserializer("./config/sound.xml");
		if (d->fileok) {
			readFrom(d);
		} else {
			//Allow the user to choose a playback device
		pDeviceList = cAudio::createAudioDeviceList();
		unsigned int deviceCount = pDeviceList->getDeviceCount();
		cAudio::cAudioString defaultDeviceName = pDeviceList->getDefaultDeviceName();
		for(unsigned int i=0; i<deviceCount; ++i)
		{
			cAudio::cAudioString deviceName = pDeviceList->getDeviceName(i);
			if(deviceName.compare(defaultDeviceName) == 0)
				std::cout << i << "): " << deviceName.c_str() << " [DEFAULT] \n";
			else
				std::cout << i << "): " << deviceName.c_str() << " \n";
		}
			std::cout << std::endl;
			std::cout << "Choose a device by number: ";
			deviceSelection = 1;
			std::cin >> deviceSelection;
			//std::cout << std::endl;
			musicvolume = 1.0f;
			fxvolume = 1.0f;
			std::cout << "saving configs in ./config/sound.xml\ndelete it to rerun this config\n";
			writeTo(new Serializer("./config/sound.xml"));
		}
		//Initialize the manager with the user settings
		manager->initialize(pDeviceList->getDeviceName(deviceSelection).c_str());
		CAUDIO_DELETE pDeviceList;
		pDeviceList = 0;
		// load menumusic
		menumusic = SoundManager::manager->create("bling",
				"media/Sounds/bomber1.ogg", false);

		gamemusic = SoundManager::manager->create("bling",
				"media/Sounds/GENERIC.ogg", false);

	}

}
;
bool SoundManager::finalize(void) {
//Delete all IAudio sounds
manager->releaseAllSources();
//Shutdown cAudio
manager->shutDown();

cAudio::destroyAudioManager(manager);

}
;

void SoundManager::readFrom(Deserializer *d) {
d->beginAttribute("sound");
long tmp;
d->readParameter("driver", tmp);
deviceSelection = tmp;
d->readParameter("musicvolume", musicvolume);
d->readParameter("fxvolume", fxvolume);

}

void SoundManager::writeTo(Serializer *s) {
s->beginAttribute("sound");
long tmp = deviceSelection;
s->writeParameter("driver", tmp);
s->writeParameter("musicvolume", musicvolume);
s->writeParameter("fxvolume", fxvolume);
s->endAttribute();
s->endAttribute();
}

