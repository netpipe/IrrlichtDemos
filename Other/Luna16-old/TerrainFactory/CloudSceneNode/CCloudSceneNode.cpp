// This file was part of the Irrlicht Engine's particle system, Copyright (C) 2002-2005 Nikolaus Gebhardt
// Now it's a clouds scene node :) 
// by G Davidson - gaz@bitplane.net

//  Version 0.2

// Cloud scene nodes
// This cloud is the complex cloud with levels of detail
// To use, make more cloud generation functions!

#include "CCloudSceneNode.h"
#include <cstdlib>


namespace irr
{
namespace scene
{

// static variables shared between all nodes
//core::array<SCloudParticle>	  CCloudSceneNode::Particles; // Particle draw buffer
//core::array<video::S3DVertex>	CCloudSceneNode::Vertices; // 4 vertices per particle
//core::array<u16>				 CCloudSceneNode::Indices; // 6 indices per particle

//! constructor
CCloudSceneNode::CCloudSceneNode(ISceneNode* parent, ISceneManager* mgr, ITimer *devicetimer , s32 id, 
	 const core::vector3df& position = core::vector3d<f32>(0,0,0),  
	 const core::vector3df& rotation = core::vector3d<f32>(0,0,0), 
	 const core::vector3df& scale	= core::vector3d<f32>(1,1,1))
	: ISceneNode(parent, mgr, id, position, rotation, scale), 
	ZOrdering(true), IgnoreChildColor(false), MaxDepth(2), LOD(10), timer(devicetimer), SkyMoveFactor(1.0f)
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
	
	// set the material parameter and transparrent alpha channel
	Material.MaterialTypeParam = 0.01f;
	Material.ZWriteEnable = false;
	Material.Lighting = false;
	
	 
	#ifdef _DEBUG
	setDebugName("CCloudSceneNode");
	#endif
}


//! destructor
CCloudSceneNode::~CCloudSceneNode()
{

}


//! Adds new particle effector to the particle system.
void CCloudSceneNode::addAffector(IParticleAffector* affector)
{
	affector->grab();
	AffectorList.push_back(affector);
}



//! Removes all particle affectors in the particle system.
void CCloudSceneNode::removeAllAffectors()
{
	core::list<IParticleAffector*>::Iterator it = AffectorList.begin();
	while (it != AffectorList.end())
	{
		(*it)->drop();
		it = AffectorList.erase(it);
	}
}


//! Returns the material based on the zero based index i.
video::SMaterial& CCloudSceneNode::getMaterial(u32 i)
{
	return Material;
}



//! Returns amount of materials used by this scene node.
u32 CCloudSceneNode::getMaterialCount() const
{
	return 1;
}


//! turns z sorting on or off. 
//! speed increase for alpha blend clouds, or clouds that are one colour
void CCloudSceneNode::setZOrdering(bool zo) 
{
	ZOrdering = zo;
}

void CCloudSceneNode::setMaxTimePerFrame(u32 t) 
{
	MaxTimePerFrame = t;
}

//! decides whether child nodes are the same colour as their parents
void CCloudSceneNode::setIgnoreChildColor(bool b) 
{
	IgnoreChildColor = b;
}

//! sets the maximum detail depth to draw
void CCloudSceneNode::setMaxDepth(s16 maxdepth) 
{
	MaxDepth = maxdepth;
}

//! sets the level of detail ("distance/size" - scale of 1 to 10 works well)
void CCloudSceneNode::setLOD(f32 levelofdetail) 
{
	LOD = levelofdetail;
} 

void CCloudSceneNode::setSkyClouds(bool sky,f32 factor=0.0f)
{
	SkyClouds = sky;
	SkyMoveFactor = factor;
}

//! makes particle list for rendering/sorting
void CCloudSceneNode::makeParticles() 
{
	core::vector3df rot = getRotation();
	core::vector3df pos;

	core::vector3df campos = SceneManager->getActiveCamera()->getAbsolutePosition();
	core::vector3df camrot = SceneManager->getActiveCamera()->getRotation();
	u32			 farv   = int(SceneManager->getActiveCamera()->getFarValue() *10000);

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

	// make each particle
	for (i=0; i < ParticleCount; i++)
	{

		core::vector3df tmppos = ParticleData[i].pos;

		// rotate
		m.transformVect(tmppos);
		
		// move globally
		tmppos += pos;
	   
		// set distance from camera
		f32 td = (f32)campos.getDistanceFrom(tmppos);
		u32 tmpdist = (u32)(10000.0f * (f32)td);
		
		// level of detail culled?
		f32 lod = td / ParticleData[i].size.Width;
 
		 // cull if it's behind the camera
		bool cull=false;
			
		// far plane culled

		if (farv < tmpdist) 
			cull = true;
	
		
		if (lod < LOD*5 && !cull)
		{
			
			// copy particle (swap for mem copy?)
			Particles[c] = ParticleData[i];
			
			// set it up
			Particles[c].pos = tmppos;
			Particles[c].distance = tmpdist;
			
			// expand the box
			Box.addInternalPoint(Particles[c].pos);
			
			if (lod < LOD && Particles[c].children > 0 && c < CLOUD_PARTICLE_LIMIT) 
			{
				// we add child nodes
				depth = 0;
				c+= makeChildClouds(Particles[c],c+1,m,campos);
			}
		   
			if (Particles.size() < c+10)
			{
				// grow buffer
				Particles.set_used(Particles.size()+50);
			}
	
			// we added at least one
			c+=1;
		}
	}
	ParticlesToDraw = c;
	core::matrix4 absinv = AbsoluteTransformation;
	absinv.makeInverse();
	absinv.transformBox(Box);
}

s16 CCloudSceneNode::getCurrentCloudCount()
{
	return ParticlesToDraw;
}

// adds child clouds to the particle buffer
s16 CCloudSceneNode::makeChildClouds(SCloudParticle &parent,s16 start,core::matrix4 &m, core::vector3df &campos )
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
	
