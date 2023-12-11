#include "StdAfx.h"
#include "IrrBox.h"

MachineBox::MachineBox(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, float x, float y, float z)
  :IrrBox (parent, mgr, id, x, y, z)
 {
    Material.Wireframe = false;
    Material.Lighting = false;

	x *= 0.5f;
	z *= 0.5f;
	y *= 0.5f;

	core::vector3df loc = core::vector3df(0.0f, 0.0f, 0.0f);

    // top
    Vertices[0] = video::S3DVertex(loc.X - x, - y, - z,      0,1,0,  video::SColor(255,255,255,255),0,0);
    Vertices[1] = video::S3DVertex(loc.X + x, - y, - z,      0,1,0,  video::SColor(255,255,255,255),1,0);
    Vertices[2] = video::S3DVertex(loc.X - x, - y, + z,      0,1,0,  video::SColor(255,255,255,255),0,1);
    Vertices[3] = video::S3DVertex(loc.X + x, - y, + z,      0,1,0,  video::SColor(255,255,255,255),1,1);

    // bottom
    Vertices[4] = video::S3DVertex(loc.X - x, + y, - z,      0,1,0,  video::SColor(255,255,255,255),0,0);
    Vertices[5] = video::S3DVertex(loc.X + x, + y, - z,      0,1,0,  video::SColor(255,255,255,255),1,0);
    Vertices[6] = video::S3DVertex(loc.X - x, + y, + z,      0,1,0,  video::SColor(255,255,255,255),0,1);
    Vertices[7] = video::S3DVertex(loc.X + x, + y, + z,      0,1,0,  video::SColor(255,255,255,255),1,1);

    // < l
    Vertices[8]  = video::S3DVertex(loc.X + x, - y, - z,      0,1,0,  video::SColor(255,255,255,255),0,0);
    Vertices[9]  = video::S3DVertex(loc.X + x, + y, - z,      0,1,0,  video::SColor(255,255,255,255),1,0);
    Vertices[10] = video::S3DVertex(loc.X + x, - y, + z,      0,1,0,  video::SColor(255,255,255,255),0,1);
    Vertices[11] = video::S3DVertex(loc.X + x, + y, + z,      0,1,0,  video::SColor(255,255,255,255),1,1);

    // > r
    Vertices[12] = video::S3DVertex(loc.X - x, - y, - z,      0,1,0,  video::SColor(255,255,255,255),0,0);
    Vertices[13] = video::S3DVertex(loc.X - x, + y, - z,      0,1,0,  video::SColor(255,255,255,255),1,0);
    Vertices[14] = video::S3DVertex(loc.X - x, - y, + z,      0,1,0,  video::SColor(255,255,255,255),0,1);
    Vertices[15] = video::S3DVertex(loc.X - x, + y, + z,      0,1,0,  video::SColor(255,255,255,255),1,1);

    // ^ u
    Vertices[16] = video::S3DVertex(loc.X - x, - y, + z,      0,1,0,  video::SColor(255,255,255,255),0,0);
    Vertices[17] = video::S3DVertex(loc.X - x, + y, + z,      0,1,0,  video::SColor(255,255,255,255),1,0);
    Vertices[18] = video::S3DVertex(loc.X + x, - y, + z,      0,1,0,  video::SColor(255,255,255,255),0,1);
    Vertices[19] = video::S3DVertex(loc.X + x, + y, + z,      0,1,0,  video::SColor(255,255,255,255),1,1);

    // v d
    Vertices[20] = video::S3DVertex(loc.X - x, - y, - z,      0,1,0,  video::SColor(255,255,255,255),0,0);
    Vertices[21] = video::S3DVertex(loc.X - x, + y, - z,      0,1,0,  video::SColor(255,255,255,255),1,0);
    Vertices[22] = video::S3DVertex(loc.X + x, - y, - z,      0,1,0,  video::SColor(255,255,255,255),0,1);
    Vertices[23] = video::S3DVertex(loc.X + x, + y, - z,      0,1,0,  video::SColor(255,255,255,255),1,1);

    Box.reset(Vertices[0].Pos);
	for (s32 i=1; i<24; ++i)  {
        Box.addInternalPoint(Vertices[i].Pos);
	}
 }

