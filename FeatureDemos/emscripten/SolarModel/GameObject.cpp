#include "GameGlobals.h"


GameObject::GameObject()
{
	ID = Game.Map->GetFreeID();
	Type = GAME_OBJECT_TYPE_UNKNOWN;
	pNode = NULL;

	Name = L"Object_";
	Name += ID;
}


GameObject::~GameObject()
{
	// nothing
}


enum video::E_MATERIAL_TYPE GameObject::GetMaterialTypeByName(core::stringc s)
{
	s.make_upper();

	const core::stringc matnames[] = {
		"EMT_SOLID",
		"EMT_SOLID_2_LAYER",
		"EMT_LIGHTMAP",
		"EMT_LIGHTMAP_ADD",
		"EMT_LIGHTMAP_M2",
		"EMT_LIGHTMAP_M4",
		"EMT_LIGHTMAP_LIGHTING",
		"EMT_LIGHTMAP_LIGHTING_M2",
		"EMT_LIGHTMAP_LIGHTING_M4",
		"EMT_DETAIL_MAP",
		"EMT_SPHERE_MAP",
		"EMT_REFLECTION_2_LAYER",
		"EMT_TRANSPARENT_ADD_COLOR",
		"EMT_TRANSPARENT_ALPHA_CHANNEL",
		"EMT_TRANSPARENT_ALPHA_CHANNEL_REF",
		"EMT_TRANSPARENT_VERTEX_ALPHA",
		"EMT_TRANSPARENT_REFLECTION_2_LAYER",
		"EMT_NORMAL_MAP_SOLID",
		"EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR",
		"EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA",
		"EMT_PARALLAX_MAP_SOLID",
		"EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR",
		"EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA",
		""
	};

	int type = 0;

	while ( true ) {
		if ( s==matnames[type] ) break;
		if ( matnames[++type].size()==0 ) {
			GameLog( "ERROR: Bad Material Type name \"%s\"", s.c_str() );
			break;
		}
	}

	return (enum video::E_MATERIAL_TYPE) type;
}


enum video::E_MATERIAL_FLAG GameObject::GetMaterialFlagByName(core::stringc s)
{
	s.make_upper();

	const core::stringc flagnames[] = {
		"EMF_WIREFRAME",
		"EMF_POINTCLOUD",
		"EMF_GOURAUD_SHADING",
		"EMF_LIGHTING",
		"EMF_ZBUFFER",
		"EMF_ZWRITE_ENABLE",
		"EMF_BACK_FACE_CULLING",
		"EMF_FRONT_FACE_CULLING",
		"EMF_BILINEAR_FILTER",
		"EMF_TRILINEAR_FILTER",
		"EMF_ANISOTROPIC_FILTER",
		"EMF_FOG_ENABLE",
		"EMF_NORMALIZE_NORMALS",
		"EMF_TEXTURE_WRAP",
		""
	};

	int flag = 0;

	while ( true ) {
		if ( s==flagnames[flag] ) break;
		if ( flagnames[++flag].size()==0 ) {
			GameLog( "ERROR: Bad Material Flag name \"%s\"", s.c_str() );
			break;
		}
	}

	return (enum video::E_MATERIAL_FLAG) flag;
}


bool GameObject::GetBooleanByName(core::stringc s)
{
	s.make_upper();

	bool b = false;

	if ( s=="ON" ) b = true;
	else if ( s=="OFF" ) b = false;
	else GameLog( "ERROR: Bad boolean value \"%s\"", s.c_str() );

	return b;
}


void GameObject::LoadObject(io::IXMLReader *xml)
{
	assert( pNode != NULL );

	while( xml && xml->read() )
	{
		core::stringc node = xml->getNodeName();
		if (xml->getNodeType()==io::EXN_ELEMENT_END && node=="object") break;
		if (node=="name") Name = xml->getAttributeValue(L"value");
		if (node=="parent") pNode->setParent( Game.Map->GetObjectByName( xml->getAttributeValue(L"value") )->pNode );
		if (node=="position") {
			pNode->setPosition( core::vector3df(
				xml->getAttributeValueAsFloat(L"x"),
				xml->getAttributeValueAsFloat(L"y"),
				xml->getAttributeValueAsFloat(L"z")
			) );
		}
		if (node=="rotation") {
			pNode->setRotation( core::vector3df(
				xml->getAttributeValueAsFloat(L"x"),
				xml->getAttributeValueAsFloat(L"y"),
				xml->getAttributeValueAsFloat(L"z")
			) );
		}
		if (node=="scale") {
			pNode->setScale( core::vector3df(
				xml->getAttributeValueAsFloat(L"x"),
				xml->getAttributeValueAsFloat(L"y"),
				xml->getAttributeValueAsFloat(L"z")
			) );
		}
	}
}


