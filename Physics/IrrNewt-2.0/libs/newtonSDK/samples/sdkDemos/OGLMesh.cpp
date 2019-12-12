/* Copyright (c) <2009> <Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely
*/

#include "toolbox_stdafx.h"
#include "dBone.h"
#include "dSkinModifier.h"
#include "OGLMesh.h"
#include "TargaToOpenGl.h"

dMatrix OGLMesh::m_matrixPalete[64];
dFloat OGLMesh::m_weigtedVertex[3 * 8096];
dFloat OGLMesh::m_weigtedNormal[3 * 8096];


#define	SPECULAR_SHINESS	127

OGLMesh::OGLMesh(const char* name)
	:dMesh(name)
{
}

OGLMesh::OGLMesh(const char* name, const NewtonCollision* collision, const char* texture0, const char* texture1, const char* texture2)
	:dMesh(name)
{
	NewtonMesh* mesh;

	// create a helper mesh from the collision collision
	mesh = NewtonMeshCreateFromCollision(collision);

	// apply the vertex normals
	NewtonMeshCalculateVertexNormals(mesh, 30.0f * 3.141592f/180.0f);

	// apply uv projections
	NewtonCollisionInfoRecord info;
	NewtonCollisionGetInfo (collision, &info);
	switch (info.m_collisionType) 
	{
		case SERIALIZE_ID_SPHERE:
		{
			NewtonMeshApplySphericalMapping(mesh, LoadTexture (texture0));
			break;
		}

		case SERIALIZE_ID_CONE:
		case SERIALIZE_ID_CYLINDER:
		{
			NewtonMeshApplyCylindricalMapping(mesh, LoadTexture(texture0), LoadTexture(texture1));
			break;
		}
		
		case SERIALIZE_ID_CAPSULE:
		case SERIALIZE_ID_CHAMFERCYLINDER:
		{
			NewtonMeshApplySphericalMapping(mesh, LoadTexture(texture0));
			break;
		}

		default:
		{
			int tex0;
			int tex1;
			int tex2;

			tex0 = LoadTexture(texture0);
			tex1 = LoadTexture(texture1);
			tex2 = LoadTexture(texture2);
			NewtonMeshApplyBoxMapping(mesh, tex0, tex1, tex2);
			break;
		}
	}

	BuildFromMesh (mesh);

	// destroy helper mesh
	NewtonMeshDestroy(mesh);

	// optimiz thsi mesh for hardwere buffers if possible
	OptimizeForRender ();
}

OGLMesh::~OGLMesh(void)
{
}


void OGLMesh::GetName (char* nameOut) const
{
	strcpy (nameOut, m_name);
}

void OGLMesh::GetTextureName (const dSubMesh* subMesh, char* nameOut) const
{
	const char* name = FindTextureById (subMesh->m_textureHandle);
	strcpy (nameOut, name);
}


void OGLMesh::OptimizeForRender ()
{
}


