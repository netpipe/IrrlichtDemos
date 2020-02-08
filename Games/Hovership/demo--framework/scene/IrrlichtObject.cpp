#include "IrrlichtObject.h"
#include "../core/Game.h"

IrrlichtObject::IrrlichtObject(
	Game* game,
	const irr::core::stringc& name, const irr::core::stringc& meshPath, const irr::core::stringc& texture,
	const irr::core::vector3df& position, const irr::core::vector3df& rotation, const irr::core::vector3df& scale) :
	mesh(0), node(0)
{
	mesh = game->getSceneManager()->getMesh(meshPath.c_str());

	this->node = game->getSceneManager()->addAnimatedMeshSceneNode(mesh);
	if (texture != "")
		this->node->setMaterialTexture(0, game->getVideoDriver()->getTexture(texture.c_str()));
	this->node->setPosition(position);
	this->node->setRotation(rotation);
	this->node->setScale(scale);
	this->node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
	this->node->setMaterialFlag(irr::video::EMF_LIGHTING, game->getWeatherSystem()->isLightEnabled() ? true : false);
	this->node->setMaterialFlag(irr::video::EMF_FOG_ENABLE, game->getWeatherSystem()->isFogEnabled() ? true : false);
	this->node->setName(name.c_str());
	this->node->addShadowVolumeSceneNode();
}

IrrlichtObject::~IrrlichtObject()
{
	this->node->remove();
}
