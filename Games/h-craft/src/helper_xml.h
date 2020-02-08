// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef XML_HELPER_H
#define XML_HELPER_H

#include <irrlicht.h>

class TiXmlElement;

class XmlHelper
{
public:
    static void Read(const TiXmlElement & ele_, irr::core::vector3df &vec_,
                        const char* x_, const char* y_, const char* z_);
    static void Write(TiXmlElement & ele_, const irr::core::vector3df &vec_,
                        const char* x_, const char* y_, const char* z_);

    static void Read(const TiXmlElement & ele_, irr::video::SColor &col_,
                        const char* r_, const char* g_, const char* b_, const char* a_);
    static void Write(TiXmlElement & ele_, const irr::video::SColor &col_,
                        const char* r_, const char* g_, const char* b_, const char* a_);

    static void Read(const TiXmlElement & ele_, irr::core::aabbox3df &box_,
                    const char* maxx_, const char* maxy_, const char* maxz_,
                    const char* minx_, const char* miny_, const char* minz_);
    static void Write(TiXmlElement & ele_, const irr::core::aabbox3df &box_,
                    const char* maxx_, const char* maxy_, const char* maxz_,
                    const char* minx_, const char* miny_, const char* minz_);
};

#endif // XML_HELPER_H
