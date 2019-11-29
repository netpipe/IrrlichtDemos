#ifndef IHOVERCRAFT_H_
#define IHOVERCRAFT_H_

#include "../core/GameObject.h"

class Terrain;

/**
 * This class is an interface to a GameObject that can me controlled via keyboard and mouse.
 * 
 */
class IHoverCraft : public GameObject
{
	
protected:

	dReal turnSpeed;
	dReal rollSpeed;
	dReal pitchSpeed;
	dReal moveSpeed;

	dReal altitude;
	
public:

	IHoverCraft(Game* game, const irr::core::stringc& name,
		const irr::core::stringc& meshPath,
		const irr::core::stringc& texture,
		const irr::core::vector3df& position = irr::core::vector3df(0.0f, 0.0f, 0.0f),
		const irr::core::vector3df& rotation = irr::core::vector3df(0.0f, 0.0f, 0.0f),
		const irr::core::vector3df& scale = irr::core::vector3df(1.0f, 1.0f, 1.0f),
		
		const dReal density = 1.0f, const dReal weight = 1.0f,
		const bool createBody = true, const bool gravity = true);
	
	virtual ~IHoverCraft();

	virtual void hover(const Terrain* terrain, const bool drawHooverRays = false);
	
	///Turn ship left or right.
	virtual const void turn(const irr::f32 rot) const;

	///Pitch ship up or down.
	virtual const void pitch(const irr::f32 rot) const;

	///Roll ship left or right.
	virtual const void roll(const irr::f32 rot) const;

	///Moves ship forward or backfward.
	virtual const void move(const irr::f32 elapsed) const;
	
	virtual inline const dReal getAltitude() const { return this->altitude; }
};

#endif /*IHOVERCRAFT_H_*/
