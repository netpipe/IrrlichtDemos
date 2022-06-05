#ifndef CPLAYER_H
#define CPLAYER_H

#include <irrlicht.h>
#include <CState.h>
#include <CInput_catcher.h>
#include <CMap.h>
#include <CSpatialHash.h>

class gui_catch;
class CSprite;
class CStateManager;
class CEntityManager;
class CMap;
class Base_Property;
class CPlayer : public IMouseReceiver, public IKeyboardReceiver
{
    public:
        CPlayer(CStateManager* manager, CEntityManager* entityManager, CMap* map, CSpatialHash<>* unitGrid);
        virtual ~CPlayer();

        void init(void);
        void deinit(void);

        void update(irr::f32 timediff);

        //input
        void key_event(const irr::SEvent::SKeyInput& key);
        void mouse_event(const irr::SEvent::SMouseInput& mouse);
    protected:
        CStateManager* Manager;
        CState* GameState;
        CEntityManager* EntityManager;
        CMap* Map;
        CSpatialHash<>* UnitGrid;

        int Money;
        int LifePoints;
        float TimePassed;

        irr::gui::IGUIStaticText* Info;

        //buildmenu
        gui_catch* BuildMenu;
        irr::core::vector2di TileUnderMoue;
        CSprite* SelectedSprite;
        CSprite* BluePause;
        CSprite* RangeIdentifier;
        E_TOWER Tower2Build;
        bool CanBuild;

        //events
        void EnemyKilled(Base_Property* property, CEntity* sender);
        void EnemyWin(Base_Property* property, CEntity* sender);
    private:
};

#endif // CPLAYER_H
