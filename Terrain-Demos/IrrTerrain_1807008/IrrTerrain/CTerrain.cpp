/*
original code by Copland (original source) : http://forum.irrlicht.fr/viewtopic.php?id=336
modified version of _CTerrain by fredakilla@gmail.com

Modified version :
- change map loader for support 2^N+1 heightmap size
- add internally VBO support
- add VBO destuction (for paging)
- add colormap destruction (for paging)
- change uv coordinate calculation
- improve render method and add render debug data
- change bounding box calculation and remove it by default (not necessary for paging)
- add clamping on color textures (don't work with opengl - irrlicht bug ?)
- add possibility to change material type or flag externally
- remove normals recalculation (improve loading time for pager)
- remove splatting shader data and functions => changed by setShader to accept generic shader
- generals and minors code organisations
*/

#include "CTerrain.h"
#include <irrlicht.h>

using namespace irr;

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CTerrain::CTerrain(c8* HeightmapFile, E_TERRAIN_QUALITY Quality, f32 ScaleTexture, scene::ISceneNode* parent, scene::ISceneManager* smgr, s32 id) : scene::ISceneNode(parent, smgr, id)
{
	// get heightmap
    video::IImage *Heightmap = SceneManager->getVideoDriver()->createImageFromFile(HeightmapFile);

    // get heightmap dimensions
    u16 Size = Heightmap->getDimension().Width;

    // set tiles size for Terrain
    s32 SizeOfTiles = 0;

    // switch size to calculate tiles size
    switch(Size)
    {
        case 65:
            SizeOfTiles=(Size/4)+1;
            NbsTiles = 4*4;
            break;
        case 129:
            SizeOfTiles=(Size/8)+1;
            NbsTiles = 8*8;
            break;
        case 257:
			SizeOfTiles=(Size/16)+1;
            NbsTiles = 16*16;
            break;
        case 513:
            SizeOfTiles=(Size/16)+1;
            NbsTiles = 16*16;
            break;
        case 769:
            SizeOfTiles=(Size/24)+1;
            NbsTiles = 24*24;
            break;
        case 1025:
            SizeOfTiles=(Size/32)+1;
            NbsTiles = 32*32;
            break;
        case 2049:
            SizeOfTiles=(Size/32)+1;
            NbsTiles = 32*32;
            break;
        default:
            SizeOfTiles=(Size/16)+1;
            NbsTiles = 16*16;
            break;
    }

    // create terrain mesh
    TerrainMesh = new scene::SMesh();

    // compute quality factor
    u32 SOTiles = irr::core::ceil32((f32)SizeOfTiles/(f32)Quality);

    // init MeshBuffer array
    CTTileBuffer=new scene::SMeshBufferLightMap* [NbsTiles];

    // create MeshBuffer
    u32 TileX=0, TileZ=0;
    for (u32 i =0; i < NbsTiles; ++i)
    {
		CTTileBuffer[i]=new scene::SMeshBufferLightMap();
        CTTileBuffer[i]->Vertices.set_used(SizeOfTiles*SizeOfTiles);
        CTTileBuffer[i]->Indices.set_used(SizeOfTiles*SizeOfTiles*6);

        const f32 tdSize = 1.0f/(f32)((Size-1.0f));
        u32 Index=0;
        u16 NbsIndices=0,NbsVertices=0;

         for(u32 x=TileX; x<(TileX+SizeOfTiles); x+=Quality)
        {
            for (u32 z=TileZ; z<(TileZ+SizeOfTiles); z+=Quality)
            {
                if (NbsVertices < (SOTiles*SOTiles)-SOTiles-1)
                {
					// set indices
                    Index = NbsVertices;
                    u32 TestValue = ( (((x-TileX)/Quality)+1) * ((z-TileZ)/Quality) + ((x-TileX)/Quality) );
                    if (Index != TestValue || (x-TileX==0 && z < TileZ+SizeOfTiles-Quality))
                    {
                        CTTileBuffer[i]->Indices[NbsIndices++]=Index;
                        CTTileBuffer[i]->Indices[NbsIndices++]=Index+1;
                        CTTileBuffer[i]->Indices[NbsIndices++]=Index+SOTiles+1;
                        CTTileBuffer[i]->Indices[NbsIndices++]=Index;
                        CTTileBuffer[i]->Indices[NbsIndices++]=Index+SOTiles+1;
                        CTTileBuffer[i]->Indices[NbsIndices++]=Index+SOTiles;
                    }
                }

				// read heightmap and set vertices
				video::S3DVertex2TCoords Vertex;
                Vertex.Normal = core::vector3df(0,1,0);
                Vertex.Pos.X = (f32)x;
                video::SColor pixelColor(Heightmap->getPixel(x,Size-1-z));
                Vertex.Pos.Y = (f32) pixelColor.getLuminance()/10.0f;
                Vertex.Pos.Z = (f32)z;
                Vertex.TCoords = core::vector2d<f32>( (f32)(x*tdSize), (f32)((Size-1-z)*tdSize));
                Vertex.TCoords2 = core::vector2d<f32>( (f32)(x*tdSize), (f32)((Size-1-z)*tdSize))*ScaleTexture;
				Vertex.Color = video::SColor(255,255,255,255);

				CTTileBuffer[i]->Vertices[NbsVertices]=Vertex;

                NbsVertices++;
            }
        }

		// set material flags
        CTTileBuffer[i]->Material.Lighting = false;
        CTTileBuffer[i]->Material.Wireframe = false;
        CTTileBuffer[i]->Material.BackfaceCulling = true;
        CTTileBuffer[i]->Material.GouraudShading = true;
        CTTileBuffer[i]->Material.FogEnable = false;

		CTTileBuffer[i]->Vertices.set_used(NbsVertices);
        CTTileBuffer[i]->Indices.set_used(NbsIndices);

		// smoothing
		for(s32 j = 0; j < ((Quality+1)/2); j++)
        {
            for(u32 index = 2; index < (SOTiles * SOTiles - 2); index++)
            {
                CTTileBuffer[i]->Vertices[index].Pos.Y += (1/8)*
                (CTTileBuffer[i]->Vertices[index-2].Pos.Y +
                2*CTTileBuffer[i]->Vertices[index-1].Pos.Y +
                2*CTTileBuffer[i]->Vertices[index].Pos.Y +
                2*CTTileBuffer[i]->Vertices[index+1].Pos.Y +
                CTTileBuffer[i]->Vertices[index+2].Pos.Y);
            }
        }

		// smoothing
        for(s32 k = 0; k < ((Quality+1)/2); k++)
        {
            for(u32 index = SOTiles; index < (SOTiles * (SOTiles - 1)); index++)
            {
                CTTileBuffer[i]->Vertices[index].Pos.Y = (CTTileBuffer[i]->Vertices[index - SOTiles].Pos.Y + CTTileBuffer[i]->Vertices[index + SOTiles].Pos.Y ) / 2.0f;
			}
        }

        // recalculate bounding box
        CTTileBuffer[i]->recalculateBoundingBox();

		// active VBO for performance
		CTTileBuffer[i]->setHardwareMappingHint(scene::EHM_STATIC);

        // add meshBuffer to terrain mesh
        TerrainMesh->addMeshBuffer(CTTileBuffer[i]);

		// set TileX and TileY
		TileX+=SizeOfTiles-1;
		if(TileX >= NbsTiles)
        {
            TileX=0;

            TileZ+=SizeOfTiles-1;
            if(TileZ >= NbsTiles)
            {
                TileZ=0;
            }
        }
    }

	// disable automatic culling
	AutomaticCullingState = scene::EAC_OFF;

	// free heightmap
    Heightmap->drop();

	// set boundingbox
	//recalculateBoundingBox();
}


