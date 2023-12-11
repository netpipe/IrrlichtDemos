#include "Terrain.h"
#include "../core/Game.h"

Terrain::Terrain(Game* game) :
	terrain(0),
	
	physics(game->getPhysics()),
	geom(0), vertices(0), indices(0), data(0)
{
	irr::scene::ISceneManager* smgr = game->getSceneManager();
	irr::video::IVideoDriver* driver = game->getVideoDriver();

	// add terrain scene node
	terrain = smgr->addTerrainSceneNode( 
		"media/images/terrain/default/terrain-heightmap.bmp",
		0,										// parent node
		-1,										// node id
		irr::core::vector3df(0.f, 0.f, 0.f),	// position
		irr::core::vector3df(0.f, 0.f, 0.f),	// rotation
		irr::core::vector3df(40.f, 4.4f, 40.f),	// scale
		irr::video::SColor ( 255, 255, 255, 255 ),	// vertexColor,
		5,										// maxLOD
		irr::scene::ETPS_17,					// patchSize
		4										// smoothFactor
	);

	this->terrain->setMaterialType(irr::video::EMT_DETAIL_MAP);
	this->terrain->setMaterialFlag(irr::video::EMF_LIGHTING, game->getWeatherSystem()->isLightEnabled() ? true : false);
	this->terrain->setMaterialFlag(irr::video::EMF_FOG_ENABLE, game->getWeatherSystem()->isFogEnabled() ? true : false);
	this->terrain->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
	this->terrain->setMaterialTexture(0, driver->getTexture("media/images/terrain/default/terrain-texture.jpg"));
	this->terrain->setMaterialTexture(1, driver->getTexture("media/images/terrain/default/detailmap3.jpg"));
	this->terrain->scaleTexture(5.0f, 250.0f);
	this->terrain->grab();
	
	initOde();
}

const void Terrain::initOde()
{
	const irr::core::vector3df& pos = this->terrain->getPosition();
	const irr::core::vector3df& rotationRAD = (this->terrain->getRotation()*irr::core::DEGTORAD);
	const irr::core::quaternion irrQuat(rotationRAD);
	dQuaternion odeQuat;
	odeQuat[0] = irrQuat.W;
	odeQuat[1] = irrQuat.X;
	odeQuat[2] = irrQuat.Y;
	odeQuat[3] = irrQuat.Z;

	makeTriMeshData();
	this->geom = dCreateTriMesh(this->physics->getSpace(), this->data, 0, 0, 0);
	dGeomSetData(this->geom, this->terrain);
	dGeomSetPosition(this->geom, pos.X, pos.Y, pos.Z);
	dGeomSetQuaternion(this->geom, odeQuat);
}
