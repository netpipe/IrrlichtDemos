#include "EditableTerrain.h"

void EditableTerrain::RaiseTerrainVertex(s32 index, f32 step)
{
	IMesh* pMesh = terrainNode->getMesh(); 

	u32 b; 
	for (b=0; b<pMesh->getMeshBufferCount(); ++b) 
	{ 
	  IMeshBuffer* pMeshBuffer = pMesh->getMeshBuffer(b); 
	  // skip mesh buffers that are not the right type 
	  if (pMeshBuffer->getVertexType() != video::EVT_2TCOORDS) continue; 

	  video::S3DVertex2TCoords* pVertices = (video::S3DVertex2TCoords*)pMeshBuffer->getVertices(); 

	  pVertices[index].Pos.Y += step; 
	} 

	// force terrain render buffer to reload 
	terrainNode->setPosition(terrainNode->getPosition()); 
}

EditableTerrain::EditableTerrain(ISceneManager* smgr, path HeightMapFile)
{
	terrainNode = smgr->addTerrainSceneNode("heightmap.bmp");
}

void EditableTerrain::Save (IVideoDriver* driver)
{
	const core::dimension2d<u32> &dim = core::dimension2du(256,256); 
	video::IImage *img = driver->createImage(ECF_R8G8B8, dim); 

	u32 VertexCount = terrainNode->getMesh()->getMeshBuffer(0)->getVertexCount(); 
	S3DVertex2TCoords* verts = (S3DVertex2TCoords*)terrainNode->getMesh()->getMeshBuffer(0)->getVertices(); 

	for (u32 i=0; i<VertexCount; i++) 
	{ 
		S3DVertex2TCoords* vertex = verts + i; 
		u8 y = (u8)vertex->Pos.Y; 
		img->setPixel((u32)vertex->Pos.X, (u32)vertex->Pos.Z, video::SColor(0, y,y,y)); 
	} 

	driver->writeImageToFile(img, "heightmap.bmp", 0); 
	img->drop(); 
}

void EditableTerrain::DrawSelectionGrid(f32 xStart, f32 zStart, int size)
{
	IVideoDriver* driver = terrainNode->getSceneManager()->getVideoDriver();
	if (size < 1) return;
	core::array<S3DVertex> v;
	core::array<u16> idx;

	for (int x=0;x<size;++x)
	{
		for (int z=0;z<size;++z)
		{
			//USe our +0.5 trick for the rounding issue 
			S3DVertex vert;
			f32 xC = (f32)round(xStart-0.5f*size+x);
			f32 zC = (f32)round(zStart-0.5f*size+z);
			vert.Pos = vector3df(xC, terrainNode->getHeight(xC, zC)+0.1f, zC);
			vert.Color = SColor(255, 0, 255, 255);
			v.push_back(vert);
			idx.push_back(x*size+z);
		}
	}

	SMaterial mat;
	mat.BackfaceCulling = false;
	mat.Lighting = false;
	mat.Thickness = 40.0f;
	mat.DiffuseColor = SColor(255, 0, 255, 255);
	driver->setMaterial(mat);
	core::matrix4 matr = terrainNode->getAbsoluteTransformation();
	driver->setTransform(video::ETS_WORLD, matr); 
	driver->drawVertexPrimitiveList(&v[0], v.size(), &idx[0], idx.size(), EVT_STANDARD, EPT_POINTS);
}

