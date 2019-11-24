#include "config.h"

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

    azadiConf.close();

    return 0;
}
