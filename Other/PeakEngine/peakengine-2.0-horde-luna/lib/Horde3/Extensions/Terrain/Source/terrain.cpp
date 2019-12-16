// *************************************************************************************************
//
// Horde3D Terrain Extension
// --------------------------------------------------------
// Copyright (C) 2006-2008 Nicolas Schulz and Volker Wiendl
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

#include "terrain.h"
#include "egModules.h"
#include "egMaterial.h"
#include "utOpenGL.h"


namespace Horde3DTerrain
{
	const char *vsTerrainDebugView =
		"uniform mat4 worldMat;\n"
		"uniform vec4 terBlockParams;\n"
		"attribute float terHeight;\n"
		"varying vec4 color;\n"
		"void main() {\n"
		"	color = gl_Color;\n"
		"	gl_Position = gl_ModelViewProjectionMatrix * worldMat *"
		"		vec4( gl_Vertex.x * terBlockParams.z + terBlockParams.x, terHeight, "
		"			  gl_Vertex.z * terBlockParams.z + terBlockParams.y, gl_Vertex.w );\n"
		"}";

	const char *fsTerrainDebugView =
		"varying vec4 color;\n"
		"void main() {\n"
		"	gl_FragColor = color;\n"
		"}\n";

	ShaderContext TerrainNode::debugViewShader;

	
	TerrainNode::TerrainNode( const TerrainNodeTpl &terrainTpl ) :
		SceneNode( terrainTpl ), _materialRes( terrainTpl.matRes ), _blockSize( terrainTpl.blockSize ),
		_skirtHeight( terrainTpl.skirtHeight ), _lodThreshold( 1.0f / terrainTpl.meshQuality ),
		_hmapSize( 0 ), _heightData( 0x0 ), _maxLevel( 0 ), _heightArray( 0x0 ), _vertexBuffer( 0 ),
		_indexBuffer( 0 )
	{
		_renderable = true;
		if( terrainTpl.hmapRes != 0x0 ) updateHeightData( *terrainTpl.hmapRes );
		
		// Ensure correct block size
		if( _hmapSize % (_blockSize - 1) != 0 )
			_blockSize = 17;

		recreateVertexBuffer();
		calcMaxLevel();
		createBlockTree();

		_localBBox.getMinCoords() = Vec3f( 0, 0, 0 );
		_localBBox.getMaxCoords() = Vec3f( 1, 1, 1 );
	}


	TerrainNode::~TerrainNode()
	{
		delete[] _heightData;
		delete[] _heightArray;
	}


	SceneNodeTpl *TerrainNode::parsingFunc( map< string, string > &attribs )
	{
		map< string, string >::iterator itr;
		TerrainNodeTpl *terrainTpl = new TerrainNodeTpl( "", 0x0, 0x0 );

		itr = attribs.find( "heightmap" );
		if( itr != attribs.end() )
		{
			uint32 res = Modules::resMan().addResource( 
				ResourceTypes::Texture2D, 
				itr->second, 
				ResourceFlags::NoTexCompression | ResourceFlags::NoTexMipmaps,
				false );
			if( res != 0 )
				terrainTpl->hmapRes = (Texture2DResource *)Modules::resMan().resolveResHandle( res );
		}
		itr = attribs.find( "material" );
		if( itr != attribs.end() )
		{
			uint32 res = Modules::resMan().addResource( ResourceTypes::Material, itr->second, 0, false );
			if( res != 0 )
				terrainTpl->matRes = (MaterialResource *)Modules::resMan().resolveResHandle( res );
		}
		itr = attribs.find( "meshQuality" );
		if ( itr != attribs.end() )
		{
			terrainTpl->meshQuality = (float)atof( itr->second.c_str() );
		}
		itr = attribs.find( "skirtHeight" );
		if ( itr != attribs.end() )
		{
			terrainTpl->skirtHeight = (float)atof( itr->second.c_str() );
		}
		itr = attribs.find( "blockSize" );
		if ( itr != attribs.end() )
		{
			terrainTpl->blockSize = atoi( itr->second.c_str() );
		}

		return terrainTpl;
	}


	SceneNode *TerrainNode::factoryFunc( const SceneNodeTpl &nodeTpl )
	{
		if( nodeTpl.type != SNT_TerrainNode ) return 0x0;
		
		return new TerrainNode( *(TerrainNodeTpl *)&nodeTpl );
	}


