
#include "ode_utility.h"

void ODESetRotation(core::matrix4 source, dMatrix3 dest)
{
	source = source.getTransposed();
	
	for (int i = 0; i < 3; i ++)
	{
		int j = 0;
		for (; j < 3; j ++)
		{
			dest[i*4+j] = source[i*4+j];
		}
		dest[i*4+j+1] = 0;
	}
}

void ODESetRotation(const dMatrix3 source, core::matrix4 &dest)
{
	dest = dest.getTransposed();
	
	for (int i = 0; i < 3; i ++)
	{
		int j = 0;
		for (; j < 3; j ++)
		{
			dest[i*4+j] = source[i*4+j];
		}
		dest[i*4+j+1] = 0;
	}
	
	dest = dest.getTransposed();
}

core::matrix4 ODEGetBodyTransformation( dBodyID b )
{
	core::matrix4 m;
	const dReal *pos = dBodyGetPosition(b);
	ODESetRotation(dBodyGetRotation(b), m);
	m.setTranslation( core::vector3df(pos[0], pos[1], pos[2]) );
	return m;
}
core::matrix4 ODEGetGeomTransformation( dGeomID g )
{
	core::matrix4 m;
	const dReal *pos = dGeomGetPosition(g);
	ODESetRotation(dGeomGetRotation(g), m);
	m.setTranslation( core::vector3df(pos[0], pos[1], pos[2]) );
	return m;
}

void ODESetBodyTransformation( dBodyID b, const core::matrix4 &m )
{
	core::vector3df pos = m.getTranslation();
	dBodySetPosition(b, pos.X, pos.Y, pos.Z);
	dMatrix3 r;
	ODESetRotation(m, r);
	dBodySetRotation(b, r);
}
void ODESetGeomTransformation( dGeomID g, const core::matrix4 &m )
{
	core::vector3df pos = m.getTranslation();
	dGeomSetPosition(g, pos.X, pos.Y, pos.Z);
	dMatrix3 r;
	ODESetRotation(m, r);
	dGeomSetRotation(g, r);
}

core::vector3df ODEGetBodyPosition( dBodyID b )
{
	const dReal *pos = dBodyGetPosition(b);
	return core::vector3df(pos[0],pos[1],pos[2]);
}
core::vector3df ODEGetGeomPosition( dGeomID g )
{
	const dReal *pos = dGeomGetPosition(g);
	return core::vector3df(pos[0],pos[1],pos[2]);
}

void ODESetBodyPosition( dBodyID b, const core::vector3df &pos )
{
	dBodySetPosition(b, pos.X,pos.Y,pos.Z);
}
void ODESetGeomPosition( dGeomID g, const core::vector3df &pos )
{
	dGeomSetPosition(g, pos.X,pos.Y,pos.Z);
}

core::vector3df ODEGetBodyRotation( dBodyID b )
{
	core::matrix4 m;
	ODESetRotation(dBodyGetRotation(b), m);
	return m.getRotationDegrees();
}
core::vector3df ODEGetGeomRotation( dGeomID g )
{
	core::matrix4 m;
	ODESetRotation(dGeomGetRotation(g), m);
	return m.getRotationDegrees();
}

void ODESetBodyRotation( dBodyID b, const core::vector3df &rot )
{
	core::matrix4 m;
	m.setRotationDegrees(rot);
	dMatrix3 r;
	ODESetRotation(m, r);
	dBodySetRotation(b, r);
}
void ODESetGeomRotation( dGeomID g, const core::vector3df &rot )
{
	core::matrix4 m;
	m.setRotationDegrees(rot);
	dMatrix3 r;
	ODESetRotation(m, r);
	dGeomSetRotation(g, r);
}

