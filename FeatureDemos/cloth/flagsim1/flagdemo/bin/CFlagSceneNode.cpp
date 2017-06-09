//********************************************************************************************
// File:	CFlagSceneNode.cpp
// Purpose:	Realtime flag vertex animation. Based on Flagsim by
//			created by Patrick Engel and Philipp Crocoll February, 2003. Change the way
//			normal are created to a more robust method.
// Started: 24th October 2011
// Last ed: 25th October 2011
// History:
//********************************************************************************************

#include "CFlagSceneNode.h"

//********************************************************************************************
// renders the node.

void CFlagSceneNode::render()
{
	IVideoDriver* driver = SceneManager->getVideoDriver();

	if (!Mesh || !driver)
		return;

	IMeshBuffer* mb = Mesh->getMeshBuffer(0);
	driver->setTransform(ETS_WORLD, AbsoluteTransformation);
	driver->setMaterial(getMaterial());
	driver->drawMeshBuffer(mb);

// debug stuff
DebugDataVisible=true;
	if(DebugDataVisible)
	{
		SMaterial m;
		m.Lighting = true;
		m.AntiAliasing=0;

// bounding box
		if (DebugDataVisible & EDS_BBOX)
		{
			m.DiffuseColor = SColor(255,0,0,0);
			driver->setMaterial(m);
			driver->draw3DBox(Mesh->getMeshBuffer(0)->getBoundingBox(), SColor(255,255,255,255));
		}

// draw normals
EDS_NORMALS == true;
		if (DebugDataVisible & EDS_NORMALS)
		{
			vector3df normalizedNormal;
			const f32 DebugNormalLength = SceneManager->getParameters()->getAttributeAsFloat(DEBUG_NORMAL_LENGTH);
			const SColor DebugNormalColor = SceneManager->getParameters()->getAttributeAsColor(DEBUG_NORMAL_COLOR);

			const IMeshBuffer* mb = Mesh->getMeshBuffer(0);
			const u32 vSize = getVertexPitchFromType(mb->getVertexType());
			const S3DVertex* v = ( const S3DVertex*)mb->getVertices();
			const bool normalize = mb->getMaterial().NormalizeNormals;

			for (u32 i=0; i != mb->getVertexCount(); ++i)
			{
				normalizedNormal = v->Normal;
				if (normalize)
					normalizedNormal.normalize();

				driver->draw3DLine(v->Pos, v->Pos + (normalizedNormal * DebugNormalLength), DebugNormalColor);

				v = (const S3DVertex*) ((u8*)v + vSize);
			}
			m.DiffuseColor = SColor(255,255,0,0);
			driver->setMaterial(m);
			driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		}

// show wireframe
		if (DebugDataVisible & EDS_MESH_WIRE_OVERLAY)
		{
		    EDS_MESH_WIRE_OVERLAY == true;
			m.Wireframe = false;
			m.DiffuseColor = SColor(255,0,0,150);
			driver->setMaterial(m);
			driver->drawMeshBuffer(Mesh->getMeshBuffer(0));
		}
	}
}

//********************************************************************************************
// create the initial flag mesh

void CFlagSceneNode::Initialize(int xsubdiv, int ysubdiv, float FlagSize)
{
// create a new mesh buffer

	SMeshBuffer* buffer = new SMeshBuffer();

//Init vertex buffer array along with accel & velocities

	u32 numVertices = xsubdiv * ysubdiv;

	buffer->Vertices.set_used(numVertices);

	velocity = new vector3df[numVertices];
	accel = new vector3df[numVertices];

	directdistance = FlagSize/(xsubdiv - 1);
	aslantdistance = sqrt(2.0f) * directdistance;

// create the verts

	for (int yc = 0; yc < ysubdiv; yc++)
		for (int xc = 0; xc < xsubdiv; xc++)
		{
			int ArrayPos = yc*xsubdiv+xc;

			buffer->Vertices[ArrayPos].Pos = vector3df(xc * directdistance, yc * directdistance, 0.0f);
			buffer->Vertices[ArrayPos].Normal = vector3df(0.0f, 0.0f, -1.0f);
			buffer->Vertices[ArrayPos].TCoords = vector2df(xc/float(xsubdiv-1),-yc/float(ysubdiv-1));

			velocity[ArrayPos] = NULL_VECTOR;
			accel[ArrayPos] = NULL_VECTOR;
		}

// Init index array:

	u32 numIndices = 3 * 2 * (xsubdiv - 1) * (ysubdiv - 1);
	buffer->Indices.set_used(numIndices);

// create the indices

	for (int yc = 0; yc < ysubdiv - 1; yc++)
		for (int xc = 0; xc < xsubdiv - 1; xc++)
		{
// Create two tris

			int ArrayPos = 6 * ( yc * (xsubdiv - 1) + xc);
			buffer->Indices[ArrayPos + 2] = yc * xsubdiv + xc;
			buffer->Indices[ArrayPos + 1] = yc * xsubdiv + xc + 1;
			buffer->Indices[ArrayPos + 0] = (yc + 1) * (xsubdiv) + xc + 1;

			buffer->Indices[ArrayPos + 5] = yc * xsubdiv + xc;
			buffer->Indices[ArrayPos + 4] = (yc + 1) * xsubdiv + xc + 1;
			buffer->Indices[ArrayPos + 3] = (yc + 1) * xsubdiv + xc;
		}

	Xsubdivision = xsubdiv;
	Ysubdivision = ysubdiv;

// set vert buffer to stream and indices to static

	buffer->setHardwareMappingHint(EHM_STREAM,EBT_VERTEX);
	buffer->setHardwareMappingHint(EHM_STATIC,EBT_INDEX);

// create the mesh and add our mbuffer

	Mesh = new SMesh();
	Mesh->addMeshBuffer(buffer);
	buffer->drop();
}