void EditableTerrain::RaiseVertexGrid(s32 xMid, s32 zMid, s32 terrainDimension, s32 size, s32 step)
{
	IMesh* pMesh = terrainNode->getMesh(); 

	for (u32 b=0; b<pMesh->getMeshBufferCount(); ++b) 
	{ 
	  IMeshBuffer* pMeshBuffer = pMesh->getMeshBuffer(b); 
	  // skip mesh buffers that are not the right type 
	  if (pMeshBuffer->getVertexType() != video::EVT_2TCOORDS) continue; 

	  video::S3DVertex2TCoords* pVertices = (video::S3DVertex2TCoords*)pMeshBuffer->getVertices(); 

	  for (u8 x=0; x<size; x++)
		for (u8 z=0; z<size; z++)
		{
			s32 xC = (s32)round(xMid-0.5f*size+x);
			s32 zC = (s32)round(zMid-0.5f*size+z);
			pVertices[xC * terrainDimension + zC].Pos.Y += step; 
		}
	} 

	// force terrain render buffer to reload 
	terrainNode->setPosition(terrainNode->getPosition()); 
}

void EditableTerrain::SmoothGrid(s32 xMid, s32 zMid, s32 terrainDimension, s32 size)
{
	IMesh* pMesh = terrainNode->getMesh(); 

	for (u32 b=0; b<pMesh->getMeshBufferCount(); ++b) 
	{ 
		IMeshBuffer* pMeshBuffer = pMesh->getMeshBuffer(b); 
		// skip mesh buffers that are not the right type 
		if (pMeshBuffer->getVertexType() != video::EVT_2TCOORDS) continue; 

		video::S3DVertex2TCoords* pVertices = (video::S3DVertex2TCoords*)pMeshBuffer->getVertices(); 

		for (u8 x=0; x<size; x++)
			for (u8 z=0; z<size; z++)
			{
				s32 xC = (s32)round(xMid-0.5f*size+x);
				s32 zC = (s32)round(zMid-0.5f*size+z);

				int adjacentSections = 0;
				float sectionsTotal = 0.0f;

				if ((xC - 1) > 0) // Check to left
				{
					sectionsTotal += pVertices[(xC -1) * terrainDimension + zC].Pos.Y;
					adjacentSections++;

					if ((zC - 1) > 0) // Check up and to the left
					{
						sectionsTotal += pVertices[(xC - 1) * terrainDimension + (zC - 1)].Pos.Y;
						adjacentSections++;
					}

					if ((zC + 1) < terrainDimension) // Check down and to the left
					{
						sectionsTotal += pVertices[(xC -1) * terrainDimension + (zC + 1)].Pos.Y;
						adjacentSections++;
					}
				}

				if ((xC + 1) < terrainDimension) // Check to right
				{
					sectionsTotal += pVertices[(xC + 1) * terrainDimension + zC].Pos.Y;
					adjacentSections++;

					if ((zC - 1) > 0) // Check up and to the right
					{
						sectionsTotal += pVertices[(xC + 1) * terrainDimension + (zC - 1)].Pos.Y;
						adjacentSections++;
					}

					if ((zC + 1) < terrainDimension) // Check down and to the right
					{
						sectionsTotal += pVertices[(xC + 1) * terrainDimension + (zC + 1)].Pos.Y;
						adjacentSections++;
					}
				}

				if ((zC - 1) > 0) // Check above
				{
					sectionsTotal += pVertices[xC * terrainDimension + (zC - 1)].Pos.Y;
					adjacentSections++;
				}

				if ((zC + 1) < terrainDimension) // Check below
				{
					sectionsTotal += pVertices[xC * terrainDimension + (zC + 1)].Pos.Y;
					adjacentSections++;
				}

				pVertices[xC * terrainDimension + zC].Pos.Y = (pVertices[xC * terrainDimension + zC].Pos.Y + (sectionsTotal / adjacentSections)) * 0.5f;
				//Changing weight
				//pVertices[xC * terrainDimension + zC].Pos.Y = (pVertices[xC * terrainDimension + zC].Pos.Y * 0.4f + (sectionsTotal / adjacentSections * 0.5f)); //* 0.5f;
			}
		} 

	// force terrain render buffer to reload 
	terrainNode->setPosition(terrainNode->getPosition()); 
}

ITerrainSceneNode* EditableTerrain::GetTerrainNode()
{
	return terrainNode;
}