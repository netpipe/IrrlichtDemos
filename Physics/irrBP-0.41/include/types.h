#ifndef CIrrBP_TYPES_H_
#define CIrrBP_TYPES_H_

//Please note that a number <= 0 is for internal use\deactivation only. So you must use it in your program.
	enum CIB_DFLAG
	{
	NO_DEACTIVATION = 0,
	ON_COLLIDE = 1,
	TIME = 2,
	TR_DEACTIV = 3,
	ON_COLLISION_RELEASE =4,
	};
	enum BODY_OR
	{
		AUTO = -1,
		X = 0,
		Y = 1,
		Z = 2
	};
	
	enum IBP_DEBUG_FLAGS
	{
		DBG_NoDebug=0,
		DBG_DrawWireframe = 1,
		DBG_DrawAabb=2,
		DBG_DrawFeaturesText=4,
		DBG_DrawContactPoints=8,
		DBG_NoDeactivation=16,
		DBG_NoHelpText = 32,
		DBG_DrawText=64,
		DBG_ProfileTimings = 128,
		DBG_EnableSatComparison = 256,
		DBG_DisableBulletLCP = 512,
		DBG_EnableCCD = 1024,
		DBG_DrawConstraints = (1 << 11),
		DBG_DrawConstraintLimits = (1 << 12),
		DBG_FastWireframe = (1<<13),
		DBG_MAX_DEBUG_DRAW_MODE
	};

	enum COLL_OBJ_TYPE
	{
		RIGID_BODY = 1,
		SOFT_BODY = 2
	};
	enum ACTIONINT_OBJ_TYPE
	{
		VEHICLE,
		CHARCONTROLLER,
	};
	enum RIGID_BODY_TYPE
	{
		BOX = 1,
		CAPSULE,
		CONE,
		CONVEXHULL,
		CYLINDER,
		SPHERE,
		TRIMESH,
		CAM
	};
	struct contactPoint
	{
		contactPoint()
		{
		body = NULL;
		triangle = NULL;
		}
		~contactPoint()
		{
			if(triangle)
				delete triangle;
		}
		//!Contact Exists
		bool contact;

		//!Point of contact
		irr::core::vector3df point;

		//! This pointer is only set by some routines to tell which body is colliding to.
		class CIrrBPCollisionObject * body;

		//!Triangle of impact (only set for bullet trimeshes)
		irr::core::triangle3df * triangle;
		
	};
#endif