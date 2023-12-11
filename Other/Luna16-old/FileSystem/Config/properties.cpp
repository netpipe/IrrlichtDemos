#include "properties.h"

void Properties::load(iostream &file)
{
    load((ifstream&) file);
}

void Properties::load(ifstream &file)
{
    clear();
    while (!file.eof())
    {
        string line;
        string key;
        string value;
        getline(file, line);
        if (line.size() == 0)
        	continue;
        if (line.at(0) == '#' || line.at(0) == '/' || line.at(0) == '[')
        	continue;
        string::size_type loc = line.find("=", 0);
        if (loc == string::npos)
			continue;
        key = line.substr(0, loc);
        while (key.find(" ", 0) != string::npos)
        {
            string::size_type spaceloc = key.find(" ", 0);
            key.replace(spaceloc, key.substr(spaceloc+1).size(), key.substr(spaceloc+1));
            key.erase(key.end()-1);
        }
        value = line.substr(loc + 1, string::npos);
        while (value.find(" ", 0) != string::npos)
        {
            string::size_type spaceloc = value.find(" ", 0);
            value.replace(spaceloc, value.substr(spaceloc+1).size(), value.substr(spaceloc+1));
            value.erase(value.end()-1);
        }
        values[key] = value;
        keys.push_back(key);
    }
}

void Properties::save(iostream &file)
{
    save((ofstream&) file);
}

void Properties::save(ofstream &file)
{
    for (unsigned int i=0; i < keys.size(); ++i)
        file << keys[i] << "=" << values[keys[i]];
}

string Properties::getProperty(string entry, string def)
{
    if (values.count(entry) > 0)
        return values[entry];
    return def;
}

void Properties::setProperty(string entry, string value)
{
    values[entry] = value;
}

void Properties::clear()
{
    keys.clear();
    values.clear();
}
