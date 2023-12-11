#include <irrlicht.h>
#include "driverChoice.h"
#include <iostream>
#include <string>
#include <sstream>
#include "TiledTerrain.h"
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <queue>

using namespace irr;
using namespace std;
using namespace io;

/********************** Aux functios for thread cache **********************/


irr::io::IReadFile* loadFile(IrrlichtDevice* device, const char* terrain_filename)
{
  irr::io::IReadFile* file;
  s32 size;
  char* memblock;
  file = device->getFileSystem()->createAndOpenFile(terrain_filename);
  size = file->getSize();
  memblock = new char [size];
  file->read(memblock, size);
  if (size)
    return(device->getFileSystem()->createMemoryReadFile(memblock, size, terrain_filename, true));
  return NULL;
}

void workerLoadMapTilesCache(IrrlichtDevice* device, global_map_type** global_map, terrain_cache_file_type* terrain_cache_file, int num_tiles_cache, int min_row, int min_column, int max_row, int max_column)
{
  int i_cache=0;
  void * memory;
  s32 size;
  char* memblock;
  irr::io::IFileArchive ** retArchive;
  irr::io::IReadFile* file;

  // Top
  for(int i=min_column; i<=max_column;++i)
    {
      const c8* terrain_filename=global_map[min_row][i].filename.c_str();
      const c8* texture_filename=global_map[min_row][i].filename_color.c_str();
      terrain_cache_file[i_cache].row=min_row;
      terrain_cache_file[i_cache].column=i;
      terrain_cache_file[i_cache].heightmap = loadFile(device, terrain_filename);
      terrain_cache_file[i_cache].colormap = loadFile(device, texture_filename);
      ++i_cache;
    }
  // Bottom
  for(int i=min_column; i<=max_column;++i)
    {
      const c8* terrain_filename=global_map[max_row][i].filename.c_str();
      const c8* texture_filename=global_map[max_row][i].filename_color.c_str();
      terrain_cache_file[i_cache].row=max_row;
      terrain_cache_file[i_cache].column=i;
      terrain_cache_file[i_cache].heightmap = loadFile(device, terrain_filename);
      terrain_cache_file[i_cache].colormap = loadFile(device, texture_filename);
      ++i_cache;
    }
  // Left
  for(int i=min_row+1; i<=max_row-1;++i)
    {
      const c8* terrain_filename=global_map[i][min_column].filename.c_str();
      const c8* texture_filename=global_map[i][min_column].filename_color.c_str();
      terrain_cache_file[i_cache].row=i;
      terrain_cache_file[i_cache].column=min_column;
      terrain_cache_file[i_cache].heightmap = loadFile(device, terrain_filename);
      terrain_cache_file[i_cache].colormap = loadFile(device, texture_filename);
      ++i_cache;
    }
  // Right
  for(int i=min_row+1; i<=max_row-1;++i)
    {
      const c8* terrain_filename=global_map[i][max_column].filename.c_str();
      const c8* texture_filename=global_map[i][max_column].filename_color.c_str();
      terrain_cache_file[i_cache].row=i;
      terrain_cache_file[i_cache].column=max_column;
      terrain_cache_file[i_cache].heightmap = loadFile(device, terrain_filename);
      terrain_cache_file[i_cache].colormap = loadFile(device, texture_filename);
      ++i_cache;
    }
}


