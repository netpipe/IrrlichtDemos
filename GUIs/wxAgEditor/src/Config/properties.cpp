#include "properties.h"

void Properties::load(std::iostream &file)
{
    load((std::ifstream&) file);
}

void Properties::load(std::ifstream &file)
{
    clear();
    while (!file.eof())
    {
        std::string line;
        std::string key;
        std::string value;
        getline(file, line);
        if (line.size() == 0)
        	continue;
        if (line.at(0) == '#' || line.at(0) == '/' || line.at(0) == '[')
        	continue;
        std::string::size_type loc = line.find("=", 0);
        if (loc == std::string::npos)
			continue;
        key = line.substr(0, loc);
        while (key.find(" ", 0) != std::string::npos)
        {
            std::string::size_type spaceloc = key.find(" ", 0);
            key.replace(spaceloc, key.substr(spaceloc+1).size(), key.substr(spaceloc+1));
            key.erase(key.end()-1);
        }
        value = line.substr(loc + 1, std::string::npos);
        while (value.find(" ", 0) != std::string::npos)
        {
            std::string::size_type spaceloc = value.find(" ", 0);
            value.replace(spaceloc, value.substr(spaceloc+1).size(), value.substr(spaceloc+1));
            value.erase(value.end()-1);
        }
        values[key] = value;
        keys.push_back(key);
    }
}

void Properties::save(std::iostream &file)
{
    save((std::ofstream&) file);
}

void Properties::save(std::ofstream &file)
{
	file.clear();
    for (unsigned int i=0; i < keys.size(); ++i)
		file << keys[i] << "=" << values[keys[i]] << std::endl;
}

std::string Properties::getProperty(std::string entry, std::string def)
{
    if (values.count(entry) > 0)
        return values[entry];
    return def;
}

void Properties::setProperty(std::string entry, std::string value)
{
	if (values.find(entry) == values.end())
		keys.push_back(entry);
    values[entry] = value;
}

void Properties::clear()
{
    keys.clear();
    values.clear();
}
