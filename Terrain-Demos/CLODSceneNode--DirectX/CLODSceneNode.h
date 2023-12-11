//!
//! CLODSCENENODE
//! Irrlicht scenenode for loading multiple mesh lods
//!
//!
//! By Paulo Rogério de Oliveira ( paulo_cmv@hotmail.com < MSN - EMAIL > )
//! ----------------------------
//! You can use it for any purpose, as you want, only dont remove this notice.
//! If you make any changes, improvements, or you have comments, sugestions,
//! you can report it back by mail or msn. This way i can try to improve the code.


#ifndef CLODSCENENODE_H_INCLUDED
#define CLODSCENENODE_H_INCLUDED

#include "irrlicht.h"
using namespace irr;

class CLODMesh
{
public:
    video::SColor color;
private:
    f32 fadeRate;
    ITimer* tmr;
    u32 lastTick;
    scene::IMesh* mesh;
    s32 PassCount;
    core::aabbox3d<f32> BBox;
    s32 transp;
    scene::ISceneManager* SceneManager;
    video::IVideoDriver* driver;
    core::array<video::SMaterial> materials;
    f32 maxViewDistance;
    bool isBillBoard;

    void performFading();

    void getAllMaterials();

    void setVertexColors( scene::IMeshBuffer* mb );
public:

    friend class CLODSceneNode;

    CLODMesh( IrrlichtDevice* device, scene::IMesh* Mesh );

    ~CLODMesh();

    f32 getViewDistance();

    void setViewDistance( f32 newViewDistance );

    s32 getMaterialCount();

    video::SMaterial& getMaterial(s32 i);

    void setMaterialType(video::E_MATERIAL_TYPE newType);

    scene::IMesh* getMesh();

    s32 getTransparency();

    void setTransparency( s32 newTransparency );

    void setMaterialTexture(s32 textureLayer, video::ITexture* texture);

    const core::aabbox3d<f32>& getBoundingBox() const;

    void render();

    void FadeIn( f32 newFadeRate );

    void FadeOut( f32 newFadeRate );
};


class CLODSceneNode : public scene::ISceneNode
{
private:
    IrrlichtDevice* device;
    ITimer* tmr;
    u32 lastTick;

    core::list< CLODMesh* > LODs;
    s16 currentLOD;
    video::IVideoDriver* driver;
    core::aabbox3d<f32> BBox;
    s32 StandardMaterialType;
    video::SColor color;

public:

    CLODSceneNode( ISceneNode* parent,
                      IrrlichtDevice* Device,
                      s32 id,
                      const core::vector3df& position,
                      const core::vector3df& rotation,
                      const core::vector3df& scale );

    ~CLODSceneNode();

    s32 getMaterialCount();

    void setMaterialFlag(video::E_MATERIAL_FLAG flag, bool newvalue);

    const core::aabbox3d<f32>& getBoundingBox() const;

    void setColor( video::SColor newColor );

    void addLOD( scene::IMesh* newMesh, f32 maxViewDistance, bool BillBoard = false );

    CLODMesh* getLOD( s16 index );

    void OnPreRender();

    void render();
};

#endif

// rotate a node to look at target
////vector3df vect = (camera->getAbsolutePosition()) -
////(node->getAbsolutePosition());
////vect = vect.getHorizontalAngle();
////vect.Y -= 90;
////vect.Z = 0;
////vect.X = 0;
////node->setRotation(vect);
