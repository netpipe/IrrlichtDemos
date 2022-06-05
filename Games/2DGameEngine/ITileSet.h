#ifndef _I_TILE_SET_
#define _I_TILE_SET_

#include "irrlicht.h"
#include "IReferenceCounter.h"

namespace zen
{
	namespace tile
	{
		class ITileSet : public virtual IReferenceCounter
		{
		public:

			virtual ~ITileSet() {};

			virtual irr::core::rect<irr::s32> getTile(irr::s32 tileID) const = 0;

			virtual irr::u32 getNumTiles() const = 0;

			virtual const irr::video::ITexture * getTileSetTexture() const = 0;

			virtual bool setTileSet(irr::video::ITexture * tileSet, irr::u32 tileWidth, 
				irr::u32 tileHeight, irr::s32 tileSetID) = 0;

			virtual const irr::s32 getTileSetID() const = 0;

			virtual const irr::u32 getTileWidth() const = 0;

			virtual void setTileWidth(irr::u32 tileWidth) = 0;

			virtual const irr::u32 getTileHeight() const = 0;

			virtual void setTileHeight(irr::u32 tileHeight) = 0;
		};
	}
}

#endif