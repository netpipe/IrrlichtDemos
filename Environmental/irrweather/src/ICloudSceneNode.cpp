// This file was part of the Irrlicht Engine's particle system, Copyright (C) 2002-2005 Nikolaus Gebhardt
// Now it's a clouds scene node :)
// by G Davidson - gaz@bitplane.net

//  Version 0.2

// Cloud scene nodes
// This cloud is the complex cloud with levels of detail

////////////////////////////////////////////////////////////////////////
// This node has been vastly improved from an old                     //
// scene node (by Bitplane) for use in the irrWeatherManager library. //
////////////////////////////////////////////////////////////////////////
// Josiah Hartzell - fighterstw@hotmail.com                           //
////////////////////////////////////////////////////////////////////////

#include "ICloudSceneNode.h"
#include <cstdlib>
#include <IParticleAffector.h>
#include <ISceneManager.h>
#include <ICameraSceneNode.h>
#include <IVideoDriver.h>
#include <ITimer.h>


namespace irr
{
namespace scene
{

// static variables shared between all nodes
//core::array<SCloudParticle>	  ICloudSceneNode::Particles; // Particle draw buffer
//core::array<video::S3DVertex>	ICloudSceneNode::Vertices; // 4 vertices per particle
//core::array<u16>				 ICloudSceneNode::Indices; // 6 indices per particle

//! constructor
ICloudSceneNode::ICloudSceneNode(ICloudLayer* cloudLayer, ISceneNode* parent, ISceneManager* mgr, ITimer *devicetimer , s32 id,
	 const core::vector3df& position = core::vector3d<f32>(0,0,0),
	 const core::vector3df& rotation = core::vector3d<f32>(0,0,0),
	 const core::vector3df& scale	= core::vector3d<f32>(1,1,1),
	 f32 globParticleSize)
	: ISceneNode(parent, mgr, id, position, rotation, scale),
	CloudLayer(cloudLayer), ZOrdering(true), IgnoreChildColor(false), MaxDepth(2), LOD(10), timer(devicetimer),
	SkyMoveFactor(1.0f), DefaultColor(irr::video::SColor(255,255,255,255)), isLightningHost(false),
	lightningEndTime(0.0f), CurrentTime(0), LastImposterUpdateTime(0), LastCameraPosition(core::vector3df(0,0,0)),
	LastCameraTarget(core::vector3df(0,0,0)),
	FirstRun(false)

{
	setAutomaticCulling(scene::EAC_OFF);

	// create static buffers if required
	if (Particles.size() < CLOUD_PARTICLE_LIMIT)
	{
		Particles.set_used(CLOUD_PARTICLE_LIMIT);
		ParticlesToDraw =  CLOUD_PARTICLE_LIMIT;
		reallocateBuffers();
		ParticlesToDraw =  0;
	}

	setParticleTextureCoordinates();

	// set the material parameter and transparrent alpha channel
	Material.MaterialTypeParam = 0.01f;
	Material.ZWriteEnable = false;
	Material.Lighting = false;

	cloudParticleSize = globParticleSize;


	#ifdef _DEBUG
	setDebugName("ICloudSceneNode");
	#endif

    CoarseIndices[0] = 0;
    CoarseIndices[1] = 2;
    CoarseIndices[2] = 1;
    CoarseIndices[3] = 0;
    CoarseIndices[4] = 3;
    CoarseIndices[5] = 2;

    //CoarseVertices[0].TCoords.set(0.25f, 0.25f);
    CoarseVertices[0].Color = 0xffffffff;

    //CoarseVertices[1].TCoords.set(0.25f, 0.0f);
    CoarseVertices[1].Color = 0xffffffff;

    //CoarseVertices[2].TCoords.set(0.0f, 0.0f);
    CoarseVertices[2].Color = 0xffffffff;

    //CoarseVertices[3].TCoords.set(0.0f, 0.25f);
    CoarseVertices[3].Color = 0xffffffff;
}


//! destructor
ICloudSceneNode::~ICloudSceneNode()
{
}


//! Adds new particle effector to the particle system.
void ICloudSceneNode::addAffector(IParticleAffector* affector)
{
	affector->grab();
	AffectorList.push_back(affector);
}



//! Removes all particle affectors in the particle system.
void ICloudSceneNode::removeAllAffectors()
{
	core::list<IParticleAffector*>::Iterator it = AffectorList.begin();
	while (it != AffectorList.end())
	{
		(*it)->drop();
		it = AffectorList.erase(it);
	}
}


//! Returns the material based on the zero based index i.
video::SMaterial& ICloudSceneNode::getMaterial(u32 i)
{
	return Material;
}



//! Returns amount of materials used by this scene node.
u32 ICloudSceneNode::getMaterialCount() const
{
	return 1;
}


//! turns z sorting on or off.
//! speed increase for alpha blend clouds, or clouds that are one colour
void ICloudSceneNode::setZOrdering(bool zo)
{
	ZOrdering = zo;
}

void ICloudSceneNode::setMaxTimePerFrame(u32 t)
{
	MaxTimePerFrame = t;
}

//! decides whether child nodes are the same colour as their parents
void ICloudSceneNode::setIgnoreChildColor(bool b)
{
	IgnoreChildColor = b;
}

//! sets the maximum detail depth to draw
/*void ICloudSceneNode::setMaxDepth(s16 maxdepth)
{
	MaxDepth = maxdepth;
}*/

//! sets the level of detail ("distance/size" - scale of 1 to 10 works well)
/*void ICloudSceneNode::setLOD(f32 levelofdetail)
{
	LOD = levelofdetail;
}*/

void ICloudSceneNode::setSkyClouds(bool sky,f32 factor=0.0f)
{
	SkyClouds = sky;
	SkyMoveFactor = factor;
}

//! makes particle list for rendering/sorting
void ICloudSceneNode::makeParticles()
{
	core::vector3df rot = getRotation();
	core::vector3df pos;

	core::vector3df campos = SceneManager->getActiveCamera()->getAbsolutePosition();
	core::vector3df camrot = SceneManager->getActiveCamera()->getRotation();
	u32	farv = int(SceneManager->getActiveCamera()->getFarValue() * 10000);

	if (SkyClouds)
		pos = getAbsolutePosition()+campos-(campos*SkyMoveFactor);
	else
		pos = getAbsolutePosition();

	core::matrix4 m;
	core::matrix4 m2;
	s16 i,c=0;
	m.setInverseRotationDegrees(rot);
	m2.setInverseRotationDegrees(camrot);
	core::vector3df t;

	// reset bounding box
	Box.reset(getPosition());

	f32 largestParticleSize = 0.0f;

	// make each particle
	for (i=0; i < ParticleCount; ++i)
	{
		core::vector3df tmppos = ParticleData[i].pos;

		// rotate
		m.transformVect(tmppos);

		// move globally
		tmppos += pos;

		// set distance from camera
		f32 td = (f32)campos.getDistanceFrom(tmppos);
		u32 tmpdist = (u32)(1000.0f * (f32)td);

		// level of detail culled?
		f32 lod = td / ParticleData[i].size.Width;

		 // cull if it's behind the camera
		bool cull=false;

		// far plane culled

		if(farv < tmpdist)
			cull = true;

        /*if (lod < LOD*5 && !cull &&
		SceneManager->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(Particles[c].pos) !=
            core::position2di(-10000,-10000))*/


		if(!cull)
		{
			// copy particle (swap for mem copy?)
			Particles[c] = ParticleData[i];

			// set it up
			Particles[c].pos = tmppos;
			Particles[c].distance = tmpdist;

			f32 size =
			(Particles[c].size.Width > Particles[c].size.Height ? Particles[c].size.Width : Particles[c].size.Height) * 0.5f;

			if(size > largestParticleSize)
                largestParticleSize = size;

			// expand the box
			Box.addInternalPoint(Particles[c].pos);

			/*if (lod < LOD && Particles[c].children > 0 && c < CLOUD_PARTICLE_LIMIT)
			{
				// we add child nodes
				depth = 0;
				c+= makeChildClouds(Particles[c],c+1,m,campos);
			}*/

			if (Particles.size() < c+10)
			{
				// grow buffer
				Particles.set_used(Particles.size()+50);
			}

			// we added at least one
			c+=1;
		}
	}

	Box.MaxEdge.X += largestParticleSize;
	Box.MaxEdge.Y += largestParticleSize;
	Box.MaxEdge.Z += largestParticleSize;

	Box.MinEdge.X -= largestParticleSize;
	Box.MinEdge.Y -= largestParticleSize;
	Box.MinEdge.Z -= largestParticleSize;


	ParticlesToDraw = c;
	core::matrix4 absinv = AbsoluteTransformation;
	absinv.makeInverse();
	absinv.transformBox(Box);
}

s16 ICloudSceneNode::getCurrentParticleCount()
{
	return ParticlesToDraw;
}

// adds child clouds to the particle buffer
s16 ICloudSceneNode::makeChildClouds(SCloudParticle &parent,s16 start,core::matrix4 &m, core::vector3df &campos )
{
	depth+=1;

	if (depth>MaxDepth)
	{
		return (0);
	}
	if ((s32)Particles.size() < start+parent.children)
	{
		// grow buffer
		Particles.set_used(Particles.size()+50);

		// buffer too big?
		if (start+parent.children > CLOUD_PARTICLE_LIMIT)
		{
			return (0);
		}
	}
	s16 count;
	s16 c = start;

	for(count=0;count<parent.children;++count)
	{
		// copy
		Particles[c] = ParticleData[ parent.firstchild + count ];

		// scale
		Particles[c].pos.X *= parent.childscale/4;
		Particles[c].pos.Y *= parent.childscale/4;
		Particles[c].pos.Z *= parent.childscale/4;
		Particles[c].size.Width *= parent.childscale;
		Particles[c].size.Height *= parent.childscale;
		Particles[c].childscale *= parent.childscale;

		// same colour?
		if (IgnoreChildColor)
			Particles[c].color = parent.color;

		// rotate
		m.transformVect(Particles[c].pos);
		// position
		Particles[c].pos += parent.pos;

		// expand the box
		Box.addInternalPoint(Particles[c].pos);

		// set distance from camera
		f32 td = (f32)campos.getDistanceFrom(Particles[c].pos);
		Particles[c].distance = (td * 10000);

		// level of detail
		f32 lod = td / Particles[c].size.Width;
		if (lod < LOD && Particles[c].children > 0)
		{
			// we add child nodes
			c+= makeChildClouds(Particles[c],c+1,m,campos);
			depth-=1;
			// grow buffer?
			if (Particles.size() < c+Particles[c].children)
			{
				Particles.set_used(c+Particles[c].children);
			}

			if (c > CLOUD_PARTICLE_LIMIT)
			{
				return (0);
			}
		}
		// we added at least one
		c+=1;
	}
	return (c-start);
}
//! pre render event
void ICloudSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
	{
		SceneManager->registerNodeForRendering(this);
		// make cloud particles

		makeParticles();

		lastcount = ParticlesToDraw;

		// order them if needed
		//if (ZOrdering)
			//sortParticles();

		// call iscenenode's pre-render
		ISceneNode::OnRegisterSceneNode();
	}
}

void ICloudSceneNode::OnAnimate(u32 timeMs)
{
    if(isLightningHost)
    {
        if(timeMs >= lightningEndTime)
        {
            isLightningHost = false;
            lightningEndTime = 0.0f;
            getMaterial(0).AmbientColor = DefaultColor;
        }
    }

    CurrentTime = timeMs;

    ISceneNode::OnAnimate(timeMs);
}


//! render
void ICloudSceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	ICameraSceneNode* camera = SceneManager->getActiveCamera();

