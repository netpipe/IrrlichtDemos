/*  
    Written by Asger Feldthaus
    
    February 2007
*/

#ifndef _C_BILLBOARD_GROUP_SCENE_NODE_H_
#define _C_BILLBOARD_GROUP_SCENE_NODE_H_

#include <ISceneNode.h>
#include <SMeshBuffer.h>

namespace irr
{
namespace scene
{

/*!
    \brief Group of billboards.
    
    Updates and displays multiple billboards. The billboards can not be removed or moved around after they are added.
    Billboards may be added as free billboards or axis-aligned billboards.
    
    Small optimizations are made when the billboards reach a certain distance from the camera.
 */
class CBillboardGroupSceneNode : public ISceneNode
{
public:
    //! Standard constructor for scene nodes
    //! Contains no billboards by default
    CBillboardGroupSceneNode( ISceneNode* parent, ISceneManager* manager, s32 id=-1,
            const core::vector3df& position = core::vector3df(0,0,0),
            const core::vector3df& rotation = core::vector3df(0,0,0),
            const core::vector3df& scale = core::vector3df(1,1,1) );
    
    //! Adds a billboard.
    void addBillboard( const core::vector3df& position, const core::dimension2df& size, f32 roll=0.0f, video::SColor vertexColor=video::SColor(255,255,255,255) );
    
    //! Adds a billboard resitricted to an axis.
    void addBillboardWithAxis( const core::vector3df& position, const core::dimension2df& size, const core::vector3df& axis, f32 roll=0.0f, video::SColor vertexColor=video::SColor(255,255,255,255) );
    
    s32 getBillboardCount() const;
    
    virtual void OnRegisterSceneNode();
    
    virtual void render();
    
    const core::aabbox3df& getBoundingBox() const;
    
    video::SMaterial& getMaterial(u32 i);
    
    u32 getMaterialCount() const;
    
    //! Sets the vertex colors to simulate shadows on the billboards. The entire group will be treated as the upper half of
    //! a big sphere and every billboard is lighted acoording to its position on the sphere.
    //! Should be called AFTER the billboards have been added. To update new billboards, this method must be called again.
    //! This may be called many times, if the light setting should change.
    //! \param lightDir: The incoming light direction. This vector must be normalized (have length 1).
    //! \param intensity: The light intensity, typically between 0 and 1, although values greater than 1 may be used.
    //! \param ambient: The ambient light intensity. Between 0 and 1. Typically zero or close to zero.
    void applyVertexShadows( const core::vector3df& lightDir, f32 intensity, f32 ambient );
    
    //! Gives vertices back their original color.
    void resetVertexShadows();
    
private:
    void updateBillboards();
    
    f32 FarDistance;
    f32 Radius;
    
    core::vector3df LastCamDir;
    
    core::aabbox3df BoundingBox;
    video::SMaterial Material;
    
    struct SBillboard
    {
        core::vector3df Position;
        core::dimension2df Size;
        f32 Roll;
        core::vector3df Axis;
        bool HasAxis;
        video::SColor Color;
        
        SBillboard() {}
        SBillboard( const core::vector3df& pos, const core::dimension2df& size, f32 roll, video::SColor color )
            : Position(pos), Size(size), Roll(roll), HasAxis(false), Color(color) {}
    };
    
    core::array<SBillboard> Billboards; // A billboard's index is proportional to its index in the mesh buffer
    
    SMeshBuffer MeshBuffer;
};

} // namespace scene
} // namespace irr

#endif
