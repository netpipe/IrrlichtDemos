/////////////////////////////////////////////////////////////////////////////
// Name:        alchemediaStaticMesh.cpp
// Purpose:     
// Author:      Julio Jerez
// Modified by: 
// Created:     22/05/2010 08:02:08
// RCS-ID:      
// Copyright:   Copyright (c) <2010> <Newton Game Dynamics>
// License:     
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely
/////////////////////////////////////////////////////////////////////////////

#include "alchemedia.h"
#include "alchemediaScene.h"
#include "alchemediaStaticMesh.h"


D_ALCHEMDIA_REGISTER_CLASS(alchemediaStaticMesh);

alchemediaStaticMesh::alchemediaStaticMesh(void)
	:dMeshNodeInfo (), m_solidDipalyList(0), m_gouraudDipalyList(0), m_wireFrameDipalyList(0)
{
}

alchemediaStaticMesh::alchemediaStaticMesh(dScene* world)
	:dMeshNodeInfo (world), m_solidDipalyList(0), m_gouraudDipalyList(0), m_wireFrameDipalyList(0)
{
}

alchemediaStaticMesh::alchemediaStaticMesh(NewtonMesh* mesh)
	:dMeshNodeInfo (mesh), m_solidDipalyList(0), m_gouraudDipalyList(0), m_wireFrameDipalyList(0)
{
	_ASSERTE (0);
}

alchemediaStaticMesh::alchemediaStaticMesh(const alchemediaStaticMesh& me)
	:dMeshNodeInfo (me), m_solidDipalyList(0), m_gouraudDipalyList(0), m_wireFrameDipalyList(0)
{
}


alchemediaStaticMesh::~alchemediaStaticMesh(void)
{
	if (m_solidDipalyList) {
		glDeleteLists(m_gouraudDipalyList, 1);
	}

	if (m_gouraudDipalyList) {
		glDeleteLists(m_gouraudDipalyList, 1);
	}

	if (m_wireFrameDipalyList) {
		glDeleteLists(m_wireFrameDipalyList, 1);
	}
}

void alchemediaStaticMesh::InvalidateCache()
{
	if (m_solidDipalyList) {
		glDeleteLists(m_gouraudDipalyList, 1);
	}

	if (m_gouraudDipalyList) {
		glDeleteLists(m_gouraudDipalyList, 1);
	}

	if (m_wireFrameDipalyList) {
		glDeleteLists(m_wireFrameDipalyList, 1);
	}
}

void alchemediaStaticMesh::ConvertToPolygons()
{
	dMeshNodeInfo::ConvertToPolygons(); 
	InvalidateCache();
}

void alchemediaStaticMesh::ConvertToTriangles()
{
	dMeshNodeInfo::ConvertToTriangles();
	InvalidateCache();
}

void alchemediaStaticMesh::WeldTJoints ()
{
	dMeshNodeInfo::WeldTJoints ();
	InvalidateCache();
}

void alchemediaStaticMesh::RemoveUnusedVertices(dScene* const world, dScene::dTreeNode* const myNode)
{
	dMeshNodeInfo::RemoveUnusedVertices(world, myNode);
	InvalidateCache();
}

void alchemediaStaticMesh::SmoothNormals (dFloat angleInRadiants)
{
	dMeshNodeInfo::SmoothNormals (angleInRadiants);
	InvalidateCache();
}


