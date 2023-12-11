#include "GameGlobals.h"


GameSkyboxObject::GameSkyboxObject()
{
	Type = GAME_OBJECT_TYPE_SKYBOX;

	pNode = NULL;
}


GameSkyboxObject::~GameSkyboxObject()
{
	// nothing
}


void GameSkyboxObject::Load(io::IXMLReader *xml)
{
	core::stringc top, bottom, left, right, front, back;

	while( xml && xml->read() )
	{
		core::stringc node = xml->getNodeName();
		if ( xml->getNodeType()==io::EXN_ELEMENT_END && node==L"skybox" ) break;
		if ( node==L"top" ) top = xml->getAttributeValue( L"texture" );
		if ( node==L"bottom" ) bottom = xml->getAttributeValue( L"texture" );
		if ( node==L"left" ) left = xml->getAttributeValue( L"texture" );
		if ( node==L"right" ) right = xml->getAttributeValue( L"texture" );
		if ( node==L"front" ) front = xml->getAttributeValue( L"texture" );
		if ( node==L"back" ) back = xml->getAttributeValue( L"texture" );
	}

	bool tmp_bool = Game.Video->getTextureCreationFlag( video::ETCF_CREATE_MIP_MAPS );
	Game.Video->setTextureCreationFlag( video::ETCF_CREATE_MIP_MAPS, false );

	assert( pNode == NULL );
	pNode = Game.Scene->addSkyBoxSceneNode(
		Game.Video->getTexture( top.c_str() ),
		Game.Video->getTexture( bottom.c_str() ),
		Game.Video->getTexture( left.c_str() ),
		Game.Video->getTexture( right.c_str() ),
		Game.Video->getTexture( front.c_str() ),
		Game.Video->getTexture( back.c_str() )
	);
	assert( pNode != NULL );
	pNode->setID( ID );
	Game.ClearBackBuffer = false;

	Game.Video->setTextureCreationFlag( video::ETCF_CREATE_MIP_MAPS, tmp_bool );
}
