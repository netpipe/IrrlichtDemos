#ifndef _C_TILE_NODE_
#define _C_TILE_NODE_

#include "ITileNode.h"

namespace zen
{
	namespace tile
	{
		class CTileNode : public virtual ITileNode
		{
		public:

			CTileNode(irr::core::vector2df & position, irr::s32 tileSetID, irr::s32 tileID, irr::s32 nodeID, E_TILE_FLAG flags = ID_isVisibile);

			virtual ~CTileNode() {};

			virtual const irr::s32 getTileID() const;

			virtual const irr::s32 getTileSetID() const;

			virtual const irr::u32 getTileFlag(E_TILE_FLAG flag) const;

			virtual const irr::s32 getTileDepth() const;

			virtual const irr::s32 getNodeID() const;

			virtual const irr::core::vector2df getNodePosition() const;

			virtual bool setTileIDs(irr::s32 tileSetID, irr::s32 tileID);

			virtual void setTileFlag(E_TILE_FLAG flag, bool enabled);

			virtual void setTileDepth(irr::s32 tileDepth);

			virtual bool setNodePosition(const irr::core::vector2df & position);	//unfinished

		protected:

			virtual bool setNodeID(irr::s32 nodeID);	//unfinished

		private:

			irr::u32 TileFlags;
			irr::s32 TileDepth;
			irr::s32 TileSetID;
			irr::s32 TileID;
			irr::s32 NodeID;
			irr::core::vector2df NodePosition;
		};
	}
}

#endif