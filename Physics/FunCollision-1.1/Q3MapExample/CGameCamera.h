#ifndef CGAMECAMERA_H_INCLUDED
#define CGAMECAMERA_H_INCLUDED

#include <irrlicht.h>
using namespace irr;

#include "FunCollision.h"
using namespace COLL;

#include "CGameInput.h"


//! CGameCamera class
class CGameCamera
{
    public:
        //! constructor
        CGameCamera(IrrlichtDevice* Device, ICollManager* Cmgr, CGameInput *Input,
                    const core::vector3df& Pos, const core::vector3df& Scale, const core::vector3df& Origin);

        //! getCamEntity
        ICollEntity* getCamEntity() const { return camEntity; }

        //! getCamNode
        scene::ICameraSceneNode* getCamNode() const { return cam; }

        //! update
        void update();

    private:
        // instances
        IrrlichtDevice* device;
        scene::ISceneManager* smgr;
        video::IVideoDriver* driver;
        ICollManager* cmgr;
        CGameInput* input;

        // camera
        scene::ICameraSceneNode* cam;
        ICollEntity* camEntity;
        core::vector3df camPos, camRot;

}; // end class CGameCamera


#endif // CGAMECAMERA_H_INCLUDED
