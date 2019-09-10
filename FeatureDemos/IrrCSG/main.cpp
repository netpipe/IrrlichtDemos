#include <irrlicht.h>
#include "driverChoice.h"
#include "CSG/primitives.h"
#include "CSG/CSG.h"

using namespace irr;
using namespace scene;
using namespace core;
using namespace video;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

meshMesh mesh;

SMesh* m_pMesh = NULL;


int initializeMesh(meshMesh& mesh){
	meshMesh meshA;
	meshMesh meshB;
	meshMesh meshC;
	meshMesh meshD;
	if (meshInitializeBox(&meshB, -0.5, 0.5, -0.5, 0.5, -0.5, 0.5) != 0 || meshInitializeCapsule(&meshA, 0.5, 2.0, 16, 32)
		|| 	meshInitializeBox(&meshC, -0.3, 0.3, -0.3, 0.3, -0.3, 0.3) || meshInitializeSphere(&meshD, 0.3, 16, 32)) {
	 	return 1;
	}
	GLdouble translation[3] = {0.5, 0.5, 0.5};
	GLdouble translation1[3] = {0.2, 0.2, 0.3};
	meshTranslate(&meshA, translation);
	meshTranslate(&meshD, translation1);

	// boolean operations: ((capsule intersect box) union another box) subtract sphere
	CSGIntersection(&meshA, &meshB, &mesh);
	meshDestroy(&meshA);
	meshDestroy(&meshB);
	CSGUnion(&mesh, &meshC, &mesh);
	meshDestroy(&meshC);
	CSGSubtraction(&mesh, &meshD, &mesh);
	meshDestroy(&meshD);
	return 0;
}



meshMesh CreateCSGMesh(SMesh* pMesh){
	int meshBufCount = pMesh->getMeshBufferCount();
	int totalVertexCount = 0;
	int totalIndexCount = 0;
	for(int i=0; i<meshBufCount; i++){
		IMeshBuffer* meshBuffer = pMesh->getMeshBuffer(i);
		totalVertexCount += meshBuffer->getIndexCount();
		totalIndexCount += meshBuffer->getVertexCount();
	}
	meshMesh returnMesh;
	meshInitialize(&returnMesh, totalIndexCount/3, totalVertexCount, 3);
	int triIndex = 0;
	int vIndex = 0;

	for(int i=0; i<meshBufCount; i++){
		IMeshBuffer* pMeshBuffer = pMesh->getMeshBuffer(i);
		irr::u16* indices = pMeshBuffer->getIndices();
		void* vertices = pMeshBuffer->getVertices();
		irr::u32 indexCount = pMeshBuffer->getIndexCount();
		irr::u32 vertexCount = pMeshBuffer->getVertexCount();

		memcpy(returnMesh.tri + triIndex, indices, indexCount);
		triIndex += indexCount;

		memcpy(returnMesh.vert + vIndex, vertices, vertexCount);
		vIndex += vertexCount;
	}
	return returnMesh;
}



// 		driver->drawVertexPrimitiveList(&vertices[0], mesh.vertNum, mesh.tri, mesh.triNum, video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_32BIT);

//SMesh* CreateIrrMesh(const meshMesh& mesh){
//m_pMesh = new SMesh();
//
//GLuint bufCount = ceil((double)mesh.triNum/5000.0);
//
//for(GLuint bufIndex = 1; bufIndex < bufCount; bufIndex++){
//GLuint triStart = bufIndex * 5000;
//GLuint triEnd = (bufIndex + 1) * 5000;
//if(triEnd > mesh.triNum){
//triEnd = mesh.triNum;
//}
//GLuint triCount = triEnd - triStart;
//
//GLuint vertStart = triStart * 3;
//GLuint vertEnd = triEnd * 3;
//GLuint vertCount = triCount * 3;
//
//SMeshBuffer* buf = new SMeshBuffer();
//m_pMesh->addMeshBuffer(buf);
//
//buf->Vertices.set_used(vertCount);
//GLdouble* meshVertices = mesh.vert;
//for(int i=0; i<vertCount; i++){
//S3DVertex& v = buf->Vertices[i];
//v.Pos.set(meshVertices[(i + vertStart)*mesh.attrDim + 0], meshVertices[(i + vertStart)*mesh.attrDim + 1], meshVertices[(i + vertStart)*mesh.attrDim + 2]);
//v.Color.set(255, 0, 0, 255);
//}
//
//buf->Indices.set_used(3 * triCount);
//GLuint* triIndices = mesh.tri;
//for(int i=0; i<triCount; i++){
//buf->Indices[i*3 + 0] = i*3 + 0;//triIndices[(i + triStart) *3 + 0];
//buf->Indices[i*3 + 1] = i*3 + 1;//triIndices[(i + triStart) *3 + 1];
//buf->Indices[i*3 + 2] = i*3 + 2;//triIndices[(i + triStart) *3 + 2];
//}
//buf->recalculateBoundingBox();
//}
//return m_pMesh;
//}