void GameObject::LoadMaterial(io::IXMLReader *xml)
{
	assert(pNode != NULL);

	while(xml && xml->read())
	{
		core::stringc node = xml->getNodeName();

		if (xml->getNodeType() == io::EXN_ELEMENT_END &&
			node == L"material")
		{
			break;
		}

		if (node == L"type")
		{
			pNode->setMaterialType(
				GetMaterialTypeByName(xml->getAttributeValue(L"value")));
		}

		if (node == L"flag")
		{
			pNode->setMaterialFlag(
				GetMaterialFlagByName(xml->getAttributeValue(L"name")),
				GetBooleanByName(xml->getAttributeValue(L"value")));
		}

		if (node == L"texture")
		{
			core::stringc t = xml->getAttributeValue(L"file");
			pNode->setMaterialTexture(
				xml->getAttributeValueAsInt(L"layer"),
				Game.Video->getTexture(t.c_str()));
		}

		if (node == L"diffusecolor")
		{
			pNode->getMaterial(xml->getAttributeValueAsInt(L"layer")).DiffuseColor.set(
				(int)( xml->getAttributeValueAsFloat(L"a")*256.0f ),
				(int)( xml->getAttributeValueAsFloat(L"r")*256.0f ),
				(int)( xml->getAttributeValueAsFloat(L"g")*256.0f ),
				(int)( xml->getAttributeValueAsFloat(L"b")*256.0f ));
		}

		if (node == L"specularcolor")
		{
			pNode->getMaterial(xml->getAttributeValueAsInt(L"layer")).SpecularColor.set(
				(int)( xml->getAttributeValueAsFloat(L"a")*256.0f ),
				(int)( xml->getAttributeValueAsFloat(L"r")*256.0f ),
				(int)( xml->getAttributeValueAsFloat(L"g")*256.0f ),
				(int)( xml->getAttributeValueAsFloat(L"b")*256.0f ));
		}
		if (node == L"emissivecolor")
		{
			pNode->getMaterial(xml->getAttributeValueAsInt(L"layer")).EmissiveColor.set(
				(int)( xml->getAttributeValueAsFloat(L"a")*256.0f ),
				(int)( xml->getAttributeValueAsFloat(L"r")*256.0f ),
				(int)( xml->getAttributeValueAsFloat(L"g")*256.0f ),
				(int)( xml->getAttributeValueAsFloat(L"b")*256.0f ));
		}

		if (node == L"heightmap")
		{
			core::stringc t = xml->getAttributeValue(L"file");
			float h = xml->getAttributeValueAsFloat(L"height");
			float p = xml->getAttributeValueAsFloat(L"parallax");
			InitHeightMap(t.c_str(), h, p);
		}

		// ... addable ...
	}
}


void GameObject::LoadRotationAnimator(io::IXMLReader *xml)
{
	assert( pNode != NULL );

	while( xml && xml->read() )
	{
		core::stringc node = xml->getNodeName();
		if ( xml->getNodeType()==io::EXN_ELEMENT_END && node==L"rotationanimator" ) break;
		if ( node==L"rotation" ) {
			scene::ISceneNodeAnimator* a =
				Game.Scene->createRotationAnimator( core::vector3df(
					xml->getAttributeValueAsFloat(L"x"),
					xml->getAttributeValueAsFloat(L"y"),
					xml->getAttributeValueAsFloat(L"z")
				) );
			pNode->addAnimator( a );
			a->drop();
		}
	}
}


void GameObject::LoadFlyCircleAnimator(io::IXMLReader *xml)
{
	float center_x=0.0f, center_y=0.0f, center_z=0.0f;
	float direction_x=0.0f, direction_y=1.0f, direction_z=0.0f;
	float radius=100.0f;
	float speed=0.001f;

	assert( pNode != NULL );

	while( xml && xml->read() )
	{
		core::stringc node = xml->getNodeName();
		if ( xml->getNodeType()==io::EXN_ELEMENT_END && node==L"flycircleanimator" ) break;
		if ( node==L"center" ) {
			center_x = xml->getAttributeValueAsFloat(L"x");
			center_y = xml->getAttributeValueAsFloat(L"y");
			center_z = xml->getAttributeValueAsFloat(L"z");
		}
		if ( node==L"direction" ) {
			direction_x = xml->getAttributeValueAsFloat(L"x");
			direction_y = xml->getAttributeValueAsFloat(L"y");
			direction_z = xml->getAttributeValueAsFloat(L"z");
		}
		if ( node==L"radius" ) radius = xml->getAttributeValueAsFloat(L"value");
		if ( node==L"speed" ) speed = xml->getAttributeValueAsFloat(L"value");
	}

	scene::ISceneNodeAnimator* a = Game.Scene->createFlyCircleAnimator(
		core::vector3df( center_x, center_y, center_z ),
		radius, speed,
		core::vector3df( direction_x, direction_y, direction_z )
	);

	pNode->addAnimator( a );
	a->drop();
}


void GameObject::InitHeightMap(const char *texture, float height, float parallax)
{
	// This is implemented only for IMeshSceneNode;
	// to implement it for IAnimatedMeshSceneNode it is neccessary to have:
	// animatedMesh->setMesh(0, tMesh);

	if (this->Type != GAME_OBJECT_TYPE_MESH) return;

	if (Game.PerPixelLighting == 0) return;

	video::E_MATERIAL_TYPE materialType =
		Game.PerPixelLighting == 1 ?
			video::EMT_NORMAL_MAP_SOLID :
			video::EMT_PARALLAX_MAP_SOLID;

	scene::IMeshSceneNode *aNode = (scene::IMeshSceneNode *)this->pNode;
	scene::IMesh *tMesh = Game.MeshManipulator->createMeshWithTangents(aNode->getMesh());

	video::ITexture* texture_diffuse = aNode->getMaterial(0).getTexture(0);
	video::ITexture* texture_height = Game.Video->getTexture(texture);

	Game.Video->makeNormalMapTexture(texture_height, height);

	Game.MeshManipulator->recalculateNormals(tMesh, true);

	aNode->setMesh(tMesh);
	aNode->setMaterialTexture(0, texture_diffuse);
	aNode->setMaterialTexture(1, texture_height);
	aNode->setMaterialType(materialType);
	aNode->getMaterial(0).MaterialTypeParam = parallax;

	tMesh->drop();
}