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

#include "egParticle.h"
#include "egMaterial.h"
#include "egModules.h"
#include "utXMLParser.h"
#include "utPlatform.h"

#include "utDebug.h"


// *************************************************************************************************
// ParticleChannel
// *************************************************************************************************

ParticleChannel::ParticleChannel()
{
	reset();
}


void ParticleChannel::reset()
{
	startMin = 0; startMax = 0; endRate = 0;
}


bool ParticleChannel::parse( XMLNode &node )
{
	if( node.getAttribute( "startMin" ) == 0x0 ) return false;
	startMin = (float)atof( node.getAttribute( "startMin" ) );

	if( node.getAttribute( "startMax" ) != 0x0 )
		startMax = (float)atof( node.getAttribute( "startMax" ) );
	else
		startMax = startMin;
	
	endRate = (float)atof( node.getAttribute( "endRate", "1" ) );

	return true;
}


// *************************************************************************************************
// EffectResource
// *************************************************************************************************

EffectResource::EffectResource( const string &name, int flags ) :
	Resource( ResourceTypes::Effect, name, flags )
{
	initDefault();	
}


EffectResource::~EffectResource()
{
	release();
}


void EffectResource::initDefault()
{
	_lifeMin = 0; _lifeMax = 0;
	_moveVel.reset();
	_rotVel.reset();
	_size.reset();
	_colR.reset();
	_colG.reset();
	_colB.reset();
	_colA.reset();
}


void EffectResource::release()
{
}


bool EffectResource::raiseError( const string &msg, int line )
{
	// Reset
	release();
	initDefault();

	if( line < 0 )
		Modules::log().writeError( "Effect resource '%s': %s", _name.c_str(), msg.c_str() );
	else
		Modules::log().writeError( "Effect resource '%s' in line %i: %s", _name.c_str(), line, msg.c_str() );
	
	return false;
}


bool EffectResource::load( const char *data, int size )
{
	if( !Resource::load( data, size ) ) return false;
	if( data[size - 1] != '\0' )
	{	
		return raiseError( "Data block not NULL-terminated" );
	}

	// Parse effect
	XMLResults res;
	XMLNode rootNode = XMLNode::parseString( data, "ParticleConfig", &res );
	if( res.error != eXMLErrorNone )
	{
		return raiseError( XMLNode::getError( res.error ), res.nLine );
	}
	
	if( rootNode.getAttribute( "lifeMin" ) == 0x0 ) return raiseError( "Missing ParticleConfig attribute 'lifeMin'" );
	if( rootNode.getAttribute( "lifeMax" ) == 0x0 ) return raiseError( "Missing ParticleConfig attribute 'lifeMax'" );

	_lifeMin = (float)atof( rootNode.getAttribute( "lifeMin" ) );
	_lifeMax = (float)atof( rootNode.getAttribute( "lifeMax" ) );

	int nodeItr1 = 0;
	XMLNode node1 = rootNode.getChildNode( "ChannelOverLife", nodeItr1 );
	while( !node1.isEmpty() )
	{
		if( node1.getAttribute( "channel" ) != 0x0 )
		{
			if( _stricmp( node1.getAttribute( "channel" ), "moveVel" ) == 0 ) _moveVel.parse( node1 );
			else if( _stricmp( node1.getAttribute( "channel" ), "size" ) == 0 ) _size.parse( node1 );
			else if( _stricmp( node1.getAttribute( "channel" ), "rotVel" ) == 0 ) _rotVel.parse( node1 );
			else if( _stricmp( node1.getAttribute( "channel" ), "colR" ) == 0 ) _colR.parse( node1 );
			else if( _stricmp( node1.getAttribute( "channel" ), "colG" ) == 0 ) _colG.parse( node1 );
			else if( _stricmp( node1.getAttribute( "channel" ), "colB" ) == 0 ) _colB.parse( node1 );
			else if( _stricmp( node1.getAttribute( "channel" ), "colA" ) == 0 ) _colA.parse( node1 );
		}
		else
			return raiseError( "Missing ChannelOverLife attribute 'channel'" );
		
		node1 = rootNode.getChildNode( "ChannelOverLife", ++nodeItr1 );
	}
	
	return true;
}


