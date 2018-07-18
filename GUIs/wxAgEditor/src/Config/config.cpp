#include "config.h"

Config* Config::instance = NULL;

Config::Config()
{ 
	instance = this;
	fields = new Properties();
}

Config::~Config()
{
	delete fields;
}

int Config::load(const std::string &filename)
{
	ifstream configurationFile(filename.c_str(), ios::in);
	if (!configurationFile.is_open())
	{
		return -1;
	}
	fields->load(configurationFile);
	configurationFile.close();

	return 0;
}

int Config::save(const std::string &filename)
{
	ofstream configurationFile(filename.c_str(), ios::out);
    if (!configurationFile.is_open())
    {
        return -1;
    }
	fields->save(configurationFile);
    configurationFile.close();

    return 0;
}
