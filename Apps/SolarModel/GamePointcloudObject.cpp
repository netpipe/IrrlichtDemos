#include "GameGlobals.h"


GamePointcloudObject::GamePointcloudObject()
{
	Type = GAME_OBJECT_TYPE_POINTCLOUD;

	scene::SMesh* msh = new scene::SMesh();
	scene::SMeshBuffer* mb = new scene::SMeshBuffer();
	msh->addMeshBuffer( mb );
	pPointcloudNode = Game.Scene->addMeshSceneNode( msh );
	pPointcloudNode->setMaterialFlag( video::EMF_POINTCLOUD, true );
	pPointcloudNode->setMaterialFlag( video::EMF_BACK_FACE_CULLING, false );
	pPointcloudNode->setAutomaticCulling( scene::EAC_BOX );

	pNode = (scene::ISceneNode*) pPointcloudNode;

	assert( pNode != NULL );
	pNode->setID( ID );
}


GamePointcloudObject::~GamePointcloudObject()
{
	// nothing
}


void GamePointcloudObject::Load(io::IXMLReader *xml)
{
	float radius_from = 0.0f, radius_to = 100.0f;
	float height_from = -10.0f, height_to = +10.0f;
	float bright_from = 0.0f, bright_to = 1.0f;
	int pointcount = 100;

	int volume_every_at = 0;
	core::stringc volume_textures;
	int volume_texturescount = 0;

	int bb_every_at = 0;
	core::stringc bb_textures;
	int bb_texturescount = 0;

	while(xml && xml->read())
	{
		core::stringc node = xml->getNodeName();
		if (xml->getNodeType()==io::EXN_ELEMENT_END && node==L"pointcloud") break;
		if (node==L"object") LoadObject(xml);
		if (node==L"material") LoadMaterial(xml);
		if (node==L"rotationanimator") LoadRotationAnimator(xml);
		if (node==L"flycircleanimator") LoadFlyCircleAnimator(xml);
		if (node==L"radiusrange") {
			radius_from = xml->getAttributeValueAsFloat(L"from");
			radius_to = xml->getAttributeValueAsFloat(L"to");
		}
		if (node==L"heightrange") {
			height_from = xml->getAttributeValueAsFloat(L"from");
			height_to = xml->getAttributeValueAsFloat(L"to");
		}
		if (node==L"brightrange") {
			bright_from = xml->getAttributeValueAsFloat(L"from");
			bright_to = xml->getAttributeValueAsFloat(L"to");
		}
		if (node==L"points") pointcount = xml->getAttributeValueAsInt(L"count");
		if (node==L"volumes") {
			volume_every_at = xml->getAttributeValueAsInt(L"everypointat");
			volume_textures = xml->getAttributeValue(L"textures");
			volume_texturescount = xml->getAttributeValueAsInt(L"texturescount");
		}
		if (node==L"billboards") {
			bb_every_at = xml->getAttributeValueAsInt(L"everypointat");
			bb_textures = xml->getAttributeValue(L"textures");
			bb_texturescount = xml->getAttributeValueAsInt(L"texturescount");
		}
	}

	BuildPointcloud(
		radius_from, radius_to,
		height_from, height_to,
		bright_from, bright_to,
		pointcount,
		volume_every_at, volume_textures, volume_texturescount,
		bb_every_at, bb_textures, bb_texturescount
	);
}