	for(count=0;count<parent.children;count++)
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
void CCloudSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
	{
		SceneManager->registerNodeForRendering(this);
		// make cloud particles

		makeParticles();
		
		lastcount = ParticlesToDraw;
		
		// order them if needed
		if (ZOrdering)
			sortParticles();
		
		// call iscenenode's pre-render
		ISceneNode::OnRegisterSceneNode();
	}
}


//! render
void CCloudSceneNode::render()
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
	view.normalize();

	core::vector3df horizontal = up.crossProduct(view);
	horizontal.normalize();

	core::vector3df vertical = horizontal.crossProduct(view);
	vertical.normalize();

	view *= -1.0f;

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
	for (u32 i=sp; i<ParticlesToDraw; i++)
	{
		SCloudParticle& particle = Particles[i];

		core::vector3df h = horizontal * 0.5f * particle.size.Width;
		core::vector3df v = vertical * 0.5f * particle.size.Height;	

		s32 idx = (i-sp)*4;
		
		//p[0+idx].Pos	= particle.pos + h + v;
		p[0+idx].Pos.X	= particle.pos.X + h.X + v.X;
		p[0+idx].Pos.Y	= particle.pos.Y + h.Y + v.Y;
		p[0+idx].Pos.Z	= particle.pos.Z + h.Z + v.Z;
		p[0+idx].Color	= particle.color;
		// p[0+idx].Color  = particle.color;
		p[0+idx].Normal.X = view.X;
		p[0+idx].Normal.Y = view.Y;
		p[0+idx].Normal.Z = view.Z;

		//p[1+idx].Pos	= particle.pos + h - v;
		p[1+idx].Pos.X	= particle.pos.X + h.X - v.X;
		p[1+idx].Pos.Y	= particle.pos.Y + h.Y - v.Y;
		p[1+idx].Pos.Z	= particle.pos.Z + h.Z - v.Z;
		//p[1+idx].Color  = particle.color;
		p[1+idx].Normal.X = view.X;
		p[1+idx].Normal.Y = view.Y;
		p[1+idx].Normal.Z = view.Z;

		//p[2+idx].Pos	= particle.pos - h - v;
		p[2+idx].Pos.X	= particle.pos.X - h.X - v.X;
		p[2+idx].Pos.Y	= particle.pos.Y - h.Y - v.Y;
		p[2+idx].Pos.Z	= particle.pos.Z - h.Z - v.Z;
		p[2+idx].Color  = particle.color;
		//p[2+idx].Normal = view;
		p[2+idx].Normal.X = view.X;
		p[2+idx].Normal.Y = view.Y;
		p[2+idx].Normal.Z = view.Z;

		// p[3+idx].Pos = particle.pos - h + v;
		p[3+idx].Pos.X	= particle.pos.X - h.X + v.X;
		p[3+idx].Pos.Y	= particle.pos.Y - h.Y + v.Y;
		p[3+idx].Pos.Z	= particle.pos.Z - h.Z + v.Z;
		p[3+idx].Color = particle.color;
		// p[3+idx].Normal = view;
		p[3+idx].Normal.X = view.X;
		p[3+idx].Normal.Y = view.Y;
		p[3+idx].Normal.Z = view.Z;
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
	
	//printf("drew %d in %d\n", lastcount,lasttime);

	// for debug purposes only:
	if (DebugDataVisible)
	{
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		video::SMaterial m;
		m.Lighting = false;
		driver->setMaterial(m);
		driver->draw3DBox(Box, video::SColor(0,255,255,255));
	}
}



