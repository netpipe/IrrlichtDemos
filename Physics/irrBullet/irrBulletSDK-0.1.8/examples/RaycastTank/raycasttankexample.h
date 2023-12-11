#ifndef __RAYCAST_VEHICLE_EXAMPLE_H_INCLUDED__
#define __RAYCAST_VEHICLE_EXAMPLE_H_INCLUDED__

#include "../framework/exampleframework.h"
#include <irrString.h>


class IRigidBody;
class IRaycastVehicle;

namespace irr
{
    namespace scene
    {
        class IBoneSceneNode;
    }
}

class CRaycastTankExample : public CExampleFramework
{
    public:
        CRaycastTankExample();
        virtual ~CRaycastTankExample();

        void updateTank();

        void createTerrain();

        void createTank(const irr::core::stringw file, const irr::core::stringw collFile,
            const irr::core::vector3df &pos, irr::f32 mass);
        void createMuzzleFlash(irr::scene::IBoneSceneNode *node);

        void runExample();

        bool OnEvent(const irr::SEvent& event);

        bool IsKeyDown(irr::EKEY_CODE keyCode) const
        {
            return KeyIsDown[keyCode];
        }

    private:
        IRigidBody *tank;
        IRaycastVehicle *vehicle;

        bool KeyIsDown[irr::KEY_KEY_CODES_COUNT];

        irr::f32 leftTrackSpeed;
        irr::f32 rightTrackSpeed;

};

#endif // __RAYCAST_VEHICLE_EXAMPLE_H_INCLUDED__
