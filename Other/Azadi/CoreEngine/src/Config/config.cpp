#include "config.h"
#include "../Azadi.h"

Config* Config::instance = NULL;

int Config::load()
{
	ifstream azadiConf(CONFIG_FILE);
	if (!azadiConf.is_open())
	{
		return -1;
	}
	Properties* coreProperties = new Properties();
	coreProperties->load(azadiConf);
	azadiConf.close();

    Azadi::getInstance()->setWorkingDirectory(coreProperties->getProperty("GameDir", "data"));
    Azadi::getInstance()->setExtensions(Bool::parseBool(coreProperties->getProperty("Extensions", "true")));
    Azadi::getInstance()->setShaders(Bool::parseBool(coreProperties->getProperty("Shaders", "true")));
	Azadi::getInstance()->setPostProcessing(Bool::parseBool(coreProperties->getProperty("PostProcessing", "false")));
	Azadi::getInstance()->setAntiAliasing(Bool::parseBool(coreProperties->getProperty("AntiAlias", "false")));
	Azadi::getInstance()->setDelay(Bool::parseBool(coreProperties->getProperty("UseDelay", "false")));
	Azadi::getInstance()->setScreenWidth(LongInt::parseLong(coreProperties->getProperty("ScreenWidth", "1024")));
	Azadi::getInstance()->setScreenHeight(LongInt::parseLong(coreProperties->getProperty("ScreenHeight", "768")));
	Azadi::getInstance()->setFullScreen(Bool::parseBool(coreProperties->getProperty("Fullscreen", "false")));
	Azadi::getInstance()->setTerrain(Bool::parseBool(coreProperties->getProperty("Terrain", "true")));

	delete coreProperties;
	return 0;
}

int Config::save()
{
    ofstream azadiConf(CONFIG_FILE);
    if (!azadiConf.is_open())
    {
        return -1;
    }

    /*==============================================

        Base options

    ===============================================*/

    azadiConf << "[Base options]\n";
    azadiConf << "GameDir = " << Azadi::getInstance()->getWorkingDirectory() << "\n";
    azadiConf << "Extensions = " << Bool::toBool(Azadi::getInstance()->usingExtensions()) << "\n";
    azadiConf << "Shaders = " << Bool::toBool(Azadi::getInstance()->usingShaders()) << "\n";
    azadiConf << "PostProcessing = " << Bool::toBool(Azadi::getInstance()->isPostProcessing()) << "\n";
    azadiConf << "AntiAlias = " << Bool::toBool(Azadi::getInstance()->isAntiAlias()) << "\n";
    azadiConf << "UseDelay = " << Bool::toBool(Azadi::getInstance()->isDelay()) << "\n";
    azadiConf << "ScreenWidth = " << LongInt::toLong(Azadi::getInstance()->getScreenWidth()) << "\n";
    azadiConf << "ScreenHeight = " << LongInt::toLong(Azadi::getInstance()->getScreenHeight()) << "\n";
    azadiConf << "Fullscreen = " << Bool::toBool(Azadi::getInstance()->isFullScreen()) << "\n";
    azadiConf << "Terrain = " << Bool::toBool(Azadi::getInstance()->isTerrainEnabled()) << "\n";

    azadiConf.close();

    return 0;
}
