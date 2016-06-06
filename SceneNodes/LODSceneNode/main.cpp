
#include <irrlicht.h>
#include <iostream>
#include <time.h>
//#include <windows.h>
using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;
#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif



class CLODSceneNode : public scene::ISceneNode
{

	class Vertex;
	class Triangle;
	class Triangle {
	public:
		Vertex* vertex[3];
		Triangle(Vertex* v1, Vertex* v2, Vertex* v3) {
			vertex[0] = v1;
			vertex[1] = v2;
			vertex[2] = v3;
			for (int x = 0; x < 3; x++) {
				vertex[x]->face.push_back(this);
				for (int y = 0; y < 3; y++) {
					bool add = true;
					for(int i = 0; i < vertex[x]->neighbors.size(); i++) {
						if (vertex[y]->id  == vertex[x]->neighbors[i]->id || y == x) {
							add = false;
						}
					}
					if (add) {
						vertex[x]->neighbors.push_back(vertex[y]);

					}
				}
			}
		}
		bool HasVertex(Vertex* v) { return (v->position  == vertex[0]->position || v->position == vertex[1]->position  || v->position == vertex[2]->position ); };
		void ReplaceVertex(Vertex* target, Vertex* v) {
			for (int x = 0; x < 3; x++)
				if (vertex[x]->position == target->position ) {
					vertex[x] = v;

				}
		}
		bool CheckAndRemove(int num, array<Triangle*> *list) {
			if (vertex[0]->position == vertex[1]->position || vertex[1]->position == vertex[2]->position || vertex[2]->position == vertex[0]->position) {
				list->erase(num);
				return true;
			}
		}
	};
	class Vertex {
	public:
		vector3df position;
		array<Vertex*> neighbors;
		array<Triangle*> face;
		int id;
	};



	IMeshBuffer* BuildMeshBufferFromTriangles(IMeshBuffer* oldmb, array<Triangle*> arr) {
		SMeshBuffer* newm = new SMeshBuffer();
		newm->Material = oldmb->getMaterial();
		u16* indices = new u16[arr.size()*3];
		for (int x = 0; x < arr.size(); x++) {
			for (int y = 0; y < 3; y++) {
				indices[x*3+y] = arr[x]->vertex[y]->id;
			}
		}
		newm->append(oldmb->getVertices(),oldmb->getVertexCount(),indices,arr.size()*3);
		return newm;
	}



	IMesh* DefaultMesh;
	IMesh** LODMesh;
	IMesh* CurrentMesh;
	IMeshBuffer* test;
	char CurrentLevel;
	char LevelCount;
	float LODBegin;
	float LODLast;
	bool LODOn;
	array<Vertex*> Verts[8];
	array<Triangle*> Tris[8];
public:

	CLODSceneNode(IMesh* mesh, scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, int numOfCollapseOnLast, char numOfLevels = 4, float LODBeginDist = 10, float LODLastDist = 30, bool combineDuplicateVertices = true)
		: scene::ISceneNode(parent, mgr, id)
	{
		DefaultMesh = mesh;
		CurrentMesh = mesh;
		//CurrentMesh->getMeshBuffer(0)->getMaterial().Wireframe = true;
		LODBegin = LODBeginDist;
		LODLast = LODLastDist;
		LevelCount = numOfLevels;
		CurrentLevel = 0;
		LODMesh = new IMesh*[numOfLevels];
		LODOn = true;
		if (combineDuplicateVertices)
			DefaultMesh = SceneManager->getMeshManipulator()->createMeshWelded(DefaultMesh);


		for (int x = 0; x < DefaultMesh->getMeshBufferCount(); x++) {
			S3DVertex* verts = (S3DVertex*)DefaultMesh->getMeshBuffer(x)->getVertices();


			for (int y = 0; y < DefaultMesh->getMeshBuffer(x)->getVertexCount(); y++) {
				Vertex* vert = new Vertex();
				vert->position = verts[y].Pos;
				vert->id = y;
				Verts[x].push_back(vert);
			}

		}

		for (int x = 0; x < DefaultMesh->getMeshBufferCount(); x++) {
			u16* indices = DefaultMesh->getMeshBuffer(x)->getIndices();

			for (int y = 0; y < DefaultMesh->getMeshBuffer(x)->getIndexCount(); y += 3) {
				Triangle *tri = new Triangle(Verts[x][indices[y]],Verts[x][indices[y+1]],Verts[x][indices[y+2]]);

				Tris[x].push_back(tri);
			}

		}

		for (int x = 0; x < LevelCount; x++)
			LODMesh[x] = DefaultMesh;


		int collapsePerLevel = numOfCollapseOnLast/(LevelCount-1);
		int collapsePerBuffer = collapsePerLevel/DefaultMesh->getMeshBufferCount();


		for (int x = 1; x < LevelCount; x++) {
			SMesh* newLod = new SMesh();
			for (int y = 0; y < DefaultMesh->getMeshBufferCount(); y++) {
				for (int i = 0; i < collapsePerBuffer; i++) {
					int vert = rand()%Verts[y].size();
					int neighbor = rand()%Verts[y][vert]->neighbors.size();
					for (int u = 0; u < Tris[y].size(); u++) {
						if (Tris[y][u]->HasVertex(Verts[y][vert])) {
							Tris[y][u]->ReplaceVertex(Verts[y][vert],Verts[y][vert]->neighbors[neighbor]);

						}
					}
				}
				SMeshBuffer* mn = new SMeshBuffer();
				for (int x = 0; x < Tris[0].size(); x++) {
					if (Tris[0][x]->CheckAndRemove(x,&Tris[0])) {
						x--;
					}
				}
				mn = (SMeshBuffer*)BuildMeshBufferFromTriangles(DefaultMesh->getMeshBuffer(y),Tris[y]);
				newLod->addMeshBuffer(mn);
				newLod->recalculateBoundingBox();
			}
			LODMesh[x] = newLod;

		}







	}


