// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "helper_xml.h"
#include "tinyxml/tinyxml.h"

void XmlHelper::Read(const TiXmlElement & ele_, irr::core::vector3df &vec_,
                        const char* x_, const char* y_, const char* z_)
{
    ele_.QueryFloatAttribute(x_, &vec_.X);
    ele_.QueryFloatAttribute(y_, &vec_.Y);
    ele_.QueryFloatAttribute(z_, &vec_.Z);
}

void XmlHelper::Write(TiXmlElement & ele_, const irr::core::vector3df &vec_,
                        const char* x_, const char* y_, const char* z_)
{
    ele_.SetDoubleAttribute(x_, vec_.X);
    ele_.SetDoubleAttribute(y_, vec_.Y);
    ele_.SetDoubleAttribute(z_, vec_.Z);
}

void XmlHelper::Read(const TiXmlElement & ele_, irr::video::SColor &col_,
                        const char* r_, const char* g_, const char* b_, const char* a_)
{
    int col;
    if ( TIXML_SUCCESS == ele_.QueryIntAttribute(r_, &col) )
        col_.setRed(col);
    if ( TIXML_SUCCESS == ele_.QueryIntAttribute(g_, &col) )
        col_.setGreen(col);
    if ( TIXML_SUCCESS == ele_.QueryIntAttribute(b_, &col) )
        col_.setBlue(col);
    if ( TIXML_SUCCESS == ele_.QueryIntAttribute(a_, &col) )
        col_.setAlpha(col);
}

void XmlHelper::Write(TiXmlElement & ele_, const irr::video::SColor &col_,
                        const char* r_, const char* g_, const char* b_, const char* a_)
{
    ele_.SetAttribute(r_, col_.getRed() );
    ele_.SetAttribute(g_, col_.getGreen() );
    ele_.SetAttribute(b_, col_.getBlue() );
    ele_.SetAttribute(a_, col_.getAlpha() );
}

void XmlHelper::Read(const TiXmlElement & ele_, irr::core::aabbox3df &box_,
                    const char* maxx_, const char* maxy_, const char* maxz_,
                    const char* minx_, const char* miny_, const char* minz_)
{
    ele_.QueryFloatAttribute(maxx_, &box_.MaxEdge.X);
    ele_.QueryFloatAttribute(maxy_, &box_.MaxEdge.Y);
    ele_.QueryFloatAttribute(maxz_, &box_.MaxEdge.Z);
    ele_.QueryFloatAttribute(minx_, &box_.MinEdge.X);
    ele_.QueryFloatAttribute(miny_, &box_.MinEdge.Y);
    ele_.QueryFloatAttribute(minz_, &box_.MinEdge.Z);
}

void XmlHelper::Write(TiXmlElement & ele_, const irr::core::aabbox3df &box_,
                    const char* maxx_, const char* maxy_, const char* maxz_,
                    const char* minx_, const char* miny_, const char* minz_)
{
    ele_.SetDoubleAttribute(maxx_, box_.MaxEdge.X);
    ele_.SetDoubleAttribute(maxy_, box_.MaxEdge.Y);
    ele_.SetDoubleAttribute(maxz_, box_.MaxEdge.Z);
    ele_.SetDoubleAttribute(minx_, box_.MinEdge.X);
    ele_.SetDoubleAttribute(miny_, box_.MinEdge.Y);
    ele_.SetDoubleAttribute(minz_, box_.MinEdge.Z);
}
