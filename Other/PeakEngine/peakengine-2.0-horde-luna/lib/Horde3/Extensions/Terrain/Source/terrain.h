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

#ifndef _Horde3DTerrain_terrain_H_
#define _Horde3DTerrain_terrain_H_

#include "egPrerequisites.h"
#include "utMath.h"
#include "egMaterial.h"
#include "egTextures.h"
#include "egScene.h"


namespace Horde3DTerrain
{
	const int SNT_TerrainNode = 100;

	extern const char *vsTerrainDebugView;
	extern const char *fsTerrainDebugView;	
	
	struct TerrainNodeTpl : public SceneNodeTpl
	{
		PTexture2DResource	hmapRes;
		PMaterialResource	matRes;
		float				meshQuality;
		float				skirtHeight;
		int					blockSize;

		TerrainNodeTpl( const string &name, Texture2DResource *hmapRes, MaterialResource *matRes ) :
			SceneNodeTpl( SNT_TerrainNode, name ), hmapRes( hmapRes ), matRes( matRes ),
			meshQuality( 50.0f ), skirtHeight( 0.1f ), blockSize( 17 )
		{
		}
	};

	
	struct TerrainNodeParams
	{
		enum List
		{
			HeightMapRes = 10000,
			MaterialRes,
			MeshQuality,
			SkirtHeight,
			BlockSize
		};
	};
	
	struct BlockInfo
	{	
		float minHeight;
		float maxHeight;
		float geoError;		// Maximum geometric error

		BlockInfo() : minHeight( 1.0f ), maxHeight( 0.0f ), geoError( 0.0f ) {}
	};

	class TerrainNode : public SceneNode
	{
	protected:
		
		PMaterialResource	_materialRes;
		uint32				_blockSize;
		float				_skirtHeight;
		float				_lodThreshold;
		
		uint32				_hmapSize;
		unsigned short		*_heightData;
		uint32				_maxLevel;
		float				*_heightArray;
		uint32				_vertexBuffer, _indexBuffer;
		BoundingBox			_localBBox;

		vector< BlockInfo >	_blockTree;


		TerrainNode( const TerrainNodeTpl &terrainTpl );
		
		bool updateHeightData( Texture2DResource &hmap );
		void calcMaxLevel();
		
		uint32 getVertexCount();
		float *createVertices();
		uint32 getIndexCount();
		unsigned short *createIndices();
		void recreateVertexBuffer();
		
		void buildBlockInfo( BlockInfo &block, float minU, float minV, float maxU, float maxV );
		void createBlockTree();

		static void drawTerrainBlock( TerrainNode *terrain, float minU, float minV, float maxU, float maxV,
									  int level, float scale, const Vec3f &localCamPos, const Frustum *frust1,
									  const Frustum *frust2, int uni_terBlockParams );

		uint32 calculateGeometryBlockCount( float lodThreshold, float minU, float minV,
											float maxU, float maxV, int level, float scale);
		void createGeometryVertices( float lodThreshold, float minU, float minV,
									 float maxU, float maxV, int level, float scale, 
									 float *&vertData, unsigned int *&indexData, uint32 &index );

	public:

		static ShaderContext debugViewShader;
		
		~TerrainNode();

		static SceneNodeTpl *parsingFunc( map< string, string > &attribs );
		static SceneNode *factoryFunc( const SceneNodeTpl &nodeTpl );
		static void renderFunc( const string &shaderContext, const string &theClass, bool debugView,
			const Frustum *frust1, const Frustum *frust2, RenderingOrder::List order, int occSet );

		bool canAttach( SceneNode &parent );
		int getParami( int param );
		bool setParami( int param, int value );
		float getParamf( int param );
		bool setParamf( int param, float value );

		bool checkIntersection( const Vec3f &rayOrig, const Vec3f &rayDir, Vec3f &intsPos ) const;

		ResHandle createGeometryResource( const string &name, float lodThreshold );
		
		BoundingBox *getLocalBBox() { return &_localBBox; }
		float getHeight( float x, float y )
			{ return _heightData[(int)(y * _hmapSize + 0.5f) * (_hmapSize + 1) + (int)(x * _hmapSize + 0.5f) ] / 65535.0f; }
	};
}

#endif // _Horde3DTerrain_terrain_H_
