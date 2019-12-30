//
// Flag Scene-Node for Irrlicht 1.7.1
//
// v 1.7.1a
// Not very precise, still a little rusty,
// but gets the job done
//
// Created by FMX for the Irrlicht community
// zlib licence
//


#ifndef _C_FLAG_SCENENODE_H_INCLUDED_
#define _C_FLAG_SCENENODE_H_INCLUDED_

#include "irrlicht.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;


class SimpleFlagNode : public scene::ISceneNode
{
	core::aabbox3d<f32> Box;
	video::SMaterial Material;

	array<u16>			Indices;
	array<S3DVertex>	Verts;

	u32					Polys;


	vector3df			Gravity;
	vector3df			NetForce;		// Vector representing Net of External Forces

	u32					Iterations;
	f32					SpringStiffness;

	struct CC_Spring
	{ public:

		u16				Node_1;				// ID of the Cloth Node this vert refers to
		u16				Node_2;				// ID of the Cloth Node this vert refers to
		float			Length;				// Current Length of Spring
		float			LengthIdle;			// Initial Length of Spring
	};
	u32					NumSprings;

	array<CC_Spring> 	SPRING; 			// storage for all the SPRINGS

	array<bool>			VertIsPinned;
	array<vector3df>	VertInitialPos;
	array<vector3df>	VertPosOld;

public:
//	f32		WindWave ;
	virtual void setForce	( const vector3df &newForce ) { NetForce  = newForce; }
	virtual void applyForce	( const vector3df &addForce ) { NetForce += addForce; }
	virtual void setGravity	( const vector3df &gravity )  { Gravity = gravity; }

	~SimpleFlagNode()
	{
		Indices.clear();
		Verts.clear();
		SPRING.clear();
		VertIsPinned.clear();
		VertInitialPos.clear();
		VertPosOld.clear();
	}

	SimpleFlagNode
		(
			const f32 &Height,	// height of flag
			const f32 &Length,	// length of flag
			const int &HeightTesselation, // tessaltion along height
			const int &LengthTesselation, // tessaltion along length

			SMaterial Mat,				// initial material properties

			const vector3df &gravity,	// initial gravity
			const u32		&iterations,
			const f32		&spring_stiffness,

			scene::ISceneNode* parent,
			scene::ISceneManager* mgr, s32 id
		)
		:
			scene::ISceneNode(parent, mgr, id),
			Iterations		( iterations ),
			SpringStiffness	( spring_stiffness ),
			Gravity			( gravity ),
			Material		( Mat )
	{
		NetForce = vector3df(0);

	// generate Flag mesh
		Verts.clear();

		VertIsPinned.clear();
		VertInitialPos.clear();
		VertPosOld.clear();

		Indices.clear();
		Polys = 0;

		NumSprings = 0;
  		SPRING.clear();

		Box.reset( vector3df(0) );

		f32 xD = Length / f32(LengthTesselation);
		f32 yD = Height / f32(HeightTesselation);

		f32 xUV = 1.f / f32(LengthTesselation);
		f32 yUV = 1.f / f32(HeightTesselation);

		for( int y = 0;  y < HeightTesselation+1;  y++ )
		{
			for( int x = 0;  x < LengthTesselation+1;  x++ )
			{
				S3DVertex v;
					v.Color	  = SColor( 255,  255, 255, 255 );
					v.Pos	  = vector3df( xD*x, yD*y, 0 );
					v.TCoords = vector2df( xUV*x, 1.f-yUV*y );
				Verts.push_back( v );

				if( x == 0 ){ VertIsPinned.push_back( true  ); }
				else		{ VertIsPinned.push_back( false ); }

				VertInitialPos.push_back( v.Pos );
				VertPosOld.push_back	( v.Pos );

				Box.addInternalPoint(v.Pos);
   			}
		}
		Box.repair();


		for( int y = 0;  y < HeightTesselation;  y++ )
		{
			for( int x = 0;  x < LengthTesselation;  x++ )
			{
				int iV[4];
					iV[0] = (y*(LengthTesselation+1))+x;
					iV[1] = iV[0]+1;
					iV[2] = iV[0]+LengthTesselation+1;
					iV[3] = iV[1]+LengthTesselation+1;

				if( iV[0] < Verts.size() )
				{
					Indices.push_back( iV[0] );
					Indices.push_back( iV[2] );
					Indices.push_back( iV[1] );

					Indices.push_back( iV[1] );
					Indices.push_back( iV[2] );
					Indices.push_back( iV[3] );

					Polys += 2;

				// Springs
					CC_Spring 	temp_SPRING;
						temp_SPRING.Node_1 = iV[0];
						temp_SPRING.Node_2 = iV[1];
					SPRING.push_back( temp_SPRING );
						temp_SPRING.Node_1 = iV[0];
						temp_SPRING.Node_2 = iV[2];
					SPRING.push_back( temp_SPRING );

					if( x == LengthTesselation - 1 )
					{
						temp_SPRING.Node_1 = iV[1];
						temp_SPRING.Node_2 = iV[3];
					SPRING.push_back( temp_SPRING );
					}

					if( y == HeightTesselation - 1 )
					{
						temp_SPRING.Node_1 = iV[2];
						temp_SPRING.Node_2 = iV[3];
					SPRING.push_back( temp_SPRING );
					}
				}
			}
		}

		NumSprings = SPRING.size();
	}

