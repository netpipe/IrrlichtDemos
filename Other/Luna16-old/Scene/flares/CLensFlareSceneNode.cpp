#include "irrlicht.h"
#include "CLensFlareSceneNode.h"
#include <string>
#include <iostream>

using namespace std;

namespace irr
{
namespace scene
{

// --- Creates a Lens Flare node ---
// \param parent, the node to extend (can be null)
// \param mgr the scene manager to call functions from
// \param id this scenes ID
// \param position starting position
CLensFlareSceneNode::CLensFlareSceneNode(ISceneNode* parent,
										 ISceneManager* mgr, s32 id,
										 const core::vector3df& position )
						: ILensFlareSceneNode(parent, mgr, id, position) {
    AutomaticCullingState = EAC_BOX;

    indices[0] = 0;
    indices[1] = 2;
    indices[2] = 1;
    indices[3] = 0;
    indices[4] = 3;
    indices[5] = 2;

    (this)->setID(id);
    (this)->setParent(parent);

    vertices[0].TCoords.set(0.0f, 1.0f);
    vertices[0].Color = 0xffffffff;
    vertices[1].TCoords.set(0.0f, 0.0f);
    vertices[1].Color = 0xffffffff;
    vertices[2].TCoords.set(1.0f, 0.0f);
    vertices[2].Color = 0xffffffff;
    vertices[3].TCoords.set(1.0f, 1.0f);
    vertices[3].Color = 0xffffffff;

    screensize = SceneManager->getVideoDriver()->getScreenSize();
    material.Lighting = false;
    material.MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;
    material.ZBuffer = false;
    material.ZWriteEnable = false;
    BBox.MinEdge = core::vector3df(-2, -2, -2 );
    BBox.MaxEdge = core::vector3df( 2,  2,  2 );
	setPosition(position);
	startPosition = position;
}

// --- Returns material list ---
// \param i unused (for inheritance)
// \returns Material with a list of textures
video::SMaterial& CLensFlareSceneNode::getMaterial(s32 i) {
    return material;
}

// --- is called upon creation ---
void CLensFlareSceneNode::OnRegisterSceneNode() {
    if(IsVisible) {
        core::position2d<s32> pm = SceneManager->
			getSceneCollisionManager()->
			getScreenCoordinatesFrom3DPosition(
			getAbsolutePosition(),
				SceneManager->getActiveCamera() );

        if(core::rect<s32>(core::position2d<s32>(0,0), screensize).isPointInside(pm)) {
            SceneManager->registerNodeForRendering(this, scene::ESNRP_TRANSPARENT);

            ISceneNode::OnRegisterSceneNode();
        }
    }
}

// --- Every time it gets drawn, this function gets called ---
void CLensFlareSceneNode::render() {

 //   setPosition(irr::core::vector3df(0,1000,0));

    video::IVideoDriver* driver = SceneManager->getVideoDriver();
    ICameraSceneNode* camera = SceneManager->getActiveCamera();

	// Bail out if function dosn't have enough info
	// Will fail here if no textures supplied
    if (!camera || !driver || !material.getTexture(0)) return;

    driver->setTransform(video::ETS_WORLD, core::matrix4());
    core::vector3df campos = camera->getAbsolutePosition();
    core::dimension2du sz = SceneManager->getVideoDriver()->getScreenSize();
    core::vector2df mid = core::vector2df((f32)sz.Width, (f32)sz.Height );
    mid /= 2;

    core::position2d< s32 > lp = SceneManager->getSceneCollisionManager()->
		getScreenCoordinatesFrom3DPosition(Posit, camera );

    core::vector2df lightpos = core::vector2df((f32)lp.X, (f32)lp.Y );

    core::vector2df ipos;

    int nframes = material.getTexture(0)->getOriginalSize().Width /
		material.getTexture(0)->getOriginalSize().Height;
    int imageheight = material.getTexture(0)->getSize().Height;
    int texw = 8;
    float texp = 1.0f / nframes;

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
    for (s32 i=0; i<4; ++i)
		vertices[i].Normal = view;
    core::vector3df hor;
    core::vector3df ver;
    driver->setMaterial(material);
    core::vector3df pos;

    for (int ax = 0; ax < nframes; ax++ ) {
        if (ax == 0 ) {
			pos = Posit; //used to be getabsolute position
            texw = imageheight;
        } else {
            ipos = mid.getInterpolated(lightpos, (2.0f / nframes ) * ax );

            pos = SceneManager->getSceneCollisionManager()->getRayFromScreenCoordinates(
				core::position2d<s32>(int(ipos.X ), int(ipos.Y ) ), camera ).end;
				core::vector3df dir = core::vector3df(campos - pos ).normalize();
            pos = campos + (dir * -10.0f );
            texw = 4;
        }

        vertices[0].TCoords.set(   ax * texp,  1.0f);
        vertices[1].TCoords.set(   ax * texp,  0.0f);
        vertices[2].TCoords.set((ax+1) * texp, 0.0f);
        vertices[3].TCoords.set((ax+1) * texp, 1.0f);

        hor = horizontal * (0.5f * texw );
        ver = vertical   * (0.5f * texw );

        vertices[0].Pos = pos + hor + ver;
        vertices[1].Pos = pos + hor - ver;
        vertices[2].Pos = pos - hor - ver;
        vertices[3].Pos = pos - hor + ver;

        driver->drawIndexedTriangleList(vertices, 4, indices, 2);
    }
}

// --- returns a 'bounding box' of this node ---
const core::aabbox3d<f32>& CLensFlareSceneNode::getBoundingBox() const {
	return BBox;
}

// --- resets the screen size
void CLensFlareSceneNode::setScreenSize(const core::dimension2d<s32> &newSize) {
	screensize = newSize;
}

// --- returns the 3d Position where the flare started ---
core::vector3df CLensFlareSceneNode::getStartPosition() {
	return startPosition;
}

void CLensFlareSceneNode::setposition(irr::core::vector3df pos) {
//    setPosition(position);
    Posit = pos;
//	return startPosition;
}

}// end irr namespace
}// end scene namespace