	if (!camera || !driver)
		return;

	// calculate vectors for letting particles look to camera
	core::vector3df campos = camera->getAbsolutePosition();
	core::vector3df target = camera->getTarget();
	core::vector3df up = camera->getUpVector();
	core::vector3df view = target - campos;
	bool cameraPositionChanged = FirstRun ? true : (LastCameraPosition.getDistanceFrom(campos) > CloudLayer->getCameraPositionThreshold());
	bool cameraTargetChanged = FirstRun ? true : (LastCameraTarget.getDistanceFrom(target) > CloudLayer->getCameraTargetThreshold());
	view.normalize();

	core::vector3df horizontal = up.crossProduct(view);
	horizontal.normalize();

	core::vector3df vertical = horizontal.crossProduct(view);
	vertical.normalize();

	view *= -1.0f;

    if(CloudLayer->getImposterEnabled())
    {
        if(CurrentTime > (LastImposterUpdateTime+CloudLayer->getImposterUpdateFrequency()))
        {
            if(getAbsolutePosition().getDistanceFrom(camera->getAbsolutePosition()) > CloudLayer->getImposterDistance())
            {
                if(cameraPositionChanged || cameraTargetChanged)
                {
                    horizontal *= 0.5f * (getBoundingBox().MaxEdge.X - getBoundingBox().MinEdge.X) + cloudParticleSize*0.5;
                    vertical *= 0.5f * (getBoundingBox().MaxEdge.Y - getBoundingBox().MinEdge.Y);

                    CoarseVertices[0].Pos = getAbsolutePosition() + horizontal + vertical;
                    CoarseVertices[1].Pos = getAbsolutePosition() + horizontal - vertical;
                    CoarseVertices[2].Pos = getAbsolutePosition() - horizontal - vertical;
                    CoarseVertices[3].Pos = getAbsolutePosition() - horizontal + vertical;

                    view *= -1.0f;

                    for (s32 i=0; i<4; ++i)
                      CoarseVertices[i].Normal = view;
                }


                core::matrix4 mat;
                driver->setTransform(video::ETS_WORLD, mat);

                //video::ITexture* originalTexture = getMaterial(0).getTexture(0);
                //getMaterial(0).setTexture(0, RenderTextureRight);
                driver->setMaterial(Material);
                //getMaterial(0).setTexture(0, originalTexture);

                driver->drawIndexedTriangleList(CoarseVertices, 4, CoarseIndices, 2);

                return;
            }

            LastImposterUpdateTime = CurrentTime;
        }
    }

