#include "irrlicht.h"
#include "CLensFlaresSceneNode.h"

using namespace scene;

CLensFlareSceneNode::CLensFlareSceneNode( ISceneNode* parent, ISceneManager* mgr, s32 id, const core::vector3df& position )
                         : ISceneNode( parent, mgr, id, position ) {
//	AutomaticCullingEnabled = true;
	indices[0] = 0;
	indices[1] = 2;
	indices[2] = 1;
	indices[3] = 0;
	indices[4] = 3;
	indices[5] = 2;

	vertices[0].TCoords.set(0.0f, 1.0f);
	vertices[0].Color = 0xffffffff;

	vertices[1].TCoords.set(0.0f, 0.0f);
	vertices[1].Color = 0xffffffff;

	vertices[2].TCoords.set(1.0f, 0.0f);
	vertices[2].Color = 0xffffffff;

	vertices[3].TCoords.set(1.0f, 1.0f);
	vertices[3].Color = 0xffffffff;

    screensize = SceneManager->getVideoDriver()->getScreenSize();
    setMaterialFlag( video::EMF_LIGHTING, false );
    setMaterialType( video::EMT_TRANSPARENT_ADD_COLOR );
    material.MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;
    material.ZBuffer = false;
    material.ZWriteEnable = false;
    BBox.MinEdge = core::vector3df( -2, -2, -2 );
    BBox.MaxEdge = core::vector3df(  2,  2,  2 );
}

video::SMaterial& CLensFlareSceneNode::getMaterial(s32 i) { return material; }

int CLensFlareSceneNode::getMaterialCount() { return 1; }

void CLensFlareSceneNode::OnPreRender() {
	if ( IsVisible ) {
        core::position2d<s32> pm = SceneManager->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition( getAbsolutePosition(), SceneManager->getActiveCamera() );
        if ( core::rect<s32>( core::position2d<s32>(0,0), screensize ).isPointInside( pm ) ) {
		    SceneManager->registerNodeForRendering( this, scene::ESNRP_TRANSPARENT );
//		    ISceneNode::OnPreRender();
        }
    }
}

void CLensFlareSceneNode::render() {

	video::IVideoDriver* driver = SceneManager->getVideoDriver();
    ICameraSceneNode* camera = SceneManager->getActiveCamera();

//    if (!camera || !driver || !material.Texture1 ) return;

	driver->setTransform(video::ETS_WORLD, core::matrix4() );

    core::vector3df campos = camera->getAbsolutePosition();

    core::dimension2d< u32 > sz = SceneManager->getVideoDriver()->getScreenSize();
    core::vector2df mid = core::vector2df( sz.Width, sz.Height );
    mid /= 2;

    core::position2d< s32 > lp = SceneManager->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition( getAbsolutePosition(), camera );
    core::vector2df lightpos = core::vector2df( lp.X, lp.Y );

    core::vector2df ipos;

    int nframes = material.getTexture(1)->getOriginalSize().Width / material.getTexture(1)->getOriginalSize().Height;
    int imageheight = material.getTexture(1)->getSize().Height;
    int texw = 8; // fix height of lens flares to 32 pixels
    float texp = 1.0f / nframes;

	// calc billboard rotation to face camera
    core::vector3df target = camera->getTarget();
	core::vector3df up = camera->getUpVector();
	core::vector3df view = target - campos;
	view.normalize();
	core::vector3df horizontal = up.crossProduct(view);
	horizontal.normalize();
	core::vector3df vertical;
    vertical = horizontal.crossProduct(view);
	vertical.normalize();
	view *= -1.0f;
    for ( s32 i=0; i<4; ++i) vertices[i].Normal = view;

    core::vector3df hor;
    core::vector3df ver;

    // set material
    driver->setMaterial( material );
    core::vector3df pos;
    for ( int ax = 0; ax < nframes; ax++ ) {

        if ( ax == 0 ) { // first burst is 3d
            pos = getAbsolutePosition();
            texw = imageheight;
        } else {
            // calc screen position for each flare
            ipos = mid.getInterpolated( lightpos, ( 2.0f / nframes ) * ax );

            // try to find the correspondent world coordinate for screen position
            pos = SceneManager->getSceneCollisionManager()->getRayFromScreenCoordinates(
	                              core::position2d<s32>( int( ipos.X ), int( ipos.Y ) ), camera ).end;

            // try to put this position vetor near in front of camera
            core::vector3df dir = core::vector3df( campos - pos ).normalize();
            pos = campos + ( dir * -10.0f );// put 10 units in front
            texw = 4;
        }

        // render flares

        // move texcoords to next flare in texture
	    vertices[0].TCoords.set(    ax * texp,  1.0f);
	    vertices[1].TCoords.set(    ax * texp,  0.0f);
	    vertices[2].TCoords.set( (ax+1) * texp, 0.0f);
	    vertices[3].TCoords.set( (ax+1) * texp, 1.0f);

        hor = horizontal * ( 0.5f * texw );
        ver = vertical   * ( 0.5f * texw );

	    vertices[0].Pos = pos + hor + ver;
	    vertices[1].Pos = pos + hor - ver;
	    vertices[2].Pos = pos - hor - ver;
	    vertices[3].Pos = pos - hor + ver;

        // draw image part
	    driver->drawIndexedTriangleList(vertices, 4, indices, 2);
    }
}

const core::aabbox3d<f32>& CLensFlareSceneNode::getBoundingBox() const { return BBox; }
