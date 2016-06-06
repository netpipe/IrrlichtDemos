//!
//! CQUADTREESCENENODE
//! Irrlicht scenenode for quadtree frustrum culling
//!
//!
//! By Paulo Rogério de Oliveira ( paulo_cmv@hotmail.com < MSN - EMAIL > )
//! ----------------------------
//! You can use it for any purpose, as you want, only dont remove this notice.
//! If you make any changes, improvements, or you have comments, sugestions,
//! you can report it back by mail or msn. This way i can try to improve the code.

#include "stdlib.h"
#include "irrlicht.h"
#include "CQuadTreeSceneNode.h"

CQuadTreeSceneNode::CQuadTreeSceneNode( ISceneNode* parent, ISceneManager* mgr, s32 id, int idepth, int cLevel ) :
    ISceneNode( parent, mgr, id )
{
	#ifdef _DEBUG
	setDebugName("CQuadTreeSceneNode");
	#endif
	Depth = idepth;
    Level = cLevel;
    smgr = mgr;
    if ( Level < Depth - 1 ) {
        printf( "Creating level %i\n", Level+1 );
        childs[0] = new CQuadTreeSceneNode( this, mgr, -1, idepth, Level+1 );
        childs[1] = new CQuadTreeSceneNode( this, mgr, -1, idepth, Level+1 );
        childs[2] = new CQuadTreeSceneNode( this, mgr, -1, idepth, Level+1 );
        childs[3] = new CQuadTreeSceneNode( this, mgr, -1, idepth, Level+1 );
    }
    started = false;
    material.Lighting = false;
    material.DiffuseColor = video::SColor( 255, 255, 255, 255 );
//    AutomaticCullingEnabled = false;
}

void CQuadTreeSceneNode::setDebugDataVisible( bool visible ) {
    DebugDataVisible = visible;
    if ( Level > ( Depth - 2 ) ) return;
    CQuadTreeSceneNode::CQuadTreeSceneNode* nod = this;
    nod->childs[0]->setDebugDataVisible( visible );
    nod->childs[1]->setDebugDataVisible( visible );
    nod->childs[2]->setDebugDataVisible( visible );
    nod->childs[3]->setDebugDataVisible( visible );
}

void CQuadTreeSceneNode::startAdding() {
    started = true;
    allAdded = 0;
    arr.empty();
}

void CQuadTreeSceneNode::addNode( ISceneNode* node ) {
    if ( !started ) return;
    arr.push_back( node );
}

void CQuadTreeSceneNode::OnPreRender() {
    ICameraSceneNode* cam = smgr->getActiveCamera();

//	bool vis = ( cam->getViewFrustrum()->getBoundingBox().intersectsWithBox( bbox ) );// getTransformedBoundingBox() ) );

   // setVisible( vis );
    SceneManager->registerNodeForRendering( this );
//    ISceneNode::OnPreRender();
}

void CQuadTreeSceneNode::render() {
    if ( allAdded == 9 ) prepareNodes();
    if ( allAdded < 10 ) allAdded++;

    if (DebugDataVisible)
    {
        video::IVideoDriver* driver = SceneManager->getVideoDriver();
        driver->setTransform( video::ETS_WORLD, core::matrix4() );// AbsoluteTransformation);
        driver->setMaterial( material );
        driver->draw3DBox( bbox );
    }
}

const core::aabbox3d<f32>& CQuadTreeSceneNode::getBoundingBox() const
{
	return bbox;
}

void CQuadTreeSceneNode::endAdding()
{
    allAdded = 1;
}

void CQuadTreeSceneNode::prepareNodes()
{
    started = false;
    if ( Level > ( Depth - 1 ) ) return;
    if ( arr.size() == 0 ) return;

    bbox = arr[0]->getTransformedBoundingBox();
    for (int x = 0; x < arr.size(); x++)
    {
        arr[x]->updateAbsolutePosition ();
        core::aabbox3df bb = arr[x]->getTransformedBoundingBox();
         bbox.addInternalBox( bb );

    }
    core::vector3df mid = core::vector3df( bbox.MinEdge + ( ( bbox.MaxEdge - bbox.MinEdge ) / 2 ) );
    if ( Level > ( Depth - 2 ) ) return;
    childs[0]->startAdding();
    childs[1]->startAdding();
    childs[2]->startAdding();
    childs[3]->startAdding();
    ISceneNode* nod;
    core::vector3df p;
    printf( "Add to " );
    for (int x = 0; x < arr.size(); x++)
    {
        nod = arr[x];
        p = nod->getPosition();
        if ( p.X > mid.X )
        {
            if ( p.Z > mid.Z )
            {
                printf( "0, " );
                nod->setParent( childs[0] );
                childs[0]->addNode( nod );
            }
            else
            {
                printf( "1, " );
                nod->setParent( childs[1] );
                childs[1]->addNode( nod );
            };
        }
        else
        {
            if ( p.Z > mid.Z )
            {
                printf( "2, " );
                nod->setParent( childs[2] );
                childs[2]->addNode( nod );
            }
            else
            {
                printf( "3, " );
                nod->setParent( childs[3] );
                childs[3]->addNode( nod );
            }
        }
    }
    printf( " Level: %i \n", Level );
    childs[0]->endAdding();
    childs[1]->endAdding();
    childs[2]->endAdding();
    childs[3]->endAdding();
}