	virtual void OnRegisterSceneNode()
	{
		if (IsVisible)
			SceneManager->registerNodeForRendering(this);

		ISceneNode::OnRegisterSceneNode();
	}


	virtual void render()
	{
		if (LODOn) {
			vector3df cameraPos = SceneManager->getActiveCamera()->getPosition();
			f32 dist = AbsoluteTransformation.getTranslation().getDistanceFrom(cameraPos);
			CurrentLevel = 0;
			float increment = (LODLast-LODBegin)/LevelCount;
			for (int x = 0; x < LevelCount; x++) {
				if (dist >= (LODBegin+increment*x)*(LODBegin+increment*x))
					CurrentLevel = x;
			}
			CurrentMesh = LODMesh[CurrentLevel];

		}
		else
			CurrentMesh = DefaultMesh;
		IVideoDriver* driver = SceneManager->getVideoDriver();
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		for (int x = 0; x < CurrentMesh->getMeshBufferCount(); x++) {
			driver->setMaterial(CurrentMesh->getMeshBuffer(x)->getMaterial());
			driver->drawMeshBuffer(CurrentMesh->getMeshBuffer(x));
		}
	}

	virtual const core::aabbox3d<f32>& getBoundingBox() const
	{
		return CurrentMesh->getBoundingBox();
	}

	void setLODOn(bool on) { LODOn = on; };
	bool getLODOn() { return LODOn; };

};


int main()
{

	srand(time(NULL));
	video::E_DRIVER_TYPE driverType;

	printf("Please select the driver you want for this example:\n"\
		" (a) Direct3D 9.0c\n (b) Direct3D 8.1\n (c) OpenGL 1.5\n"\
		" (d) Software Renderer\n (e) Burning's Software Renderer\n"\
		" (f) NullDevice\n (otherKey) exit\n\n");

	char i;
	std::cin >> i;

	switch(i)
	{
		case 'a': driverType = video::EDT_DIRECT3D9;break;
		case 'b': driverType = video::EDT_DIRECT3D8;break;
		case 'c': driverType = video::EDT_OPENGL;   break;
		case 'd': driverType = video::EDT_SOFTWARE; break;
		case 'e': driverType = video::EDT_BURNINGSVIDEO;break;
		case 'f': driverType = video::EDT_NULL;     break;
		default: return 0;
	}



	IrrlichtDevice *device = createDevice(driverType,
			core::dimension2du(1024, 768), 32, false);

	if (device == 0)
		return 1;

	device->setWindowCaption(L"Custom Scene Node - Irrlicht Engine Demo");

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();
	smgr->addLightSceneNode(camera,vector3df(0,0,0),SColor(1,1,1,1),10000);
	const int num = 10;
	CLODSceneNode *myNode[num];
	for (int x = 0; x < num; x++) {
	myNode[x] =
			new CLODSceneNode(smgr->getMesh("Albatros.obj"),smgr->getRootSceneNode(), smgr, 666, 1000, 5, 10, 50);
			myNode[x]->setPosition(vector3df(rand()%300-600,rand()%300-600,rand()%300-600));
	}



	scene::ISceneNodeAnimator* anim =
		smgr->createRotationAnimator(core::vector3df(0.8f, 0, 0.8f));


	u32 frames=0;
	while(device->run())
	{
		driver->beginScene(true, true, video::SColor(0,100,100,100));

		smgr->drawAll();

		for (int x = 0; x < num; x++) {
//			myNode[x]->setLODOn(!(GetAsyncKeyState('L') < 0));
		}

		driver->endScene();
		if (++frames==100)
		{
			core::stringw str = L"Irrlicht Engine [";
			str += driver->getName();
			str += L"] FPS: ";
			str += (s32)driver->getFPS();
			str += L" Tris: ";
			str += driver->getPrimitiveCountDrawn();

			device->setWindowCaption(str.c_str());
			frames=0;
		}
	}

	device->drop();

	return 0;
}


