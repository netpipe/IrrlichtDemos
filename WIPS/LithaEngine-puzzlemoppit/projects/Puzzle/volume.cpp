
#include "volume.h"

extern ISound *bgAmbientSound;
extern ISound *bgMusic;

s32 windVolume;
s32 sfxVolume;
s32 musicVolume;

void set_wind_volume(s32 amount)
{
	ASSERT( bgAmbientSound );
	windVolume = core::clamp(amount, 0, 5);
	//bgAmbientSound->SetVolume( 0.01166 * windVolume );
	bgAmbientSound->SetVolume( 0.035 * windVolume );
}

void set_sfx_volume(s32 amount)
{
	sfxVolume = core::clamp(amount, 0, 5);
	GetEngine()->GetSoundSystem()->SetGlobalVolume3D( 0.333 * sfxVolume );
}

void set_music_volume(s32 amount)
{
	ASSERT(bgMusic);
	musicVolume = core::clamp(amount, 0, 1);
	bgMusic->SetVolume( 0.1 * musicVolume );
}

s32 get_wind_volume()
{
	return windVolume;
}

s32 get_sfx_volume()
{
	return sfxVolume;
}

s32 get_music_volume()
{
	return musicVolume;
}

void set_volumes_from_settings(VariantMap settings)
{
	if (settings.count("windVolume"))
		set_wind_volume(settings["windVolume"]);
	else
		set_wind_volume(3);
	
	if (settings.count("sfxVolume"))
		set_sfx_volume(settings["sfxVolume"]);
	else
		set_sfx_volume(3);
	
	if (settings.count("musicVolume"))
		set_music_volume(settings["musicVolume"]);
	else
		set_music_volume(0);
}