float EffectResource::getParamf( int param )
{
	switch( param )
	{
	case EffectResParams::LifeMin:
		return _lifeMin;
	case EffectResParams::LifeMax:
		return _lifeMax;
	case EffectResParams::MoveVelMin:
		return _moveVel.startMin;
	case EffectResParams::MoveVelMax:
		return _moveVel.startMax;
	case EffectResParams::MoveVelEndRate:
		return _moveVel.endRate;
	case EffectResParams::RotVelMin:
		return _moveVel.startMin;
	case EffectResParams::RotVelMax:
		return _moveVel.startMax;
	case EffectResParams::RotVelEndRate:
		return _moveVel.endRate;
	case EffectResParams::SizeMin:
		return _size.startMin;
	case EffectResParams::SizeMax:
		return _size.startMax;
	case EffectResParams::SizeEndRate:
		return _size.endRate;
	case EffectResParams::Col_R_Min:
		return _colR.startMin;
	case EffectResParams::Col_R_Max:
		return _colR.startMax;
	case EffectResParams::Col_R_EndRate:
		return _colR.endRate;
	case EffectResParams::Col_G_Min:
		return _colG.startMin;
	case EffectResParams::Col_G_Max:
		return _colG.startMax;
	case EffectResParams::Col_G_EndRate:
		return _colG.endRate;
	case EffectResParams::Col_B_Min:
		return _colB.startMin;
	case EffectResParams::Col_B_Max:
		return _colB.startMax;
	case EffectResParams::Col_B_EndRate:
		return _colB.endRate;
	case EffectResParams::Col_A_Min:
		return _colA.startMin;
	case EffectResParams::Col_A_Max:
		return _colA.startMax;
	case EffectResParams::Col_A_EndRate:
		return _colA.endRate;
	default:
		return Resource::getParamf( param );
	}
}


bool EffectResource::setParamf( int param, float value )
{
	switch( param )
	{
	case EffectResParams::LifeMin:
		_lifeMin = value;
		return true;
	case EffectResParams::LifeMax:
		_lifeMax = value;
		return true;
	case EffectResParams::MoveVelMin:
		_moveVel.startMin = value;
		return true;
	case EffectResParams::MoveVelMax:
		_moveVel.startMax = value;
		return true;
	case EffectResParams::MoveVelEndRate:
		_moveVel.endRate = value;
		return true;
	case EffectResParams::RotVelMin:
		_rotVel.startMin = value;
		return true;
	case EffectResParams::RotVelMax:
		_rotVel.startMax = value;
		return true;
	case EffectResParams::RotVelEndRate:
		_rotVel.endRate = value;
		return true;
	case EffectResParams::SizeMin:
		_size.startMin = value;
		return true;
	case EffectResParams::SizeMax:
		_size.startMax = value;
		return true;
	case EffectResParams::SizeEndRate:
		_size.endRate = value;
		return true;
	case EffectResParams::Col_R_Min:
		_colR.startMin = value;
		return true;
	case EffectResParams::Col_R_Max:
		_colR.startMax = value;
		return true;
	case EffectResParams::Col_R_EndRate:
		_colR.endRate = value;
		return true;
	case EffectResParams::Col_G_Min:
		_colG.startMin = value;
		return true;
	case EffectResParams::Col_G_Max:
		_colG.startMax = value;
		return true;
	case EffectResParams::Col_G_EndRate:
		_colG.endRate = value;
		return true;
	case EffectResParams::Col_B_Min:
		_colB.startMin = value;
		return true;
	case EffectResParams::Col_B_Max:
		_colB.startMax = value;
		return true;
	case EffectResParams::Col_B_EndRate:
		_colB.endRate = value;
		return true;
	case EffectResParams::Col_A_Min:
		_colA.startMin = value;
		return true;
	case EffectResParams::Col_A_Max:
		_colA.startMax = value;
		return true;
	case EffectResParams::Col_A_EndRate:
		_colA.endRate = value;
		return true;
	default:
		return Resource::setParamf( param, value );
	}
}


// *************************************************************************************************
// EmitterNode
// *************************************************************************************************

EmitterNode::EmitterNode( const EmitterNodeTpl &emitterTpl ) :
	SceneNode( emitterTpl )
{
	_renderable = true;
	_materialRes = emitterTpl.matRes;
	_effectRes = emitterTpl.effectRes;
	_particleCount = emitterTpl.maxParticleCount;
	_respawnCount = emitterTpl.respawnCount;
	_delay = emitterTpl.delay;
	_emissionRate = emitterTpl.emissionRate;
	_spreadAngle = emitterTpl.spreadAngle;
	_force = Vec3f( emitterTpl.fx, emitterTpl.fy, emitterTpl.fz );

	_emissionAccum = 0;

	_particles = 0x0;
	_parPositions = 0x0;
	_parSizesANDRotations = 0x0;
	_parColors = 0x0;

	setMaxParticleCount( _particleCount );
}


