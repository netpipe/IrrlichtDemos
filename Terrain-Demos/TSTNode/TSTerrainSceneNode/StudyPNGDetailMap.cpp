#include <stdio.h>
#include <irrlicht.h>
#include <d3dx9.h>
#include <malloc.h>

using namespace irr;

#pragma comment(lib, "Irrlicht.lib")

#include "RealisticWater.h"
#include "TSTerrainSceneNode.h"


inline void checkCurrentHeap()
{
	int heapStatus;

	heapStatus = _heapchk();

	if(heapStatus == _HEAPOK)
		printf(TEXT("\nTest Heap: Good\n"));
	else if(heapStatus == _HEAPEMPTY)
		printf(TEXT("\nTest Heap: Empty\n"));
	else if(heapStatus == _HEAPBADBEGIN)
		printf(TEXT("\nTest Heap: Bad start of Heap\n"));
	else if(heapStatus == _HEAPBADNODE)
		printf(TEXT("\nTest Heap: Bad Node in Heap\n"));

	getchar();
};

//#define _USE_FOG

//#define _USE_RW_SCENENODE

//#define _TEST_ITERRAIN_SCENENODE
//#define _TEST_IANIMATEDMESH_SCENENODE
#define _TEST_TSHEIGHTTERRAIN_SCENENODE

RealisticWaterSceneNode* WATER;


#ifdef _TEST_ITERRAIN_SCENENODE 
scene::ITerrainSceneNode	*terrain = NULL;

//#define _TEST_TERRAIN
#endif

#ifdef _TEST_IANIMATEDMESH_SCENENODE
scene::IAnimatedMeshSceneNode *terrain = NULL;

#define _TEST_TERRAIN
#endif

#ifdef _TEST_TSHEIGHTTERRAIN_SCENENODE
TSTerrainSceneNode *terrain = NULL;

//#define _TEST_TERRAIN
#endif

class TempEventReceiver: public IEventReceiver
{
public:

	virtual bool OnEvent(SEvent inEvent)
	{
		if(inEvent.EventType == irr::EET_KEY_INPUT_EVENT && !inEvent.KeyInput.PressedDown)
		{
			if(inEvent.KeyInput.Key == irr::KEY_KEY_1)
			{
//#ifdef _TEST_TERRAIN
				if(terrain != NULL)
				{
					static bool test = true;

					terrain->setMaterialFlag(video::EMF_WIREFRAME, test);

					test = !test;

					return true;
				}
//#endif
			}
			if(inEvent.KeyInput.Key == irr::KEY_KEY_W)
			{
				if(WATER != NULL)
					WATER->setPosition(WATER->getPosition() - core::vector3df(0, 10, 0));
			}
			if(inEvent.KeyInput.Key == irr::KEY_KEY_S)
			{
				if(WATER != NULL)
					WATER->setPosition(WATER->getPosition() - core::vector3df(0, -10, 0));
			}
			if(inEvent.KeyInput.Key == irr::KEY_KEY_A)
			{
				if(WATER != NULL)
					WATER->setPosition(WATER->getPosition() - core::vector3df(10, 0, 0));
			}
			if(inEvent.KeyInput.Key == irr::KEY_KEY_D)
			{
				if(WATER != NULL)
					WATER->setPosition(WATER->getPosition() - core::vector3df(-10, 0, 0));
			}
			if(inEvent.KeyInput.Key == irr::KEY_KEY_Q)
			{
				if(WATER != NULL)
					WATER->setPosition(WATER->getPosition() - core::vector3df(0, 0, 10));
			}
			if(inEvent.KeyInput.Key == irr::KEY_KEY_E)
			{
				if(WATER != NULL)
					WATER->setPosition(WATER->getPosition() - core::vector3df(0, 0, -10));
			}
			if(inEvent.KeyInput.Key == irr::KEY_PLUS)
			{
				if(terrain != NULL)
				{
					core::vector3df scale = terrain->getScale();
					scale = scale * 1.5;

					terrain->setScale(scale);
				}
			}
			if(inEvent.KeyInput.Key == irr::KEY_MINUS)
			{
				if(terrain != NULL)
				{
					core::vector3df scale = terrain->getScale();
					scale = scale * 0.5;

					terrain->setScale(scale);
				}
			}
		}

		return false;
	}
};


