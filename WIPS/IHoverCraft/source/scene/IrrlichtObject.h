#ifndef IRRLICHTOBJECT_H_
#define IRRLICHTOBJECT_H_

#include <IAnimatedMeshSceneNode.h>

class Game;

/**
 * This class is one of the base classes of GameObject.
 */
class IrrlichtObject
{

protected:

	irr::scene::IAnimatedMesh* mesh;

	irr::scene::IAnimatedMeshSceneNode* node;

public:
	
	IrrlichtObject(Game* game, const irr::core::stringc& name,
		const irr::core::stringc& meshPath,	const irr::core::stringc& texture,
		const irr::core::vector3df& position = irr::core::vector3df(0.0f, 0.0f, 0.0f),
		const irr::core::vector3df& rotation = irr::core::vector3df(0.0f, 0.0f, 0.0f),
		const irr::core::vector3df& scale = irr::core::vector3df(1.0f, 1.0f, 1.0f)
	);

	virtual ~IrrlichtObject();
	
	virtual inline irr::scene::IAnimatedMeshSceneNode* getNode() const { return this->node; }
};

#endif /*IRRLICHTENTITY_H_*/
