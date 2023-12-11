// Based upon work Copyright (C) 2002-2007 Nikolaus Gebhardt
// Based upon a file that is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in the Irrlicht
// engine irrlicht.h file

// The code for the TerrainSceneNode is based on the GeoMipMapSceneNode
// developed by Spintz. He made it available for Irrlicht and allowed it to be
// distributed under this licence. I only modified some parts. A lot of thanks
// go to him.

// The TerrainSphereNode is an extention to the terrain module that groups six
// terrain nodes into a cube and then applies a spherical distortion to the
// 'faces' of this cube creating six terrain objects formed into a sphere
// modifications by Frank Dodd and released under the above Irrlicht License.

#include "CTerrainSphereNode.h"
//#include "CTerrainTriangleSelector.h"
#include "IVideoDriver.h"
#include "ICameraSceneNode.h"
#include "SMeshBufferLightMap.h"
#include "SViewFrustum.h"
#include "irrMath.h"
//#include "os.h"
#include "IGUIFont.h"
#include "IFileSystem.h"
#include "IReadFile.h"
#include "IAnimatedMesh.h"
#include "CDynamicMeshBuffer.h"


namespace irr
{
namespace scene
{
s32 debug = 0, debug2 = 0;

	//! constructor
	CTerrainSphereNode::CTerrainSphereNode(ISceneNode* parent, 
											ISceneManager* mgr,
											io::IFileSystem* fs, 
											s32 id, 
											s32 maxLOD, 
											E_TERRAIN_PATCH_SIZE patchSize,
											const core::vector3df& position,
											const core::vector3df& rotation,
											const core::vector3df& scale)
											: ITerrainSphereNode(parent, mgr, id, position, rotation, scale),
											TerrainData(patchSize, maxLOD, position, rotation, scale),
											DynamicSelectorUpdate(false),
											OverrideDistanceThreshold(false), UseDefaultRotationPivot(true), ForceRecalculation(false),
											OldCameraPosition(core::vector3df(-99999.9f, -99999.9f, -99999.9f)),
											OldCameraRotation(core::vector3df(-99999.9f, -99999.9f, -99999.9f)),
											CameraMovementDelta(10.0f), CameraRotationDelta(1.0f),
											TCoordScale1(1.0f), TCoordScale2(1.0f), FileSystem(fs)
	{
		#ifdef _DEBUG
		setDebugName("CTerrainSphereNode");
		#endif

		Mesh = new SMesh();

		for ( s32 face = 0; face < CUBE_FACES; ++face )
		{
			IndicesToRender[face] = 0;
			VerticesToRender[face] = 0;
			
			RenderBuffer[face] = new CDynamicMeshBuffer(video::EVT_2TCOORDS, video::EIT_16BIT);
			RenderBuffer[face]->setHardwareMappingHint(scene::EHM_STATIC, scene::EBT_VERTEX);
			//Causes nasty display driver reset on my machine ?
			//RenderBuffer[face]->setHardwareMappingHint(scene::EHM_DYNAMIC, scene::EBT_INDEX);
			
		}

		if (FileSystem)
			FileSystem->grab();

		setAutomaticCulling( scene::EAC_OFF );
	}


	//! destructor
	CTerrainSphereNode::~CTerrainSphereNode ( )
	{
		if (TerrainData.LODDistanceThreshold)
			delete [] TerrainData.LODDistanceThreshold;

		if (TerrainData.Patches)
			delete [] TerrainData.Patches;

		if (FileSystem)
			FileSystem->drop();

		if(Mesh)
			Mesh->drop();
	}

	//! distort the terrain geometry before the heightmap is applied
	void CTerrainSphereNode::distort( core::vector3df &v, float scale, s32 face, bool invX, bool invZ )
	{
		// spherical distortion requires the grid co-ordinates to lie between -1 and 1
		// scale the vertex
		float x, y, z, height, swap;
		x = ( v.X / scale * 2 ) - 1.0f;
		y = 1.0f;
		z = ( v.Z / scale * 2 ) - 1.0f;

		// the heightmapping will be used to scale the entire spherical patch
		height = 1.0f + ( v.Y / scale / 40.0f );

		// apply the distortion presented by Phil on MathProofs (Mapping a Cube to a Sphere)
        // and the example posted on Infinity-Universe forum
		v.X = x * scale * sqrtf( 1 - ( y * y * 0.5f ) - ( z * z * 0.5f ) + ((( y * y ) * ( z * z )) / 3.0f ));
		v.Y = y * scale * sqrtf( 1 - ( z * z * 0.5f ) - ( x * x * 0.5f ) + ((( z * z ) * ( x * x )) / 3.0f ));
		v.Z = z * scale * sqrtf( 1 - ( x * x * 0.5f ) - ( y * y * 0.5f ) + ((( x * x ) * ( y * y )) / 3.0f ));

		v.X *= height; v.Y *= height; v.Z *= height;

		/* depending on which face is being processed swap co-ordinate about to
		orientate it to face a new direction */
		switch ( face )
		{
			case 1:
				// front face swap z and y
				swap = v.Y;
				v.Y = v.Z;
//				v.X = -v.X;
				v.Z = swap;
				break;

			case 2:
				// back face swap z and y and invert y
				swap = v.Y;
				v.Y = v.Z;
				v.Z = -swap;
				break;

			case 3:
				// left face swap x and y
				swap = v.Y;
				v.Y = v.X;
				v.X = swap;
				break;

			case 4:
				// right face swap x and y and invert y
				swap = v.Y;
				v.Y = v.X;
				v.X = -swap;
				break;

			case 5:
				// bottom face invert y
				v.Y = -v.Y;
				v.X = v.X;
				break;

			default:
				// the top face (or an unknown face)
				break;
		}
	}

	//! smooth the terrain
	void CTerrainSphereNode::smoothTerrain(IDynamicMeshBuffer* mb, s32 smoothFactor)
	{
		for (s32 run = 0; run < smoothFactor; ++run)
		{
			s32 yd = TerrainData.Size;
			for (s32 y = 1; y < TerrainData.Size - 1; ++y)
			{
				for (s32 x = 1; x < TerrainData.Size - 1; ++x)
				{
					mb->getVertexBuffer()[x + yd].Pos.Y =
						(mb->getVertexBuffer()[x-1 + yd].Pos.Y + //left
						mb->getVertexBuffer()[x+1 + yd].Pos.Y + //right
						mb->getVertexBuffer()[x   + yd - TerrainData.Size].Pos.Y + //above
						mb->getVertexBuffer()[x   + yd + TerrainData.Size].Pos.Y) * 0.25f; //below
				}
				yd += TerrainData.Size;
			}
		}
	}

	//! Initializes the terrain data.  Loads the vertices from the heightMapFile
	bool CTerrainSphereNode::loadHeightMap( io::IReadFile** file, video::SColor vertexColor, s32 smoothFactor, f64 terrainHeight)
	{
	    s32 face;
	    video::IImage * heightMap[CUBE_FACES];

		if( !file )
			return false;

        for ( face = 0; face < CUBE_FACES; face++ )
        {
            heightMap[face] = SceneManager->getVideoDriver()->createImageFromFile( file[face] );

            if( !heightMap )
            {
                return false;
            }
        }

		HeightmapFile = file[0]->getFileName();

		// Get the dimension of the heightmap data
		TerrainData.Size = heightMap[0]->getDimension().Width;

		switch( TerrainData.PatchSize )
		{
			case ETPS_9:
				if( TerrainData.MaxLOD > 3 )
				{
					TerrainData.MaxLOD = 3;
				}
			break;
			case ETPS_17:
				if( TerrainData.MaxLOD > 4 )
				{
					TerrainData.MaxLOD = 4;
				}
			break;
			case ETPS_33:
				if( TerrainData.MaxLOD > 5 )
				{
					TerrainData.MaxLOD = 5;
				}
			break;
			case ETPS_65:
				if( TerrainData.MaxLOD > 6 )
				{
					TerrainData.MaxLOD = 6;
				}
			break;
			case ETPS_129:
				if( TerrainData.MaxLOD > 7 )
				{
					TerrainData.MaxLOD = 7;
				}
			break;
		}

		
		// itterate the six faces of a cube
 		for ( face = 0; face < CUBE_FACES; ++face )
		{
			// --- Generate vertex data from heightmap ----
			// resize the vertex array for the mesh buffer one time (makes loading faster)
			scene::CDynamicMeshBuffer *mb=0;

			const u32 numVertices = TerrainData.Size * TerrainData.Size;
			
			if (numVertices <= 65536)
			{
				//small enough for 16bit buffers
				mb=new scene::CDynamicMeshBuffer(video::EVT_2TCOORDS, video::EIT_16BIT);
				RenderBuffer[face]->getIndexBuffer().setType(video::EIT_16BIT);
			}
			else
			{
				//we need 32bit buffers
				mb=new scene::CDynamicMeshBuffer(video::EVT_2TCOORDS, video::EIT_32BIT);
				RenderBuffer[face]->getIndexBuffer().setType(video::EIT_32BIT);
			}
			
			
			mb->getVertexBuffer().set_used( numVertices );

			//video::S3DVertex2TCoords vertex;
			
			s32 TerrainSize = ((TerrainData.Size - 1) / ( TerrainData.CalcPatchSize )) * TerrainData.CalcPatchSize;

			// Read the heightmap to get the vertex data
			// Apply positions changes, scaling changes
			const f32 tdSize = 1.0f/(f32)(TerrainData.Size-1);
//			const f32 tdSize = 1.0f/(f32)(TerrainSize-1);
			s32 index = 0;
			for( s32 x = 0; x < TerrainData.Size; ++x )
			{
				for( s32 z = 0; z < TerrainData.Size; ++z )
				{
				    video::S3DVertex2TCoords& vertex= static_cast<video::S3DVertex2TCoords*>(mb->getVertexBuffer().pointer())[index++];
					vertex.Normal.set( 0.0f, 1.0f, 0.0f );
					vertex.Color = vertexColor;
					// adjustments for face allignment
				    s32 px, pz;
				    switch(face)
				    {
				        case 0:
                            px=TerrainData.Size-1-x; pz=z;
                            break;
				        case 1:
                            px=TerrainData.Size-1-x; pz=TerrainData.Size-1-z;
                            break;
				        case 2:
                            px=x; pz=TerrainData.Size-1-z;
                            break;
				        case 3:
                            px=z; pz=TerrainData.Size-1-x;
                            break;
				        case 4:
                            px=TerrainData.Size-1-z; pz=TerrainData.Size-1-x;
                            break;
				        case 5:
                            px=x; pz=z;
                            break;
				    }

					vertex.Pos.X = (f32)x;
					video::SColor pixelColor(heightMap[face]->getPixel(px,pz));
					/* as the terrain displays one less patch at the top and left
					scale our heightmap data to utilise the	whole image map */
//					tx = (s32)(x*(TerrainData.Size)*tdSize); if ( tx > 255 ) tx = 255;
//					tz = (s32)(z*(TerrainData.Size)*tdSize); if ( tz > 255 ) tz = 255;
//					video::SColor pixelColor( heightMap->getPixel( tx, tz ));
					vertex.Pos.Y = (f32) pixelColor.getLuminance()*terrainHeight;
					vertex.Pos.Z = (f32)z;

					vertex.TCoords.X = vertex.TCoords2.X = px * tdSize;
					vertex.TCoords.Y = vertex.TCoords2.Y = pz * tdSize;

					// Frank: apply spherical transformation
//					distort( vertex.Pos, (float)(TerrainSize), face );
					distort( vertex.Pos, (float)(TerrainData.Size-1), face );

					//pMeshBuffer->Vertices[index] = vertex;
				}
			}

			smoothTerrain(mb, smoothFactor);

			// calculate smooth normals for the vertices
			calculateNormals( mb, face );

			// add the MeshBuffer to the mesh
			Mesh->addMeshBuffer( mb );
			
			// We copy the data to the renderBuffer, after the normals have been calculated.
			RenderBuffer[face]->getVertexBuffer().set_used(numVertices);

			for( u32 i = 0; i < numVertices; ++i )
			{
				RenderBuffer[face]->getVertexBuffer()[i] = mb->getVertexBuffer()[i];
				RenderBuffer[face]->getVertexBuffer()[i].Pos *= TerrainData.Scale;
				RenderBuffer[face]->getVertexBuffer()[i].Pos += TerrainData.Position;
			}

			// We no longer need the pMeshBuffer
			mb->drop();
		}

		// drop heightMap, no longer needed
        for ( face = 0; face < CUBE_FACES; face++ )
        {
            heightMap[face]->drop();
        }

		// calculate all the necessary data for the patches and the terrain
		calculateDistanceThresholds();
		createPatches();
		calculatePatchData();

		// set the default rotation pivot point to the terrain nodes center
		TerrainData.RotationPivot = TerrainData.Center;
//		TerrainData.RotationPivot = core::vector3df(0.0f,0.0f,0.0f); // TerrainData.Center;

		// Rotate the vertices of the terrain by the rotation
		// specified.  Must be done after calculating the terrain data,
		// so we know what the current center of the terrain is.
		setRotation( TerrainData.Rotation );

		// Pre-allocate memory for indices
		for ( s32 face = 0; face < CUBE_FACES; ++face )
		{
			RenderBuffer[face]->getIndexBuffer().set_used( TerrainData.PatchCount * TerrainData.PatchCount *
				TerrainData.CalcPatchSize * TerrainData.CalcPatchSize * 6 );
		}

		//RenderBuffer[face]->setDirty();

/*		const u32 endTime = os::Timer::getRealTime();

		c8 tmp[255];
		sprintf(tmp, "Generated terrain data (%dx%d) in %.4f seconds",
			TerrainData.Size, TerrainData.Size, ( endTime - startTime ) / 1000.0f );
		os::Printer::log( tmp );
*/
		return true;
	}

