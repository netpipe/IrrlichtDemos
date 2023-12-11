/***************************************************************
*                Terrain Engine issue of					   *
*							Dreamotion3D engine				   *
*															   *
**************************************************************** 
*File Created: 29.03.08                                        *
*File updated: 05.12.08                                        *
*Author:		TMyke                                          *
*Contens: Implementation of TerrainNode headers	               *
***************************************************************/
#ifndef _CTERRAIN_
#define _CTERRAIN_

#include "CList.h"
#include "CHeight.h"
#include "CQuad.h"

using namespace irr;

// class externes
class CQuadNode;

//-----------------------------------------------------------------------------
// Name: Plane class
// Desc: petite classe plane personnalisée pour pour ce codage.
//-----------------------------------------------------------------------------
class Plane
{
public:
	float a;
	float b;
	float c;
	float d;

	Plane():  a(0),b(0),c(0),d(0) {}
	Plane(float aa, float bb, float cc, float dd): a(aa), b(bb), c(cc), d(dd){}
	Plane operator-(const Plane& other) const { return Plane(a - other.a, b - other.b, c - other.c, d - other.d); }
	Plane operator+(const Plane& other) const { return Plane(a + other.a, b + other.b, c + other.c, d + other.d); }
	float operator[](unsigned i) const 	{ switch(i)	{case 0: return a; case 1: return b; case 2: return c; case 3: return d;default: return 0;	}	}
	void Normalize() { float m;	m = sqrt( a*a + b*b + c*c + d*d );	a = a / m;	b = b / m;	c = c/ m;	d = d /m;	}
	float DotCoord(float *V) {	return (a * V[0] + b * V[1] + c * V[2] + d);	}
};


//-------------------------------------------------
// CTerrainNode()
//-------------------------------------------------
// classe TerrainNode, dérivée de ISceneNode 
class CTerrainNode : public scene::ISceneNode
{
		scene::ISceneManager*   t_mgr;
		video::SMaterial	    Material;		/*!< material du terrain. */
		core::aabbox3d<f32>		Box;			/*!< BoundingBox du terrain global. */
		COLLIDE_INFO			global_pick;	/*!< structure de récupération des info de collision ou de picking. */

   public:

		CList<CQuadNode> DelQuad;		/*!< conteneur servant à la libération des ressources. */	
		CHeightMap      *m_pHeightMap;	/*!< pointeur vers l'objet heightmap. */

		int             m_iSize;		/*!< taille du terrain. */
		int             m_iSizeQuad;	/*!< taille de chaque quad (tile) du terrain (m_iSize / m_rc). */
		int				numSheet;		/*!< nombre de sheet. */
		int				m_larg;			/*!< largeur de chaque quad, en nombre de  faces. */
		int				m_rc;			/*!< taille du terrain en Quad x Quad  (m_rc * m_rc). */   
		int				m_rk;			/*!< m_iSize / HeightMap Width. */
		int				m_cote;			/*!< HeightMap / m_rc. */
		int				numQuad;		/*!< nombre total de quad définit. */
		float			max_h;			/*!< point haut maxi du terrain. */

		CQuadNode       *m_pQuadTree;	/*!< pointeur sur le quad originel. */
		CQuadNode		**grille;		/*!< table de tous les Quad 3D définis. */
		int				max_grille;		/*!< taille de la grille. */
		int				nQuad;			/*!< nombre de quad rendu par renderpass. */

		Plane			mFrustumPlane[6];	/*!< plan pour le frustum. */


	//************************************************
	//***************** methods
	//************************************************
	~CTerrainNode();

	// constructeur
	CTerrainNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id);

	// etabli la liste des quad à effacer
	void DeleteQuadList(CQuadNode *Pquad);

	// SetValues
	void SetValues(int numQuads=8, int numFaces=16, int sizeTerrain=2048);

	// LoadHeightMap
	bool LoadHeightMap( char *filename, float HScale );

	// Construct
	void Construct(char *filename, float scale);

	// OnRegisterSceneNode
	virtual void OnRegisterSceneNode();

	// render
	virtual void render();

	// getBoundingBox
	virtual const core::aabbox3d<f32>& getBoundingBox() const;

	//setMaterialTexture
	void setMaterialTexture(u32 textureLayer, video::ITexture* texture);

	//setMaterialType
	void setMaterialType(video::E_MATERIAL_TYPE newType);

	//setMaterialFlag
	void setMaterialFlag(video::E_MATERIAL_FLAG flag, bool newvalue);

	//GetTerrainHeight
	float GetTerrainHeight(float x, float z);

	// GetQuadNode
	CQuadNode* GetQuadNode(int num);
	CQuadNode* GetQuadNode(int x, int y);

	//Update Cull Info, attention  avec la camera courante.
	void UpdateCullInfo();

	// test de visibilité d'un mesh
	int CTerrainNode::_isVisible(const core::aabbox3d<f32>& box);

	// TextureTerrain
	void TextureTerrain(int nquad, int stage, int n,  video::ITexture* texture);

	// compute les vecteur d'origine et de direction en fonction des onfo souris/camera
	void CameraVectorComputePick(int x, int y, core::vector3df *vPickRayOrig, core::vector3df *vPickRayDir);

	// picking sur le terrain, procedure spécifiques
	bool  PickTerrain(int x, int y, int DistBase);

	// test de collision
	bool  Raycast(core::vector3df vPickRayDir, core::vector3df vPickRayOrig, int DistBase);

	// Test de collision avec un quad()
	bool QuadCollideTest(int num_quad, core::vector3df vPickRayDir, core::vector3df vPickRayOrig, int DistBase);

	// permet de récupérer les info de position suite au picking ou a un test de collision
	core::vector3df getPickedPosition();

	// permet de récupérer le quad 'pické' suite au picking ou a un test de collision
	int getPickedQuad();

	// permet de récupérer la normal du triangle 'pické' suite au picking ou a un test de collision
	core::vector3df getPickedNormal();

	// permet de récupérer le triangle 'pické' suite au picking ou a un test de collision
	core::triangle3df getPickedTriangle();

	// teste de triangle collision
	bool getCollisionPoint(const core::line3d<f32>& ray,
							scene::ITriangleSelector* selector, core::vector3df& outIntersection,
							core::triangle3df& outTriangle);

	// permet de changer la hauteur terrain (sur le heightmap) et de la répercuter sur la geométrie des quad concernés
	void SetHeightTerrain(int x, int y, float hh);


};


#endif