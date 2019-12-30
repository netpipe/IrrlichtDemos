/*  
    Written by Asger Feldthaus
    
    February 2007
*/

#ifndef _C_TREE_SCENE_NODE_H_
#define _C_TREE_SCENE_NODE_H_

#include <ISceneNode.h>
#include "CBillboardGroupSceneNode.h"
#include "CTreeGenerator.h"

namespace irr
{
namespace scene
{

/*!
    \brief A tree with three levels of detail.
    
    CTreeSceneNode is a tree with three levels of detail. The highest LOD is the mesh with full detail. The second LOD is another mesh with fewer polygons.
    The last LOD is a billboard. The leaves are displayed in the two first levels of detail.
    
    Call setup() to initialize the scene node.
*/
class CTreeSceneNode : public ISceneNode
{
public:
    //! Standard constructor for scene nodes. Call setup() to initialize the scene node.
    CTreeSceneNode( ISceneNode* parent, ISceneManager* manager, s32 id=-1,
        const core::vector3df& position = core::vector3df(0,0,0),
        const core::vector3df& rotation = core::vector3df(0,0,0),
        const core::vector3df& scale = core::vector3df(1,1,1) );
    
    virtual ~CTreeSceneNode();
    
    //! Sets the meshes used by the scene node, the leaf node used and the billboard texture.
    //! \param highLod: The mesh to use when the tree is close to the camera. Must not be 0!
    //! \param midLod: The mesh to use when the camera is at mediocre distance. If 0 highLod is used instead.
    //! \param leafNode: The scene node displaying the leaves on the tree. If 0 no leaves will be displayed.
    //! \param billboardTexture: The texture to display on the billboard when the tree is very far from the camera. If 0 it will not turn into a billboard.
    void setup( const STreeMesh* highLOD, const STreeMesh* midLOD, video::ITexture* billboardTexture=0 );
    
    //! Generates meshes and leaves for the scene node automatically, and specifies the billboard texture.
    //! \param seed: The random number seed used to generate the tree.
    //! \param billboardTexture: The texture to display on the billboard when the tree is very far from the camera. If 0 it will not turn into a billboard.
    void setup( CTreeGenerator* generator, s32 seed=0, video::ITexture* billboardTexture=0 );
    
    //! Specifies where the tree should switch to medium LOD, and where it should switch to a billboard.
    //! \param midRange: If the camera is closer than this, it will use high LOD. If it is greater than this, but lower than farRange, it will use medium LOD.
    //! \param farRange: If the camera is further away than this, a billboard will be displayed instead of the tree.
    void setDistances( f32 midRange, f32 farRange );
    
    virtual void OnRegisterSceneNode();
    
    virtual void render();
    
    virtual video::SMaterial& getMaterial(u32 i);
    
    virtual u32 getMaterialCount() const;
    
    virtual const core::aabbox3d<f32>& getBoundingBox() const;
    
    s32 getVertexCount() const;
    
    //! Returns the leaf node associated with the tree. Useful for settings the correct texture and material settings.
    CBillboardGroupSceneNode* getLeafNode();
    
private:
    void updateBillboard();
    void updateSizeAndRange();
    
    video::SMaterial TreeMaterial;
    video::SMaterial BillboardMaterial;
    
    f32 MidRange;
    f32 FarRange;
    f32 Size;
    
    CBillboardGroupSceneNode* LeafNode;
    
    SMeshBuffer* HighLODMeshBuffer;
    SMeshBuffer* MidLODMeshBuffer;
    
    SMeshBuffer BillboardMeshBuffer;
    
    f64 DistSQ;
    
    core::vector3df LastViewVec;
};

} // namespace scene
} // namespace irr

#endif
