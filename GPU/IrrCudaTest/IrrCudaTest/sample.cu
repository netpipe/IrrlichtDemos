/********************************************************************
*  sample.cu
*  This is a example of the CUDA program.
*********************************************************************/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <cutil_inline.h>
#include <iostream>
#include <map>
#include <vector>
#include <list>

#include <irrlicht.h>
#pragma comment (lib, "Irrlicht.lib")

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

struct MeshData
{
	// Irrlicht�p
	int			indexCount;
	int			irr_vertexCount;
	u16         *irr_indices;
	S3DVertex   *irr_vertices;
	// CUDA�p
	float		*cuda_vertices;
	int			cuda_vertexCount;
	dim3		dimGrid;
	float		*device_result;
	// Irrlicht��CUDA�̊֘A�t��
	std::map<int, int> irr_cuda_map;
};

/************************************************************************/
/* Init CUDA                                                            */
/************************************************************************/
#if __DEVICE_EMULATION__

bool InitCUDA(void){return true;}

#else
bool InitCUDA(void)
{
	int count = 0;
	int i = 0;

	cudaGetDeviceCount(&count);
	if(count == 0) {
		fprintf(stderr, "There is no device.\n");
		return false;
	}

	for(i = 0; i < count; i++) {
		cudaDeviceProp prop;
		if(cudaGetDeviceProperties(&prop, i) == cudaSuccess) {
			if(prop.major >= 1) {
				break;
			}
		}
	}
	if(i == count) {
		fprintf(stderr, "There is no device supporting CUDA.\n");
		return false;
	}
	cudaSetDevice(i);

	printf("CUDA initialized.\n");
	return true;
}

#endif
/************************************************************************/
/* Example                                                              */
/************************************************************************/
__global__ static void HelloCUDA(float* vertex, int vertexCount, float(dt))
{
	int index = blockIdx.x * 256 + threadIdx.y * 16 + threadIdx.x;

	if (index >= vertexCount) 
	{ 
		return;
	}

	// �K���ɒ��_�A�j���[�V����
	vertex[3*index+0] += 0.002f*cos(2.0f*dt+0.1f*(float)threadIdx.x);
	vertex[3*index+1] += 0.001f*sin(dt);
	vertex[3*index+2] += 0.002f*cos(3.0f*dt+0.1f*(float)threadIdx.y);
}


/************************************************************************/
/* Irrlicht                                                             */
/************************************************************************/

