#include "CTileNode.h"

namespace zen
{
	namespace tile
	{
		CTileNode::CTileNode(irr::core::vector2df & position, irr::s32 tileSetID, irr::s32 tileID, irr::s32 nodeID, E_TILE_FLAG flags)
			: NodePosition(position), TileSetID(tileSetID), TileID(tileID), NodeID(nodeID), TileFlags(flags)
		{}

		const irr::s32 CTileNode::getTileID() const
		{
			return TileID;
		}

		const irr::s32 CTileNode::getTileSetID() const
		{
			return TileSetID;
		}

		const irr::s32 CTileNode::getTileDepth() const
		{
			return TileDepth;
		}

		const irr::u32 CTileNode::getTileFlag(E_TILE_FLAG flag) const
		{
			return (TileFlags & flag);
		}

		const irr::s32 CTileNode::getNodeID() const
		{
			return NodeID;
		}

		const irr::core::vector2df CTileNode::getNodePosition() const
		{
			return NodePosition;
		}

		bool CTileNode::setTileIDs(irr::s32 tileSetID, irr::s32 tileID)
		{
			if (tileID >= 0 && tileSetID >=0)
			{
				TileSetID = tileSetID;
				TileID = tileID;
				return true;
			}
			else
				return false;
		}

		void CTileNode::setTileFlag(E_TILE_FLAG flag, bool enabled)
		{
			if (enabled)
				TileFlags = TileFlags | flag;
			else
				TileFlags = TileFlags & ~flag;
		}

		void CTileNode::setTileDepth(irr::s32 tileDepth)
		{
			TileDepth = tileDepth;
		}

		bool CTileNode::setNodeID(irr::s32 nodeID)	// unfinished (check to make sure no nodes have same ID)
		{
			NodeID = nodeID;
			return true;
		}

		bool CTileNode::setNodePosition(const irr::core::vector2df & position)	// unfinished (check bounds)
		{
			NodePosition = position;
			return true;
		}
	}
}