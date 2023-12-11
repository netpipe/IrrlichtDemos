#include "GameGlobals.h"


GameMeshObject::GameMeshObject(core::stringc file)
{
	Type = GAME_OBJECT_TYPE_MESH;

	pMeshNode = Game.Scene->addMeshSceneNode(Game.Scene->getMesh(file.c_str()));
	pNode = (scene::ISceneNode*)pMeshNode;

	assert(pNode != NULL);
	pNode->setID(ID);
}


GameMeshObject::~GameMeshObject()
{
	// nothing
}


void GameMeshObject::Load(io::IXMLReader *xml)
{
	while(xml && xml->read())
	{
		core::stringc node = xml->getNodeName();
		if (xml->getNodeType() == io::EXN_ELEMENT_END && node == L"mesh") break;
		if (node == L"object") LoadObject(xml);
		if (node == L"material") LoadMaterial(xml);
		if (node == L"rotationanimator") LoadRotationAnimator(xml);
		if (node == L"flycircleanimator") LoadFlyCircleAnimator(xml);
	}
}