//----------------------------------------------------------------------------------------
// destructor (important for paging)
//----------------------------------------------------------------------------------------
CTerrain::~CTerrain()
{
	// free color texture
	SceneManager->getVideoDriver()->removeTexture(CTTileBuffer[0]->Material.getTexture(0));

	// remove MeshBuffers
	for(u32 i=0;i<NbsTiles;++i)
    {
        if (CTerrain::CTTileBuffer[i] != NULL)
        {
			SceneManager->getVideoDriver()->removeHardwareBuffer(CTTileBuffer[i]);
			CTerrain::CTTileBuffer[i]->drop();
        }
    }
	delete [] CTTileBuffer;

	// remove terrain mesh
	SceneManager->getMeshCache()->removeMesh(TerrainMesh);

    if(CTerrain::TerrainMesh != NULL)
    {
        CTerrain::TerrainMesh->drop();
    }

	this->remove();

}

//----------------------------------------------------------------------------------------
// register scene node
//----------------------------------------------------------------------------------------
void CTerrain::OnRegisterSceneNode()
{
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
}


//----------------------------------------------------------------------------------------
// render terrain
//----------------------------------------------------------------------------------------
void CTerrain::render()
{
    scene::ICameraSceneNode* cam = SceneManager->getActiveCamera();
    const scene::SViewFrustum* frustum = cam->getViewFrustum();
    video::IVideoDriver* Driver = SceneManager->getVideoDriver();
    core::vector3df Pos = cam->getPosition();
	Pos.Y = getPosition().Y;
	Driver->setTransform(video::ETS_WORLD,AbsoluteTransformation);

	// debug with wireframe
	if (DebugDataVisible & scene::EDS_MESH_WIRE_OVERLAY)
	{
		video::SMaterial m = CTTileBuffer[0]->getMaterial();
		m.Lighting = false;
		m.Wireframe = true;
        m.BackfaceCulling = false;
		Driver->setMaterial(m);
	}
	else
	{
		// set material
		Driver->setMaterial(video::SMaterial(CTTileBuffer[0]->Material));
	}

	// for each TilesBuffer
    for (u32 i=0; i<NbsTiles; i++)
    {
		// if buffer
        if (CTTileBuffer[i] != NULL)
        {
			// if frustum intersect with TileBuffer then render TileBuffer
            if( frustum->getBoundingBox().intersectsWithBox(CTTileBuffer[i]->getBoundingBox())==true)
            {
                // if TileBuffer is in max distance then render TileBuffer
				if(CTTileBuffer[i]->BoundingBox.getCenter().getDistanceFromSQ(Pos) < RenderDistance)
                {
					// render TileBuffer
					Driver->drawMeshBuffer(CTTileBuffer[i]);
				}
            }
        }
    }

	// debug mode
	if(DebugDataVisible)
    {
        for (u32 i=0;i<NbsTiles;i++)
        {
			if (CTTileBuffer[i] != NULL)
            {
                // Tiles Box Buffer
				if (DebugDataVisible & scene::EDS_BBOX_BUFFERS)
				{
					video::SMaterial m;
					m.Lighting = false;
					Driver->setMaterial(m);
					video::SColor c (255,0,255,0);

					Driver->draw3DBox(CTTileBuffer[i]->getBoundingBox(), c);
				}

				// Entire Terrain Box
				if (DebugDataVisible & scene::EDS_BBOX)
				{
					video::SMaterial m;
					m.Lighting = false;
					Driver->setMaterial(m);
					video::SColor c (255,255,0,0);

					Driver->draw3DBox(this->getBoundingBox(), c);
				}

				// add frustum optimisation
				if( frustum->getBoundingBox().intersectsWithBox(CTTileBuffer[i]->getBoundingBox())==true)
                {
                    f64 ActualDistance = CTTileBuffer[i]->BoundingBox.getCenter().getDistanceFromSQ(Pos);
                    if(ActualDistance < RenderDistance)
                    {
						// normals
						if (DebugDataVisible & scene::EDS_NORMALS)
						{
							video::SMaterial m;
							m.Lighting = false;
							Driver->setMaterial(m);
							video::SColor c (255,0,0,255 );

							for(u16 j = 0 ; j < CTTileBuffer[i]->getVertexCount(); j++)
							{
								video::S3DVertex2TCoords v = CTTileBuffer[i]->Vertices[j];
								core::vector3df h = v.Normal * getScale().Y * 4;
								Driver->draw3DLine ( v.Pos, v.Pos + h, c );
							}
						}
                    }
                }
            }
        }
    }
}