EmitterNode::~EmitterNode()
{
	delete[] _particles;
	delete[] _parPositions;
	delete[] _parSizesANDRotations;
	delete[] _parColors;
}


SceneNodeTpl *EmitterNode::parsingFunc( map< string, string > &attribs )
{
	bool result = true;
	
	map< string, string >::iterator itr;
	EmitterNodeTpl *emitterTpl = new EmitterNodeTpl( "", 0x0, 0x0, 0, 0 );

	itr = attribs.find( "material" );
	if( itr != attribs.end() )
	{
		uint32 res = Modules::resMan().addResource( ResourceTypes::Material, itr->second, 0, false );
		if( res != 0 )
			emitterTpl->matRes = (MaterialResource *)Modules::resMan().resolveResHandle( res );
	}
	else result = false;
	itr = attribs.find( "effect" );
	if( itr != attribs.end() )
	{
		uint32 res = Modules::resMan().addResource( ResourceTypes::Effect, itr->second, 0, false );
		if( res != 0 )
			emitterTpl->effectRes = (EffectResource *)Modules::resMan().resolveResHandle( res );
	}
	else result = false;
	itr = attribs.find( "maxCount" );
	if( itr != attribs.end() ) emitterTpl->maxParticleCount = atoi( itr->second.c_str() );
	else result = false;
	itr = attribs.find( "respawnCount" );
	if( itr != attribs.end() ) emitterTpl->respawnCount = atoi( itr->second.c_str() );
	else result = false;
	itr = attribs.find( "delay" );
	if( itr != attribs.end() ) emitterTpl->delay = (float)atof( itr->second.c_str() );
	itr = attribs.find( "emissionRate" );
	if( itr != attribs.end() ) emitterTpl->emissionRate = (float)atof( itr->second.c_str() );
	itr = attribs.find( "spreadAngle" );
	if( itr != attribs.end() ) emitterTpl->spreadAngle = (float)atof( itr->second.c_str() );
	itr = attribs.find( "forceX" );
	if( itr != attribs.end() ) emitterTpl->fx = (float)atof( itr->second.c_str() );
	itr = attribs.find( "forceY" );
	if( itr != attribs.end() ) emitterTpl->fy = (float)atof( itr->second.c_str() );
	itr = attribs.find( "forceZ" );
	if( itr != attribs.end() ) emitterTpl->fz = (float)atof( itr->second.c_str() );
	
	if( !result )
	{
		delete emitterTpl; emitterTpl = 0x0;
	}
	
	return emitterTpl;
}


SceneNode *EmitterNode::factoryFunc( const SceneNodeTpl &nodeTpl )
{
	if( nodeTpl.type != SceneNodeTypes::Emitter ) return 0x0;

	return new EmitterNode( *(EmitterNodeTpl *)&nodeTpl );
}


void EmitterNode::setMaxParticleCount( uint32 maxParticleCount )
{
	// Delete particles
	delete[] _particles; _particles = 0x0;
	delete[] _parPositions; _parPositions = 0x0;
	delete[] _parSizesANDRotations; _parSizesANDRotations = 0x0;
	delete[] _parColors; _parColors = 0x0;
	
	// Initialize particles
	_particleCount = maxParticleCount;
	_particles = new ParticleData[_particleCount];
	_parPositions = new Vec3f[_particleCount];
	_parSizesANDRotations = new float[_particleCount * 2];
	_parColors = new float[_particleCount * 4];
	for( uint32 i = 0; i < _particleCount; ++i )
	{
		_particles[i].life = 0;
		_particles[i].respawnCounter = 0;
		
		_parPositions[i] = Vec3f( 0, 0, 0 );
		_parSizesANDRotations[i*2+0] = 0.0f;
		_parSizesANDRotations[i*2+1] = 0.0f;
		_parColors[i*4+0] = 0.0f;
		_parColors[i*4+1] = 0.0f;
		_parColors[i*4+2] = 0.0f;
		_parColors[i*4+3] = 0.0f;
	}
}


