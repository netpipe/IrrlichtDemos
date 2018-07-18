/*
  Copyright (C) 2016 Santiago Alberto Navascués González

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

/**Cylindrical mapping

What is?
it is a texture projection  system which allows to read a texture as if it was wrapped into a cylinder (or a sphere)

Why cylindrical mapping?
the reflections on real time graphics have been many times subject to discussion about which is the best
approach. Cube maps, dual paraboloid maps, and spherical projections all share this objective, but with different results.

Cubemaps: require of special support to use 6 textures at a time. Their main advantage is that is very easy to create cubemaps and to
use them. Their main drawback, the cubemap is a special texture type, which may require diferent set ups from the regular textures.

Dual Paraboloid maps: They are 2 textures at a time. Their main advantage is that, for instance, to generate real time reflections
they only require 2 renders, instead of 6, if we used a cubemap approach, and only 2 regular textures, but this render isn't trivial,
in the sense that we need a special projection for the dual paraboloids to work, they aren't as much efficient as a cube map would
be, and use two texture slots, instead of one.

Spherical maps: It is a single texture of an image of a sphere centered on it, in which a spherical projection is applied. Their main
advantage is that it is a single standard texture map, their disadvantages are is that this texture map is bound to a single direction,
i.e. you can't change the reflection direction to get the reflections from another point of view, and it is even harder to sythethise
a spherical map, and also, involve somewhat complex maths, like square roots

The cylindrical projections, on the other hand, share some benefits of all the above methods, although, with some disadvantages as well,
eventhough, those can be minor if they're done with some careful set ups. They use a single standard texture, they don't have special
projection needs to render, or perhaps, these can be addressed with ease, and they aren't bound to any direction, so they can be
projected over world directions, not only the view one, and their maths are somewhat simple, as they require a single normalization
that is done in a 2D vector.

Eventhough it could be appropriate to call this approach "spherical projection" it is closer to a cylinder in the sense that the height
(the V coordinate of the texture maps) is calculated completely independent of the horizontal coordinate, being able to be
free in that meaning, and able to reach any height, athough, if it is clamped to the texture space, this can be used as a better
"spherical maps" projection, given they can point to any direction
*/

#ifndef __SPHEREMAP_GENERATOR_H_INCLUDED__
#define __SPHEREMAP_GENERATOR_H_INCLUDED__

#include <irrlicht.h>

namespace irr
{
namespace scene
{

class SpheremapGenerator
{
    irr::video::IImage* Skydome;
    irr::core::array< irr::video::IImage* > imagesCubemap;
    irr::scene::ISceneManager* smgr;
    irr::video::IVideoDriver * driver;
    irr::core::dimension2du skydomeRes;

public:
	SpheremapGenerator( irr::scene::ISceneManager * SceneManager , const irr::core::dimension2du skydomeResolution = irr::core::dimension2du( 2048, 2048 ) );

	~SpheremapGenerator()
	{

	}

	//Generates a skydome out of a cubemap
	void cubemap2skydome( irr::core::array< irr::video::ITexture* > cubemap,  const io::path & relativeRute = L"Skydome.png");

	irr::video::IImage* getSkydome(){ return Skydome; }

	void setSkyDomeResolution(const irr::core::dimension2du Resolution );

private:
	//Max operation
    inline irr::f32 maxi(const irr::f32 a, const irr::f32 b) { return (a >= b ? a : b); }

    //maps a 2D texture coordinate into a 3D vector
    irr::core::vector3df uv2xyz(const irr::core::vector2df inTexCoord);

    //maps a 3D vector into a texture
    irr::core::vector2df xyz2uv(const irr::core::vector3df vector);

    //reads a single pixel from a texture map
    irr::video::SColor readTexturemap(const irr::core::vector2df texCoord, irr::video::IImage* image);

    //reads a pixel from a cubemap. The images are a vector of 6 images
    irr::video::SColor readCubemap(const irr::core::vector3df vector, irr::core::array< irr::video::IImage* >& images);

};


} // end namespace scene
} // end namespace irr

#endif