//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CCloudSceneNode::getBoundingBox() const
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

void CCloudSceneNode::sortParticles()
{
	u32 i,j, increment;

	// because copying particles around in the shell sort was showing up as 99% of
	// all cpu time, we'll deal with a list of pointers instead.
	
	// init arrays
	if (tmppointers.size() < ParticlesToDraw) tmppointers.set_used(ParticlesToDraw);
	if (tmpclouds.size() < ParticlesToDraw) tmpclouds.set_used(ParticlesToDraw);
	
	SCloudParticle *temp = Particles.pointer();
	
	// copy pointers
	for (i=0;i<ParticlesToDraw;i++) 
		tmppointers[i] = temp +i;
	
	// sort the list (shell sort)

	SCloudParticle **tp = tmppointers.pointer();
	
	increment = 3;
	while (increment > 0)
	{
		for (i=0; i < ParticlesToDraw; i++)
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
	for (i=0;i<ParticlesToDraw;i++)
		tmpclouds[i] = tp[i][0];
	
	// memcopy back to particles
	memcpy( Particles.pointer(),tmpclouds.const_pointer() , sizeof(SCloudParticle)*ParticlesToDraw);
//	for (i=0;i<ParticlesToDraw;i++)
//		Particles[i] = tmpclouds[i];
}

/*

void CCloudSceneNode::makeRandomCloud(u32 count)
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

void CCloudSceneNode::makeRandomCloud(s16 count)
{
	// this sets the number of top level clouds
	ParticleCount = count;
	// ParticleData[<ParticleCount] = top level cloud data
	// ParticleData[>=ParticleCount] = child node data
	
	if ((s32)ParticleData.size() < count+50)
		ParticleData.set_used(count+50);
	
	u32 i;

	// make particles
	for (i=1; i<count+25; i++) 
	{
		f32 a,b,c;
		// random position
		if (i < count)  
		{
			a = rand() / 1.5 - (rand() / 1.5);
			c = rand() / 1.5 - (rand() / 1.5);			
			b = rand() / 50.0 - (rand() / 50.0);
		}
		else 
		{
			a = rand() / 10.0 - (rand() / 10.0);
			c = rand() / 10.0 - (rand() / 10.0);			
			b = rand() / 10.0 - (rand() / 10.0);
		}
		ParticleData[i].pos = core::vector3d<f32>(a,b,c);

		f32 f = rand() / 50; 
		if (f < 0) f = -f;
		
		// set the size
		ParticleData[i].size = core::dimension2d<f32>(2000+f,2000+f);
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
	i++;
	
	//2
	ParticleData[i].color = video::SColor(255,0,0,0);
	ParticleData[i].children = 0;
	ParticleData[i].firstchild = 50+4;
	ParticleData[i].childscale = cs;
	ParticleData[i].pos = core::vector3d<f32>(-d,0,e);
	ParticleData[i].size = core::dimension2d<f32>(f,f);
	i++;

	//3
	ParticleData[i].color = video::SColor(255,0,0,0);
	ParticleData[i].children = 4;
	ParticleData[i].firstchild = 50+4;
	ParticleData[i].childscale = cs;
	ParticleData[i].pos = core::vector3d<f32>(d,0,-e);
	ParticleData[i].size = core::dimension2d<f32>(f,f);
	i++;

	//4
	ParticleData[i].color = video::SColor(255,0,0,0);
	ParticleData[i].children = 4;
	ParticleData[i].firstchild = 50+4;
	ParticleData[i].childscale = cs;
	ParticleData[i].pos = core::vector3d<f32>(-d,0,-e);
	ParticleData[i].size = core::dimension2d<f32>(f,f);
	i++;

	d  = rand()/1500;
	e  = rand()/1500;

	//1
	ParticleData[i].color = video::SColor(255,0,0,0);
	ParticleData[i].children = 0;
	ParticleData[i].firstchild = 50;
	ParticleData[i].childscale = cs;
	ParticleData[i].pos = core::vector3d<f32>(d,e,0);
	ParticleData[i].size = core::dimension2d<f32>(f,f);
	i++;

	//2
	ParticleData[i].color = video::SColor(255,0,0,0);
	ParticleData[i].children = 0;
	ParticleData[i].firstchild = 50;
	ParticleData[i].childscale = cs;
	ParticleData[i].pos = core::vector3d<f32>(-d,e,0);
	ParticleData[i].size = core::dimension2d<f32>(f,f);
	i++;
		d  = rand()/1500;
	//3
	ParticleData[i].color = video::SColor(255,0,0,0);
	ParticleData[i].children = 4;
	ParticleData[i].firstchild = 50;
	ParticleData[i].childscale =cs;
	ParticleData[i].pos = core::vector3d<f32>(d,-e,0);
	ParticleData[i].size = core::dimension2d<f32>(f,f);
	i++;

	//4
	ParticleData[i].color = video::SColor(255,0,0,0);
	ParticleData[i].children = 4;
	ParticleData[i].firstchild = 50;
	ParticleData[i].childscale = cs;
	ParticleData[i].pos = core::vector3d<f32>(-d,-e,0);
	ParticleData[i].size = core::dimension2d<f32>(f,f);
	i++;
		 d  = rand()/1500;
		*/
}


void CCloudSceneNode::reallocateBuffers()
{
	if (ParticlesToDraw * 4 > Vertices.size() ||
		ParticlesToDraw * 6 > Indices.size())
	{
		s32 oldSize = Vertices.size();
		Vertices.set_used(ParticlesToDraw * 4);

		u32 i;

		// fill remaining vertices
		for (i=oldSize; i<Vertices.size(); i+=4)
		{
			Vertices[0+i].TCoords.set(0.0f, 0.0f);
			Vertices[1+i].TCoords.set(0.0f, 1.0f);
			Vertices[2+i].TCoords.set(1.0f, 1.0f);
			Vertices[3+i].TCoords.set(1.0f, 0.0f);
		}

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

