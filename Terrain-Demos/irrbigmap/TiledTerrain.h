#ifndef TERRAIN_TILE
#define TERRAIN_TILE

#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <queue>

using namespace irr;
using namespace std;


struct global_map_type {
  //    string path;
  string filename;
  //    string path_color;
  string filename_color;
};


struct terrain_cache_file_type {
  int row;
  int column;
  irr::io::IReadFile* heightmap;
  irr::io::IReadFile* colormap;
};

// node is important for 2 reasons: 
// - A bug in ITerrainSceneNode that doesnt update AbsolutePosition 
// - So that the scale of objectes (like trees) is independent of terrain scale
struct terrain_type {
  scene::ITerrainSceneNode* terrain; // terrain
  irr::scene::ISceneNode* node; // node
};

struct terrainQueueItemType {
  terrain_type* terrain_tile;
  f32 posX;
  f32 posZ;
  int global_map_row;
  int global_map_column;
};

/********************** CLASS TiledTerrain **********************/ 
class TiledTerrain
{
public:

  TiledTerrain(int MaxRow,
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
	       f32 margin
	       );

  // placeTerrain - place 1 terrain
  void placeTerrain( terrain_type* terrain_tile, f32 posX, f32 posZ, int global_map_row, int global_map_column);

  scene::ITerrainSceneNode* getCurrentTerrain();

  void placeCube(terrain_type* terrain_tile, f32 AbsolutePosX, f32 AbsolutePosZ, f32 posX, f32 posZ, f32 size, f32 above );

  void placeSimpleObj(terrain_type* terrain_tile, f32 AbsolutePosX, f32 AbsolutePosZ, f32 posX, f32 posZ, f32 scale, const c8* path_mesh, f32 height_ground, core::vector3df rotation);

  void placeTree(terrain_type* terrain_tile, f32 AbsolutePosX, f32 AbsolutePosZ, f32 posX, f32 posZ, f32 scaleXZ, f32 scaleY);

  void placeNtrees(terrain_type* terrain_tile, f32 AbsoluteStartX, f32 AbsoluteStartZ, f32 startX, f32 startZ,f32 numX, f32 numZ, f32 distance, f32 scaleXY, f32 scaleZ);

  bool shiftTerrain(scene::ICameraSceneNode* camera);

  void placeTerrain_in_queue( terrain_type* terrain_tile, f32 posX, f32 posZ, int global_map_row, int global_map_column);

  void placeTerrain_from_queue( );

  void placeTerrain_from_queue_all( );



private:  
  f32 margin;
  f32 scaleXZ; 
  f32 scaleY;
  f32 tile_size;
  int num_layers;
  int MaxRow;
  int MaxColumn;
  int tileset_width;
  int current_row;
  int current_column;
  int center_tile_row;
  int center_tile_column;
  IrrlichtDevice* device;
  scene::ISceneManager* smgr;
  video::IVideoDriver* driver;
  global_map_type** global_map;
  terrain_type** terrain_tileset;
  terrain_cache_file_type* terrain_cache_file;
  int num_tiles_cache;
  scene::ITerrainSceneNode* current_terrain;
  boost::thread *threadLoadMapTilesCache;
  int current_frame;
  queue <terrainQueueItemType *> terrainQueue;


  void remove_line(int row);
  void add_line(int row);
  void remove_column(int column);
  void add_column(int column);
  int findCacheTerrain(int row, int column); 
};

#endif
