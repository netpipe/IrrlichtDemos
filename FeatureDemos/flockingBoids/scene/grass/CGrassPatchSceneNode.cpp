// Grass node by Gaz Davidson 2006
// created from Irrlicht's particle system node by Nikolaus Gebhardt

#include "CGrassPatchSceneNode.h"

namespace irr
{
namespace scene
{

//! constructor
CGrassPatchSceneNode::CGrassPatchSceneNode(
	ITerrainSceneNode* const terrain, ISceneManager* mgr, const s32 id, const core::vector3d<s32>& gridpos,
	const char* filepath, const video::IImage* const heightMap, const video::IImage* const colourMap,
	const video::IImage* const grassMap, scene::IWindGenerator* const windgen) :

	ISceneNode((ITerrainSceneNode*)terrain, mgr, -1),
	DrawDistanceSQ(GRASS_PATCH_SIZE*1.5*GRASS_PATCH_SIZE*1.5),
	MaxDensity(800),
	Terrain(terrain),
	TerrainHeightMap(heightMap),
	TerrainColourMap(colourMap),
	TerrainGrassMap(grassMap),
	WindGen(windgen),
	fpsLock(30),
	lastwindtime(0),
	redrawnextloop(true),
	lastdrawcount(0)
{

#ifdef _DEBUG
	setDebugName("CGrassPatchSceneNode");
#endif

	// set position
	setPosition(core::vector3df((f32)(gridpos.X*GRASS_PATCH_SIZE),0 , (f32)(gridpos.Z*GRASS_PATCH_SIZE)));

	setWindRes(5);

	// set default image count
	setImageCount(core::dimension2d<s32>(4, 2));

	// create grass
	Create();
}

//! destructor
CGrassPatchSceneNode::~CGrassPatchSceneNode()
{

}

void CGrassPatchSceneNode::setWindRes(const u32 newres)
{
	// set size of wind grid
	windGridRes = newres < 2 ? 2 : newres;
	WindGrid.set_used((windGridRes + 1)*(windGridRes + 1));
/*
	for (u32 i=0; i < WindGrid.size(); ++i)
	{
		WindGrid[i] = core::vector2df(0,0);
	}
*/
}

u32 CGrassPatchSceneNode::getWindRes() const { return windGridRes; }

video::SMaterial& CGrassPatchSceneNode::getMaterial(u32 i) { return Material; }

u32	CGrassPatchSceneNode::getMaterialCount() const { return 1; }

const core::dimension2d<s32>& CGrassPatchSceneNode::getImageCount() const { return imagecount; }

void CGrassPatchSceneNode::setImageCount(const core::dimension2d<s32>& ic)
{
	imagecount = ic;
	imagesize.Width = 1.0f/f32(imagecount.Width);
	imagesize.Height = 1.0f/f32(imagecount.Height);

	v1.set_used(imagecount.Width*imagecount.Height);
	v2.set_used(imagecount.Width*imagecount.Height);
	v3.set_used(imagecount.Width*imagecount.Height);
	v4.set_used(imagecount.Width*imagecount.Height);

	int x;
	for (x = 0; x < imagecount.Width; ++x)
	{
		int y;
		for (y = 0; y <imagecount.Height; ++y)
		{
			v1[(imagecount.Width*y) + x] = imagesize.Width*x;
			v2[(imagecount.Width*y) + x] = imagesize.Height*(y + 1);
			v3[(imagecount.Width*y) + x] = imagesize.Height*y;
			v4[(imagecount.Width*y) + x] = imagesize.Width*(x + 1);
		}
	}
}

void CGrassPatchSceneNode::setRefreshDelay(const u32 ms) { fpsLock = ms;  }

u32 CGrassPatchSceneNode::getRefreshDelay() const { return fpsLock; }

void CGrassPatchSceneNode::setDrawDistance(const f32 draw) { DrawDistanceSQ = draw*draw; }

f32 CGrassPatchSceneNode::getDrawDistance() const { return (f32)sqrt(DrawDistanceSQ); }

void CGrassPatchSceneNode::setMaxDensity(const u32 max) { MaxDensity = max;	}

u32 CGrassPatchSceneNode::getMaxDensity() const { return MaxDensity; }

// creates random grass,
bool CGrassPatchSceneNode::Create()
{
	srand((100 * gridpos.X) + gridpos.Z);
	// particle count
	s32 count = 3000; // lots. we dont need them all

	// create them
	Particles.set_used(count);

	Box.reset(0,0,0);
	core::matrix4 m;
	m.setRotationDegrees(Terrain->getRotation());
	m.setTranslation(Terrain->getAbsolutePosition());
	m.makeInverse();

	s32 i;
	for(i = 0; i < count; ++i)
	{
		// get random x and z axis
		f32 x = (rand()%(GRASS_PATCH_SIZE*10))/10.0f;
		f32 z = (rand()%(GRASS_PATCH_SIZE*10))/10.0f;

		x -= GRASS_PATCH_SIZE/2.0f;
		z -= GRASS_PATCH_SIZE/2.0f;

		Particles[i].pos.X = x;
		Particles[i].pos.Z = z;

		Particles[i].sprite.Width = rand()%imagecount.Width;

		if (i < 30)
			Particles[i].sprite.Height = 1 *(rand() % imagecount.Height); // usually grass, sometimes a flower
		else
			Particles[i].sprite.Height = 0;

		m.transformVect(Particles[i].pos);

		// patch position + particle position - terrain position = position in terrain
		const core::vector3df p = getPosition()  + Particles[i].pos; //  - Terrain->getPosition();

		core::dimension2d<f32> size;

		const core::vector3df xz(p.X/Terrain->getScale().X, 0.0f, p.Z/Terrain->getScale().Z);
		const s32 x1 = (s32)floorf(xz.X);
		const s32 z1 = (s32)floorf(xz.Z);

		f32 height;

		// drop blades that are outside the terrain
		if (x1 < 1 || z1 < 1 || x1 > TerrainHeightMap->getDimension().Width - 1 || z1 > TerrainHeightMap->getDimension().Height - 1 )
		{
			--count; --i;
			Particles.set_used(count);
			continue;
		}

		// drop blades that aren't so dense
		const video::SColor cDensity  = TerrainGrassMap->getPixel(TerrainGrassMap->getDimension().Width - x1, z1);
		if ((u32)(rand() % 255) > cDensity.getAlpha() || cDensity.getAlpha() < 1 )
		{
			--count; --i;
			Particles.set_used(count);
			continue;
		}

		height = Terrain->getHeight(p.X, p.Z);

		size = core::dimension2d<f32>((f32)(rand() % 50 + 20), 100.0f);
		size.Height *= (f32)cDensity.getBlue()/200.0f;

		Particles[i].flex = size.Height/120.0f;
		Particles[i].pos.Y = height + (size.Height*0.5f);
		Particles[i].color = TerrainColourMap->getPixel(TerrainColourMap->getDimension().Width - x1,z1);
		Particles[i].startColor = TerrainColourMap->getPixel(TerrainColourMap->getDimension().Width - x1,z1);

		Box.addInternalPoint(Particles[i].pos);

		const f32 rotation = (rand()%3600)/10.0f;

		core::matrix4 m2;
		m2.setRotationDegrees(core::vector3df(0.0f, rotation, 0.0f)); //rand() % 20,rotation,rand() % 20));
		core::vector3df dimensions(0.5f * size.Width, -0.5f * size.Height,0);
		m2.rotateVect(dimensions);

		const core::vector3df h = core::vector3df(dimensions.X,0.0f,dimensions.Z);
		const core::vector3df v = core::vector3df(0.0f,dimensions.Y,0.0f);

		Particles[i].points[0] = Particles[i].pos + core::vector3df(dimensions.X,dimensions.Y,dimensions.Z);
		Particles[i].points[1] = Particles[i].pos + core::vector3df(dimensions.X,-dimensions.Y,dimensions.Z);
		Particles[i].points[2] = Particles[i].pos - core::vector3df(dimensions.X,dimensions.Y,dimensions.Z);
		Particles[i].points[3] = Particles[i].pos - core::vector3df(dimensions.X,-dimensions.Y,dimensions.Z);
/*
		core::vector3d<f32> xz1(Particles[i].points[0].X/Terrain->getScale().X,0.0f,Particles[i].points[0].Z/Terrain->getScale().Z);
		x1 = (s32) xz1.X;
		z1 = (s32) xz1.Z;
		video::SColor c1 = TerrainHeightMap->getPixel(x1, z1);
		f32 height1 = c1.getBlue()* Terrain->getScale().Y;

		core::vector3d<f32> xz2(Particles[i].points[3].X/Terrain->getScale().X,0.0f,Particles[i].points[3].Z/Terrain->getScale().Z);
		x1 = (s32) xz2.X - 10;
		z1 = (s32) xz2.Z - 10;
		video::SColor c2 = TerrainHeightMap->getPixel(x1, z1);
		f32 height2 = c2.getBlue()* Terrain->getScale().Y;

		Particles[i].points[0].Y = Particles[i].pos.Y + height1;
		Particles[i].points[3].Y = Particles[i].pos.Y + height2;
*/

//		Particles[i].points[0].Y -= 30;
//		Particles[i].points[3].Y -= 30;
//		Particles[i].points[1].Y += 30;
//		Particles[i].points[2].Y += 30;
	}

	return true;
}

//! pre render
void CGrassPatchSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
	{
		if (Particles.size() != 0)
		{
			if ((Box.getCenter() + getPosition()).getDistanceFromSQ(
				SceneManager->getActiveCamera()->getPosition()) < DrawDistanceSQ*1.5)
			{
				SceneManager->registerNodeForRendering(this);
				ISceneNode::OnRegisterSceneNode();
			}
		}
	}
}

void CGrassPatchSceneNode::OnAnimate(u32 timeMs)
{
	//only change the grass if so many ms have passed since the last time we did it
	// at 0 = 140fps, at 30 = 320fps, at 3000 = 370fps. woot!
	if (lastwindtime < timeMs - fpsLock)
	{
		lastwindtime   = timeMs;
		redrawnextloop = true;

		// populate wind grid
		const f32 dist = (Box.getCenter() + getPosition()).getDistanceFromSQ(SceneManager->getActiveCamera()->getPosition()) + 1;

		if (WindGen && dist < DrawDistanceSQ*1.5)
		{
			u32 x;
			for (x = 0; x < windGridRes+1; ++x)
			{
				u32 z;
				for (z = 0; z < windGridRes+1; ++z)
				{
					WindGrid[x*windGridRes + z] = //core::vector2df(0,0);
						WindGen->getWind(core::vector3df(
								( (f32)(gridpos.X*GRASS_PATCH_SIZE) + (f32)x*((f32)GRASS_PATCH_SIZE/(f32)windGridRes) - (GRASS_PATCH_SIZE/2.0f) ),
								0.0f,
								( (f32)(gridpos.X*GRASS_PATCH_SIZE) + (f32)x*((f32)GRASS_PATCH_SIZE/(f32)windGridRes) - (GRASS_PATCH_SIZE/2.0f) )),
								timeMs);
				}
			}
		}
	}
	else
	{
		redrawnextloop = false;
	}

	ISceneNode::OnAnimate(timeMs);
}

