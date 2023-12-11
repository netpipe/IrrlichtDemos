/*
    Written by Asger Feldthaus

    February 2007
*/

#include <irrlicht.h>
#include "CBillboardGroupSceneNode.h"

#include <ICameraSceneNode.h>
#include <IVideoDriver.h>

namespace irr
{
namespace scene
////////////// MESH DECLARATION /////////////////////////////
{

CBillboardGroupSceneNode::CBillboardGroupSceneNode( ISceneNode* parent, ISceneManager* manager, s32 id,
        const core::vector3df& position,
        const core::vector3df& rotation,
        const core::vector3df& scale )
    : ISceneNode( parent, manager, id, position, rotation, scale )
{
    FarDistance = 256.0f;
    Radius = 0.0f;
    Material.Lighting = false;
}

void CBillboardGroupSceneNode::addBillboard( const core::vector3df& position, const core::dimension2df& size, f32 roll, video::SColor vertexColor )
{
    Billboards.push_back( SBillboard( position, size, roll, vertexColor ) );

    s32 vertexIndex = MeshBuffer.Vertices.size();

    f32 maxDimension = size.Width;
    if ( size.Height > maxDimension )
        maxDimension = size.Height;

    core::vector3df vDim = core::vector3df(maxDimension,maxDimension,maxDimension);

    BoundingBox.addInternalBox( core::aabbox3df(
            position - vDim,
            position + vDim
        ));

    Radius = BoundingBox.getExtent().getLength() / 2.0f;

    // Don't bother setting correct positions here, they are updated in OnPreRender anyway
    MeshBuffer.Vertices.push_back( video::S3DVertex( position, core::vector3df(0,1,0), vertexColor, core::vector2df(0,0) ) );
    MeshBuffer.Vertices.push_back( video::S3DVertex( position, core::vector3df(0,1,0), vertexColor, core::vector2df(1,0) ) );
    MeshBuffer.Vertices.push_back( video::S3DVertex( position, core::vector3df(0,1,0), vertexColor, core::vector2df(1,1) ) );
    MeshBuffer.Vertices.push_back( video::S3DVertex( position, core::vector3df(0,1,0), vertexColor, core::vector2df(0,1) ) );

    /*
        Vertices are placed like this:
            0---1
            |   |
            3---2
    */

    MeshBuffer.Indices.push_back( vertexIndex );
    MeshBuffer.Indices.push_back( vertexIndex + 1 );
    MeshBuffer.Indices.push_back( vertexIndex + 2 );

    MeshBuffer.Indices.push_back( vertexIndex + 2 );
    MeshBuffer.Indices.push_back( vertexIndex + 3 );
    MeshBuffer.Indices.push_back( vertexIndex );
}

void CBillboardGroupSceneNode::addBillboardWithAxis( const core::vector3df& position, const core::dimension2df& size, const core::vector3df& axis, f32 roll, video::SColor vertexColor )
{
    s32 index = Billboards.size();

    addBillboard( position, size, roll, vertexColor );

    Billboards[index].HasAxis = true;
    Billboards[index].Axis = axis;
}

s32 CBillboardGroupSceneNode::getBillboardCount() const
{
    return Billboards.size();
}

void CBillboardGroupSceneNode::OnRegisterSceneNode()
{
    if ( IsVisible )
    {
        SceneManager->registerNodeForRendering( this );
    }
}

void CBillboardGroupSceneNode::render()
{
    updateBillboards();

    video::IVideoDriver* driver = SceneManager->getVideoDriver();

    driver->setTransform( video::ETS_WORLD, core::matrix4() );

    driver->setMaterial( Material );

    driver->drawMeshBuffer( &MeshBuffer );
}

const core::aabbox3df& CBillboardGroupSceneNode::getBoundingBox() const
{
    return BoundingBox;
}

video::SMaterial& CBillboardGroupSceneNode::getMaterial(u32 i)
{
    return Material;
}

u32 CBillboardGroupSceneNode::getMaterialCount() const
{
    return 1;
}

void CBillboardGroupSceneNode::applyVertexShadows( const core::vector3df& lightDir, f32 intensity, f32 ambient )
{
    for ( s32 i=0; i<Billboards.size(); i++ )
    {
        core::vector3df normal = Billboards[i].Position;

        normal.normalize();

        f32 light = -lightDir.dotProduct(normal)*intensity + ambient;

        if ( light < 0 )
            light = 0;

        if ( light > 1 )
            light = 1;

        video::SColor color;

        color.setRed( (u8)(Billboards[i].Color.getRed() * light) );
        color.setGreen( (u8)(Billboards[i].Color.getGreen() * light) );
        color.setBlue( (u8)(Billboards[i].Color.getBlue() * light) );
        color.setAlpha( Billboards[i].Color.getAlpha() );

        for ( s32 j=0; j<4; j++ )
        {
            MeshBuffer.Vertices[i*4+j].Color = color;
        }
    }
}

//! Gives vertices back their original color.
void CBillboardGroupSceneNode::resetVertexShadows()
{
    for ( s32 i=0; i<Billboards.size(); i++ )
    {
        for ( s32 j=0; j<4; j++ )
        {
            MeshBuffer.Vertices[i*4+j].Color = Billboards[i].Color;
        }
    }
}

void CBillboardGroupSceneNode::updateBillboards()
{
    ICameraSceneNode* camera = SceneManager->getActiveCamera();

    if ( !camera )
        return;

    core::vector3df camPos = camera->getAbsolutePosition();

    core::vector3df ref = core::vector3df(0,1,0);

    camera->getAbsoluteTransformation().rotateVect(ref);

    core::vector3df view, right, up;

    bool farAway = false;

    core::vector3df center = BoundingBox.getCenter();
    AbsoluteTransformation.transformVect(center);

    core::vector3df camDir = camPos - center;

    if ( camDir.getLengthSQ() >= (FarDistance + Radius)*(FarDistance + Radius) )
    {
        farAway = true;
        view = center - camPos;
        view.normalize();

        right = ref.crossProduct( view );

        up = view.crossProduct( right );
    }

    core::vector3df rotatedCamDir = camDir;
    AbsoluteTransformation.inverseRotateVect( rotatedCamDir );

    if ( farAway && (rotatedCamDir - LastCamDir).getLengthSQ() < 1000.0f )
    {
        return;
    }

    LastCamDir = rotatedCamDir;

    // Update the position of every billboard
    for ( s32 i=0; i<Billboards.size(); i++ )
    {
        if ( !farAway )
        {
            core::vector3df pos = Billboards[i].Position;

            AbsoluteTransformation.transformVect( pos );

            view = pos - camPos;

            view.normalize();
        }

        core::vector3df thisRight = right;
        core::vector3df thisUp = up;

        if ( Billboards[i].HasAxis )
        {
            core::vector3df axis = Billboards[i].Axis;

            AbsoluteTransformation.rotateVect(axis);

            thisRight = axis.crossProduct( view );

            thisUp = axis;
        }
        else if ( !farAway )
        {
            thisRight = ref.crossProduct( view );

            thisUp = view.crossProduct( thisRight );
        }

        f32 rollrad = Billboards[i].Roll * core::DEGTORAD;
        f32 cos_roll = cos( rollrad );
        f32 sin_roll = sin( rollrad );

        core::vector3df a =  cos_roll * thisRight + sin_roll * thisUp;
        core::vector3df b = -sin_roll * thisRight + cos_roll * thisUp;

        a *= Billboards[i].Size.Width / 2.0f;
        b *= Billboards[i].Size.Height / 2.0f;

        s32 vertexIndex = 4 * i;  // 4 vertices per billboard

        core::vector3df billPos = Billboards[i].Position;

        AbsoluteTransformation.transformVect(billPos);

        MeshBuffer.Vertices[vertexIndex  ].Pos = billPos - a + b;
        MeshBuffer.Vertices[vertexIndex+1].Pos = billPos + a + b;
        MeshBuffer.Vertices[vertexIndex+2].Pos = billPos + a - b;
        MeshBuffer.Vertices[vertexIndex+3].Pos = billPos - a - b;
    }
}

} // namespace scene
} // namespace irr
