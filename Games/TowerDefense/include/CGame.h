#ifndef CGAME_H
#define CGAME_H

#include <CState.h>
#include <CInput_catcher.h>
#include <CMap.h>
#include <CSpatialHash.h>

class CEntityManager;
class gui_catch;
class CSprite;
class CEntity;
class CWaveSpawner;
class CPlayer;
class CGame : public CState, public IKeyboardReceiver
{
    public:
        /** Default constructor */
        CGame(CStateManager* manager);

        /** Default destructor */
        virtual ~CGame();

        void update(irr::f32 timediff);

        //input
        void key_event(const irr::SEvent::SKeyInput& key);
    protected:
        void becomeMainState(void);
        void becomeSlaveState(void);

        CEntityManager* EntityManager;
        CMap Map;
        CSpatialHash<> UnitGrid;

        CPlayer* Player;

        //Enemies
        CWaveSpawner* Spawner;
    private:
};

#endif // CGAME_H
