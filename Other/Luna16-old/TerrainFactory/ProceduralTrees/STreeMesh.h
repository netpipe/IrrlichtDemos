/*  
    Written by Asger Feldthaus
    
    February 2007
*/

#ifndef _S_TREE_MESH_H_
#define _S_TREE_MESH_H_

#include <irrArray.h>
#include <SMeshBuffer.h>
#include <vector3d.h>
#include <dimension2d.h>
#include <SMaterial.h>

namespace irr
{
namespace scene
{

struct STreeLeaf
{
    core::vector3df Position;
    video::SColor Color;
    core::vector3df Axis;
    bool HasAxis;
    core::dimension2df Size;
    f32 Roll;
};

struct STreeMesh : public IReferenceCounted
{
    STreeMesh() : MeshBuffer(0) {}
    
    virtual ~STreeMesh()
    {
        if ( MeshBuffer )
            MeshBuffer->drop();
    }
    
    void setMeshBuffer( SMeshBuffer* mb )
    {
        if ( mb )
            mb->grab();
        
        if ( MeshBuffer )
            MeshBuffer->drop();
        
        MeshBuffer = mb;
    }
    
    SMeshBuffer* MeshBuffer;
    
    core::array<STreeLeaf> Leaves;
};

} // namespace scene
} // namespace irr

#endif