NewtonMesh* OGLMesh::BuildMesh (const NewtonWorld* world) const
{
	dFloat *uv;
	dFloat *point;
	dFloat *normal;
	NewtonMesh* mesh;

	// create a newton mesh
	mesh = NewtonMeshCreate(world);

	// add the triangles to the mesh 
	NewtonMeshBeginFace(mesh);

	point = m_vertex;
	normal = m_normal;
	uv = m_uv;
	for (dListNode* node = GetFirst(); node; node = node->GetNext()) {
		GLuint textureID;
		//GLuint *indexLixt;
		unsigned int *indexLixt;
		const dSubMesh& info = node->GetInfo();

		indexLixt = info.m_indexes;
		textureID = info.m_textureHandle; 
		for (int i = 0; i < info.m_indexCount; i += 3){
			struct TrianglePoint
			{
				dFloat m_x;
				dFloat m_y;
				dFloat m_z;
				dFloat m_nx;
				dFloat m_ny;
				dFloat m_nz;
				dFloat m_u0;
				dFloat m_v0;
				dFloat m_u1;
				dFloat m_v1;
			} triangle[3];

			for (int j = 0; j < 3; j ++) {
				int k; 

				k = indexLixt[i + j] * 3;
				triangle[j].m_x = point[k + 0];
				triangle[j].m_y = point[k + 1];
				triangle[j].m_z = point[k + 2];

				k = indexLixt[i + j] * 3;
				triangle[j].m_nx = normal[k + 0];
				triangle[j].m_ny = normal[k + 1];
				triangle[j].m_nz = normal[k + 2];

				k = indexLixt[i + j] * 2;
				triangle[j].m_u0 = uv[k + 0];
				triangle[j].m_v0 = uv[k + 1];

				triangle[j].m_u1 = uv[k + 0];
				triangle[j].m_v1 = uv[k + 1];
			}

			NewtonMeshAddFace(mesh, 3, &triangle[0].m_x, sizeof (TrianglePoint), textureID);
		}
	}

	// finish mesh constructions
	NewtonMeshEndFace(mesh);


/*
	int faceCount;
	int indexCount;
	faceCount = NewtonMeshGetTotalFaceCount (mesh);
	indexCount = NewtonMeshGetTotalIndexCount (mesh);

	int* faceIndexCount = new int[faceCount];
	int* faceMaterial = new int[faceCount];
	int* faceIndexArray = new int[indexCount];

	// get all of teh face in these array
	NewtonMeshGetFaces (mesh, faceIndexCount, faceMaterial, faceIndexArray);

	int* faceStart = new int[faceCount];
	int start = 0;
	for (int i = 0; i < faceCount; i ++)
	{
		faceStart[i] = start;
		start += faceIndexCount[i];
	}

	// now you are ready to use all the faces
	// faceIndexCount[0] number of indices of face 0
	// faceMaterial[0] matrail of face 0
	// faceIndexArray[faceStart[0]] start index of face 0

	// faceIndexCount[1] number of indices of face 1
	// faceMaterial[1] matrail of face 1
	// faceIndexArray[faceStart[1]] start index of face 1

	// ...


	// do not forget to delete teh arrays.
*/

	return mesh;
}



void OGLMesh::BuildFromMesh (const NewtonMesh* mesh)
{
	int vertexCount;

	// extract vertex data  from the newton mesh		
	vertexCount = NewtonMeshGetVertexCount (mesh); 
	AllocVertexData(vertexCount);
	NewtonMeshGetVertexStreams (mesh, 
		3 * sizeof (dFloat), (dFloat*) m_vertex,
		3 * sizeof (dFloat), (dFloat*) m_normal,
		2 * sizeof (dFloat), (dFloat*) m_uv, 
		2 * sizeof (dFloat), (dFloat*) m_uv);

	// extract the materials index array for mesh
	void* geometryHandle;
	geometryHandle = NewtonMeshBeginHandle (mesh); 
	for (int handle = NewtonMeshFirstMaterial (mesh, geometryHandle); handle != -1; handle = NewtonMeshNextMaterial (mesh, geometryHandle, handle)) {

		int material; 
		int indexCount; 
		dSubMesh* segment;

		material = NewtonMeshMaterialGetMaterial (mesh, geometryHandle, handle); 
		indexCount = NewtonMeshMaterialGetIndexCount (mesh, geometryHandle, handle); 

		segment = AddSubMesh();
		segment->m_textureHandle = (GLuint)material;

		segment->AllocIndexData (indexCount);
//		NewtonMeshMaterialGetIndexStreamShort (mesh, geometryHandle, handle, (short int*)segment->m_indexes); 
		NewtonMeshMaterialGetIndexStream (mesh, geometryHandle, handle, (int*)segment->m_indexes); 
	}
	NewtonMeshEndHandle (mesh, geometryHandle); 
}


void OGLMesh::Render(const dMatrix* matrixPallete, dSkinModifier* modifier)
{
	if (modifier) {
		RenderSoftwareSkin(matrixPallete, modifier);
	} else {
		RenderSoftwareSimpleMesh();
	}
}

void OGLMesh::RenderSoftwareSimpleMesh() const
{
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_NORMAL_ARRAY);
	glEnableClientState (GL_TEXTURE_COORD_ARRAY);

	glVertexPointer (3, GL_FLOAT, 0, m_vertex);
	glNormalPointer (GL_FLOAT, 0, m_normal);
	glTexCoordPointer (2, GL_FLOAT, 0, m_uv);