/********************** CLASS TiledTerrain **********************/
TiledTerrain::TiledTerrain(int MaxRow,
			   int MaxColumn,
			   int num_layers,
			   IrrlichtDevice* device,
			   scene::ISceneManager* smgr,
			   video::IVideoDriver* driver,
			   int initial_row,
			   int initial_column,
			   f32 tile_size,
			   f32 scaleXZ,
                           f32 scaleY,
			   f32 margin)
{
  this->threadLoadMapTilesCache = NULL;
  this->margin = margin;
  this->scaleXZ = scaleXZ;
  this->scaleY = scaleY;
  this->tile_size = tile_size;
  this->MaxRow = MaxRow;
  this->MaxColumn = MaxColumn;
  this->num_layers = num_layers;
  this->tileset_width=num_layers*2+1;
  this->smgr = smgr;
  this->device = device;
  this->driver = driver;
  this->num_tiles_cache = (num_layers+1)*8; // number of items to get an "extra" layer
  this->current_frame = 0;

  // create global_map[MaxRow][MaxColumn]
  global_map=new global_map_type*[MaxRow];
  for(int i=0; i<MaxRow; ++i)
    global_map[i]=new global_map_type[MaxColumn];

  // create terrain_tileset[tileset_width][tileset_width]
  terrain_tileset=new terrain_type*[tileset_width];
  for(int i=0; i<tileset_width; ++i)
      terrain_tileset[i]=new terrain_type[tileset_width];

  // create terrain_cache_file[num_tiles_cache]
  terrain_cache_file = new terrain_cache_file_type[num_tiles_cache];
  for(int i=0; i<num_tiles_cache; ++i)
    {
      terrain_cache_file[i].row=0;
      terrain_cache_file[i].column=0;
      terrain_cache_file[i].heightmap=0;
      terrain_cache_file[i].colormap=0;
    }


  // Create Global Map structure
  // For now, uses 2 type of files:
  // - heightmap_r1_c99.bmp  (heightmap)
  // - color_r1_c99.png      (colormap)
  int row, column;
  for(row=0; row<MaxRow; row++)
    {
      for(column=0; column<MaxColumn; column++)
	{
	  stringstream sstream_filename, sstream_color_filename;
	  sstream_filename << "heightmap_r" << row+1 << "_c" << column+1 <<".bmp";
	  sstream_color_filename << "color_r" << row+1 << "_c" << column+1 <<".png";
	  global_map[row][column].filename = sstream_filename.str();
	  global_map[row][column].filename_color = sstream_color_filename.str();
	}
    }

  this->current_row=initial_row;
  this->current_column=initial_column;
  this->center_tile_row = this->num_layers;
  this->center_tile_column = this->num_layers;

  // Terrain Files
  // For now, all files must be compressed on 2 zips: heightmap.zip and color.zip
  device->getFileSystem()->addFileArchive("heightmap.zip");
  device->getFileSystem()->addFileArchive("color.zip");

  // Place initial terrain tiles
  f32 offset_x;
  f32 offset_y;
  for(row=0; row < tileset_width; row ++)
    {
      offset_y = (row)*tile_size;
      for(column=0; column<tileset_width; column++)
	{
	  offset_x = -1*(column)*tile_size;
	  placeTerrain(&terrain_tileset[row][column], offset_x,offset_y, initial_row+row-num_layers, initial_column+column-num_layers);
	}
    }
  this->current_terrain = terrain_tileset[center_tile_row][center_tile_column].terrain;

  // load cache
  threadLoadMapTilesCache = new boost::thread(workerLoadMapTilesCache, device, global_map, terrain_cache_file, num_tiles_cache, current_row-num_layers-1, current_column-num_layers-1, current_row-num_layers+tileset_width, current_column-num_layers+tileset_width);
}


int TiledTerrain::findCacheTerrain(int row, int column)
{
  for(int i=0; i<num_tiles_cache; ++i)
    {
      if(terrain_cache_file[i].row==row and terrain_cache_file[i].column==column)
	return i;
    }
  return 0;
}


void TiledTerrain::placeTerrain_in_queue( terrain_type* terrain_tile, f32 posX, f32 posZ, int global_map_row, int global_map_column)
{
  terrainQueueItemType* terrainQueueItem = new terrainQueueItemType;
  terrainQueueItem->terrain_tile = terrain_tile;
  terrainQueueItem->posX = posX;
  terrainQueueItem->posZ = posZ;
  terrainQueueItem->global_map_row = global_map_row;
  terrainQueueItem->global_map_column = global_map_column;
  terrainQueue.push(terrainQueueItem);
}

