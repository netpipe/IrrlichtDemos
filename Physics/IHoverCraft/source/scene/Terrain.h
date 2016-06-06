#ifndef TERRAIN_H_
#define TERRAIN_H_

#include <ITerrainSceneNode.h>

#include "../physics/Physics.h"
class game;
class Game;

class Terrain
{

private:

	irr::scene::ITerrainSceneNode* terrain;


	Physics* physics;

	///A handle to the geometry data of the object, the main handle for collision interactions.
	dGeomID geom;

	dVector3* vertices;

	dTriIndex* indices;

	dTriMeshDataID data;

	const void initOde();

	const void makeTriMeshData()
	{
		irr::scene::SMeshBufferLightMap mb;
//		this->terrain->getMeshBufferForLOD(mb, 0);
	//	mb.Vertices.set_free_when_destroyed(false); //valgrind detects a memory leak if uncommented
	//	mb.Indices.set_free_when_destroyed(false);

		this->vertices = new dVector3[mb.getVertexCount()];
		this->indices = new dTriIndex[mb.getIndexCount()];

		const irr::core::vector3df& scale = terrain->getScale();

		irr::u32 i;
		for (i = 0; i < mb.getVertexCount(); i++)
		{
			this->vertices[i][0] = mb.Vertices[i].Pos.X*scale.X;
			this->vertices[i][1] = mb.Vertices[i].Pos.Y*scale.Y;
			this->vertices[i][2] = mb.Vertices[i].Pos.Z*scale.Z;
		}

		irr::u32 j;
		for (j = 0; j < mb.getIndexCount(); ++j)
			this->indices[j] = mb.Indices[j];


		this->data = dGeomTriMeshDataCreate();
		dGeomTriMeshDataBuildSimple(
			this->data, // the dTriMeshDataID to use
			(const dReal*)this->vertices, mb.getVertexCount(),
			(const dTriIndex*)this->indices, mb.getIndexCount()
		);
	}

public:

	Terrain(Game* game);

	~Terrain()
	{
		dGeomDestroy(this->geom);
		dGeomTriMeshDataDestroy(data);

		delete[] this->indices;
		delete[] this->vertices;

		this->terrain->remove();
		this->terrain->drop();
	}

	inline irr::scene::ITerrainSceneNode* getTerrain() const { return this->terrain; }

	inline const dGeomID getGeom() const { return this->geom; }

};

#endif /*TERRAIN_H_*/
