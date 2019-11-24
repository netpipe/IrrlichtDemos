#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <fstream>

#include "Helpers/defaults.h"
#include "properties.h"
#include "Helpers/types.h"

using namespace std;

class Config
{
	public:
		Config() { instance = this; }

		int load();
		int save();

		static Config* getInstance() { return instance; }
	private:
		static Config* instance;
};

#endif
