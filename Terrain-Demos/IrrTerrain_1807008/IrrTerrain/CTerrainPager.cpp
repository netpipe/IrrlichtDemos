/*
Terrain Pager Class

Author : fredakilla@gmail.com

Description :

Create a terrain pager for the custom Tiled-Terrain SceneNode originally created 
by Copland (http://forum.irrlicht.fr).

Terrain Pager gives the possibility to Load/Unload terrains depending of the camera position 
(huge terrain render and low cost memory).


Description (FR) :

Créé un systeme de pagination de terrain afin de permettre de charger et decharger
à la volé des parcelles de terrains en vue d'un rendu d'un enorme terrain tout en optimisant
la mémoire vive consommé. Seules les parcelles de terrain visibles et dans une zone
entourant la camera sont chargées. 
Il n'est pas nécéssaire d'utiliser un systeme de threads pour le chargement des terrains mais
dans ce cas la taille des maps pour chaque terrain ne devrait pas exceder 257x257 sinon 
un lag due au chargement de la map est constaté. 
Le chargement et dechargement des terrains s'effectue par rapport a la distance entre 
la position de la camera et les bounding box theoriques des parcelles de terrains.
Idéalement la zone de chargement d'une parcelle est fixé à 138% de la taille d'une map de 256.
La zone de dechargement est de 138% +5% de marge pour eviter de decharger la map si on  
viens juste de la charger.
La distance de visibilité (DistanceMaxRender) peut osciller entre 100 et 250 pour des 
performances et une qualité optimale au dessus de 250, les performances commencent 
à se deteriorer largement.
En mode DEBUG le chargement des map est perceptible mais compilé en mode RELEASE
le lag disparait (pour des map de 257), rendant inutile un systeme de chargement des 
parcelles de terrain par threads.

2 mode de texturing sont possibles
- avec des colormaps pour chaque parcelle et une/des detailmap
- avec un shader

Problemes connus :
- avec OpenGL des bordures entres les colormap sont visibles

En construction :
- shader specifique pour texturer les terrains via blending et alphamaps
*/


#include "CTerrainPager.h"
#include <irrlicht.h>

using namespace irr;

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CTerrainPager::CTerrainPager(STerrainParameters param, scene::ISceneNode* parent, 
			scene::ISceneManager* smgr, s32 id) : scene::ISceneNode(parent, smgr, id)
{ 	
	this->SceneManager = smgr;

	// get parameters
	Param = param;	
	Param.DistanceMaxRender = param.DistanceMaxRender * Param.Scale.X; // set with scale factor

	// nb terrains
	u16 NbTiles = Param.Dimension.Width * Param.Dimension.Height;
	
	posX = 0.0f;
	posZ = 0.0f;
	x=0;

	// charge dans un vecteur la liste de nom des fichiers map
	for(u16 i=0; i<NbTiles; i++)
	{
		heightMapList.push_back(Param.MapListFileName[i]);
	}


	// crée les zones buffer
	for(u16 i=0; i<NbTiles; i++)
	{		
		CBufferZone * zone = new CBufferZone(Param.MapSize,
										core::vector3df(posX, 0, posZ),
										Param.Scale,
 										this, smgr, 0);
			zoneList.push_back(zone);
			computePosition(i);
	}

	setAutomaticCulling(scene::EAC_OFF);
}

//----------------------------------------------------------------------------------------
// Destructor 
//----------------------------------------------------------------------------------------
CTerrainPager::~CTerrainPager()
{
}

//----------------------------------------------------------------------------------------
// compute tile position
// TODO : tweak for use with different  
//----------------------------------------------------------------------------------------
void CTerrainPager::computePosition(u16)
{
	f32 imageSize = Param.MapSize;

	x++;
	if(x >= Param.Dimension.Width)
	{
		posX = 0.0f;
		posZ += imageSize;
		x=0;
	}
	else
	{			
		posX += imageSize;
	}
}

//----------------------------------------------------------------------------------------
// load terrain
//----------------------------------------------------------------------------------------
void CTerrainPager::loadTerrain(u16 index)
{
	CTerrain * Terrain = new CTerrain(heightMapList[index], Param.Quality, Param.LodTextureScale, this, this->SceneManager, 0);
	Terrain->setPosition(zoneList[index]->getPosition());
	Terrain->setScale(zoneList[index]->getScale());
	Terrain->setRenderDistance(Param.DistanceMaxRender);

	// normal texturing (colormap + detailmap)
	Terrain->setColorTexture(Param.ColorMapListFileName[index]);	
	Terrain->setDetailTexture("Media2/detail.jpg");

	// shader texturing (under construction)
	//Terrain->setShader("shaders/test4.hlsl", SceneManager);
	//Terrain->setTexture(0,Param.ColorMapListFileName[index]);	
	//Terrain->setTexture(1,"Media/Image2.jpg");		
	//Terrain->setTexture(2,"Media/lightmap.jpg");	
	//Terrain->setTexture(3,"Media/dirt.bmp");	

	// set fog
	if(Param.Fog)
		Terrain->setMaterialFlag(video::EMF_FOG_ENABLE, true);

	// show bounding box per terrain
	if(Param.Debug)
	{
		this->zoneList[index]->setDebugDataVisible(scene::EDS_BBOX);
		//Terrain->recalculateBoundingBox();
		//Terrain->setDebugDataVisible(scene::EDS_BBOX);
	}

	zoneList[index]->setLoaded(true);	
	zoneList[index]->setObjectInside(Terrain);
}


//----------------------------------------------------------------------------------------
// Check if a terrain need to be load or unload
//----------------------------------------------------------------------------------------
void CTerrainPager::render()
{	
  	scene::ICameraSceneNode* cam = SceneManager->getActiveCamera();
	const scene::SViewFrustum* frustum = cam->getViewFrustum();
	video::IVideoDriver* Driver = SceneManager->getVideoDriver();
	core::vector3df Pos = cam->getPosition();	
	Pos.Y = getPosition().Y;	
	Driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

	size_t size = zoneList.size();
	for (u32 i=0; i<size; i++)
	{			
		f64 ActualDistance = zoneList[i]->getBoundingBox().getCenter().getDistanceFrom(Pos);
		bool load = zoneList[i]->isLoaded();

		if(ActualDistance < Param.DistanceLoad && load == false)
		{
			loadTerrain(i);
		}
		else if(ActualDistance > Param.DistanceUnload && load == true)
		{ 
			delete zoneList[i]->getObjectInside();				
			zoneList[i]->setLoaded(false);					
		}		
	}
}

void CTerrainPager::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}


const core::aabbox3d<f32>& CTerrainPager::getBoundingBox() const
{
	return Box;
}