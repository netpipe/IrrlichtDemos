#ifndef _I_TILE_MANAGER_
#define _I_TILE_MANAGER_

#include <vector>
#include "irrlicht.h"
#include "IReferenceCounter.h"

class ITileNode;
class ITileSet;

namespace zen
{
	namespace tile
	{
		class ITileManager : public virtual IReferenceCounter
		{
		public:

			virtual ~ITileManager() {};

			virtual void addTileSet(irr::video::ITexture * tileSet, irr::u32 tileWidth, 
				irr::u32 tileHeight) = 0;
			
			virtual void addTileNode(irr::core::vector2df & position, irr::s32 tileSetID, irr::s32 tileID) = 0;

			virtual ITileNode * getTileNode(irr::s32 tileNodeID) const = 0;

			virtual ITileSet * getTileSet(irr::s32 tilesetID) const = 0;

			virtual void draw() const = 0;
		};
	}
}

#endif