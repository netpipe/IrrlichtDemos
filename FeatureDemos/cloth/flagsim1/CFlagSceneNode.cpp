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
DebugDataVisible==true;
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
EDS_NORMALS==true;
		if (DebugDataVisible & EDS_NORMALS)
		{
			vector3df normalizedNormal;
			const f32 DebugNormalLength = SceneManager->getParameters()->getAttributeAsFloat(DEBUG_NORMAL_LENGTH);
			const SColor DebugNormalColor = SceneManager->getParameters()->getAttributeAsColor(DEBUG_NORMAL_COLOR);

			const IMeshBuffer* mb = Mesh->getMeshBuffer(0);
			const u32 vSize = getVertexPitchFromType(mb->getVertexType());
			const S3DVertex* v = ( const S3DVertex*)mb->getVertices();
			const bool normalize = mb->getMaterial().NormalizeNormals;

			u32 vcount = mb->getVertexCount();
			for (u32 i=0; i != vcount; ++i)
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
			m.Wireframe = false;
			m.DiffuseColor = SColor(255,0,0,150);
			driver->setMaterial(m);
			driver->drawMeshBuffer(Mesh->getMeshBuffer(0));
		}
	}
}

//********************************************************************************************
// create the initial flag mesh

void CFlagSceneNode::Initialize(int xsubdiv, int ysubdiv, float FlagSize,IImage* vmap)
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

	dimension2di vmapdim;
	if(vmap)
		vmapdim = vmap->getDimension();

	for (int yc = 0; yc < ysubdiv; yc++)
		for (int xc = 0; xc < xsubdiv; xc++)
		{
			int ArrayPos = yc*xsubdiv+xc;

			buffer->Vertices[ArrayPos].Pos = vector3df(xc * directdistance, yc * directdistance, 0.0f);
			buffer->Vertices[ArrayPos].Normal = vector3df(0.0f, 0.0f, -1.0f);
			buffer->Vertices[ArrayPos].TCoords = vector2df(xc/float(xsubdiv-1),-yc/float(ysubdiv-1));

			if(vmap)
			{
				u32 x = (xc/float(xsubdiv)) * vmapdim.Width;
				u32 y = (yc/float(ysubdiv)) * vmapdim.Height;
				buffer->Vertices[ArrayPos].Color = vmap->getPixel(x,vmapdim.Height - y - 1);
			}
			else // no map set first row to black, rest white
				buffer->Vertices[ArrayPos].Color = (xc == 0) ? 0x0 : 0xffffffff;

			velocity[ArrayPos] = vector3df(0.0f, 0.0f, 0.0f);
			accel[ArrayPos] = vector3df(0.0f, 0.0f, 0.0f);
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

		vector3df Distance = verts[xOtherVertex + Xsubdivision * yOtherVertex].Pos - VertexPos;
		float len = Distance.getLength();
		if(len == 0.0f)
			len = 0.01f;
		result = Distance * (len - StandardDistance)/len;
		result = result * spring_const;
	}
	else
		result = vector3df(0.0f, 0.0f, 0.0f);
}

//********************************************************************************************
// my brute force method, replaces the original pervertex approach as it had edge issues

void CFlagSceneNode::UpdateNormals()
{
	S3DVertex*	verts = (S3DVertex*)Mesh->getMeshBuffer(0)->getVertices();
	u16*		indices = (u16*)Mesh->getMeshBuffer(0)->getIndices();
	vector3df	v1,v2,v3, u, v, n;

// sum all normals from each face for all verts

	u32 numIndices = Mesh->getMeshBuffer(0)->getIndexCount();
	for(u32 i = 0; i < numIndices; i += 3)
	{
		v1 = verts[indices[i]].Pos;
		v2 = verts[indices[i + 1]].Pos;
		v3 = verts[indices[i + 2]].Pos;

// calc the normal

		u = v1 - v2;
		v = v1 - v3;
		n = u.crossProduct(v);

// keep adding them normals

		verts[indices[i]].Normal = verts[indices[i]].Normal + n;
		verts[indices[i+1]].Normal = verts[indices[i+1]].Normal + n;
		verts[indices[i+2]].Normal = verts[indices[i+2]].Normal + n;
	}

// average all the normals per vert

	u32 numVerts = Mesh->getMeshBuffer(0)->getVertexCount();
	for(u32 v = 0; v < numVerts; v++)
		verts[v].Normal.normalize();
}

//********************************************************************************************

void CFlagSceneNode::UpdateVertices(float timeOffset)
{
	S3DVertex* verts = (S3DVertex*)Mesh->getMeshBuffer(0)->getVertices();
	int numverts = Mesh->getMeshBuffer(0)->getVertexCount();

// get the current wind

	vector3df wind,winddir;
	windgen->get(timeOffset,wind,winddir);

// compute the forces

	for (int xc = 0; xc < Xsubdivision; xc ++)
	{
		for (int yc = 0; yc < Ysubdivision; yc++)
		{
			int ArrayPos = yc * Xsubdivision + xc;
			vector3df VertexPos = verts[ArrayPos].Pos;
			vector3df force;
			SColor vcol = verts[ArrayPos].Color;
			accel[ArrayPos] = gravity;

			GetForce(xc - 1,yc, VertexPos, directdistance,force);
			accel[ArrayPos] = accel[ArrayPos] + force;

			GetForce(xc + 1,yc, VertexPos, directdistance,force);
			accel[ArrayPos] = accel[ArrayPos] + force;

			GetForce(xc,yc - 1, VertexPos, directdistance,force);
			accel[ArrayPos] = accel[ArrayPos] + force;

			GetForce(xc,yc + 1, VertexPos, directdistance,force);
			accel[ArrayPos] = accel[ArrayPos] + force;

			GetForce(xc - 1,yc - 1,VertexPos, aslantdistance,force);
			accel[ArrayPos] = accel[ArrayPos] + force;

			GetForce(xc - 1,yc + 1,VertexPos, aslantdistance,force);
			accel[ArrayPos] = accel[ArrayPos] + force;

			GetForce(xc + 1,yc - 1,VertexPos, aslantdistance,force);
			accel[ArrayPos] = accel[ArrayPos] + force;


			GetForce(xc + 1,yc + 1,VertexPos, aslantdistance,force);
			accel[ArrayPos] = accel[ArrayPos] + force;


			float dp =  winddir.dotProduct(verts[ArrayPos].Normal);
			if(dp < 0.0f)
				dp *= -1;
			accel[ArrayPos] = (accel[ArrayPos] + wind * dp) * vcol.getRed()/255.0f;
		}
	}

// calc new position

	for (int i = 0; i < numverts; i++)
	{
		velocity[i] = velocity[i] + accel[i] * deltatime;
		velocity[i] = velocity[i] * damping;
		verts[i].Pos = verts[i].Pos + velocity[i] * deltatime;
	}
	UpdateNormals();
	Mesh->getMeshBuffer(0)->recalculateBoundingBox();
}

//********************************************************************************************
