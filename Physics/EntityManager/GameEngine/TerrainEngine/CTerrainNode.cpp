/***************************************************************
*                Terrain Engine issue of					   *
*							Dreamotion3D engine				   *
*															   *
**************************************************************** 
*File Created: 29.03.08                                        *
*File updated: 05.12.08                                        *
*Author:	 TMyke	                                           *
*Contens: Implementation of TerrainNode algorithme             *
***************************************************************/
#include <stdio.h>
#include <irrlicht.h>

#include "Utils.h"
#include "CTerrainNode.h"
#include "TTriangleSelector.h"

#pragma warning( disable : 4996 ) // disable deprecated warning 
using namespace irr;

/****************************************************************/
/****************************************************************/
//=========================================================
//---------------------------------------------------------
// Name: constructor()
// 
//---------------------------------------------------------
CTerrainNode::CTerrainNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id)
										: scene::ISceneNode(parent, mgr, id)
{
	m_iSize			= 0;
	numSheet		= 0;
	m_pHeightMap	= new CHeightMap();
	max_h			= 0;
	t_mgr			= mgr;
	// valeurs de base par defaut
	m_rc			= 8;
	m_larg			= 16;
	m_iSize			= 2048;

	this->setVisible( true);
	Material.setFlag(video::EMF_LIGHTING, true);

	Material.Lighting = true;
	Material.BackfaceCulling = true;
	Material.GouraudShading=true;
	Material.FogEnable=false;

}
//---------------------------------------------------------
// Name: destructor()
// 
//---------------------------------------------------------
CTerrainNode::~CTerrainNode()
{
	m_pHeightMap->Destroy();
	delete m_pHeightMap;
	delete  []grille;

	DelQuad.flush();
	// parcours l'arbre pour établir tous les quad à effacer
	DeleteQuadList(m_pQuadTree);
	for(int i=0; i<DelQuad.nbrelement; i++)
	{
		CQuadNode *quad = DelQuad.get(i);
		delete quad;
	}
	DelQuad.flush();
}
//-------------------------------------------------
// DeleteQuadList()
//-------------------------------------------------
void CTerrainNode::DeleteQuadList(CQuadNode *Pquad)
{
	DelQuad.add(Pquad);

	if( Pquad->m_pChildren[0] != NULL )
	{	// c'est un noeud
	  if( Pquad->m_pChildren[0] )	DeleteQuadList( Pquad->m_pChildren[0] );
	  if( Pquad->m_pChildren[1] )	DeleteQuadList( Pquad->m_pChildren[1] );
	  if( Pquad->m_pChildren[2] )	DeleteQuadList( Pquad->m_pChildren[2] );
	  if( Pquad->m_pChildren[3] )	DeleteQuadList( Pquad->m_pChildren[3] );
	}  
}
//=========================================================
//---------------------------------------------------------
// Name: SetValues()
// 
// numQuads    : largeur du terrain en nombre de Quads 
//               (valeur multiple de 2^n, 2,4,8,16,32,64,128,256,etc...)
// numFaces    : largeur de chaque Quad en nombre de faces 
//               (résolution de chaque Quad, valeur multiple de 2^n, 2,4,8,16,32,64,128,256,etc...)
// sizeTerrain : taille générale du terrain
//---------------------------------------------------------
void CTerrainNode::SetValues(int numQuads, int numFaces, int sizeTerrain)
{
	m_rc = numQuads;
	m_larg = numFaces;
	m_iSize = sizeTerrain;
}
//=========================================================
//---------------------------------------------------------
// Name: Construct()
// 
//---------------------------------------------------------
void CTerrainNode::Construct(char *filename, float scale)
{
	LoadHeightMap( filename, scale);


	// intialisation, et construction du noeud de départ
   max_h=0;
   m_pQuadTree = new CQuadNode( NULL, this );
   m_pQuadTree->m_iCorners[0] = vector3df(0.0f,0.0f,0.0f);
   m_pQuadTree->m_iCorners[1] = vector3df((float)m_iSize ,0.0f,0.0f);
   m_pQuadTree->m_iCorners[2] = vector3df((float)m_iSize ,0.0f,(float)m_iSize );
   m_pQuadTree->m_iCorners[3] = vector3df(0.0f,0.0f,(float)m_iSize);
   m_pQuadTree->Box.MinEdge = vector3df(0.0f,0.0f,0.0f);
   m_pQuadTree->Box.MaxEdge = vector3df((float)m_iSize,(float)m_iSize/10.0f,(float)m_iSize);
   m_pQuadTree->BuildQuadTree();

	// calcul de quelques valeurs de bases:
	m_cote = m_pHeightMap->m_iWidth / m_rc;		// raport largeur du HeightMap / m_rc  (par exemple un height de 1024 et 16 quad de cote = 64)
	m_rk = m_iSize / m_pHeightMap->m_iWidth;
	m_iSizeQuad = m_iSize / m_rc;
	m_pQuadTree->CalculYCorner();

	// init la grille maintenant des Quad 3D
	int n = m_rc * m_rc;
	grille = new CQuadNode*[n];
	max_grille = n;
	memset(grille,0, sizeof(CQuadNode*)*n);
	m_pQuadTree->AffecteQuad();
	numQuad=m_rc * m_rc;

	// le bounding Box de TerrainNode lui même
	// égale à celui du Quad Supprème
	Box = m_pQuadTree->Box;
	// élargie le boundingBox pour éviter l'effet d'écartement visuel par le rendu
	Box.MaxEdge *=1.5;
	Box.MinEdge *=1.5;

	m_pQuadTree->Create3D_Quad(t_mgr);
	m_pQuadTree->SetMaxBox();
	setPosition( vector3df(0,0,0) );
}
//=========================================================
//---------------------------------------------------------
// Name: LoadHeightMap()
// 
// desc: chargement de l'image permettant la constitution
// du relief.
//---------------------------------------------------------
bool CTerrainNode::LoadHeightMap( char *fname, float HScale)
{
	if( !m_pHeightMap->LoadHeightMap( t_mgr, fname, HScale ))
	 {	printf(" <<ERROR>> CTerrainNode(LoadHeightMap)couldn't load heightmap  \r\n");                         
		return false;
	}
	//m_pHeightMap->SetBorders(-20.0);
	m_pHeightMap->SmoothMap();

	return true;
}
//=========================================================
//---------------------------------------------------------
// Name: Render()
// Desc: routine de rendu standart
//	
//---------------------------------------------------------
void CTerrainNode::render()
{
	// mise à jour des info camera frustum
	UpdateCullInfo();

	video::IVideoDriver* driver = t_mgr->getVideoDriver();
	driver->setMaterial(Material);
	driver->setTransform(video::ETS_WORLD, this->getAbsoluteTransformation());

	nQuad=0;
	m_pQuadTree->RenderQuad(t_mgr, 0);
}
//=========================================================
//---------------------------------------------------------
// Name: OnRegisterSceneNode()
// Desc: 
//	
//---------------------------------------------------------
void CTerrainNode::OnRegisterSceneNode()
{
		if(IsVisible)
			SceneManager->registerNodeForRendering(this);

		ISceneNode::OnRegisterSceneNode();
}
//=========================================================
//---------------------------------------------------------
// Name: getBoundingBox()
// Desc: retourne le boundingbox de TerrainNode
//	
//---------------------------------------------------------
const core::aabbox3d<f32>& CTerrainNode::getBoundingBox() const
{
	return Box;
}
//=========================================================
//---------------------------------------------------------
// Name: setMaterialTexture()
// Desc: travail sur la textures des Quad du Terrain
//	
//---------------------------------------------------------
void CTerrainNode::setMaterialTexture(u32 textureLayer, video::ITexture* texture)
{
	if (textureLayer >= video::MATERIAL_MAX_TEXTURES)    return;

    Material.setTexture(textureLayer, texture);
	// on applique a tous les quad maintenant
	for(int j=0 ; j<max_grille; j++)
	{
		CQuadNode  *pQuad = this->grille[j];
		pQuad->Material.setTexture(textureLayer, texture);
	}
}
//=========================================================
//---------------------------------------------------------
// Name: setMaterialType()
// Desc: travail sur le type de aterial des Quad du Terrain
//	
//---------------------------------------------------------
void CTerrainNode::setMaterialType(video::E_MATERIAL_TYPE newType)
{
    Material.MaterialType = newType;
	// on applique a tous les quad maintenant
	for(int j=0 ; j<max_grille; j++)
	{
		CQuadNode  *pQuad = this->grille[j];
		pQuad->Material.MaterialType = newType;
	}
}
//=========================================================
//---------------------------------------------------------
// Name: setMaterialFlag()
// Desc: travail sur les flag des materials des Quad du Terrain
//	
//---------------------------------------------------------
void CTerrainNode::setMaterialFlag(video::E_MATERIAL_FLAG flag, bool newvalue)
{
	Material.setFlag(flag, newvalue);
	// on applique a tous les quad maintenant
	for(int j=0 ; j<max_grille; j++)
	{
		CQuadNode  *pQuad = this->grille[j];
		pQuad->Material.setFlag(flag, newvalue);
	}
}
//======================================================================================
//--------------------------------------------------------------------------------------
// name:	DM_GetTerrainHeight()
//
// Retourne la hauteur du terrain en fonction des coordonnées x et z fournies.
//--------------------------------------------------------------------------------------
float CTerrainNode::GetTerrainHeight(float x, float z)
{
	return this->m_pHeightMap->GetInterpolateHeight(x/m_rk,  z/m_rk);
}
//======================================================================================
//--------------------------------------------------------------------------------------
// name:	GetQuadNode()
//
// Retourne le pointeur d'un Quad souhaité
//--------------------------------------------------------------------------------------
CQuadNode* CTerrainNode::GetQuadNode(int num)
{
	return this->grille[num];
}
CQuadNode* CTerrainNode::GetQuadNode(int x, int y)
{
	return this->grille[x + y*m_rc];
}
//======================================================================================
//--------------------------------------------------------------------------------------
// Name: UpdateCullInfo()
//
// Desc: Update Cull Info, attention  avec la camera courante
//--------------------------------------------------------------------------------------
void CTerrainNode::UpdateCullInfo()
{
	scene::ICameraSceneNode* cam = t_mgr->getActiveCamera();
	core::matrix4 pMatProj, pMatView;
	pMatProj = cam->getProjectionMatrix();
	pMatView = cam->getViewMatrix();
	core::matrix4 VPI = pMatProj * pMatView;

	Plane col0(VPI(0,0), VPI(1,0), VPI(2,0), VPI(3,0));
	Plane col1(VPI(0,1), VPI(1,1), VPI(2,1), VPI(3,1));
	Plane col2(VPI(0,2), VPI(1,2), VPI(2,2), VPI(3,2));
	Plane col3(VPI(0,3), VPI(1,3), VPI(2,3), VPI(3,3));
	// construit les 6 plane du Frustum view
	mFrustumPlane[0] = col2;          // near
	mFrustumPlane[1] = (col3 - col2); // far
	mFrustumPlane[2] = (col3 + col0); // left
	mFrustumPlane[3] = (col3 - col0); // right
	mFrustumPlane[4] = (col3 - col1); // top
	mFrustumPlane[5] = (col3 + col1); // bottom
	for(int i = 0; i < 6; i++)		
	{
		mFrustumPlane[i].Normalize();
	}
}
//======================================================================================
//--------------------------------------------------------------------------------------
// Name: _isVisible()
//
// Desc: test de visibilité d'un mesh
//--------------------------------------------------------------------------------------
int CTerrainNode::_isVisible(const core::aabbox3d<f32>& box)
{
	float P[3];
	float Q[3];
	// la classe vector3d ne possédant pas d'opérateur de surcharge []....
	float Min[3];	memcpy( Min, &box.MinEdge.X, sizeof(float)*3);
	float Max[3];	memcpy( Max, &box.MaxEdge.X, sizeof(float)*3);
	for(int i = 0; i < 6; ++i)
	{
		// pour chaque coordonnées x, y, z...
		for(int j = 0; j < 3; ++j)
		{
			// etabli le poiont PQ dans la même direction que le plan normal sur l'axe.
			if( mFrustumPlane[i][j] >= 0.0f )
			{
				P[j] = Min[j];
				Q[j] = Max[j];
			}
			else 
			{
				P[j] = Max[j];
				Q[j] = Min[j];
			}
		}
		if( mFrustumPlane[i].DotCoord(Q) < 0.0f  ) // en dehor, exit procedure
			return false;
	}
	return true;
}
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//-----------------------------------------------------------------------------
// Name: TextureTerrain()
// Desc: 
//       
//	nquad = quad d'origine pour le dépard de la texturation
//	layer = étage de texture concerné (de 0 ou 1 )
//	n	  = largeur en quad de la texturation
//-----------------------------------------------------------------------------
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
void CTerrainNode::TextureTerrain(int nquad, int layer, int n,  video::ITexture* texture)
{
	float i2,j2;
	int j,i,kx, ky;
	// recupère le qued en X et Z
	int fact = this->m_rc;
	int x = nquad / fact;
	int z = nquad - (x*fact);
	int	larg = this->m_larg;

	CQuadNode   *pQuad;
	pQuad = this->grille[nquad];
	pQuad->Material.setTexture(layer, texture);
	
	CQuadNode *QuadR, *Quadd;
	// quad de reference
	QuadR = this->grille[nquad];
	int id;
	for(j=0 ; j<n; j++)
	{
		for(i=0; i<n; i++)
		{	id = nquad+i + j*fact;
			// test les bornes
			if( (id<0) || (id> this->max_grille-1) )
			{
				// erreur
				return;
			}
			Quadd = this->grille[id];
			Quadd->Material.setTexture(layer, texture);
		}
	}
	// nouvelle buté
	float pa = 1.0f / (float)n;	
	float pa2= 1.0f / ( (float)QuadR->m_pQuadTerrain->m_larg * (float)n );
	layer = layer * 2;
	for(j=0 ; j<n; j++)
	{
		for(i=0; i<n; i++)
		{
			Quadd = this->grille[nquad+ i + j*fact];
			i2 = 1.0f - (float)j * pa ;
			i2 = i2 - pa2;
			j2 = (float)i * pa;
			// accès vertices
			int pp=0;
			int cur=0;
			scene::SMeshBufferLightMap* vbuffer = Quadd->mbuffer;
			for(kx=0; kx<larg; kx++)
			{
				for(ky=0; ky<larg; ky++)
				{
					if(layer)	{vbuffer->Vertices[cur].TCoords2.X = i2+pa2;	vbuffer->Vertices[cur].TCoords2.Y = j2+pa2;}
					else		{vbuffer->Vertices[cur].TCoords.X = i2+pa2;		vbuffer->Vertices[cur].TCoords.Y = j2+pa2;}
					cur++;

					if(layer)	{vbuffer->Vertices[cur].TCoords2.X = i2+pa2; vbuffer->Vertices[cur].TCoords2.Y = j2;	}
					else		{vbuffer->Vertices[cur].TCoords.X = i2+pa2;	 vbuffer->Vertices[cur].TCoords.Y = j2;	}
					cur++;

					if(layer)	{vbuffer->Vertices[cur].TCoords2.X = i2;	vbuffer->Vertices[cur].TCoords2.Y = j2;	}
					else		{vbuffer->Vertices[cur].TCoords.X = i2;		vbuffer->Vertices[cur].TCoords.Y = j2;	}
					cur++;

					if(layer)	{vbuffer->Vertices[cur].TCoords2.X = i2;	vbuffer->Vertices[cur].TCoords2.Y = j2+pa2;	}
					else		{vbuffer->Vertices[cur].TCoords.X = i2;		vbuffer->Vertices[cur].TCoords.Y = j2+pa2;	}
					cur++;
				
					j2 = j2 + pa2;
				}
				j2 = (float)i * pa;
				i2=i2-pa2;
			}
		}
	}
}
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
// partie pour les tests de collisions et de picking
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//------------------------------------------------------------------
// retourne la dernière position Picked sur entity ou terrain
//------------------------------------------------------------------
core::vector3df CTerrainNode::getPickedPosition()
{
	return global_pick.position;
}
core::vector3df CTerrainNode::getPickedNormal()
{
	return global_pick.normal;
}
int CTerrainNode::getPickedQuad()
{
	return global_pick.quad;
}
core::triangle3df CTerrainNode::getPickedTriangle()
{
	return global_pick.tri;
}
//=========================================================
//---------------------------------------------------------
// Name: CameraVectorComputePick()
// picking a travers un espace définit par une camera
//=========================================================
void CTerrainNode::CameraVectorComputePick(int x, int y, core::vector3df *vPickRayOrig, core::vector3df *vPickRayDir)
{
	core::matrix4 mView, mProj;

	scene::ICameraSceneNode* cam = t_mgr->getActiveCamera();
	mProj = cam->getProjectionMatrix();
	mView = cam->getViewMatrix();

	video::IVideoDriver* driver = t_mgr->getVideoDriver();
	core::dimension2d<s32> viewPort = driver->getScreenSize();

    core::vector3df v;
	v.X =  ( ( ( 2.0f * x ) / viewPort.Width  ) - 1 ) / mProj(0,0);
	v.Y = -( ( ( 2.0f * y ) / viewPort.Height ) - 1 ) / mProj(1,1);
    v.Z =  1.0f;

    // inverse la matrice de view
    core::matrix4 m = mView;
	m.makeInverse();

    // Transforme le pick ray de l'ecran dans l'espace 3D
	vPickRayDir->X  = v.X*m(0,0) + v.Y*m(1,0) + v.Z*m(2,0);
    vPickRayDir->Y  = v.X*m(0,1) + v.Y*m(1,1) + v.Z*m(2,1);
    vPickRayDir->Z  = v.X*m(0,2) + v.Y*m(1,2) + v.Z*m(2,2);
	vPickRayDir->normalize();
    vPickRayOrig->X = m(3,0);
    vPickRayOrig->Y = m(3,1);
    vPickRayOrig->Z = m(3,2);
	*vPickRayOrig += *vPickRayDir*1.0f;
}
//-----------------------------------------------------------------------------
// Name: PickTerrain()
// Desc: tests de picking
//-----------------------------------------------------------------------------
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
bool  CTerrainNode::PickTerrain(int x, int y, int DistBase)
{
	core::vector3df vPickRayDir;
    core::vector3df vPickRayOrig;

	// compute les vecteur 3D Ray
	CameraVectorComputePick(x, y, &vPickRayOrig, &vPickRayDir);
	return Raycast(vPickRayDir, vPickRayOrig, DistBase);
}
//-----------------------------------------------------------------------------
// Name: Raycast()
// Desc: Effectue un test de RayCasting sur le terrain, et renvoi dans une structure 
// les différentess info.
//-----------------------------------------------------------------------------
bool CTerrainNode::Raycast(core::vector3df vPickRayDir, core::vector3df vPickRayOrig, int DistBase)
{
	core::matrix4 mWorld, mView, mProj;
	bool fflag=false;
	bool flag2=false;

	// convert la ligne de picking dans l'espace du model
	core::vector3df vNear,vDir;
	core::matrix4 invMat;

	invMat = this->getAbsoluteTransformation();
	invMat.makeInverse();
	invMat.transformVect(vPickRayOrig, vPickRayOrig);
	invMat.transformVect(vPickRayDir,vPickRayDir);
	vPickRayDir.normalize();

		core::vector3df pos;
		pos = vPickRayOrig;
		float xl = (float)this->m_pHeightMap->m_iHeight *this->m_rk;
		float yl = (float)this->m_pHeightMap->m_iWidth *this->m_rk;
		float hh;
		for(int i=0; i<DistBase; i++)
		{
			// vérifie les bord
			if( (pos.X>xl) || (pos.X<0) || (pos.Z>yl) || (pos.Z<0) )
			{
				hh=1000.0;
				fflag=false;
				//return false;
			}
			else
			{	
				hh = this->m_pHeightMap->GetInterpolateHeight((float)pos.X/this->m_rk, (float) pos.Z/this->m_rk);		
				fflag=true;
			}

			if(hh<pos.Y) flag2=true;
			
			if( (hh>pos.Y) && fflag && flag2)
			{
				global_pick.position.X = pos.X;
				global_pick.position.Y = hh;
				global_pick.position.Z = pos.Z;
				int xx = (int)pos.X;
				int yy = (int)pos.Z;
				int ll = this->m_iSize / this->m_rc;	// taille de chaque Quad 
				xx = xx / ll;
				yy = yy / ll;
				xx = (yy * this->m_rc)  + xx;
				global_pick.quad = xx;
				QuadCollideTest(xx, vPickRayDir, vPickRayOrig, DistBase);
				return true;
			}
			pos = pos + vPickRayDir;
		}
	return false;
}
//-----------------------------------------------------------------------------
// Name: QuadCollideTest
// Desc: Test de collision avec un quad()
//-----------------------------------------------------------------------------
bool CTerrainNode::QuadCollideTest(int num_quad, core::vector3df vPickRayDir, core::vector3df vPickRayOrig, int DistBase)
{
	CQuadNode *quad = this->grille[num_quad];

	scene::ITriangleSelector* selector = new scene::TTriangleSelector(quad->mbuffer);

	core::line3d<f32> line;
	line.start = vPickRayOrig;
	line.end = line.start + (vPickRayDir * (float)DistBase);
	core::vector3df intersection;
	core::triangle3df tri;

	bool res = this->getCollisionPoint(line, selector, intersection, tri);
	selector->drop();
	
	global_pick.tri = tri;
	global_pick.normal = tri.getNormal();

	return res;
}
//-----------------------------------------------------------------------------
// Name: getCollisionPoint
// Desc: Test de collision point interne (visiblement celui en natif plante, et je ne comprend pas le passage des
//		 parametres)
//-----------------------------------------------------------------------------
bool CTerrainNode::getCollisionPoint(const core::line3d<f32>& ray,
									 scene::ITriangleSelector* selector, core::vector3df& outIntersection,
									 core::triangle3df& outTriangle)
{
	if (!selector)	return false;

	core::array<core::triangle3df> Triangles; // triangle buffer 

	s32 totalcnt = selector->getTriangleCount();
	Triangles.set_used(totalcnt);

	s32 cnt = 0;
	core::matrix4 mat;	mat.makeIdentity();
	selector->getTriangles(Triangles.pointer(), totalcnt, cnt, &mat);

	const core::vector3df linevect = ray.getVector().normalize();
	core::vector3df intersection;
	f32 nearest = 9999999999999.0f;
	bool found = false;
	const f32 raylength = ray.getLengthSQ();

	for (s32 i=0; i<cnt; ++i)
	{
		if (Triangles[i].getIntersectionWithLine(ray.start, linevect, intersection))
		{
			const f32 tmp = intersection.getDistanceFromSQ(ray.start);
			const f32 tmp2 = intersection.getDistanceFromSQ(ray.end);

			if (tmp < raylength && tmp2 < raylength && tmp < nearest)
			{
				nearest = tmp;
				outTriangle = Triangles[i];
				outIntersection = intersection;
				found = true;
			}
		}
	}
	return found;
}
//======================================================================================
//--------------------------------------------------------------------------------------
// name:	DM_SetHeightTerrain()
// 
// Desc: permet de changer la hauteur terrain (sur le heightmap) et de la répercuter
//       sur la geométrie des quad concernés
//======================================================================================
bool TestBorneGrille(CTerrainNode *terrain, int val)
{
	if( (val>=0) && (val<terrain->numQuad) ) return true;
	return false;
}
void CTerrainNode::SetHeightTerrain(int x, int y, float hh)
{
	int xx = x;
	int yy = y;
	int ll = this->m_iSize / this->m_rc;	// taille de chaque Quad 
	xx = xx / ll;
	yy = yy / ll;
	xx = (yy * this->m_rc)  + xx;
	// numéro du quad dans 'xx'
	CQuadNode  *pQuad = this->grille[xx];


	int width = this->m_pHeightMap->m_iWidth;
	int ratio = this->m_iSize / this->m_pHeightMap->m_iWidth;
	int xt = x / ratio;
	int yt = y / ratio;
	this->m_pHeightMap->m_pHeightMap[xt + yt * width] = hh;

	pQuad->ReScale(t_mgr);

	int perif;
	int id = this->m_rc;
	perif = xx -1;		if(TestBorneGrille(this, perif)) {  pQuad = (CQuadNode*)this->grille[perif];	pQuad->ReScale(t_mgr);	}
	perif = xx +1;		if(TestBorneGrille(this, perif)) {  pQuad = (CQuadNode*)this->grille[perif];	pQuad->ReScale(t_mgr);	}
	perif = xx-id-1;	if(TestBorneGrille(this, perif)) {  pQuad = (CQuadNode*)this->grille[perif];	pQuad->ReScale(t_mgr);	}
	perif = xx-id+1;	if(TestBorneGrille(this, perif)) {  pQuad = (CQuadNode*)this->grille[perif];	pQuad->ReScale(t_mgr);	}
	perif = xx-id;		if(TestBorneGrille(this, perif)) {  pQuad = (CQuadNode*)this->grille[perif];	pQuad->ReScale(t_mgr);	}
	perif = xx+id;		if(TestBorneGrille(this, perif)) {  pQuad = (CQuadNode*)this->grille[perif];	pQuad->ReScale(t_mgr);	}
	perif = xx+id+1;	if(TestBorneGrille(this, perif)) {  pQuad = (CQuadNode*)this->grille[perif];	pQuad->ReScale(t_mgr);	}
	perif = xx+id-1;	if(TestBorneGrille(this, perif)) {  pQuad = (CQuadNode*)this->grille[perif];	pQuad->ReScale(t_mgr);	}

}