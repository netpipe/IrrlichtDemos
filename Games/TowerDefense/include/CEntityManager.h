#ifndef CENTITYMANAGER_H
#define CENTITYMANAGER_H

#include <irrlicht.h>

class CEntity;
class CStateManager;
class CComponentFactory;
class CComponent;
class CEntityManager
{
    public:
        /** Default constructor */
        CEntityManager(CStateManager* manager);

        /** Default destructor */
        virtual ~CEntityManager();

        CStateManager* getManager(void);

        CEntity* createEntity(const irr::c8* name);

        void addEntityToDeletenList(CEntity* entity);

        void update(irr::f32 timeDiff);

        void registerComponent(CComponentFactory* factory);

        CComponent* addComponentToEntity(const irr::core::stringc& name, CEntity* entity);
    protected:
        CStateManager* StateManager;
        irr::core::list<CEntity*> Entities;
        irr::core::list<CEntity*> Entities_to_Delete;
        irr::core::map<irr::core::stringc, CComponentFactory*> ComponentFactories;
    private:
};

#endif // CENTITYMANAGER_H
