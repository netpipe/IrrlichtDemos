#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "../scene/IrrlichtObject.h"
#include "../physics/OdeObject.h"

/**
 * This class represents the base class of all game entities
 * 
 * It consists of an Irrlicht part and an ODE part.
 */
class GameObject : public IrrlichtObject, public OdeObject
{

private:
	
	virtual const void makeTriMeshData();
	
	///Callback function for moving bodies.
	static void update(const dBodyID body);

	///Initializes ode geom, body, mass, etc.
	virtual const void init(const bool createBody,  const bool gravityMode);
	
protected:

	Game* game;

	virtual const void internalSetUpMass();

    virtual const void internalCreateGeom();

    static const irr::core::matrix4 getTransformationMatrix(const dBodyID body);

public:

	GameObject(
		Game* game,
		const irr::core::stringc& name,

		//irrlicht stuff
		const irr::core::stringc& meshPath,
		const irr::core::stringc& texture,
		const irr::core::vector3df& position,
		const irr::core::vector3df& rotation,
		const irr::core::vector3df& scale,

		//ode stuff
		const irr::f32 density, const irr::f32 weight,
		const bool createBody, const bool gravity
	);
};

#endif /*GAMEOBJECT_H_*/
