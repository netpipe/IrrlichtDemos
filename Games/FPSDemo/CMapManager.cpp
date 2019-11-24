#include "CMapManager.h"

namespace agEngine
{
    namespace maps
    {
        CMapManager::CMapManager(scene::ISceneManager* sceneMgr, entities::CEntityManager* entityMgr)
        : smgr(sceneMgr), emgr(entityMgr)
        {
            if (sceneMgr)
                sceneMgr->grab();

            if (emgr)
                emgr->grab();
        }

        CMapManager::~CMapManager()
        {
            if (smgr)
                smgr->drop();

            if (emgr)
                emgr->drop();
        }

        bool CMapManager::loadMap(c8* filename)
        {
            return false;
        }

        bool CMapManager::loadMap(io::IReadFile* file)
        {
            return false;
        }

        bool CMapManager::saveMap(c8* filename)
        {
            return false;
        }

        bool CMapManager::saveMap(io::IWriteFile* file)
        {
            return false;
        }

        bool CMapManager::loadMapState(c8* filename)
        {
            return false;
        }

        bool CMapManager::loadMapState(io::IReadFile* file)
        {
            return false;
        }

        bool CMapManager::saveMapState(c8* filename)
        {
            return false;
        }

        bool CMapManager::saveMapState(io::IWriteFile* file)
        {
            return false;
        }
    }
}
