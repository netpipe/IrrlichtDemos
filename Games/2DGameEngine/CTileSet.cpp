#include "CTileSet.h"

namespace zen
{
	namespace tile
	{
		CTileSet::CTileSet(irr::video::ITexture * tileSet, irr::u32 tileWidth, 
							irr::u32 tileHeight, irr::s32 tileSetID)
		{
			setTileSet(tileSet, tileWidth, tileHeight, tileSetID);
		}

		CTileSet::~CTileSet()
		{
			if (Texture)
				Texture->drop();
		}

		const irr::video::ITexture * CTileSet::getTileSetTexture() const
		{
			return Texture;
		}

		bool CTileSet::setTileSet(irr::video::ITexture * tileSet, irr::u32 tileWidth, 
			irr::u32 tileHeight, irr::s32 tileSetID)
		{
			Texture = tileSet;
			Texture->grab();
			TileWidth = tileWidth;
			TileHeight = tileHeight;
			TileSetID = tileSetID;
			if (((Texture->getSize().Width % TileWidth) != 0) && (Texture->getSize().Height % TileHeight != 0))
				return false;
			else
				NumColumns = Texture->getSize().Width / TileWidth;
			
			return true;
		}

		irr::core::rect<irr::s32> CTileSet::getTile(irr::s32 tileID) const
		{
			irr::u32 column = tileID % NumColumns;
			irr::u32 row = tileID / NumColumns;

			irr::core::rect<irr::s32> tile(column * TileWidth, row * TileHeight, 
										(column + 1) * TileWidth, (row + 1) * TileHeight);

			return tile;
		}

		irr::u32 CTileSet::getNumTiles() const
		{
			irr::u32 numRows = Texture->getSize().Height / TileHeight;
			return NumColumns * numRows;
		}

		const irr::s32 CTileSet::getTileSetID() const
		{
			return TileSetID;
		}

		const irr::u32 CTileSet::getTileWidth() const
		{
			return TileWidth;
		}

		void CTileSet::setTileWidth(irr::u32 tileWidth)
		{
			TileWidth = tileWidth;
		}

		const irr::u32 CTileSet::getTileHeight() const
		{
			return TileHeight;
		}

		void CTileSet::setTileHeight(irr::u32 tileHeight)
		{
			TileHeight = tileHeight;
		}
	}	// end tile namespace
}	//end zen namespace