SMesh* CreateIrrMesh(meshMesh& mesh){
	SMesh* pMesh = new SMesh();

	GLuint bufCount = ceil((double)mesh.triNum/5000.0);

	for(GLuint bufIndex = 1; bufIndex < bufCount; bufIndex++){
		GLuint triStart = bufIndex * 5000;
		GLuint triEnd = (bufIndex + 1) * 5000;
		if(triEnd > mesh.triNum){
			triEnd = mesh.triNum;
		}
		GLuint triCount = triEnd - triStart;

		GLuint vertStart = triStart * 3;
		GLuint vertEnd = triEnd * 3;
		GLuint vertCount = triCount * 3;

		SMeshBuffer* buf = new SMeshBuffer();
		pMesh->addMeshBuffer(buf);

		buf->Vertices.set_used(vertCount);
		GLdouble* meshVertices = mesh.vert;
		for(int i=0; i<vertCount; i++){
			S3DVertex& v = buf->Vertices[i];
			v.Pos.set(meshVertices[(i + vertStart)*mesh.attrDim + 0], meshVertices[(i + vertStart)*mesh.attrDim + 1], meshVertices[(i + vertStart)*mesh.attrDim + 2]);
			v.Color.set(255, 0, 0, 255);
		}

		buf->Indices.set_used(3 * triCount);
		GLuint* triIndices = mesh.tri;
		for(int i=0; i<triCount; i++){
			buf->Indices[i*3 + 0] = i*3 + 0;//triIndices[(i + triStart) *3 + 0];
			buf->Indices[i*3 + 1] = i*3 + 1;//triIndices[(i + triStart) *3 + 1];
			buf->Indices[i*3 + 2] = i*3 + 2;//triIndices[(i + triStart) *3 + 2];
		}
		buf->recalculateBoundingBox();
	}
	return pMesh;
}

int main(){
	video::E_DRIVER_TYPE driverType = driverChoiceConsole();
	if(driverType == video::EDT_COUNT){
		return 1;
	}

	IrrlichtDevice* device = createDevice(driverType, core::dimension2d<u32>(640, 480), 32, false);
	if(device == 0){
		return 1;
	}
	device->setWindowCaption(L"Custom Scene Node - Isslicht Engine Demo");
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	meshMesh mesh;
	initializeMesh(mesh);
	//meshInitializeBox(&mesh, -0.5, 50.5, -50.5, 50.5, -0.5, 0.5);
	SMesh* pMesh = CreateIrrMesh(mesh);

	meshMesh csgMesh = CreateCSGMesh(pMesh);

	SMesh* pIrrMesg = CreateIrrMesh(csgMesh);

	IMeshSceneNode* meshnode = smgr -> addMeshSceneNode(pMesh);
	meshnode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);


	ICameraSceneNode* camera = smgr->addCameraSceneNodeMaya();
	if(camera != NULL){
		camera->setTarget(vector3df(0, 0, 0));
		camera->setPosition(vector3df(5, 0, 0));
	}//(NULL, vector3df(5, 0, 0), vector3df(0, 0, 0));
	//scene::ISceneNodeAnimator* anim = smgr->createRotationAnimator(core::vector3df(0.8f, 0, 0.8f));
	//if(anim){
		//myNode->addAnimator(anim);
		//anim->drop();
		//anim = 0;
//	}
	//myNode->drop();
	//myNode = 0;

	u32 frames = 0;
	while(device->run()){
		driver->beginScene(true, true, video::SColor(0, 100, 100, 100));
		smgr->drawAll();
		driver->endScene();

		if(++frames == 100){
			core::stringw str =L"Irrlicht Engine [";
			str += driver->getName();
			str += L"] FPS: ";

			str += (s32)driver->getFPS();
			device->setWindowCaption(str.c_str());
			frames = 0;
		}
	}
	device->drop();
	return 0;
}