// returns the axis aligned bounding box of terrain
const core::aabbox3d<f32>& CTerrain::getBoundingBox() const
{
    return BoundingBox;
}


//----------------------------------------------------------------------------------------
// recalculate terrain bounding box
//----------------------------------------------------------------------------------------
void CTerrain::recalculateBoundingBox()
{
	// get Min boundingbox of the first TileBuffer
	BoundingBox.MinEdge.X = CTTileBuffer[0]->getBoundingBox().MinEdge.X;
    BoundingBox.MinEdge.Y = CTTileBuffer[0]->getBoundingBox().MinEdge.Y;
    BoundingBox.MinEdge.Z = CTTileBuffer[0]->getBoundingBox().MinEdge.Z;

	// get Max boundingBox of the last TileBuffer
	BoundingBox.MaxEdge.X = CTTileBuffer[NbsTiles-1]->getBoundingBox().MaxEdge.X;
	BoundingBox.MaxEdge.Y = CTTileBuffer[NbsTiles-1]->getBoundingBox().MaxEdge.Y;
	BoundingBox.MaxEdge.Z = CTTileBuffer[NbsTiles-1]->getBoundingBox().MaxEdge.Z;

	// get higher and lower TileBuffer
    for(s32 i=0; i<NbsTiles; i++)
	{
		if(BoundingBox.MinEdge.Y > CTTileBuffer[i]->getBoundingBox().MinEdge.Y)
			BoundingBox.MinEdge.Y = CTTileBuffer[i]->getBoundingBox().MinEdge.Y;

        if(BoundingBox.MaxEdge.Y < CTTileBuffer[i]->getBoundingBox().MaxEdge.Y)
			BoundingBox.MaxEdge.Y = CTTileBuffer[i]->getBoundingBox().MaxEdge.Y;
    }
}


