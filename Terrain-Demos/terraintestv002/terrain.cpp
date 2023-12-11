// terrainrender v0.0.2, 26.09.2003, irrlicht licence
// a hack from Nikos CGeometryCreator
// if you want to help, please contact
// kortyburns or knightoflight, irrlicht-forummembers



#include "terrain.h"
#include "SAnimatedMesh.h"
#include "SMeshBuffer.h"
#include "SMesh.h"

using namespace std;

int terrain::getHeight(int X, int Z)
{
	if(!heightmap) return -1;
	return heightmap[X][Z];
}

void terrain::loadRawMap(char *name )
{
	ifstream File;
	unsigned char height;
	File.open(name,ios::in | ios::binary);
	for (int i=0;i<mapsizex;i++)
		for (int j=0;j<mapsizez;j++)
		{
			height=File.get();
			height=File.get();
			height=File.get();
			heightmap[i][j]=height;
		};
	File.close();
}

// This renderterrain-version is from kortyburns, France, thank you very much
// Its good to tune the terrain for better performance/fps
// Cause you can scale the number of indices
IAnimatedMesh* terrain::renderterrainscaled(const c8* name,
	const core::dimension2d<f32>& tileSize,
	video::SMaterial* material,
	const core::dimension2d<f32>& textureRepeatCount, int heightstretch, int scale)
{
	int h;
	s32 x = 0;
	s32 z = 0;

	SMeshBuffer* buffer = new SMeshBuffer();
	SMesh* mesh = new SMesh();
	video::S3DVertex vtx;
	vtx.Color.set(255,255,255,255);
	vtx.Normal.set(0,1,0);


	float halfX = (tileSize.Width * mapsizex) / 2;
	float halfY = (tileSize.Height * mapsizez) / 2;

	core::dimension2d<f32> tx;
	tx.Width = 1.0f / (mapsizex / textureRepeatCount.Width);
	tx.Height = 1.0f / (mapsizez / textureRepeatCount.Height);

	for (x=0; x<mapsizex; ++x)
		for (z=0; z<mapsizez; ++z)
		{
			vtx.Pos.set(tileSize.Width * x - halfX, 0, tileSize.Height * z - halfY);
			vtx.TCoords.set(-(f32)x * tx.Width, (f32)z * tx.Height);

			h=heightmap[x][z];
			vtx.Pos.Y = h*heightstretch;

			buffer->Vertices.push_back(vtx);
		}

	for (x=0; x<mapsizex-scale; x+=scale)
		for (z=0; z<mapsizez-scale; z+=scale)
		{
			s32 current = z*mapsizex + x;

			buffer->Indices.push_back(current);
			buffer->Indices.push_back(current + scale);
			buffer->Indices.push_back(current + mapsizex*scale);

			buffer->Indices.push_back(current + scale);
			buffer->Indices.push_back(current + scale + mapsizex*scale);
			buffer->Indices.push_back(current + mapsizex*scale);
		}

	for (s32 i=0; i<(s32)buffer->Indices.size(); i+=3)
	{
		core::plane3d<f32> p(
			buffer->Vertices[buffer->Indices[i+0]].Pos,
			buffer->Vertices[buffer->Indices[i+1]].Pos,
			buffer->Vertices[buffer->Indices[i+2]].Pos);
		p.Normal.normalize();

		buffer->Vertices[buffer->Indices[i+0]].Normal = p.Normal;
		buffer->Vertices[buffer->Indices[i+1]].Normal = p.Normal;
		buffer->Vertices[buffer->Indices[i+2]].Normal = p.Normal;
	}

	if (material)
		buffer->Material = *material;

	buffer->recalculateBoundingBox();

	SAnimatedMesh* animatedMesh = new SAnimatedMesh();
	mesh->addMeshBuffer(buffer);
	mesh->recalculateBoundingBox();
	animatedMesh->addMesh(mesh);
	animatedMesh->recalculateBoundingBox();

	mesh->drop();
	buffer->drop();

	return animatedMesh;
}