		//! render
		void CGrassPatchSceneNode::render()
		{
			video::IVideoDriver* const driver = SceneManager->getVideoDriver();
			const ICameraSceneNode* const camera = SceneManager->getActiveCamera();

			if (!camera || !driver)
				return;

			if (!redrawnextloop)
			{
				// just draw the old buffer
				driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
				driver->setMaterial(Material);
				driver->drawIndexedTriangleList(
						Vertices.pointer(), lastdrawcount*4, Indices.pointer(), lastdrawcount*4);
			}
			else
			{
				// reallocate arrays, if they are too small
				reallocateBuffers();

				const core::vector3df& campos = camera->getAbsolutePosition();
				const core::aabbox3d<f32>& cbox = camera->getViewFrustum()->getBoundingBox();
				const core::vector3df& pos = getPosition();

				u32 drawcount = 0;
				u32 max = (Particles.size() < MaxDensity) ? Particles.size() : MaxDensity;

				// in far boxes we dont loop so deep
				// 175 to 255 fps
				const f32 d = getPosition().getDistanceFromSQ(campos)/(f32)(GRASS_PATCH_SIZE*GRASS_PATCH_SIZE);
				if (d > 1.0)
				{
					//printf("max = %d, %d\n", max, (int)(max/d));
					max = u32(f32(max)/d);
				}

				// i likes teh oldenpointer
				f32* const v1p = &v1[0];
				f32* const v2p = &v2[0];
				f32* const v3p = &v3[0];
				f32* const v4p = &v4[0];

				// make the matrix outside the main loop
				core::matrix4 m;

				// create particle vertex data
				u32 i;
				for (i = 0; i < max; ++i)
				{
					const s32 idx = drawcount*4;
					const SGrassParticle& particle = Particles[i];
					const core::vector3df gpos = particle.pos + pos;

					// bounding box check: 150fps to 175fps
					if (!cbox.isPointInside(gpos))
						continue;

					// distance checks: 145fps to 175fps
					const f32 dist = campos.getDistanceFromSQ(gpos);

					if (dist > DrawDistanceSQ)  // too far
					{
						continue;
					}
					else if (dist > DrawDistanceSQ * 0.5) // if we're fading in
					{
						const f32 i1 = f32(i)/f32(max);
						const f32 i2 = (dist/DrawDistanceSQ)/2;

						if (particle.sprite.Height) // first row is boring grass, the rest doesnt fade
						{
							//particle.color = video::SColor(255,255,255,255);
						}
						else
						{
							if ( i1 < i2)
							{
								continue;
							}
							else
							{
								// fade this particle
								//s32 c = s32(255.0f-f32(i2/i1)*255.0f);
							}
						}
					}
					else
					{
						// particle.color = video::SColor(255,255,255,255);
					}

					//
					// wind
					//

					const s32 igridsize = GRASS_PATCH_SIZE/(windGridRes-1);
					const s32 ihalfres  = (windGridRes-1)/2;

					// get wind modifier from wind grid
					const u32 xgrid = u32((particle.pos.X/(f32)igridsize) +(f32)ihalfres);
					const u32 zgrid = u32((particle.pos.Z/(f32)igridsize) +(f32)ihalfres);

					// get fraction to next grid pos
					const f32 xnext = particle.pos.X/((f32)GRASS_PATCH_SIZE/(f32)windGridRes)+ (windGridRes/2.0f) - xgrid;
					const f32 znext = particle.pos.Z/((f32)GRASS_PATCH_SIZE/(f32)windGridRes)+ (windGridRes/2.0f) - zgrid;

					// get wind positions
					const core::vector2df& wind1 = WindGrid[ xgrid   * windGridRes   +zgrid];
					const core::vector2df& wind2 = WindGrid[(xgrid+1)* windGridRes   +zgrid];
					const core::vector2df& wind3 = WindGrid[ xgrid   *(windGridRes+1)+zgrid];
					const core::vector2df& wind4 = WindGrid[(xgrid+1)*(windGridRes+1)+zgrid];

					core::vector2df wind2d = wind1*(
							1.0f-xnext)*(1.0f-znext) +
							wind2*xnext*(1.0f-znext) +
							wind3*(1.0f-xnext)*znext +
							wind4*xnext*znext;
					// flexibility of this grass strand
					wind2d *= particle.flex;

					// convert to 3d vector
					const core::vector3df wind(wind2d.X, 0.0f, wind2d.Y);

					// blade colour change along Y axis
					const video::SColor gcol(particle.color.getAlpha(),
							(s32)(particle.color.getRed()*0.8f),
							(s32)(particle.color.getGreen()*0.8f),
							(s32)(particle.color.getBlue()*0.8f));


					Vertices[0+idx].Pos = particle.points[0]; //particle.pos + core::vector3df(dimensions.X,dimensions.Y,dimensions.Z);
					Vertices[0+idx].Color = gcol; // particle.color;

					Vertices[1+idx].Pos = particle.points[1] + wind; //particle.pos + core::vector3df(dimensions.X,-dimensions.Y,dimensions.Z) + wind;
					Vertices[1+idx].Color = particle.color;

					Vertices[2+idx].Pos = particle.points[2] + wind; //particle.pos - core::vector3df(dimensions.X,dimensions.Y,dimensions.Z) + wind;
					Vertices[2+idx].Color = particle.color;

					Vertices[3+idx].Pos = particle.points[3]; //particle.pos - core::vector3df(dimensions.X,-dimensions.Y,dimensions.Z);
					Vertices[3+idx].Color = gcol ; //particle.color;

					const s32 arrpos = (imagecount.Width*particle.sprite.Height) + particle.sprite.Width;

					// set tcoords
					Vertices[0+idx].TCoords.set(v1p[arrpos], v2p[arrpos]);
					Vertices[1+idx].TCoords.set(v1p[arrpos], v3p[arrpos]);
					Vertices[2+idx].TCoords.set(v4p[arrpos], v3p[arrpos]);
					Vertices[3+idx].TCoords.set(v4p[arrpos], v2p[arrpos]);

					drawcount++;
				}

				driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
				driver->setMaterial(Material);
				driver->drawIndexedTriangleList(
						Vertices.pointer(), drawcount*4, Indices.pointer(), drawcount*4);
				lastdrawcount = drawcount;
			}

			// for debug purposes only:
			if (DebugDataVisible)
			{
				driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
				video::SMaterial m;
				m.Lighting = false;
				driver->setMaterial(m);
				driver->draw3DBox(Box, video::SColor(0,255,255,255));
				core::aabbox3d<f32> b2;
				b2.reset(core::vector3df(0,0,0));

				b2.addInternalPoint(Box.MaxEdge*0.01f);
				driver->draw3DBox(b2, video::SColor(0,255,255,255));
			}
		}

//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CGrassPatchSceneNode::getBoundingBox() const
{
	return Box;
}

void CGrassPatchSceneNode::reallocateBuffers()
{
	if (Particles.size() * 4 > Vertices.size() ||
			Particles.size() * 6 > Indices.size())
	{
		const s32 oldSize = Vertices.size();
		Vertices.set_used(Particles.size() * 4);

		// fill remaining vertices
		u32 i;
		for (i = oldSize; i < Vertices.size(); i += 4)
		{
			Vertices[0+i].Normal = core::vector3df(0.0f, 1.0f, 0.0f);
			Vertices[1+i].Normal = core::vector3df(0.0f, 1.0f, 0.0f);
			Vertices[2+i].Normal = core::vector3df(0.0f, 1.0f, 0.0f);
			Vertices[3+i].Normal = core::vector3df(0.0f, 1.0f, 0.0f);
		}

		// fill remaining indices
		const s32 oldIdxSize = Indices.size();
		s32 oldvertices = oldSize;
		Indices.set_used(Particles.size() * 12);

		for (i = oldIdxSize; i < Indices.size(); i+=12)
		{
			Indices[0+i] = 0 + oldvertices;
			Indices[1+i] = 2 + oldvertices;
			Indices[2+i] = 1 + oldvertices;
			Indices[3+i] = 0 + oldvertices;
			Indices[4+i] = 3 + oldvertices;
			Indices[5+i] = 2 + oldvertices;

			Indices[6+i] = 1 + oldvertices;
			Indices[7+i] = 2 + oldvertices;
			Indices[8+i] = 0 + oldvertices;
			Indices[9+i] = 2 + oldvertices;
			Indices[10+i] = 3 + oldvertices;
			Indices[11+i] = 0 + oldvertices;

			oldvertices += 4;
		}
	}
}

} // end namespace scene
} // end namespace irr