    //! Returns the material based on the zero based index i. This scene node only uses
    //! 1 material.
    //! \param i: Zero based index i. UNUSED, left in for virtual purposes.
    //! \return Returns the single material this scene node uses.
    void CTerrainSphereNode::setTextures (video::ITexture *textureTop,
										video::ITexture *textureFront,
										video::ITexture *textureBack,
										video::ITexture *textureLeft,
										video::ITexture *textureRight,
										video::ITexture *textureBottom,
										video::SColor ambientColor,
										video::SColor emissiveColor,
										u32 materialIndex )
    {
        Mesh->getMeshBuffer(0)->getMaterial().setTexture( materialIndex, textureTop );
        Mesh->getMeshBuffer(1)->getMaterial().setTexture( materialIndex, textureFront);
        Mesh->getMeshBuffer(2)->getMaterial().setTexture( materialIndex, textureBack);
        Mesh->getMeshBuffer(3)->getMaterial().setTexture( materialIndex, textureLeft);
        Mesh->getMeshBuffer(4)->getMaterial().setTexture( materialIndex, textureRight);
        Mesh->getMeshBuffer(5)->getMaterial().setTexture( materialIndex, textureBottom);

		Mesh->getMeshBuffer(0)->getMaterial().TextureLayer[0].TextureWrapU = video::ETC_CLAMP_TO_EDGE;
		Mesh->getMeshBuffer(0)->getMaterial().TextureLayer[0].TextureWrapV = video::ETC_CLAMP_TO_EDGE;
		Mesh->getMeshBuffer(0)->getMaterial().EmissiveColor = emissiveColor;
		Mesh->getMeshBuffer(0)->getMaterial().AmbientColor = ambientColor;

		Mesh->getMeshBuffer(1)->getMaterial().TextureLayer[0].TextureWrapU = video::ETC_CLAMP_TO_EDGE;
		Mesh->getMeshBuffer(1)->getMaterial().TextureLayer[0].TextureWrapV = video::ETC_CLAMP_TO_EDGE;
		Mesh->getMeshBuffer(1)->getMaterial().EmissiveColor = emissiveColor;
		Mesh->getMeshBuffer(1)->getMaterial().AmbientColor = ambientColor;

		Mesh->getMeshBuffer(2)->getMaterial().TextureLayer[0].TextureWrapU = video::ETC_CLAMP_TO_EDGE;
		Mesh->getMeshBuffer(2)->getMaterial().TextureLayer[0].TextureWrapV = video::ETC_CLAMP_TO_EDGE;
		Mesh->getMeshBuffer(2)->getMaterial().EmissiveColor = emissiveColor;
		Mesh->getMeshBuffer(2)->getMaterial().AmbientColor = ambientColor;

		Mesh->getMeshBuffer(3)->getMaterial().TextureLayer[0].TextureWrapU = video::ETC_CLAMP_TO_EDGE;
		Mesh->getMeshBuffer(3)->getMaterial().TextureLayer[0].TextureWrapV = video::ETC_CLAMP_TO_EDGE;
		Mesh->getMeshBuffer(3)->getMaterial().EmissiveColor = emissiveColor;
		Mesh->getMeshBuffer(3)->getMaterial().AmbientColor = ambientColor;

		Mesh->getMeshBuffer(4)->getMaterial().TextureLayer[0].TextureWrapU = video::ETC_CLAMP_TO_EDGE;
		Mesh->getMeshBuffer(4)->getMaterial().TextureLayer[0].TextureWrapV = video::ETC_CLAMP_TO_EDGE;
		Mesh->getMeshBuffer(4)->getMaterial().EmissiveColor = emissiveColor;
		Mesh->getMeshBuffer(4)->getMaterial().AmbientColor = ambientColor;

		Mesh->getMeshBuffer(5)->getMaterial().TextureLayer[0].TextureWrapU = video::ETC_CLAMP_TO_EDGE;
		Mesh->getMeshBuffer(5)->getMaterial().TextureLayer[0].TextureWrapV = video::ETC_CLAMP_TO_EDGE;
		Mesh->getMeshBuffer(5)->getMaterial().EmissiveColor = emissiveColor;
		Mesh->getMeshBuffer(5)->getMaterial().AmbientColor = ambientColor;

       
    }


    //! load the vertex colors ,of each of the faces from a series of image files
    //! param 1: an array of six image files
	void CTerrainSphereNode::loadVertexColor( video::IImage **images )
	{
		// itterate the six faces of a cube
 		for ( s32 face = 0; face < CUBE_FACES; ++face )
		{
			s32 index = 0;
			for( s32 x = 0; x < TerrainData.Size; ++x )
			{
				for( s32 z = 0; z < TerrainData.Size; ++z )
				{
				    // adjustments for face allignment
				    s32 px, pz;
				    switch(face)
				    {
				        case 0:
                            px=TerrainData.Size-1-x; pz=z;
                            break;
				        case 1:
                            px=TerrainData.Size-1-x; pz=TerrainData.Size-1-z;
                            break;
				        case 2:
                            px=x; pz=TerrainData.Size-1-z;
                            break;
				        case 3:
                            px=z; pz=TerrainData.Size-1-x;
                            break;
				        case 4:
                            px=TerrainData.Size-1-z; pz=TerrainData.Size-1-x;
                            break;
				        case 5:
                            px=x; pz=z;
                            break;
				    }

                    video::SColor color = images[face]->getPixel(px, pz);
					RenderBuffer[face]->getVertexBuffer()[index++].Color = color;
				}
			}
		}
	}


