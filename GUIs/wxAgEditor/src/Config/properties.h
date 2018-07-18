#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

class Properties
{
    public:
		void load(std::iostream &file);
        void load(std::ifstream &file);
        void save(std::iostream &file);
        void save(std::ofstream &file);
		std::string getProperty(std::string entry, std::string def);
		void setProperty(std::string entry, std::string value);
    private:
        void clear();
		std::map<std::string, std::string> values;
		std::vector<std::string> keys;
};

#endif
