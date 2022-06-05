#include "CTileNode.h"
#include "CTileSet.h"
#include "CTileManager.h"

namespace zen
{
	namespace tile
	{
		CTileManager::CTileManager() : TileNodes(0), TileSets(0), NextTileSetID(0), NextNodeID(0)
		{

		}

		CTileManager::~CTileManager()
		{
			for (irr::u32 i = 0; i < TileSets.size(); ++i)
				TileSets[i]->drop();
			for (irr::u32 i = 0; i < TileNodes.size(); ++i)
				TileNodes[i]->drop();
		}

		void CTileManager::addTileSet(irr::video::ITexture * tileSet, irr::u32 tileWidth,
										irr::u32 tileHeight)
		{
			CTileSet * newTileSet = new CTileSet(tileSet, tileWidth, tileHeight, NextTileSetID);
			newTileSet->grab();
			++NextTileSetID;
			TileSets.push_back(newTileSet);
		}

		void CTileManager::addTileNode(irr::core::vector2df & position, irr::s32 tileSetID, irr::s32 tileID)
		{
			CTileNode * newTileNode = new CTileNode(position, tileSetID, tileID, NextNodeID);
			newTileNode->grab();
			++NextNodeID;
			TileNodes.push_back(newTileNode);
		}

		ITileNode * CTileManager::getTileNode(irr::s32 tileNodeID) const	// return struct of node info instead of actual node?
		{
			for (irr::u32 i = 0; i < TileNodes.size(); ++i)
			{
				if (tileNodeID == TileNodes[i]->getNodeID())
				{
					return TileNodes[i];
				}
			}
			return 0;
		}

		ITileSet * CTileManager::getTileSet(irr::s32 tilesetID) const	// return struct of node info instead of actual node?
		{
			for (irr::u32 i = 0; i < TileSets.size(); ++i)
			{
				if (tilesetID == TileSets[i]->getTileSetID())
				{
					return TileSets[i];
				}
			}
			return 0;
		}

		void CTileManager::draw() const
		{

		}
	}
}
