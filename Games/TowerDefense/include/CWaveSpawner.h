#ifndef CWAVESPAWNER_H
#define CWAVESPAWNER_H

#include <irrlicht.h>

class CEntityManager;
class CWaveSpawner
{
    public:
        CWaveSpawner(CEntityManager* manager, irr::core::vector3df startpos);
        virtual ~CWaveSpawner();

        void update(float timediff);
    protected:
        CEntityManager* Manager;
        irr::core::vector3df StartPos;
        float Timer;
        float TimeToSpawn;
        int Wave;
        int UnitsToSpawn;
        irr::gui::IGUIStaticText* Info;
    private:
};

#endif // CWAVESPAWNER_H
