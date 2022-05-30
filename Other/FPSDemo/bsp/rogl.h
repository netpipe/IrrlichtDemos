//#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#define LMAP_SIDE			8
#define LMAP_SIDE2			3
#define LMAP_SIZE			(LMAP_SIDE<<4)
#define LMAP_SIZE2			(LMAP_SIDE2+4)
#define MAX_LMAP_BANKS			(100*65536/(LMAP_SIZE*LMAP_SIZE))
#define LMAP_FACES			(LMAP_SIDE*LMAP_SIDE)
#define LMAP_FACES2			(LMAP_SIDE2+LMAP_SIDE2)

extern byte *TexPalette;
extern Camera *ccam;
extern BSP *cbsp;

// Texture management
void InitGLTextures(void);
void Tex_PreCacheBSPTextures(void);
void Tex_SetBSPLightmapBank(int bank);
int  Tex_IsDynamicLightmap(int face_idx);
void Tex_BeginDynamicLighting(void);
void Tex_SetDynamicLightmap(int face_idx);
void Tex_SetBSPTexture(int tex_idx);
void Tex_SetSkyTexture(int idx);



