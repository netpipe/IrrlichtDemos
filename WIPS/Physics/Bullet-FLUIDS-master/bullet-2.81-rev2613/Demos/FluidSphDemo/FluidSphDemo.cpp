/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include <stdio.h> 	//printf debugging

#include "FluidSphDemo.h"
#include "GlutStuff.h"

#include "btBulletDynamicsCommon.h"
#include "LinearMath/btRandom.h"		//GEN_rand(), GEN_RAND_MAX


//Link to e.g. 'BulletMultiThreaded.lib' if enabling this
//(must build Bullet with CMake to get BulletMultiThreaded library)
//#define ENABLE_MULTITHREADED_FLUID_SOLVER
#ifdef ENABLE_MULTITHREADED_FLUID_SOLVER
	#include "BulletMultiThreaded/btFluidSphSolverMultithreaded.h"
	const int NUM_THREADS = 4;
#endif //ENABLE_MULTITHREADED_FLUID_SOLVER

#include "BulletFluids/Sph/Experimental/btFluidSphSolverMultiphase.h"
#include "BulletFluids/Sph/Experimental/btFluidSphSolverPCISPH.h"

FluidSphDemo::FluidSphDemo()
{
	setTexturing(true);
	setShadows(true);
	setCameraDistance(50.0);

	m_fluidRenderMode = FRM_Points;
	m_maxFluidParticles = MIN_FLUID_PARTICLES;
	m_useFluidSolverOpenCL = false;

	m_fluidWorld = 0;
	m_fluidSolverCPU = 0;
	m_fluidSolverGPU = 0;
	
	m_screenSpaceRenderer = 0;
	
	initPhysics();
	initDemos();
}
FluidSphDemo::~FluidSphDemo() 
{
	if(m_screenSpaceRenderer)delete m_screenSpaceRenderer;

	exitDemos();
	exitPhysics(); 
}

