// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "hover.h"
#include "main.h"
#include "string_table.h"
#include "tinyxml/tinyxml.h"

// --------------------------------------------------------
Hover::Hover()
: mPointsNeeded(0)
{
}

void Hover::ReadFromXml(const TiXmlElement * settings_)
{
    if ( !settings_ )
        return;

    const char * val = settings_->Attribute("name");
    if ( val )
    {
        irr::core::stringw wStr(APP.GetStringTable()->Get(val));
        mName = wStr.c_str();
    }

    val = settings_->Attribute("model");
    if ( val )
    {
        mModel = val;
    }

    val = settings_->Attribute("img");
    if ( val )
        mImage = val;

    settings_->Attribute("points", &mPointsNeeded);

    val = settings_->Attribute("lock");
    if ( val )
    {
        irr::core::stringw wStr(APP.GetStringTable()->Get(val));
        mLockText = wStr.c_str();
    }
}
