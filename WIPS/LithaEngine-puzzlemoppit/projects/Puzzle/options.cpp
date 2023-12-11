
#include "options.h"
#include "volume.h"

extern bool gridBasedMovement;
extern bool skyEffectsEnabled;
extern bool invertMouseX;
extern bool invertMouseY;

// get stuff used in options menu
VariantMap get_current_config()
{
	VariantMap creationSettings = GetEngine()->GetCreationSettings();
	
	VariantMap settings;
	settings["fullScreen"] = creationSettings["fullScreen"];
	settings["screenWidth"] = creationSettings["screenWidth"];
	settings["screenHeight"] = creationSettings["screenHeight"];
	settings["shadersEnabled"] = creationSettings["shadersEnabled"];
	settings["skyEffects"] = skyEffectsEnabled;
	settings["postProcessingEnabled"]= creationSettings["postProcessingEnabled"];
	settings["windVolume"] = get_wind_volume();
	settings["musicVolume"] = get_music_volume();
	settings["sfxVolume"] = get_sfx_volume();
	settings["gridBasedMovement"] = gridBasedMovement;
	settings["invertMouseX"] = invertMouseX;
	settings["invertMouseY"] = invertMouseY;
	return settings;
}

// apply all things that don't require a restart
void apply_temp_config(VariantMap config)
{
	set_wind_volume(config["windVolume"]);
	set_music_volume(config["musicVolume"]);
	set_sfx_volume(config["sfxVolume"]);
	gridBasedMovement = config["gridBasedMovement"];
	invertMouseX = config["invertMouseX"];
	invertMouseY = config["invertMouseY"];
}

// apply *everything*, even things that do require restart, and then
// restart if necessary. And save new config.
void apply_permanent_config(VariantMap config)
{
	// Load old settings from disk
	io::path fullSettingsPath = GetEngine()->GetSettingsPath();
	VariantMap oldSettings = file::loadsettings(fullSettingsPath);
	
	// settings could not be loaded?
	if (!oldSettings.size())
	{
		NOTE << "settings could not be loaded, so using creation settings.";
		oldSettings = GetEngine()->GetCreationSettings();
	}
	
	// Override old settings with new
	override_variantmap(oldSettings, config);
	
	// Then save.
	if (!file::savesettings(fullSettingsPath, oldSettings))
		WARN << "Could not save settings: " << fullSettingsPath;
	
	// update stuff like volumes
	// (since we *might* not restart)
	apply_temp_config(config);
	
	// Should we restart?
	// (is any settings changed that require a restart? Current state != new config state ?)
	
	VariantMap creationSettings = GetEngine()->GetCreationSettings();
	
	if ( !(config["fullScreen"] == creationSettings["fullScreen"]
			&& config["shadersEnabled"] == creationSettings["shadersEnabled"]
			&& config["skyEffects"] == skyEffectsEnabled
			&& config["postProcessingEnabled"] == creationSettings["postProcessingEnabled"]
			&& config["screenWidth"] == creationSettings["screenWidth"]
			&& config["screenHeight"] == creationSettings["screenHeight"])
			)
	{
		NOTE << "An option was changed that requires the application to restart.";
		// Attempt to restart application.
		GetEngine()->Restart();
	}
}