void FluidSphDemo::initPhysics()
{
	//btFluidRigidCollisionConfiguration adds fluid collision algorithms
	m_collisionConfiguration = new btFluidRigidCollisionConfiguration();

	//'standard' Bullet configuration
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	m_broadphase = new btDbvtBroadphase();
	m_solver = new btSequentialImpulseConstraintSolver();
	
	//btFluidSphSolver determines how the particles move and interact with each other
#ifndef ENABLE_MULTITHREADED_FLUID_SOLVER
	m_fluidSolverCPU = new btFluidSphSolverDefault();						//Standard optimized CPU solver
	//m_fluidSolverCPU = new btFluidSphSolverPCISPH();						//Work in progress CPU solver; do not use.
#else
	m_fluidSolverCPU = new btFluidSphSolverMultithreaded(NUM_THREADS);		//Multithreaded CPU solver
#endif
	
	//m_fluidSolverCPU = new btFluidSphSolverMultiphase();					//Experimental, unoptimized solver with btFluidSph-btFluidSph interaction
		
#ifdef ENABLE_OPENCL_FLUID_SOLVER
	static OpenCLConfig configCL;
	m_fluidSolverGPU = new btFluidSphSolverOpenCL(configCL.m_context, configCL.m_commandQueue, configCL.m_device);
#endif

	m_dynamicsWorld = new btFluidRigidDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration, m_fluidSolverCPU);
	m_fluidWorld = static_cast<btFluidRigidDynamicsWorld*>(m_dynamicsWorld);
	
	m_fluidWorld->setGravity( btVector3(0.0, -9.8, 0.0) );

	//Create a very large static box as the ground
	//We can also use DemoApplication::localCreateRigidBody, but for clarity it is provided here:
	{
		btCollisionShape* groundShape = new btBoxShape( btVector3(50.0, 50.0, 50.0) );
		m_collisionShapes.push_back(groundShape);

		btScalar mass(0.f);

		//Rigid bodies are dynamic if and only if mass is non zero, otherwise static
		btVector3 localInertia(0,0,0);
		if(mass != 0.f) groundShape->calculateLocalInertia(mass,localInertia);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin( btVector3(0,-50,0) );
		
		//Using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		m_fluidWorld->addRigidBody(body);
	}
	
	//Create btFluidSph(s), which contain groups of particles
	{
		btFluidSph* fluid;
		
		fluid = new btFluidSph(m_fluidWorld->getGlobalParameters(), MIN_FLUID_PARTICLES);
		m_fluids.push_back(fluid);
		
		//Also create an emitter
		{
			btFluidEmitter* emitter = new btFluidEmitter();
			m_emitter = emitter;
			
			emitter->m_fluid = fluid;
			
			const btScalar OFFSET(2.0);
			emitter->m_positions.push_back( btVector3(0, 0, 0) );
			emitter->m_positions.push_back( btVector3(0, OFFSET, 0) );
			emitter->m_positions.push_back( btVector3(0, -OFFSET, 0) );
			emitter->m_positions.push_back( btVector3(OFFSET, 0, 0) );
			emitter->m_positions.push_back( btVector3(-OFFSET, 0, 0) );
			
			emitter->m_speed = 2.5f;
			
			emitter->m_center.setValue(10.f, 20.f, 10.f);
			emitter->m_rotation.setEuler(90, -45, 0 );
			
			m_fluidWorld->addSphEmitter(emitter);
		}
		
		fluid = new btFluidSph(m_fluidWorld->getGlobalParameters(), 0);
		{
			btFluidSphParametersLocal FL = fluid->getLocalParameters();
			FL.m_restDensity *= 3.0f;
			FL.m_sphParticleMass *= 3.0f;
			//FL.m_stiffness /= 3.0f;
			fluid->setLocalParameters(FL);
		}
		m_fluids.push_back(fluid);
		
		for(int i = 0; i < m_fluids.size(); ++i) 
		{
			const bool ENABLE_CCD = true;
			if(ENABLE_CCD) m_fluids[i]->setCcdMotionThreshold( m_fluids[i]->getLocalParameters().m_particleRadius );
		
			m_fluidWorld->addFluidSph(m_fluids[i]);
		}
		
	}
}
void FluidSphDemo::exitPhysics()
{
	//Cleanup in the reverse order of creation/initialization

	//Remove the rigidbodies from the dynamics world and delete them
	for(int i = m_fluidWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = m_fluidWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if( body && body->getMotionState() ) delete body->getMotionState();
		
		m_fluidWorld->removeCollisionObject(obj);	//Removes btCollisionObject, btRigidBody, and btFluidSph
		delete obj;
	}

	//Delete collision shapes
	for(int j = 0; j < m_collisionShapes.size(); j++)
	{
		btCollisionShape* shape = m_collisionShapes[j];
		delete shape;
	}
	m_collisionShapes.clear();

	//
	delete m_fluidWorld;
	delete m_solver;
	delete m_broadphase;
	delete m_dispatcher;
	delete m_collisionConfiguration;
	if(m_fluidSolverCPU) delete m_fluidSolverCPU;
	if(m_fluidSolverGPU) delete m_fluidSolverGPU;
	
	//
	m_fluids.clear();
	
}

void FluidSphDemo::clientMoveAndDisplay()
{
	btScalar secondsElapsed = getDeltaTimeMicroseconds() * 0.000001f;
	
	const btFluidSphParametersGlobal& FG = m_fluidWorld->getGlobalParameters();
	if(m_fluidWorld)
	{
		const bool USE_SYNCRONIZED_TIME_STEP = false;	//Default: Rigid bodies == 16ms, Sph particles == 3ms time step
		if(USE_SYNCRONIZED_TIME_STEP)
		{
			btScalar timeStep = m_fluidWorld->getGlobalParameters().m_timeStep;
			m_fluidWorld->stepSimulation(timeStep, 0, timeStep);
		}
		else m_fluidWorld->stepSimulation(secondsElapsed);
		
		if( m_demos.size() ) m_demos[m_currentDemoIndex]->stepSimulation(*m_fluidWorld, &m_fluids);
	}
	
	{
		static int counter = 0;
		if(++counter > 100)
		{
			counter = 0;
			
			for(int i = 0; i < m_fluidWorld->getNumFluidSph(); ++i)
				printf( "m_fluidWorld->getFluidSph(%d)->numParticles(): %d \n", i, m_fluidWorld->getFluidSph(i)->numParticles() );
		}
	}
		
	displayCallback();
}