	// reallocate arrays, if they are too small
	reallocateBuffers();

	u32 sp=0;

	// if there's more particles that what's allowed, cull the back ones
	if (ParticlesToDraw > CLOUD_PARTICLE_LIMIT)
	{
		sp = ParticlesToDraw - CLOUD_PARTICLE_LIMIT;
	}

	// lets not use niko's arrays, use a pointer instead
	video::S3DVertex *p = Vertices.pointer();

	// create particle vertex data
	for (u32 i=sp; i<ParticlesToDraw; ++i)
	{
		SCloudParticle& particle = Particles[i];

		core::vector3df h = horizontal * 0.5f * cloudParticleSize;
		core::vector3df v = vertical * 0.5f * cloudParticleSize;
		/*core::vector3df h = horizontal * 0.5f * particle.size.Width;
		core::vector3df v = vertical * 0.5f * particle.size.Height;*/

		s32 idx = (i-sp)*4;

        if(cameraPositionChanged || cameraTargetChanged)
        {
            //p[0+idx].Pos	= particle.pos + h + v;
            p[0+idx].Pos.X	= particle.pos.X + h.X + v.X;
            p[0+idx].Pos.Y	= particle.pos.Y + h.Y + v.Y;
            p[0+idx].Pos.Z	= particle.pos.Z + h.Z + v.Z;
            // p[0+idx].Color  = particle.color;
            p[0+idx].Normal.X = view.X;
            p[0+idx].Normal.Y = view.Y;
            p[0+idx].Normal.Z = view.Z;

            //p[1+idx].Pos	= particle.pos + h - v;
            p[1+idx].Pos.X	= particle.pos.X + h.X - v.X;
            p[1+idx].Pos.Y	= particle.pos.Y + h.Y - v.Y;
            p[1+idx].Pos.Z	= particle.pos.Z + h.Z - v.Z;
            p[1+idx].Normal.X = view.X;
            p[1+idx].Normal.Y = view.Y;
            p[1+idx].Normal.Z = view.Z;

            //p[2+idx].Pos	= particle.pos - h - v;
            p[2+idx].Pos.X	= particle.pos.X - h.X - v.X;
            p[2+idx].Pos.Y	= particle.pos.Y - h.Y - v.Y;
            p[2+idx].Pos.Z	= particle.pos.Z - h.Z - v.Z;
            //p[2+idx].Normal = view;
            p[2+idx].Normal.X = view.X;
            p[2+idx].Normal.Y = view.Y;
            p[2+idx].Normal.Z = view.Z;

            // p[3+idx].Pos = particle.pos - h + v;
            p[3+idx].Pos.X	= particle.pos.X - h.X + v.X;
            p[3+idx].Pos.Y	= particle.pos.Y - h.Y + v.Y;
            p[3+idx].Pos.Z	= particle.pos.Z - h.Z + v.Z;
            p[3+idx].Normal.X = view.X;
            p[3+idx].Normal.Y = view.Y;
            p[3+idx].Normal.Z = view.Z;
        }

        p[0+idx].Color  = video::SColor(0,0,0,0);
        p[1+idx].Color  = video::SColor(0,0,0,0);
        p[2+idx].Color  = video::SColor(0,0,0,0);
        p[3+idx].Color  = video::SColor(0,0,0,0);
	}