	/*! Initializes the terrain data.  Loads the vertices from the heightMapFile
	bool CTerrainSphereNode::loadHeightMapRAW( io::IReadFile* file, s32 bitsPerPixel, video::SColor vertexColor, s32 smoothFactor )
	{
		if( !file )
			return false;

		// start reading
//		u32 startTime = os::Timer::getTime();

		// get file size
		const long fileSize = file->getSize();
		const s32 bytesPerPixel = bitsPerPixel / 8;

		// Get the dimension of the heightmap data
		TerrainData.Size = core::floor32(sqrtf( (f32)( fileSize / bytesPerPixel ) ));

		switch( TerrainData.PatchSize )
		{
			case ETPS_9:
				if( TerrainData.MaxLOD > 3 )
				{
					TerrainData.MaxLOD = 3;
				}
			break;
			case ETPS_17:
				if( TerrainData.MaxLOD > 4 )
				{
					TerrainData.MaxLOD = 4;
				}
			break;
			case ETPS_33:
				if( TerrainData.MaxLOD > 5 )
				{
					TerrainData.MaxLOD = 5;
				}
			break;
			case ETPS_65:
				if( TerrainData.MaxLOD > 6 )
				{
					TerrainData.MaxLOD = 6;
				}
			break;
			case ETPS_129:
				if( TerrainData.MaxLOD > 7 )
				{
					TerrainData.MaxLOD = 7;
				}
			break;
		}

		// itterate the six faces of a cube
		for ( s32 face = 0; face < CUBE_FACES; ++face )
		{
			// --- Generate vertex data from heightmap ----
			// resize the vertex array for the mesh buffer one time ( makes loading faster )
			SMeshBufferLightMap* pMeshBuffer = new SMeshBufferLightMap();
			pMeshBuffer->Vertices.reallocate( TerrainData.Size * TerrainData.Size );

			video::S3DVertex2TCoords vertex;
			vertex.Normal.set( 0.0f, 1.0f, 0.0f );
			vertex.Color = vertexColor;

			// Read the heightmap to get the vertex data
			// Apply positions changes, scaling changes
			const f32 tdSize = 1.0f/(f32)(TerrainData.Size-1);
			for( s32 x = 0; x < TerrainData.Size; ++x )
			{
				for( s32 z = 0; z < TerrainData.Size; ++z )
				{
					vertex.Pos.X = (f32)x;

					if( file->read( &vertex.Pos.Y, bytesPerPixel ) != bytesPerPixel )
					{
//						os::Printer::print("Error reading heightmap RAW file.");
						pMeshBuffer->drop();
						return false;
					}

					vertex.Pos.Z = (f32)z;

					vertex.TCoords.X = vertex.TCoords2.X = x * tdSize;
					vertex.TCoords.Y = vertex.TCoords2.Y = z * tdSize;

					pMeshBuffer->Vertices.push_back( vertex );
				}
			}

			//! Terrain smoothing. Applause to DeusXL!
			// http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?p=91272#91272

			s32 run;
			s32 index;

			for( run = 0; run < smoothFactor; ++run )
			{
				for( index = 2; index < (TerrainData.Size * TerrainData.Size - 2); ++index)
				{
					pMeshBuffer->Vertices[index].Pos.Y =
						(pMeshBuffer->Vertices[index - 2].Pos.Y + pMeshBuffer->Vertices[index - 1].Pos.Y +
						pMeshBuffer->Vertices[index + 1].Pos.Y + pMeshBuffer->Vertices[index + 2].Pos.Y) / 4.0f;
				}
			}

			for( run = 0; run < smoothFactor; ++run)
			{
				for( index = TerrainData.Size; index < (TerrainData.Size * (TerrainData.Size - 1)); ++index)
				{
					pMeshBuffer->Vertices[index].Pos.Y =
						(pMeshBuffer->Vertices[index - TerrainData.Size].Pos.Y +
						pMeshBuffer->Vertices[index + TerrainData.Size].Pos.Y ) / 2.0f;
				}
			}

			// calculate smooth normals for the vertices
			calculateNormals( pMeshBuffer, face );

			// add the MeshBuffer to the mesh
			Mesh->addMeshBuffer( pMeshBuffer );
			const u32 vertexCount = pMeshBuffer->getVertexCount();

			// We copy the data to the renderBuffer, after the normals have been calculated.
			RenderBuffer[face].Vertices.set_used( vertexCount );

			for( u32 i = 0; i < vertexCount; i++ )
			{
				RenderBuffer[face].Vertices[i] = pMeshBuffer->Vertices[i];
				RenderBuffer[face].Vertices[i].Pos *= TerrainData.Scale;
				RenderBuffer[face].Vertices[i].Pos += TerrainData.Position;
			}

			// We no longer need the pMeshBuffer
			pMeshBuffer->drop();
		}

		// calculate all the necessary data for the patches and the terrain
		calculateDistanceThresholds();
		createPatches();
		calculatePatchData();

		// set the default rotation pivot point to the terrain nodes center
		TerrainData.RotationPivot = core::vector3df(0.0f,0.0f,0.0f); // TerrainData.Center;

		// Rotate the vertices of the terrain by the rotation specified.  Must be done
		// after calculating the terrain data, so we know what the current center of the
		// terrain is.
		setRotation( TerrainData.Rotation );

		// Pre-allocate memory for indices
		for ( s32 face = 0; face < CUBE_FACES; ++face )
		{
			RenderBuffer[face].Indices.set_used( TerrainData.PatchCount * TerrainData.PatchCount *
				TerrainData.CalcPatchSize * TerrainData.CalcPatchSize * 6 );
		}

		u32 endTime = os::Timer::getTime();

		c8 tmp[255];
		sprintf( tmp, "Generated terrain data (%dx%d) in %.4f seconds",
			TerrainData.Size, TerrainData.Size, (endTime - startTime) / 1000.0f );
		os::Printer::print( tmp );

		return true;
	}
	*/

	//! Sets the scale of the scene node.
	//! \param scale: New scale of the node
	void CTerrainSphereNode::setScale(const core::vector3df& scale)
	{
		TerrainData.Scale = scale;
		applyTransformation();
		ForceRecalculation = true;
	}

	//! Sets the rotation of the node. This only modifies
	//! the relative rotation of the node.
	//! \param rotation: New rotation of the node in degrees.
	void CTerrainSphereNode::setRotation(const core::vector3df& rotation)
	{
		TerrainData.Rotation = rotation;
		applyTransformation();
		ForceRecalculation = true;
	}

	//! Sets the pivot point for rotation of this node.  This is useful for the TiledTerrainManager to
	//! rotate all terrain tiles around a global world point.
	//! NOTE: The default for the RotationPivot will be the center of the individual tile.
	void CTerrainSphereNode::setRotationPivot( const core::vector3df& pivot )
	{
		UseDefaultRotationPivot = false;
		TerrainData.RotationPivot = pivot;
	}

	//! Sets the position of the node.
	//! \param newpos: New postition of the scene node.
	void CTerrainSphereNode::setPosition ( const core::vector3df& newpos )
	{
		TerrainData.Position = newpos;
		applyTransformation();
		ForceRecalculation = true;
	}
	
	//! Apply transformation changes( scale, position, rotation )
	void CTerrainSphereNode::applyTransformation()
	{
		if( !Mesh->getMeshBufferCount() )
			return;

		for ( s32 face = 0; face < CUBE_FACES; ++face )
		{
			TerrainData.Position = TerrainData.Position;
			video::S3DVertex2TCoords* meshVertices = (video::S3DVertex2TCoords*)Mesh->getMeshBuffer( face )->getVertices();
			s32 vtxCount = Mesh->getMeshBuffer( face )->getVertexCount();
			core::matrix4 rotMatrix;
			rotMatrix.setRotationDegrees( TerrainData.Rotation );

			for( s32 i = 0; i < vtxCount; ++i )
			{
				RenderBuffer[face]->getVertexBuffer()[i].Pos = meshVertices[i].Pos * TerrainData.Scale + TerrainData.Position;

				RenderBuffer[face]->getVertexBuffer()[i].Pos -= TerrainData.RotationPivot;
				rotMatrix.inverseRotateVect( RenderBuffer[face]->getVertexBuffer()[i].Pos );
				RenderBuffer[face]->getVertexBuffer()[i].Pos += TerrainData.RotationPivot;
			}

			//calculateNormals(&RenderBuffer[face], face);
			RenderBuffer[face]->setDirty(EBT_VERTEX);
		}
		calculateDistanceThresholds( true );
		calculatePatchData();
	}

	//! Updates the scene nodes indices if the camera has moved or rotated by a certain
	//! threshold, which can be changed using the SetCameraMovementDeltaThreshold and
	//! SetCameraRotationDeltaThreshold functions.  This also determines if a given patch
	//! for the scene node is within the view frustum and if it's not the indices are not
	//! generated for that patch.
	void CTerrainSphereNode::OnRegisterSceneNode()
	{
		if (!IsVisible || !SceneManager->getActiveCamera())
			return;

		preRenderLODCalculations();
		preRenderIndicesCalculations();
		ISceneNode::OnRegisterSceneNode();
		ForceRecalculation = false;
	}

	void CTerrainSphereNode::preRenderLODCalculations()
	{
		SceneManager->registerNodeForRendering( this );
		// Do Not call ISceneNode::OnRegisterSceneNode ( ), this node should have no children

		// Determine the camera rotation, based on the camera direction.
		core::line3d<f32> line;
		line.start = SceneManager->getActiveCamera()->getAbsolutePosition();
		line.end = SceneManager->getActiveCamera()->getTarget();
		core::vector3df cameraRotation = line.getVector().getHorizontalAngle();
		core::vector3df cameraPosition = SceneManager->getActiveCamera()->getPosition ( );

		// Only check on the Camera's Y Rotation
		if (!ForceRecalculation)
		{
			if (( fabs(cameraRotation.X - OldCameraRotation.X) < CameraRotationDelta) &&
				( fabs(cameraRotation.Y - OldCameraRotation.Y) < CameraRotationDelta))
			{
				if ((fabs(cameraPosition.X - OldCameraPosition.X) < CameraMovementDelta) &&
					(fabs(cameraPosition.Y - OldCameraPosition.Y) < CameraMovementDelta) &&
					(fabs(cameraPosition.Z - OldCameraPosition.Z) < CameraMovementDelta))
				{
					return;
				}
			}
		}

		OldCameraPosition = cameraPosition;
		OldCameraRotation = cameraRotation;
		const SViewFrustum* frustum = SceneManager->getActiveCamera()->getViewFrustum();

		// Determine each patches LOD based on distance from camera ( and whether or not they are in
		// the view frustum ).
		for( s32 j = 0; j < TerrainData.PatchCount * TerrainData.PatchCount * CUBE_FACES; ++j )
		{
//if ( j == debug ) printf( "Testing box %d\n", debug );
			if( frustum->getBoundingBox().intersectsWithBox( TerrainData.Patches[j].BoundingBox ) )
			{
				f32 distance = (cameraPosition.X - TerrainData.Patches[j].Center.X) * (cameraPosition.X - TerrainData.Patches[j].Center.X) +
					(cameraPosition.Y - TerrainData.Patches[j].Center.Y) * (cameraPosition.Y - TerrainData.Patches[j].Center.Y) +
					(cameraPosition.Z - TerrainData.Patches[j].Center.Z) * (cameraPosition.Z - TerrainData.Patches[j].Center.Z);
//if ( j == debug ) printf( "Frustrum Intersects at distance of %f\n", distance );

                // If we've turned off a patch from viewing, because of the frustum, and now we turn around and it's
                // too close, we need to turn it back on, at the highest LOD.  The if above doesn't catch this.
                TerrainData.Patches[j].CurrentLOD = 0;

				for( s32 i = TerrainData.MaxLOD - 1; i >= 0; --i )
				{
//if ( j == debug ) printf( "Testing against %f\n", TerrainData.LODDistanceThreshold[i] );
					if( distance >= TerrainData.LODDistanceThreshold[i] )
					{
						TerrainData.Patches[j].CurrentLOD = i;
//if ( j == debug ) printf( "Setting LOD to %d\n", i );
						break;
					}
					//else if( i == 0 )
					{
						// If we've turned off a patch from viewing, because of the frustum, and now we turn around and it's
						// too close, we need to turn it back on, at the highest LOD.  The if above doesn't catch this.
//						TerrainData.Patches[j].CurrentLOD = 0;
					}
				}
			}
			else
			{
				TerrainData.Patches[j].CurrentLOD = -1;
			}
		}
	}

