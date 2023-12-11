#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <sqlite3.h>

// include irrlicht

#include <irrlicht.h>

#include "CLODTreeNode.h"

// sqlite database


using namespace std;

using namespace irr;
using namespace core;

/*

// entities table
// --------------
 CREATE TABLE entities (pid   int,   uid  int,   oid int,
                        flags int,   seed int,
                        px    float, py   float, pz  float,
                        rx    float, ry   float, rz  float,
                        sx    float, sy   float, sz  float);
 CREATE INDEX "entity pos" on entities (pid);
 CREATE INDEX "entity uid" on entities (uid);


enum LOD_FLAGS
{


}
*/


scene::ISceneNode* node = 0;
IrrlichtDevice* device = 0;
scene::ISceneManager* smgr;

int main()   {

	device = createDevice( video::EDT_OPENGL, core::dimension2d<s32>(800, 600));

	video::IVideoDriver* driver = device->getVideoDriver();
	smgr = device->getSceneManager();

    driver->setTextureCreationFlag( video::ETCF_ALWAYS_32_BIT, true );

	ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0, 100.0f);
	device->getCursorControl()->setVisible(false);

    cam->setFarValue( 20000 );
	int lastFPS = -1;

    // create a quadtreescenenode ( parent, SceneManager, ID, Tree Depth );
    LODTreeSceneNode* qt = new LODTreeSceneNode(0, smgr, -1);
    // set to show tree bounding boxes
    qt->setDebugDataVisible( true );

    smgr->addCubeSceneNode();

    // move camera to center of forest
    core::vector3df mid = qt->getBoundingBox().MaxEdge / 2;
    mid.Y == 0;
    cam->setPosition( vector3df(0,0,0) );

	while(device->run()) {
		driver->beginScene(true, true, video::SColor(255,15,128,240));
		smgr->drawAll();
		driver->endScene();

		int fps = driver->getFPS();

        //cam->setPosition(vector3df(cam->getPosition().X,23,cam->getPosition().Z));

		if (lastFPS != fps) {
			wchar_t tmp[1024];
			swprintf(tmp, 1024, L"Quad World [%s] Triangle count: %d - fps:%d", driver->getName(), driver->getPrimitiveCountDrawn(), fps);
			device->setWindowCaption(tmp);
			lastFPS = fps;
		}
	}

	device->drop();

	return 0;
}

int mainz(int argc, char **argv){
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;

//  if( argc!=3 ){
//    fprintf(stderr, "Usage: %s DATABASE SQL-STATEMENT\n", argv[0]);
//    exit(1);
//  }
  rc = sqlite3_open("data.db", &db);
  if( rc ){
    printf("Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    system("PAUSE");
    exit(1);
  }
  rc = sqlite3_exec(db, "SELECT * FROM ENTITIES;", callback, 0, &zErrMsg);
  if( rc!=SQLITE_OK ){
    printf("SQL error: %s\n", zErrMsg);
  }
  sqlite3_close(db);

  vector3df pos(900,120,0);
  s32 inlevel = 6;  // 0 to 8, 6=8m range
  u32 out     = 0;
  int TREE_MAX_LOD = 8;

  u32 level = 1024;
  vector3df lastpos(0,0,0);
  u32 p;
  for (int n =1; n < TREE_MAX_LOD; n++)
  {
      level /= 2;
      p = 0;
      if (n <= inlevel)
      {
           if ( pos.X > lastpos.X + level)
           {
              lastpos.X += level;
              // x = 2 + 1
              p |= 3;
           }
           if ( pos.Y > lastpos.Y + level)
           {
              lastpos.Y += level;
              // y = 4 + 1
              p |= 5;
           }
      }
      // else { blank is 0 }
      out = (out * 8) + p;
  }

  printf("out=%d\n", ((2 & 2) == 2));

  system("PAUSE");
  return 0;
}

int maina(int argc, char *argv[])
{


    system("PAUSE");
    return EXIT_SUCCESS;
}
