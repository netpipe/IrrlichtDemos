/*
 * Deserializer.h
 *
 *  Created on: Mar 29, 2011
 *      based on http://digitalbreed.com/tutorials/electively-serialize-to-or-from-xml-or-binary-in-c from Mathias Gall
 */

#ifndef DESERIALIZER_H_
#define DESERIALIZER_H_
#include <string>
#include "irrlicht.h"
#include "tinyxml/tinyxml.h"
#include <sstream>


 class Deserializer
 {
 public:
     Deserializer();
     Deserializer( const std::string& fileName );
     virtual ~Deserializer();


     virtual void beginAttribute( const std::string& name );

     virtual u_long readNumberOfElements();

     virtual void readParameter( const std::string& name, std::string& value );
//     virtual void readParameter( const std::string& name, bool& value );
     virtual void readParameter( const std::string& name, long& value );
     virtual void readParameter( const std::string& name, float& value );
//     virtual void readParameter( const std::string& name, irr::core::vector3df& value );
//     virtual void readParameter( const std::string& name, irr::video::SColorf& value );

     //virtual void endAttribute();

     bool fileok;

 protected:
     const std::string getValue( const std::string& name );

     std::string fileName;
     TiXmlDocument doc;
     TiXmlElement* currentElement;
 };

#endif /* DESERIALIZER_H_ */
