#include "CEntityManager.h"

namespace agEngine
{
    namespace entities
    {
        CEntityManager::CEntityManager(scene::ISceneManager* smgr, io::IFileSystem* fs, ILogger* logger)
        : CBaseEntity(), FileSystem(fs), SceneManager(smgr), Logger(logger), IRR_XML_FORMAT_ENTMANAGER(L"entity_manager"), IRR_XML_FORMAT_ENTITY(L"entity"), IRR_XML_FORMAT_ENTITYAFTERTYPE(L"type")
        {
            if (SceneManager)
                SceneManager->grab();

            if (FileSystem)
                FileSystem->grab();

            if (Logger)
                Logger->grab();

            CEntityFactory* factory = new CDefaultEntityFactory(this);
            registerEntityFactory(factory);
            factory->drop();
        }

        CEntityManager::~CEntityManager()
        {
            for (u32 i = 0; i < EntityFactoryList.size(); ++i)
                EntityFactoryList[i]->drop();

            EntityFactoryList.clear();
            clear();

            if (Logger)
                Logger->drop();

            if (FileSystem)
                FileSystem->drop();

            if (SceneManager)
                SceneManager->drop();
        }

        void CEntityManager::addEntity(CBaseEntity* ent)
        {
            EntityList.push_back(ent);
            ent->grab();
        }

        void CEntityManager::updateAll()
        {
            for (u32 i = 0; i < EntityList.size(); ++i)
                EntityList[i]->update(this);
        }

        void CEntityManager::clear()
        {
            for (u32 i = 0; i < EntityList.size(); ++i)
                EntityList[i]->drop();

            EntityList.clear();
        }

        void CEntityManager::registerEntityFactory(CEntityFactory* emgr)
        {
            EntityFactoryList.push_back(emgr);
            emgr->grab();
        }

        u32 CEntityManager::getEntityCount() const
        {
            return EntityList.size();
        }


        bool CEntityManager::saveEntities(const c8* filename)
        {
            bool ret = false;
            io::IWriteFile* file = FileSystem->createAndWriteFile(filename);
            if (file)
            {
                ret = saveEntities(file);
                file->drop();
            }
            _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
            return ret;
        }

        bool CEntityManager::saveEntities(io::IWriteFile* file)
        {
            if (!file)
            {
                _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
                return false;
            }

            io::IXMLWriter* writer = FileSystem->createXMLWriter(file);
            if (!writer)
            {
                _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
                return false;
            }

            writer->writeXMLHeader();
            writeEntities(writer, this);
            writer->drop();

            return true;
        }

        void CEntityManager::writeEntities(io::IXMLWriter* writer, CBaseEntity* entity)
        {
            if (!writer || !entity)
                return;

            const wchar_t* name;

            if (entity == this)
            {
                name = IRR_XML_FORMAT_ENTMANAGER.c_str();
                writer->writeElement(name, false);
            }
            else
            {
                name = IRR_XML_FORMAT_ENTITY.c_str();
                writer->writeElement(name, false, IRR_XML_FORMAT_ENTITYAFTERTYPE.c_str(), core::stringw(getEntityTypeName(entity->getEType())).c_str());
            }

            writer->writeLineBreak();
            writer->writeLineBreak();

            io::IAttributes* attr = FileSystem->createEmptyAttributes();
            entity->serializeParameters(attr);

            if (attr->getAttributeCount() != 0)
            {
                attr->write(writer, false);
                writer->writeLineBreak();
            }

            attr->drop();

            writer->writeClosingTag(name);
            writer->writeLineBreak();
            writer->writeLineBreak();

            // write the rest of the entities
            if (entity == this)
            {
                u32 i;
                for (i = 0; i < EntityList.size(); ++i)
                    writeEntities(writer, EntityList[i]);
            }
        }

        bool CEntityManager::loadEntities(const c8* file)
        {
            bool ret = false;

            io::IReadFile* read = FileSystem->createAndOpenFile(file);
            if (!read)
            {

            }
            else
            {
                ret = loadEntities(read);
                read->drop();
            }

            _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
            return ret;
        }

        bool CEntityManager::loadEntities(io::IReadFile* file)
        {
            if (!file)
            {
                _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
                return false;
            }

            io::IXMLReader* reader = FileSystem->createXMLReader(file);
            if (!reader)
            {
                _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
                return false;
            }

            while (reader->read())
            {
                readEntities(reader);
            }

            reader->drop();
            return true;
        }

