/*
Copyright (C) 2008  Mathias Gottschlag

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "graphic/CubeSceneNode.h"
#include "graphic/GraphicsEngine.h"

#include <irrlicht.h>
#include <SMeshBuffer.h>
using namespace irr;

namespace peak
{
	CubeSceneNode::CubeSceneNode(float size, SceneNode *parent)
	{
		// Create mesh
		scene::SMeshBuffer *buffer = new scene::SMeshBuffer();
		video::SColor colour(255,255,255,255);
		// Vertices
		buffer->Vertices.set_used(24);
		buffer->Vertices[0] = video::S3DVertex(-0.5f,-0.5f,-0.5f, 0,0,-1, colour, 0, 1);
		buffer->Vertices[1] = video::S3DVertex(0.5f,-0.5f,-0.5f, 0,0,-1, colour, 1, 1);
		buffer->Vertices[2] = video::S3DVertex(-0.5f,0.5f,-0.5f, 0,0,-1, colour, 0, 0);
		buffer->Vertices[3] = video::S3DVertex(0.5f,0.5f,-0.5f, 0,0,-1, colour, 1, 0);
		buffer->Vertices[4] = video::S3DVertex(0.5f,-0.5f,-0.5f, 1,0,0, colour, 0, 1);
		buffer->Vertices[5] = video::S3DVertex(0.5f,-0.5f,0.5f, 1,0,0, colour, 1, 1);
		buffer->Vertices[6] = video::S3DVertex(0.5f,0.5f,-0.5f, 1,0,0, colour, 0, 0);
		buffer->Vertices[7] = video::S3DVertex(0.5f,0.5f,0.5f, 1,0,0, colour, 1, 0);

		buffer->Vertices[8] = video::S3DVertex(-0.5f,-0.5f,0.5f, -1,0,0, colour, 0, 1);
		buffer->Vertices[9] = video::S3DVertex(-0.5f,-0.5f,-0.5f, -1,0,0, colour, 1, 1);
		buffer->Vertices[10] = video::S3DVertex(-0.5f,0.5f,0.5f, -1,0,0, colour, 0, 0);
		buffer->Vertices[11] = video::S3DVertex(-0.5f,0.5f,-0.5f, -1,0,0, colour, 1, 0);
		buffer->Vertices[12] = video::S3DVertex(0.5f,-0.5f,0.5f, 0,0,1, colour, 0, 1);
		buffer->Vertices[13] = video::S3DVertex(-0.5f,-0.5f,0.5f, 0,0,1, colour, 1, 1);
		buffer->Vertices[14] = video::S3DVertex(0.5f,0.5f,0.5f, 0,0,1, colour, 0, 0);
		buffer->Vertices[15] = video::S3DVertex(-0.5f,0.5f,0.5f, 0,0,1, colour, 1, 0);

		buffer->Vertices[16] = video::S3DVertex(-0.5f,0.5f,-0.5f, 0,1,0, colour, 0, 1);
		buffer->Vertices[17] = video::S3DVertex(0.5f,0.5f,-0.5f, 0,1,0, colour, 1, 1);
		buffer->Vertices[18] = video::S3DVertex(-0.5f,0.5f,0.5f, 0,1,0, colour, 0, 0);
		buffer->Vertices[19] = video::S3DVertex(0.5f,0.5f,0.5f, 0,1,0, colour, 1, 0);
		buffer->Vertices[20] = video::S3DVertex(-0.5f,-0.5f,0.5f, 0,-1,0, colour, 0, 1);
		buffer->Vertices[21] = video::S3DVertex(0.5f,-0.5f,0.5f, 0,-1,0, colour, 1, 1);
		buffer->Vertices[22] = video::S3DVertex(-0.5f,-0.5f,-0.5f, 0,-1,0, colour, 0, 0);
		buffer->Vertices[23] = video::S3DVertex(0.5f,-0.5f,-0.5f, 0,-1,0, colour, 1, 0);
		// Indices
		u16 indices[36] = {0,2,3,
			0,3,1,
			4,6,7,
			4,7,5,
			8,10,11,
			8,11,9,
			12,14,15,
			12,15,13,
			16,18,19,
			16,19,17,
			20,22,23,
			20,23,21};

		buffer->Indices.set_used(36);

		for (int i = 0; i < 36; i++)
		{
			buffer->Indices[i] = indices[i];
		}


		buffer->BoundingBox.reset(0,0,0);

		for (int i = 0; i < 24; i++)
		{
			buffer->Vertices[i].Pos *= size;
			buffer->BoundingBox.addInternalPoint(buffer->Vertices[i].Pos);
		}

		scene::ISceneManager *smgr = GraphicsEngine::get()->getSceneManager();

		scene::SMesh *mesh = new scene::SMesh();
		mesh->addMeshBuffer(buffer);
		scene::IMesh* tangentmesh = smgr->getMeshManipulator()->createMeshWithTangents(mesh);

		node = smgr->addMeshSceneNode(tangentmesh);

		if (parent)
		{
			setParent(parent);
		}
	}
}

