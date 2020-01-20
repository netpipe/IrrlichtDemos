
#include "Physics.h"
#include "MeshCollisionGeometry.h"
#include "BoxCollisionGeometry.h"
#include "SphereCollisionGeometry.h"
#include "StaticBody.h"
#include "DynamicBody.h"
#include "IMesh.h"
#include "CollisionMaterial.h"
#include "IWorld.h"
#include "ray_collision.h"

Physics::Physics(IWorld *lithaWorld)
{
	this->lithaWorld = lithaWorld;
	
	dInitODE();
	
	world = dWorldCreate();
	space = dHashSpaceCreate(0);
	perStepContactJointGroup = dJointGroupCreate(0);
	
	//dWorldSetContactSurfaceLayer(world,0.001);
	//dWorldSetCFM(world, 0.001);
	
	dWorldSetAutoDisableFlag(world, true);
	dWorldSetAutoDisableLinearThreshold(world, 0.05);
	dWorldSetAutoDisableAngularThreshold(world, 0.05);
	dWorldSetAutoDisableSteps(world, 10);
	dWorldSetAutoDisableTime(world, 0);
	
	SetMaxContacts(12);
	
	SetGravity( core::vector3df(0.0,-9.8,0.0) );
	
	// Create default material and an interaction with itself
	
	defaultMaterial = AddCollisionMaterial();
	
	// defaultInteraction is a member attribute. Default values are set
	// in CollisionMaterialInteraction constructor.
	SetCollisionMaterialInteraction(defaultMaterial,defaultMaterial, defaultInteraction);
}

Physics::~Physics()
{
	for (u32 i = 0; i < materials.size(); i ++)
		materials[i]->drop();
	
	dSpaceDestroy		(space);
	dWorldDestroy		(world);
	dJointGroupDestroy	(perStepContactJointGroup);
	
	dCloseODE();
}

void Physics::Step(f32 dt)
{
	dSpaceCollide(space, this, ODE_Callback);
	dWorldQuickStep(world, dt);
	dJointGroupEmpty(perStepContactJointGroup);
}

void Physics::SetMaxContacts(s32 amount)
{
	maxContactsPerCollision = amount;
}

void Physics::SetGravity(const core::vector3df &grav)
{
	dWorldSetGravity(world, grav.X,grav.Y,grav.Z);
}

ICollisionMaterial *Physics::AddCollisionMaterial()
{
	materials.push_back(new CollisionMaterial());
	return materials.back();
}

void Physics::SetCollisionMaterialInteraction(ICollisionMaterial *m1, ICollisionMaterial *m2,
		const CollisionMaterialInteraction &interaction)
{
	// Both orderings are the same!
	materialInteractions[m1][m2] = interaction;
	materialInteractions[m2][m1] = interaction;
}

ICollisionMaterial &Physics::GetDefaultCollisionMaterial()
{
	return *defaultMaterial;
}

CollisionMaterialInteraction *Physics::GetCollisionMaterialInteraction(ICollisionMaterial *m1,
		ICollisionMaterial *m2)
{
	if (materialInteractions.count(m1))
	{
		std::map<ICollisionMaterial *, CollisionMaterialInteraction> &sub = materialInteractions[m1];
		
		if (sub.count(m2))
			return &sub[m2];
	}
	
	return NULL;
}

IMeshCollisionGeometry *Physics::CreateMeshCollisionGeometry(IMesh *mesh)
{
	return new MeshCollisionGeometry(space, mesh);
}

IBoxCollisionGeometry *Physics::CreateBoxCollisionGeometry(const core::vector3df &size)
{
	return new BoxCollisionGeometry(space, size);
}

IBoxCollisionGeometry *Physics::CreateBoxCollisionGeometryFromBB(IMesh *mesh)
{
	return CreateBoxCollisionGeometry( mesh->GetBoundingBox().getExtent() );
}

ISphereCollisionGeometry *Physics::CreateSphereCollisionGeometry(f32 radius)
{
	return new SphereCollisionGeometry(space, radius);
}