void TiledTerrain::placeTerrain_from_queue()
{
  if (terrainQueue.empty())
    return;
  terrainQueueItemType* terrainQueueItem = terrainQueue.front();
  terrainQueue.pop();
  placeTerrain(terrainQueueItem->terrain_tile, terrainQueueItem->posX, terrainQueueItem->posZ, terrainQueueItem->global_map_row, terrainQueueItem->global_map_column);
  delete[] terrainQueueItem;
}

void TiledTerrain::placeTerrain_from_queue_all()
{
  while(not terrainQueue.empty())
    {
      terrainQueueItemType* terrainQueueItem = terrainQueue.front();
      terrainQueue.pop();
      placeTerrain(terrainQueueItem->terrain_tile, terrainQueueItem->posX, terrainQueueItem->posZ, terrainQueueItem->global_map_row, terrainQueueItem->global_map_column);
      delete[] terrainQueueItem;
    }
}


  // placeTerrain - place 1 terrain
void TiledTerrain::placeTerrain( terrain_type* terrain_tile, f32 posX, f32 posZ, int global_map_row, int global_map_column)
{
  scene::ITerrainSceneNode* terrain;
  const c8* terrain_filename=global_map[global_map_row][global_map_column].filename.c_str();
  const c8* texture_filename=global_map[global_map_row][global_map_column].filename_color.c_str();
  int i_cached;

  if(threadLoadMapTilesCache)
    {
      threadLoadMapTilesCache->join();
      threadLoadMapTilesCache = NULL;
    };

  i_cached = findCacheTerrain(global_map_row, global_map_column);
  if(i_cached and terrain_cache_file[i_cached].heightmap)
    {

      terrain = smgr->addTerrainSceneNode(
					  terrain_cache_file[i_cached].heightmap,
					  0,					  // parent node
					  -1,					  // node id
					  core::vector3df(posX, 0.f, posZ),	  // position
					  core::vector3df(0.f, 0.f, 0.f),		  // rotation
					  core::vector3df(scaleXZ, scaleY, scaleXZ),  // scale
					  video::SColor ( 255, 255, 255, 255 ),	  // vertexColor
					  1,					  // maxLOD
					  scene::ETPS_33,				  // patchSize
					  1					  // smoothFactor
					  );
    }
  else // if not in cache, disk load
    {
      terrain = smgr->addTerrainSceneNode(
					  terrain_filename,
					  0,					  // parent node
					  -1,					  // node id
					  core::vector3df(posX, 0.f, posZ),	  // position
					  core::vector3df(0.f, 0.f, 0.f),		  // rotation
					  core::vector3df(scaleXZ, scaleY, scaleXZ),  // scale
					  video::SColor ( 255, 255, 255, 255 ),	  // vertexColor
					  1,					  // maxLOD
					  scene::ETPS_33,				  // patchSize
					  1					  // smoothFactor
					  );
    }

  terrain->setMaterialFlag(video::EMF_LIGHTING, false);
  terrain->setMaterialFlag(video::EMF_BILINEAR_FILTER, true);
  terrain->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER , true);
  terrain->setMaterialFlag(video::EMF_NORMALIZE_NORMALS  , true);
  terrain->setMaterialFlag(video::EMF_TEXTURE_WRAP   , true);

  if(i_cached and terrain_cache_file[i_cached].colormap)
    terrain->setMaterialTexture(0, driver->getTexture(terrain_cache_file[i_cached].colormap));
  else
    terrain->setMaterialTexture(0, driver->getTexture(texture_filename));

  terrain->setMaterialTexture(1, driver->getTexture("./detailmap3.jpg"));
  terrain->setMaterialType(video::EMT_DETAIL_MAP);
  terrain->scaleTexture(1.0f, 1.0f * 255 * 255 * 90);
  terrain->setMaterialFlag(video::EMF_FOG_ENABLE, false);
  terrain->setName(terrain_filename);


  //addthis tecan for ogl fix
    terrain->getMaterial(0).MaterialTypeParam = video::pack_textureBlendFunc
  (
    video::EBF_SRC_ALPHA,
    video::EBF_ONE_MINUS_SRC_ALPHA,
    video::EMFN_MODULATE_1X,
    video::EAS_TEXTURE | video::EAS_VERTEX_COLOR
  );


  // you need one of the following:
  terrain->getMaterial(0).MaterialType = video::EMT_ONETEXTURE_BLEND;
  //terrain->getMaterial(0).MaterialType = video::EMT_SOLID;
  //DEBUG
  bool showDebug =false;
  terrain->setDebugDataVisible(showDebug?scene::EDS_BBOX:scene::EDS_OFF);
  (*terrain_tile).terrain = terrain;
  (*terrain_tile).node = smgr->addEmptySceneNode();
  (*terrain_tile).node->setPosition(core::vector3df(posX, 0.f, posZ));

  placeNtrees(terrain_tile, posX, posZ,57600,57600, 10, 10, 400.f, 1.f, 1.f);
  placeSimpleObj(terrain_tile, posX, posZ, 57400, 57600, 0.3f, "farmhouse3/farmhouse.obj", 0, core::vector3df(0,0,0));
}


