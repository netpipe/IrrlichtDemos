// *************************************************************************************************
//
// Horde3D
//   Next-Generation Graphics Engine
// --------------------------------------
// Copyright (C) 2006-2008 Nicolas Schulz
//
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// *************************************************************************************************

#ifndef _egParticle_H_
#define _egParticle_H_

#include "egPrerequisites.h"
#include "utMath.h"
#include "egMaterial.h"
#include "egScene.h"

struct XMLNode;


struct EmitterNodeParams
{
	enum List
	{
		MaterialRes = 700,
		EffectRes,
		MaxCount,
		RespawnCount,
		Delay,
		EmissionRate,
		SpreadAngle,
		ForceX,
		ForceY,
		ForceZ
	};
};

struct EffectResParams
{
	enum List
	{
		LifeMin = 900,
		LifeMax,
		MoveVelMin,
		MoveVelMax,
		MoveVelEndRate,
		RotVelMin,
		RotVelMax,
		RotVelEndRate,
		SizeMin,
		SizeMax,
		SizeEndRate,
		Col_R_Min,
		Col_R_Max,
		Col_R_EndRate,
		Col_G_Min,
		Col_G_Max,
		Col_G_EndRate,
		Col_B_Min,
		Col_B_Max,
		Col_B_EndRate,
		Col_A_Min,
		Col_A_Max,
		Col_A_EndRate
	};
};

struct ParticleChannel
{
	float	startMin, startMax;
	float	endRate;

	ParticleChannel();
	void reset();
	bool parse( XMLNode &node );
};

class EffectResource : public Resource
{
private:

	float				_lifeMin, _lifeMax;
	ParticleChannel		_moveVel, _rotVel;
	ParticleChannel		_size;
	ParticleChannel		_colR, _colG, _colB, _colA;

	bool raiseError( const string &msg, int line = -1 );

public:
	
	static Resource *factoryFunc( const string &name, int flags )
		{ return new EffectResource( name, flags ); }
	
	EffectResource( const string &name, int flags );
	~EffectResource();
	
	void initDefault();
	void release();
	bool load( const char *data, int size );

	float getParamf( int param );
	bool setParamf( int param, float value );

	friend class EmitterNode;
};

typedef SmartResPtr< EffectResource > PEffectResource;


struct ParticleData
{
	float		life, maxLife;
	Vec3f		dir;
	uint32		respawnCounter;

	// Start values
	float		moveVel0, rotVel0;
	float		size0;
	float		r0, g0, b0, a0;
};

struct EmitterNodeTpl : public SceneNodeTpl
{
	PMaterialResource	matRes;
	PEffectResource		effectRes;
	uint32				maxParticleCount;
	int					respawnCount;
	float				delay, emissionRate, spreadAngle;
	float				fx, fy, fz;

	EmitterNodeTpl( const string &name, MaterialResource *materialRes, EffectResource *effectRes,
					uint32 maxParticleCount, int respawnCount) :
		SceneNodeTpl( SceneNodeTypes::Emitter, name ),
		matRes( materialRes ), effectRes( effectRes ), maxParticleCount( maxParticleCount ),
		respawnCount( respawnCount ), delay( 0 ), emissionRate( 0 ), spreadAngle( 0 ),
		fx( 0 ), fy( 0 ), fz( 0 )
	{
	}
};

class EmitterNode : public SceneNode
{
protected:

	BoundingBox			_localBBox;
	
	// Emitter params
	PMaterialResource	_materialRes;
	PEffectResource		_effectRes;
	uint32				_particleCount;
	int					_respawnCount;
	float				_delay, _emissionRate, _spreadAngle;
	Vec3f				_force;

	// Emitter data
	float				_emissionAccum;

	// Particle data
	ParticleData		*_particles;
	Vec3f				*_parPositions;
	float				*_parSizesANDRotations;
	float				*_parColors;

	vector< uint32 >	_occQueries;
	vector< uint32 >	_lastVisited;

	EmitterNode( const EmitterNodeTpl &emitterTpl );
	void setMaxParticleCount( uint32 maxParticleCount );

public:
	
	~EmitterNode();

	static SceneNodeTpl *parsingFunc( map< string, string > &attribs );
	static SceneNode *factoryFunc( const SceneNodeTpl &nodeTpl );

	BoundingBox *getLocalBBox() { return &_localBBox; }
	float getParamf( int param );
	bool setParamf( int param, float value );
	int getParami( int param );
	bool setParami( int param, int value );

	void advanceTime( float timeDelta );
	bool hasFinished();

	friend class Renderer;
};

#endif // _egParticle_H_

