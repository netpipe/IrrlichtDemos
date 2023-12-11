#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

using namespace std;

class Properties
{
    public:
        void load(iostream &file);
        void load(ifstream &file);
        void save(iostream &file);
        void save(ofstream &file);
        string getProperty(string entry, string def);
        void setProperty(string entry, string value);
    private:
        void clear();
        map<string, string> values;
        vector<string> keys;
};

#endif
