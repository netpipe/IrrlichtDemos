/*
 * PathCreator v0.1
 *
 * Adam "aek" Kazimierczak
 * adam.kazimierczak@gmail.com
 * May 2010
 *
 * License: zlib (http://www.gzip.org/zlib/zlib_license.html)
 *
*/


#include "PathCreator.h"
#include <iostream>
#include <fstream>
#include <ctime>

PathCreator::PathCreator(video::IVideoDriver* driver,
		scene::ISceneNode* node,
		char* filename,
		char* arrayname) {
	this->driver = driver;
	this->node = node;
	this->filename = filename;
	this->arrayname = arrayname;
	
	this->material.Lighting = false;
	this->color = video::SColor(255, 0, 255, 0);
	this->namespacelvl = 1; // default 1 = core::
}


void PathCreator::setNamespaceLvl(int lvl) {
	this->namespacelvl = lvl;
}


void PathCreator::setColor(video::SColor color) {
	this->color = color;
}


void PathCreator::drawPath() {
	driver->setTransform(video::ETS_WORLD, core::matrix4());
	driver->setMaterial(material);

	// i=1 is not mistake!
	for(int i=1; i<points.size(); i++) {
		driver->draw3DLine(points[i-1], points[i], color); 
	}
}


void PathCreator::addPath() {
	sleep(15);
	points.push_back(node->getPosition());
}


void PathCreator::delPath() {
	sleep(10);
	if(points.size()-1 > 0)
		points.erase(points.size()-1);
}


void PathCreator::conPath() {
	sleep(10);
	if(points.size() > 0)
		points.push_back(points[0]);
}


void PathCreator::save() {
	sleep(35); // Give little more time for this function

	std::ofstream file;
	file.open(filename, std::ios::in | std::ios::trunc);

	// Check namespace level
	char* namelvl;
	switch(namespacelvl) {
	case 0:
		namelvl = "";
		break;
	case 1:
		namelvl = "core::";
		break;
	case 2:
		namelvl = "irr::core::";
		break;
	}

	// add array definition
	file << namelvl << "array"
		 << "<" << namelvl
		 << "vector3df> " << arrayname
		 << ";" <<  std::endl;

	// add points
	for(int i=0; i<points.size(); i++) { // ignore first element
		file << arrayname << ".push_back("
			 << namelvl << "vector3df("
			 << points[i].X << ","
			 << points[i].Y << ","
			 << points[i].Z << "));"
			 << std::endl;
	}

	file.close();
}


/*
 This function is needed to make some delay between addPath and delPath,
 becasue in standard case (without sleep()), user may call one of this functions on key press event,
 so if user have pressed key too long (like 10ms too long), addPath function may be called many times in one second,
 so that will add many same, or very close (about 0.0001) entries. sleep() is for prevent this.
 Also, I made my own sleep() function to make code portable between operating systems.
*/
void PathCreator::sleep(float ms) {
	ms /= 100;
	clock_t a = clock () + ms * CLOCKS_PER_SEC ;
	while (clock() < a) {}
}