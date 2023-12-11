#ifndef COMPASS_H_
#define COMPASS_H_

#include <IGUIElement.h>
#include <SMeshBuffer.h>

/**
 * Displays a compass which is derived from IGUIElement.
 *
 * Code is originally from vitek and drac_gd
 * See http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=19005
 */
class Compass : public irr::gui::IGUIElement
{

public:

	Compass(const irr::core::rect<irr::s32>& rect, irr::gui::IGUIEnvironment* const env, irr::gui::IGUIElement* const parent);

	//! render the compass
	virtual void draw();

	//! set body of compass texture
	inline void SetCompassBodyTexture(irr::video::ITexture* const texture)
	{
	    BodyMesh.getMaterial().setTexture(0, texture);
	}

	//! set needle of compass texture
	inline void SetCompassNeedleTexture(irr::video::ITexture* const texture)
	{
	    NeedleMesh.getMaterial().setTexture(0, texture);
	}

	//! set compass heading
	inline void SetCompassHeading(const irr::f32 deg)
	{
	    Matrix.makeIdentity();
	    Matrix.setRotationDegrees(irr::core::vector3df(0.0f, 0.0f, deg));
	}

private:

	irr::scene::SMeshBuffer BodyMesh;

	irr::scene::SMeshBuffer NeedleMesh;

	irr::core::matrix4 Matrix;

	//! sets up a quad mesh of given dimension
	const void SetupQuadMesh(irr::scene::SMeshBuffer& mesh, const irr::f32 f32Width) const;
};

#endif /*COMPASS_H_*/
