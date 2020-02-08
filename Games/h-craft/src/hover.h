// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef HOVER_H
#define HOVER_H

#include <string>

class TiXmlElement;

struct Hover
{
    Hover();
    void ReadFromXml(const TiXmlElement * settings_);

    std::wstring    mName;
    std::string     mModel;
    std::string     mImage; // texture name relative to media folder
    std::wstring    mLockText;
    int             mPointsNeeded;
};

#endif // HOVER_H

