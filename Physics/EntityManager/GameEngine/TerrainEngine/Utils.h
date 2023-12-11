/***************************************************************
*                Terrain Engine issue of					   *
*							Dreamotion3D engine				   *
*		UTILS FUNCTIONS	HEADER								   *
**************************************************************** 
*File Created: 29.03.08                                        *
*Author:			                                           *
*Contens: utilities functions header				           *
***************************************************************/
#ifndef _CUTILS_
#define _CUTILS_

using namespace irr;
using namespace core;

typedef struct COLLIDE_INFO
{ 
	int					quad;			// numéro du quad trouvé
	float				distance;		// les distance
	int					nFace;			// numéro de face traversé
	core::triangle3df	tri;			// triangle traversé.
	core::vector3df		position;		// position globale d'intersection
	core::vector3df		normal;			// normal par rapport à la face
} COLLIDE_INFO;


float	Distance3(vector3df* p1, vector3df *p2);
void	Direction3(vector3df* p1, vector3df* p2, vector3df* rp);
void	xform(const core::matrix4& M, core::aabbox3d<f32>& out);

#endif