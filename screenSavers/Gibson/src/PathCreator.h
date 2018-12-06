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


#include <irrlicht.h>

using namespace irr;

/*
 * Constructor params:
 *   driver      -  pointer to driver
 *   node        -  usually camera, from this node code will get point positions while addPath() is called
 *   filename    -  path to file, where waypoints should be saved
 *   arrayname   -  how to name array in output file?
 *
 * Also, additional configs via methods:
 *   setColor(SColor)       -  color of line drawed by drawPath()
 *   setNamespaceLvl(int)   -  level of namespace "depth" used in naming types in output file, examples:
 *                               0  = (no namespace) = array<vector3df> points;
 *                               1  = (core::)       = core::array<core::vector3df> points;
 *                               2  = (irr::core::)  = irr::core::array<irr::core::vector3df> points
 *
 * Example of use:
 *  PathCreator creator(driver, cam, "test.txt", "points");
 *	while(device->run())
 *	{
 *		if(receiver.IsKeyDown(irr::KEY_RETURN))
 *			creator.addPath();
 *		else if (receiver.IsKeyDown(irr::KEY_BACK))
 *			creator.delPath();
 *		else if (receiver.IsKeyDown(irr::KEY_KEY_C))
 *			creator.conPath();
 *		else if (receiver.IsKeyDown(irr::KEY_ESCAPE))
 *			creator.save();
 *
 *		driver->beginScene(true, true, video::SColor(255,100,100,100));
 *		cretor.drawPath();
 *		smgr->drawAll();
 *		driver->endScene();
 *  }
*/

class PathCreator {
public:
	PathCreator(video::IVideoDriver* driver,
		scene::ISceneNode* node,
		char* filename,
		char* arrayname);

	void drawPath(); 

	void addPath(); 
	void delPath(); 
	void conPath();

	void save();

	void setColor(video::SColor color); 
	void setNamespaceLvl(int lvl); // 0=no namespace use; 1=irr; 2=irr::core

private:
	void sleep(float ms);

	video::IVideoDriver* driver;
	scene::ISceneNode* node;
	core::array<core::vector3df> points;
	video::SMaterial material;
	video::SColor color;

	char* filename;
	char* arrayname;
	int namespacelvl;
};