void TiledTerrain::placeCube(terrain_type* terrain_tile, f32 AbsolutePosX, f32 AbsolutePosZ, f32 posX, f32 posZ, f32 size, f32 above )
{
  scene::ISceneNode* cube = smgr->addCubeSceneNode(size, (*terrain_tile).node);
  cube->setPosition(core::vector3df(posX,(*terrain_tile).terrain->getHeight(AbsolutePosX+posX,AbsolutePosZ+posZ)+above,posZ));
  cube->setMaterialFlag(video::EMF_LIGHTING, false);
}

void TiledTerrain::placeSimpleObj(terrain_type* terrain_tile, f32 AbsolutePosX, f32 AbsolutePosZ, f32 posX, f32 posZ, f32 scale, const c8* path_mesh, f32 height_ground, core::vector3df rotation)
{
  scene::IMeshSceneNode* sobject = smgr->addMeshSceneNode(smgr->getMesh(path_mesh), (*terrain_tile).node);
  sobject->setDebugDataVisible(false);
  sobject->setName(path_mesh);
  sobject->setPosition(core::vector3df(posX,(*terrain_tile).terrain->getHeight(AbsolutePosX+posX,AbsolutePosZ+posZ)+height_ground,posZ));
  sobject->setRotation(rotation);
  sobject->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, false);
  sobject->setMaterialFlag(video::EMF_LIGHTING, false);
  sobject->setScale(core::vector3df(scale,scale,scale));
  sobject->setVisible(true);
}

void TiledTerrain::placeTree(terrain_type* terrain_tile, f32 AbsolutePosX, f32 AbsolutePosZ, f32 posX, f32 posZ, f32 scaleXZ, f32 scaleY)
{
  if ((*terrain_tile).terrain->getHeight(AbsolutePosX+posX,AbsolutePosZ+posZ)>10 )
    {
      scene::IMeshSceneNode* pinet1 = smgr->addMeshSceneNode(smgr->getMesh("./f_pinetree1/b3d__pinet1.b3d"), (*terrain_tile).node);
      pinet1->setMaterialTexture( 0, driver->getTexture("./f_pinetree1/pinet1.png") );
      pinet1->setName("tree");
      pinet1->setPosition(core::vector3df(posX,(*terrain_tile).terrain->getHeight(AbsolutePosX+posX,AbsolutePosZ+posZ),posZ));
      pinet1->setMaterialFlag(video::EMF_LIGHTING, false);
      pinet1->setScale(core::vector3df(scaleXZ,scaleY,scaleXZ));
      pinet1->setVisible(true);
    }
}

void TiledTerrain::placeNtrees(terrain_type* terrain_tile, f32 AbsoluteStartX, f32 AbsoluteStartZ, f32 startX, f32 startZ,f32 numX, f32 numZ, f32 distance, f32 scaleXY, f32 scaleZ)
{
  int x,z;
  f32 posX, posZ, AbsolutePosX, AbsolutePosZ;
  posX = startX;
  AbsolutePosX = AbsoluteStartX;
  AbsolutePosZ = AbsoluteStartZ;
  for(x=1; x<=numX; x++)
    {
      posZ = startZ;
      for(z=1; z<=numZ; z++)
	{
	  placeTree(terrain_tile,  AbsolutePosX, AbsolutePosZ, posX, posZ, scaleXY, scaleZ);
	  posZ += distance + (rand() % int(distance)) ;
	}
	posX += distance + (rand() % int(distance)) ;
    }
}