float EmitterNode::getParamf( int param )
{
	switch( param )
	{
	case EmitterNodeParams::Delay:
		return _delay;
	case EmitterNodeParams::EmissionRate:
		return _emissionRate;
	case EmitterNodeParams::SpreadAngle:
		return _spreadAngle;
	case EmitterNodeParams::ForceX:
		return _force.x;
	case EmitterNodeParams::ForceY:
		return _force.y;
	case EmitterNodeParams::ForceZ:
		return _force.z;
	default:
		return SceneNode::getParamf( param );
	}
}


bool EmitterNode::setParamf( int param, float value )
{
	switch( param )
	{
	case EmitterNodeParams::Delay:
		_delay = value;
		return true;
	case EmitterNodeParams::EmissionRate:
		_emissionRate = value;
		return true;
	case EmitterNodeParams::SpreadAngle:
		_spreadAngle = value;
		return true;
	case EmitterNodeParams::ForceX:
		_force.x = value;
		return true;
	case EmitterNodeParams::ForceY:
		_force.y = value;
		return true;
	case EmitterNodeParams::ForceZ:
		_force.z = value;
		return true;
	default:
		return SceneNode::setParamf( param, value );
	}
}


int EmitterNode::getParami( int param )
{
	switch( param )
	{
	case EmitterNodeParams::MaterialRes:
		if( _materialRes != 0x0 ) return _materialRes->getHandle();
		else return 0;
	case EmitterNodeParams::EffectRes:
		if( _effectRes != 0x0 ) return _effectRes->getHandle();
		else return 0;
	case EmitterNodeParams::MaxCount:
		return (int)_particleCount;
	case EmitterNodeParams::RespawnCount:
		return _respawnCount;
	default:
		return SceneNode::getParami( param );
	}
}


bool EmitterNode::setParami( int param, int value )
{
	Resource *res;
	
	switch( param )
	{
	case EmitterNodeParams::MaterialRes:
		res = Modules::resMan().resolveResHandle( value );
		if( res == 0x0 || res->getType() != ResourceTypes::Material )
		{	
			Modules::log().writeDebugInfo( "Invalid Material resource for Emitter node %i", _handle );
			return false;
		}
		_materialRes = (MaterialResource *)res;
		return true;
	case EmitterNodeParams::EffectRes:
		res = Modules::resMan().resolveResHandle( value );
		if( res == 0x0 || res->getType() != ResourceTypes::Effect )
		{	
			Modules::log().writeDebugInfo( "Invalid Effect resource for Emitter node %i", _handle );
			return false;
		}
		_effectRes = (EffectResource *)res;
		return true;
	case EmitterNodeParams::MaxCount:
		setMaxParticleCount( (uint32)value );
		return true;
	case EmitterNodeParams::RespawnCount:
		_respawnCount = value;
		return true;
	default:
		return SceneNode::setParami( param, value );
	}
}


float randomF( float min, float max )
{
	if ((int)(max * 100.0f) - (int)(min * 100.0f) == 0)
	{
		return min;
	}
	else
	{
		return (rand()%((int)(max * 100.0f) - (int)(min * 100.0f))) / 100.0f + min;
	}
}


