
#include "GameGlobals.h"


GameBillboardObject::GameBillboardObject()
{
	Type = GAME_OBJECT_TYPE_BILLBOARD;

	pBillboardNode = Game.Scene->addBillboardSceneNode();
	pNode = (scene::ISceneNode*) pBillboardNode;

	assert( pNode != NULL );
	pNode->setID( ID );
}


GameBillboardObject::~GameBillboardObject()
{
	// nothing
}


void GameBillboardObject::Load(io::IXMLReader *xml)
{
	while( xml && xml->read() )
	{
		core::stringc node = xml->getNodeName();
		if ( xml->getNodeType()==io::EXN_ELEMENT_END && node==L"billboard" ) break;
		if ( node==L"object" ) LoadObject(xml);
		if ( node==L"material" ) LoadMaterial(xml);
		if ( node==L"rotationanimator" ) LoadRotationAnimator(xml);
		if ( node==L"flycircleanimator" ) LoadFlyCircleAnimator(xml);
		if ( node==L"size" ) {
			pBillboardNode->setSize( core::dimension2df(
				xml->getAttributeValueAsFloat(L"width"),
				xml->getAttributeValueAsFloat(L"height")
			) );
		}
	}
}
