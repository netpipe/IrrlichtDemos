/***************************************************************
*                Terrain Engine issue of					   *
*							Dreamotion3D engine				   *
*															   *
**************************************************************** 
*File Created: 29.03.08                                        *
*File updated: 05.12.08                                        *
*Author:			                                           *
*Contens: Implementation of QuadTree   algorithme              *
***************************************************************/
#include "windows.h"
#include <stdio.h>
#include <irrlicht.h>

#include "CList.h"
#include "Utils.h"
#include "CList.h"
#include "CTerrainNode.h"
#include "CQuad.h"
#pragma warning( disable : 4996 ) // disable deprecated warning 


using namespace irr;
using namespace core;




/*********************************************************************************
//********************************************************************************
//********************************************************************************/
//-------------------------------------------------
// CQuadNode()
// Desc:  constructeur 
//
// *parent : parent dans l'arbre quadtree
// *mgr    : récupération du pointeur ISceneManager
CQuadNode::CQuadNode(CQuadNode *Parent, CTerrainNode *Terrain, scene::ISceneManager* mgr)
{
    m_pParent      = Parent;
	for(int i = 0; i < 4; i++)
	{
	  m_pChildren[i] = NULL;
  	  m_iCorners[i]  = vector3df(0.0,0.0,0.0);
	}
	m_pQuadTerrain    = Terrain;
	m_cCenter         = vector3df( 0,0,0 );
	Debug=false;
	mbuffer=NULL;
}
//-------------------------------------------------
// ~CQuadNode()
//-------------------------------------------------
// Destructeur 
CQuadNode::~CQuadNode()
{
	if(mbuffer) mbuffer->drop();
}
//-------------------------------------------------
// SetBox()
//-------------------------------------------------
// établit la boite englobante de chaque élément
// de l'arbre
void CQuadNode::SetBox()
{
	vector3df max = vector3df(0,0,0);
	vector3df min = vector3df(999990,999990,999990);
	for(int i=0; i<4; i++)
	{
		if(m_iCorners[i].X<min.X) min.X =  m_iCorners[i].X;
		if(m_iCorners[i].Y<min.Y) min.Y =  m_iCorners[i].Y;
		if(m_iCorners[i].Z<min.Z) min.Z =  m_iCorners[i].Z;
		
		if(m_iCorners[i].X>max.X) max.X =  m_iCorners[i].X;
		if(m_iCorners[i].Y>max.Y) max.Y =  m_iCorners[i].Y;
		if(m_iCorners[i].Z>max.Z) max.Z =  m_iCorners[i].Z;
	}
	min.Y=0.0f;
	max.Y=0.0f;
	Box.MaxEdge = max;
	Box.MinEdge = min;
	//Box = aabbox3d<f32>(min.X,  min.Y, min.Z, max.X, max.Y, max.Z);
}
//-------------------------------------------------
// SetMaxBox()
//-------------------------------------------------
// redéfinit rétrospectivement la boite englobante
// de chaque élément avec la valeur max en hauteur
void CQuadNode::SetMaxBox()
{	
	Box.MaxEdge.Y = m_pQuadTerrain->max_h;
  if( m_pChildren[0] != NULL )
  {	// c'est un noeud
	  if( m_pChildren[0] )
		  m_pChildren[0]->SetMaxBox();
	  if( m_pChildren[1] )
		  m_pChildren[1]->SetMaxBox();
	  if( m_pChildren[2] )
		  m_pChildren[2]->SetMaxBox();
	  if( m_pChildren[3] )
		  m_pChildren[3]->SetMaxBox();
  }  
}
//-------------------------------------------------
// Subdivide()
//-------------------------------------------------
// Subdivision récursive des éléments de l'arbre pour
// arriver au plus petit élément souhaité
bool CQuadNode::Subdivide()
{
	vector3df iCorners[4];
    float x0 = ( m_iCorners[0].X + m_iCorners[1].X ) / 2;
	float z0 = ( m_iCorners[0].Z + m_iCorners[1].Z ) / 2;

    float x1 = ( m_iCorners[1].X + m_iCorners[2].X ) / 2;
	float z1 = ( m_iCorners[1].Z + m_iCorners[2].Z ) / 2;

    float x2 = ( m_iCorners[3].X + m_iCorners[2].X ) / 2;
	float z2 = ( m_iCorners[3].Z + m_iCorners[2].Z ) / 2;

    float x3 = ( m_iCorners[3].X + m_iCorners[0].X ) / 2;
	float z3 = ( m_iCorners[3].Z + m_iCorners[0].Z ) / 2;

	vector3df center = (m_iCorners[0]+m_iCorners[1]+m_iCorners[2]+m_iCorners[3]) / 4; 
	radius = Distance3( &m_iCorners[0], &center);

	if( ( m_iCorners[1].X - m_iCorners[0].X ) <= (float)(m_pQuadTerrain->m_iSize/m_pQuadTerrain->m_rc))
					return false;   //on ne peut pas subdiviser plus

	// children 0
	m_pChildren[ 0 ] = new CQuadNode( this, m_pQuadTerrain );
	 if( !m_pChildren[0] )
	 {	 printf(" <<ERROR>> CQuad(Subdivide): Subdivide: out of memory  \r\n");                         
		 return false;
	 }
	 iCorners[0] = m_iCorners[0];
	 iCorners[1] = vector3df(x0,0.0,z0);
	 iCorners[2] = center;
	 iCorners[3] = vector3df(x3,0.0,z3);
	 m_pChildren[ 0 ]->Allocate( iCorners);
	 m_pChildren[ 0 ]->SetBox();

     m_pChildren[ 1 ] = new CQuadNode(this, m_pQuadTerrain );
	 if( !m_pChildren[1] )
	 {	 printf(" <<ERROR>> CQuad(Subdivide): Subdivide: out of memory  \r\n");                         
		 return false;
	 }
	 iCorners[0] = vector3df(x0,0.0,z0);
	 iCorners[1] = m_iCorners[1];
	 iCorners[2] = vector3df(x1,0.0,z1);
	 iCorners[3] = center;
	 m_pChildren[ 1 ]->Allocate( iCorners);
	 m_pChildren[ 1 ]->SetBox();

     m_pChildren[ 2 ] = new CQuadNode( this, m_pQuadTerrain );
	 if( !m_pChildren[2] )
	 {   printf(" <<ERROR>> CQuad(Subdivide): Subdivide: out of memory  \r\n");                         
		 return false;
	 }
	 iCorners[0] = center;
	 iCorners[1] = vector3df(x1,0.0,z1);
	 iCorners[2] = m_iCorners[2];
	 iCorners[3] = vector3df(x2,0.0,z2);
	 m_pChildren[ 2 ]->Allocate( iCorners );
	 m_pChildren[ 2 ]->SetBox();

     m_pChildren[ 3 ] = new CQuadNode( this, m_pQuadTerrain );
	 if( !m_pChildren[3] )
	 {	 printf(" <<ERROR>> CQuad(Subdivide): Subdivide: out of memory  \r\n");                         
		 return false;
	 }
	 iCorners[0] = vector3df(x3,0.0,z3);
	 iCorners[1] = center;
	 iCorners[2] = vector3df(x2,0.0,z2);
	 iCorners[3] = m_iCorners[3];
	 m_pChildren[ 3 ]->Allocate( iCorners );
	 m_pChildren[ 3 ]->SetBox();

  return true;
}
//-------------------------------------------------
// Allocate()
//-------------------------------------------------
// Allocation des valeurs de base pour les cellules
// filles
// Corners[4] : prochaines dimension à transmettre
bool CQuadNode::Allocate( vector3df Corners[4])
{
	for(int i = 0; i < 4; i++)
	{
	  m_pChildren[i] = NULL;
	  m_iCorners[i]  = Corners[i];
	}
	m_cCenter = (m_iCorners[0]+m_iCorners[1]+m_iCorners[2]+m_iCorners[3]) / 4;
	return true;
}
//-------------------------------------------------
// BuildQuadTree()
//-------------------------------------------------
// point d'entré pour la construction de l'arbre
void CQuadNode::BuildQuadTree()
{	
    if( Subdivide() )
	{
		m_pChildren[0]->BuildQuadTree();
		m_pChildren[1]->BuildQuadTree();
		m_pChildren[2]->BuildQuadTree();
		m_pChildren[3]->BuildQuadTree();
	}	
	else 
		m_pQuadTerrain->numSheet++;	
}
//-------------------------------------------------
// CalculYCorner()
//-------------------------------------------------
// calcul des coins d'un quad
void CQuadNode::CalculYCorner()
{ 
	float f = (float) m_pQuadTerrain->m_rk;
	
  for(short i=0; i<4; i++)
	  m_iCorners[i].Y = m_pQuadTerrain->m_pHeightMap->GetHeightValue( m_iCorners[i].X/f,
																	  m_iCorners[i].Z/f);

  if( m_pChildren[0] != NULL )
  {	// c'est un noeud
	  if( m_pChildren[0] )
		  m_pChildren[0]->CalculYCorner();
	  if( m_pChildren[1] )
		  m_pChildren[1]->CalculYCorner();
	  if( m_pChildren[2] )
		  m_pChildren[2]->CalculYCorner();
	  if( m_pChildren[3] )
		  m_pChildren[3]->CalculYCorner();
  } 
  else
	  return;
}
//-------------------------------------------------
// Create3D_Quad()
//-------------------------------------------------
// point d'entré pour la creation géometrique des
// éléments terminaux de l'abre
#define DIFF 0xFFFFFFFF
void CQuadNode::Create3D_Quad(scene::ISceneManager *mgr)
{ 
	if( m_pChildren[0] != NULL )
	{	// c'est un noeud
	  if( m_pChildren[0] )
		  m_pChildren[0]->Create3D_Quad(mgr);
	  if( m_pChildren[1] )
		  m_pChildren[1]->Create3D_Quad(mgr);
	  if( m_pChildren[2] )
		  m_pChildren[2]->Create3D_Quad(mgr);
	  if( m_pChildren[3] )
		  m_pChildren[3]->Create3D_Quad(mgr);
	}  
	else
	{ // c'est un Sheet, creation du 3D Buffer
			CreateQuadMesh(mgr);
	}
	// calcul le centre geométrique des coins
	vector3df coin = 	m_iCorners[0]+m_iCorners[1]+m_iCorners[2]+m_iCorners[3]; 
	coin = coin / 4.0;
	d_mCorners = coin;
}
//-------------------------------------------------
// CreateQuadMesh()
//-------------------------------------------------
// construction 3D d'un élément noeud terminal.
// volontairement décomposé, pour une meilleurs
// compréhension
void CQuadNode::CreateQuadMesh(scene::ISceneManager*	mgr)
{
	vector3df origin;
	vector3df v1,v2,v3,v4,av,bv,Normal;
	short i,j,p;
	float pa,ps,ii,jj,facteur;
	float pa2,i2,j2;
	bool flagNew=false;

	// instanciation du mbuffer
	if(mbuffer==NULL)
	{	flagNew=true;
		mbuffer=new scene::SMeshBufferLightMap;
		// réservaton des espaces nescessaire dans les buffer Vertices et Indices
		mbuffer->Indices.set_used(m_pQuadTerrain->m_larg * m_pQuadTerrain->m_larg*2*3);
		mbuffer->Vertices.set_used(m_pQuadTerrain->m_larg * m_pQuadTerrain->m_larg*4);
	}

	// coin de départ
	origin = m_iCorners[0];

	// calcul du 'pas' pour les texture map
	pa  = 1.0f/(float)(m_pQuadTerrain->m_rc*m_pQuadTerrain->m_larg);
	pa2 = 1.0f /m_pQuadTerrain->m_larg;

	// calcul du 'pas' entre les faces
	ps = abs(m_iCorners[0].X-m_iCorners[1].X)/ m_pQuadTerrain->m_larg;

	float index = (float)(m_pQuadTerrain->m_iSize/m_pQuadTerrain->m_rc);
	int icz = (int) (m_iCorners[0].X / index);
	int icx = (int) (m_iCorners[0].Z / index);
	
	ii=1.0f - (float)(icx * m_pQuadTerrain->m_larg) * pa; 
	ii = ii - pa;
	jj=0.0f + (float)(icz * m_pQuadTerrain->m_larg) * pa;
	facteur = (float) m_pQuadTerrain->m_rk;
	j2 = 0.0f;
	i2 = 1.0f-pa2;

	// on a enfin les éléments pour constituer la geométrie
	int num=0;
	p=0;	// maintenant p sert d'index	
	for( i=0; i<m_pQuadTerrain->m_larg; i++)
	{                           
	 for( j=0; j<m_pQuadTerrain->m_larg; j++)
	 {		// triangle - P0
			float ju = j*ps;
			float iu = i*ps;
			if(flagNew)
			{ mbuffer->Vertices[p].Pos.X = origin.X+ju+ps;
			  mbuffer->Vertices[p].Pos.Z = origin.Z+iu;
			  mbuffer->Vertices[p].TCoords.X = ii+pa;
			  mbuffer->Vertices[p].TCoords.Y = jj+pa;
			  mbuffer->Vertices[p].TCoords2.X = i2+pa2;
			  mbuffer->Vertices[p].TCoords2.Y = j2+pa2;
			  mbuffer->Vertices[p].Color = DIFF; }
			mbuffer->Vertices[p].Pos.Y = m_pQuadTerrain->m_pHeightMap->GetHeightValue( mbuffer->Vertices[p].Pos.X/facteur,mbuffer->Vertices[p].Pos.Z/facteur );
			//v1 = mbuffer->Vertices[p].Pos;
			p++;
			// triangle - P1
			if(flagNew)
			{ mbuffer->Vertices[p].Pos.X = origin.X+ju;
			  mbuffer->Vertices[p].Pos.Z = origin.Z+iu;
			  mbuffer->Vertices[p].TCoords.X = ii+pa;
			  mbuffer->Vertices[p].TCoords.Y = jj;
			  mbuffer->Vertices[p].TCoords2.X = i2+pa2;
			  mbuffer->Vertices[p].TCoords2.Y = j2;
			  mbuffer->Vertices[p].Color = DIFF; }
			mbuffer->Vertices[p].Pos.Y = m_pQuadTerrain->m_pHeightMap->GetHeightValue( mbuffer->Vertices[p].Pos.X/facteur,mbuffer->Vertices[p].Pos.Z/facteur );
			v2 = mbuffer->Vertices[p].Pos;
			p++;
			// triangle - P2
			if(flagNew)
			{ mbuffer->Vertices[p].Pos.X = origin.X+ju;
			  mbuffer->Vertices[p].Pos.Z = origin.Z+iu+ps;
			  mbuffer->Vertices[p].TCoords.X = ii;
			  mbuffer->Vertices[p].TCoords.Y = jj;
			  mbuffer->Vertices[p].TCoords2.X = i2;
			  mbuffer->Vertices[p].TCoords2.Y = j2;
			  mbuffer->Vertices[p].Color = DIFF; }
			mbuffer->Vertices[p].Pos.Y = m_pQuadTerrain->m_pHeightMap->GetHeightValue( mbuffer->Vertices[p].Pos.X/facteur,mbuffer->Vertices[p].Pos.Z/facteur );
			v3 = mbuffer->Vertices[p].Pos;
			p++;
			// triangle	- P3
			if(flagNew)
			{ mbuffer->Vertices[p].Pos.X = origin.X+ju+ps;
			  mbuffer->Vertices[p].Pos.Z = origin.Z+iu+ps;
			  mbuffer->Vertices[p].TCoords.X = ii;
			  mbuffer->Vertices[p].TCoords.Y = jj+pa;
			  mbuffer->Vertices[p].TCoords2.X = i2;
			  mbuffer->Vertices[p].TCoords2.Y = j2+pa2;
			  mbuffer->Vertices[p].Color = DIFF;	}
			mbuffer->Vertices[p].Pos.Y = m_pQuadTerrain->m_pHeightMap->GetHeightValue( mbuffer->Vertices[p].Pos.X/facteur,mbuffer->Vertices[p].Pos.Z/facteur );
			v4 = mbuffer->Vertices[p].Pos;
			p++;

			jj = jj + pa;
			j2 = j2 + pa2;
			if (j2>0.99f) { j2=0.0f; i2=i2-pa2; }

			mbuffer->Indices[num] =   0 + p-4 ;	// 0
			mbuffer->Indices[num+1] = 1 + p-4 ;	// 1
			mbuffer->Indices[num+2] = 2 + p-4 ;	// 2
			mbuffer->Indices[num+3] = 0 + p-4 ;	// 0
			mbuffer->Indices[num+4] = 2 + p-4 ;	// 2
			mbuffer->Indices[num+5] = 3 + p-4 ;	// 3
			num += 6;

		}
		jj= (float)(icz * m_pQuadTerrain->m_larg) * pa;
		ii=ii-pa;
	}

	mbuffer->recalculateBoundingBox();
	Box = mbuffer->BoundingBox;
	// on fait recalculer les normal smooth par Irrlicht (pas très concluant pour le moment)
	scene::IMeshManipulator *manipulator = mgr->getMeshManipulator();
	manipulator->recalculateNormals( mbuffer, true);
	// récupération de la hauteur maxi du terrain
	Box.MinEdge.Y=0;
	if(Box.MaxEdge.Y > m_pQuadTerrain->max_h) m_pQuadTerrain->max_h = Box.MaxEdge.Y;
}
//-------------------------------------------------
// AffecteQuad()
//-------------------------------------------------
// Pour facilité la manipulation des éléments nodes 
// 3D une fois l'arbre créé, on les cole dans un tableau
// facilement consultable
void CQuadNode::AffecteQuad()
{	
  if( m_pChildren[0] != NULL )
  {	// c'est un noeud
	  if( m_pChildren[0] )
		  m_pChildren[0]->AffecteQuad();
	  if( m_pChildren[1] )
		  m_pChildren[1]->AffecteQuad();
	  if( m_pChildren[2] )
		  m_pChildren[2]->AffecteQuad();
	  if( m_pChildren[3] )
		  m_pChildren[3]->AffecteQuad();
  }  
  else
  {	// calcul	
	int x = (int) m_iCorners[1].X;
	int z = (int) m_iCorners[3].Z;
	int f = (int) m_pQuadTerrain->m_iSize / m_pQuadTerrain->m_rc;
	x = (x / f)-1;
	z = (z / f)-1;
	x = x + (z * m_pQuadTerrain->m_rc);
	m_pQuadTerrain->grille[x] = this;
  }
}
//-------------------------------------------------
// RenderQuad()
//-------------------------------------------------
// la pass de rendu 
void CQuadNode::RenderQuad( scene::ISceneManager* mgr , int FrustumCode)
{
  float rr=0;

  // parcours de l'arbre pour représenter  
  if( m_pParent == NULL )
  {
	  FrustumCode = 1; //FRUSTUM_INTERSECT; 
  } 
  else
  {
 	if( FrustumCode )	//FRUSTUM_ALLIN
	{
		FrustumCode = m_pQuadTerrain->_isVisible(Box);
		if( !FrustumCode  && m_pParent )		return; 
	}
  }

  if( m_pChildren[0] != NULL )
  {	// c'est un noeud
	  if( m_pChildren[0] )
	  		m_pChildren[0]->RenderQuad(mgr, FrustumCode );
	  if( m_pChildren[1] )
			m_pChildren[1]->RenderQuad(mgr, FrustumCode );
	  if( m_pChildren[2] )
			m_pChildren[2]->RenderQuad(mgr, FrustumCode );
	  if( m_pChildren[3] )
			m_pChildren[3]->RenderQuad(mgr, FrustumCode );
  }  
  else
  {		m_pQuadTerrain->nQuad++;
        //rendu  du mesh;
		video::IVideoDriver* driver = mgr->getVideoDriver();

		//DEBUG ZONE
		if(Debug)
		{
		    video::SMaterial mat;
			mat.AmbientColor = video::SColor(255,255,255,255);
		    mat.DiffuseColor = video::SColor(255,255,255,255);
		    mat.EmissiveColor = video::SColor(255,0,255,0);
		    driver->setMaterial(mat);
			driver->draw3DBox(Box, video::SColor(255,255,255,255));
		}
		driver->setMaterial(Material);
		driver->drawIndexedTriangleList(&mbuffer->Vertices[0],
										mbuffer->getVertexCount(),
										&mbuffer->Indices[0], 
										mbuffer->getIndexCount()/3 );
		

  }
}
//-------------------------------------------------
// getBoundingBox()
//-------------------------------------------------
// retourne le BoundingBox du quad
const core::aabbox3d<f32>& CQuadNode::getBoundingBox() const
{
	return Box;
}
//=========================================================
//---------------------------------------------------------
// Name: setMaterialTexture()
// Desc: travail sur la textures des Quad du Terrain
//	
//---------------------------------------------------------
void CQuadNode::setMaterialTexture(u32 textureLayer, video::ITexture* texture)
{
	if (textureLayer >= video::MATERIAL_MAX_TEXTURES)    return;
    Material.setTexture(textureLayer, texture);
}
//=========================================================
//---------------------------------------------------------
// Name: setMaterialType()
// Desc: travail sur le type de Material des Quad du Terrain
//	
//---------------------------------------------------------
void CQuadNode::setMaterialType(video::E_MATERIAL_TYPE newType)
{
    Material.MaterialType = newType;
}
//=========================================================
//---------------------------------------------------------
// Name: setMaterialFlag()
// Desc: travail sur les flag des materials des Quad du Terrain
//	
//---------------------------------------------------------
void CQuadNode::setMaterialFlag(video::E_MATERIAL_FLAG flag, bool newvalue)
{
	Material.setFlag(flag, newvalue);
}
//=========================================================
//---------------------------------------------------------
// Name: ReScale()
// Desc: recalcul de la géometrie d'un Quad
//	
//---------------------------------------------------------
void CQuadNode::ReScale(scene::ISceneManager*	mgr)
{ 
	video::S3DVertex2TCoords *v ;
	core::vector3df origin;
	core::vector3df v1,v2,v3,av,bv,Normal;
	short i,j;
	float ps,facteur;

	// coin de départ
	origin = m_iCorners[0];

	// pas entre les faces
	ps = abs(m_iCorners[0].X-m_iCorners[1].X)/ m_pQuadTerrain->m_larg;

	float index = (float)(m_pQuadTerrain->m_iSize/m_pQuadTerrain->m_rc);
	int icz = (int)(m_iCorners[0].X / index);
	int icx = (int)(m_iCorners[0].Z / index);
	facteur = (float) m_pQuadTerrain->m_rk;

	v = (video::S3DVertex2TCoords *) mbuffer->getVertices();

	u32 num=0;
	for( i=0; i<m_pQuadTerrain->m_larg; i++)
	{                           
	 for( j=0; j<m_pQuadTerrain->m_larg; j++)
	 {	// triangle 1	- P0
		float ju = j*ps;
		float iu = i*ps;
		float y;
		core::vector3df vp;

		vp = v[num].Pos;
		v[num].Pos.Y = m_pQuadTerrain->m_pHeightMap->GetHeightValue( vp.X/facteur,vp.Z/facteur );
		num++;
		vp = v[num].Pos;
		v[num].Pos.Y = m_pQuadTerrain->m_pHeightMap->GetHeightValue( vp.X/facteur,vp.Z/facteur );
		num++;
		vp = v[num].Pos;
		v[num].Pos.Y = m_pQuadTerrain->m_pHeightMap->GetHeightValue( vp.X/facteur,vp.Z/facteur );
		num++;
		vp = v[num].Pos;
		v[num].Pos.Y = m_pQuadTerrain->m_pHeightMap->GetHeightValue( vp.X/facteur,vp.Z/facteur );
		num++;
	 }
	}
	mbuffer->recalculateBoundingBox();
	Box = mbuffer->BoundingBox;
	// on fait recalculer les normal smooth par Irrlicht (pas très concluant pour le moment)
	scene::IMeshManipulator *manipulator = mgr->getMeshManipulator();
	manipulator->recalculateNormals( mbuffer, true);
	// récupération de la hauteur maxi du terrain
	Box.MinEdge.Y=0;
	if(Box.MaxEdge.Y > m_pQuadTerrain->max_h) m_pQuadTerrain->max_h = Box.MaxEdge.Y;
}