	virtual void OnRegisterSceneNode()
	{
		if (IsVisible) { SceneManager->registerNodeForRendering(this); }

		ISceneNode::OnRegisterSceneNode();
	}
f32	WindWave;
	virtual void OnAnimate(u32 timeMs)
	{

       if (IsVisible)
     //    UpdatePhysics(timeMs* 0.01f);
            UpdatePhysics( .0085f);

		if( 1 ) //dowind
		{
            		WindWave += 0.01f;
			if( WindWave > 1 ) { WindWave = 0; }

			vector3df Wind = vector3df(
							 (sin( WindWave * 3 ) + 0.5f) * 5,
							 (sin( WindWave ) * 0.5f) * 40,
							 (cos( WindWave * 3)) * 5
							 );

			// Apply the Force
			applyForce( Wind );
            }
	}
	virtual void render()
	{
	    if (IsVisible){
		if( !Verts.size() ) return;

		// Render
		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		driver->setMaterial(Material);
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		driver->drawVertexPrimitiveList(&Verts[0], Verts.size(), &Indices[0], Polys, video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);
	    }
	}

	virtual const core::aabbox3d<f32>& getBoundingBox() const
	{
		return Box;
	}

	virtual u32 getMaterialCount() const
	{
		return 1;
	}

	virtual video::SMaterial& getMaterial(u32 i)
	{
		return Material;
	}



	// UPDATE
	void UpdatePhysics( const f32 &deltaTime )
	{
		u32 i, n, s;

		matrix4 Mi = getAbsoluteTransformation();
		Mi.makeInverse();

	// rotate forces by nodes orientation
		vector3df Force = NetForce + Gravity;
		Mi.rotateVect(Force);
		Force *= getScale();

	// Apply Forces to every NODE, and update its Position
  		for( n = 0;  n < Verts.size();  n ++ )
  		{
			vector3df position = Verts[ n ].Pos;
    		if( !VertIsPinned[ n ] )	{ Verts[ n ].Pos += (position - VertPosOld[ n ]) * 0.99f + Force * deltaTime * deltaTime;  }
    		else						{ Verts[ n ].Pos = VertInitialPos[ n ]; }
    		VertPosOld[ n ] = position;
  		}
		NetForce = vector3df(0);

	// Adujust the NODEs depending on how the SPRINGs have changed
  		for( i = 0;  i < Iterations;  i ++ )
  		{
    		for( s = 0;  s < NumSprings;  s ++ )
    		{
				u16 vert_1 = SPRING[ s ].Node_1;
				u16 vert_2 = SPRING[ s ].Node_2;

      			vector3df	d = Verts[ vert_2 ].Pos - Verts[ vert_1 ].Pos;
				SPRING[ s ].Length = d.getLength();
				if( SPRING[ s ].Length < 0 ){ SPRING[ s ].Length = 0 - SPRING[ s ].Length; }

      			if( SPRING[ s ].Length > 0 )
      			{
					f32 vert_mod = (SPRING[ s ].Length - .9 ) / SPRING[ s ].Length;

		   			if( !VertIsPinned[ vert_1 ] )
		   			{
		   				if( !VertIsPinned[ vert_2 ] )
						{
							d *= vert_mod / SpringStiffness;
           					Verts[ vert_1 ].Pos += d;
           					Verts[ vert_2 ].Pos -= d;
						}
						else
						{ Verts[ vert_1 ].Pos += d * vert_mod; }
					}
					else if( !VertIsPinned[ vert_2 ] )
					{ Verts[ vert_2 ].Pos -= d * vert_mod; }

           		}
    		}
  		}

	}


};



#endif  // _C_FLAG_SCENENODE_H_INCLUDED_

