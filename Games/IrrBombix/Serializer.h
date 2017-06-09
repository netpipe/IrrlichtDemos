/*
 * Serializer.h
 *
 *  Created on: Mar 29, 2011
 *      based on http://digitalbreed.com/tutorials/electively-serialize-to-or-from-xml-or-binary-in-c from Mathias Gall
 */

#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include "tinyxml/tinyxml.h"
#include "irrlicht.h"
#include <string>
#include <sstream>

 class Serializer
 {
 public:
	 Serializer();
     ~Serializer();

     Serializer( const std::string& fileName );
     virtual void beginAttribute( const std::string& name );

     //void writeNumberOfElements( u_long numElements ) = 0;

     virtual void writeParameter( const std::string& name, const std::string& value );
//     virtual void writeParameter( const std::string& name, const bool& value ) = 0;
     virtual void writeParameter( const std::string& name, const long& value );
     virtual void writeParameter( const std::string& name, const float& value );
//     virtual void writeParameter( const std::string& name, const irr::core::vector3df& value ) = 0;
//     virtual void writeParameter( const std::string& name, const irr::video::SColorf& value ) = 0;

     virtual void endAttribute();
 protected:
     std::string filename;
     TiXmlDocument doc; // The actual XML document created by the serializer
     TiXmlElement* currentElement; // The current node we're working on
 };

#endif /* SERIALIZER_H_ */
