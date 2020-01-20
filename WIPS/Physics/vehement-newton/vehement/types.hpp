// Vehement (C) Steven Dorman 2008
#pragma once
#define	 VEHEMENT_TYPES_HPP

// using the ctypes namespace
namespace ctypes
{
	//! Structure which holds map details
	struct mapobj
	{
		char		file		[50];
		char		gfx			[50];
		char		heightmap	[50];
		char		texture		[50];
		char		detail		[50];
		int			iobj;
		
		vector3df			playerstart;
	};

	//! Structure which holds user detials
	struct player
	{
		char		name		[50];
		char		currentmap	[50];
		int			level;
		int			health;
		int			power;
		vector3df	position;
		vector3df	rotation;
	};

	//! Holds details of the loaded object
	struct object
	{
		bool		vegetation;
		bool		isStatic;
		bool		primitive;
		bool		texture;
		bool		alpha;
		bool		scripted;

		char		name[50];
		char		model[50];
		char		gfx[50];
		char		script[50];

		int			weight;

		vector3df	location;
		vector3df	rotation;
		vector3df	scale;
	};

	//! Holds the state of the objects position
	struct cpos
	{
		vector3df	location;
		vector3df	rotation;
	};

	//! struct which holds item detals
	struct item
	{
	public:
		//Details
		char		name[50];
		char		model[50];
		char		gfx[50];
		vector3df	location;
		vector3df	rotation;
		vector3df	scale;
	};

	//! Binds together irrlicht and the physics engine
	class entity 
	{
	public:
		//Is this entity static? //Use primitive?  // Weight
		bool		isStatic;
		bool		primitive;
		int			weight;

		//Irrlicht pointers
		ITerrainSceneNode		* terrain;
		ISceneNode				* node;
		IAnimatedMeshSceneNode  * mynode;
		ITriangleSelector		* selector;

		//Newton body data
		newton::IBody			*	body;
		newton::IMaterial		*	material;
		newton::SBodyFromNode		bData;

		//Create as a member of the physics world
		void Start(newton::IWorld *	p_world)
		{
			switch (isStatic)
			{
			case true:
				if (primitive==false)
					body = p_world->createBody(mynode,mynode->getMesh(),newton::EBT_TREE);
				else
					body = p_world->createBody(mynode,mynode->getMesh(),newton::EBT_PRIMITIVE_BOX);

				body->setMass(0);
				body->setFreeze(true);
				material = p_world->createMaterial();
				body->setMaterial(material);
				break;
			case false:
				if (primitive==false)
					body = p_world->createBody(mynode,mynode->getMesh(),newton::EBT_CONVEX_HULL);
				else
					body = p_world->createBody(mynode,mynode->getMesh(),newton::EBT_PRIMITIVE_BOX);
					
				if (weight!=0)
					body->setMass((f32)weight);

				material = p_world->createMaterial();
				body->setMaterial(material);
				body->addGravityForce();
				break;
			};
		}

	};


};