	void CTerrainSphereNode::preRenderIndicesCalculations()
	{
		s32 index11;
		s32 index21;
		s32 index12;
		s32 index22;
		s32 highest = 0;
		s32 highestpatch = 0;

		for ( s32 face = 0; face < CUBE_FACES; ++face )
		{
			IndicesToRender[face] = 0;

			// Then generate the indices for all patches that are visible.
			for( s32 i = 0; i < TerrainData.PatchCount; ++i )
			{
				for( s32 j = 0; j < TerrainData.PatchCount; ++j )
				{
					// calculate the patch index number
					s32 index = face * TerrainData.PatchCount * TerrainData.PatchCount +
								i * TerrainData.PatchCount + j;

					if( TerrainData.Patches[index].CurrentLOD >= 0 )
					{
						s32 x = 0;
						s32 z = 0;

						// calculate the step we take this patch, based on the patches current LOD
						s32 step = 1 << TerrainData.Patches[index].CurrentLOD;
						//	if ( index == debug ) printf( "Patch LOD is %d\n", TerrainData.Patches[index].CurrentLOD );

						// Loop through patch and generate indices
						while( z < TerrainData.CalcPatchSize )
						{
							index11 = getIndex( j, i, index, x, z );
							index21 = getIndex( j, i, index, x + step, z );
							index12 = getIndex( j, i, index, x, z + step );
							index22 = getIndex( j, i, index, x + step, z + step );

							/* color verticies based on LOD */
	/*							s32 vertColor = step * 10;
								s32 vertColor = j * 16;
						if (( index == debug ) || ( index == debug2 ))
						{
							switch ( face )
							{
                            case 0: // TOP
                                RenderBuffer[face].Vertices[index11].Color.setRed( vertColor );
                                RenderBuffer[face].Vertices[index11].Color.setGreen( 0 );
                                RenderBuffer[face].Vertices[index11].Color.setBlue( 0 );

                                RenderBuffer[face].Vertices[index21].Color.setRed( vertColor );
                                RenderBuffer[face].Vertices[index21].Color.setGreen( 0 );
                                RenderBuffer[face].Vertices[index21].Color.setBlue( 0 );

                                RenderBuffer[face].Vertices[index12].Color.setRed( vertColor );
                                RenderBuffer[face].Vertices[index12].Color.setGreen( 0 );
                                RenderBuffer[face].Vertices[index12].Color.setBlue( 0 );

                                RenderBuffer[face].Vertices[index22].Color.setRed( vertColor );
                                RenderBuffer[face].Vertices[index22].Color.setGreen( 0 );
                                RenderBuffer[face].Vertices[index22].Color.setBlue( 0 );
                                break;

                            case 4: // RIGHT
                                RenderBuffer[face].Vertices[index11].Color.setRed( 0 );
                                RenderBuffer[face].Vertices[index11].Color.setGreen( vertColor );
                                RenderBuffer[face].Vertices[index11].Color.setBlue( 0 );

                                RenderBuffer[face].Vertices[index21].Color.setRed( 0 );
                                RenderBuffer[face].Vertices[index21].Color.setGreen( vertColor );
                                RenderBuffer[face].Vertices[index21].Color.setBlue( 0 );

                                RenderBuffer[face].Vertices[index12].Color.setRed( 0 );
                                RenderBuffer[face].Vertices[index12].Color.setGreen( vertColor );
                                RenderBuffer[face].Vertices[index12].Color.setBlue( 0 );

                                RenderBuffer[face].Vertices[index22].Color.setRed( 0 );
                                RenderBuffer[face].Vertices[index22].Color.setGreen( vertColor );
                                RenderBuffer[face].Vertices[index22].Color.setBlue( 0 );
                                break;

                            default:
                                RenderBuffer[face].Vertices[index11].Color.setRed( vertColor );
                                RenderBuffer[face].Vertices[index11].Color.setGreen( vertColor );
                                RenderBuffer[face].Vertices[index11].Color.setBlue( vertColor );

                                RenderBuffer[face].Vertices[index21].Color.setRed( vertColor );
                                RenderBuffer[face].Vertices[index21].Color.setGreen( vertColor );
                                RenderBuffer[face].Vertices[index21].Color.setBlue( vertColor );

                                RenderBuffer[face].Vertices[index12].Color.setRed( vertColor );
                                RenderBuffer[face].Vertices[index12].Color.setGreen( vertColor );
                                RenderBuffer[face].Vertices[index12].Color.setBlue( vertColor );

                                RenderBuffer[face].Vertices[index22].Color.setRed( vertColor );
                                RenderBuffer[face].Vertices[index22].Color.setGreen( vertColor );
                                RenderBuffer[face].Vertices[index22].Color.setBlue( vertColor );
                                break;
							}
}
*/
                            // Invert the normals of the front left and bottom faces
                            
							if (( face == 1 ) || ( face == 3 ) || ( face == 5 ))
                            {
								RenderBuffer[face]->getIndexBuffer().setValue(IndicesToRender[face]++,index22);
								RenderBuffer[face]->getIndexBuffer().setValue(IndicesToRender[face]++,index11);
								RenderBuffer[face]->getIndexBuffer().setValue(IndicesToRender[face]++,index12);
								RenderBuffer[face]->getIndexBuffer().setValue(IndicesToRender[face]++,index21);
								RenderBuffer[face]->getIndexBuffer().setValue(IndicesToRender[face]++,index11);
								RenderBuffer[face]->getIndexBuffer().setValue(IndicesToRender[face]++,index22);
								/*
								RenderBuffer[face].Indices[IndicesToRender[face]++] = index22;
                                RenderBuffer[face].Indices[IndicesToRender[face]++] = index11;
                                RenderBuffer[face].Indices[IndicesToRender[face]++] = index12;
                                RenderBuffer[face].Indices[IndicesToRender[face]++] = index21;
                                RenderBuffer[face].Indices[IndicesToRender[face]++] = index11;
                                RenderBuffer[face].Indices[IndicesToRender[face]++] = index22;
								*/
                            }
                            else
                            {
								RenderBuffer[face]->getIndexBuffer().setValue(IndicesToRender[face]++,index12);
								RenderBuffer[face]->getIndexBuffer().setValue(IndicesToRender[face]++,index11);
								RenderBuffer[face]->getIndexBuffer().setValue(IndicesToRender[face]++,index22);
								RenderBuffer[face]->getIndexBuffer().setValue(IndicesToRender[face]++,index22);
								RenderBuffer[face]->getIndexBuffer().setValue(IndicesToRender[face]++,index11);
								RenderBuffer[face]->getIndexBuffer().setValue(IndicesToRender[face]++,index21);
                                /*
								RenderBuffer[face].Indices[IndicesToRender[face]++] = index12;
                                RenderBuffer[face].Indices[IndicesToRender[face]++] = index11;
                                RenderBuffer[face].Indices[IndicesToRender[face]++] = index22;
                                RenderBuffer[face].Indices[IndicesToRender[face]++] = index22;
                                RenderBuffer[face].Indices[IndicesToRender[face]++] = index11;
                                RenderBuffer[face].Indices[IndicesToRender[face]++] = index21;
								*/
                            }

							// increment index position horizontally
							x += step;
							if (( x + j * TerrainData.CalcPatchSize ) > highest ) highest = ( x + j * TerrainData.CalcPatchSize );
							if ( j  > highestpatch ) highestpatch = j;

							if ( x >= TerrainData.CalcPatchSize ) // we've hit an edge
							{
								x = 0;
								z += step;
							}
						}
					}
				}
			}
		}
//{ c8 tmp[255]; sprintf( tmp, "highpoint %d, highpatch %d, patchcount %d, patchsize %d", highest, highestpatch, TerrainData.PatchCount, TerrainData.CalcPatchSize ); os::Printer::print( tmp ); }
/*
		if ( DynamicSelectorUpdate && TriangleSelector )
		{
			CTerrainTriangleSelector* selector = (CTerrainTriangleSelector*)TriangleSelector;
			selector->setTriangleData ( this, -1 );
		}
*/
	}


	

	//! Render the scene node
	void CTerrainSphereNode::render()
	{
		if (!IsVisible || !SceneManager->getActiveCamera())
			return;

		if (!Mesh->getMeshBufferCount())
			return;

		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		driver->setTransform (video::ETS_WORLD, core::IdentityMatrix);

		for ( s32 face = 0; face < CUBE_FACES; ++face )
		{
			driver->setMaterial(Mesh->getMeshBuffer(face)->getMaterial());
			RenderBuffer[face]->getIndexBuffer().set_used(IndicesToRender[face]);

			// For use with geomorphing
			driver->drawMeshBuffer(RenderBuffer[face]);

			RenderBuffer[face]->getIndexBuffer().set_used(RenderBuffer[face]->getIndexBuffer().allocated_size());

			// for debug purposes only:
			if (DebugDataVisible)
			{
				video::SMaterial m;
				m.Lighting = false;
				driver->setMaterial(m);
				if (DebugDataVisible & scene::EDS_BBOX)
					driver->draw3DBox(TerrainData.BoundingBox, video::SColor(255,255,255,255));

				const s32 count = TerrainData.PatchCount * TerrainData.PatchCount;
				s32 visible = 0;
			
				if (DebugDataVisible & scene::EDS_BBOX_BUFFERS)
				{
					for (s32 j = 0; j < count; ++j)
					{
						driver->draw3DBox(TerrainData.Patches[j].BoundingBox, video::SColor(255,255,0,0));
						visible += (TerrainData.Patches[j].CurrentLOD >= 0);
					}
				}

				if (DebugDataVisible & scene::EDS_NORMALS)
				{
					IAnimatedMesh * arrow = SceneManager->addArrowMesh(
							"__debugnormal", 0xFFECEC00,
							0xFF999900, 4, 8, 1.f, 0.6f, 0.05f,
							0.3f);
					if (0 == arrow)
					{
						arrow = SceneManager->getMesh( "__debugnormal");
					}
					IMesh *mesh = arrow->getMesh(0);

					// find a good scaling factor

					core::matrix4 m2;

					// draw normals
					driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
					for (u32 i=0; i != RenderBuffer[face]->getVertexCount(); ++i)
					{
						const core::vector3df& v = RenderBuffer[face]->getNormal(i);
						// align to v->Normal
						if (core::vector3df(0,-1,0)==v)
						{
							m2.makeIdentity();
							m2[5]=-m2[5];
						}
						else
						{
							core::quaternion quatRot;
							m2=quatRot.rotationFromTo(v,core::vector3df(0,1,0)).getMatrix();
						}

						m2.setTranslation(RenderBuffer[face]->getPosition(i));
						m2=AbsoluteTransformation*m2;

						driver->setTransform(video::ETS_WORLD, m2 );
						for (u32 a = 0; a != mesh->getMeshBufferCount(); ++a)
							driver->drawMeshBuffer(mesh->getMeshBuffer(a));
					}
					driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
				}
				
				/*
				static u32 lastTime = 0;

				const u32 now = os::Timer::getRealTime();
				if (now - lastTime > 1000)
				{
					char buf[64];
					snprintf(buf, 64, "Count: %d, Visible: %d", count, visible);
					os::Printer::log(buf);

					lastTime = now;
				}
				*/
				
			}
			
		}
		
	}

