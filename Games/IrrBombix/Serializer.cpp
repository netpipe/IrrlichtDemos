/*
 * Serializer.cpp
 *
 *  Created on: Mar 29, 2011
 *      based on http://digitalbreed.com/tutorials/electively-serialize-to-or-from-xml-or-binary-in-c from Mathias Gall
 */

#include "Serializer.h"

Serializer::Serializer() {
	//this.fileName=fileName;
	currentElement = 0;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);
}
Serializer::Serializer( const std::string& fileName )
{
	filename=fileName;
	currentElement = 0;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);
}

Serializer::~Serializer() {
	// TODO Auto-generated destructor stub
}

void Serializer::beginAttribute(const std::string& name) {
	TiXmlElement* child = new TiXmlElement(name);

	if (currentElement == 0) {
		doc.LinkEndChild(child);
	} else {
		currentElement->LinkEndChild(child);
	}

	currentElement = child;
}
void Serializer::writeParameter(const std::string& name,
		const std::string& value) {
	//VTE_ASSERT( currentElement != 0 ); // Make sure that beginAttribute has been called!
	if (currentElement) {
		currentElement->SetAttribute(name, value);
	}
}

void Serializer::writeParameter( const std::string& name, const float& value ){
	// Int -> String
	std::stringstream sstr;
	sstr << value;
	writeParameter(name, sstr.str());
};

void Serializer::writeParameter( const std::string& name, const long& value ){
	// Int -> String
	std::stringstream sstr;
	sstr << value;
	writeParameter(name, sstr.str());
};




void Serializer::endAttribute() {
	//VTE_ASSERT( currentElement != 0 ); // Make sure that beginAttribute has been called!

	if (currentElement && currentElement->Parent() != 0) {
		// becomes 0 if the document is reached
		currentElement = currentElement->Parent()->ToElement();
	} else {
		// write??
		doc.SaveFile(filename.c_str());
	}
}