void FluidSphDemo::displayCallback(void) 
{
	//BT_PROFILE() does not work correctly in this function;
	//timings are captured only when the camera is moving.
	//BT_PROFILE("FluidSphDemo::displayCallback()");

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	renderme();

	renderFluids();
	
	if(m_fluidWorld) m_fluidWorld->debugDrawWorld();		//Optional but useful: debug drawing to detect problems

	glFlush();
	swapBuffers();
}

GLuint generateSphereList(float radius)
{
	//Sphere generation code from FLUIDS v.2
	GLuint glSphereList = glGenLists(1);
	glNewList(glSphereList, GL_COMPILE);
		glBegin(GL_TRIANGLE_STRIP);
			for(float tilt = -90.0f; tilt <= 90.0f; tilt += 10.0f) 
			{
				for(float ang = 0.f; ang <= 360.0f; ang += 30.0f) 
				{
					const float DEGREES_TO_RADIANS = 3.141592f/180.0f;
					
					float ang_radians = ang * DEGREES_TO_RADIANS;
					float tilt_radians = tilt * DEGREES_TO_RADIANS;
					float tilt1_radians = (tilt + 10.0f) * DEGREES_TO_RADIANS;
				
					float x = sin(ang_radians) * cos(tilt_radians);
					float y = cos(ang_radians) * cos(tilt_radians);
					float z = sin(tilt_radians);
					float x1 = sin(ang_radians) * cos(tilt1_radians);
					float y1 = cos(ang_radians) * cos(tilt1_radians);
					float z1 = sin(tilt1_radians);
					
					glNormal3f(x, y, z);	glVertex3f(x*radius, y*radius, z*radius);		
					glNormal3f(x1, y1, z1);	glVertex3f(x1*radius, y1*radius, z1*radius);
				}
			}
		glEnd();
	glEndList();
	
	return glSphereList;
}
void drawSphere(GLuint glSphereList, const btVector3& position, float r, float g, float b)
{	
	glPushMatrix();
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
		//glColor4f(0.9f, 0.9f, 0.9f, 0.6f);
		
		glColor3f(r, g, b);
		glTranslatef( position.x(), position.y(), position.z() );
		glCallList(glSphereList);
	glPopMatrix();
}
void getFluidColors(bool drawFluidsWithMultipleColors, int fluidIndex, btFluidSph* fluid, int particleIndex, float* out_r, float* out_g, float* out_b)
{
	const float COLOR_R = 0.3f;
	const float COLOR_G = 0.7f;
	const float COLOR_B = 1.0f;

	if(!drawFluidsWithMultipleColors)
	{
		float brightness = fluid->getVelocity(particleIndex).length() * 2.0f;
		if(brightness < 0.f)brightness = 0.f;
		if(brightness > 1.f)brightness = 1.f;
		
		const float MIN_BRIGHTNESS(0.15f);
		brightness = brightness * (1.0f - MIN_BRIGHTNESS) + MIN_BRIGHTNESS;
		
		*out_r = COLOR_R * brightness;
		*out_g = COLOR_G * brightness;
		*out_b = COLOR_B * brightness;
	}
	else
	{
		*out_r = COLOR_R; 
		*out_g = COLOR_G;
		*out_b = COLOR_B;
		
		if(fluidIndex % 2)
		{
			*out_r = 1.0f - COLOR_R;
			*out_g = 1.0f - COLOR_G;
			*out_b = 1.0f - COLOR_B;
		}
	}
}
void FluidSphDemo::renderFluids()
{
	static bool areSpheresGenerated = false;
	static GLuint glSmallSphereList;
	static GLuint glMediumSphereList;
	static GLuint glLargeSphereList;
	if(!areSpheresGenerated)
	{
		const float PARTICLE_RADIUS = 1.0f;
	
		areSpheresGenerated = true;
		glSmallSphereList = generateSphereList(0.1f);
		glMediumSphereList = generateSphereList(PARTICLE_RADIUS * 0.3f);
		glLargeSphereList = generateSphereList(PARTICLE_RADIUS);
	}
	
	bool drawFluidsWithMultipleColors = m_demos[m_currentDemoIndex]->isMultiFluidDemo();
	
	if(m_fluidRenderMode != FRM_MarchingCubes && m_fluidRenderMode != FRM_ScreenSpace)
	{
		//BT_PROFILE("Draw fluids - spheres");
		
		GLuint glSphereList;
		switch(m_fluidRenderMode)
		{
			case FRM_LargeSpheres:
				glSphereList = glLargeSphereList;
				break;
			case FRM_MediumSpheres:
				glSphereList = glMediumSphereList;
				break;
			case FRM_Points:
			default:
				glSphereList = glSmallSphereList;
				break;
		}
			
		for(int i = 0; i < m_fluids.size(); ++i)
			for(int n = 0; n < m_fluids[i]->numParticles(); ++n)
			{
				float r, g, b;
				getFluidColors(drawFluidsWithMultipleColors, i, m_fluids[i], n, &r, &g, &b);
				
				drawSphere(glSphereList, m_fluids[i]->getPosition(n), r, g, b);
			}
	}
	else if(m_fluidRenderMode == FRM_ScreenSpace)
	{
		//BT_PROFILE("Draw fluids - screen space");
		
		if(m_screenSpaceRenderer)
		{
			if(m_ortho)
			{
				printf("Orthogonal rendering not implemented for ScreenSpaceFluidRendererGL.\n");
				return;
			}
			
			for(int i = 0; i < m_fluids.size(); ++i)
			{
				const btFluidSphParametersLocal& FL = m_fluids[i]->getLocalParameters();
				btScalar particleRadius = FL.m_particleRadius;
			
				float r = 0.5f;
				float g = 0.8f;
				float b = 1.0f;
				
				//Beer's law constants
				//Controls the darkening of the fluid's color based on its thickness
				//For a constant k, (k > 1) == darkens faster; (k < 1) == darkens slower; (k == 0) == disable
				float absorptionR = 2.5;	
				float absorptionG = 1.0;
				float absorptionB = 0.5;
	
				if(drawFluidsWithMultipleColors)
				{
					r = 0.3f; 
					g = 0.7f;
					b = 1.0f;
					if(i % 2)
					{
						r = 1.0f - r;
						g = 1.0f - g;
						b = 1.0f - b;
					}
					
					absorptionR = 1.0;
					absorptionG = 1.0;
					absorptionB = 1.0;
				}
				
				m_screenSpaceRenderer->render(m_fluids[i]->internalGetParticles().m_pos, particleRadius * 1.5f,
											  r, g, b, absorptionR, absorptionG, absorptionB);
			}
		}
	}
	else 	//(m_fluidRenderMode == FRM_MarchingCubes)
	{
		//BT_PROFILE("Draw fluids - marching cubes");
			
		const int CELLS_PER_EDGE = 32;
		static MarchingCubes* marchingCubes = 0;
		if(!marchingCubes) 
		{
			marchingCubes = new MarchingCubes;
			marchingCubes->initialize(CELLS_PER_EDGE);
		}
		
		for(int i = 0; i < m_fluids.size(); ++i)
		{
			marchingCubes->generateMesh(*m_fluids[i]);
			
			const btAlignedObjectArray<float>& vertices = marchingCubes->getTriangleVertices();
			if( vertices.size() )
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

				float r = 0.9f;
				float g = 0.9f;
				float b = 0.9f;
				if(drawFluidsWithMultipleColors)
				{
					r = 0.3f; 
					g = 0.7f;
					b = 1.0f;
					if(i % 2)
					{
						r = 1.0f - r;
						g = 1.0f - g;
						b = 1.0f - b;
					}
				}
				
				glEnableClientState(GL_VERTEX_ARRAY);
					glColor4f(r, g, b, 0.6f);
					glVertexPointer( 3, GL_FLOAT, 0, &vertices[0] );
					glDrawArrays( GL_TRIANGLES, 0, vertices.size()/3 );	
				glDisableClientState(GL_VERTEX_ARRAY);
			}
		}
	}
}