IrrBox::IrrBox (scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, float x, float y, float z)
	:scene::ISceneNode(parent, mgr, id)
{

    Material.Wireframe = false;
    Material.Lighting = false;

	x *= 0.5f;
	z *= 0.5f;
	y *= 0.5f;

	core::vector3df loc = core::vector3df(x, 0.0f, 0.0f);

    // top
    Vertices[0] = video::S3DVertex(loc.X - x, - y, - z,      0,1,0,  video::SColor(255,255,255,255),0,0);
    Vertices[1] = video::S3DVertex(loc.X + x, - y, - z,      0,1,0,  video::SColor(255,255,255,255),1,0);
    Vertices[2] = video::S3DVertex(loc.X - x, - y, + z,      0,1,0,  video::SColor(255,255,255,255),0,1);
    Vertices[3] = video::S3DVertex(loc.X + x, - y, + z,      0,1,0,  video::SColor(255,255,255,255),1,1);

    // bottom
    Vertices[4] = video::S3DVertex(loc.X - x, + y, - z,      0,1,0,  video::SColor(255,255,255,255),0,0);
    Vertices[5] = video::S3DVertex(loc.X + x, + y, - z,      0,1,0,  video::SColor(255,255,255,255),1,0);
    Vertices[6] = video::S3DVertex(loc.X - x, + y, + z,      0,1,0,  video::SColor(255,255,255,255),0,1);
    Vertices[7] = video::S3DVertex(loc.X + x, + y, + z,      0,1,0,  video::SColor(255,255,255,255),1,1);

    // < l
    Vertices[8]  = video::S3DVertex(loc.X + x, - y, - z,      0,1,0,  video::SColor(255,255,255,255),0,0);
    Vertices[9]  = video::S3DVertex(loc.X + x, + y, - z,      0,1,0,  video::SColor(255,255,255,255),1,0);
    Vertices[10] = video::S3DVertex(loc.X + x, - y, + z,      0,1,0,  video::SColor(255,255,255,255),0,1);
    Vertices[11] = video::S3DVertex(loc.X + x, + y, + z,      0,1,0,  video::SColor(255,255,255,255),1,1);

    // > r
    Vertices[12] = video::S3DVertex(loc.X - x, - y, - z,      0,1,0,  video::SColor(255,255,255,255),0,0);
    Vertices[13] = video::S3DVertex(loc.X - x, + y, - z,      0,1,0,  video::SColor(255,255,255,255),1,0);
    Vertices[14] = video::S3DVertex(loc.X - x, - y, + z,      0,1,0,  video::SColor(255,255,255,255),0,1);
    Vertices[15] = video::S3DVertex(loc.X - x, + y, + z,      0,1,0,  video::SColor(255,255,255,255),1,1);

    // ^ u
    Vertices[16] = video::S3DVertex(loc.X - x, - y, + z,      0,1,0,  video::SColor(255,255,255,255),0,0);
    Vertices[17] = video::S3DVertex(loc.X - x, + y, + z,      0,1,0,  video::SColor(255,255,255,255),1,0);
    Vertices[18] = video::S3DVertex(loc.X + x, - y, + z,      0,1,0,  video::SColor(255,255,255,255),0,1);
    Vertices[19] = video::S3DVertex(loc.X + x, + y, + z,      0,1,0,  video::SColor(255,255,255,255),1,1);

    // v d
    Vertices[20] = video::S3DVertex(loc.X - x, - y, - z,      0,1,0,  video::SColor(255,255,255,255),0,0);
    Vertices[21] = video::S3DVertex(loc.X - x, + y, - z,      0,1,0,  video::SColor(255,255,255,255),1,0);
    Vertices[22] = video::S3DVertex(loc.X + x, - y, - z,      0,1,0,  video::SColor(255,255,255,255),0,1);
    Vertices[23] = video::S3DVertex(loc.X + x, + y, - z,      0,1,0,  video::SColor(255,255,255,255),1,1);

    Box.reset(Vertices[0].Pos);
	for (s32 i=1; i<24; ++i)  {
        Box.addInternalPoint(Vertices[i].Pos);
	}
}

void IrrBox::OnPreRender()
{
	if (IsVisible) {
        SceneManager->registerNodeForRendering(this);
	}

//    ISceneNode::OnPreRender();
}

void IrrBox::render()
{
    u16 indices[] = { 0,1,2, 3,2,1, 4,6,5, 7,5,6, 8,9,10, 11,10,9, 12,14,13, 15,13,14, 16,18,17, 19,17,18, 20,21,22, 23,22,21 };

    video::IVideoDriver* driver = SceneManager->getVideoDriver();
    driver->setMaterial(Material);
    driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
    driver->drawIndexedTriangleList(&Vertices[0], 24, &indices[0], 12); // 12 ind
}

