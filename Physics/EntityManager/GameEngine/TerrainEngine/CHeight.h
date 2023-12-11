/***************************************************************
*                Terrain Engine issue of					   *
*							Dreamotion3D engine				   *
*															   *
**************************************************************** 
*File Created: 29.03.08                                        *
*Author:			                                           *
*Contens: Implementation of HeightMap headers	               *
***************************************************************/
#ifndef _CHEIGHT_
#define _CHEIGHT_

using namespace irr;

/**
* CHeightMap class .
* Class to manage the height map for terrain construct.
* @param none.
* @return none.
*/
class CHeightMap
{
  private:
		void SetBorders( float fHeight );
		float* LoadTexture32(char *filename, scene::ISceneManager* mgr);
		void GetMinMax();
		core::vector3df GetVertexNormal( float fX, float fY, float fSizeScale );
		float* ScaleHeightmap_( float *Src, int SrcWidth, int SrcHeight, int DstWidth, int DstHeight );

  public:

	   float	*m_pHeightMap;			/*!< tableau global des hauteurs terrain. */	
	   int		m_iWidth;				/*!< largeur du heightMap. */
	   int		m_iHeight;				/*!< hauteur du heightMap. */
	   float	m_fMinHeight;			/*!< Min value du Height. */
	   float	m_fMaxHeight;			/*!< Max value du Height. */


		// ------------------------------
		//	Constructeurs - Destructeurs
		// ------------------------------/
	   ~CHeightMap();

	/**
	* constructor().
	*/
    CHeightMap();

	/**
	* constructor().
	*/
	CHeightMap( int Width, int Height );

	/**
	* Destroy().
	*/
	void Destroy();

	/**
	* Create().
	*/
	void Create( int Width, int Height );

	/**
	* LoadHeightMap().
	*/
	bool LoadHeightMap(scene::ISceneManager* mgr, char *filename, float HScale );

	/**
	* Scale().
	*/
	void Scale( float fScale );

	/**
	* SmoothMap().
	*/
	void SmoothMap();

	/**
	* GetHeightValue().
	*/
	float GetHeightValue( int iX, int iY );

	/**
	* GetHeightValue().
	*/
	float GetHeightValue( float fX, float fY );

	/**
	* GetInterpolateHeight().
	*/
	float GetInterpolateHeight( float fX, float fY );
};


#endif