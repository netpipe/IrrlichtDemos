typedef struct {
	char name[64];
	byte *pic;
	byte pal[768];
	int is_rgba;
	int width, height;
} MDLSkin;

typedef struct {
	char           Name[64];
	dmdl_t        *Header;			// Q2 data Header 
	MDLSkin        Skins[MAX_MD2SKINS];	// Skins
	dstvert_t     *TexVerts;		// Texture verts
	dtriangle_t   *Tris;			// Triangles
	daliasframe_t *Frames[MAX_FRAMES];	// Pointers to frames
	byte          *glCommands;
	int NumSkins,NumVerts,NumTexVerts,NumTris,NumFrames,NumGLCommands;
	int cache_offset;
	byte *Raw;
} MDL;


int  MDL_Load(MDL *mdl, char *fname);
int  MDL_LoadSkin(MDL *mdl, char *fname);
void MDL_Free(MDL *mdl);
int  MDL_FirstFrame(MDL *mdl,char *name);
int  MDL_LastFrame(MDL *mdl,char *name);

