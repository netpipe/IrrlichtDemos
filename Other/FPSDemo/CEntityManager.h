#ifndef _CENTITYMANAGER_H_
#define _CENTITYMANAGER_H_

#include <irrlicht.h>

#include "CDefaultEntityFactory.h"
#include "CBaseEntity.h"
#include "CPositionalAudioEntity.h"
#include "CPlayerEntity.h"

using namespace irr;
using namespace scene;

namespace agEngine
{
    namespace entities
    {
        class CBaseEntity;
        class CEntityFactory;

        class CEntityManager : public virtual IReferenceCounted, CBaseEntity
        {
        public:
            CEntityManager(scene::ISceneManager* smgr, io::IFileSystem* fs, irr::ILogger* logger);
            virtual ~CEntityManager();

            virtual void addEntity(CBaseEntity* ent);
            virtual void updateAll();

            virtual void clear();

            virtual void registerEntityFactory(CEntityFactory* emgr);

            virtual u32 getEntityCount() const;



            virtual bool saveEntities(const c8* filename);
            virtual bool saveEntities(io::IWriteFile* file);

            virtual bool loadEntities(const c8* filename);
            virtual bool loadEntities(io::IReadFile* file);

            virtual void writeEntities(io::IXMLWriter* writer, CBaseEntity* entity);
            virtual void readEntities(io::IXMLReader* reader);

            virtual const c8* getEntityTypeName(ENTITY_TYPE type) const;
            virtual CBaseEntity* getClosestEntity(CBaseEntity* client);
            virtual CBaseEntity* getClosestEntityOfSameType(CBaseEntity* client);
            virtual CBaseEntity* getTargetedEntity(CBaseEntity* client);
            virtual CBaseEntity* getEntityAssociatedWithNode(ISceneNode* node);
            virtual CBaseEntity* getEntityByName(const core::stringc& name);

            // Adding different entities
            virtual CBaseEntity* addBaseEntity();
            virtual CPositionalAudioEntity* addPositionalAudioEntity();
            virtual CPlayerEntity* addPlayerEntity();
        private:

            core::array<CBaseEntity*> EntityList;

            io::IFileSystem* FileSystem;
            scene::ISceneManager* SceneManager;
            ILogger* Logger;

            core::array<CEntityFactory*> EntityFactoryList;

            const core::stringw IRR_XML_FORMAT_ENTMANAGER;
            const core::stringw IRR_XML_FORMAT_ENTITY;
            const core::stringw IRR_XML_FORMAT_ENTITYAFTERTYPE;
        };
    }
}

#endif