	core::matrix4 mat;
	driver->setTransform(video::ETS_WORLD, mat);

//	driver->draw3DBox( Box, video::SColor(0,255,255,255));

	driver->setMaterial(Material);

	// get the time
	u32 t = timer->getTime();

	// draw the mesh
	driver->drawIndexedTriangleList(Vertices.pointer(), (ParticlesToDraw-sp)*4,
		Indices.pointer(), (ParticlesToDraw-sp)*2);

	// get the time taken, so we can cull far clouds next loop
	lasttime  = timer->getTime()-t;
	lastcount = ParticlesToDraw;

    if(cameraPositionChanged)
        LastCameraPosition = campos;

    if(cameraTargetChanged)
        LastCameraTarget = target;

    if(FirstRun)
        FirstRun = false;

	//printf("drew %d in %d\n", lastcount,lasttime);

	// for debug purposes only:
	if(DebugDataVisible)
	{
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		video::SMaterial m;
		m.Lighting = false;
		driver->setMaterial(m);
		driver->draw3DBox(Box, video::SColor(0,255,0,0));
	}
}



//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& ICloudSceneNode::getBoundingBox() const
{
	return Box;
}


/*

	/// might come in handy this stuff...

	// run affectors
	core::list<IParticleAffector*>::Iterator ait;
	ait = AffectorList.begin();
		while (ait != AffectorList.end())
		{
			(*ait)->affect(now, Particles.pointer(), Particles.size());
			++ait;
		}

	Box.addInternalPoint(Particles[i].pos);

	if (ParticlesAreGlobal)
	{
		core::matrix4 absinv = AbsoluteTransformation;
		absinv.makeInverse();
		absinv.transformBox(Box);
	}
*/