scene::ITerrainSceneNode* TiledTerrain::getCurrentTerrain()
{
  return current_terrain;
}


void TiledTerrain::remove_line(int row)
{
  for(int column=0; column<tileset_width; column++)
    {
      this->terrain_tileset[row][column].terrain->removeAll();
      terrain_tileset[row][column].terrain->remove();
      terrain_tileset[row][column].node->removeAll();
      terrain_tileset[row][column].node->remove();
    }
}

void TiledTerrain::remove_column(int column)
{
  for(int row=0; row<tileset_width; row++)
    {
      terrain_tileset[row][column].terrain->removeAll();
      terrain_tileset[row][column].terrain->remove();
      terrain_tileset[row][column].node->removeAll();
      terrain_tileset[row][column].node->remove();
    }
}

void TiledTerrain::add_line(int row)
{
  f32 offset_y = tile_size * row;
  f32 offset_x;
  for(int column=0; column<tileset_width; column++)
    {
      offset_x = -1*(column)*tile_size;
      placeTerrain_in_queue(&terrain_tileset[row][column], offset_x,offset_y, current_row+row-num_layers, current_column+column-num_layers);
    }
 }

void TiledTerrain::add_column(int column)
{
  f32 offset_x = -1 * tile_size * column;
  f32 offset_y;
      for(int row=0; row<tileset_width; row++)
	{
	  offset_y = (row)*tile_size;
	  placeTerrain_in_queue(&terrain_tileset[row][column], offset_x,offset_y, current_row+row-num_layers, current_column+column-num_layers);
	}
}


