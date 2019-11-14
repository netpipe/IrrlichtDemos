#include "GameGlobals.h"


GameLightObject::GameLightObject()
{
	Type = GAME_OBJECT_TYPE_LIGHT;

	pLightNode = Game.Scene->addLightSceneNode( NULL, core::vector3df(1,1,1) );
	pNode = (scene::ISceneNode*) pLightNode;

	assert( pNode != NULL );
	pNode->setID( ID );
}


GameLightObject::~GameLightObject()
{
	// nothing
}


enum video::E_LIGHT_TYPE GameLightObject::GetLightTypeByName(core::stringc s)
{
	s.make_upper();

	const core::stringc lightnames[] = {
		"ELT_POINT",
		"ELT_DIRECTIONAL",
		""
	};

	int type = 0;

	while ( true ) {
		if ( s==lightnames[type] ) break;
		if ( lightnames[++type].size()==0 ) {
			GameLog( "ERROR: Bad Light Type name \"%s\"", s.c_str() );
			break;
		}
	}

	return (enum video::E_LIGHT_TYPE) type;
}


void GameLightObject::Load(io::IXMLReader *xml)
{
	assert( pLightNode != NULL );

	while( xml && xml->read() )
	{
		core::stringc node = xml->getNodeName();
		if ( xml->getNodeType()==io::EXN_ELEMENT_END && node==L"light" ) break;
		if ( node==L"object" ) LoadObject(xml);
		if ( node==L"material" ) LoadMaterial(xml);
		if ( node==L"rotationanimator" ) LoadRotationAnimator(xml);
		if ( node==L"flycircleanimator" ) LoadFlyCircleAnimator(xml);
		if ( node==L"type" ) {
			pLightNode->getLightData().Type =
				GetLightTypeByName( xml->getAttributeValue(L"value") );
		}
		if ( node==L"radius" ) {
			pLightNode->getLightData().Radius =
				xml->getAttributeValueAsFloat(L"value");
		}
		if ( node==L"ambientcolor" ) {
			pLightNode->getLightData().AmbientColor.set(
				xml->getAttributeValueAsFloat(L"r"),
				xml->getAttributeValueAsFloat(L"g"),
				xml->getAttributeValueAsFloat(L"b")
			);
		}
		if ( node==L"diffusecolor" ) {
			pLightNode->getLightData().DiffuseColor.set(
				xml->getAttributeValueAsFloat(L"r"),
				xml->getAttributeValueAsFloat(L"g"),
				xml->getAttributeValueAsFloat(L"b")
			);
		}
		if ( node==L"specularcolor" ) {
			pLightNode->getLightData().SpecularColor.set(
				xml->getAttributeValueAsFloat(L"r"),
				xml->getAttributeValueAsFloat(L"g"),
				xml->getAttributeValueAsFloat(L"b")
			);
		}
	}
}