	const core::aabbox3d<f32>& CTerrainSphereNode::getBoundingBox() const
	{
		return TerrainData.BoundingBox;
	}

	//! Return the bounding box of a patch
	const core::aabbox3d<f32>& CTerrainSphereNode::getBoundingBox( s32 patchX, s32 patchZ, s32 face ) const
	{
		return TerrainData.Patches[face * TerrainData.PatchCount * TerrainData.PatchCount +
								   patchX * TerrainData.PatchCount + patchZ].BoundingBox;
	}

	//! Gets the meshbuffer data based on a specified Level of Detail.
	//! \param mb: A reference to an SMeshBuffer object
	//! \param LOD: The Level Of Detail you want the indices from.
	void CTerrainSphereNode::getMeshBufferForLOD(SMeshBufferLightMap& mb, s32 LOD ) const
	{
		if (!Mesh->getMeshBufferCount())
			return;

		if ( LOD < 0 )
			LOD = 0;
		else if ( LOD > TerrainData.MaxLOD - 1 )
			LOD = TerrainData.MaxLOD - 1;

		s32 numVertices = Mesh->getMeshBuffer( 0 )->getVertexCount ( );
		mb.Vertices.reallocate ( numVertices );
		video::S3DVertex2TCoords* vertices = (video::S3DVertex2TCoords*)Mesh->getMeshBuffer ( 0 )->getVertices ( );

		s32 i;
		for (i=0; i<numVertices; ++i)
			mb.Vertices.push_back(vertices[i]);

		// calculate the step we take for all patches, since LOD is the same
		s32 step = 1 << LOD;
		s32 index11;
		s32 index21;
		s32 index12;
		s32 index22;

		// Generate the indices for all patches at the specified LOD
		for ( s32 face = 0; face < CUBE_FACES; ++face )
		{
			for (i=0; i<TerrainData.PatchCount; ++i)
			{
				for (s32 j=0; j<TerrainData.PatchCount; ++j)
				{
					s32 index = face * TerrainData.PatchCount * TerrainData.PatchCount +
								i * TerrainData.PatchCount + j;
					s32 x = 0;
					s32 z = 0;

					// Loop through patch and generate indices
					while (z < TerrainData.CalcPatchSize)
					{
						index11 = getIndex( j, i, index, x, z );
						index21 = getIndex( j, i, index, x + step, z );
						index12 = getIndex( j, i, index, x, z + step );
						index22 = getIndex( j, i, index, x + step, z + step );

						mb.Indices.push_back( index12 );
						mb.Indices.push_back( index11 );
						mb.Indices.push_back( index22 );
						mb.Indices.push_back( index22 );
						mb.Indices.push_back( index11 );
						mb.Indices.push_back( index21 );

						// increment index position horizontally
						x += step;

						if (x >= TerrainData.CalcPatchSize) // we've hit an edge
						{
							x = 0;
							z += step;
						}
					}
				}
			}
		}
	}

	//! Gets the indices for a specified patch at a specified Level of Detail.
	//! \param mb: A reference to an array of u32 indices.
	//! \param patchX: Patch x coordinate.
	//! \param patchZ: Patch z coordinate.
	//! \param LOD: The level of detail to get for that patch.  If -1, then get
	//! the CurrentLOD.  If the CurrentLOD is set to -1, meaning it's not shown,
	//! then it will retrieve the triangles at the highest LOD ( 0 ).
	//! \return: Number if indices put into the buffer.
	s32 CTerrainSphereNode::getIndicesForPatch(core::array<u32>& indices, s32 patchX, s32 patchZ, s32 face, s32 LOD)
	{
		if ( patchX < 0 || patchX > TerrainData.PatchCount - 1 || patchZ < 0 || patchZ > TerrainData.PatchCount - 1 )
			return -1;

		if ( LOD < -1 || LOD > TerrainData.MaxLOD - 1 )
			return -1;

		s32 rv = 0;
		core::array<s32> cLODs;
		bool setLODs = false;

		// If LOD of -1 was passed in, use the CurrentLOD of the patch specified
		if ( LOD == -1 )
		{
			LOD = TerrainData.Patches[patchX * TerrainData.PatchCount + patchZ].CurrentLOD;
		}
		else
		{
			getCurrentLODOfPatches(cLODs);
			setCurrentLODOfPatches(LOD);
			setLODs = true;
		}

		if ( LOD < 0 )
			return -2; // Patch not visible, don't generate indices.

		// calculate the step we take for this LOD
		s32 step = 1 << LOD;

		// Generate the indices for the specified patch at the specified LOD
		s32 index = face * TerrainData.PatchCount * TerrainData.PatchCount +
					patchX * TerrainData.PatchCount + patchZ;

		s32 x = 0;
		s32 z = 0;
		s32 index11;
		s32 index21;
		s32 index12;
		s32 index22;

		indices.set_used ( TerrainData.PatchSize * TerrainData.PatchSize * 6 );

		// Loop through patch and generate indices
		while (z<TerrainData.CalcPatchSize)
		{
			index11 = getIndex( patchZ, patchX, index, x, z );
			index21 = getIndex( patchZ, patchX, index, x + step, z );
			index12 = getIndex( patchZ, patchX, index, x, z + step );
			index22 = getIndex( patchZ, patchX, index, x + step, z + step );

			indices[rv++] = index12;
			indices[rv++] = index11;
			indices[rv++] = index22;
			indices[rv++] = index22;
			indices[rv++] = index11;
			indices[rv++] = index21;

			// increment index position horizontally
			x += step;

			if (x >= TerrainData.CalcPatchSize) // we've hit an edge
			{
				x = 0;
				z += step;
			}
		}

		if ( setLODs )
			setCurrentLODOfPatches (cLODs);

		return rv;
	}

	//! Populates an array with the CurrentLOD of each patch.
	//! \param LODs: A reference to a core::array<s32> to hold the values
	//! \return Returns the number of elements in the array
	s32 CTerrainSphereNode::getCurrentLODOfPatches(core::array<s32>& LODs) const
	{
		s32 numLODs;
		LODs.clear ( );

		for ( numLODs = 0; numLODs < TerrainData.PatchCount * TerrainData.PatchCount; numLODs++ )
			LODs.push_back ( TerrainData.Patches[numLODs].CurrentLOD );

		return LODs.size();
	}


	//! Manually sets the LOD of a patch
	//! \param patchX: Patch x coordinate.
	//! \param patchZ: Patch z coordinate.
	//! \param LOD: The level of detail to set the patch to.
	void CTerrainSphereNode::setLODOfPatch( s32 patchX, s32 patchZ, s32 face, s32 LOD )
	{
		TerrainData.Patches[face * TerrainData.PatchCount * TerrainData.PatchCount +
							patchX * TerrainData.PatchCount + patchZ].CurrentLOD = LOD;
	}


	//! Override the default generation of distance thresholds for determining the LOD a patch
	//! is rendered at.
	bool CTerrainSphereNode::overrideLODDistance(s32 LOD, f64 newDistance)
	{
		OverrideDistanceThreshold = true;

		if ( LOD < 0 || LOD > TerrainData.MaxLOD - 1 )
			return false;

		TerrainData.LODDistanceThreshold[LOD] = newDistance * newDistance;

		return true;
	}

	//! Creates a planar texture mapping on the terrain
	//! \param resolution: resolution of the planar mapping. This is the value
	//! specifying the relation between world space and texture coordinate space.
	void CTerrainSphereNode::scaleTexture(f32 resolution, f32 resolution2)
	{
		for ( s32 face = 0; face < CUBE_FACES; ++face )
		{
            TCoordScale1 = resolution;
            TCoordScale2 = resolution2;

            const f32 resBySize = resolution / (f32)(TerrainData.Size-1);
            const f32 res2BySize = resolution2 / (f32)(TerrainData.Size-1);
            u32 index = 0;
            f32 xval = 0, zval;
            f32 x2val = 0, z2val=0;

			for (s32 x=0; x<TerrainData.Size; ++x)
			{
				zval=z2val=0;
				for (s32 z=0; z<TerrainData.Size; ++z)
				{
					//RenderBuffer[face]->getVertexBuffer()[index].TCoords
					RenderBuffer[face]->getVertexBuffer()[index].TCoords.X = xval;
					RenderBuffer[face]->getVertexBuffer()[index].TCoords.Y = zval;

				    // adjustments for face allignment
				    switch(face)
				    {
				        case 0:
//                            px=TerrainData.Size-1-x; pz=z;
                            RenderBuffer[face]->getVertexBuffer()[index].TCoords.X = resBySize-1-xval;
                            RenderBuffer[face]->getVertexBuffer()[index].TCoords.Y = zval;
                            break;
				        case 1:
//                            px=TerrainData.Size-1-x; pz=TerrainData.Size-1-z;
                            RenderBuffer[face]->getVertexBuffer()[index].TCoords.X = resBySize-1-xval;
                            RenderBuffer[face]->getVertexBuffer()[index].TCoords.Y = resBySize-1-zval;
                            break;
				        case 2:
//                            px=x; pz=TerrainData.Size-1-z;
                            RenderBuffer[face]->getVertexBuffer()[index].TCoords.X = xval;
                            RenderBuffer[face]->getVertexBuffer()[index].TCoords.Y = resBySize-1-zval;
                            break;
				        case 3:
//                            px=z; pz=TerrainData.Size-1-x;
                            RenderBuffer[face]->getVertexBuffer()[index].TCoords.X = zval;
                            RenderBuffer[face]->getVertexBuffer()[index].TCoords.Y = resBySize-1-xval;
                            break;
				        case 4:
//                            px=TerrainData.Size-1-z; pz=TerrainData.Size-1-x;
                            RenderBuffer[face]->getVertexBuffer()[index].TCoords.X = resBySize-1-zval;
                            RenderBuffer[face]->getVertexBuffer()[index].TCoords.Y = resBySize-1-xval;
                            break;
				        case 5:
//                            px=x; pz=z;
                            RenderBuffer[face]->getVertexBuffer()[index].TCoords.X = xval;
                            RenderBuffer[face]->getVertexBuffer()[index].TCoords.Y = zval;
                            break;
				    }

					/*
					if ( resolution2 == 0 )
					{
						RenderBuffer[face].Vertices[index].TCoords2 = RenderBuffer[face].Vertices[index].TCoords;
					}
					else
					{
//						RenderBuffer[face].Vertices[index].TCoords2.X = x2val;
//						RenderBuffer[face].Vertices[index].TCoords2.Y = z2val;
                        // adjustments for face allignment
                        switch(face)
                        {
                            case 0:
    //                            px=TerrainData.Size-1-x; pz=z;
                                RenderBuffer[face].Vertices[index].TCoords2.X = res2BySize-1-x2val;
                                RenderBuffer[face].Vertices[index].TCoords2.Y = z2val;
                                break;
                            case 1:
    //                            px=TerrainData.Size-1-x; pz=TerrainData.Size-1-z;
                                RenderBuffer[face].Vertices[index].TCoords2.X = res2BySize-1-x2val;
                                RenderBuffer[face].Vertices[index].TCoords2.Y = res2BySize-1-z2val;
                                break;
                            case 2:
    //                            px=x; pz=TerrainData.Size-1-z;
                                RenderBuffer[face].Vertices[index].TCoords2.X = x2val;
                                RenderBuffer[face].Vertices[index].TCoords2.Y = res2BySize-1-z2val;
                                break;
                            case 3:
    //                            px=z; pz=TerrainData.Size-1-x;
                                RenderBuffer[face].Vertices[index].TCoords2.X = z2val;
                                RenderBuffer[face].Vertices[index].TCoords2.Y = res2BySize-1-x2val;
                                break;
                            case 4:
    //                            px=TerrainData.Size-1-z; pz=TerrainData.Size-1-x;
                                RenderBuffer[face].Vertices[index].TCoords2.X = res2BySize-1-z2val;
                                RenderBuffer[face].Vertices[index].TCoords2.Y = res2BySize-1-x2val;
                                break;
                            case 5:
    //                            px=x; pz=z;
                                RenderBuffer[face].Vertices[index].TCoords2.X = x2val;
                                RenderBuffer[face].Vertices[index].TCoords2.Y = z2val;
                                break;
                        }
					}
					*/
					++index;
					zval += resBySize;
					z2val += res2BySize;
				}
				xval += resBySize;
				x2val += res2BySize;
			}
		}
	}