void ICloudSceneNode::sortParticles()
{
	u32 i,j, increment;

	// because copying particles around in the shell sort was showing up as 99% of
	// all cpu time, we'll deal with a list of pointers instead.

	// init arrays
	if (tmppointers.size() < ParticlesToDraw) tmppointers.set_used(ParticlesToDraw);
	if (tmpclouds.size() < ParticlesToDraw) tmpclouds.set_used(ParticlesToDraw);

	SCloudParticle *temp = Particles.pointer();

	// copy pointers
	for (i=0;i<ParticlesToDraw;++i)
		tmppointers[i] = temp +i;

	// sort the list (shell sort)

	SCloudParticle **tp = tmppointers.pointer();

	increment = 3;
	while (increment > 0)
	{
		for (i=0; i < ParticlesToDraw; ++i)
		{
		  j = i;
		  temp = tp[i];
		  while ((j >= increment) && (tp[j-increment]->distance < temp->distance))
		  {
			tp[j] = tp[j - increment];
			j = j - increment;
		  }
		  tp[j] = temp;
		}
		if (increment/2 != 0)
		  increment = increment/2;
		else if (increment == 1)
		  increment = 0;
		else
		  increment = 1;
	  }

	// take list to temp buffer
	for (i=0;i<ParticlesToDraw;++i)
		tmpclouds[i] = tp[i][0];

	// memcopy back to particles
	memcpy( Particles.pointer(),tmpclouds.const_pointer() , sizeof(SCloudParticle)*ParticlesToDraw);
//	for (i=0;i<ParticlesToDraw;i++)
//		Particles[i] = tmpclouds[i];
}

