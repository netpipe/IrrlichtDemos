#ifndef _C_TILE_MANAGER_
#define _C_TILE_MANAGER_

#include "ITileManager.h"
#include "irrArray.h"

class ITileNode;
class ITileSet;

namespace zen
{
	namespace tile
	{
		class CTileManager : public virtual ITileManager
		{
		public:

			CTileManager();

			virtual ~CTileManager();

			virtual void addTileSet(irr::video::ITexture * tileSet, irr::u32 tileWidth,
				irr::u32 tileHeight);	// irr::s32 tileSetID = -1?

			virtual void addTileNode(irr::core::vector2df & position,
				irr::s32 tileSetID = -1, irr::s32 tileID = -1);

			virtual ITileNode * getTileNode(irr::s32 tileNodeID) const;

			virtual ITileSet * getTileSet(irr::s32 tilesetID) const;

			virtual void draw() const;

		private:

			irr::core::array< ITileSet * > TileSets;
			irr::core::array< ITileNode * > TileNodes;
			irr::s32 NextTileSetID;
			irr::s32 NextNodeID;
		};
	}
}

#endif
