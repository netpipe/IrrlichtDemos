/*
  IWindGenerator.h - thank you Klasker :)

  http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=10762&start=15
 */

#ifndef IWINDGENERATOR_H_INCLUDED
#define IWINDGENERATOR_H_INCLUDED

#include <irrlicht.h>
#include <cstdlib>

namespace irr
{
namespace scene
{

class IWindGenerator : public IReferenceCounted
{

public:

	//setter

	virtual inline void setStrength(const f32 strength) = 0;

	virtual inline void setRegularity(const f32 regularity) = 0;

	//getter

	virtual inline f32 getStrength() const = 0;

	virtual inline f32 getRegularity() const = 0;

	virtual const core::vector2df& getWind(const core::vector3df& position, const u32 timeMs) = 0;

protected:

	core::vector2df wind;
};

IWindGenerator* createWindGenerator(const f32 strength = 3.0f, const f32 regularity = 1.0f);

} // scene
} // irr

#endif