/*

void ICloudSceneNode::makeRandomCloud(u32 count)
{
	// make lots of particles
	ParticleCount = count;
	ParticleData.set_used(count);
	Particles.set_used(count);

	u32 i;

	// make particles
	for (i=0; i<count; i+=1)
	{

		f32 a = rand() / 500 - (rand() / 500);
		f32 c = rand() / 500 - (rand() / 500);
		f32 b = rand() / 100 - (rand() / 100);
		b = 200-b;
		if (b <0.0f) b*=-1.0f;
		a = a*(b*0.01);
		c = c*(b*0.01);
		ParticleData[i].pos = core::vector3d<f32>(a,b,c);
		f32 f = b/4;
		ParticleData[i].size = core::dimension2d<f32>(f,f);
		// distance to dark spot
		int f2 = (1.0/rand())*255;
		f2+=1;
		ParticleData[i].color = video::SColor(255,100+(b/4),100+(b/4),100+(b/4));
	}

   // Particles[0].pos = getPosition() + core::vector3d<f32>(0,0,0);
   // Particles[0].size = core::dimension2d<f32>(10,10);
   // Particles[0].color = video::SColor(255,55,55,55);


	//Vertices.set_used(count*4);

	reallocateBuffers();
	/*
	// fix textures
	for (i=0; i<Vertices.size(); i+=4)
	{
		printf("mooo%d\n",i);
		Vertices[0+i].TCoords.set(0.0f, 0.0f);
		Vertices[1+i].TCoords.set(0.0f, 1.0f);
		Vertices[2+i].TCoords.set(1.0f, 1.0f);
		Vertices[3+i].TCoords.set(1.0f, 0.0f);
	}

	Indices.set_used(ParticlesToDraw * 6);
	/

	Box=core::aabbox3d<f32>(-1000,-1000,-1000,1000,1000,1000);

}
*/