	//! used to get the indices when generating index data for patches at varying levels of detail.
	u32 CTerrainSphereNode::getIndex(const s32& PatchX, const s32& PatchZ,
					const s32& PatchIndex, u32 vX, u32 vZ) const
	{
		// top border
		if (vZ == 0)
		{
			if (TerrainData.Patches[PatchIndex].Top &&
				TerrainData.Patches[PatchIndex].CurrentLOD < TerrainData.Patches[PatchIndex].Top->CurrentLOD &&
				(vX % ( 1 << TerrainData.Patches[PatchIndex].Top->CurrentLOD)) != 0 )
			{
				vX = vX - vX % ( 1 << TerrainData.Patches[PatchIndex].Top->CurrentLOD );
			}
		}
		else
		if ( vZ == (u32)TerrainData.CalcPatchSize ) // bottom border
		{
			if (TerrainData.Patches[PatchIndex].Bottom &&
				TerrainData.Patches[PatchIndex].CurrentLOD < TerrainData.Patches[PatchIndex].Bottom->CurrentLOD &&
				(vX % ( 1 << TerrainData.Patches[PatchIndex].Bottom->CurrentLOD)) != 0)
			{
				vX = vX - vX % ( 1 << TerrainData.Patches[PatchIndex].Bottom->CurrentLOD );
			}
		}

		// left border
		if ( vX == 0 )
		{
			if (TerrainData.Patches[PatchIndex].Left &&
				TerrainData.Patches[PatchIndex].CurrentLOD < TerrainData.Patches[PatchIndex].Left->CurrentLOD &&
				( vZ % ( 1 << TerrainData.Patches[PatchIndex].Left->CurrentLOD ) ) != 0)
			{
				vZ = vZ - vZ % ( 1 << TerrainData.Patches[PatchIndex].Left->CurrentLOD );
			}
		}
		else
		if  ( vX == (u32)TerrainData.CalcPatchSize ) // right border
		{
			if (TerrainData.Patches[PatchIndex].Right &&
				TerrainData.Patches[PatchIndex].CurrentLOD < TerrainData.Patches[PatchIndex].Right->CurrentLOD &&
				( vZ %  ( 1 << TerrainData.Patches[PatchIndex].Right->CurrentLOD ) ) != 0)
			{
				vZ = vZ - vZ % ( 1 << TerrainData.Patches[PatchIndex].Right->CurrentLOD );
			}
		}

		if ( vZ >= (u32)TerrainData.PatchSize )
			vZ = TerrainData.CalcPatchSize;

		if ( vX >= (u32)TerrainData.PatchSize )
			vX = TerrainData.CalcPatchSize;

		if (vZ + ((TerrainData.CalcPatchSize) * PatchZ) >= (u32)TerrainData.Size ) vZ--;
		if (vX + ((TerrainData.CalcPatchSize) * PatchX) >= (u32)TerrainData.Size ) vX--;

		return (vZ + ((TerrainData.CalcPatchSize) * PatchZ)) * TerrainData.Size +
			   (vX + ((TerrainData.CalcPatchSize) * PatchX));
	}

	//! calculate smooth normals
	void CTerrainSphereNode::calculateNormals (scene::CDynamicMeshBuffer* pMeshBuffer, s32 face )
	{
		s32 count;
		core::vector3df a, b, c, t;

		for (s32 x=0; x<TerrainData.Size; ++x)
		{
			for (s32 z=0; z<TerrainData.Size; ++z)
			{
				count = 0;
				core::vector3df normal;

				// top left
				if (x>0 && z>0)
				{
                    // Invert the normals of the front left and bottom faces
                    if (( face == 1 ) || ( face == 3 ) || ( face == 5 ))
                    {
                        c = pMeshBuffer->getVertexBuffer()[(x-1)*TerrainData.Size+z-1].Pos;
                        b = pMeshBuffer->getVertexBuffer()[(x-1)*TerrainData.Size+z].Pos;
                        a = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z].Pos;
                    }
                    else
                    {
                        a = pMeshBuffer->getVertexBuffer()[(x-1)*TerrainData.Size+z-1].Pos;
                        b = pMeshBuffer->getVertexBuffer()[(x-1)*TerrainData.Size+z].Pos;
                        c = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z].Pos;
                    }
					b -= a;
					c -= a;
					t = b.crossProduct ( c );
					t.normalize ( );
					normal += t;

                    // Invert the normals of the front left and bottom faces
                    if (( face == 1 ) || ( face == 3 ) || ( face == 5 ))
                    {
                        c = pMeshBuffer->getVertexBuffer()[(x-1)*TerrainData.Size+z-1].Pos;
                        b = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z-1].Pos;
                        a = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z].Pos;
                    }
                    else
                    {
                        a = pMeshBuffer->getVertexBuffer()[(x-1)*TerrainData.Size+z-1].Pos;
                        b = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z-1].Pos;
                        c = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z].Pos;
                    }
					b -= a;
					c -= a;
					t = b.crossProduct ( c );
					t.normalize ( );
					normal += t;

					count += 2;
				}
				else
				{
				    /* TO DO: we are at the edge of a face at the moment just
				    set it as a flat normal on the sphere this would be better
				    properly handled to take into account adjacent faces */
				    normal = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z].Pos;
	    			count += 2;
				}


				// top right
				if (x>0 && z<TerrainData.Size-1)
				{
                    // Invert the normals of the front left and bottom faces
                    if (( face == 1 ) || ( face == 3 ) || ( face == 5 ))
                    {
                        c = pMeshBuffer->getVertexBuffer()[(x-1)*TerrainData.Size+z].Pos;
                        b = pMeshBuffer->getVertexBuffer()[(x-1)*TerrainData.Size+z+1].Pos;
                        a = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z+1].Pos;
                    }
                    else
                    {
                        a = pMeshBuffer->getVertexBuffer()[(x-1)*TerrainData.Size+z].Pos;
                        b = pMeshBuffer->getVertexBuffer()[(x-1)*TerrainData.Size+z+1].Pos;
                        c = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z+1].Pos;
                    }
					b -= a;
					c -= a;
					t = b.crossProduct ( c );
					t.normalize ( );
					normal += t;

                    // Invert the normals of the front left and bottom faces
                    if (( face == 1 ) || ( face == 3 ) || ( face == 5 ))
                    {
                        c = pMeshBuffer->getVertexBuffer()[(x-1)*TerrainData.Size+z].Pos;
                        b = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z+1].Pos;
                        a = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z].Pos;
                    }
                    else
                    {
                        a = pMeshBuffer->getVertexBuffer()[(x-1)*TerrainData.Size+z].Pos;
                        b = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z+1].Pos;
                        c = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z].Pos;
                    }
					b -= a;
					c -= a;
					t = b.crossProduct ( c );
					t.normalize ( );
					normal += t;

					count += 2;
				}
				else
				{
				    /* TO DO: we are at the edge of a face at the moment just
				    set it as a flat normal on the sphere this would be better
				    properly handled to take into account adjacent faces */
				    normal = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z].Pos;
	    			count += 2;
				}

				// bottom right
				if (x<TerrainData.Size-1 && z<TerrainData.Size-1)
				{
                    // Invert the normals of the front left and bottom faces
                    if (( face == 1 ) || ( face == 3 ) || ( face == 5 ))
                    {
                        c = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z+1].Pos;
                        b = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z].Pos;
                        a = pMeshBuffer->getVertexBuffer()[(x+1)*TerrainData.Size+z+1].Pos;
                    }
                    else
                    {
                        a = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z+1].Pos;
                        b = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z].Pos;
                        c = pMeshBuffer->getVertexBuffer()[(x+1)*TerrainData.Size+z+1].Pos;
                    }
					b -= a;
					c -= a;
					t = b.crossProduct ( c );
					t.normalize ( );
					normal += t;

                    // Invert the normals of the front left and bottom faces
                    if (( face == 1 ) || ( face == 3 ) || ( face == 5 ))
                    {
                        c = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z+1].Pos;
                        b = pMeshBuffer->getVertexBuffer()[(x+1)*TerrainData.Size+z+1].Pos;
                        a = pMeshBuffer->getVertexBuffer()[(x+1)*TerrainData.Size+z].Pos;
                    }
                    else
                    {
                        a = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z+1].Pos;
                        b = pMeshBuffer->getVertexBuffer()[(x+1)*TerrainData.Size+z+1].Pos;
                        c = pMeshBuffer->getVertexBuffer()[(x+1)*TerrainData.Size+z].Pos;
                    }
					b -= a;
					c -= a;
					t = b.crossProduct ( c );
					t.normalize ( );
					normal += t;

					count += 2;
				}
				else
				{
				    /* TO DO: we are at the edge of a face at the moment just
				    set it as a flat normal on the sphere this would be better
				    properly handled to take into account adjacent faces */
				    normal = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z].Pos;
	    			count += 2;
				}

				// bottom left
				if (x<TerrainData.Size-1 && z>0)
				{
                    // Invert the normals of the front left and bottom faces
                    if (( face == 1 ) || ( face == 3 ) || ( face == 5 ))
                    {
                        c = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z-1].Pos;
                        b = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z].Pos;
                        a = pMeshBuffer->getVertexBuffer()[(x+1)*TerrainData.Size+z].Pos;
                    }
                    else
                    {
                        a = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z-1].Pos;
                        b = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z].Pos;
                        c = pMeshBuffer->getVertexBuffer()[(x+1)*TerrainData.Size+z].Pos;
                    }
					b -= a;
					c -= a;
					t = b.crossProduct ( c );
					t.normalize ( );
					normal += t;

                    // Invert the normals of the front left and bottom faces
                    if (( face == 1 ) || ( face == 3 ) || ( face == 5 ))
                    {
                        c = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z-1].Pos;
                        b = pMeshBuffer->getVertexBuffer()[(x+1)*TerrainData.Size+z].Pos;
                        a = pMeshBuffer->getVertexBuffer()[(x+1)*TerrainData.Size+z-1].Pos;
                    }
                    else
                    {
                        a = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z-1].Pos;
                        b = pMeshBuffer->getVertexBuffer()[(x+1)*TerrainData.Size+z].Pos;
                        c = pMeshBuffer->getVertexBuffer()[(x+1)*TerrainData.Size+z-1].Pos;
                    }
					b -= a;
					c -= a;
					t = b.crossProduct ( c );
					t.normalize ( );
					normal += t;

					count += 2;
				}
				else
				{
				    /* TO DO: we are at the edge of a face at the moment just
				    set it as a flat normal on the sphere this would be better
				    properly handled to take into account adjacent faces */
				    normal = pMeshBuffer->getVertexBuffer()[x*TerrainData.Size+z].Pos;
	    			count += 2;
				}

				if ( count != 0 )
				{
					normal.normalize ( );
				}
				else
				{
					normal.set( 0.0f, 1.0f, 0.0f );
				}

				pMeshBuffer->getVertexBuffer()[x * TerrainData.Size + z].Normal = normal;
			}
		}
	}

	//! create patches, stuff that needs to be done only once for patches goes here.
	void CTerrainSphereNode::createPatches()
	{
		TerrainData.PatchCount = (TerrainData.Size ) / ( TerrainData.CalcPatchSize );

		if (TerrainData.Patches)
			delete [] TerrainData.Patches;

		TerrainData.Patches = new SPatch[TerrainData.PatchCount * TerrainData.PatchCount * CUBE_FACES];
	}