	void TerrainNode::drawTerrainBlock( TerrainNode *terrain, float minU, float minV, float maxU, float maxV,
										int level, float scale, const Vec3f &localCamPos, const Frustum *frust1,
										const Frustum *frust2, int uni_terBlockParams )
	{
		const float halfU = (minU + maxU) / 2.0f;
		const float halfV = (minV + maxV) / 2.0f;

		uint32 offset = 0;
		for( int i = 0; i < level; ++i ) offset += (1 << i) * (1 << i);
		
		const uint32 blockIndex =  offset + (int)(minV * (1 << level)) * (1 << level) + (int)(minU * (1 << level));
		BlockInfo &block = terrain->_blockTree[blockIndex];

		// Create AABB for block
		Vec3f bBMin( minU, block.minHeight - terrain->_skirtHeight, minV), bBMax( maxU, block.maxHeight, maxV );
		
		// Frustum culling
		BoundingBox bb;
		bb.getMinCoords() = terrain->_absTrans * bBMin;
		bb.getMaxCoords() = terrain->_absTrans * bBMax;
		if( frust1 != 0x0 && frust1->cullBox( bb ) ) return;
		if( frust2 != 0x0 && frust2->cullBox( bb ) ) return;

		// Determine level of detail
		float dist = maxf( nearestDistToAABB( localCamPos, bBMin, bBMax ), 0.00001f );	
		float p = block.geoError / dist;

		if( p < terrain->_lodThreshold || level == terrain->_maxLevel )
		{
			// Render terrain block
			if( uni_terBlockParams >= 0 )
				glUniform4f( uni_terBlockParams, minU, minV, scale, scale );  // Bias and scale
		
			const uint32 size = terrain->_blockSize + 2;
			const float invScale = 1.0f / ( terrain->_blockSize - 1 );
			
			for( uint32 v = 0; v < size; ++v )
			{	
				float t = (v - 1) * invScale;
				if( v == 0 ) t = 0.0f; else if( v == size - 1 ) t = 1.0f;	// Skirt
				
				for( uint32 u = 0; u < size; ++u )
				{
					float s = (u - 1) * invScale;
					if( u == 0 ) s = 0.0f; else if( u == size - 1 ) s = 1.0f;	// Skirt
					
					float *vertHeight = &terrain->_heightArray[v * size + u];
					const float newU = (s * scale + minU) * (terrain->_hmapSize) + 0.5f;
					const float newV = (t * scale + minV) * (terrain->_hmapSize) + 0.5f;
					uint32 index = ((int)newV * (terrain->_hmapSize + 1) + (int)newU );
					
					*vertHeight = terrain->_heightData[index] / 65535.0f;
					
					// Create skirt
					if( v == 0 || v == size - 1 || u == 0 || u == size - 1 )
					{
						// Skirt can be smaller when camera is near
						const float skirtHeight = terrain->_skirtHeight * dist;
						*vertHeight = maxf( *vertHeight - skirtHeight, 0 );
					}
				}
			}
			
			Modules::renderer().updateVertices( terrain->_heightArray, terrain->getVertexCount() * sizeof( float ) * 3,
				terrain->getVertexCount() * sizeof( float ), terrain->_vertexBuffer );
			glDrawElements( GL_TRIANGLE_STRIP, terrain->getIndexCount(), GL_UNSIGNED_SHORT, (char *)0 );
			Modules::renderer().incStat( EngineStats::BatchCount, 1 );
			Modules::renderer().incStat( EngineStats::TriCount, (terrain->_blockSize - 1) * (terrain->_blockSize - 1) * 2.0f );
		}
		else 
		{
			// Subdivide terrain block
			scale /= 2;
			
			Vec4f blocks[] =
			{
				Vec4f( minU, minV, halfU, halfV ),
				Vec4f( halfU, minV, maxU, halfV ),
				Vec4f( minU, halfV, halfU, maxV ),
				Vec4f( halfU, halfV, maxU, maxV )
			};
			
			// Sort blocks by distance from camera
			if( localCamPos.x > halfU )
			{
				std::swap( blocks[0], blocks[1] );
				std::swap( blocks[2], blocks[3] );
			}
			if( localCamPos.z > halfV )
			{
				std::swap( blocks[0], blocks[2] );
				std::swap( blocks[1], blocks[3] );
			}

			for( uint32 i = 0; i < 4; ++i )
			{
				drawTerrainBlock( terrain, blocks[i].x, blocks[i].y, blocks[i].z, blocks[i].w,
								  level + 1, scale, localCamPos, frust1, frust2, uni_terBlockParams );
			}
		}
	}
	
	
	void TerrainNode::renderFunc( const string &shaderContext, const string &theClass, bool debugView,
								  const Frustum *frust1, const Frustum *frust2, RenderingOrder::List order,
								  int occSet )
	{
		CameraNode *curCam = Modules::renderer().getCurCamera();
		if( curCam == 0x0 ) return;

		Modules::renderer().setMaterial( 0x0, "" );

		// Loop through terrain queue
		for( uint32 i = 0, s = (uint32)Modules::sceneMan().getRenderableQueue().size(); i < s; ++i )
		{
			if( Modules::sceneMan().getRenderableQueue()[i]->getType() != SNT_TerrainNode ) continue;
			
			TerrainNode *terrain = (TerrainNode *)Modules::sceneMan().getRenderableQueue()[i];
			
			if( !debugView )
			{
				if( !terrain->_materialRes->isOfClass( theClass ) ) continue;
				if( !Modules::renderer().setMaterial( terrain->_materialRes, shaderContext ) ) continue;
			}
			else
			{
				Modules::renderer().setShader( &debugViewShader );
			}
			
			int attrib_terHeight = glGetAttribLocation( Modules::renderer().getCurShader()->shaderObject, "terHeight" );
			if( attrib_terHeight < 0 ) continue;
			int uni_terBlockParams = glGetUniformLocation( Modules::renderer().getCurShader()->shaderObject, "terBlockParams" );

			Vec3f localCamPos( curCam->getAbsTrans().x[12], curCam->getAbsTrans().x[13], curCam->getAbsTrans().x[14] );
			localCamPos = terrain->_absTrans.inverted() * localCamPos;
			
			// Bind VBO
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->_indexBuffer );
			glBindBuffer( GL_ARRAY_BUFFER, terrain->_vertexBuffer );
			glVertexPointer( 3, GL_FLOAT, sizeof( float ) * 3, (char *)0 );
			glEnableClientState( GL_VERTEX_ARRAY );
			glVertexAttribPointer( attrib_terHeight, 1, GL_FLOAT, GL_FALSE, sizeof( float ),
								   (char *)0 + terrain->getVertexCount() * sizeof( float ) * 3 );
			glEnableVertexAttribArray( attrib_terHeight );
		
			// World transformation
			ShaderContext *curShader = Modules::renderer().getCurShader();
			if( curShader->uni_worldMat >= 0 )
			{
				glUniformMatrix4fv( curShader->uni_worldMat, 1, false, &terrain->_absTrans.x[0] );
			}
			if( curShader->uni_worldNormalMat >= 0 )
			{
				Matrix4f normalMat4 = terrain->_absTrans.inverted().transposed();
				float normalMat[9] = { normalMat4.x[0], normalMat4.x[1], normalMat4.x[2],
									   normalMat4.x[4], normalMat4.x[5], normalMat4.x[6],
									   normalMat4.x[8], normalMat4.x[9], normalMat4.x[10] };
				glUniformMatrix3fv( curShader->uni_worldNormalMat, 1, false, normalMat );
			}

			drawTerrainBlock( terrain, 0.0f, 0.0f, 1.0f, 1.0f, 0, 1.0f, localCamPos, frust1, frust2, uni_terBlockParams );

			glDisableClientState( GL_VERTEX_ARRAY );
			glDisableVertexAttribArray( attrib_terHeight );
		}
	}


	bool TerrainNode::canAttach( SceneNode &parent )
	{
		return true;
	}


	bool TerrainNode::updateHeightData( Texture2DResource &hmap )
	{
		delete[] _heightData; _heightData = 0x0;

		if( hmap.getComps() == 4 && !hmap.isHDR() && hmap.getWidth() == hmap.getHeight() &&
			(hmap.getWidth() == 32 || hmap.getWidth() == 64 || hmap.getWidth() == 128 ||
			hmap.getWidth() == 256 || hmap.getWidth() == 512 || hmap.getWidth() == 1024 ||
			hmap.getWidth() == 2048 || hmap.getWidth() == 4096 || hmap.getWidth() == 8192) )
		{
			_hmapSize = hmap.getWidth();
			_heightData = new unsigned short[(_hmapSize + 1) * (_hmapSize + 1)];
			
			float *pixels = hmap.downloadImageData();
			
			for( uint32 i = 0; i < _hmapSize; ++i )
			{
				for( uint32 j = 0; j < _hmapSize; ++j )
				{
					unsigned short height;
					
					// Decode 16 bit data from red and green channels
					height = (unsigned short)(clamp( pixels[(i * _hmapSize + j) * 4], 0, 1 ) * 255 * 256 +
						clamp( pixels[(i * _hmapSize + j) * 4 + 1], 0, 1 ) * 255);
					
					_heightData[i * (_hmapSize + 1) + j] = height;
				}
			}                                                                                                     
            // Fill in last rows (just repeat last texture pixels)                                               
            for( uint32 i = 0; i < _hmapSize; ++i )                                                               
			{                                                                                                     
				unsigned short height;                                                                       
				// Decode 16 bit data from red and green channels                                             
				height = (unsigned short)(clamp( pixels[(i * _hmapSize + _hmapSize - 1) * 4], 0, 1 ) * 255 * 256 +
							clamp( pixels[(i * _hmapSize + _hmapSize - 1) * 4 + 1], 0, 1 ) * 255);                                                                                                                         
				_heightData[i * (_hmapSize + 1) + _hmapSize] = height;                                           
			}                                                                                                         
                                                                                                                      
			for( uint32 i = 0; i < _hmapSize + 1; ++i )                                                               
			{                                                                                                         
				_heightData[_hmapSize * (_hmapSize + 1) + i] = _heightData[(_hmapSize - 1) * (_hmapSize + 1) + i];
			}                             

			delete[] pixels;
			return true;
		}
		else
		{
			// Init default data
			_hmapSize = 32;
			_heightData = new unsigned short[ (_hmapSize + 1) * (_hmapSize + 1)];
			memset( _heightData, 0, (_hmapSize + 1) * (_hmapSize + 1) );
			Modules::log().writeDebugInfo( "Invalid heightmap data in Terrain node %i", _handle );
			return false;
		}
	}


	void TerrainNode::calcMaxLevel()
	{
		const uint32 pow2 = _hmapSize / (_blockSize - 1);
		
		_maxLevel = 0;
		for( uint32 i = 1; i < pow2; i *= 2 ) ++_maxLevel;
	}


	uint32 TerrainNode::getVertexCount()
	{
		return (_blockSize + 2) * (_blockSize + 2);
	}


	float *TerrainNode::createVertices()
	{
		float *positions = new float[getVertexCount() * 4];	// After the positions, the height is stored as separate stream
		float *posIterator = positions;
		const uint32 size = _blockSize + 2;
		const float invScale = 1.0f / (_blockSize - 1);

		// Create vertex positions and leave height uninitialized
		for( uint32 v = 0; v < size; ++v )
		{
			for( uint32 u = 0; u < size; ++u )
			{
				*(posIterator + 0) = (u - 1) * invScale;
				*(posIterator + 1) = 0.0f;
				*(posIterator + 2) = (v - 1) * invScale;

				// Skirt vertices
				if( u == 0 ) *(posIterator + 0) = 0;
				if( v == 0 ) *(posIterator + 2) = 0;
				if( u == size - 1 ) *(posIterator + 0) = 1.0f;
				if( v == size - 1 ) *(posIterator + 2) = 1.0f;

				posIterator += 3;
			}
		}

		return positions;
	}


	uint32 TerrainNode::getIndexCount()
	{
		return (2 * (_blockSize + 2) + 1) * ((_blockSize + 2) - 1); 
	}


	unsigned short *TerrainNode::createIndices()
	{
		unsigned short *indices = new unsigned short[getIndexCount()];
		unsigned short *indexItr = indices;
		const uint32 size = _blockSize + 2;
		bool forward = true;

		// Create indices for triangle strip
		for( uint32 v = 0; v < size - 1; ++v )
		{
			for( uint32 u = 0; u < size; ++u )
			{
				// Rows go from left to right and after that right to left for best vertex cache efficieny
				uint32 indU = forward ? u : (size - 1) - u;
				
				*indexItr++ = v * size + indU;			// vert[u, v]
				*indexItr++ = (v + 1) * size + indU;	// vert[u, v+1]
			}

			// Add degenerated triangle
			*indexItr++ = *(indexItr - 1); 

			forward = !forward;
		}

		return indices;
	}


	void TerrainNode::recreateVertexBuffer()
	{
		Modules::renderer().unloadBuffers( _vertexBuffer, _indexBuffer );
		
		delete[] _heightArray; _heightArray = 0x0;
		_heightArray = new float[getVertexCount()];
		float *posArray = createVertices();
		_vertexBuffer = Modules::renderer().uploadVertices( posArray, getVertexCount() * sizeof( float ) * 4 );
		delete[] posArray;

		unsigned short *indices = createIndices();
		_indexBuffer = Modules::renderer().uploadIndices( indices, getIndexCount() * sizeof( short ) );
		delete[] indices;
	}


	void TerrainNode::buildBlockInfo( BlockInfo &block, float minU, float minV, float maxU, float maxV )
	{
		const float pixelStep = 1.0f / _hmapSize;
		const float stepU = (maxU - minU) / (_blockSize - 1);
		const float stepV = (maxV - minV) / (_blockSize - 1);
		
		for( uint32 v = 0; v < _blockSize - 1; ++v )
		{
			for( uint32 u = 0; u < _blockSize - 1; ++u )
			{
				Vec3f corner0( minU + u * stepU, 0, minV + v * stepV);
				Vec3f corner1( minU + u * stepU, 0, minV + (v + 1) * stepV);
				Vec3f corner2( minU + (u + 1) * stepU, 0, minV + v * stepV);
				Vec3f corner3( minU + (u + 1) * stepU, 0, minV + (v + 1) * stepV);

				corner0.y = getHeight( corner0.x, corner0.z );
				corner1.y = getHeight( corner1.x, corner1.z );
				corner2.y = getHeight( corner2.x, corner2.z );
				corner3.y = getHeight( corner3.x, corner3.z );
				
				Plane tri0( corner0, corner1, corner2 );
				Plane tri1( corner1, corner2, corner3 );

				for( float vv = 0; vv <= stepV; vv += pixelStep )
				{
					for( float uu = 0; uu <= stepU; uu += pixelStep )
					{
						Plane &curTri = uu <= vv ? tri0 : tri1;
						
						Vec3f point( minU + u * stepU + uu, 0, minV + v * stepV + vv );
						point.y = getHeight( point.x, point.z );
						
						block.minHeight = minf( point.y, block.minHeight );
						block.maxHeight = maxf( point.y, block.maxHeight );
						block.geoError = maxf( block.geoError, fabsf( curTri.distToPoint( point ) ));
					}
				}			
			}
		}
	}


	void TerrainNode::createBlockTree()
	{
		// The block tree contains the renderable blocks for each quad tree level, starting at the
		// lowest resolution level 0 (just one block for the complete terrain)

		uint32 size = 0, index = 0;
		for( uint32 i = 0; i <= _maxLevel; ++i ) size += (1 << i) * (1 << i);
		_blockTree.resize( size );

		for( uint32 i = 0; i <= _maxLevel; ++i )
		{
			uint32 numBlocks = 1 << i;

			for( uint32 y = 0; y < numBlocks; ++y )
			{
				for( uint32 x = 0; x < numBlocks; ++x )
				{
					buildBlockInfo( _blockTree[index++], (float)x / numBlocks, (float)y / numBlocks,
									(float)(x + 1) / numBlocks, (float)(y + 1) / numBlocks );
				}
			}
		}
	}


	int TerrainNode::getParami( int param )
	{
		switch( param )
		{
		case TerrainNodeParams::MaterialRes:
			if( _materialRes != 0x0 ) return _materialRes->getHandle();
			else return 0;
		case TerrainNodeParams::BlockSize:
			return _blockSize;
		default:
			return SceneNode::getParami( param );
		}
	}


	bool TerrainNode::setParami( int param, int value )
	{
		Resource *res;
		switch( param )
		{
		case TerrainNodeParams::HeightMapRes:
			res = Modules::resMan().resolveResHandle( value );
			if ( res == 0x0 || res->getType() != ResourceTypes::Texture2D )
			{
				Modules::log().writeDebugInfo( "Invalid Texture2D resource for Terrain node %i", _handle );
				return false;
			}
			updateHeightData( *((Texture2DResource *)res) );
			
			recreateVertexBuffer();
			calcMaxLevel();
			createBlockTree();
			return true;
		case TerrainNodeParams::MaterialRes:
			res = Modules::resMan().resolveResHandle( value );
			if( res == 0x0 || res->getType() != ResourceTypes::Material )
			{	
				Modules::log().writeDebugInfo( "Invalid Material resource for Terrain node %i", _handle );
				return false;
			}
			_materialRes = (MaterialResource *)res;
			return true;
		case TerrainNodeParams::BlockSize:
			if( _hmapSize % (value - 1) != 0 || (unsigned)value > _hmapSize )
			{
				Modules::log().writeDebugInfo( "Invalid blockSize for Terrain node %i (must be 2^x + 1)", _handle );
				return false;
			}			
			
			if (_blockSize == value)
				return true;

			_blockSize = value;
			recreateVertexBuffer();
			calcMaxLevel();
			createBlockTree();
			return true;
		default:
			return SceneNode::setParami( param, value );
		}
	}


	float TerrainNode::getParamf( int param )
	{
		switch( param )
		{
		case TerrainNodeParams::MeshQuality:
			return 1.0f / _lodThreshold;
		case TerrainNodeParams::SkirtHeight:
			return _skirtHeight;
		default:
			return SceneNode::getParamf( param );
		}
	}


	bool TerrainNode::setParamf( int param, float value )
	{
		switch( param )
		{
		case TerrainNodeParams::MeshQuality:
			_lodThreshold = 1.0f / value; 
			return true;
		case TerrainNodeParams::SkirtHeight:
			_skirtHeight = value; 
			return true;
		default:
			return SceneNode::setParamf( param, value );
		}
	}

	
	bool TerrainNode::checkIntersection( const Vec3f &rayOrig, const Vec3f &rayDir, Vec3f &intsPos ) const
	{
		// Transform ray to local space
		Matrix4f m = _absTrans.inverted();
		Vec3f orig = m * rayOrig;
		Vec3f dir = m * (rayOrig + rayDir) - orig;
	
		float t1 = 0, t2 = 1;
		// Calculate segment points inside the texture using Liang-Barsky algorithm
		if( orig.x <= orig.x + dir.x )
		{
			t1 = maxf( orig.x / -dir.x, t1 );
			t2 = minf( (1 - orig.x) / dir.x, t2 );
		}
		else
		{
			t1 = maxf( (1 - orig.x) / dir.x , t1 );
			t2 = minf( orig.x / -dir.x, t2 );
		}
		if (orig.z <= orig.z + dir.z)
		{
			t1 = maxf( orig.z / -dir.z, t1 );
			t2 = minf( (1 - orig.z) / dir.z, t2 );
		}
		else
		{
			t1 = maxf( (1 - orig.z) / dir.z, t1 );
			t2 = minf( orig.z / -dir.z, t2 );
		}
		// If t1 is greater than t2, the line segment is outside of the texture (should never be the
		// case since usually we have already checked against the bounding box)
		if( t1 > t2 ) return false;

		float startX = minf( 1, maxf( 0, orig.x + t1 * dir.x ) );
		float startZ = minf( 1, maxf( 0, orig.z + t1 * dir.z ) );
		float endX = minf( 1, maxf( 0, orig.x + t2 * dir.x ) );
		float endZ = minf( 1, maxf( 0, orig.z + t2 * dir.z ) );

		dir /= dir.length();

		int x = (int)(startX * (_hmapSize + 1) );
		int y = (int)(startZ * (_hmapSize + 1) );

		// Check heightmap with Bresenham algorithm (code based on http://de.wikipedia.org/wiki/Bresenham-Algorithmus)
		int t, dx, dy, incX, incY, pdx, pdy, ddx, ddy, err_step_fast, err_step_slow, err;

		// Get deltas in image space
		dx = (int)((endX - startX) * (_hmapSize + 1) );
		dy = (int)((endZ - startZ) * (_hmapSize + 1) );

		// Check directions
		incX = (dx > 0) ? 1 : (dx < 0) ? -1 : 0;
		incY = (dy > 0) ? 1 : (dy < 0) ? -1 : 0;
		if( dx < 0 ) dx = -dx;
		if( dy < 0 ) dy = -dy;

		if( dx > dy )  // x is the fast direction
		{
			pdx = incX; pdy = 0;
			ddx = incX; ddy = incY;
			err_step_fast = dy; err_step_slow = dx;  // Error steps fast, slow
		}
		else // y is the fast direction
		{			
			pdx = 0;    pdy = incY; // pd. is parallel step 
			ddx = incX; ddy = incY; // dd. ist diagonal step
			err_step_fast = dx;   err_step_slow = dy;   // Error steps fast, slow
		}

		// Init error
		err = err_step_slow / 2;		
		
		float height1 = _heightData[y * (_hmapSize + 1) + x] / 65535.0f, height2;

		Vec3f pos;
		Vec3f prevPos;
		
		// Check for perpendicular ray
		if( fabs(dir.z) <= Math::Epsilon && fabs(dir.x) <= Math::Epsilon )
		{
			if( (height1 < orig.y && height1 > dir.y) || (height1 > orig.y && height1 < dir.y) )
			{
				intsPos = _absTrans * Vec3f(orig.x, height1, orig.z);
				return true;
			}
			else
				return false;
		}
		else if( fabs(dir.z) <= Math::Epsilon)
			prevPos = Vec3f( startX, (dir.y * (startX - orig.x) + dir.x * orig.y) / dir.x, startZ );
		else
			prevPos = Vec3f( startX, (dir.y * (startZ - orig.z) + dir.z * orig.y) / dir.z, startZ );
				
		for( t=0; t < err_step_slow; ++t ) // error_step_slow is equal to the number of pixels to be checked
		{
			// Update error
			err -= err_step_fast; 
			if( err<0 )
			{
				// Make error positive again
				err += err_step_slow;
				// Take step in slow direction
				x += ddx;
				y += ddy;
			} 
			else // Take step in fast direction
			{
				x += pdx;
				y += pdy;
			}					
			height2 = _heightData[y * (_hmapSize + 1) + x] / 65535.0f;

			pos.x = x / (float)_hmapSize;
			pos.z = y / (float)_hmapSize;
			// Calculate y value based on the fraction of the current position on the direction vector						
			if( fabs(dir.z) <= Math::Epsilon)
				pos.y = (dir.y * (pos.x - orig.x) + dir.x * orig.y) / dir.x;
			else
				pos.y = (dir.y * (pos.z - orig.z) + dir.z * orig.y) / dir.z;

			if( prevPos.y >= pos.y && prevPos.y >= height1 && pos.y <= height2 ) 
			{
				intsPos = _absTrans * pos;
				return true;
			}
			if( prevPos.y <= pos.y && prevPos.y <= height1 && pos.y >= height2 )
			{
				intsPos = _absTrans * pos;
				return true;
			}
			height1 = height2;
			prevPos = pos;	
		}
		
		return false;
	}


	uint32 TerrainNode::calculateGeometryBlockCount( float lodThreshold, float minU, float minV,
													 float maxU, float maxV, int level, float scale)
	{
		int blockCount = 0;
		const float halfU = (minU + maxU) / 2.0f;
		const float halfV = (minV + maxV) / 2.0f;

		uint32 offset = 0;
		for( int i = 0; i < level; ++i ) offset += (1 << i) * (1 << i);

		const uint32 blockIndex =  offset + (int)(minV * (1 << level)) * (1 << level) + (int)(minU * (1 << level));
		BlockInfo &block = _blockTree[blockIndex];
		
		const float p = block.geoError;

		if( p < lodThreshold || level == _maxLevel )
		{
			return 1;
		}
		else
		{
			scale /= 2;

			Vec4f blocks[] =
			{
				Vec4f( minU, minV, halfU, halfV ),
				Vec4f( halfU, minV, maxU, halfV ),
				Vec4f( minU, halfV, halfU, maxV ),
				Vec4f( halfU, halfV, maxU, maxV )
			};

			for( uint32 i = 0; i < 4; ++i )
			{
				blockCount += calculateGeometryBlockCount( lodThreshold, blocks[i].x, blocks[i].y,
														   blocks[i].z, blocks[i].w, level + 1, scale );
			}
		}
		return blockCount;
	}


	void TerrainNode::createGeometryVertices( float lodThreshold, float minU, float minV, float maxU,
		float maxV, int level, float scale, float *&vertData, unsigned int *&indexData, uint32& index )
	{
		const float halfU = (minU + maxU) / 2.0f;
		const float halfV = (minV + maxV) / 2.0f;

		uint32 offset = 0;
		for( int i = 0; i < level; ++i ) offset += (1 << i) * (1 << i);

		const uint32 blockIndex =  offset + (int)(minV * (1 << level)) * (1 << level) + (int)(minU * (1 << level));
		BlockInfo &block = _blockTree[blockIndex];

		// Determine level of detail
		const float p = block.geoError;
	
		if( p < lodThreshold || level == _maxLevel )
		{
			const uint32 size = _blockSize + 2;
			const float invScale = 1.0f / (_blockSize - 1);

			for( uint32 v = 0; v < size; ++v )
			{	
				float t = (v - 1) * invScale;
				if( v == 0 ) t = 0.0f; else if( v == size - 1 ) t = 1.0f;	// Skirt

				for( uint32 u = 0; u < size; ++u )
				{
					float s = (u - 1) * invScale;
					if( u == 0 ) s = 0.0f; else if( u == size - 1 ) s = 1.0f;	// Skirt

					const float newU = (s * scale + minU) * _hmapSize + 0.5f;
					const float newV = (t * scale + minV) * _hmapSize + 0.5f;
					uint32 index = (int)newV * (_hmapSize + 1) + (int)newU;

					*vertData++ = (s * scale + minU);

					if( v == 0 || v == size - 1 || u == 0 || u == size - 1 )
						*vertData++ = maxf( _heightData[index] / 65535.0f - _skirtHeight, 0 );
					else
						*vertData++ = _heightData[index] / 65535.0f;

					*vertData++ = (t * scale + minV);
				}
			}

			for( uint32 v = 0; v < size - 1; ++v )
			{
				for( uint32 u = 0; u < size - 1; ++u )
				{
					*indexData++ = index + v * size + u;
					*indexData++ = index + (v + 1) * size + u;
					*indexData++ = index + (v + 1) * size + u + 1;

					*indexData++ = index + v * size + u;
					*indexData++ = index + (v + 1) * size + u + 1;
					*indexData++ = index + v * size + u + 1;
				}
			}
			index += size * size;
		}
		else 
		{
			scale /= 2;

			Vec4f blocks[] =
			{
				Vec4f( minU, minV, halfU, halfV ),
				Vec4f( halfU, minV, maxU, halfV ),
				Vec4f( minU, halfV, halfU, maxV ),
				Vec4f( halfU, halfV, maxU, maxV )
			};

			for( uint32 i = 0; i < 4; ++i )
			{
				createGeometryVertices( lodThreshold, blocks[i].x, blocks[i].y, blocks[i].z, blocks[i].w,
										level + 1, scale, vertData, indexData, index);
			}
		}
	}


	ResHandle TerrainNode::createGeometryResource( const string &name, float lodThreshold )
	{
		if( name == "" ) return 0;

		Resource *resObj = Modules::resMan().findResource( ResourceTypes::Geometry, name );
		if (resObj != 0x0)
		{	
			Modules::log().writeDebugInfo( "Resource name specified in createGeometryResource already exists" );
			return 0;
		}

		uint32 blockCount = calculateGeometryBlockCount( lodThreshold, 0.0f, 0.0f, 1.0f, 1.0f, 0, 1.0f );
		// Calculate number of vertices 
		const uint32 streamSize = blockCount * getVertexCount();
		// Calculate size of elements in stream
		const uint32 streamElementSize = sizeof( float ) * 3;
	
		const uint32 indexCount = (blockCount * (_blockSize + 1) * (_blockSize + 1) * 6);

		// Horde Flag (4) + Version (sizeof(uint32)) + Joint Count (sizeof(uint32)) + number of streams (sizeof(uint32)) +
		// + streamsize (sizeof(uint32)) + stream id (sizeof(uint32)) + stream element size (sizeof(uint32))
		// + stream (streamsize * stream element size) +  index count (sizeof(uint32)) +
		// + indices (sizeof(uint32) * index count) + number of morph targets (sizeof(uint32))
		const uint32 size = 4 + sizeof( uint32 ) * 8 + streamSize * streamElementSize + indexCount * sizeof( uint32 );

		// Create geometry data block
		char *data = new char[size];

		char *mydata = data;
		// Write Horde flag
		mydata[0] = 'H'; mydata[1] = '3'; mydata[2] = 'D'; mydata[3] = 'G'; mydata += 4;
		// Set version to 5 
		*( (uint32 *)mydata ) = 5; mydata += sizeof( uint32 );
		// Set joint count (zero for terrains)
		*( (uint32 *)mydata ) = 0; mydata += sizeof( uint32 );
		// Set number of streams
		*( (uint32 *)mydata ) = 1; mydata += sizeof( uint32 );
		// Write size of each stream
		*( (uint32 *)mydata ) = streamSize; mydata += sizeof( uint32 );
		
		// Beginning of stream data
		// Stream ID
		*( (uint32 *)mydata ) = 0; mydata += sizeof( uint32 );
		// set stream element size
		*( (uint32 *)mydata ) = streamElementSize; mydata += sizeof( uint32 );

		uint32 index = 0;
		float *vertexData = (float *)mydata;
		//const unsigned int* const refIndexData = (unsigned int*)(mydata + streamSize * streamElementSize + sizeof( uint32 ));
		uint32 *indexData = (uint32 *)(mydata + streamSize * streamElementSize + sizeof( uint32 ));
		
		createGeometryVertices( lodThreshold, 0.0f, 0.0f, 1.0f, 1.0f, 0, 1.0f, vertexData, indexData, index );
		
		// Skip vertex data
		mydata += streamSize * streamElementSize;

		// Set number of indices
		*( (uint32 *) mydata ) = indexCount; mydata += sizeof( uint32 );
		
		// Skip index data
		mydata += indexCount * sizeof( uint32 );				

		// Set number of morph targets
		*( (uint32 *) mydata ) = 0;	mydata += sizeof( uint32 );

		ResHandle res = Modules::resMan().addResource( ResourceTypes::Geometry, name, 0, true );
		resObj = Modules::resMan().resolveResHandle( res );
		if ( resObj ) resObj->load( data, size );
		delete[] data;
		
		return res;
	}	
}