/************************************************************************/
/* HelloCUDA                                                            */
/************************************************************************/
int main(int argc, char* argv[])
{
	// �R�������Ă��Ȃ��ƃt�@�C�������E�E�E
	std::locale::global(std::locale("japanese"));

	// CUDA�̏�����
	if(!InitCUDA()) {
		return 0;
	}

	// window size
	const core::dimension2d<u32>& windowSize = (core::dimension2d<u32>(640, 480));

	// �f�o�C�X�̍쐬
	IrrlichtDevice *device = createDevice( EDT_OPENGL, windowSize, 32,
		false, true, false, 0);

	if (!device)
	{
		return false;
	}
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager *smgr = device->getSceneManager();

	// �J�����̐ݒ�
	ICameraSceneNode *camera = smgr->addCameraSceneNodeMaya(0);

	// ���b�V���f�[�^���t�@�C������ǂݍ��݁iIrrlicht�p�j
	IAnimatedMesh *cubeMesh = smgr->getMesh("../../media/moko_one.3ds");
	IAnimatedMeshSceneNode *cubeNode = smgr->addAnimatedMeshSceneNode(cubeMesh, 0, -1, vector3df(0, 80, 0), vector3df(-90,0,0), vector3df(1,1,1), false);
	cubeNode->setMaterialFlag(video::EMF_LIGHTING, false);
	cubeNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);

	// �n�ʂ̍쐬�iIrrlicht�p�j
	IAnimatedMesh *planemesh = smgr->addHillPlaneMesh("myHill", dimension2d<f32>(24, 24), dimension2d<u32>(100, 100));
	ISceneNode *q3sn = smgr->addOctTreeSceneNode(planemesh);
	q3sn->setMaterialFlag(video::EMF_LIGHTING, false);
	q3sn->setMaterialTexture(0, driver->getTexture("../../media/wall.jpg"));

	// create skybox
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	smgr->addSkyBoxSceneNode(
		driver->getTexture("../../media/irrlicht2_up.jpg"),
		driver->getTexture("../../media/irrlicht2_dn.jpg"),
		driver->getTexture("../../media/irrlicht2_lf.jpg"),
		driver->getTexture("../../media/irrlicht2_rt.jpg"),
		driver->getTexture("../../media/irrlicht2_ft.jpg"),
		driver->getTexture("../../media/irrlicht2_bk.jpg"));

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

	int cMeshBuffer, cMeshBufferCount, j;
	// MeshBuffer
	IMeshBuffer **mb;
	// MeshData struct
	MeshData *testMesh;

	std::cout << "get Mesh Buffer : ";
	cMeshBufferCount = cubeMesh->getMesh(0)->getMeshBufferCount();
	std::cout << cMeshBufferCount << std::endl;
	mb = new IMeshBuffer*[cMeshBufferCount];
	testMesh = new MeshData[cMeshBufferCount];

	// getMesh�Ŏ擾�������b�V�����̎擾
	for (cMeshBuffer=0; cMeshBuffer<cMeshBufferCount; cMeshBuffer++) 
	{
		// ���b�V���o�b�t�@�̎擾
		mb[cMeshBuffer] = cubeMesh->getMesh(0)->getMeshBuffer(cMeshBuffer); 

		// ���_���̃|�C���^���擾�B���̒l������������Β��_���W��ύX�ł���B
		testMesh[cMeshBuffer].irr_vertices = (irr::video::S3DVertex*)mb[cMeshBuffer]->getVertices();
		testMesh[cMeshBuffer].irr_vertexCount = mb[cMeshBuffer]->getVertexCount();
		// �ʂ̃C���f�b�N�X�̃|�C���^���擾�B���̒l������������Ζʂ̃C���f�b�N�X��ύX�ł���B
		testMesh[cMeshBuffer].irr_indices  = mb[cMeshBuffer]->getIndices(); 
		testMesh[cMeshBuffer].indexCount = mb[cMeshBuffer]->getIndexCount();
		// ���̑�������
		testMesh[cMeshBuffer].irr_cuda_map.clear();
		testMesh[cMeshBuffer].cuda_vertices = NULL;
		testMesh[cMeshBuffer].device_result = NULL;
		std::cout << mb[cMeshBuffer]->getVertexCount() << ":" << mb[cMeshBuffer]->getIndexCount() << std::endl;
	} 

	// �ǂݍ��񂾃��b�V���f�[�^�̍œK��
	// 3ds�t�@�C���i.obj���H�j��Irrlicht�œǂݍ��ނƁA�iIndex����Vertex���j�ƂȂ�AIndex����ӂŁAVertex���d������B
	// �������_�����x��CUDA�Ōv�Z�������Ȃ��̂ŁAVertex�̏d�����Ȃ����B
	// ���̂��߂ɂ܂��A�d���̂Ȃ�Index�Əd���̂���Index�̃}�b�v���쐬
	std::map<int, int> index_map;
	std::map<int, int> index2_map;
	std::map<int, S3DVertex> vertex_map;
	int count = 0;

	for (cMeshBuffer = 0; cMeshBuffer < cMeshBufferCount; cMeshBuffer++)
	{
		// Vertex����Index���ł͂Ȃ��ꍇ�͏������Ȃ�
		if (mb[cMeshBuffer]->getVertexCount() != mb[cMeshBuffer]->getIndexCount())
		{
			continue;
		}
		count = 0;
		index_map.clear();
		index2_map.clear();
		vertex_map.clear();
		// �i�d�����Ă���jVertex�̐���Index�̐������J��Ԃ�
		for (int i=0; i<testMesh[cMeshBuffer].irr_vertexCount; i++)
		{
			// i�Ԗڂ̃C���f�b�N�X�ƒ��_���W���擾
			int iIndex = testMesh[cMeshBuffer].irr_indices[i];
			vector3df iVertex = testMesh[cMeshBuffer].irr_vertices[iIndex].Pos;
			// �d���������_���ǂ����̊m�F
			bool isFirst = true;
			// 0����i-1�Ԗڂ̃C���f�b�N�X�̒��_���W�Ɣ�r
			for (int j=0; j<i; j++)
			{
				// j�Ԗڂ̃C���f�b�N�X�ƒ��_���W���擾
				int jIndex = testMesh[cMeshBuffer].irr_indices[j];
				vector3df jVertex = testMesh[cMeshBuffer].irr_vertices[jIndex].Pos;
				// �����C���f�b�N�X�ԍ��̏ꍇ
				if (iVertex == jVertex)
				{
					// i�Ԗڂ̃C���f�b�N�X�́Aj�Ԗڂ̃C���f�b�N�X�Ɠ����Ƃ�����w���Ă���
					index_map.insert(std::make_pair(i, j));
					// Irrlicht->CUDA��Index�̃}�b�s���O
					int cuda_index = index2_map.find(j)->second;
					testMesh[cMeshBuffer].irr_cuda_map.insert(std::make_pair(i, cuda_index));
					isFirst = false;
					break;
				}
			}
			// �d�����钸�_���Ȃ��ꍇ��Index�ƒ��_���֘A�t��
			if (isFirst)
			{
				// Irrlicht��Index���d���̂���Index
				index_map.insert(std::make_pair(i, i));
				// �ƂтƂт�Index���A����Index
				index2_map.insert(std::make_pair(i, count));
				// Irrlicht->CUDA��Index�̃}�b�s���O
				testMesh[cMeshBuffer].irr_cuda_map.insert(std::make_pair(i, count));
				// �A��Index�ƒ��_���̃}�b�v
				vertex_map.insert(std::make_pair(count, testMesh[cMeshBuffer].irr_vertices[iIndex]));
				count++;
			}
		}

		// index-vertex data for cuda
		testMesh[cMeshBuffer].cuda_vertexCount = vertex_map.size();
		// CUDA���ŃO���[�o�����������m��
		cutilSafeCall( cudaMalloc((void**) &testMesh[cMeshBuffer].device_result, 
			sizeof(float) * testMesh[cMeshBuffer].cuda_vertexCount*3));
		// CUDA�ɓn���̂�Host�Ƃ��Ċm��
		cutilSafeCall( cudaMallocHost((void**) &testMesh[cMeshBuffer].cuda_vertices, 
			sizeof(float) * testMesh[cMeshBuffer].cuda_vertexCount*3));

		// ���b�V������|���S�������擾���z��Ɋi�[
		for (j=0; j<testMesh[cMeshBuffer].cuda_vertexCount; j++)
		{
			testMesh[cMeshBuffer].cuda_vertices[3*j]   =  vertex_map[j].Pos.X;
			testMesh[cMeshBuffer].cuda_vertices[3*j+1] =  vertex_map[j].Pos.Y;
			testMesh[cMeshBuffer].cuda_vertices[3*j+2] =  vertex_map[j].Pos.Z;
		}

		// �������_�f�[�^��]��
		cutilSafeCall( cudaMemcpy(testMesh[cMeshBuffer].device_result, testMesh[cMeshBuffer].cuda_vertices, 
			sizeof(float) * testMesh[cMeshBuffer].cuda_vertexCount*3, cudaMemcpyHostToDevice));
	}

	// CUDA�Ŋe���_���v�Z���邽�߂Ƀu���b�N��������
	int lastVertexCount = 0, blockNum = 0;
	// 1�u���b�N��16x16��256�X���b�h�ɌŒ�
	dim3 dimBlock(16, 16, 1);

	for (cMeshBuffer=0; cMeshBuffer<cMeshBufferCount; cMeshBuffer++) 
	{
		// ��ԍŌ�̃u���b�N���������钸�_��
		lastVertexCount = testMesh[cMeshBuffer].cuda_vertexCount%256;
		// 0�̏ꍇ�A����؂��̂Ńu���b�N���͒��_��/256
		if (lastVertexCount == 0)
		{
			blockNum = testMesh[cMeshBuffer].cuda_vertexCount/256;
		}
		// 0���傫���ꍇ�A����؂�Ȃ��̂Ŏc�������_����������u���b�N���K�v
		else
		{
			blockNum = testMesh[cMeshBuffer].cuda_vertexCount/256+1;
		}
		std::cout << "blockNum=" << blockNum << std::endl;
		// 1�O���b�h��blockNum�u���b�N�Ɍ���
		testMesh[cMeshBuffer].dimGrid = dim3(blockNum, 1, 1);
	}

	// FPS�v�Z�p
	wchar_t tmp[32] = L"";
	int fps, lastFPS  = -1;

	// ���_�A�j���[�V�����p
	static float dt = 0.0f;

	while(device->run())
	{
		for (cMeshBuffer=0; cMeshBuffer<cMeshBufferCount; cMeshBuffer++) 
		{
			// CUDA�Œ��_�A�j���[�V����
			HelloCUDA<<<testMesh[cMeshBuffer].dimGrid, dimBlock>>>(testMesh[cMeshBuffer].device_result, testMesh[cMeshBuffer].cuda_vertexCount, dt);
			cutilCheckMsg("Kernel execution failed\n");

			// �v�Z���ʂ̓]��
			cutilSafeCall( cudaMemcpy(testMesh[cMeshBuffer].cuda_vertices, testMesh[cMeshBuffer].device_result, 
				sizeof(float) * testMesh[cMeshBuffer].cuda_vertexCount * 3, cudaMemcpyDeviceToHost));

			// �v�Z���ʂ��璸�_���W���X�V
			for (int i=0; i<testMesh[cMeshBuffer].indexCount; i++)
			{
				// Irrlict�̒��_�ԍ���CUDA�̒��_�ԍ�
				int irr_index = testMesh[cMeshBuffer].irr_indices[i];
				int cuda_index = testMesh[cMeshBuffer].irr_cuda_map.find(i)->second;
				// Irrlicht����mb_vertices[i]��OK
				testMesh[cMeshBuffer].irr_vertices[irr_index].Pos.X = testMesh[cMeshBuffer].cuda_vertices[3*cuda_index+0];
				testMesh[cMeshBuffer].irr_vertices[irr_index].Pos.Y = testMesh[cMeshBuffer].cuda_vertices[3*cuda_index+1];
				testMesh[cMeshBuffer].irr_vertices[irr_index].Pos.Z = testMesh[cMeshBuffer].cuda_vertices[3*cuda_index+2];
			}
		} 
		dt += 0.001f;
		if (GetAsyncKeyState(VK_SPACE))
		{

		}
		else if (GetAsyncKeyState(VK_ESCAPE)) 
		{
			break;
		}

		// �`�揈��
		driver->beginScene(true, true, SColor(0,200,200,200));
		smgr->drawAll();
		driver->endScene();

		//FPS�擾/�\��
		fps = driver->getFPS();
		if(lastFPS != fps)
		{
			swprintf(tmp, 32, L"Irrlicht fps:%d", fps);
			device->setWindowCaption(tmp);
			lastFPS = fps;
		}
	}
	// Irrlicht�̏I��
	device->drop();

	// �������̊J��
	for (cMeshBuffer=0; cMeshBuffer<cMeshBufferCount; cMeshBuffer++)
	{
		if (testMesh[cMeshBuffer].cuda_vertices != NULL)
		{
			cutilSafeCall( cudaFreeHost(testMesh[cMeshBuffer].cuda_vertices) );
		}
		if (testMesh[cMeshBuffer].device_result != NULL)
		{
			cutilSafeCall( cudaFree(testMesh[cMeshBuffer].device_result));
		}
	}
	delete [] testMesh;
	delete [] mb;

	return 0;
}