void FluidSphDemo::initDemos()
{
	m_demos.push_back( new Demo_DamBreak() );
	m_demos.push_back( new Demo_Drop() );
	m_demos.push_back( new Demo_EmitterAndAbsorber() );
	m_demos.push_back( new Demo_Levee() );
	m_demos.push_back( new Demo_Drain() );
	m_demos.push_back( new Demo_DynamicBox() );
	m_demos.push_back( new Demo_HollowBox() );
	m_demos.push_back( new Demo_Heightfield() );
	m_demos.push_back( new Demo_FluidToRigidBody() );
	m_demos.push_back( new Demo_WaveGenerator() );
	m_demos.push_back( new Demo_MultiFluid() );
	m_demos.push_back( new Demo_ManyParticles() );
	m_demos.push_back( new Demo_Column() );
	
	for(int i = 0; i < m_demos.size(); ++i) m_demos[i]->initialize(&m_collisionShapes);
	
	m_currentDemoIndex = 0;
	m_maxFluidParticles = 1024;
	startDemo(m_currentDemoIndex);
}
void FluidSphDemo::exitDemos()
{
	stopDemo(m_currentDemoIndex);

	for(int i = 0; i < m_demos.size(); ++i) 
	{
		if(m_demos[i])
		{
			m_demos[i]->deactivate();
			delete m_demos[i];
		}
	}
	m_demos.clear();
}

