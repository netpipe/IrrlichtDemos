/*
	Programmed by Do-young Chung

	Programmed Date 080526

	For:

	TSTerrain(Texture Splatting Terrain) SceneNode

*/

#define _USING_SMOOTH_FACTOR


#include "TSTerrainSceneNode.h"


// Constructor and Deconstructor ------------------------------------------------//

TSTerrainSceneNode::TSTerrainSceneNode(
	ILogger					*logger,
	scene::ISceneManager	*sceneManager,
	ITimer					*timer,
	const c8*				name,
	const c8*				heightMapFileName,
	const core::dimension2d<f32>&	stretchSize,
	f32								maxHeight,
	const core::dimension2d<s32>&	defaultVertexBlockSize,
	f32 text1Scale, f32 text2Scale, f32 text3Scale, f32 text4Scale,
	scene::ISceneNode* parent/* = 0*/, s32 id/* = -1*/):
		scene::ISceneNode( parent?parent:sceneManager->getRootSceneNode(), sceneManager, id),
		my_sceneManager(sceneManager), my_timer(timer),
		my_detailTextScale(1.0f), my_lightMapTextScale(1.0f), my_colorMapTextScale(1.0f),
		my_text1Scale(text1Scale), my_text2Scale(text2Scale), my_text3Scale(text3Scale), my_text4Scale(text4Scale)
{
	#ifdef _DEBUG
	setDebugName("TSTerrainSceneNode");
	#endif

	my_logger		= logger;

	my_videoDriver	= my_sceneManager->getVideoDriver();

	my_mesh			= NULL;

	my_size;

	video::IImage *heightMapImage = this->my_videoDriver->createImageFromFile(heightMapFileName);

	if(this->loadHeightMap(name, heightMapImage, stretchSize, maxHeight, defaultVertexBlockSize, false))
	{
		video::IGPUProgrammingServices* GPUProgrammingServices = my_videoDriver->getGPUProgrammingServices();

		std::string textureSplattingPixelShader		= "";
		std::string textureSplattingVertexShader	= "";

		if (my_videoDriver->getDriverType()		== video::EDT_DIRECT3D9)
		{
			textureSplattingPixelShader		= "./TSTerrainSN_Data/textureSplatting_ps.hlsl";
			textureSplattingVertexShader	= "./TSTerrainSN_Data/textureSplatting_vs.hlsl";
		}
		/*
		DC NOTE: For now, it's not working for OpenGL

		else if(my_videoDriver->getDriverType()	== video::EDT_OPENGL)
		{
			textureSplattingPixelShader		= "textureSplatting_ps.glsl";
			textureSplattingVertexShader	= "textureSplatting_vs.glsl";
		}
		*/

		my_solidTSMaterial = GPUProgrammingServices->addHighLevelShaderMaterialFromFiles(
			textureSplattingVertexShader.c_str(),	"main", video::EVST_VS_2_0,
			textureSplattingPixelShader.c_str(),	"main", video::EPST_PS_2_0,
			this,
			video::EMT_LIGHTMAP);

		my_transparentTSMaterial = GPUProgrammingServices->addHighLevelShaderMaterialFromFiles(
			textureSplattingVertexShader.c_str(),	"main", video::EVST_VS_2_0,
			textureSplattingPixelShader.c_str(),	"main", video::EPST_PS_2_0,
			this,
			video::EMT_TRANSPARENT_ADD_COLOR);

		this->setMaterialType((video::E_MATERIAL_TYPE)my_solidTSMaterial);
	}
	else
	{
		my_solidTSMaterial			= -1;
		my_transparentTSMaterial	= -1;
	}

	if(heightMapImage != NULL)
		heightMapImage->drop();

	this->isTransparent = false;
}

TSTerrainSceneNode::~TSTerrainSceneNode()
{
	if(my_mesh != NULL)
		my_mesh->drop();
}


// Inherited From scene::ISceneNode ---------------------------------------------//