void alchemediaStaticMesh::DrawWireFrame(dScene* const world, dScene::dTreeNode* const myNode, const dVector& color) const
{
	_ASSERTE (world->GetInfoFromNode(myNode) == this);


	if (!m_wireFrameDipalyList) {
		m_wireFrameDipalyList = glGenLists(1);

		glNewList(m_wireFrameDipalyList, GL_COMPILE);

		int stride = NewtonMeshGetVertexStrideInByte(m_mesh) / sizeof (dFloat);
		float* const vertexArray = NewtonMeshGetVertexArray(m_mesh);

		glColor3f (color.m_x, color.m_y, color.m_z);
		glBegin(GL_LINES);
		for (void* edge = NewtonMeshGetFirstEdge (m_mesh); edge; edge = NewtonMeshGetNextEdge (m_mesh, edge)) {
			int i0;
			int i1;
			NewtonMeshGetEdgeIndices (m_mesh, edge, &i0, &i1);

			int index = i0 * stride;
			dVector point0 (vertexArray[index], vertexArray[index + 1], vertexArray[index + 2], 0.0f);

			index = i1 * stride;
			dVector point1 (vertexArray[index], vertexArray[index + 1], vertexArray[index + 2], 0.0f);

//int xxx = NewtonMeshGetFaceMaterial (m_mesh, edge);
//if (xxx == 16){	

			glVertex3f(point0.m_x, point0.m_y, point0.m_z);
			glVertex3f(point1.m_x, point1.m_y, point1.m_z);
//}
		}

		glEnd();
		glEndList();
	}

	glPushMatrix();
	glMultMatrix(&m_matrix[0][0]);

	glCallList(m_wireFrameDipalyList);

	glPopMatrix();
}


void alchemediaStaticMesh::DrawSolidWireFrame(dScene* const world, dScene::dTreeNode* const myNode, const dVector& color) const
{
	if (!m_solidDipalyList) {

		m_solidDipalyList = glGenLists(1);

		glNewList(m_solidDipalyList, GL_COMPILE);


		int stride = NewtonMeshGetVertexStrideInByte(m_mesh) / sizeof (dFloat);;
		float* const vertexArray = NewtonMeshGetVertexArray(m_mesh);
		
		glColor3f (0.5f, 0.5f, 0.5f);
		glBegin(GL_TRIANGLES);
		for (void* face = NewtonMeshGetFirstFace (m_mesh); face; face = NewtonMeshGetNextFace (m_mesh, face)) {
			if (!NewtonMeshIsFaceOpen (m_mesh, face)) {
				int indices[1024];
				int vertexCount = NewtonMeshGetFaceIndexCount (m_mesh, face);
				NewtonMeshGetFaceIndices (m_mesh, face, indices);

				int index = indices[0] * stride;
				dVector point0 (vertexArray[index], vertexArray[index + 1], vertexArray[index + 2], 0.0f);

				index = indices[1] * stride;
				dVector point1 (vertexArray[index], vertexArray[index + 1], vertexArray[index + 2], 0.0f);

				for (int i = 2; i < vertexCount; i ++) {
					int index = indices[i] * stride;
					dVector point2 (vertexArray[index], vertexArray[index + 1], vertexArray[index + 2], 0.0f);

					glVertex3f(point0.m_x, point0.m_y, point0.m_z);
					glVertex3f(point1.m_x, point1.m_y, point1.m_z);
					glVertex3f(point2.m_x, point2.m_y, point2.m_z);
					point1 = point2;
				}
			}
		}

		glEnd();

		glEndList();
	}



	glPushMatrix();
	glMultMatrix(&m_matrix[0][0]);

	glCallList(m_solidDipalyList);

	glPopMatrix();

}