void FluidSphDemo::prevDemo()
{
	if(m_currentDemoIndex - 1 >= 0)
	{
		stopDemo(m_currentDemoIndex);
		startDemo(--m_currentDemoIndex);
	}
}
void FluidSphDemo::nextDemo()
{
	if(m_currentDemoIndex + 1 <= m_demos.size() - 1)
	{
		stopDemo(m_currentDemoIndex);
		startDemo(++m_currentDemoIndex);
	}
}

inline int emitParticle(btFluidSph* fluid, const btVector3& position, const btVector3& velocity)
{
	int index = fluid->addParticle(position);
	if( index != fluid->numParticles() ) fluid->setVelocity(index, velocity);
	else
	{
		index = ( fluid->numParticles() - 1 ) * GEN_rand() / GEN_RAND_MAX;		//Random index
	
		fluid->setPosition(index, position);
		fluid->setVelocity(index, velocity);
	}	
	
	return index;
}
void FluidSphDemo::keyboardCallback(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'q':
		{
			int currentRenderMode = static_cast<int>(m_fluidRenderMode);
			m_fluidRenderMode = static_cast<FluidRenderMode>(currentRenderMode + 1);
		
			if(m_fluidRenderMode > FRM_MarchingCubes)m_fluidRenderMode = FRM_Points;
			return;
		}
		
		case 'e':
			if(m_fluidSolverGPU)
			{
				m_useFluidSolverOpenCL = !m_useFluidSolverOpenCL;
				
				if(m_useFluidSolverOpenCL)m_fluidWorld->setFluidSolver(m_fluidSolverGPU);
				else m_fluidWorld->setFluidSolver(m_fluidSolverCPU);
			}
			return;
			
		case ' ':
			resetCurrentDemo();
			return;
		
		case '[':
			prevDemo();
			return;
			
		case ']':
			nextDemo();
			return;
			
		case 'n':
			if(m_maxFluidParticles/2 >= MIN_FLUID_PARTICLES) m_maxFluidParticles /= 2;
			resetCurrentDemo();
			return;
		case 'm':
			if(m_maxFluidParticles*2 <= MAX_FLUID_PARTICLES) m_maxFluidParticles *= 2;
			resetCurrentDemo();
			return;
			
		case '/':
			if( m_fluids.size() && m_fluids[0] )
			{
				const btScalar SPEED = 2.0f;
				btVector3 position = getCameraPosition();
				position.setY( position.y() - btScalar(5.0) );
				
				btVector3 direction = (getRayTo(x,y) - position).normalized();
				btVector3 velocity = direction * SPEED;
				
				const btVector3 X_AXIS(1, 0, 0);
				const btVector3 Y_AXIS(0, 1, 0);
				const btVector3 Z_AXIS(0, 0, 1);
				btQuaternion rotation = shortestArcQuat(Z_AXIS, direction);
				btVector3 up = quatRotate(rotation, Y_AXIS);
				btVector3 left = quatRotate(rotation, X_AXIS);
				
				const btScalar SPACING(2.5);
				btFluidSph *fluid = m_fluids[0];
				emitParticle(fluid, position, velocity);
				emitParticle(fluid, position + up*SPACING, velocity);
				emitParticle(fluid, position + -up*SPACING, velocity);
				emitParticle(fluid, position + left*SPACING, velocity);
				emitParticle(fluid, position + -left*SPACING, velocity);
			}
			return;
	}
	
	PlatformDemoApplication::keyboardCallback(key, x, y);
}