void ICloudSceneNode::makeRandomCloud(s16 count)
{
	// this sets the number of top level clouds
	ParticleCount = count;
	// ParticleData[<ParticleCount] = top level cloud data
	// ParticleData[>=ParticleCount] = child node data

	if ((s32)ParticleData.size() < count+50)
		ParticleData.set_used(count+50);

	u32 i;

	// make particles
	for(i=0; i<count+25; ++i)
	{
		f32 a,b,c;
		// random position
		if (i < count)
		{
			a = rand() / 1.5 - (rand() / 1.5);
			c = rand() / 1.5 - (rand() / 1.5);
			b = rand() / 5.0 - (rand() / 5.0); // 50.0
		}
		else
		{
			a = rand() / 10.0 - (rand() / 10.0);
			c = rand() / 10.0 - (rand() / 10.0);
			b = rand() / 10.0 - (rand() / 10.0);
		}
		ParticleData[i].pos = core::vector3d<f32>(a,b,c);

		f32 f = rand() / 50 - 1;
		if (f < 0) f = -f;

		// set the size
		ParticleData[i].size = core::dimension2d<f32>(cloudParticleSize+f,cloudParticleSize+f);
		// distance to dark spot
		int f2 = int(rand())%190;
		f2+=1;
		ParticleData[i].color = video::SColor(255,255-f2,255-f2,255-f2);
		ParticleData[i].children = 0;
		if (rand()/500 > 20) ParticleData[i].children = int(rand()) %5;
		int j = int(rand()) %10;
		ParticleData[i].firstchild = count +j;
		ParticleData[i].childscale = 0.5;
	}

	// a recursive pattern here-
/*
	f32 f = 100.0;
	f32 cs = 0.5;
	f32 d  = rand()/1500;
	f32 e  = rand()/1500;
	//1
	ParticleData[i].color = video::SColor(255,0,0,0);
	ParticleData[i].children = 0;
	ParticleData[i].firstchild = 50+4;
	ParticleData[i].childscale = cs;
	ParticleData[i].pos = core::vector3d<f32>(d,0,e);
	ParticleData[i].size = core::dimension2d<f32>(f,f);
	++i;

	//2
	ParticleData[i].color = video::SColor(255,0,0,0);
	ParticleData[i].children = 0;
	ParticleData[i].firstchild = 50+4;
	ParticleData[i].childscale = cs;
	ParticleData[i].pos = core::vector3d<f32>(-d,0,e);
	ParticleData[i].size = core::dimension2d<f32>(f,f);
	++i;

	//3
	ParticleData[i].color = video::SColor(255,0,0,0);
	ParticleData[i].children = 4;
	ParticleData[i].firstchild = 50+4;
	ParticleData[i].childscale = cs;
	ParticleData[i].pos = core::vector3d<f32>(d,0,-e);
	ParticleData[i].size = core::dimension2d<f32>(f,f);
	++i;

	//4
	ParticleData[i].color = video::SColor(255,0,0,0);
	ParticleData[i].children = 4;
	ParticleData[i].firstchild = 50+4;
	ParticleData[i].childscale = cs;
	ParticleData[i].pos = core::vector3d<f32>(-d,0,-e);
	ParticleData[i].size = core::dimension2d<f32>(f,f);
	++i;

	d  = rand()/1500;
	e  = rand()/1500;

	//1
	ParticleData[i].color = video::SColor(255,0,0,0);
	ParticleData[i].children = 0;
	ParticleData[i].firstchild = 50;
	ParticleData[i].childscale = cs;
	ParticleData[i].pos = core::vector3d<f32>(d,e,0);
	ParticleData[i].size = core::dimension2d<f32>(f,f);
	++i;

	//2
	ParticleData[i].color = video::SColor(255,0,0,0);
	ParticleData[i].children = 0;
	ParticleData[i].firstchild = 50;
	ParticleData[i].childscale = cs;
	ParticleData[i].pos = core::vector3d<f32>(-d,e,0);
	ParticleData[i].size = core::dimension2d<f32>(f,f);
	++i;
		d  = rand()/1500;
	//3
	ParticleData[i].color = video::SColor(255,0,0,0);
	ParticleData[i].children = 4;
	ParticleData[i].firstchild = 50;
	ParticleData[i].childscale =cs;
	ParticleData[i].pos = core::vector3d<f32>(d,-e,0);
	ParticleData[i].size = core::dimension2d<f32>(f,f);
	++i;

	//4
	ParticleData[i].color = video::SColor(255,0,0,0);
	ParticleData[i].children = 4;
	ParticleData[i].firstchild = 50;
	ParticleData[i].childscale = cs;
	ParticleData[i].pos = core::vector3d<f32>(-d,-e,0);
	ParticleData[i].size = core::dimension2d<f32>(f,f);
	++i;
		 d  = rand()/1500;
		*/
}

void ICloudSceneNode::setScale(const irr::core::vector3df &scale)
{
    for (u32 i=0; i<ParticleData.size(); ++i)
    {
        ParticleData[i].pos *= scale;
    }
}

