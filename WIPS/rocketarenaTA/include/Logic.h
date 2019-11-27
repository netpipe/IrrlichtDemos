/*

Logic - header file

*/

#ifndef __LOGIC_H
#define __LOGIC_H

#include <irrlicht.h>

using namespace irr;

#include <Init.h>


class cLogic {
public:
	
	// constructor
	cLogic(cInit *setInit);
	
	// destructor
	~cLogic();
	
	
private:
	cInit *init;
	
};

#endif
