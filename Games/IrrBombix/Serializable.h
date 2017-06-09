/*
 * Serializable.h
 *
 *  Created on: Mar 29, 2011
 *      based on http://digitalbreed.com/tutorials/electively-serialize-to-or-from-xml-or-binary-in-c from Mathias Gall
  */

#ifndef SERIALIZABLE_H_
#define SERIALIZABLE_H_

class Serializer;
class Deserializer;

class Serializable {
public:
	Serializable();
	~Serializable();

	virtual void readFrom(Deserializer* source)=0;
	virtual void writeTo(Serializer* destination)=0;
};

#endif /* SERIALIZABLE_H_ */