//----------------------------------------------------------------------------------------
// set terrain position for each buffer
//----------------------------------------------------------------------------------------
void CTerrain::setPosition(const core::vector3df &Pos)
{
	for (u32 i=0;i<NbsTiles;i++)
    {
        if (CTTileBuffer[i] != NULL)
        {
            for (u32 j=0;j<CTTileBuffer[i]->getVertexCount();++j)
            {
                CTTileBuffer[i]->Vertices[j].Pos+=Pos;
            }
            CTTileBuffer[i]->recalculateBoundingBox();
        }
    }
	//recalculateBoundingBox();
}


//----------------------------------------------------------------------------------------
// set terrain scale for each buffer
//----------------------------------------------------------------------------------------
void CTerrain::setScale(const core::vector3df &Scale)
{
	for (u32 i=0;i<NbsTiles;i++)
    {
        if (CTTileBuffer[i] != NULL)
        {
            for (u32 j=0;j<CTTileBuffer[i]->getVertexCount();++j)
            {
                CTTileBuffer[i]->Vertices[j].Pos*=Scale;
            }
            CTTileBuffer[i]->recalculateBoundingBox();
        }
    }
	//recalculateBoundingBox();
}


u32 CTerrain::getMaterialCount()
{
    return 1;
}

video::SMaterial& CTerrain::getMaterial(u32 i)
{
    return Material;
}


//----------------------------------------------------------------------------------------
// set terrain colormap for each buffer
//----------------------------------------------------------------------------------------
void CTerrain::setColorTexture(c8* FileName)
{
    for (u32 i=0;i<NbsTiles;++i)
    {
        if (CTTileBuffer[i] != NULL)
        {
			CTTileBuffer[i]->Material.MaterialType = video::EMT_SOLID;
            CTTileBuffer[i]->Material.TextureLayer[0].Texture = SceneManager->getVideoDriver()->getTexture(FileName);
			CTTileBuffer[i]->Material.TextureLayer[0].TextureWrapU = video::ETC_CLAMP;
		}
    }
}


//----------------------------------------------------------------------------------------
// set terrain detailmap for each buffer
//----------------------------------------------------------------------------------------
void CTerrain::setDetailTexture(c8* FileName)
{
	for (u32 i=0;i<NbsTiles;++i)
    {
        if (CTTileBuffer[i] != NULL)
        {

            CTTileBuffer[i]->Material.MaterialType = video::EMT_DETAIL_MAP;
            CTTileBuffer[i]->Material.TextureLayer[1].Texture = SceneManager->getVideoDriver()->getTexture(FileName);
			//CTTileBuffer[i]->Material.TextureLayer[1].TextureWrap = video::ETC_REPEAT;
		}
    }
}

//----------------------------------------------------------------------------------------
// set terrain max render distance
//----------------------------------------------------------------------------------------
void CTerrain::setRenderDistance(f32 Distance)
{
    RenderDistance = Distance * Distance;
}



//----------------------------------------------------------------------------------------
// set terrain material type for each buffer
//----------------------------------------------------------------------------------------
void CTerrain::setMaterialType(video::E_MATERIAL_TYPE Mat)
{
    for (u32 i=0;i < NbsTiles;++i)
    {
        if (CTTileBuffer[i] != NULL)
        {
            CTTileBuffer[i]->Material.MaterialType = Mat;
        }
    }
}

//----------------------------------------------------------------------------------------
// set terrain material flag for each buffer
//----------------------------------------------------------------------------------------
void CTerrain::setMaterialFlag(video::E_MATERIAL_FLAG Mat, bool Flag)
{
	for (u32 i=0;i < NbsTiles;++i)
    {
        if (CTTileBuffer[i] != NULL)
        {
			CTTileBuffer[i]->Material.setFlag(Mat, Flag);
        }
    }
}