        // Reads the entities from the provided XML
        void CEntityManager::readEntities(io::IXMLReader* reader)
        {
            // Ensure that the reader is a valid pointer
            if (!reader)
                return;

            // Creating a pointer for entity creation
            CBaseEntity* entity = 0;

            // Check if it's the entity manager tag, and if so, set the entity to it
            if (IRR_XML_FORMAT_ENTMANAGER==reader->getNodeName())
                entity = this;

            // Otherwise it's a normal entity tag
            else if (IRR_XML_FORMAT_ENTITY==reader->getNodeName())
            {
                // Get the "type" attribute from the entity tag
                core::stringc attrName = reader->getAttributeValue(IRR_XML_FORMAT_ENTITYAFTERTYPE.c_str());

                // Go through all the entity factories, and ask them to make this entity. Only 1 factory should respond
                for (s32 i = (s32)EntityFactoryList.size() - 1; i >= 0 && !entity; --i)
                {
                    entity = EntityFactoryList[i]->addEntity(attrName.c_str());
                    if (entity)
                        break;
                }

                // If after having all the factories have had their go and entity is not valid, something is messed up
                if (!entity)
                    Logger->log("Could not create entity node of unknown type", attrName.c_str());
            }

            // If entity hasn't been resolved yet, get out of the function
            if (!entity)
                return;

            // From here on, we're going to read every line after the entity tag
            while(reader->read())
            {
                // Switch to see if we have reached the end
                bool endreached = false;

                // Find out what kind of node it is
                switch (reader->getNodeType())
                {
                // Is this an end of entity tag? "</entity>"
                case io::EXN_ELEMENT_END:
                    // Is this an <entity> or an <entity_manager> ?
                    if ((IRR_XML_FORMAT_ENTITY==reader->getNodeName()) ||
                        (IRR_XML_FORMAT_ENTMANAGER==reader->getNodeName()))
                    {
                        // We've reached the end of the line for this entity
                        endreached = true;
                    }
                    break;
                // Are these regular elements within the entity?
                case io::EXN_ELEMENT:
                    // Are we at the attributes elements?
                    if (core::stringw(L"attributes")==reader->getNodeName())
                    {
                        // Create an empty set of attributes
                        io::IAttributes* attr = FileSystem->createEmptyAttributes();
                        // Read all the attributes into the empty set
                        attr->read(reader);

                        // If we have a valid entity, let the entity deserialize all the parameters
                        if (entity)
                            entity->deserializeParameters(attr, SceneManager);

                        // Drop the pointer for the attribute
                        attr->drop();
                    }
                    break;
                // We don't care about anything else in the element
                default:
                    break;
                }

                // If we have reached the end, stop looping
                if (endreached)
                    break;

            }
        }

        const c8* CEntityManager::getEntityTypeName(ENTITY_TYPE type) const
        {
            const c8* name = 0;

            for (s32 i = (s32)EntityFactoryList.size()-1; !name && i>=0; --i)
                name = EntityFactoryList[i]->getCreateableEntityTypeName(type);

            return name;
        }

        CBaseEntity* CEntityManager::getClosestEntity(CBaseEntity* client)
        {
            f32 closestDistance = 0.0f;
            u32 closestEntity = 0;

            core::vector3d<f32> clientPosition(client->getPosition());

            for (u32 i = 0; i < EntityList.size(); ++i)
            {
                f32 tempDist = clientPosition.getDistanceFrom(EntityList[i]->getPosition());

                if (i == 0)
                {
                    closestDistance = tempDist;
                    closestEntity = i;
                    continue;
                }
                else if (tempDist < closestDistance)
                {
                    closestDistance = tempDist;
                    closestEntity = i;
                }
            }

            if (closestEntity)
                return EntityList[closestEntity];

            return NULL;
        }

        CBaseEntity* CEntityManager::getClosestEntityOfSameType(CBaseEntity* client)
        {
            f32 closestDistance = 0.0f;
            u32 closestEntity = 0;

            core::vector3d<f32> clientPosition(client->getPosition());

            for (u32 i = 0; i < EntityList.size(); ++i)
            {
                if (EntityList[i]->getEType() == client->getEType())
                {
                    f32 tempDist = clientPosition.getDistanceFrom(EntityList[i]->getPosition());

                    if (i == 0)
                    {
                        closestDistance = tempDist;
                        closestEntity = i;
                        continue;
                    }
                    else if (tempDist < closestDistance)
                    {
                        closestDistance = tempDist;
                        closestEntity = i;
                    }
                }
            }

            if (closestEntity)
                return EntityList[closestEntity];

            return NULL;
        }

        CBaseEntity* CEntityManager::getTargetedEntity(CBaseEntity* client)
        {
            core::line3d<f32> ray;
            ray.start = client->getPosition();
            ray.end = ray.start + (client->getTarget() - ray.start).normalize() * 1000.0f;
            ISceneNode* targettedNode = SceneManager->getSceneCollisionManager()->getSceneNodeFromRayBB(ray);
            if (targettedNode)
            {
                return getEntityAssociatedWithNode(targettedNode);
            }

            return NULL;
        }

        CBaseEntity* CEntityManager::getEntityAssociatedWithNode(ISceneNode* node)
        {
            for (u32 i = 0; i < EntityList.size(); ++i)
            {
                if (EntityList[i]->getAssociatedSceneNode() == node)
                    return EntityList[i];
            }

            return NULL;
        }

        CBaseEntity* CEntityManager::getEntityByName(const core::stringc& name)
        {
            for (u32 i = 0; i < EntityList.size(); ++i)
            {
                if (EntityList[i]->getStringParam(nodeName) == name)
                    return EntityList[i];
            }

            return NULL;
        }


        CBaseEntity* CEntityManager::addBaseEntity()
        {
            CBaseEntity* ent = new CBaseEntity();
            EntityList.push_back(ent);
            ent->grab();
            return ent;
        }

        CPositionalAudioEntity* CEntityManager::addPositionalAudioEntity()
        {
            CPositionalAudioEntity* ent = new CPositionalAudioEntity();
            EntityList.push_back(ent);
            ent->grab();
            return ent;
        }

        CPlayerEntity* CEntityManager::addPlayerEntity()
        {
            CPlayerEntity* ent = new CPlayerEntity();
            EntityList.push_back(ent);
            ent->grab();
            return ent;
        }
    }
}
