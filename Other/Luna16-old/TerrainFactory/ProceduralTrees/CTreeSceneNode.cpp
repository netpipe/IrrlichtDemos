/*
    Written by Asger Feldthaus

    February 2007
*/

#include "CTreeSceneNode.h"

#include <ICameraSceneNode.h>
#include <IVideoDriver.h>

namespace irr
{
namespace scene
{

const f32 CAMERA_UPDATE_DISTANCE = 50.0f;

CTreeSceneNode::CTreeSceneNode( ISceneNode* parent, ISceneManager* manager, s32 id,
        const core::vector3df& position,
        const core::vector3df& rotation,
        const core::vector3df& scale )
    : ISceneNode( parent, manager, id, position, rotation, scale )
{
    MidRange = 10.0f;
    FarRange = 1500.0f;
    Size = 0.0f;

    HighLODMeshBuffer = 0;
    MidLODMeshBuffer = 0;
    LeafNode = 0;

    BillboardMaterial.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
    BillboardMaterial.Lighting = false;

    BillboardMeshBuffer.Indices.push_back( 2 );
    BillboardMeshBuffer.Indices.push_back( 1 );
    BillboardMeshBuffer.Indices.push_back( 0 );

    BillboardMeshBuffer.Indices.push_back( 0 );
    BillboardMeshBuffer.Indices.push_back( 3 );
    BillboardMeshBuffer.Indices.push_back( 2 );

    BillboardMeshBuffer.Vertices.push_back( video::S3DVertex( 0,0,0,  0,1,0, video::SColor(255,255,255,255), 0,1 ) );
    BillboardMeshBuffer.Vertices.push_back( video::S3DVertex( 0,0,0,  0,1,0, video::SColor(255,255,255,255), 0,0 ) );
    BillboardMeshBuffer.Vertices.push_back( video::S3DVertex( 0,0,0,  0,1,0, video::SColor(255,255,255,255), 1,0 ) );
    BillboardMeshBuffer.Vertices.push_back( video::S3DVertex( 0,0,0,  0,1,0, video::SColor(255,255,255,255), 1,1 ) );

}

CTreeSceneNode::~CTreeSceneNode()
{
    if ( HighLODMeshBuffer )
        HighLODMeshBuffer->drop();

    if ( MidLODMeshBuffer )
        MidLODMeshBuffer->drop();
}

void CTreeSceneNode::setup( const STreeMesh* highLOD, const STreeMesh* midLOD, video::ITexture* billboardTexture )
{
    if ( HighLODMeshBuffer )
        HighLODMeshBuffer->drop();

    if ( MidLODMeshBuffer )
        MidLODMeshBuffer->drop();

    if ( LeafNode )
        LeafNode->remove();

    HighLODMeshBuffer = highLOD->MeshBuffer;
    MidLODMeshBuffer = midLOD->MeshBuffer;

    LeafNode = 0;
    if ( highLOD->Leaves.size() > 0 )
    {
        LeafNode = new CBillboardGroupSceneNode( this, SceneManager );
        LeafNode->drop();

        for ( u32 i=0; i<highLOD->Leaves.size(); i++ )
        {
            if ( highLOD->Leaves[i].HasAxis )
                LeafNode->addBillboardWithAxis( highLOD->Leaves[i].Position, highLOD->Leaves[i].Size, highLOD->Leaves[i].Axis, highLOD->Leaves[i].Roll, highLOD->Leaves[i].Color );
            else
                LeafNode->addBillboard( highLOD->Leaves[i].Position, highLOD->Leaves[i].Size, highLOD->Leaves[i].Roll, highLOD->Leaves[i].Color );
        }
    }

	BillboardMaterial.TextureLayer[0].Texture = billboardTexture;

    if ( HighLODMeshBuffer )
    {
        HighLODMeshBuffer->grab();
        Size = HighLODMeshBuffer->BoundingBox.getExtent().getLength() / 2.0f;
    }

    if ( MidLODMeshBuffer )
        MidLODMeshBuffer->grab();

    LastViewVec = core::vector3df(0,0,0);
}

void CTreeSceneNode::setup( CTreeGenerator* generator, s32 seed, video::ITexture* billboardTexture )
{
    STreeMesh* highLOD = generator->generateTree( 8, seed, true, 0 );
    STreeMesh* midLOD = generator->generateTree( 4, seed, false, 1 );

    setup( highLOD, midLOD, billboardTexture );

    highLOD->drop();
    midLOD->drop();
}

void CTreeSceneNode::setDistances( f32 midRange, f32 farRange )
{
    MidRange = midRange;
    FarRange = farRange;
}

void CTreeSceneNode::OnRegisterSceneNode()
{
    if ( IsVisible )
    {
        ICameraSceneNode* camera = SceneManager->getActiveCamera();

        DistSQ = 0.0f;
        if ( camera )
        {
            core::vector3df campos = camera->getAbsolutePosition();

            core::vector3df center = HighLODMeshBuffer->BoundingBox.getCenter();
            AbsoluteTransformation.rotateVect(center);

            center += getAbsolutePosition();

            DistSQ = (campos - center).getLengthSQ();
        }

        if ( LeafNode )
        {
            f32 far = FarRange + Size;
            LeafNode->setVisible( far*far >= DistSQ );
        }

        SceneManager->registerNodeForRendering( this );
    }
    ISceneNode::OnRegisterSceneNode();
}

void CTreeSceneNode::render()
{
    video::IVideoDriver* driver = SceneManager->getVideoDriver();

    f32 far = FarRange + Size;
    f32 mid = MidRange + Size;

	if ( far*far < DistSQ && BillboardMaterial.TextureLayer[0].Texture != 0 )
    {
        driver->setTransform( video::ETS_WORLD, core::matrix4() );

        driver->setMaterial( BillboardMaterial );

        ICameraSceneNode* camera = SceneManager->getActiveCamera();

        core::vector3df view = camera->getAbsolutePosition() - getAbsolutePosition();

        if ( view.getDistanceFromSQ( LastViewVec ) >= CAMERA_UPDATE_DISTANCE*CAMERA_UPDATE_DISTANCE )
        {
            updateBillboard();
            LastViewVec = view;
        }

        driver->drawMeshBuffer( &BillboardMeshBuffer );
    }
    else
    if ( mid*mid < DistSQ && MidLODMeshBuffer != 0 )
    {
        driver->setTransform( video::ETS_WORLD, AbsoluteTransformation );

        driver->setMaterial( TreeMaterial );

        driver->drawMeshBuffer( MidLODMeshBuffer );
    }
    else
    {
        driver->setTransform( video::ETS_WORLD, AbsoluteTransformation );

        driver->setMaterial( TreeMaterial );

        driver->drawMeshBuffer( HighLODMeshBuffer );
    }
}

video::SMaterial& CTreeSceneNode::getMaterial(u32 i)
{
    return TreeMaterial;
}

u32 CTreeSceneNode::getMaterialCount() const
{
    return 1;
}

const core::aabbox3d<f32>& CTreeSceneNode::getBoundingBox() const
{
    return HighLODMeshBuffer->BoundingBox;
}

s32 CTreeSceneNode::getVertexCount() const
{
    return HighLODMeshBuffer->getVertexCount();
}

CBillboardGroupSceneNode* CTreeSceneNode::getLeafNode()
{
    return LeafNode;
}

void CTreeSceneNode::updateBillboard()
{
    ICameraSceneNode* camera = SceneManager->getActiveCamera();

    if (!camera)
        return;

    core::vector3df campos = camera->getAbsolutePosition();

    core::vector3df pos = getAbsolutePosition();

    core::vector3df view = pos - campos;

    view.normalize();

    core::vector3df up = core::vector3df(0,1,0);

    AbsoluteTransformation.rotateVect( up );
    up.normalize();

    core::vector3df left = view.crossProduct(up);
    left.normalize();

    core::vector3df extent = HighLODMeshBuffer->BoundingBox.getExtent();

    core::vector3df yscale = core::vector3df( 0, 1, 0 );
    AbsoluteTransformation.rotateVect( yscale );    // Find the y scale and apply to the height

    up *= extent.Y * yscale.getLength();;

    core::vector3df xz = core::vector3df( 1, 0, 1 );    // Find the xz scale and apply to the width

    AbsoluteTransformation.rotateVect(xz);
    extent.Y = 0.0f;
    f32 len = extent.getLength() * xz.getLength() / 1.4f;   // Divide by 1.4f to compensate for the extra length of xz.

    left *= len / 2.0f;

    /*
        1--2
        |  |
        0--3
    */

    BillboardMeshBuffer.Vertices[0].Pos = pos - left;
    BillboardMeshBuffer.Vertices[1].Pos = pos - left + up;
    BillboardMeshBuffer.Vertices[2].Pos = pos + left + up;
    BillboardMeshBuffer.Vertices[3].Pos = pos + left;
}


} // namespace scene
} // namespace irr
