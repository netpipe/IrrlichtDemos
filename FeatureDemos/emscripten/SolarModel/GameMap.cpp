#include "GameGlobals.h"


GameMap::GameMap()
{
	Objects = NULL;
	ObjectsCount = 0;
}


GameMap::~GameMap()
{
	if ( Objects != NULL && ObjectsCount != 0 )
	{
		for ( int i=0; i<ObjectsCount; i++ ) delete Objects[ i ];
		delete Objects;
		ObjectsCount = 0;
	}
}


int GameMap::GetFreeID()
{
	int id = 1001;
	
	for ( int i=0; i<ObjectsCount; i++ ) {
		if ( Objects[i]->ID == id ) {
			id++;
			i = 0;
		}
	}

	return id;
}


GameObject* GameMap::GetObjectByName(core::stringc s)
{
	for ( int i=0; i<ObjectsCount; i++ ) {
		if ( s==Objects[i]->Name ) {
			return Objects[i];
		}
	}

	GameLog("ERROR: object %s not found!", s.c_str());
	return NULL;
}


void GameMap::AddObject(GameObject* obj)
{
	GameObject **newObjects = new GameObject*[ObjectsCount+1];
	for ( int i=0; i<ObjectsCount; i++ ) newObjects[i] = Objects[i];
	newObjects[ObjectsCount++] = obj;
	if ( Objects != NULL ) delete Objects;
	Objects = newObjects;
}


void GameMap::Load(core::stringc fn)
{
	GameLog("Map %s loading started...", fn.c_str());
	io::IXMLReader *xml = Game.FS->createXMLReader(fn.c_str());
	
	scene::ISceneNode *rootNode = Game.Scene->getRootSceneNode();
	core::vector3df mapshift(0,0,0);
	GameObject *loadedObject;

	while( xml && xml->read() )
	{
		loadedObject = 0;

		core::stringc node = xml->getNodeName();

		if (node == L"skybox")
		{
			GameSkyboxObject* skybox = new GameSkyboxObject();
			AddObject(skybox);
			skybox->Load(xml);
			GameLog("Skybox loaded.");
			loadedObject = skybox;
		}
		
		if (node == L"light")
		{
			GameLightObject* light = new GameLightObject();
			AddObject(light);
			light->Load(xml);
			GameLog("Light %s loaded.", light->Name.c_str());
			loadedObject = light;
		}
		
		if (node == L"billboard")
		{
			GameBillboardObject* billboard = new GameBillboardObject();
			AddObject(billboard);
			billboard->Load(xml);
			GameLog("Billboard %s loaded.", billboard->Name.c_str());
			loadedObject = billboard;
		}

		if (node == L"animatedmesh")
		{
			GameAnimatedMeshObject* animatedmesh =
				new GameAnimatedMeshObject(xml->getAttributeValue(L"file"));

			AddObject(animatedmesh);
			animatedmesh->Load(xml);
			GameLog("AnimatedMesh %s loaded.", animatedmesh->Name.c_str());
			loadedObject = animatedmesh;
		}

		if (node == L"pointcloud")
		{
			GamePointcloudObject* pointcloud = new GamePointcloudObject();
			AddObject(pointcloud);
			pointcloud->Load(xml);
			GameLog("Pointcloud %s loaded.", pointcloud->Name.c_str());
			loadedObject = pointcloud;
		}

		if (node == L"mesh")
		{
			GameMeshObject* mesh =
				new GameMeshObject(xml->getAttributeValue(L"file"));

			AddObject(mesh);
			mesh->Load(xml);
			GameLog("Mesh %s loaded.", mesh->Name.c_str());
			loadedObject = mesh;
		}

		// ...addable...

		if (node == L"mapshift")
		{
			float x = xml->getAttributeValueAsFloat(L"x");
			float y = xml->getAttributeValueAsFloat(L"y");
			float z = xml->getAttributeValueAsFloat(L"z");
			mapshift = core::vector3df(x, y, z);
		}

		if (loadedObject != 0 &&
			loadedObject->pNode->getParent() == rootNode &&
			mapshift.getLengthSQ() > 0.0001f)
		{
			core::vector3df pos = loadedObject->pNode->getPosition();
			loadedObject->pNode->setPosition(pos + mapshift);
		}
	}

	xml->drop();
	GameLog("Map %s loaded.", fn.c_str());
}
