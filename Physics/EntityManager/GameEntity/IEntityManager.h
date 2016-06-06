#ifndef _ENTITYMANAGER_H_
#define _ENTITYMANAGER_H_

/** entity licence is defined on "const char GameEntity::EntityLicence[]" **/
#include "Entity/GlobalEntity.h"

namespace GameEntity
{
    static const char EntityLicence[] = "by Magun on www.immortal-galaxy.com\nplease specified my web site on your modifed source and credit\nthis is a creative commons licence with by-nc-sa attribute\nhttp://creativecommons.org/licenses/by-nc-sa/2.0/\n";
    class IEntityManager
    {
        public:
            IEntityManager(irr::IrrlichtDevice *device);
            virtual ~IEntityManager( );
            void     drawAll();

            IEntity *addEntity(GamePhysique::OBJECT,unsigned int id,float mass=0.1f);
            IEntity *addEntity(GamePhysique::OBJECT,irr::scene::IMesh*,unsigned int id,float mass=0.1f);
            IEntity *addEntity(GamePhysique::OBJECT,irr::scene::IMeshSceneNode*,unsigned int id,float mass=0.1f);
            IEntity *addEmptyEntity(IEntity*,GamePhysique::OBJECT,unsigned int);
            void removeEntity (IEntity*entity);

            void saveEntityScene();
            void loadEntityScene(const char*);
            void*saveEntitySceneData();

            void setDebugMode(bool);//set all entity at debug or no debug mode
            bool isDebugMode() { return debug; }
            unsigned int    getNomberEntity() const;
            IEntity *getEntity    (unsigned int loc);
            IEntity *getEntityByID(unsigned int id );

            irr::IrrlichtDevice*getDevice()          { return device; }
            btDiscreteDynamicsWorld *getWorldPhysic(){ return phy->getWorldPhysic(); }

            //void addColisionToCamera();
            //void deleteColisionToCamera();
            void clearEntity();
        protected:
            std::list<IEntity*> ent;
            irr::u32 TimeStamp, DeltaTime;
        private:
            bool debug;
            irr::IrrlichtDevice *device;
            GamePhysique::Physique *phy;
    };
}

#endif // ENTITYMANAGER_H