bool TiledTerrain::shiftTerrain(scene::ICameraSceneNode* camera)
{
  bool moved=false;
  int column, row;
  f32 offset_x, offset_y;

  ++current_frame;

  if (current_frame>=1000)
    current_frame = 1;
  if (current_frame % 2 == 0) // every 10 frames, place 1 terrain (if in queue)
    placeTerrain_from_queue();


  //////// Map Shift UP /////////
  if(camera->getAbsolutePosition().Z < (tile_size * num_layers)-margin )
    {
      //moving too fast !
      if(not terrainQueue.empty())
	placeTerrain_from_queue_all();
      //remove_line
      remove_line(tileset_width-1);
      // shift terrain
      for(row=tileset_width-2; row >= 0; row --)
	{
	  offset_y = (row+1)*tile_size;
	  for(column=0; column<tileset_width; column++)
	    {
	      offset_x = -1*(column)*tile_size;
	      terrain_tileset[row+1][column].terrain = terrain_tileset[row][column].terrain;
	      terrain_tileset[row+1][column].terrain->setPosition(core::vector3df(offset_x, 0.f, offset_y));
	      terrain_tileset[row+1][column].node = terrain_tileset[row][column].node;
	      terrain_tileset[row+1][column].node->setPosition(core::vector3df(offset_x, 0.f, offset_y));
	    }
	}
      // shift camera
      camera->setPosition(core::vector3df(camera->getAbsolutePosition().X,
					  camera->getAbsolutePosition().Y,
					  camera->getAbsolutePosition().Z+tile_size));
      // add line
      current_row--;
      add_line(0);
      //Current terrain
      this->current_terrain = terrain_tileset[center_tile_row][center_tile_column].terrain;
      moved=true;
    }
  //////// Map Shift DOWN /////////
  if(camera->getAbsolutePosition().Z > (tile_size * (num_layers+1) + margin))
    {
      //moving too fast !
      if(not terrainQueue.empty())
	placeTerrain_from_queue_all();
      // remove_line
      remove_line(0);
      // shift terrain
      for(row=1; row <= tileset_width-1; row ++)
	{
	  offset_y = (row-1)*tile_size;
	  for(column=0; column<tileset_width; column++)
	    {
	      offset_x = -1*(column)*tile_size;
	      terrain_tileset[row-1][column].terrain = terrain_tileset[row][column].terrain;
	      terrain_tileset[row-1][column].terrain->setPosition(core::vector3df(offset_x, 0.f, offset_y));
	      terrain_tileset[row-1][column].node = terrain_tileset[row][column].node;
	      terrain_tileset[row-1][column].node->setPosition(core::vector3df(offset_x, 0.f, offset_y));
	    }
	}
      // shift camera
      camera->setPosition(core::vector3df(camera->getAbsolutePosition().X,
					  camera->getAbsolutePosition().Y,
					  camera->getAbsolutePosition().Z-tile_size));
      // add line
      current_row++;
      add_line(tileset_width-1);
      //Current terrain
      this->current_terrain = terrain_tileset[center_tile_row][center_tile_column].terrain;
      moved = true;;
    }
  //////// Map Shift LEFT/////////
  if(camera->getAbsolutePosition().X > ((-1) * tile_size * (num_layers-1)+margin))
    {
      //moving too fast !
      if(not terrainQueue.empty())
	placeTerrain_from_queue_all();
      // remove_column
      remove_column(tileset_width-1);

      //shift terrain
      for(column=tileset_width-2; column >= 0; column --)
	{
	  offset_x = -1*(column+1)*tile_size;
	  for(row=0; row<tileset_width; row++)
		  {
		    offset_y = (row)*tile_size;
		    terrain_tileset[row][column+1].terrain = terrain_tileset[row][column].terrain;
		    terrain_tileset[row][column].terrain->setPosition(core::vector3df(offset_x, 0.f, offset_y));
		    terrain_tileset[row][column+1].node = terrain_tileset[row][column].node;
		    terrain_tileset[row][column].node->setPosition(core::vector3df(offset_x, 0.f, offset_y));
		  }
	}
      // shift camera
      camera->setPosition(core::vector3df(camera->getAbsolutePosition().X-tile_size,
					  camera->getAbsolutePosition().Y,
					  camera->getAbsolutePosition().Z));
      // add column
      current_column--;
      add_column(0);
      //Current terrain
      this->current_terrain = terrain_tileset[center_tile_row][center_tile_column].terrain;
      moved=true;
    }
  //////// Map Shift RIGHT /////////
  if(camera->getAbsolutePosition().X < ((-1) * tile_size * (num_layers)-margin) )
    {
      //moving too fast !
      if(not terrainQueue.empty())
	placeTerrain_from_queue_all();
      // remove_column
      remove_column(0);
      // shift terrain
      for(column=1; column <= tileset_width-1; column ++)
	{
	  offset_x = -1*(column-1)*tile_size;
	  for(row=0; row<tileset_width; row++)
	    {
	      offset_y = (row)*tile_size;
	      terrain_tileset[row][column-1].terrain = terrain_tileset[row][column].terrain;
	      terrain_tileset[row][column].terrain->setPosition(core::vector3df(offset_x, 0.f, offset_y));
	      terrain_tileset[row][column-1].node = terrain_tileset[row][column].node;
	      terrain_tileset[row][column].node->setPosition(core::vector3df(offset_x, 0.f, offset_y));
	    }
	}
      // shift camera
      camera->setPosition(core::vector3df(camera->getAbsolutePosition().X+tile_size,
					  camera->getAbsolutePosition().Y,
					  camera->getAbsolutePosition().Z));


      // add column
      current_column++;
      add_column(tileset_width-1);
      //Current terrain
      this->current_terrain = terrain_tileset[center_tile_row][center_tile_column].terrain;
      moved=true;
    }

  //////// end Map Shift /////////
  if(moved)
    {
      threadLoadMapTilesCache = new boost::thread(workerLoadMapTilesCache, device, global_map, terrain_cache_file, num_tiles_cache, current_row-num_layers-1, current_column-num_layers-1, current_row-num_layers+tileset_width, current_column-num_layers+tileset_width);
    }
  return moved;
}
