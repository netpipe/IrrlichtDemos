#pragma once
#ifdef __APPLE__

#include "irrlicht.h"
#include "IBaseEntity.hpp"

#else

#include "irrlicht/irrlicht.h"
#include "desteer/entity/IBaseEntity.hpp"

#endif

namespace desteer{
namespace entity {

class IrrlichtBaseEntity : public IBaseEntity
{
private:
    irr::scene::ISceneNode *_node;

public :
    IrrlichtBaseEntity(irr::scene::ISceneNode * node) {_node = node;}

    virtual irr::scene::ISceneNode* Node() const {return _node;}

	virtual irr::core::vector3df Position() const {return _node->getPosition();};
	virtual void SetPosition(irr::core::vector3df position) {_node->setPosition(position);};

	virtual float Radius() const {return (_node->getBoundingBox().getExtent().getLength() / 2);};

	virtual void Update(float timeElapsed) {return;};
};

}//end namespace entity
}//end namespace desteer
