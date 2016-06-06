/***************************************************************
*                Terrain Engine issue of					   *
*							Dreamotion3D engine				   *
*															   *
**************************************************************** 
*File Created: 29.03.08                                        *
*File updated: 05.12.08                                        *
*Author:			                                           *
*Contens: Implementation of QuadTree   headers	               *
***************************************************************/
#ifndef _CQUADN_
#define _CQUADN_


#define TRIANGULATE_UP    1
#define TRIANGULATE_DOWN  2
#define TRIANGULATE_LEFT  4
#define TRIANGULATE_RIGHT 8

using namespace irr;
using namespace core;

// class externes
class CTerrainNode;


//-------------------------------------------------
// CQuadNode()
//-------------------------------------------------
// classe  
class CQuadNode	
{
private:

		CQuadNode*				m_pParent;			/*!< le parent de ce quad. */

		vector3df				d_mCorners;			/*!< coin de référence pour les calcul. */
		vector3df				m_cCenter;			/*!< centre du quad. */
		float					radius;				/*!< radius du Quad. */

public:
		CQuadNode*				m_pChildren[ 4 ];	/*!< 4 enfants par Quad. */
		video::SMaterial		Material;			/*!< material du quad. */
		bool					Debug;				/*!< flag pour affichage en mode débug. */
		core::aabbox3d<f32>		Box;				/*!< boundingBox du Quad. */
		vector3df				m_iCorners[ 4 ];	/*!< dimension des coins de ce quad. */
		CTerrainNode*			m_pQuadTerrain;		/*!< objet CTerrainNode propriétaire de ce Quad. */
		
		scene::SMeshBufferLightMap* mbuffer;		/*!< buffer graphique. */

	//---------------------------------------------------------
	// procedure et fonctions
	//---------------------------------------------------------
public:
	    ~CQuadNode();

	// constructeur().
    CQuadNode(CQuadNode *Parent, CTerrainNode *Terrain, scene::ISceneManager* mgr=NULL);

	// CreateQuadMesh().
	void CreateQuadMesh(scene::ISceneManager*	mgr);

	// Create3D_Quad().
	void Create3D_Quad(scene::ISceneManager*	mgr);

	// Subdivide().
	bool Subdivide();

	// Allocate().
	bool Allocate( vector3df Corners[4]);

	// BuildQuadTree().
	void BuildQuadTree(); 

	// CalculYCorner().
	void CalculYCorner();

	// SetBox().
	void SetBox();

	// AffecteQuad().
	void AffecteQuad();

	// SetMaxBox().
	void SetMaxBox();

	// RenderQuad
	void RenderQuad(scene::ISceneManager* mgr, int FrustumCode);

	// getBoundingBox
	const core::aabbox3d<f32>& getBoundingBox() const;

	//	setMaterialTexture
	void setMaterialTexture(u32 textureLayer, video::ITexture* texture);

	//	setMaterialType
	void setMaterialType(video::E_MATERIAL_TYPE newType);

	//	setMaterialFlag
	void setMaterialFlag(video::E_MATERIAL_FLAG flag, bool newvalue);

	// recalcul de la géometrie d'un Quad
	void ReScale(scene::ISceneManager*	mgr);

};

#endif