ISphereCollisionGeometry *Physics::CreateSphereCollisionGeometryFromBB(IMesh *mesh, bool inside)
{
	core::vector3df bb = mesh->GetBoundingBox().getExtent();
	
	if (inside)
	{
		// Find the shortest side length
		f32 min = bb.X < bb.Y ? bb.X : bb.Y;
		min = min < bb.Z ? min : bb.Z;
		
		return CreateSphereCollisionGeometry(min/2.0);
	}
	else
	{
		return CreateSphereCollisionGeometry(bb.getLength()/2.0);
	}
}

IDynamicBody *Physics::AddDynamicBody()
{
	IDynamicBody *body = new DynamicBody(world);
	lithaWorld->AddTransformable(body);
	body->drop();
	return body;
}

IStaticBody *Physics::AddStaticBody()
{
	IStaticBody *body = new StaticBody();
	lithaWorld->AddTransformable(body);
	body->drop();
	return body;
}

void Physics::SetLayerCollisions(u32 layer1, u32 layer2, bool enabled)
{
	layerCollisions[layer1][layer2] = enabled;
	layerCollisions[layer2][layer1] = enabled;
}

bool Physics::GetLayerCollisions(u32 layer1, u32 layer2)
{
	if (layer1 == layer2)
		return true;
	else if (layerCollisions.count(layer1) && layerCollisions[layer1].count(layer2))
		return layerCollisions[layer1][layer2];
	else // collisions between all layers enabled by default
		return true;
}

bool Physics::RayCast(const core::line3df &ray, RayCollision *collisionResult, u32 layer)
{
	return RayCast(ray, collisionResult, NULL, NULL, layer);
}

bool Physics::RayCastIncluding(const core::line3df &ray, const Set<ICollisionGeometry *> &includingGeometry,
		RayCollision *collisionResult, u32 layer)
{
	return RayCast(ray, collisionResult, &includingGeometry, NULL, layer);
}

bool Physics::RayCastExcluding(const core::line3df &ray, const Set<ICollisionGeometry *> &excludingGeometry,
		RayCollision *collisionResult, u32 layer)
{
	return RayCast(ray, collisionResult, NULL, &excludingGeometry, layer);
}

bool Physics::RayCast(const core::line3df &ray, RayCollision *collisionResult,
		const Set<ICollisionGeometry *> *includingGeometry,
		const Set<ICollisionGeometry *> *excludingGeometry,
		u32 layer)
{
	// Collide ray
	const std::vector<RayCollision> &collisions = ray_cast(ray, space, includingGeometry, excludingGeometry, layer, this);
	
	// Find closest collision point, and set it as the result
	// If a result is desired...
	if (collisionResult && collisions.size())
	{
		const RayCollision *closest = &collisions[0];
		
		for (u32 i = 0; i < collisions.size(); i ++)
		{
			if (collisions[i].collision.depth < closest->collision.depth)
				closest = &collisions[i];
		}
		
		*collisionResult = *closest;
	}
	
	return collisions.size();
}

std::vector<RayCollision> Physics::RayCast(const core::line3df &ray, u32 layer)
{
	return ray_cast(ray, space, NULL, NULL, layer, this);
}

std::vector<RayCollision> Physics::RayCastIncluding(const core::line3df &ray,
		const Set<ICollisionGeometry *> &includingGeometry, u32 layer)
{
	return ray_cast(ray, space, &includingGeometry, NULL, layer, this);
}

std::vector<RayCollision> Physics::RayCastExcluding(const core::line3df &ray,
		const Set<ICollisionGeometry *> &excludingGeometry, u32 layer)
{
	return ray_cast(ray, space, NULL, &excludingGeometry, layer, this);
}