void GamePointcloudObject::BuildPointcloud(
	float radius_from, float radius_to, float height_from, float height_to,
	float bright_from, float bright_to, int pointcount,
	int volume_every_at, core::stringc volume_textures, int volume_texturescount,
	int bb_every_at, core::stringc bb_textures, int bb_texturescount)
{
	assert( pPointcloudNode != NULL );

	scene::SMesh* msh = new scene::SMesh();
	scene::SMeshBuffer* mb = new scene::SMeshBuffer();
	msh->addMeshBuffer( mb );

	u32 c;
	float x, y, z;
	int radius_delta = (int) ( radius_to-radius_from );
	int height_delta = (int) ( height_to-height_from );
	int bright_delta = (int) ( 255.0*(bright_to-bright_from) );

	video::S3DVertex* t_vtx = new video::S3DVertex[ pointcount ];

	for( int i = 0; i < pointcount; i++ )
	{ 
		float degInRad = ( rand() % 36000 ) / 100.0f;

		x = (float)cos(degInRad) * ( radius_from + (float)( rand() % radius_delta ) );
		z = (float)sin(degInRad) * ( radius_from + (float)( rand() % radius_delta ) );
		y = height_from + (float)( rand() % height_delta );

		c = (int)(bright_from*255.0) + ( rand() % bright_delta );
		t_vtx[i] = video::S3DVertex(
			core::vector3df( x, y, z ),
			core::vector3df( 0, 0, 1 ),
			video::SColor( 255, c, c, c ),
			core::vector2df( 0, 0 )
		);

		if (bb_every_at>0 && (i%bb_every_at)==0)
		{
			float s = (float)(rand()%100) / 100.0f;
			scene::IBillboardSceneNode *b = Game.Scene->addBillboardSceneNode(
				pPointcloudNode, core::dimension2d<f32>(s, s),
				core::vector3df(x, y, z), -1, -1, -1
			);
			b->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
			b->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
			b->setMaterialFlag(video::EMF_LIGHTING, false);

			char ttt[ 256 ];
			sprintf(ttt, bb_textures.c_str(), rand()%bb_texturescount);
			b->setMaterialTexture(0, Game.Video->getTexture(ttt));
		}

		if (volume_every_at>0 && (i%volume_every_at)==0)
		{
			scene::ISceneNode* n = Game.Scene->addSphereSceneNode(
				((float)(rand()%200) + 50.0f) / 80.0f,
				(rand()%3) + 3, pPointcloudNode, -1,
				core::vector3df(x, y, z),
				core::vector3df((float)(rand()%360), (float)(rand()%360), (float)(rand()%360)),
				core::vector3df(((rand()%10)/10.0f)+0.5f, ((rand()%10)/10.0f)+0.5f, 0.75f)
			);

			if ((rand()%2)==0)
			{
				scene::ISceneNodeAnimator* a = 0;
				switch (rand()%3)
				{
				case 0:
					a = Game.Scene->createRotationAnimator( core::vector3df(
						( (float)(rand()%10) ) / 10000.0f, 0.0, 0.0
					) );
					break;
				case 1:
					a = Game.Scene->createRotationAnimator( core::vector3df(
						0.0, ( (float)(rand()%10) ) / 10000.0f, 0.0
					) );
					break;
				case 2:
					a = Game.Scene->createRotationAnimator( core::vector3df(
						0.0, 0.0, ( (float)(rand()%10) ) / 10000.0f
					) );
					break;
				}
				n->addAnimator(a);
				a->drop();
			}

			n->getMaterial(0) = pPointcloudNode->getMaterial(0);
			n->setMaterialFlag( video::EMF_BACK_FACE_CULLING, true );
			n->setMaterialFlag( video::EMF_POINTCLOUD, false );
			n->setMaterialFlag( video::EMF_LIGHTING, true );

			char ttt[ 256 ];
			sprintf(ttt, volume_textures.c_str(), rand()%volume_texturescount);
			n->setMaterialTexture(0, Game.Video->getTexture(ttt));
		}
	} 

	for ( int i = 0; i < pointcount; i++ )
	{ 
		mb->Vertices.push_back(t_vtx[i]);
		mb->Indices.push_back(i);
	} 

	delete t_vtx;

	msh->getMeshBuffer(0)->getMaterial() = pPointcloudNode->getMaterial(0);
	msh->recalculateBoundingBox();

	pPointcloudNode->setMesh( msh );

	// pPointcloudNode->setDebugDataVisible( scene::EDS_BBOX );

	pPointcloudNode->getMesh()->setBoundingBox( core::aabbox3df(
		-radius_to, height_from, -radius_to, radius_to, height_to, radius_to
	) );
}