void FluidSphDemo::specialKeyboard(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_END:
		{
			int numObj = getDynamicsWorld()->getNumCollisionObjects();
			if(numObj)
			{
				btCollisionObject* obj = getDynamicsWorld()->getCollisionObjectArray()[numObj-1];
				if( btFluidSph::upcast(obj) ) return;	//Deleting btFluidSph will cause crashes in FluidSphDemo
				
				getDynamicsWorld()->removeCollisionObject(obj);
				
				btRigidBody* body = btRigidBody::upcast(obj);
				if(body && body->getMotionState()) delete body->getMotionState();
				delete obj;

			}
			return;
		}
	}
	
	PlatformDemoApplication::specialKeyboard(key, x, y);
}

void FluidSphDemo::setShootBoxShape()
{
	if (!m_shootBoxShape)
	{
		const btScalar BOX_DIMENSIONS = 1.5f;
	
		btBoxShape* box = new btBoxShape( btVector3(BOX_DIMENSIONS, BOX_DIMENSIONS, BOX_DIMENSIONS) );
		box->initializePolyhedralFeatures();
		m_shootBoxShape = box;
		
		//m_shootBoxShape = new btCapsuleShape( btScalar(3.5), btScalar(7.0) );
		//m_shootBoxShape = new btConeShape( btScalar(7.0), btScalar(14.0) );
		//m_shootBoxShape = new btCylinderShape( btVector3(5.0, 3.0, 5.0) );
	}
}

void FluidSphDemo::myinit()
{
	DemoApplication::myinit();
	
	//ScreenSpaceFluidRendererGL may initialize GLEW, which requires an existing OpenGL context
	if(!m_screenSpaceRenderer) m_screenSpaceRenderer = new ScreenSpaceFluidRendererGL(m_glutScreenWidth, m_glutScreenHeight);
}

void FluidSphDemo::reshape(int w, int h)
{
	DemoApplication::reshape(w, h);
	
	if(m_screenSpaceRenderer) 
	{
		m_screenSpaceRenderer->setWindowResolution(w, h);
		m_screenSpaceRenderer->setRenderingResolution(w, h);
	}
}

void FluidSphDemo::clientResetScene()
{
	exitPhysics();
	initPhysics();
}

