/*
 * Deserializer.cpp
 *
 *  Created on: Mar 29, 2011
 *      based on http://digitalbreed.com/tutorials/electively-serialize-to-or-from-xml-or-binary-in-c from Mathias Gall
 */

#include "Deserializer.h"
#include <iostream>

Deserializer::Deserializer() {
  // TODO Auto-generated constructor stub
  fileok=false;
}

Deserializer::~Deserializer() {
  // TODO Auto-generated destructor stub
}

Deserializer::Deserializer(const std::string& fileName) :
  fileName(fileName), currentElement(0) {
  //std::cout << "Deserializer "<< fileName <<  " try to parse\n";
  bool result = doc.LoadFile(fileName);
  if (!result) {
    std::ostringstream str;
    str << "Unable to parse XML file '" << fileName << ":" << std::endl;
    str << doc.ErrorDesc() << " in line " << doc.ErrorRow() << ", column "
	<< doc.ErrorCol() << std::endl;

    //throw XMLParseException(str.str());
  }else{
    //std::cout << "Deserializer "<< fileName <<  "parse done\n";
  }
  fileok=result;
}

void Deserializer::beginAttribute(const std::string& name) {
  if (fileok){
    //std::cout << "Deserializer beginAttribute "<< name <<  "\n";
    if (currentElement == 0) {
      currentElement = doc.FirstChild(name)->ToElement();
    } else {
      currentElement = currentElement->FirstChild(name)->ToElement();
    }
  }
  //VTE_ASSERT( currentElement != 0 );
}

u_long Deserializer::readNumberOfElements() {
  if (fileok){
    //VTE_ASSERT( currentElement != 0 );

    TiXmlNode* child = currentElement->FirstChild();
    long count = 0;
    while (child != 0) {
      if (child->Type() != TiXmlNode::TINYXML_COMMENT) {
	count++;
      }

      child = child->NextSibling();
    }

    return count;
  }
}

const std::string Deserializer::getValue(const std::string& name) {
  const std::string* str = new std::string("no valid xml file");
  if (fileok){
    str = currentElement->Attribute(name);
    //std::cout << "Deserializer::getValue Attribute"<< str << "\n";
    if (0 == str) {
      std::ostringstream str2;
      str2 << "Missing attribute '" << name << "' for element '"
	   << currentElement->Value() << "' ";
      str2 << "in line " << currentElement->Row() << ", column "
	   << currentElement->Column() << " ";
      str2 << "in file '" << fileName << "'";

      //throw XMLParseException(str.str());

      //str = new std::string(str2.str().c_str());
      str = new std::string("");
    }
    //std::cout << "Deserializer::getValue "<< str->c_str() << "\n";
  }else
    {
      std::cout << "Deserializer::getValue cant get "<< name << "\n";
    }
  return *str;

}

void Deserializer::readParameter(const std::string& name, std::string& value) {
  //VTE_ASSERT( currentElement != 0 );
  value = getValue(name);
}

void Deserializer::readParameter( const std::string& name, long& value ){
  std::stringstream sstr(getValue(name));
  sstr >> value;
}

void Deserializer::readParameter( const std::string& name, float& value ){
  std::stringstream sstr(getValue(name));
  sstr >> value;
}