// Makes an ODEMesh structure from an Irrlicht mesh
// All meshbuffers are combined into a single mesh.
// An improvement might be to keep them as separate geoms...
ODEMeshData *ODECreateMeshData(scene::IMesh *mesh, core::vector3df scale = core::vector3df(1,1,1))
{
	core::aabbox3df boundingBox;
	
	// Count total vertex and index counts from all mesh buffers
	u32 vertexCountTotal = 0;
	u32 indexCountTotal = 0;
	for (u32 i = 0; i < mesh->getMeshBufferCount(); i ++)
	{
		scene::IMeshBuffer *mb = mesh->getMeshBuffer(i);
		vertexCountTotal += mb->getVertexCount();
		indexCountTotal += mb->getIndexCount();
	}
	
	// Create ODE storage.
	dVector3 *vertices = new dVector3[vertexCountTotal];
	int *indices = new int[indexCountTotal];
	
	// Fill ODE mesh data...
	vertexCountTotal = 0;
	indexCountTotal = 0;
	for (u32 i = 0; i < mesh->getMeshBufferCount(); i ++)
	{
		scene::IMeshBuffer *mb = mesh->getMeshBuffer(i);
		
		u32 vertexCount = mb->getVertexCount();
		u32 indexCount = mb->getIndexCount();
		
		// Copy vertex positions, depending on Irrlicht vertex type
		
		switch (mb->getVertexType())
		{
		case video::EVT_STANDARD:
			{
				video::S3DVertex *iVertices = (video::S3DVertex *)mb->getVertices();
				for (u32 j = 0; j < vertexCount; j ++)
				{
					vertices[vertexCountTotal+j][0] = iVertices[j].Pos.X*scale.X;
					vertices[vertexCountTotal+j][1] = iVertices[j].Pos.Y*scale.Y;
					vertices[vertexCountTotal+j][2] = iVertices[j].Pos.Z*scale.Z;
					vertices[vertexCountTotal+j][3] = 0;			
				}
				break;
			}
		case video::EVT_2TCOORDS:
			{
				video::S3DVertex2TCoords *iVertices = (video::S3DVertex2TCoords *)mb->getVertices();
				for (u32 j = 0; j < vertexCount; j ++)
				{
					vertices[vertexCountTotal+j][0] = iVertices[j].Pos.X*scale.X;
					vertices[vertexCountTotal+j][1] = iVertices[j].Pos.Y*scale.Y;
					vertices[vertexCountTotal+j][2] = iVertices[j].Pos.Z*scale.Z;
					vertices[vertexCountTotal+j][3] = 0;			
				}
				break;
			}
		case video::EVT_TANGENTS:
			{
				video::S3DVertex *iVertices = (video::S3DVertexTangents *)mb->getVertices();
				for (u32 j = 0; j < vertexCount; j ++)
				{
					vertices[vertexCountTotal+j][0] = iVertices[j].Pos.X*scale.X;
					vertices[vertexCountTotal+j][1] = iVertices[j].Pos.Y*scale.Y;
					vertices[vertexCountTotal+j][2] = iVertices[j].Pos.Z*scale.Z;
					vertices[vertexCountTotal+j][3] = 0;			
				}
				break;
			}
		}
		
		// Calculating bounds...
		for (u32 j = 0; j < vertexCount; j ++)
		{
			boundingBox.addInternalPoint(
					vertices[vertexCountTotal+j][0],
					vertices[vertexCountTotal+j][1],
					vertices[vertexCountTotal+j][2]
					);
		}
		
		u16 *iIndices = mb->getIndices();
		
		for (u32 j = 0; j < indexCount; j ++)
		{
			indices[indexCountTotal+j] = vertexCountTotal+iIndices[j];
		}
		
		vertexCountTotal += vertexCount;
		indexCountTotal += indexCount;
	}
	
	// Make geoms and stuff
	dTriMeshDataID triMeshData = dGeomTriMeshDataCreate();
	
	dGeomTriMeshDataBuildSingle(triMeshData,
		(const dReal *)vertices, 4*sizeof(dReal), vertexCountTotal,
		indices, indexCountTotal, 3*sizeof(int));
	
	ODEMeshData *md	= new ODEMeshData;
	md->triMeshData	= triMeshData;
	md->vertices	= vertices;
	md->indices		= indices;
	md->size		= boundingBox.getExtent();
	
	return md;
}


