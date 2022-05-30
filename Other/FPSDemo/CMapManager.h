#ifndef _CMAPMANAGER_H_
#define _CMAPMANAGER_H_

#include <irrlicht.h>
#include "CEntityManager.h"

using namespace irr;

namespace agEngine
{
    namespace maps
    {
        class CMapManager : public virtual IReferenceCounted
        {
        public:
            CMapManager(scene::ISceneManager* smgr, entities::CEntityManager* emgr);
            virtual ~CMapManager();

            virtual bool loadMap(c8* filename);
            virtual bool loadMap(io::IReadFile* file);
            virtual bool saveMap(c8* filename);
            virtual bool saveMap(io::IWriteFile* file);

            virtual bool loadMapState(c8* filename);
            virtual bool loadMapState(io::IReadFile* file);
            virtual bool saveMapState(c8* filename);
            virtual bool saveMapState(io::IWriteFile* file);
        private:
            scene::ISceneManager* smgr;
            entities::CEntityManager* emgr;
        };
    }
}

#endif
