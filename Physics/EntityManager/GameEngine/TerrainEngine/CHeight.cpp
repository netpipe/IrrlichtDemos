/***************************************************************
*                Terrain Engine issue of					   *
*							Dreamotion3D engine				   *
*															   *
**************************************************************** 
*File Created: 29.03.08                                        *
*Author:			                                           *
*Contens: Implementation of HeightMap algorithme               *
***************************************************************/
#include <irrlicht.h>
#include <iostream>
#include <stdio.h>

#include "CHeight.h"
#pragma warning( disable : 4996 ) // disable deprecated warning 


using namespace irr;

//******************************
// Variables globales 
//******************************
int			heightT,widthT;
#define		TMapHauteur(x, y) (m_pHeightMap[x+y*m_NbPointX])
#define		TMapHauteurLisser(x, y) (pMapHauteurLisser[x+y*m_NbPointX])


//=========================================================
//---------------------------------------------------------
// Name: constructor()
// 
//---------------------------------------------------------
CHeightMap::CHeightMap()
{
	m_pHeightMap = NULL;
	m_iWidth     = 0;
	m_iHeight    = 0;
	m_fMinHeight = 0.0f;
    m_fMaxHeight = 0.0f;
}
CHeightMap::CHeightMap( int Width, int Height )
{
	m_iWidth  = Width;
	m_iHeight = Height;
	m_pHeightMap = new float[ Width * Height ];
	m_fMinHeight = 0.0f;
    m_fMaxHeight = 0.0f;
}
CHeightMap::~CHeightMap()
{
}
//=============================================================================
//-----------------------------------------------------------------------------
// Name: Create()
// 
//---------------------------------------------------------
void CHeightMap::Create( int Width, int Height )
{
	m_iWidth  = Width;
	m_iHeight = Height;
	m_pHeightMap = new float[ Width * Height ];
	m_fMinHeight = 0.0f;
    m_fMaxHeight = 0.0f;
}
//=============================================================================
//-----------------------------------------------------------------------------
// Name: Destroy()
// 
//---------------------------------------------------------
void CHeightMap::Destroy()
{
	delete  m_pHeightMap;
	m_iWidth = m_iHeight = 0;
}
//=============================================================================
//-----------------------------------------------------------------------------
// Name: Scale()
// 
//---------------------------------------------------------
void CHeightMap::Scale( float fScale )
{
	float *hbuf;
	int x,y;

   if( m_pHeightMap == NULL || ( m_iWidth <= 0 || m_iHeight <= 0 ))
	   return;

	hbuf = m_pHeightMap;

	for( x = 0; x < m_iWidth; x++ )
	{
		for( y = 0; y < m_iHeight; y++ )
		{
			   hbuf[ y * m_iWidth + x ] *= fScale;
		}
	}
	   GetMinMax();
}
//=============================================================================
//-----------------------------------------------------------------------------
// Name: ScaleHeightmap_()
// changement d'echelle de la surface HeightMap
//---------------------------------------------------------------------------------------
float* CHeightMap::ScaleHeightmap_( float *Src, int SrcWidth, int SrcHeight, 
					               int DstWidth, int DstHeight )
{
	float x_step, y_step;
	float *Dst;

	Dst = new float[ DstWidth * DstHeight ];

	x_step = (float)SrcWidth  / (float)DstWidth;
	y_step = (float)SrcHeight / (float)DstHeight;

	float fXT, fYT, fXPos, fYPos;
	int x,y, iX, iY;
	iX = iY = 0;
	fXT = fYT = 0.0f;
	fXPos = fYPos = 0.0f;

	for( y = 0; y < DstHeight; y++ )
	{
		fXPos = 0.0f;
	    for(x = 0; x < DstWidth; x++)
		{
			Dst[ y * DstWidth + x ] = Src[ iY * SrcWidth + iX ];

			(float)fXPos += (float)x_step;
			(float)fXT    = (float)fXPos;
			(int)iX       = (int)fXT;
		}
		(float)fYPos += (float)y_step;
		(float)fYT    = (float)fYPos;
		(int)iY       = (int)fYT;
	}
   return (float*)Dst;
}
//=============================================================================
//-----------------------------------------------------------------------------
// Name: LoadHeightMap()
// charge le fichier image représentative du HeightMap
//---------------------------------------------------------------------------------------
bool CHeightMap::LoadHeightMap(scene::ISceneManager* mgr, char *fname, float HScale )
{
  //char *Filename;
  float *buffer;
  int x,y, width, height;
  float r;
  float *fHeightMap;

  buffer = LoadTexture32(fname, mgr);
  width  = widthT; 
  height = heightT;
  m_iWidth   = width;
  m_iHeight  = height;


	fHeightMap = new float[ width * height ];
	memset( fHeightMap, 0,  sizeof(float) * width * height );

    for(x = 0; x < width; x++)
	    for( y = 0; y < height; y++)
		{
			r = buffer[ (y * width + x) ];
          fHeightMap[  y * width + x ] = (r)*HScale; 
		}

	delete  buffer ;
    if( width != m_iWidth || height != m_iHeight )
	{
		delete m_pHeightMap;
	   m_pHeightMap = ScaleHeightmap_( fHeightMap, width, height, m_iWidth, m_iHeight );
	   delete  fHeightMap;	   
	   GetMinMax();
	   return true;
	} else 
	{
		delete m_pHeightMap;
		m_pHeightMap = fHeightMap;
	    GetMinMax();
		return true;
	}
}
//=============================================================================
//-----------------------------------------------------------------------------
// Name: GetMinMax()
// recupère les deux sommet (positif et négatif)
//---------------------------------------------------------------------------------------
void CHeightMap::GetMinMax()
{
  float *hbuf = m_pHeightMap;
  int x,y;
  float miny, maxy, h;

  miny = hbuf[ 0 ];
  maxy = hbuf[ 0 ];
	for( x = 0; x < m_iWidth; x++ )
	{
		for( y = 0; y < m_iHeight; y++ )
		{
			   h = hbuf[ y * m_iWidth + x ];
			   if( h < miny ) miny = h;
			   if( h > maxy ) maxy = h;
		}
	}
  m_fMinHeight = (float) miny;
  m_fMaxHeight = (float) maxy;
}
//=============================================================================
//-----------------------------------------------------------------------------
// Name: SmoothMap()
// On va lisser le terrain en hauteur pour evite les effets d'escalier à cause des valeurs entière
//-----------------------------------------------------------------------------------------------------------
void CHeightMap::SmoothMap()
{
	int m_NbPointX=m_iWidth;
	int m_NbPointY=m_iHeight;
	float* pMapHauteurLisser = new float[m_NbPointX * m_NbPointY];

	// Lissage de l'image sans les bordures
	for(int j=1; j<m_NbPointY-1; j++)
	{
		for(int i=1; i<m_NbPointX-1; i++)
		{
			TMapHauteurLisser(i, j) =  (TMapHauteur(i    ,     j)
									+	TMapHauteur(i    , (j-1))
									+	TMapHauteur(i    , (j+1))
									+	TMapHauteur((i-1),     j)
									+	TMapHauteur((i+1),     j)
									+	TMapHauteur((i-1), (j-1))
									+	TMapHauteur((i+1), (j+1))
									+	TMapHauteur((i+1), (j-1))
									+	TMapHauteur((i-1), (j+1))) / 9.0f;
		}
	}
	// Lissage bordure gauche
	for(int j=1; j<m_NbPointY-1; j++)
	{
		TMapHauteurLisser(0, j) =  (TMapHauteur(0,     j)
								+   TMapHauteur(0, (j-1))
								+   TMapHauteur(0, (j+1))
								+   TMapHauteur(1,     j)
								+   TMapHauteur(1, (j-1))
								+   TMapHauteur(1, (j+1))) / 6.0f;
	}
	// Lissage bordure droite
	for(int j=1; j<m_NbPointY-1; j++)
	{
		TMapHauteurLisser((m_NbPointX-1), j) =  (TMapHauteur((m_NbPointX-1),     j)
											 +   TMapHauteur((m_NbPointX-1), (j-1))
								             +   TMapHauteur((m_NbPointX-1), (j+1))
								             +   TMapHauteur((m_NbPointX-2),     j)
								             +   TMapHauteur((m_NbPointX-2), (j-1))
								             +   TMapHauteur((m_NbPointX-2), (j+1))) / 6.0f;
	}
	// Lissage bordure haut
	for(int i=1; i<m_NbPointX-1; i++)
	{
		TMapHauteurLisser(i, 0) =  (TMapHauteur(i    , 0)
								+   TMapHauteur((i+1), 0)
								+   TMapHauteur((i-1), 0)
								+   TMapHauteur(i    , 1)
								+   TMapHauteur((i+1), 1)
								+   TMapHauteur((i-1), 1)) / 6.0f;
	}
	// Lissage bordure bas
	for(int i=1; i<m_NbPointX-1; i++)
	{
		TMapHauteurLisser(i, (m_NbPointY-1)) =  (TMapHauteur(i    , (m_NbPointY-1))
								             +   TMapHauteur((i+1), (m_NbPointY-1))
								             +   TMapHauteur((i-1), (m_NbPointY-1))
								             +   TMapHauteur(i    , (m_NbPointY-2))
								             +   TMapHauteur((i+1), (m_NbPointY-2))
								             +   TMapHauteur((i-1), (m_NbPointY-2))) / 6.0f;
	}
	// Lissage des angles de l'image
	TMapHauteurLisser(0, 0) = (TMapHauteur(0, 0) + TMapHauteur(0, 1) + TMapHauteur(1, 0) + TMapHauteur(1, 1)) / 4.0f;
	TMapHauteurLisser((m_NbPointX-1), 0) = (TMapHauteur((m_NbPointX-1), 0) + TMapHauteur((m_NbPointX-1), 1) + TMapHauteur((m_NbPointX-2), 0) + TMapHauteur((m_NbPointX-2), 1)) / 4.0f;
	TMapHauteurLisser(0, (m_NbPointY-1)) = (TMapHauteur(0, (m_NbPointY-1)) + TMapHauteur(1, (m_NbPointY-1)) + TMapHauteur(0, (m_NbPointY-2)) + TMapHauteur(1, (m_NbPointY-2))) / 4.0f;
	TMapHauteurLisser((m_NbPointX-1), (m_NbPointY-1)) = (TMapHauteur((m_NbPointX-1), (m_NbPointY-1)) + TMapHauteur((m_NbPointX-2), (m_NbPointY-1)) + TMapHauteur((m_NbPointX-1), (m_NbPointY-2)) + TMapHauteur((m_NbPointX-2), (m_NbPointY-2))) / 4.0f;

	// On libère l'ancien tableau et on le remplace par le nouveau
	delete[] m_pHeightMap;
	m_pHeightMap = pMapHauteurLisser;
}
//=============================================================================
//-----------------------------------------------------------------------------
// Name: SetBorders()
// met en place des bord, plus jolie au niveau du rendu
//---------------------------------------------------------------------------------------
void CHeightMap::SetBorders( float fHeight )
{
	float *hbuf;
	int x,y;

   if( m_pHeightMap == NULL || ( m_iWidth <= 0 || m_iHeight <= 0 ))
	   return;

	hbuf = m_pHeightMap;

	for( x = 0; x < m_iWidth; x++ )
	{
		for( y = 0; y < m_iHeight; y++ )
		{
		   if( x == 0 || x == (m_iWidth-1) || y == 0 || y == (m_iHeight-1) )
			   hbuf[ y * m_iWidth + x ] = fHeight;
		}
	}
	GetMinMax();
}
//=============================================================================
//-----------------------------------------------------------------------------
// Name: GetHeightValue()
// récupère une hauteur terrain en fonction de coordonnées X et Y
//---------------------------------------------------------------------------------------
float CHeightMap::GetHeightValue( int iX, int iY )
{
	if( iX >= m_iWidth ) 
		iX = m_iWidth-1;
	if( iY >= m_iHeight )
		iY = m_iHeight-1;
	return m_pHeightMap[ iY * m_iWidth + iX ];
}
//=============================================================================
//-----------------------------------------------------------------------------
// Name: GetHeightValue()
// récupère une hauteur terrain en fonction de coordonnées X et Y
//---------------------------------------------------------------------------------------
float CHeightMap::GetHeightValue( float fX, float fY )
{
	int iX = (int)fX;
	int iY = (int)fY;
	if( iX >= m_iWidth ) 
		iX = m_iWidth-1;
	if( iY >= m_iHeight )
		iY = m_iHeight-1;
	return (float) m_pHeightMap[ iY * m_iWidth + iX ];
}
//=============================================================================
//-----------------------------------------------------------------------------
// Name: GetInterpolateHeight()
// récupère une hauteur terrain en fonction de coordonnées X et Y avec interpolation
// (non tributaire de l'echelle du terrain)
//---------------------------------------------------------------------------------------
float CHeightMap::GetInterpolateHeight( float fX, float fY )
{
    int iX, iY;
	float dX, dY;


    iX = (int)fX;
	iY = (int)fY;
    dX = fX - (float)iX;
	dY = fY - (float)iY;

	if( (iX >(m_iWidth-1)) || (iY >(m_iHeight-1)) ) 		return GetHeightValue( m_iWidth-1, m_iHeight-1 );
	if( (iX<0) || (iY<0) ) return GetHeightValue( 0, 0 );

	float hY, hY2, Height;
	int   idY=1;
	int   idX=1;

	if( iX==(m_iWidth-1)) idX=0;
	if( iY==(m_iHeight-1)) idY=0;

	hY =  m_pHeightMap[ iY * m_iWidth + iX ] + 
	( m_pHeightMap[ (iY+idY) * m_iWidth + iX ] - m_pHeightMap[ iY * m_iWidth + iX ] ) * dY;


	hY2= m_pHeightMap[ iY * m_iWidth + iX + idX ] + 
	( m_pHeightMap[ (iY+idY) * m_iWidth + iX + idX ] - m_pHeightMap[ iY * m_iWidth + iX + idX ] ) * dY;


	Height = hY + ( hY2 - hY ) * dX;
	return Height;
}
//=============================================================================
//-----------------------------------------------------------------------------
// Name: GetVertexNormal()
// 
//---------------------------------------------------------------------------------------
core::vector3df CHeightMap::GetVertexNormal( float fX, float fY, float fSizeScale )
{
   core::vector3df vNorm = core::vector3df( 0,0,0 );

   float iX, iY;
   iX = fX;
   iY = fY;

   if( (iX > 0) && (iX < ( m_iWidth - 1 )))
   {
	   vNorm.X = GetInterpolateHeight( iX - 1, iY ) - GetInterpolateHeight( iX + 1, iY );
   } 
   else if( iX > 0 )
   {
	   vNorm.X = 2.0f * GetInterpolateHeight( iX - 1, iY ) - GetInterpolateHeight( iX, iY );
   } 
   else
   {
	   vNorm.X = 2.0f * GetInterpolateHeight( iX, iY ) - GetInterpolateHeight( iX + 1, iY );
   }

   if( (iY > 0) && (iY < ( m_iHeight - 1 )))
   {
	   vNorm.Z = GetInterpolateHeight( iX, iY - 1 ) - GetInterpolateHeight( iX, iY + 1 );
   } 
   else if( iY > 0 )
   {
	   vNorm.Z = 2.0f * GetInterpolateHeight( iX, iY - 1 ) - GetInterpolateHeight( iX, iY );
   } 
   else
   {
	   vNorm.Z = 2.0f * GetInterpolateHeight( iX, iY ) - GetInterpolateHeight( iX, iY + 1 );
   }

   vNorm.Y = 2.0f * fSizeScale;

   vNorm.normalize();
	vNorm= -vNorm;

   return vNorm;
}
//=============================================================================
//-----------------------------------------------------------------------------
// Name: LoadTexture()
// methode pour le chargement du fichier image représentant les hauteurs
//---------------------------------------------------------------------------------------
float* CHeightMap::LoadTexture32(char *filename, scene::ISceneManager* mgr)
{	
	float *img;

  //Get the Heightmap
	video::IImage *heightmap = mgr->getVideoDriver()->createImageFromFile(filename);

  //Get dimension of heightmap
	widthT = heightmap->getDimension().Width;
  heightT = heightmap->getDimension().Height;

	img = new float[widthT * heightT];

	for(int u=0;u<heightT;u++) 
	{ // pour chaque ligne de la texture
		for(int v=0;v<widthT;v++) 
		{  // pour chaque pixel de la ligne
			video::SColor pixelcolor(heightmap->getPixel(v,u));
			//vertex.pos.y = (f32) pixelcolor.getluminance()/10.0f;

			float color = pixelcolor.getLuminance();
			img[v+u*heightT] =  color;
		}
	}

/*
	LPDIRECT3DTEXTURE9 tTexture  = NULL;
    D3DSURFACE_DESC pDesc;

	// charge une texture représentative des hauteurs
	if(FAILED(D3DXCreateTextureFromFile( curent_App->d3ddevice, filename, &tTexture)))
	{
		char nn[128];
		sprintf( nn, "Load texture failed: %s  \r\n", filename);
		SENDLOG(log_engine, " <<ERROR>> CHeightMap(LoadTexture32)  \r\n", true);
		SENDLOG(log_engine, nn, false);
	}

	// récupère les dimensions
	tTexture->GetLevelDesc(0,&pDesc);
	widthT = pDesc.Width;
	heightT = pDesc.Height;
	img = new float[widthT * heightT];
	// dévérouille pour l'accès direct aux pixels

	D3DLOCKED_RECT  rect;
	BYTE			*ptr;
	UINT			d;
	tTexture->LockRect(0,&rect,NULL,0);
	ptr = (BYTE *)rect.pBits;
	d = rect.Pitch - heightT * sizeof(D3DCOLOR);  // pitch : décalage supplémentaire en octets
	for(int u=0;u<heightT;u++) { // pour chaque ligne de la texture
		for(int v=0;v<widthT;v++) 
		{  // pour chaque pixel de la ligne
			DWORD color;
			memcpy(&color, ptr, sizeof(D3DCOLOR));
			img[v+u*heightT] = (float) color;
			ptr += sizeof(D3DCOLOR);
		}
		ptr += d;
	}
	tTexture->UnlockRect(0);
*/
	//------------------------------------
	// rotion du terrain de 90°
	float *img2;
	img2 = new float[widthT * heightT];
	for(int y=0; y<heightT; y++)
	{
		for(int x=0; x<widthT; x++)
		{
			int xo = (widthT * x + widthT)-1-y;
			img2[x+y*heightT] = img[xo];
		}
	}
	delete  []img;

	return img2;
}