//----------------------------------------------------------------------------------------
// get terrain mesh
//----------------------------------------------------------------------------------------
scene::IMesh* CTerrain::getMesh()
{
    return (scene::IMesh*)TerrainMesh;
}


//----------------------------------------------------------------------------------------
// get height
//----------------------------------------------------------------------------------------
f32 CTerrain::getHeight(f32 x, f32 z)
{
    scene::SMeshBufferLightMap* TempBuffer=NULL;

    f32 ValueReturn = 0.0f;

    for (u32 i=0;i < NbsTiles;++i)
    {
        if (CTTileBuffer[i] != NULL)
        {
            core::vector3df Vec(x,CTTileBuffer[i]->getBoundingBox().getCenter().Y,z);
            if(CTTileBuffer[i]->getBoundingBox().isPointInside(Vec))
            {
                TempBuffer=CTTileBuffer[i];
                break;
            }
        }
    }

    if(TempBuffer != NULL)
    {
        //Si le TempBuffer renvoi différent de null, on se situ dans un tile
        scene::SMesh* Mesh=new scene::SMesh();
        Mesh->addMeshBuffer(TempBuffer);
        scene::ITriangleSelector* selector= SceneManager->createTriangleSelector((scene::IMesh*)Mesh,this);
        this->setTriangleSelector(selector);

        core::line3d<f32> line;
        line.start = core::vector3d<f32>(x,-200000,z);
        line.end = core::vector3d<f32>(x,200000,z);

        core::vector3df intersection;
        core::triangle3df tri;
        ISceneNode* outNode;

        if (SceneManager->getSceneCollisionManager()->getCollisionPoint(line, selector, intersection,tri,outNode))
        {
            ValueReturn = intersection.Y;
        }
        selector->drop();
        Mesh->drop();
    }
    else
    {
        ValueReturn=-999999.999f;
    }

    return ValueReturn;
}



//----------------------------------------------------------------------------------------
// set terrain material and texture by texture slot (under construction)
//----------------------------------------------------------------------------------------
void CTerrain::setTexture(u32 NumTex, c8* FileName)
{
    for (u32 i=0;i<NbsTiles;++i)
    {
        if (CTTileBuffer[i] != NULL)
        {
			irr::video::IVideoDriver * driver = this->SceneManager->getVideoDriver();

			switch(NumTex)
			{
			case 0:
				//irr::video::IVideoDriver * driver = this->SceneManager->getVideoDriver();
				//driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
				CTTileBuffer[i]->Material.TextureLayer[0].Texture = SceneManager->getVideoDriver()->getTexture(FileName);
				CTTileBuffer[i]->Material.TextureLayer[0].TextureWrapU = video::ETC_CLAMP;
				//driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
				break;

			case 1:
				CTTileBuffer[i]->Material.TextureLayer[1].BilinearFilter = true;
				//CTTileBuffer[i]->Material.TextureLayer[1].TrilinearFilter = true;
				//driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
				CTTileBuffer[i]->Material.TextureLayer[1].Texture = SceneManager->getVideoDriver()->getTexture(FileName);
				//CTTileBuffer[i]->Material.TextureLayer[1].TextureWrap = video::ETC_REPEAT;
				//driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

				break;

			default:
				//irr::video::IVideoDriver * driver = this->SceneManager->getVideoDriver();
				//driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
				CTTileBuffer[i]->Material.TextureLayer[NumTex].Texture = SceneManager->getVideoDriver()->getTexture(FileName);
				//CTTileBuffer[i]->Material.TextureLayer[1].TextureWrap = video::ETC_CLAMP;
				//driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
				break;
			}
		}
    }
}


//----------------------------------------------------------------------------------------
// set shader (under construction)
//----------------------------------------------------------------------------------------
void CTerrain::setShader(c8 * shaderFileName, scene::ISceneManager* smgr)
{
    video::IVideoDriver* driver = smgr->getVideoDriver();
    s32 newMaterialType1 = 0;
    video::IGPUProgrammingServices* gpu = smgr->getVideoDriver()->getGPUProgrammingServices();

    if (gpu)
    {
        MyShaderCallBack* mc = new MyShaderCallBack();

		newMaterialType1 = gpu->addHighLevelShaderMaterialFromFiles(
			shaderFileName, "VS", video::EVST_VS_2_0,
			shaderFileName, "PS", video::EPST_PS_2_0,
			mc, video::EMT_SOLID);

		mc->drop();
    }

    setMaterialType((video::E_MATERIAL_TYPE)newMaterialType1);
}
