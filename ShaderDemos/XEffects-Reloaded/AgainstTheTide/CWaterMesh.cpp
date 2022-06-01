#include "CWaterMesh.h"

using namespace irr;
using namespace video;
using namespace scene;
using namespace core;

CWaterMesh::CWaterMesh(	irr::core::dimension2d<f32> tileSize,
						irr::core::dimension2d<u32> tileCount) 
	: mesh(new SMesh()), tileSze(tileSize), tileCnt(tileCount)
{
	meshBuffer = new SMeshBuffer();
	meshBuffer->setHardwareMappingHint(EHM_STATIC);
	
	for(u32 x = 0;x < tileCount.Width;++x)
	{
		for(u32 y = 0;y < tileCount.Height;++y)
		{
			vector3df vertPos = vector3df(x * tileSize.Width, 0, y * tileSize.Height);

			meshBuffer->Vertices.push_back(S3DVertex(vertPos, vector3df(0, 1, 0),
				SColor(255, 255, 255, 255),	
				vector2df((f32)x / tileCount.Width, (f32)y / tileCount.Height)));
		}
	}

	for(u32 x = 0;x < tileCount.Width - 1;++x)
	{
		for(u32 y = 0;y < tileCount.Height - 1;++y)
		{
			u32 currIndex = x * tileCount.Width + y;
			meshBuffer->Indices.push_back(currIndex);
			meshBuffer->Indices.push_back(currIndex + 1);
			meshBuffer->Indices.push_back(currIndex + tileCount.Width + 1);
	
			meshBuffer->Indices.push_back(currIndex);
			meshBuffer->Indices.push_back(currIndex + tileCount.Width + 1);
			meshBuffer->Indices.push_back(currIndex + tileCount.Width);
		}
	}

	mesh->addMeshBuffer(meshBuffer);
	meshBuffer->recalculateBoundingBox();
	mesh->recalculateBoundingBox();
	meshBuffer->drop();
}


CWaterMesh::~CWaterMesh()
{
	mesh->drop();
}


irr::video::S3DVertex& CWaterMesh::getVertex(irr::u32 x, irr::u32 y)
{
	return ((S3DVertex*)meshBuffer->getVertices())[x * tileCnt.Width + y];
}


void CWaterMesh::setDirty()
{
	meshBuffer->setDirty(EBT_VERTEX);
}


void CWaterMesh::recalculateNormals()
{
	for(u32 x = 1;x < tileCnt.Width - 1;++x)
	{
		for(u32 y = 1;y < tileCnt.Height - 1;++y)
		{
			const vector3df currPos = getVertex(x, y).Pos;
			const vector3df v0 = getVertex(x-1, y).Pos - currPos;
			const vector3df v1 = getVertex(x-1, y-1).Pos - currPos;
			const vector3df n0 = v1.crossProduct(v0);

			const vector3df v2 = getVertex(x, y-1).Pos - currPos;
			const vector3df n1 = v2.crossProduct(v1);

			const vector3df v3 = getVertex(x+1, y).Pos - currPos;
			const vector3df n2 = v3.crossProduct(v2);

			const vector3df v4 = getVertex(x+1, y+1).Pos - currPos;
			const vector3df n3 = v4.crossProduct(v3);

			const vector3df v5 = getVertex(x, y+1).Pos - currPos;
			const vector3df n4 = v5.crossProduct(v4);

			const vector3df v6 = getVertex(x-1, y).Pos - currPos;
			const vector3df n5 = v6.crossProduct(v5);

			const vector3df m0 = (n1 + n0) * 0.5f;
			const vector3df m2 = (n4 + n3) * 0.5f;

			const vector3df k0 = (m2 + m0) * 0.5f;
			const vector3df k1 = (n5 + n2) * 0.5f;

			vector3df n = (k1 + k0) * 0.5f;
			getVertex(x, y).Normal = n.normalize();
		}
	}
}

void CWaterMesh::recalculateBoundingBox()
{
	meshBuffer->recalculateBoundingBox();
	mesh->recalculateBoundingBox();
}