int ICloudSceneNode::rand2(int lim, int seed)
{
    static long a = seed; // could be made the seed value

    a = (a * 32719 + 3) % 32749;

    return ((a % lim) + 1);
}


void ICloudSceneNode::setParticleTextureCoordinates()
{
    // fill remaining vertices
    for (u32 i=0; i<Vertices.size(); i+=4)
    {
        /*Vertices[0+i].TCoords.set(0.0f, 0.0f);
        Vertices[1+i].TCoords.set(0.0f, 1.0f);
        Vertices[2+i].TCoords.set(1.0f, 1.0f);
        Vertices[3+i].TCoords.set(1.0f, 0.0f);*/
        //srand ( timer->getTime() );
        int column = rand2(4, timer->getTime());
        int row = rand2(4, timer->getTime());
        //int point = 2;

        //printf("ROW: %d, COLUMN: %d\n", row, column);

        /*Vertices[0+i].TCoords.set((column-1)*0.25f, 0.0f);
        Vertices[1+i].TCoords.set((column-1)*0.25f, 0.25f);
        Vertices[2+i].TCoords.set(0.25f*column, 0.25f);
        Vertices[3+i].TCoords.set(0.25f*column, 0.0f);*/

        Vertices[0+i].TCoords.set((column-1)*0.25f, (row-1)*0.25f);
        Vertices[1+i].TCoords.set((column-1)*0.25f, 0.25f*row);
        Vertices[2+i].TCoords.set(0.25f*column, 0.25f*row);
        Vertices[3+i].TCoords.set(0.25f*column, (row-1)*0.25f);
    }

        /*Vertices[0+i].TCoords.set(0.0f, 0.0f);
        Vertices[1+i].TCoords.set(0.0f, 1.0f);
        Vertices[2+i].TCoords.set(1.0f, 1.0f);
        Vertices[3+i].TCoords.set(1.0f, 0.0f);*/
        //srand ( timer->getTime() );
        int column = rand2(4, timer->getTime());
        int row = rand2(4, timer->getTime());
        //int point = 2;

        //printf("ROW: %d, COLUMN: %d\n", row, column);

        /*Vertices[0+i].TCoords.set((column-1)*0.25f, 0.0f);
        Vertices[1+i].TCoords.set((column-1)*0.25f, 0.25f);
        Vertices[2+i].TCoords.set(0.25f*column, 0.25f);
        Vertices[3+i].TCoords.set(0.25f*column, 0.0f);*/

        CoarseVertices[0].TCoords.set((column-1)*0.25f, (row-1)*0.25f);
        CoarseVertices[1].TCoords.set((column-1)*0.25f, 0.25f*row);
        CoarseVertices[2].TCoords.set(0.25f*column, 0.25f*row);
        CoarseVertices[3].TCoords.set(0.25f*column, (row-1)*0.25f);
}


void ICloudSceneNode::reallocateBuffers()
{
	if (ParticlesToDraw * 4 > Vertices.size() ||
		ParticlesToDraw * 6 > Indices.size())
	{

		s32 oldSize = Vertices.size();
		Vertices.set_used(ParticlesToDraw * 4);

		u32 i;

        /*if(firstRun)
        {
            // Do stuff on first run
            firstRun = false;
        }*/

		// fill remaining indices
		s32 oldIdxSize = Indices.size();
		s32 oldvertices = oldSize;
		Indices.set_used(ParticlesToDraw * 6);

		for (i=oldIdxSize; i<Indices.size(); i+=6)
		{
			Indices[0+i] = 0+oldvertices;
			Indices[1+i] = 2+oldvertices;
			Indices[2+i] = 1+oldvertices;
			Indices[3+i] = 0+oldvertices;
			Indices[4+i] = 3+oldvertices;
			Indices[5+i] = 2+oldvertices;
			oldvertices += 4;
		}
	}
}


} // end namespace scene
} // end namespace irr

