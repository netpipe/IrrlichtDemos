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

#if !defined __SoundManager_h__
#define __SoundManager_h__

#include "cAudio/include/cAudio.h"
#include "Serializable.h"

class SoundManager: public Serializable{
public:
	SoundManager(void);
	~SoundManager(void);


public:
	bool initialize();
	bool finalize(void);

public:
	static cAudio::IAudioManager* manager;
	static cAudio::IAudioSource* menumusic;
	static cAudio::IAudioSource* gamemusic;
	static SoundManager* soundmanager;
	static float musicvolume;
	static float fxvolume;

	void readFrom(Deserializer* id);
	void writeTo(Serializer* is);
	void save();
private:
	unsigned int deviceSelection;
};
#endif