#define TOP_FACE
#define FRONT_FACE	patchesPerFace
#define BACK_FACE	(patchesPerFace * 2)
#define LEFT_FACE	(patchesPerFace * 3)
#define RIGHT_FACE	(patchesPerFace * 4)
#define BOTTOM_FACE	(patchesPerFace * 5)

#define TOP_EDGE    index
#define LEFT_EDGE   ( TerrainData.PatchCount ) * index
#define RIGHT_EDGE  (( TerrainData.PatchCount - 1 ) + ( TerrainData.PatchCount ) * index)
#define BOTTOM_EDGE ((( TerrainData.PatchCount - 1 ) * ( TerrainData.PatchCount )) + index)

	CTerrainSphereNode::SPatch * CTerrainSphereNode::cubePatch( s32 face, s32 edge, s32 index )
	{
		s32 patchIndex = 0;
		s32 patchesPerFace = TerrainData.PatchCount * TerrainData.PatchCount;

		switch ( face )
		{
			case 0:
				// Face 0: Top face
				switch ( edge )
				{
					case 0:
						// matched to edge 1 of the right face
						patchIndex = RIGHT_FACE + BOTTOM_EDGE;
						break;

					case 1:
						// matched to edge 0 of the left face
						patchIndex = LEFT_FACE + BOTTOM_EDGE;
						break;

					case 2:
						// matched to edge 3 of the back face
						patchIndex = BACK_FACE + RIGHT_EDGE;
						break;

					case 3:
						// matched to edge 2 of the front face
						patchIndex = FRONT_FACE + RIGHT_EDGE;
						break;
				}
				break;

			case 1:
				// Face 1: front face swap z and y
				switch ( edge )
				{
					case 0:
						// matched to edge 1 of the right face
						patchIndex = RIGHT_FACE + RIGHT_EDGE;
						break;

					case 1:
						// matched to edge 0 of the left face
						patchIndex = LEFT_FACE + RIGHT_EDGE;
						break;

					case 2:
						// matched to edge 3 of the back face
						patchIndex = BOTTOM_FACE + RIGHT_EDGE;
						break;

					case 3:
						// matched to edge 2 of the front face
						patchIndex = TOP_FACE + RIGHT_EDGE;
						break;
				}
				break;

			case 2:
				// Face 2: back face swap z and y and invert y
				switch ( edge )
				{
					case 0:
						// matched to edge 1 of the right face
						patchIndex = RIGHT_FACE + LEFT_EDGE;
						break;

					case 1:
						// matched to edge 0 of the left face
						patchIndex = LEFT_FACE + LEFT_EDGE;
						break;

					case 2:
						// matched to edge 3 of the back face
						patchIndex = BOTTOM_FACE + LEFT_EDGE;
						break;

					case 3:
						// matched to edge 2 of the top face
						patchIndex = TOP_FACE + LEFT_EDGE;
						break;
				}
				break;

			case 3:
				// Face 3: left face swap x and y
				switch ( edge )
				{
					case 0:
						// matched to edge 1 of the top face
						patchIndex = BOTTOM_FACE +	BOTTOM_EDGE;
						break;

					case 1:
						// matched to edge 0 of the left face
						patchIndex = TOP_FACE + BOTTOM_EDGE;
						break;

					case 2:
						// matched to edge 3 of the back face
						patchIndex = BACK_FACE + BOTTOM_EDGE;
						break;

					case 3:
						// matched to edge 2 of the top face
						patchIndex = FRONT_FACE + BOTTOM_EDGE;
						break;
				}
				break;

			case 4:
				// Face 4:right face
				switch ( edge )
				{
					case 0:
						// matched to edge 1 of the top face
						patchIndex = BOTTOM_FACE +	TOP_EDGE;
						break;

					case 1:
						// matched to edge 0 of the top face
						patchIndex = TOP_FACE + TOP_EDGE;
						break;

					case 2:
						// matched to edge 3 of the back face
						patchIndex = BACK_FACE + TOP_EDGE;
						break;

					case 3:
						// matched to edge 2 of the top face
						patchIndex = FRONT_FACE + TOP_EDGE;
						break;
				}
				break;

			case 5:
				// Face 5: bottom face invert y
				switch ( edge )
				{
					case 0:
						// matched to edge 1 of the top face
						patchIndex = RIGHT_FACE + TOP_EDGE;
						break;

					case 1:
						// matched to edge 0 of the top face
						patchIndex = LEFT_FACE + TOP_EDGE;
						break;

					case 2:
						// matched to edge 3 of the back face
						patchIndex = BACK_FACE + LEFT_EDGE;
						break;

					case 3:
						// matched to edge 2 of the top face
						patchIndex = FRONT_FACE + LEFT_EDGE;
//						printf( " Face %d, Edge %d, Index %d = %d\n", face, edge, index, patchIndex );
//						if ( index == 4 ) debug = patchIndex;
						break;
				}
				break;

			default:
				// an unknown face
				break;
		}

		return &TerrainData.Patches[patchIndex];
	}



	//! used to calculate the internal STerrainData structure both at creation and after scaling/position calls.
	void CTerrainSphereNode::calculatePatchData()
	{
		// Reset the Terrains Bounding Box for re-calculation
		TerrainData.BoundingBox = core::aabbox3df ( 999999.9f, 999999.9f, 999999.9f, -999999.9f, -999999.9f, -999999.9f );

		for ( s32 face = 0; face < CUBE_FACES; ++face )
		{
		    s32 patchesToFace = face * TerrainData.PatchCount * TerrainData.PatchCount;

			for( s32 x = 0; x < TerrainData.PatchCount; ++x )
			{
				for( s32 z = 0; z < TerrainData.PatchCount; ++z )
				{
					s32 index = patchesToFace +	x * TerrainData.PatchCount + z;
					TerrainData.Patches[index].CurrentLOD = 0;

					// For each patch, calculate the bounding box ( mins and maxes )
					TerrainData.Patches[index].BoundingBox =  core::aabbox3df (999999.9f, 999999.9f, 999999.9f,
						-999999.9f, -999999.9f, -999999.9f );

					for( s32 xx = x*(TerrainData.CalcPatchSize); xx < ( x + 1 ) * TerrainData.CalcPatchSize; ++xx )
						for( s32 zz = z*(TerrainData.CalcPatchSize); zz < ( z + 1 ) * TerrainData.CalcPatchSize; ++zz )
							TerrainData.Patches[index].BoundingBox.addInternalPoint( RenderBuffer[face]->getVertexBuffer()[xx * TerrainData.Size + zz].Pos );

					// Reconfigure the bounding box of the terrain as a whole
					TerrainData.BoundingBox.addInternalBox( TerrainData.Patches[index].BoundingBox );

					// get center of Patch
					TerrainData.Patches[index].Center = TerrainData.Patches[index].BoundingBox.getCenter();

					// Assign Neighbours
					// Top
					if( x > 0 )
						TerrainData.Patches[index].Top = &TerrainData.Patches[patchesToFace + (x-1) * TerrainData.PatchCount + z];
					else
						TerrainData.Patches[index].Top = cubePatch( face, 0, z );

					// Bottom
					if( x < TerrainData.PatchCount - 1 )
						TerrainData.Patches[index].Bottom = &TerrainData.Patches[patchesToFace + (x+1) * TerrainData.PatchCount + z];
					else
						TerrainData.Patches[index].Bottom = cubePatch( face, 1, z );

					// Left
					if( z > 0 )
						TerrainData.Patches[index].Left = &TerrainData.Patches[patchesToFace + x * TerrainData.PatchCount + z - 1];
					else
						TerrainData.Patches[index].Left = cubePatch( face, 2, x );

					// Right
					if( z < TerrainData.PatchCount - 1 )
						TerrainData.Patches[index].Right = &TerrainData.Patches[patchesToFace + x * TerrainData.PatchCount + z + 1];
					else
						TerrainData.Patches[index].Right = cubePatch( face, 3, x );

					if ( TerrainData.Patches[index].DebugText )
					{
						TerrainData.Patches[index].DebugText->setPosition ( TerrainData.Patches[index].Center );
					}
if (( debug ) && !(debug2)) debug2 = index;
				}
			}
		}

		// get center of Terrain
		TerrainData.Center = TerrainData.BoundingBox.getCenter();

		// if the default rotation pivot is still being used, update it.
		if( UseDefaultRotationPivot )
		{
			TerrainData.RotationPivot = TerrainData.Center;
		}
	}


	//! used to calculate or recalculate the distance thresholds
	void CTerrainSphereNode::calculateDistanceThresholds(bool scalechanged)
	{
		// Only update the LODDistanceThreshold if it's not manually changed
		if (!OverrideDistanceThreshold)
		{
			if( TerrainData.LODDistanceThreshold )
			{
				delete [] TerrainData.LODDistanceThreshold;
			}

			// Determine new distance threshold for determining what LOD to draw patches at
			TerrainData.LODDistanceThreshold = new f64[TerrainData.MaxLOD];

			for (s32 i=0; i<TerrainData.MaxLOD; ++i)
			{
				TerrainData.LODDistanceThreshold[i] =
					(TerrainData.PatchSize * TerrainData.PatchSize) *
					(TerrainData.Scale.X * TerrainData.Scale.Z) *
					((i+1+ i / 2) * (i+1+ i / 2));
			}
		}
	}

	void CTerrainSphereNode::setCurrentLODOfPatches(s32 lod)
	{
		for (s32 i=0; i< TerrainData.PatchCount * TerrainData.PatchCount; ++i)
			TerrainData.Patches[i].CurrentLOD = lod;
	}

	void CTerrainSphereNode::setCurrentLODOfPatches(core::array<s32>& lodarray)
	{
		for (s32 i=0; i<TerrainData.PatchCount * TerrainData.PatchCount; ++i)
			TerrainData.Patches[i].CurrentLOD = lodarray[i];
	}


	//! Gets the height
	f32 CTerrainSphereNode::getHeight( f32 x, f32 z ) const
	{
		if (!Mesh->getMeshBufferCount())
			return 0;

		f32 height = -999999.9f;

		core::matrix4 rotMatrix;
		rotMatrix.setRotationDegrees( TerrainData.Rotation );
		core::vector3df pos( x, 0.0f, z );
		rotMatrix.rotateVect( pos );
		pos -= TerrainData.Position;
		pos /= TerrainData.Scale;

		s32 X(core::floor32( pos.X ));
		s32 Z(core::floor32( pos.Z ));

		if( X >= 0 && X < TerrainData.Size && Z >= 0 && Z <= TerrainData.Size )
		{
			const video::S3DVertex2TCoords* Vertices = (const video::S3DVertex2TCoords*)Mesh->getMeshBuffer( 0 )->getVertices();
			const core::vector3df& a = Vertices[ X * TerrainData.Size + Z ].Pos;
			const core::vector3df& b = Vertices[ (X + 1) * TerrainData.Size + Z ].Pos;
			const core::vector3df& c = Vertices[ X * TerrainData.Size + ( Z + 1 ) ].Pos;
			const core::vector3df& d = Vertices[ (X + 1) * TerrainData.Size + ( Z + 1 ) ].Pos;

			// offset from integer position
			const f32 dx = pos.X - X;
			const f32 dz = pos.Z - Z;

			if( dx > dz )
				height = a.Y + (d.Y - b.Y)*dz + (b.Y - a.Y)*dx;
			else
				height = a.Y + (d.Y - c.Y)*dx + (c.Y - a.Y)*dz;

			height *= TerrainData.Scale.Y;
			height += TerrainData.Position.Y;
		}

		return height;
	}


	//! Gets a surface position
	void CTerrainSphereNode::getSurfacePosition( s32 face, f32 x, f32 z, core::vector3df &pos ) const
	{
		if ((!Mesh->getMeshBufferCount()) || (face < 0 ) || ( face > 5 ))
		{
		    pos.set( 0.0f, 0.0f, 0.0f );
            return;
		}

        // adjustments for face allignment
        f32 sizeX = (TerrainData.Size - 1.0f);
        f32 sizeZ = (TerrainData.Size - 1.0f);
        f32 px=x, pz=z;
        switch(face)
        {
            case 0:
                x=sizeX-px; z=pz;
                break;
            case 1:
                x=sizeX-px; z=sizeZ-pz;
                break;
            case 2:
                x=px; z=sizeZ-pz;
                break;
            case 3:
                x=sizeZ-pz; z=px;
                break;
            case 4:
                x=sizeZ-pz; z=sizeX-px;
                break;
            case 5:
                x=px; z=pz;
                break;
        }

        // offset from integer position
        s32 X, Z;
        f32 dx, dz;
        if ( x < TerrainData.Size - 1 )
        {
            X = core::floor32( x );
            dx = x - X;
        }
        else
        {
            dx = 1.0;
            X = TerrainData.Size - 2;
        }

        if ( z < TerrainData.Size - 1 )
        {
            Z = core::floor32( z );
            dz = z - Z;
        }
        else
        {
            dz = 1.0;
            Z = TerrainData.Size - 2;
        }


		if( X >= 0 && X < TerrainData.Size - 1 && Z >= 0 && Z < TerrainData.Size - 1 )
		{
			const video::S3DVertex2TCoords* Vertices = (const video::S3DVertex2TCoords*)Mesh->getMeshBuffer( face )->getVertices();
			const core::vector3df& a = Vertices[ X * TerrainData.Size + Z ].Pos;
			const core::vector3df& b = Vertices[ (X + 1) * TerrainData.Size + Z ].Pos;
			const core::vector3df& c = Vertices[ X * TerrainData.Size + ( Z + 1 ) ].Pos;
			const core::vector3df& d = Vertices[ (X + 1) * TerrainData.Size + ( Z + 1 ) ].Pos;

			if( dx > dz )
			{
				pos.X = a.X + (d.X - b.X)*dz + (b.X - a.X)*dx;
				pos.Y = a.Y + (d.Y - b.Y)*dz + (b.Y - a.Y)*dx;
				pos.Z = a.Z + (d.Z - b.Z)*dz + (b.Z - a.Z)*dx;
			}
			else
			{
				pos.X = a.X + (d.X - c.X)*dx + (c.X - a.X)*dz;
				pos.Y = a.Y + (d.Y - c.Y)*dx + (c.Y - a.Y)*dz;
				pos.Z = a.Z + (d.Z - c.Z)*dx + (c.Z - a.Z)*dz;
			}

			pos *= TerrainData.Scale;
			pos += TerrainData.Position;
		}
	}


	//! Writes attributes of the scene node.
	void CTerrainSphereNode::serializeAttributes(io::IAttributes* out,
				io::SAttributeReadWriteOptions* options) const
	{
		ISceneNode::serializeAttributes(out, options);

		out->addString("Heightmap", HeightmapFile.c_str());
		out->addFloat("TextureScale1", TCoordScale1);
		out->addFloat("TextureScale2", TCoordScale2);
	}


	//! Reads attributes of the scene node.
	void CTerrainSphereNode::deserializeAttributes(io::IAttributes* in,
													io::SAttributeReadWriteOptions* options)
	{
		core::stringc newHeightmap = in->getAttributeAsString("Heightmap");
		f32 tcoordScale1 = in->getAttributeAsFloat("TextureScale1");
		f32 tcoordScale2 = in->getAttributeAsFloat("TextureScale2");

		// set possible new heightmap

		if (newHeightmap.size() > 0 &&
			newHeightmap != HeightmapFile)
		{
			io::IReadFile* file = FileSystem->createAndOpenFile(newHeightmap.c_str());
			if (file)
			{
//				loadHeightMap(file, video::SColor(255,255,255,255), 0);
				file->drop();
			}
//			else
//				os::Printer::log("could not open heightmap", newHeightmap.c_str());
		}

		// set possible new scale

		if (core::equals(tcoordScale1, 0.f))
			tcoordScale1 = 1.0f;

		if (core::equals(tcoordScale2, 0.f))
			tcoordScale2 = 1.0f;

		if (!core::equals(tcoordScale1, TCoordScale1) ||
			!core::equals(tcoordScale2, TCoordScale2))
		{
			scaleTexture(tcoordScale1, tcoordScale2);
		}

		ISceneNode::deserializeAttributes(in, options);
	}


	//! Creates a clone of this scene node and its children.
	ISceneNode* CTerrainSphereNode::clone(ISceneNode* newParent, ISceneManager* newManager)
	{
		if (!newParent)
			newParent = Parent;
		if (!newManager)
			newManager = SceneManager;

		CTerrainSphereNode* nb = new CTerrainSphereNode(
			newParent, newManager, FileSystem, ID,
			4, ETPS_17, getPosition(), getRotation(), getScale());

		nb->cloneMembers(this, newManager);

		// instead of cloning the data structures, recreate the terrain.
		// (temporary solution)

		// load file

		io::IReadFile* file = FileSystem->createAndOpenFile(HeightmapFile.c_str());
		if (file)
		{
//			nb->loadHeightMap(file, video::SColor(255,255,255,255), 0);
			file->drop();
		}

		// scale textures

		nb->scaleTexture(TCoordScale1, TCoordScale2);

		// copy materials

		for (unsigned int m = 0; m<Mesh->getMeshBufferCount(); ++m)
		{
			if (nb->Mesh->getMeshBufferCount()>m &&
				nb->Mesh->getMeshBuffer(m) &&
				Mesh->getMeshBuffer(m))
			{
				nb->Mesh->getMeshBuffer(m)->getMaterial() =
					Mesh->getMeshBuffer(m)->getMaterial();
			}
		}

		for ( s32 face = 0; face < CUBE_FACES; ++face )
		{
			nb->RenderBuffer[face]->getMaterial() = RenderBuffer[face]->getMaterial();
		}

		// finish

		nb->drop();
		return nb;
	}

} // end namespace scene
} // end namespace irr

