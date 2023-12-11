#ifndef __RAYCAST_VEHICLE_EXAMPLE_H_INCLUDED__
#define __RAYCAST_VEHICLE_EXAMPLE_H_INCLUDED__

#include "../framework/exampleframework.h"
#include <irrString.h>
#include <irrTypes.h>

#include "irrlicht.h"
using namespace irr;


class IRigidBody;

class CHelloWorldExample : public CExampleFramework
{
    public:
        CHelloWorldExample();
        virtual ~CHelloWorldExample();

        virtual void runExample();
//void convertIrrMeshBufferBtTriangleMesh(scene::IMeshBuffer *meshBuffer, btTriangleMesh *collisionMesh, core::vector3df scale);
        virtual bool OnEvent(const irr::SEvent& event);

        void createBoxes();

        void createGround();

        void handle_input(core::array<SJoystickInfo> joystickInfo);
        void logic();

    private:
        irr::u32 rows;
        irr::u32 columns;
        core::vector3df translation, rotation;


};

#endif // __RAYCAST_VEHICLE_EXAMPLE_H_INCLUDED__
