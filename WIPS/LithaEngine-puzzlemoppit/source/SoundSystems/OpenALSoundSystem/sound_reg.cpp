
#include "sound_reg.h"
#include "OpenALSound2D.h"
#include "OpenALSound3D.h"

std::vector<OpenALSound *> allSounds;
std::vector<OpenALSound2D *> allSounds2D;
std::vector<OpenALSound3D *> allSounds3D;

void register_sound2d(OpenALSound2D *sound)
{
	allSounds.push_back(sound);
	allSounds2D.push_back(sound);
}

void unregister_sound2d(OpenALSound2D *sound)
{
	for (u32 i = 0; i < allSounds.size(); i ++)
	{
		if (allSounds[i] == sound)
		{
			allSounds.erase(allSounds.begin()+i);
			break;
		}
	}
	for (u32 i = 0; i < allSounds2D.size(); i ++)
	{
		if (allSounds2D[i] == sound)
		{
			allSounds2D.erase(allSounds2D.begin()+i);
			break;
		}
	}
}

void register_sound3d(OpenALSound3D *sound)
{
	allSounds.push_back(sound);
	allSounds3D.push_back(sound);
}

void unregister_sound3d(OpenALSound3D *sound)
{
	for (u32 i = 0; i < allSounds.size(); i ++)
	{
		if (allSounds[i] == sound)
		{
			allSounds.erase(allSounds.begin()+i);
			break;
		}
	}
	for (u32 i = 0; i < allSounds3D.size(); i ++)
	{
		if (allSounds3D[i] == sound)
		{
			allSounds3D.erase(allSounds3D.begin()+i);
			break;
		}
	}
}