void Physics::ODE_GeomCollide(dGeomID o1, dGeomID o2)
{
	// Should both be geoms
	ASSERT( !dGeomIsSpace(o1) );
	ASSERT( !dGeomIsSpace(o2) );
	
	// Neither should be rays... (not supported yet anyway)
	ASSERT( dGeomGetClass(o1) != dRayClass );
	ASSERT( dGeomGetClass(o2) != dRayClass );
	
	// Get ICollisionGeometries.
	// all ODE geoms should have had one set.
	ICollisionGeometry *collisionGeom1 = (ICollisionGeometry *)dGeomGetData(o1);
	ICollisionGeometry *collisionGeom2 = (ICollisionGeometry *)dGeomGetData(o2);
	
	ASSERT( collisionGeom1 );
	ASSERT( collisionGeom2 );
	
	// Are collisions between the two geom's layers even enabled?
	if (!GetLayerCollisions(collisionGeom1->GetCollisionLayer(), collisionGeom2->GetCollisionLayer()))
		return;
	
	// Get bodies
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);
	
	// Prevent intersecting bodiless meshes from hogging resources.
	if (!b1 && !b2 && dGeomGetClass(o1) == dTriMeshClass && dGeomGetClass(o2) == dTriMeshClass)
		return;
	
	// Don't collide bodies which are joined together (excluding a contact joint)
	// Might be useful for rag dolls. Or might not.
	//if (b1 && b2 && dAreConnectedExcluding(b1, b2, dJointTypeContact)) return;
	
	// setup an array of available contact points
	dContact contact[maxContactsPerCollision];
	
	s32 numc = dCollide(o1, o2, maxContactsPerCollision, &contact[0].geom, sizeof(dContact));
	
	if (numc == 0)
		return;
	
	
	// Find materials and an interaction.
	// If a material is not set a default will be used.
	
	ICollisionMaterial *material1 = collisionGeom1->GetMaterial();
	ICollisionMaterial *material2 = collisionGeom2->GetMaterial();
	
	if (!material1)
		material1 = defaultMaterial;
	
	if (!material2)
		material2 = defaultMaterial;
	
	CollisionMaterialInteraction *interaction = GetCollisionMaterialInteraction(material1, material2);
	
	if (!interaction)
		interaction = &defaultInteraction;
	
	// Now create contact joints between bodies
	// (if one geom does not have a body then it is connected to the world.
	// e.g. for a terrain mesh)
	
	// But not for a ray
	// (this probably isn't needed as I assert for rays above)
	if ( (dGeomGetClass(o1) == dRayClass) || (dGeomGetClass(o2) == dRayClass) )
		return;
	
	dSurfaceParameters surface;
	
	// these are default settings for the ODE surface.
	// actual settings are taken from the interaction in the loop below.
	surface.mode		= dContactBounce | dContactSoftCFM;
	surface.mu			= 0;
	surface.bounce		= 0;
	surface.bounce_vel	= 0;
	surface.soft_erp	= 0;
	surface.soft_cfm	= 0;
	
	// for each contact, create an actual contact joint between two bodies
	// these will be removed after simulation stepping
	for (s32 i = 0; i < numc; i++)
	{
		// Is there a custom preparator?
		// (i.e. a bit of code that may alter the interaction settings depending on the collision)
		if (interaction->preparator)
		{
			// preparator may want details of the collision
			
			dContactGeom &geom = contact[i].geom;
			
			Collision collision;
			collision.pos = core::vector3df(geom.pos[0],geom.pos[1],geom.pos[2]);
			collision.normal = core::vector3df(geom.normal[0],geom.normal[1],geom.normal[2]);
			collision.depth = geom.depth;
			
			// preparator may modify the interaction
			// so we use a copy
			CollisionMaterialInteraction interactionTemp = *interaction;
			interaction->preparator->Prepare(interactionTemp, collision);
			
			// Set the ODE surface using this temporary interaction modified with Prepare.
			SetSurfaceFromInteraction(surface, interactionTemp);
		}
		else
		{
			// No "preparation" done, so just set the ODE surface from the original interaction.
			SetSurfaceFromInteraction(surface, *interaction);
		}
		
		contact[i].surface = surface;
		dJointID c = dJointCreateContact(world, perStepContactJointGroup, contact+i);
		dJointAttach(c, b1, b2);
	}
}

void Physics::SetSurfaceFromInteraction(dSurfaceParameters &surface, const CollisionMaterialInteraction &interaction)
{
	surface.mu = interaction.friction;
}

