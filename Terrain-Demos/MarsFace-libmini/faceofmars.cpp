// Example Mars for libmini-terrainengine with Irrlicht
// try to fly with the cursor keys and mouse!
// v.01 16.10.2004
// v.02 17.10.2004
// v.03 18.10.2004 	delete vertex behind line of sight (deleted in v.04 cause user-defined-culling isnt part of this tutorial)
// v.04 change from keyscrolling to FPS-camera, delete the userculling from v.03
// btw: if you want user-defined-culling, you can delete vertex/indices
// from buffer after the stub->draw-function
//
// The mini library is the core of the terrain renderer described in the
// paper "Real-Time Generation of Continuous Levels of Detail for Height Fields".
// Copyright (c) 1995-2004 by Stefan Roettger (Version 5.02 as of 18.August.2004).
//
// The terrain renderer is licensed under the terms of the LGPL (see
// http://www.gnu.org/copyleft/ for more information on the license).
// Any commercial use of the code or parts of it requires the explicit
// permission of the author!
//
// The libmini-author's contact address is:
//
// mailto:roettger@cs.fau.de
// http://www9.cs.fau.de/Persons/Roettger
//
// The original terrain rendering paper and the talk are available here:
//
// http://www9.cs.fau.de/Persons/Roettger/papers/TERRAIN.PDF
// http://www9.cs.fau.de/Persons/Roettger/papers/WSCG98.PPT
//
//
// example and Irrlicht-interface-functions by zenprogramming
// (with help from Stefan Roettger ;-) )
// mailto: zenprogramming at yahoo dot de
// http://zenprogramming.tripod.com

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;



#pragma comment(lib, "Irrlicht.lib")
#include "ministub.h"


int triangleindex=-1;
int vertex0=0;
SMeshBuffer* buffer = 0;
video::S3DVertex vertex;


int size=256;
float dim=5.0f;
float scale=0.8f;
float res=1000.0f;
float ex=0.0f,ey=10.0f,ez=-30.0f;
float dx=0.0f,dy=-0.25f,dz=-1.0f;
float ux=0.0f,uy=1.0f,uz=0.0f;
float fovy=100.0f;
float aspect=1.0f;
float nearp=1.0f;
float farp=1000.0f;



//libmini->Irrlichtinterfacefunctions
//this function starts every trianglefan
void mybeginfan()
{
	triangleindex=0;
	vertex0=buffer->getVertexCount();
}

//libmini->Irrlichtinterfacefunctions
//this function gives one vertex
void myfanvertex(float i,float y,float j)
{
	vertex.Pos.set(dim*i-size/2*dim,
		y*scale,
		size/2*dim-dim*j);
	vertex.TCoords.set(1.0f-(float)i/(size-1),
                      1.0f-(float)j/(size-1));

	// instead of using white vertex-colors with a texture,
	// you could use the vertex-colors for shadows from heightdelta
	// vertex.Color.set(255,from heightdelta,from heightdelta,from heightdelta);
	buffer->Vertices.push_back(vertex);
	if (triangleindex==2)
	{
		buffer->Indices.push_back(vertex0);
		buffer->Indices.push_back(buffer->getVertexCount()-2);
		buffer->Indices.push_back(buffer->getVertexCount()-1);
		return;
	}
	triangleindex++;
}


int main(int argc,char *argv[])
{
	scene::IAnimatedMesh* mesh = 0;
	video::IVideoDriver* driver=0;
	scene::ICameraSceneNode* camera=0;
	video::SMaterial material;
	vector3df cameraPos;
	vector3df cameraTarget;
	int cameraoffset=120;

	IrrlichtDevice *device =
	createDevice(video::EDT_OPENGL, core::dimension2d<s32>(640, 480));

	driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	material.setTexture(1,0);
	material.Lighting=false;
	driver->setMaterial(material);

    IImage* heightimage = driver->createImageFromFile("FaceOfMarsMap.bmp");
	size = heightimage->getDimension().Width;

	short int *hfield = new short int[size*size];

	for (int i=0; i<size; i++)
		for (int j=0; j<size; j++)
			hfield[i+j*size] = heightimage->getPixel(i,size-1-j).getRed();


	ministub *stub;
	stub=new ministub(hfield,
		&size,&dim,scale,1.0f,
		0,0,0,
        mybeginfan,myfanvertex,0,0,
        NULL);

	int lastFPS = -1;
	vertex.Normal.set(0,1,0);
	vertex.Color.set(255,255,255,255);

	scene::IAnimatedMeshSceneNode* terrainnode = 0;
	ITexture* terraintexture = driver->getTexture("faceofmars.jpg");

	camera = smgr->addCameraSceneNodeFPS(0, 100);
	camera->setPosition(vector3df(0.0f,0.0f,0.0f));
	camera->setTarget(vector3df(0.0f,0.0f,-1.0f));
	camera->setUpVector(vector3df(0.0f,1.0f,0.0f));

	while(device->run())
	{

		buffer = new SMeshBuffer();

		float aspect = camera->getAspectRatio();
		float fovy = 180.0f/PI*camera->getFOV();
		float nearp = camera->getNearValue();
		float farp = camera->getFarValue();

		vector3df pos = camera->getPosition();
		vector3df tgt = camera->getTarget();

		float glide = stub->getheight(pos.X,pos.Z)+50.0f*nearp;
		if (glide<0.0f) glide = 100.0f;

		camera->setPosition(vector3df(pos.X,glide,pos.Z));

		if (tgt.X==pos.X && tgt.Y==pos.Y && tgt.Z==pos.Z) tgt.Z-=1.0f;

		stub->draw(res,
			pos.X,glide,pos.Z,
            tgt.X-pos.X,tgt.Y-pos.Y,tgt.Z-pos.Z,
            0.0f,1.0f,0.0f,
            2.0f*fovy,aspect,
            nearp,farp);

		SMesh* meshtmp = new SMesh();
		SAnimatedMesh* animatedMesh = new SAnimatedMesh();
		buffer->recalculateBoundingBox();
		meshtmp->addMeshBuffer(buffer);
		meshtmp->recalculateBoundingBox();
		animatedMesh->addMesh(meshtmp);
		animatedMesh->recalculateBoundingBox();
		mesh = animatedMesh;
		buffer->drop();
		terrainnode = smgr->addAnimatedMeshSceneNode(mesh);
		terrainnode->setMaterialFlag(video::EMF_LIGHTING, false);
		terrainnode->setMaterialTexture(0, terraintexture);

		driver->beginScene(true, true, video::SColor(0,150,50,0));

		smgr->drawAll();
		driver->endScene();
		int fps = driver->getFPS();
		if (lastFPS != fps)
		{
			wchar_t tmp[1024];
			swprintf(tmp, 1024, L"Marsdemo: Terrainengine-libmini and Irrlicht Engine (fps:%d) Triangles:%d",
				fps, driver->getPrimitiveCountDrawn());
			device->setWindowCaption(tmp);
			lastFPS = fps;
		}
		terrainnode->remove();
		meshtmp->drop();
		animatedMesh->drop();
		device->sleep(10);
	}
	delete(stub);
	delete(hfield);
	heightimage->drop();
	device->drop();

	return 0;
}