// This was the version of v0.0.1
// You can change the height of the terrain with heightstretch
IAnimatedMesh* terrain::renderterrain(const c8* name,
	const core::dimension2d<f32>& tileSize,
	video::SMaterial* material,
	const core::dimension2d<f32>& textureRepeatCount, int heightstretch)
{
	unsigned char mapheight;
	int h;
	s32 x = 0;
	s32 z = 0;

	SMeshBuffer* buffer = new SMeshBuffer();
	SMesh* mesh = new SMesh();
	video::S3DVertex vtx;
	vtx.Color.set(255,255,255,255);
	vtx.Normal.set(0,1,0);


	float halfX = (tileSize.Width * mapsizex) / 2;
	float halfY = (tileSize.Height * mapsizez) / 2;

	core::dimension2d<f32> tx;
	tx.Width = 1.0f / (mapsizex / textureRepeatCount.Width);
	tx.Height = 1.0f / (mapsizez / textureRepeatCount.Height);

	for (x=0; x<mapsizex; ++x)
		for (z=0; z<mapsizez; ++z)
		{
			vtx.Pos.set(tileSize.Width * x - halfX, 0, tileSize.Height * z - halfY);
			vtx.TCoords.set(-(f32)x * tx.Width, (f32)z * tx.Height);

			h=heightmap[x][z];
			vtx.Pos.Y = h*heightstretch;

			buffer->Vertices.push_back(vtx);
		}

	for (x=0; x<mapsizex-1; ++x)
		for (z=0; z<mapsizez-1; ++z)
		{
			s32 current = z*mapsizex + x;

			buffer->Indices.push_back(current);
			buffer->Indices.push_back(current + 1);
			buffer->Indices.push_back(current + mapsizex);

			buffer->Indices.push_back(current + 1);
			buffer->Indices.push_back(current + 1 + mapsizex);
			buffer->Indices.push_back(current + mapsizex);
		}

	for (s32 i=0; i<(s32)buffer->Indices.size(); i+=3)
	{
		core::plane3d<f32> p(
			buffer->Vertices[buffer->Indices[i+0]].Pos,
			buffer->Vertices[buffer->Indices[i+1]].Pos,
			buffer->Vertices[buffer->Indices[i+2]].Pos);
		p.Normal.normalize();

		buffer->Vertices[buffer->Indices[i+0]].Normal = p.Normal;
		buffer->Vertices[buffer->Indices[i+1]].Normal = p.Normal;
		buffer->Vertices[buffer->Indices[i+2]].Normal = p.Normal;
	}

	if (material)
		buffer->Material = *material;

	buffer->recalculateBoundingBox();

	SAnimatedMesh* animatedMesh = new SAnimatedMesh();
	mesh->addMeshBuffer(buffer);
	mesh->recalculateBoundingBox();
	animatedMesh->addMesh(mesh);
	animatedMesh->recalculateBoundingBox();

	mesh->drop();
	buffer->drop();

	return animatedMesh;
}


// Here you can see, how to colour direct the vertices
// you can use to do groundcolors and use a detailtexture
// for better look in the near or to do shading, for example
// with a grey cloudtexture for cloudshadows on the ground

/*
IAnimatedMesh* terrain::renderterrainwithvertexcolors(const c8* name,
	video::ITexture* texture,
	const core::dimension2d<f32>& tileSize,
	video::SMaterial* material,
	const core::dimension2d<f32>& textureRepeatCount,
	int heightstretch)

{
	core::dimension2d<s32> texdim;
	unsigned char mapheight, red, green, blue;
	int h, tRCw, tRCh, ired, igreen, iblue;
	s32 x = 0;
	s32 z = 0;
	s32 c = 0;

	tRCw=textureRepeatCount.Width;
	tRCh=textureRepeatCount.Width;

	s16 *p = (s16*)texture->lock();
	texdim = texture->getSize();
	s32 pitch = texture->getPitch() / 2;

	SMeshBuffer* buffer = new SMeshBuffer();
	SMesh* mesh = new SMesh();
	video::S3DVertex vtx;
	vtx.Color.set(255,255,255,255);
	vtx.Normal.set(0,1,0);


	float halfX = (tileSize.Width * mapsizex) / 2;
	float halfY = (tileSize.Height * mapsizez) / 2;

	core::dimension2d<f32> tx;
	tx.Width = 1.0f / (mapsizex / tRCw);
	tx.Height = 1.0f / (mapsizez / tRCh);

	for (x=0; x<mapsizex; ++x)
		for (z=0; z<mapsizez; ++z)
		{
			vtx.Pos.set(tileSize.Width * x - halfX, 0, tileSize.Height * z - halfY);
			vtx.TCoords.set(-(f32)x * tx.Width, (f32)z * tx.Height);

			h=heightmap[x][z];
			vtx.Pos.Y = h*heightstretch;

			//get colors from R5G5B5-format
			c=p[z*pitch+x];
			ired=((c >> 10)&0x1F ) *8;
			igreen=((c >> 5)&0x1F) *8;
			iblue=(c & 0x001F) *8;
			vtx.Color.set(255,ired,igreen,iblue);

			buffer->Vertices.push_back(vtx);
		}

	texture->unlock();

	for (x=0; x<mapsizex-1; ++x)
		for (z=0; z<mapsizez-1; ++z)
		{
			s32 current = z*mapsizex + x;

			buffer->Indices.push_back(current);
			buffer->Indices.push_back(current + 1);
			buffer->Indices.push_back(current + mapsizex);

			buffer->Indices.push_back(current + 1);
			buffer->Indices.push_back(current + 1 + mapsizex);
			buffer->Indices.push_back(current + mapsizex);
		}

	for (s32 i=0; i<(s32)buffer->Indices.size(); i+=3)
	{
		core::plane3d<f32> p(
			buffer->Vertices[buffer->Indices[i+0]].Pos,
			buffer->Vertices[buffer->Indices[i+1]].Pos,
			buffer->Vertices[buffer->Indices[i+2]].Pos);
		p.Normal.normalize();

		buffer->Vertices[buffer->Indices[i+0]].Normal = p.Normal;
		buffer->Vertices[buffer->Indices[i+1]].Normal = p.Normal;
		buffer->Vertices[buffer->Indices[i+2]].Normal = p.Normal;
	}

	if (material)
		buffer->Material = *material;

	buffer->recalculateBoundingBox();

	SAnimatedMesh* animatedMesh = new SAnimatedMesh();
	mesh->addMeshBuffer(buffer);
	mesh->recalculateBoundingBox();
	animatedMesh->addMesh(mesh);
	animatedMesh->recalculateBoundingBox();

	mesh->drop();
	buffer->drop();

	return animatedMesh;
}
*/