//********************************************************************************************

void CFlagSceneNode::GetForce(int xOtherVertex, int yOtherVertex, vector3df& VertexPos, float StandardDistance, vector3df& result)
{
	if ((xOtherVertex >= 0 ) && (xOtherVertex < Xsubdivision) && (yOtherVertex >= 0 ) && (yOtherVertex < Ysubdivision))
	{
		S3DVertex* verts = (S3DVertex*)Mesh->getMeshBuffer(0)->getVertices();

		vector3df Distance;
		VectorSubtract(verts[xOtherVertex + Xsubdivision * yOtherVertex].Pos,VertexPos,Distance);
		float len = VectorLength(Distance);
		if(len == 0.0f)
			result = NULL_VECTOR;  //not correct, but cannot be handled another way..

		VectorMultiply(Distance,((len - StandardDistance)/len), result );
		VectorMultiply(result,spring_const, result );

	//	if (VectorLength(result) > 10 )
	//		result = NULL_VECTOR;

	}
	else
		result = NULL_VECTOR;
}

//********************************************************************************************
// my brute force method, replaces the original pervertex approach as it had edge issues

void CFlagSceneNode::UpdateNormals()
{
	S3DVertex*	verts = (S3DVertex*)Mesh->getMeshBuffer(0)->getVertices();
	u16*		indices = (u16*)Mesh->getMeshBuffer(0)->getIndices();
	vector3df	v1,v2,v3, u, v, n;

// sum all normals from each face for all verts

	for(u32 i = 0; i < Mesh->getMeshBuffer(0)->getIndexCount(); i += 3)
	{
		v1 = verts[indices[i]].Pos;
		v2 = verts[indices[i + 1]].Pos;
		v3 = verts[indices[i + 2]].Pos;

		VectorSubtract(v1,v2, u);
		VectorSubtract(v1,v3, v);
		CrossProduct(u,v,n);

// keep adding them normals

		VectorAdd(verts[indices[i]].Normal,n,verts[indices[i]].Normal);
		VectorAdd(verts[indices[i + 1]].Normal,n,verts[indices[i + 1]].Normal);
		VectorAdd(verts[indices[i + 2]].Normal,n,verts[indices[i + 2]].Normal);
	}

// average all the normals per vert

	for(u32 v = 0; v < Mesh->getMeshBuffer(0)->getVertexCount(); v++)
		VectorNormalizeFast(verts[v].Normal);
}

//********************************************************************************************

void CFlagSceneNode::UpdateVertices(float timeOffset)
{
	S3DVertex* verts = (S3DVertex*)Mesh->getMeshBuffer(0)->getVertices();

// get the current wind

	vector3df wind,winddir;
	windgen->get(timeOffset,wind,winddir);

// compute the forces

	for (int xc = 0; xc < Xsubdivision; xc ++)
	{
		for (int yc = 0; yc < Ysubdivision; yc++)
		{
			if (xc > 0) // leave first row of verts static
			{
				int ArrayPos = yc * Xsubdivision + xc;
				vector3df VertexPos = verts[ArrayPos].Pos;
				vector3df force;
				accel[ArrayPos] = gravity;

				GetForce(xc - 1,yc, VertexPos, directdistance,force);
				VectorAdd(accel[ArrayPos],force, accel[ArrayPos]);

				GetForce(xc + 1,yc, VertexPos, directdistance,force);
				VectorAdd(accel[ArrayPos],force, accel[ArrayPos]);

				GetForce(xc,yc - 1, VertexPos, directdistance,force);
				VectorAdd(accel[ArrayPos],force, accel[ArrayPos]);

				GetForce(xc,yc + 1, VertexPos, directdistance,force);
				VectorAdd(accel[ArrayPos],force, accel[ArrayPos]);

				GetForce(xc - 1,yc - 1,VertexPos, aslantdistance,force);
				VectorAdd(accel[ArrayPos],force, accel[ArrayPos]);

				GetForce(xc - 1,yc + 1,VertexPos, aslantdistance,force);
				VectorAdd(accel[ArrayPos],force, accel[ArrayPos]);

				GetForce(xc + 1,yc - 1,VertexPos, aslantdistance,force);
				VectorAdd(accel[ArrayPos],force, accel[ArrayPos]);

				GetForce(xc + 1,yc + 1,VertexPos, aslantdistance,force);
				VectorAdd(accel[ArrayPos],force, accel[ArrayPos]);

				float dp =  DotProduct(winddir, verts[ArrayPos].Normal);
				if(dp < 0.0f)
					dp *= -1;
				accel[ArrayPos] = accel[ArrayPos] + wind * dp ;
			}
			else
				accel[yc * Xsubdivision + xc] = NULL_VECTOR;
		}
	}

// calc new position

	for (int xc = 0; xc < Xsubdivision; xc ++)
	{
		for (int yc = 0; yc < Ysubdivision; yc++)
		{
			int ArrayPos = yc * Xsubdivision + xc;

			velocity[ArrayPos] = velocity[ArrayPos] + accel[ArrayPos] * deltatime;
			velocity[ArrayPos] = velocity[ArrayPos] * damping;
			verts[ArrayPos].Pos = verts[ArrayPos].Pos + velocity[ArrayPos] * deltatime;
		}
	}
	UpdateNormals();
	Mesh->getMeshBuffer(0)->recalculateBoundingBox();
}

//********************************************************************************************
