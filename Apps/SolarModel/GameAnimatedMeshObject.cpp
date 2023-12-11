#include "GameGlobals.h"


GameAnimatedMeshObject::GameAnimatedMeshObject(core::stringc file)
{
	Type = GAME_OBJECT_TYPE_ANIMATEDMESH;

	pAnimatedMeshNode = Game.Scene->addAnimatedMeshSceneNode( Game.Scene->getMesh(file.c_str()) );
	pNode = (scene::ISceneNode*) pAnimatedMeshNode;

	assert( pNode != NULL );
	pNode->setID( ID );
}


GameAnimatedMeshObject::~GameAnimatedMeshObject()
{
	// nothing
}


void GameAnimatedMeshObject::Load(io::IXMLReader *xml)
{
	while( xml && xml->read() )
	{
		core::stringc node = xml->getNodeName();
		if ( xml->getNodeType()==io::EXN_ELEMENT_END && node==L"animatedmesh" ) break;
		if ( node==L"object" ) LoadObject(xml);
		if ( node==L"material" ) LoadMaterial(xml);
		if ( node==L"rotationanimator" ) LoadRotationAnimator(xml);
		if ( node==L"flycircleanimator" ) LoadFlyCircleAnimator(xml);
	}
}