void main()
{
	IrrlichtDevice			*irrDevice			= createDevice(video::EDT_DIRECT3D9, core::dimension2d<s32>(1024, 768));
	video::IVideoDriver		*videoDriver		= irrDevice->getVideoDriver();
	scene::ISceneManager	*sceneManager		= irrDevice->getSceneManager();

	TempEventReceiver eReceiver;

	irrDevice->setEventReceiver(&eReceiver);

#ifdef _TEST_IANIMATEDMESH_SCENENODE
	scene::IAnimatedMesh *terrainMesh = sceneManager->addTerrainMesh(
		"test03", videoDriver->createImageFromFile("test03.png"), videoDriver->createImageFromFile("test03.png"),
		core::dimension2d<f32>(1.0, 1.0), 173.0f,
		core::dimension2d<s32>(32, 32),
		9.39, 12.85, 7.947, 11.478);

	terrain = sceneManager->addAnimatedMeshSceneNode(terrainMesh);
#endif

	sceneManager->addSkyBoxSceneNode(
		videoDriver->getTexture("./TSTerrainSN_Data/irrlicht2_up.jpg"),
		videoDriver->getTexture("./TSTerrainSN_Data/irrlicht2_dn.jpg"),
		videoDriver->getTexture("./TSTerrainSN_Data/irrlicht2_lf.jpg"),
		videoDriver->getTexture("./TSTerrainSN_Data/irrlicht2_rt.jpg"),
		videoDriver->getTexture("./TSTerrainSN_Data/irrlicht2_ft.jpg"),
		videoDriver->getTexture("./TSTerrainSN_Data/irrlicht2_bk.jpg"));

#ifdef _TEST_TSHEIGHTTERRAIN_SCENENODE

	terrain = new TSTerrainSceneNode(
		irrDevice->getLogger(), sceneManager, irrDevice->getTimer(), "test03",
		"./TSTerrainSN_Data/test03.png",
		core::dimension2d<f32>(1.0, 1.0), /*173.0f*/250.0f,
		core::dimension2d<s32>(32, 32),
		9.39f, 12.85f, 7.947f, 11.478f);

	terrain->setMaterialTexture(0, videoDriver->getTexture("./TSTerrainSN_Data/test03_d.png"));
	terrain->setMaterialTexture(1, videoDriver->getTexture("./TSTerrainSN_Data/grayRock.png"));
	terrain->setMaterialTexture(2, videoDriver->getTexture("./TSTerrainSN_Data/hardDirt.png"));
	terrain->setMaterialTexture(3, videoDriver->getTexture("./TSTerrainSN_Data/bigRockFace.png"));
	terrain->setMaterialTexture(4, videoDriver->getTexture("./TSTerrainSN_Data/shortGrass.png"));
	terrain->setMaterialTexture(5, videoDriver->getTexture("./TSTerrainSN_Data/test03_l.png"));
	terrain->setMaterialTexture(6, videoDriver->getTexture("./TSTerrainSN_Data/test03_c.png"));

	terrain->setPosition(core::vector3df(0.0f, 0.0f, 0.0f));
	terrain->setScale(core::vector3df(20, 13.75, 20));
	terrain->setMaterialFlag(video::EMF_LIGHTING, false);

#ifdef _USE_FOG
	terrain->setMaterialFlag(video::EMF_FOG_ENABLE, true);
#endif

	terrain->drop();

#endif

#ifdef _TEST_ITERRAIN_SCENENODE 
	terrain										= sceneManager->addTerrainSceneNode(
		"test03.png", NULL, -1, core::vector3df(0, 0, 0), core::vector3df(0, 0, 0), core::vector3df(1, 1, 1),
		video::SColor(255, 255, 255, 255), 6, scene::ETPS_33, 1, false);

	terrain->setMaterialFlag(video::EMF_LIGHTING, false);
#endif

	scene::ICameraSceneNode		*camera			= sceneManager->addCameraSceneNodeFPS(0,100.0f,600.0f);

	camera->setPosition(core::vector3df(4232, 3360, 882));
	camera->setRotation(core::vector3df(48, -35, 0));

	camera->setFarValue(10000.0f);

#ifdef _TEST_ITERRAIN_SCENENODE 
	//terrain->scaleTexture(1, 9.39, 12.85, 7.947, 11.478);
	terrain->setMaterialTexture(0, videoDriver->getTexture("test.png"));
#endif

#ifdef _TEST_IANIMATEDMESH_SCENENODE
	terrain->setMaterialTexture(0, videoDriver->getTexture("test03_d.png"));
#endif

#ifdef _TEST_TERRAIN
	terrain->setPosition(core::vector3df(0.0f, 0.0f, 0.0f));
	terrain->setScale(core::vector3df(/*20, 13.75, 20*/200, 137.5, 200));
	terrain->setMaterialFlag(video::EMF_LIGHTING, false);
	//terrain->setMaterialType(video::EMT_LIGHTMAP);
	//terrain->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
#endif

	/* -------------------------------------------------------------------------------- */
#ifdef _TEST_IANIMATEDMESH_SCENENODE

	IDirect3DDevice9* d3dDevice = videoDriver->getExposedVideoData().D3D9.D3DDev9;

#endif

#ifdef _TEST_ITERRAIN_SCENENODE

//	IDirect3DDevice9* d3dDevice = videoDriver->getExposedVideoData().D3D9.D3DDev9;

#endif

/*
	d3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	d3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	// Allow multiple passes to blend together correctly
	d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	d3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	d3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	d3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// Prevent some ugliness with the alphamaps
	d3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	d3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	d3dDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	d3dDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	d3dDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	d3dDevice->SetSamplerState(2, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	d3dDevice->SetSamplerState(2, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	d3dDevice->SetSamplerState(2, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	d3dDevice->SetSamplerState(3, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	d3dDevice->SetSamplerState(3, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	d3dDevice->SetSamplerState(3, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	d3dDevice->SetSamplerState(4, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	d3dDevice->SetSamplerState(4, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	d3dDevice->SetSamplerState(4, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	d3dDevice->SetSamplerState(5, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	d3dDevice->SetSamplerState(5, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	d3dDevice->SetSamplerState(5, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	d3dDevice->SetSamplerState(6, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	d3dDevice->SetSamplerState(6, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	d3dDevice->SetSamplerState(6, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
*/
	/*
    // Alphamap: take the alpha from the alphamap, we don't care about the color
    d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

    // Texture: take the color from the texture, take the alpha from the previous stage
    d3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    d3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    d3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    d3dDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT); 
	*/
	/* -------------------------------------------------------------------------------- */

	/* -------------------------------------------------------------------------------- */

#ifdef _TEST_TERRAIN
	IDirect3DTexture9* texture1 = NULL;
	IDirect3DTexture9* texture2 = NULL;
	IDirect3DTexture9* texture3 = NULL;
	IDirect3DTexture9* texture4 = NULL;

	IDirect3DTexture9* combinedAlphamap = NULL;
	IDirect3DTexture9* colormap = NULL;
	IDirect3DTexture9* lightmap = NULL;

	if(FAILED(D3DXCreateTextureFromFile(d3dDevice, "grayRock.png", &texture1))) {
		MessageBox(NULL, "Failed to load 'texture1.png'", "Error", MB_OK);
		return;
	}

	if(FAILED(D3DXCreateTextureFromFile(d3dDevice, "hardDirt.png", &texture2))) {
		MessageBox(NULL, "Failed to load 'texture2.png'", "Error", MB_OK);
		return;
	}

	if(FAILED(D3DXCreateTextureFromFile(d3dDevice, "bigRockFace.png", &texture3))) {
		MessageBox(NULL, "Failed to load 'texture3.png'", "Error", MB_OK);
		return;
	}

	if(FAILED(D3DXCreateTextureFromFile(d3dDevice, "shortGrass.png", &texture4))) {
		MessageBox(NULL, "Failed to load 'texture4.png'", "Error", MB_OK);
		return;
	}


	if(FAILED(D3DXCreateTextureFromFile(d3dDevice, "test03_d.png", &combinedAlphamap))) {
		MessageBox(NULL, "Failed to load 'default_d.png'", "Error", MB_OK);
		return;
	}

	
	if(FAILED(D3DXCreateTextureFromFile(d3dDevice, "test03_c.png", &colormap))) {
		MessageBox(NULL, "Failed to load 'default_c.png'", "Error", MB_OK);
		return;
	}

	if(FAILED(D3DXCreateTextureFromFile(d3dDevice, "test03_l.png", &lightmap))) {
		MessageBox(NULL, "Failed to load 'default_l.png'", "Error", MB_OK);
		return;
	}
#endif


	/* -------------------------------------------------------------------------------- */

	/* -------------------------------------------------------------------------------- */

#ifdef _USE_RW_SCENENODE
	RealisticWaterSceneNode* waterSN=new RealisticWaterSceneNode(
		sceneManager, 256.0f, 256.0f,	
		videoDriver->getTexture("data/waterbump.png"),irrDevice->getTimer());
	waterSN->setPosition(core::vector3df(0.f, 0.f, 0.f));
	//waterSN->setScale(core::vector3df(11.5f, 11.5f, 11.5f));
	waterSN->setScale(core::vector3df(1, 1, 1));

	WATER = waterSN;

	waterSN->setScale(core::vector3df(100, 100, 100));

#ifdef _USE_FOG
	waterSN->setMaterialFlag(video::EMF_FOG_ENABLE, true);
#endif

#endif

	/* -------------------------------------------------------------------------------- */

	/* -------------------------------------------------------------------------------- */

#ifdef _TEST_TERRAIN
	D3DCAPS9 caps;

	d3dDevice->GetDeviceCaps(&caps);

	IDirect3DPixelShader9* splatShader = NULL;
	ID3DXBuffer* errorBuffer = 0;

	if(caps.PixelShaderVersion >= D3DPS_VERSION(2, 0)) {
		ID3DXBuffer* pShader = NULL;

		if(FAILED(D3DXAssembleShaderFromFile("splatshader01.psh", NULL, NULL, 0, &pShader, &errorBuffer))) {
			MessageBox(NULL, "D3DXAssembleShaderFromFile failed", "Error", MB_OK);
			if(pShader != NULL) pShader->Release();
			return;
		}

		d3dDevice->CreatePixelShader((DWORD*)pShader->GetBufferPointer(), &splatShader);
		pShader->Release();
	}
	else
		MessageBox(NULL, "Your video card does not support Pixel Shader 1.4.\nYou will only be able to view this example through the Fixed Function Pipeline.", "Warning", MB_OK);
#endif


#ifdef _USE_FOG
	videoDriver->setFog(video::SColor(0, 231, 249, 221), true, 1500, 3000, 0.01, true, true);
#endif

	/* -------------------------------------------------------------------------------- */
	if(irrDevice)
	{
		while(irrDevice->run())
		{
			if (irrDevice->isWindowActive())
			{

				videoDriver->beginScene(true, true, video::SColor(0, 255, 255, 255));

#ifdef _TEST_TERRAIN

				d3dDevice->SetPixelShader(splatShader);

				d3dDevice->SetTexture(0, combinedAlphamap);
				
				d3dDevice->SetTexture(1, texture1);
				d3dDevice->SetTexture(2, texture2);
				d3dDevice->SetTexture(3, texture3);
				d3dDevice->SetTexture(4, texture4);


				d3dDevice->SetTexture(5, lightmap);

#endif

				//d3dDevice->SetTexture(5, colormap);
//d3dDevice->SetTexture(6, colormap);
/*
	//
	d3dDevice->SetTextureStageState(5, D3DTSS_TEXCOORDINDEX, 1);

	d3dDevice->SetTextureStageState(5, D3DTSS_COLOROP, D3DTOP_MODULATE4X);

	d3dDevice->SetTextureStageState(5, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	d3dDevice->SetTextureStageState(5, D3DTSS_COLORARG2, D3DTA_CURRENT);
	d3dDevice->SetTextureStageState (5, D3DTSS_ALPHAOP,  D3DTOP_DISABLE);	

				*/
				
/*
	//
	d3dDevice->SetTextureStageState(6, D3DTSS_TEXCOORDINDEX, 1);

	d3dDevice->SetTextureStageState(6, D3DTSS_COLOROP, D3DTOP_MODULATE);

	d3dDevice->SetTextureStageState(6, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	d3dDevice->SetTextureStageState(6, D3DTSS_COLORARG2, D3DTA_CURRENT);
	d3dDevice->SetTextureStageState (6, D3DTSS_ALPHAOP,  D3DTOP_DISABLE);	
*/
				//g_Direct3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_Vertices, sizeof(QuadVertex));

				sceneManager->drawAll();
				
				videoDriver->endScene();
			}

		}
/*
		combinedAlphamap->Release();
		combinedAlphamap = NULL;
*/

#ifdef _TEST_IANIMATEDMESH_SCENENODE
		texture1->Release();
		texture1 = NULL;
		texture2->Release();
		texture2 = NULL;
		texture3->Release();
		texture3 = NULL;
		texture4->Release();
		texture4 = NULL;
		colormap->Release();
		colormap = NULL;
		lightmap->Release();
		lightmap = NULL;
#endif

#ifdef _TEST_TSHEIGHTTERRAIN_SCENENODE
		terrain = NULL;
#endif

#ifdef _USE_RW_SCENENODE
		waterSN->drop();
#endif

		irrDevice->drop();
	}

	//::checkCurrentHeap();
}