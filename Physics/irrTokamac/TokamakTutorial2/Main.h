
#ifndef MAIN_H
#define MAIN_H


#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <D3DX8.h>


// Allow other modules to see the following global variables
extern IDirect3DDevice8 *gD3DDevice;




// Vertex structure
struct strVertex
{
    FLOAT fX, fY, fZ;   // The untransformed position for the vertex.
    FLOAT nx,ny,nz;
    DWORD dwColour;
    FLOAT tu, tv;    // The texture coordinates

	void SetPoint(float x, float y, float z)
	{		fX = x; fY = y; fZ = z;		}

	void SetNormal(float x, float y, float z)
	{		nx = x; ny = y; nz = z;		}

	void SetTexCoords(float u, float v)
	{		tu = u; tv = v;		}

};
#define D3D8T_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_NORMAL|D3DFVF_DIFFUSE)


#endif


