#ifndef _I_TILE_NODE_
#define _I_TILE_NODE_

#include "irrlicht.h"
#include "IReferenceCounter.h"

namespace zen
{
	namespace tile
	{
		enum E_TILE_FLAG
		{
			ID_isSolid = 1,
			ID_isVisibile = 2,
			ID_isDefault = 4,
			ID_isAnimated = 8
		};

		class ITileNode : public virtual IReferenceCounter
		{
		public:

			virtual ~ITileNode() {};

			virtual const irr::s32 getTileID() const = 0;

			virtual const irr::s32 getTileSetID() const = 0;

			virtual const irr::u32 getTileFlag(E_TILE_FLAG) const = 0;

			virtual const irr::s32 getTileDepth() const = 0;

			virtual const irr::s32 getNodeID() const = 0;

			virtual const irr::core::vector2df getNodePosition() const = 0;

			virtual bool setTileIDs(irr::s32 tileSetID, irr::s32 tileID) = 0;

			virtual void setTileFlag(E_TILE_FLAG flag, bool enabled) = 0;

			virtual void setTileDepth(irr::s32 tileDepth) = 0;

			virtual bool setNodePosition(const irr::core::vector2df & position) = 0;

		protected:

			virtual bool setNodeID(irr::s32 nodeID) = 0;
		};
	}
}

#endif