// draw scene in Gouraud shaded normal textured mode 
void alchemediaStaticMesh::DrawGouraudShaded(dScene* const world, dScene::dTreeNode* const myNode, const dVector& color) const
{
	_ASSERTE (world->GetInfoFromNode(myNode) == this);
	if (!m_gouraudDipalyList) {

		m_gouraudDipalyList = glGenLists(1);

		glNewList(m_gouraudDipalyList, GL_COMPILE);

		int stride = NewtonMeshGetPointStrideInByte(m_mesh) / sizeof (dFloat);
		float* const vertexArray = NewtonMeshGetPointArray(m_mesh);
		float* const normalArray = NewtonMeshGetNormalArray(m_mesh);
		float* const uv0Array = NewtonMeshGetUV0Array(m_mesh);

		for (void* ptr = world->GetFirstChild(myNode); ptr; ptr = world->GetNextChild(myNode, ptr)) {
			dScene::dTreeNode* const materialNode = world->GetNodeFromLink (ptr);
			dNodeInfo* const info = world->GetInfoFromNode(materialNode);
			if (info->IsType(dMaterialNodeInfo::GetRttiType())) {
				dMaterialNodeInfo* const material = (dMaterialNodeInfo*) info;

				glMaterialfv(GL_FRONT, GL_SPECULAR, &material->GetSpecularColor().m_x);
				glMaterialfv(GL_FRONT, GL_AMBIENT, &material->GetAmbientColor().m_x);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, &material->GetDiffuseColor().m_x);
				glMaterialfv(GL_FRONT, GL_EMISSION, &material->GetEmissiveColor().m_x);
				glMaterialf(GL_FRONT, GL_SHININESS, material->GetShininess());

				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

				dTextureNodeInfo* diffuseTex = NULL;
				dScene::dTreeNode* diffTexNode = world->FindTextureByTextId (materialNode, material->GetDiffuseTextId());
				if (diffTexNode){
					diffuseTex = (dTextureNodeInfo*) world->GetInfoFromNode(diffTexNode);
				}

				if (diffuseTex && (diffuseTex->GetInternalId() != -1)) {
					glEnable(GL_TEXTURE_2D);		
					glBindTexture(GL_TEXTURE_2D, GLuint (diffuseTex->GetInternalId()));
				} else {
					glDisable(GL_TEXTURE_2D);
				}

				glBegin(GL_TRIANGLES);
				int id = material->GetId();



				for (void* face = NewtonMeshGetFirstFace (m_mesh); face; face = NewtonMeshGetNextFace (m_mesh, face)) {

					if (!NewtonMeshIsFaceOpen (m_mesh, face)) {
						int materialID = NewtonMeshGetFaceMaterial (m_mesh, face);	

						if (materialID == id) {
							int vertexCount = NewtonMeshGetFaceIndexCount (m_mesh, face);
							int indices[1024];
							NewtonMeshGetFacePointIndices (m_mesh, face, indices);

							int index = indices[0] * stride;
							dVector uv0 (uv0Array[index], uv0Array[index + 1], 0.0f, 0.0f);
							dVector normal0 (normalArray[index], normalArray[index + 1], normalArray[index + 2], 0.0f);
							dVector point0 (vertexArray[index], vertexArray[index + 1], vertexArray[index + 2], 0.0f);

							index = indices[1] * stride;
							dVector uv1 (uv0Array[index], uv0Array[index + 1], 0.0f, 0.0f);
							dVector normal1 (normalArray[index], normalArray[index + 1], normalArray[index + 2], 0.0f);
							dVector point1 (vertexArray[index], vertexArray[index + 1], vertexArray[index + 2], 0.0f);

							for (int i = 2; i < vertexCount; i ++) {

								int index = indices[i] * stride;
								dVector uv2 (uv0Array[index], uv0Array[index + 1], 0.0f, 0.0f);
								dVector normal2 (normalArray[index], normalArray[index + 1], normalArray[index + 2], 0.0f);
								dVector point2 (vertexArray[index], vertexArray[index + 1], vertexArray[index + 2], 0.0f);

								glTexCoord2f(uv0.m_x, uv0.m_y); 
								glNormal3f (normal0.m_x, normal0.m_y, normal0.m_z); 
								glVertex3f(point0.m_x, point0.m_y, point0.m_z);

								glTexCoord2f(uv1.m_x, uv1.m_y); 
								glNormal3f (normal1.m_x, normal1.m_y, normal1.m_z); 
								glVertex3f(point1.m_x, point1.m_y, point1.m_z);

								glTexCoord2f(uv2.m_x, uv2.m_y); 
								glNormal3f (normal2.m_x, normal2.m_y, normal2.m_z); 
								glVertex3f(point2.m_x, point2.m_y, point2.m_z);
								uv1 = uv2;
								normal1 = normal2;
								point1 = point2;
							}
						}
					}
				}
				glEnd();
			}
		}

		glEndList();
	}

//glEnable(GL_POLYGON_OFFSET_FILL);
//glPolygonOffset (1.0f, 1.0f);


	glPushMatrix();
	glMultMatrix(&m_matrix[0][0]);

	glCallList(m_gouraudDipalyList);

	glPopMatrix();

//glDisable(GL_POLYGON_OFFSET_FILL);
}