//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	for (dMesh::dListNode* nodes = GetFirst(); nodes; nodes = nodes->GetNext()) {
		dSubMesh& segment = nodes->GetInfo();

		glMaterialfv(GL_FRONT, GL_SPECULAR, &segment.m_specular.m_x);
		glMaterialfv(GL_FRONT, GL_AMBIENT, &segment.m_ambient.m_x);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, &segment.m_diffuse.m_x);
		glMaterialf(GL_FRONT, GL_SHININESS, segment.m_shiness);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		if (segment.m_textureHandle) {
			glEnable(GL_TEXTURE_2D);		
			glBindTexture(GL_TEXTURE_2D, GLuint (segment.m_textureHandle));
		} else {
			glDisable(GL_TEXTURE_2D);
		}

		glDrawElements (GL_TRIANGLES, segment.m_indexCount, GL_UNSIGNED_INT, segment.m_indexes);
	}
	glDisableClientState(GL_VERTEX_ARRAY);	// disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);	// disable normal arrays
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);	// disable normal arrays
}




void OGLMesh::RenderSoftwareSkin(const dMatrix* matrixPallete, dSkinModifier* skinModifier) const
{
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_NORMAL_ARRAY);
	glEnableClientState (GL_TEXTURE_COORD_ARRAY);

	for (int i = 0; i < skinModifier->m_bonesCount; i ++) {
		int id;
		id = skinModifier->m_skinnedBones[i]->m_boneID;
		m_matrixPalete[i] = skinModifier->m_shapeBindMatrix * skinModifier->m_bindingMatrices[i] * matrixPallete[id];
	}

	for (int i = 0; i < m_vertexCount; i ++) {
		dVector p (dFloat (0.0f), dFloat (0.0f), dFloat (0.0f), dFloat (0.0f));   
		dVector q (m_vertex[i * 3 + 0], m_vertex[i * 3 + 1], m_vertex[i * 3 + 2], dFloat (0.0f));

		const dVector& weight = skinModifier->m_vertexWeight[i];
		const dSkinModifier::dBoneWeightIndex& weightIndex = skinModifier->m_boneWeightIndex[i];
		for (int j = 0; (j < 4) && weight[j] > dFloat(0.0); j ++) {
			int boneIndex;
			boneIndex = weightIndex.m_index[j];
			p += m_matrixPalete[boneIndex].TransformVector (q).Scale (weight[j]);
		}

		m_weigtedVertex[i * 3 + 0] = dFloat (p.m_x);
		m_weigtedVertex[i * 3 + 1] = dFloat (p.m_y);
		m_weigtedVertex[i * 3 + 2] = dFloat (p.m_z);

		dVector n (m_normal[i * 3 + 0], m_normal[i * 3 + 1], m_normal[i * 3 + 2], dFloat (0.0f));
		n = m_matrixPalete[weightIndex.m_index[0]].RotateVector (n);
		m_weigtedNormal[i * 3 + 0] = dFloat (n.m_x);
		m_weigtedNormal[i * 3 + 1] = dFloat (n.m_y);
		m_weigtedNormal[i * 3 + 2] = dFloat (n.m_z);
	}

	glVertexPointer (3, GL_FLOAT, 0, m_weigtedVertex);
	glNormalPointer (GL_FLOAT, 0, m_weigtedNormal);
	glTexCoordPointer (2, GL_FLOAT, 0, m_uv);

	for (dMesh::dListNode* nodes = GetFirst(); nodes; nodes = nodes->GetNext()) {
		dSubMesh& segment = nodes->GetInfo();

		glMaterialfv(GL_FRONT, GL_SPECULAR, &segment.m_specular.m_x);
		glMaterialfv(GL_FRONT, GL_AMBIENT, &segment.m_ambient.m_x);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, &segment.m_diffuse.m_x);
		glMaterialf(GL_FRONT, GL_SHININESS, segment.m_shiness);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		if (segment.m_textureHandle) {
			glEnable(GL_TEXTURE_2D);		
			glBindTexture(GL_TEXTURE_2D, GLuint (segment.m_textureHandle));
		} else {
			glDisable(GL_TEXTURE_2D);
		}

		glDrawElements (GL_TRIANGLES, segment.m_indexCount, GL_UNSIGNED_INT, segment.m_indexes);
	}
	glDisableClientState(GL_VERTEX_ARRAY);	// disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);	// disable normal arrays
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);	// disable normal arrays
}