void EmitterNode::advanceTime( float timeDelta )
{
	if( _effectRes == 0x0 ) return;
	
	Vec3f bBMin( Math::MaxFloat, Math::MaxFloat, Math::MaxFloat );
	Vec3f bBMax( -Math::MaxFloat, -Math::MaxFloat, -Math::MaxFloat );
	
	if( _delay <= 0 )
		_emissionAccum += _emissionRate * timeDelta;
	else
		_delay -= timeDelta;
	
	for( uint32 i = 0; i < _particleCount; ++i )
	{
		ParticleData &p = _particles[i];
		
		// Create particle
		if( p.life <= 0 && ((int)p.respawnCounter < _respawnCount || _respawnCount < 0) )
		{
			if( _emissionAccum >= 1.0f )
			{
				// Respawn
				p.maxLife = randomF( _effectRes->_lifeMin, _effectRes->_lifeMax );
				p.life = p.maxLife;
				float angle = degToRad( _spreadAngle / 2 );
				Matrix4f m = _absTrans;
				m.c[3][0] = 0; m.c[3][1] = 0; m.c[3][2] = 0;
				m.rotate( randomF( -angle, angle ), randomF( -angle, angle ), randomF( -angle, angle ) );
				p.dir = (m * Vec3f( 0, 0, -1 )).normalized();
				++p.respawnCounter;

				// Generate start values
				p.moveVel0 = randomF( _effectRes->_moveVel.startMin, _effectRes->_moveVel.startMax );
				p.rotVel0 = randomF( _effectRes->_rotVel.startMin, _effectRes->_rotVel.startMax );
				p.size0 = randomF( _effectRes->_size.startMin, _effectRes->_size.startMax );
				p.r0 = randomF( _effectRes->_colR.startMin, _effectRes->_colR.startMax );
				p.g0 = randomF( _effectRes->_colG.startMin, _effectRes->_colG.startMax );
				p.b0 = randomF( _effectRes->_colB.startMin, _effectRes->_colB.startMax );
				p.a0 = randomF( _effectRes->_colA.startMin, _effectRes->_colA.startMax );
				
				// Update arrays
				_parPositions[i] = Vec3f( _absTrans.c[3][0], _absTrans.c[3][1], _absTrans.c[3][2] );
				_parSizesANDRotations[i * 2 + 0] = p.size0;
				_parSizesANDRotations[i * 2 + 1] = randomF( 0, 360 );
				_parColors[i * 4 + 0] = p.r0;
				_parColors[i * 4 + 1] = p.g0;
				_parColors[i * 4 + 2] = p.b0;
				_parColors[i * 4 + 3] = p.a0;

				// Update emitter
				_emissionAccum -= 1;
				if( _emissionAccum < 0 ) _emissionAccum = 0;
			}
		}
		
		// Update particle
		if( p.life > 0 )
		{
			// Interpolate data
			float fac = 1.0f - (p.life / p.maxLife);
			
			float moveVel = p.moveVel0 * (1.0f + (_effectRes->_moveVel.endRate - 1.0f) * fac);
			float rotVel = p.rotVel0 * (1.0f + (_effectRes->_rotVel.endRate - 1.0f) * fac);
			_parSizesANDRotations[i * 2 + 0] = p.size0 * (1.0f + (_effectRes->_size.endRate - 1.0f) * fac);
			_parColors[i * 4 + 0] = p.r0 * (1.0f + (_effectRes->_colR.endRate - 1.0f) * fac);
			_parColors[i * 4 + 1] = p.g0 * (1.0f + (_effectRes->_colG.endRate - 1.0f) * fac);
			_parColors[i * 4 + 2] = p.b0 * (1.0f + (_effectRes->_colB.endRate - 1.0f) * fac);
			_parColors[i * 4 + 3] = p.a0 * (1.0f + (_effectRes->_colA.endRate - 1.0f) * fac);

			// Update particle position and rotation
			_parPositions[i] += p.dir * moveVel * timeDelta;
			_parPositions[i] += _force * timeDelta;
			_parSizesANDRotations[i * 2+ 1] +=  rotVel * timeDelta;

			// Decrease lifetime
			p.life -= timeDelta;
			
			// Check if particle is dying
			if( p.life <= 0 )
			{
				_parSizesANDRotations[i * 2 + 0] = 0.0f;
			}
		}

		// Update bounding box
		Vec3f &vertPos = _parPositions[i];
		if( vertPos.x < bBMin.x ) bBMin.x = vertPos.x;
		if( vertPos.y < bBMin.y ) bBMin.y = vertPos.y;
		if( vertPos.z < bBMin.z ) bBMin.z = vertPos.z;
		if( vertPos.x > bBMax.x ) bBMax.x = vertPos.x;
		if( vertPos.y > bBMax.y ) bBMax.y = vertPos.y;
		if( vertPos.z > bBMax.z ) bBMax.z = vertPos.z;
	}

	// Avoid zero box dimensions for planes
	if( bBMax.x - bBMin.x == 0 ) bBMax.x += 0.1f;
	if( bBMax.y - bBMin.y == 0 ) bBMax.y += 0.1f;
	if( bBMax.z - bBMin.z == 0 ) bBMax.z += 0.1f;
	
	_localBBox.getMinCoords() = bBMin;
	_localBBox.getMaxCoords() = bBMax;

	markDirty();
}


bool EmitterNode::hasFinished()
{
	if( _respawnCount < 0 ) return false;

	for( uint32 i = 0; i < _particleCount; ++i )
	{	
		if( _particles[i].life > 0 || (int)_particles[i].respawnCounter < _respawnCount )
		{
			return false;
		}
	}
	
	return true;
}