video::SMaterial& TSTerrainSceneNode::getMaterial(u32 i)
{
	if (i >= this->my_materials.size())
		return ISceneNode::getMaterial(i);

	return this->my_materials[i];
}

u32 TSTerrainSceneNode::getMaterialCount()
{
	return this->my_materials.size();
}

void TSTerrainSceneNode::OnRegisterSceneNode()
{
	if(this->IsVisible)
		this->my_sceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

void TSTerrainSceneNode::OnAnimate(u32 timeMs)
{
	if(this->IsVisible)
	{

	}

	ISceneNode::OnAnimate(timeMs);
}

void TSTerrainSceneNode::render()
{
	/*
		DC NOTE: Code is partially borrowed from CAnimatedMeshSceneNode(.cpp)
	*/
	if(this->IsVisible && this->my_mesh != NULL)
	{
		this->my_videoDriver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

		bool isTransparentPass =
			this->my_sceneManager->getSceneNodeRenderPass() == scene::ESNRP_TRANSPARENT;

		u32 i = 0;

	//	IDirect3DDevice9 *d3dDevice = this->my_videoDriver->getExposedVideoData().D3D9.D3DDev9;
	//	if(this->isTransparent)
	//		d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		for(i = 0; i < this->my_mesh->getMeshBufferCount(); ++i)
		{
			video::IMaterialRenderer* rnd = this->my_videoDriver->getMaterialRenderer(my_materials[i].MaterialType);
			bool transparent = (rnd && rnd->isTransparent());

			// only render transparent buffer if this is the transparent render pass
			// and solid only in solid pass
			if (transparent == isTransparentPass)
			{
				scene::IMeshBuffer* mb = this->my_mesh->getMeshBuffer(i);
				this->my_videoDriver->setMaterial(my_materials[i]);
				this->my_videoDriver->drawMeshBuffer(mb);
			}
		}

//		if(this->isTransparent)
//			d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}
}

const core::aabbox3d<f32>& TSTerrainSceneNode::getBoundingBox() const
{
	return this->my_mesh->getBoundingBox();
}


// Inherited From video::IShaderConstantSetCallBack -----------------------------//

void TSTerrainSceneNode::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
	video::IVideoDriver* driver	= services->getVideoDriver();

	core::matrix4 projection	= driver->getTransform(video::ETS_PROJECTION);
	core::matrix4 view			= driver->getTransform(video::ETS_VIEW);
	core::matrix4 world			= driver->getTransform(video::ETS_WORLD);

	core::matrix4 worldViewProj	=	projection;
	worldViewProj				*=	view;
	worldViewProj				*=	world;
	services->setVertexShaderConstant("WorldViewProj", worldViewProj.pointer(), 16);
}


// Original methods -------------------------------------------------------------//

// Setter And Getter

scene::SMesh *TSTerrainSceneNode::getMesh()
{
	return this->my_mesh;
}

void TSTerrainSceneNode::setToTransparent(bool doChange)
{
	if(doChange)
	{
		this->setMaterialType((video::E_MATERIAL_TYPE)my_transparentTSMaterial);
		this->isTransparent = true;
	}
	else {
		this->setMaterialType((video::E_MATERIAL_TYPE)my_solidTSMaterial);
		this->isTransparent = false;
	}
}


// Helper Methods ---------------------------------------------------------------//

bool TSTerrainSceneNode::loadHeightMap(
	const c8* name,
	video::IImage* heightmap,
	const core::dimension2d<f32>& stretchSize,
	f32 maxHeight, const core::dimension2d<s32> maxVtxBlockSize,
	bool debugBorders)
{
	bool result = false;

	if(name != NULL && strlen(name) > 0 &&
	   heightmap != NULL && this->my_timer != NULL && this->my_videoDriver != NULL)
	{
		u32 startTime = my_timer->getRealTime();

		video::SMaterial material;

		c8 stringBuffer[256];
		c8 textureName[64];

		s32 borderSkip = debugBorders ? 0 : 1;

		video::S3DVertex2TCoords vtx;
		vtx.Color.set(255,255,255,255);

		scene::SMesh* mesh	= new scene::SMesh();

		u32 tm							= this->my_timer->getRealTime() / 1000;
		core::dimension2d<u32> hMapSize	= heightmap->getDimension();
		core::dimension2d<u32> tMapSize = heightmap->getDimension();
		core::position2d<f32> thRel((f32)tMapSize.Width / hMapSize.Width, (f32)tMapSize.Height / hMapSize.Height);
		core::position2d<s32> processed(0,0);


#ifdef _USING_SMOOTH_FACTOR

		s32 widthInBlock			= 0;
		s32 heightInBlock			= 0;
		bool noMoreWidthCounting	= false;

		scene::SMeshBufferLightMap *wholeTerrain	= new scene::SMeshBufferLightMap();
		wholeTerrain->Vertices.set_used(hMapSize.Height * hMapSize.Width);
		u32 wholeTerrainIndex						= 0;

#endif

		while(processed.Y < hMapSize.Height)
		{
#ifdef _USING_SMOOTH_FACTOR
			heightInBlock++;
#endif

			while(processed.X < hMapSize.Width)
			{
#ifdef _USING_SMOOTH_FACTOR
				if(!noMoreWidthCounting)
					widthInBlock++;
#endif

				// 1. Calculate the block size
				core::dimension2d<s32> blockSize = maxVtxBlockSize;

				if (processed.X + blockSize.Width	> hMapSize.Width)
					blockSize.Width		= hMapSize.Width - processed.X;
				if (processed.Y + blockSize.Height	> hMapSize.Height)
					blockSize.Height	= hMapSize.Height - processed.Y;

				// DC NOTE: For Applying Texture Splatting.
				//			typedef CMeshBuffer<video::S3DVertex2TCoords> SMeshBufferLightMap
				scene::SMeshBufferLightMap* buffer = new scene::SMeshBufferLightMap();

				// 2. Add vertices of vertex block
				s32 y;

				const f32 resDTBySize	= this->my_detailTextScale	/	(f32)(hMapSize.Width - 1);
				const f32 resLMBySize	= this->my_lightMapTextScale/	(f32)(hMapSize.Width - 1);
				const f32 resCMBySize	= this->my_colorMapTextScale/	(f32)(hMapSize.Width - 1);
				const f32 res1BySize	= this->my_text1Scale		/	(f32)(hMapSize.Width - 1);
				const f32 res2BySize	= this->my_text2Scale		/	(f32)(hMapSize.Width - 1);
				const f32 res3BySize	= this->my_text3Scale		/	(f32)(hMapSize.Width - 1);
				const f32 res4BySize	= this->my_text4Scale		/	(f32)(hMapSize.Width - 1);

				f32 xDTval	= 0, zDTval	= 0;
				f32 xLMval	= 0, zLMval = 0;
				f32 xCMval	= 0, zCMval	= 0;
				f32 x1val	= 0, z1val	= 0;
				f32 x2val	= 0, z2val	= 0;
				f32 x3val	= 0, z3val	= 0;
				f32 x4val	= 0, z4val	= 0;


				for (y=0; y < blockSize.Height; ++y)
				{
					if(zDTval == 0)
					{
						zDTval	=	resDTBySize	* (y + processed.Y);
						zLMval	=	resLMBySize	* (y + processed.Y);
						zCMval	=	resCMBySize	* (y + processed.Y);
						z1val	=	res1BySize	* (y + processed.Y);
						z2val	=	res2BySize	* (y + processed.Y);
						z3val	=	res3BySize	* (y + processed.Y);
						z4val	=	res4BySize	* (y + processed.Y);
					}

					for (s32 x=0; x < blockSize.Width; ++x)
					{
						if(xDTval == 0)
						{
							xDTval	+=	resDTBySize	* (x + processed.X);
							xLMval	+=	resLMBySize	* (x + processed.X);
							xCMval	+=	resCMBySize	* (x + processed.X);
							x1val	+=	res1BySize	* (x + processed.X);
							x2val	+=	res2BySize	* (x + processed.X);
							x3val	+=	res3BySize	* (x + processed.X);
							x4val	+=	res4BySize	* (x + processed.X);
						}

						video::SColor clr = heightmap->getPixel(x + processed.X, y + processed.Y);
						f32 height = ((clr.getRed() + clr.getGreen() + clr.getBlue()) / 3.0f) / 255.0f * maxHeight;

						vtx.Pos.set((f32)(x + processed.X) * stretchSize.Width,
							height, (f32)(y + processed.Y) * stretchSize.Height);

						vtx.TCoords.set((x + 0.5f) / blockSize.Width,
							(y + 0.5f) / blockSize.Height);

						vtx.TCoords.X	= xDTval;
//						vtx.TCoords6.X	= xLMval;
//						vtx.TCoords7.X	= xCMval;
						vtx.TCoords.Y	= zDTval;
//						vtx.TCoords6.Y	= zLMval;
//						vtx.TCoords7.Y	= zCMval;

						if(this->my_text1Scale == 0)
							vtx.TCoords2	= vtx.TCoords;
						else
						{
							vtx.TCoords2.X	= x1val;
							vtx.TCoords2.Y	= z1val;
						}

//						if(this->my_text2Scale == 0)
//							vtx.TCoords3	= vtx.TCoords;
//						else
//						{
//							vtx.TCoords3.X	= x2val;
//							vtx.TCoords3.Y	= z2val;
//						}
//
//						if(this->my_text3Scale == 0)
//							vtx.TCoords4	= vtx.TCoords;
//						else
//						{
//							vtx.TCoords4.X	= x3val;
//							vtx.TCoords4.Y	= z3val;
//						}
//
//						if(this->my_text4Scale == 0)
//							vtx.TCoords5	= vtx.TCoords;
//						else
//						{
//							vtx.TCoords5.X	= x4val;
//							vtx.TCoords5.Y	= z4val;
//						}

						buffer->Vertices.push_back(vtx);

#ifdef _USING_SMOOTH_FACTOR
						wholeTerrainIndex = (x + processed.X) * hMapSize.Width + (y + processed.Y);

						wholeTerrain->Vertices[wholeTerrainIndex] = vtx;
#endif


						xDTval	+=	resDTBySize;
						xLMval	+=	resLMBySize;
						xCMval	+=	resCMBySize;
						x1val	+=	res1BySize;
						x2val	+=	res2BySize;
						x3val	+=	res3BySize;
						x4val	+=	res4BySize;
					}

					xDTval = xLMval = xCMval = x1val = x2val = x3val = x4val = 0;

					zDTval	+=	resDTBySize;
					zLMval	+=	resLMBySize;
					zCMval	+=	resCMBySize;
					z1val	+=	res1BySize;
					z2val	+=	res2BySize;
					z3val	+=	res3BySize;
					z4val	+=	res4BySize;
				}

				// 3. Add indices of vertex block
				for(y = 0; y < blockSize.Height - 1; ++y)
				{
					for (s32 x = 0; x < blockSize.Width - 1; ++x)
					{
						s32 c = (y * blockSize.Width) + x;

						buffer->Indices.push_back(c);
						buffer->Indices.push_back(c + blockSize.Width);
						buffer->Indices.push_back(c + 1);

						buffer->Indices.push_back(c + 1);
						buffer->Indices.push_back(c + blockSize.Width);
						buffer->Indices.push_back(c + 1 + blockSize.Width);
					}
				}

				// 4. Recalculate normals
				for(s32 i = 0; i < (s32)buffer->Indices.size(); i += 3)
				{
					core::plane3d<f32> p(
						buffer->Vertices[buffer->Indices[i + 0]].Pos,
						buffer->Vertices[buffer->Indices[i + 1]].Pos,
						buffer->Vertices[buffer->Indices[i + 2]].Pos);
					p.Normal.normalize();

					buffer->Vertices[buffer->Indices[i + 0]].Normal = p.Normal;
					buffer->Vertices[buffer->Indices[i + 1]].Normal = p.Normal;
					buffer->Vertices[buffer->Indices[i + 2]].Normal = p.Normal;
				}

				if (buffer->Vertices.size())
				{
					// create texture for this block
					video::IImage* img = this->my_videoDriver->createImage(heightmap,
						core::position2d<s32>((s32)(processed.X * thRel.X), (s32)(processed.Y * thRel.Y)),
						core::dimension2d<u32>((s32)(blockSize.Width * thRel.X), (s32)(blockSize.Height * thRel.Y)));

				//	sprintf_s(textureName, 64, "terrain%u_%d", tm, mesh->getMeshBufferCount());

				///	material.Textures[0] = this->my_videoDriver->addTexture(textureName, img);

//					if (material.Textures[0])
//					{
//						sprintf_s(stringBuffer, 256, "Generated terrain texture (%dx%d): %s\n",
//							material.Textures[0]->getSize().Width,
//							material.Textures[0]->getSize().Height,
//							textureName);
//						this->my_logger->log(stringBuffer);
//					}
//					else
//						printf("Could not create terrain texture.", textureName);
//
					buffer->Material = material;
					img->drop();
				}

				buffer->recalculateBoundingBox();
				mesh->addMeshBuffer(buffer);
				buffer->drop();

				processed.X += maxVtxBlockSize.Width - borderSkip;

			} // while(processed.X<hMapSize.Width)

#ifdef _USING_SMOOTH_FACTOR
			noMoreWidthCounting = true;
#endif

			// keep on processing
			processed.X = 0;
			processed.Y += maxVtxBlockSize.Height - borderSkip;
		} // while (processed.Y<hMapSize.Height)

		scene::SAnimatedMesh* animatedMesh = new scene::SAnimatedMesh();
		mesh->recalculateBoundingBox();
		this->my_mesh = mesh;


#ifdef _USING_SMOOTH_FACTOR

		for( s32 i=0; i < 5 ;i++)
		{
			for( s32 x=0; x < hMapSize.Width ; x++)
			{
				for( s32 z=0; z < hMapSize.Height ; z++)
				{
				   int counter=1;

				   if( x>0 && z > 0){
					  wholeTerrain->Vertices[x + z * hMapSize.Height].Pos.Y += wholeTerrain->Vertices[(x-1) + (z-1) * hMapSize.Height].Pos.Y;
					  counter++;
				   }
				   if( x>0 ){
					  wholeTerrain->Vertices[x + z * hMapSize.Height].Pos.Y += wholeTerrain->Vertices[(x-1) + (z) * hMapSize.Height].Pos.Y;
					  counter++;
				   }
				   if( x>0 && z < hMapSize.Height-1){
					  wholeTerrain->Vertices[x + z * hMapSize.Height].Pos.Y += wholeTerrain->Vertices[(x-1) + (z+1) * hMapSize.Height].Pos.Y;
					  counter++;
				   }
				   if( x < hMapSize.Width-1 && z > 0){
					  wholeTerrain->Vertices[x + z * hMapSize.Height].Pos.Y += wholeTerrain->Vertices[(x+1) + (z-1) * hMapSize.Height].Pos.Y;
					  counter++;
				   }
				   if( x < hMapSize.Width-1 ){
					  wholeTerrain->Vertices[x + z * hMapSize.Height].Pos.Y += wholeTerrain->Vertices[(x+1) + (z) * hMapSize.Height].Pos.Y;
					  counter++;
				   }
				   if( x < hMapSize.Width-1 && z < hMapSize.Height-1){
					  wholeTerrain->Vertices[x + z * hMapSize.Height].Pos.Y += wholeTerrain->Vertices[(x+1) + (z+1) * hMapSize.Height].Pos.Y;
					  counter++;
				   }
				   if( z > 0 ){
					  wholeTerrain->Vertices[x + z * hMapSize.Height].Pos.Y += wholeTerrain->Vertices[(x) + (z-1) * hMapSize.Height].Pos.Y;
					  counter++;
				   }
				   if( z < hMapSize.Height-1 ){
					  wholeTerrain->Vertices[x + z * hMapSize.Height].Pos.Y += wholeTerrain->Vertices[(x) + (z+1) * hMapSize.Height].Pos.Y;
					  counter++;
				   }
				   wholeTerrain->Vertices[x + z * hMapSize.Height].Pos.Y /= (float)counter;
				}
			}
		}

		this->calculateNormals(wholeTerrain, core::dimension2d<s32>(hMapSize.Width, hMapSize.Height));

		// Convert to the Blocks
		s32 curBlockIndex			= 0;
		s32 vertexIndex				= 0;

		processed.X = processed.Y	= 0;

		while(processed.Y < hMapSize.Height)
		{
			while(processed.X < hMapSize.Width)
			{

				// 1. Calculate the block size
				core::dimension2d<s32> blockSize = maxVtxBlockSize;

				if (processed.X + blockSize.Width	> hMapSize.Width)
					blockSize.Width		= hMapSize.Width - processed.X;
				if (processed.Y + blockSize.Height	> hMapSize.Height)
					blockSize.Height	= hMapSize.Height - processed.Y;

				vertexIndex = 0;

				s32 y;

				for (y=0; y < blockSize.Height; ++y)
				{
					for (s32 x=0; x < blockSize.Width; ++x)
					{
						wholeTerrainIndex = (x + processed.X) * hMapSize.Width + (y + processed.Y);

						((scene::SMeshBufferLightMap *)my_mesh->getMeshBuffer(curBlockIndex))->Vertices[vertexIndex].Pos
							= wholeTerrain->Vertices[wholeTerrainIndex].Pos;

						vertexIndex++;
					}
				}

/*
				// 4. Recalculate normals
				for(s32 i = 0; i < (s32)buffer->Indices.size(); i += 3)
				{
					core::plane3d<f32> p(
						buffer->Vertices[buffer->Indices[i + 0]].Pos,
						buffer->Vertices[buffer->Indices[i + 1]].Pos,
						buffer->Vertices[buffer->Indices[i + 2]].Pos);
					p.Normal.normalize();

					buffer->Vertices[buffer->Indices[i + 0]].Normal = p.Normal;
					buffer->Vertices[buffer->Indices[i + 1]].Normal = p.Normal;
					buffer->Vertices[buffer->Indices[i + 2]].Normal = p.Normal;
				}
*/
				curBlockIndex++;

				processed.X += maxVtxBlockSize.Width - borderSkip;

			} // while(processed.X<hMapSize.Width)

			// keep on processing
			processed.X = 0;
			processed.Y += maxVtxBlockSize.Height - borderSkip;
		} // while (processed.Y<hMapSize.Height)

		wholeTerrain->drop();
		wholeTerrain = NULL;

#endif


		video::SMaterial mat;
		for (u32 i = 0; i < this->my_mesh->getMeshBufferCount(); ++i)
		{
			scene::IMeshBuffer* mb = this->my_mesh->getMeshBuffer(i);
			if (mb)
				mat = mb->getMaterial();

			this->my_materials.push_back(mat);
		}

		u32 endTime = this->my_timer->getRealTime();

//		sprintf_s(stringBuffer, 256, "Generated terrain data (%dx%d) in %.4f seconds\n",
//			hMapSize.Width, hMapSize.Height, (endTime - startTime) / 1000.0f);

		this->my_logger->log(stringBuffer);

		if(this->my_mesh != NULL)
		{
			this->my_mesh->grab();
			result = true;
		}
	}

	return result;
}

// calculate smooth normals
void TSTerrainSceneNode::calculateNormals(scene::SMeshBufferLightMap* pMeshBuffer, core::dimension2d<s32> terrainData)
{
	s32 count;
	core::vector3df a, b, c, t;

	for (s32 x=0; x<terrainData.Width; ++x)
		for (s32 z=0; z<terrainData.Height; ++z)
		{
			count = 0;
			core::vector3df normal;

			// top left
			if (x>0 && z>0)
			{
				a = pMeshBuffer->Vertices[(x-1)*terrainData.Width+z-1].Pos;
				b = pMeshBuffer->Vertices[(x-1)*terrainData.Width+z].Pos;
				c = pMeshBuffer->Vertices[x*terrainData.Width+z].Pos;
				b -= a;
				c -= a;
				t = b.crossProduct ( c );
				t.normalize ( );
				normal += t;

				a = pMeshBuffer->Vertices[(x-1)*terrainData.Width+z-1].Pos;
				b = pMeshBuffer->Vertices[x*terrainData.Width+z-1].Pos;
				c = pMeshBuffer->Vertices[x*terrainData.Width+z].Pos;
				b -= a;
				c -= a;
				t = b.crossProduct ( c );
				t.normalize ( );
				normal += t;

				count += 2;
			}

			// top right
			if (x>0 && z<terrainData.Height-1)
			{
				a = pMeshBuffer->Vertices[(x-1)*terrainData.Width+z].Pos;
				b = pMeshBuffer->Vertices[(x-1)*terrainData.Width+z+1].Pos;
				c = pMeshBuffer->Vertices[x*terrainData.Width+z+1].Pos;
				b -= a;
				c -= a;
				t = b.crossProduct ( c );
				t.normalize ( );
				normal += t;

				a = pMeshBuffer->Vertices[(x-1)*terrainData.Width+z].Pos;
				b = pMeshBuffer->Vertices[x*terrainData.Width+z+1].Pos;
				c = pMeshBuffer->Vertices[x*terrainData.Width+z].Pos;
				b -= a;
				c -= a;
				t = b.crossProduct ( c );
				t.normalize ( );
				normal += t;

				count += 2;
			}

			// bottom right
			if (x<terrainData.Width-1 && z<terrainData.Width-1)
			{
				a = pMeshBuffer->Vertices[x*terrainData.Width+z+1].Pos;
				b = pMeshBuffer->Vertices[x*terrainData.Width+z].Pos;
				c = pMeshBuffer->Vertices[(x+1)*terrainData.Width+z+1].Pos;
				b -= a;
				c -= a;
				t = b.crossProduct ( c );
				t.normalize ( );
				normal += t;

				a = pMeshBuffer->Vertices[x*terrainData.Width+z+1].Pos;
				b = pMeshBuffer->Vertices[(x+1)*terrainData.Width+z+1].Pos;
				c = pMeshBuffer->Vertices[(x+1)*terrainData.Width+z].Pos;
				b -= a;
				c -= a;
				t = b.crossProduct ( c );
				t.normalize ( );
				normal += t;

				count += 2;
			}

			// bottom left
			if (x<terrainData.Width-1 && z>0)
			{
				a = pMeshBuffer->Vertices[x*terrainData.Width+z-1].Pos;
				b = pMeshBuffer->Vertices[x*terrainData.Width+z].Pos;
				c = pMeshBuffer->Vertices[(x+1)*terrainData.Width+z].Pos;
				b -= a;
				c -= a;
				t = b.crossProduct ( c );
				t.normalize ( );
				normal += t;

				a = pMeshBuffer->Vertices[x*terrainData.Width+z-1].Pos;
				b = pMeshBuffer->Vertices[(x+1)*terrainData.Width+z].Pos;
				c = pMeshBuffer->Vertices[(x+1)*terrainData.Width+z-1].Pos;
				b -= a;
				c -= a;
				t = b.crossProduct ( c );
				t.normalize ( );
				normal += t;

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

			pMeshBuffer->Vertices[x * terrainData.Width + z].Normal = normal;
		}
}
