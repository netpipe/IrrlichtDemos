#ifndef _C_TILE_SET_
#define _C_TILE_SET_

#include "ITileSet.h"

namespace zen
{
	namespace tile
	{
		class CTileSet : public virtual ITileSet
		{
		public:

			CTileSet(irr::video::ITexture * tileSet, irr::u32 tileWidth, irr::u32 tileHeight, irr::s32 tileSetID);

			virtual ~CTileSet();

			virtual const irr::video::ITexture * getTileSetTexture() const;

			virtual bool setTileSet(irr::video::ITexture * tileSet, irr::u32 tileWidth, 
				irr::u32 tileHeight, irr::s32 tileSetID);

			virtual irr::core::rect<irr::s32> getTile(irr::s32 tileID) const;

			virtual irr::u32 getNumTiles() const;

			virtual const irr::s32 getTileSetID() const;

			virtual const irr::u32 getTileWidth() const;

			virtual void setTileWidth(irr::u32 tileWidth);

			virtual const irr::u32 getTileHeight() const;

			virtual void setTileHeight(irr::u32 tileHeight);

		private:

			irr::s32 TileSetID;
			irr::u32 TileWidth;
			irr::u32 TileHeight;
			irr::u32 NumColumns;
			irr::video::SColor ColorKey;
			irr::video::ITexture * Texture;
		};
	}
}

#endif