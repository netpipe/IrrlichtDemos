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
	// Irrlicht用
	int			indexCount;
	int			irr_vertexCount;
	u16         *irr_indices;
	S3DVertex   *irr_vertices;
	// CUDA用
	float		*cuda_vertices;
	int			cuda_vertexCount;
	dim3		dimGrid;
	float		*device_result;
	// IrrlichtとCUDAの関連付け
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

	// 適当に頂点アニメーション
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
	// コレをしていないとファイル名が・・・
	std::locale::global(std::locale("japanese"));

	// CUDAの初期化
	if(!InitCUDA()) {
		return 0;
	}

	// window size
	const core::dimension2d<u32>& windowSize = (core::dimension2d<u32>(640, 480));

	// デバイスの作成
	IrrlichtDevice *device = createDevice( EDT_OPENGL, windowSize, 32,
		false, true, false, 0);

	if (!device)
	{
		return false;
	}
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager *smgr = device->getSceneManager();

	// カメラの設定
	ICameraSceneNode *camera = smgr->addCameraSceneNodeMaya(0);

	// メッシュデータをファイルから読み込み（Irrlicht用）
	IAnimatedMesh *cubeMesh = smgr->getMesh("../../media/moko_one.3ds");
	IAnimatedMeshSceneNode *cubeNode = smgr->addAnimatedMeshSceneNode(cubeMesh, 0, -1, vector3df(0, 80, 0), vector3df(-90,0,0), vector3df(1,1,1), false);
	cubeNode->setMaterialFlag(video::EMF_LIGHTING, false);
	cubeNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);

	// 地面の作成（Irrlicht用）
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

	// getMeshで取得したメッシュ情報の取得
	for (cMeshBuffer=0; cMeshBuffer<cMeshBufferCount; cMeshBuffer++) 
	{
		// メッシュバッファの取得
		mb[cMeshBuffer] = cubeMesh->getMesh(0)->getMeshBuffer(cMeshBuffer); 

		// 頂点情報のポインタを取得。この値を書き換えれば頂点座標を変更できる。
		testMesh[cMeshBuffer].irr_vertices = (irr::video::S3DVertex*)mb[cMeshBuffer]->getVertices();
		testMesh[cMeshBuffer].irr_vertexCount = mb[cMeshBuffer]->getVertexCount();
		// 面のインデックスのポインタを取得。この値を書き換えれば面のインデックスを変更できる。
		testMesh[cMeshBuffer].irr_indices  = mb[cMeshBuffer]->getIndices(); 
		testMesh[cMeshBuffer].indexCount = mb[cMeshBuffer]->getIndexCount();
		// その他初期化
		testMesh[cMeshBuffer].irr_cuda_map.clear();
		testMesh[cMeshBuffer].cuda_vertices = NULL;
		testMesh[cMeshBuffer].device_result = NULL;
		std::cout << mb[cMeshBuffer]->getVertexCount() << ":" << mb[cMeshBuffer]->getIndexCount() << std::endl;
	} 

	// 読み込んだメッシュデータの最適化
	// 3dsファイル（.objも？）をIrrlichtで読み込むと、（Index数＝Vertex数）となり、Indexが一意で、Vertexが重複する。
	// 同じ頂点を何度もCUDAで計算したくないので、Vertexの重複をなくす。
	// そのためにまず、重複のないIndexと重複のあるIndexのマップを作成
	std::map<int, int> index_map;
	std::map<int, int> index2_map;
	std::map<int, S3DVertex> vertex_map;
	int count = 0;

	for (cMeshBuffer = 0; cMeshBuffer < cMeshBufferCount; cMeshBuffer++)
	{
		// Vertex数＝Index数ではない場合は処理しない
		if (mb[cMeshBuffer]->getVertexCount() != mb[cMeshBuffer]->getIndexCount())
		{
			continue;
		}
		count = 0;
		index_map.clear();
		index2_map.clear();
		vertex_map.clear();
		// （重複している）Vertexの数＝Indexの数だけ繰り返す
		for (int i=0; i<testMesh[cMeshBuffer].irr_vertexCount; i++)
		{
			// i番目のインデックスと頂点座標を取得
			int iIndex = testMesh[cMeshBuffer].irr_indices[i];
			vector3df iVertex = testMesh[cMeshBuffer].irr_vertices[iIndex].Pos;
			// 重複した頂点かどうかの確認
			bool isFirst = true;
			// 0からi-1番目のインデックスの頂点座標と比較
			for (int j=0; j<i; j++)
			{
				// j番目のインデックスと頂点座標を取得
				int jIndex = testMesh[cMeshBuffer].irr_indices[j];
				vector3df jVertex = testMesh[cMeshBuffer].irr_vertices[jIndex].Pos;
				// 同じインデックス番号の場合
				if (iVertex == jVertex)
				{
					// i番目のインデックスは、j番目のインデックスと同じところを指している
					index_map.insert(std::make_pair(i, j));
					// Irrlicht->CUDAのIndexのマッピング
					int cuda_index = index2_map.find(j)->second;
					testMesh[cMeshBuffer].irr_cuda_map.insert(std::make_pair(i, cuda_index));
					isFirst = false;
					break;
				}
			}
			// 重複する頂点がない場合はIndexと頂点を関連付け
			if (isFirst)
			{
				// IrrlichtのIndex→重複のあるIndex
				index_map.insert(std::make_pair(i, i));
				// とびとびのIndex→連続のIndex
				index2_map.insert(std::make_pair(i, count));
				// Irrlicht->CUDAのIndexのマッピング
				testMesh[cMeshBuffer].irr_cuda_map.insert(std::make_pair(i, count));
				// 連続Indexと頂点情報のマップ
				vertex_map.insert(std::make_pair(count, testMesh[cMeshBuffer].irr_vertices[iIndex]));
				count++;
			}
		}

		// index-vertex data for cuda
		testMesh[cMeshBuffer].cuda_vertexCount = vertex_map.size();
		// CUDA側でグローバルメモリを確保
		cutilSafeCall( cudaMalloc((void**) &testMesh[cMeshBuffer].device_result, 
			sizeof(float) * testMesh[cMeshBuffer].cuda_vertexCount*3));
		// CUDAに渡すのでHostとして確保
		cutilSafeCall( cudaMallocHost((void**) &testMesh[cMeshBuffer].cuda_vertices, 
			sizeof(float) * testMesh[cMeshBuffer].cuda_vertexCount*3));

		// メッシュからポリゴン情報を取得し配列に格納
		for (j=0; j<testMesh[cMeshBuffer].cuda_vertexCount; j++)
		{
			testMesh[cMeshBuffer].cuda_vertices[3*j]   =  vertex_map[j].Pos.X;
			testMesh[cMeshBuffer].cuda_vertices[3*j+1] =  vertex_map[j].Pos.Y;
			testMesh[cMeshBuffer].cuda_vertices[3*j+2] =  vertex_map[j].Pos.Z;
		}

		// 初期頂点データを転送
		cutilSafeCall( cudaMemcpy(testMesh[cMeshBuffer].device_result, testMesh[cMeshBuffer].cuda_vertices, 
			sizeof(float) * testMesh[cMeshBuffer].cuda_vertexCount*3, cudaMemcpyHostToDevice));
	}

	// CUDAで各頂点を計算するためにブロック数を決定
	int lastVertexCount = 0, blockNum = 0;
	// 1ブロックは16x16の256スレッドに固定
	dim3 dimBlock(16, 16, 1);

	for (cMeshBuffer=0; cMeshBuffer<cMeshBufferCount; cMeshBuffer++) 
	{
		// 一番最後のブロックが処理する頂点数
		lastVertexCount = testMesh[cMeshBuffer].cuda_vertexCount%256;
		// 0の場合、割り切れるのでブロック数は頂点数/256
		if (lastVertexCount == 0)
		{
			blockNum = testMesh[cMeshBuffer].cuda_vertexCount/256;
		}
		// 0より大きい場合、割り切れないので残った頂点を処理するブロックが必要
		else
		{
			blockNum = testMesh[cMeshBuffer].cuda_vertexCount/256+1;
		}
		std::cout << "blockNum=" << blockNum << std::endl;
		// 1グリッドをblockNumブロックに決定
		testMesh[cMeshBuffer].dimGrid = dim3(blockNum, 1, 1);
	}

	// FPS計算用
	wchar_t tmp[32] = L"";
	int fps, lastFPS  = -1;

	// 頂点アニメーション用
	static float dt = 0.0f;

	while(device->run())
	{
		for (cMeshBuffer=0; cMeshBuffer<cMeshBufferCount; cMeshBuffer++) 
		{
			// CUDAで頂点アニメーション
			HelloCUDA<<<testMesh[cMeshBuffer].dimGrid, dimBlock>>>(testMesh[cMeshBuffer].device_result, testMesh[cMeshBuffer].cuda_vertexCount, dt);
			cutilCheckMsg("Kernel execution failed\n");

			// 計算結果の転送
			cutilSafeCall( cudaMemcpy(testMesh[cMeshBuffer].cuda_vertices, testMesh[cMeshBuffer].device_result, 
				sizeof(float) * testMesh[cMeshBuffer].cuda_vertexCount * 3, cudaMemcpyDeviceToHost));

			// 計算結果から頂点座標を更新
			for (int i=0; i<testMesh[cMeshBuffer].indexCount; i++)
			{
				// Irrlictの頂点番号→CUDAの頂点番号
				int irr_index = testMesh[cMeshBuffer].irr_indices[i];
				int cuda_index = testMesh[cMeshBuffer].irr_cuda_map.find(i)->second;
				// Irrlicht側はmb_vertices[i]でOK
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

		// 描画処理
		driver->beginScene(true, true, SColor(0,200,200,200));
		smgr->drawAll();
		driver->endScene();

		//FPS取得/表示
		fps = driver->getFPS();
		if(lastFPS != fps)
		{
			swprintf(tmp, 32, L"Irrlicht fps:%d", fps);
			device->setWindowCaption(tmp);
			lastFPS = fps;
		}
	}
	// Irrlichtの終了
	device->drop();

	// メモリの開放
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
