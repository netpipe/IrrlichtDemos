typedef struct {
	float c[3];
	int TimeStamp;
} CacheVert;

typedef struct {
	int ScrEdgeIdx;
	int TimeStamp;
} CacheEdge;

typedef struct {
	int min_s, min_t;
	int size_s, size_t;
	int cache_offset;
} CacheFace;

typedef struct {
	int type;
	float normal[3];
	float dist;
	int children[2];
	int pad;
} BSPClipNode;

typedef struct {
	byte is_open;
} BSPPortal;

typedef struct {
	byte *raw[6];
	int cache_offsets[6];
	int width, height;
} BSPSky;

typedef struct {
	miptex_t *miptex;
	int cache_offsets[4];
} BSPTexture;


typedef struct {
	dmodel_t      *Models;
	dplane_t      *Planes;
	dnode_t       *Nodes;
	dleaf_t       *Leaves;
	dbrush_t      *Brushes;
	dbrushside_t  *BrushSides;
	darea_t	      *Areas;
	dareaportal_t *AreaPortals;
	dvertex_t     *Verts;
	dedge_t       *Edges;
	dface_t       *Faces;
	texinfo_t     *TexInfo;
	word          *TextureList;	// texture idx for each TexInfo
	dvis_t        *XVis;
	word          *BrushList;
	word          *FaceList;
	int           *EdgeList;
	byte          *LightMaps;
	char         **Entities;
	CacheVert     *CacheVerts;
	CacheEdge     *CacheEdges;
	CacheFace     *CacheFaces;
	BSPPortal     *Portals;
	BSPClipNode   *ClipNodes;
	BSPSky         Sky;
	BSPTexture     Textures[1024];	// all the textures for this bsp
	int            ModelObjectIndex[MAX_MAP_MODELS];
	int NumModels, NumPlanes, NumNodes, NumLeaves;
	int NumBrushes, NumBrushSides, NumAreas, NumAreaPortals;
	int NumFaces, NumEdges, NumVerts, NumTexInfos, NumTextures;
	int NumPortals, NumEntities;
	byte *Raw;
} BSP;


int  BSP_Load(BSP *,char *);
void BSP_Free(BSP *bsp);

int  BSP_FindLeaf(BSP *bsp,float *c);
int  BSP_FindCluster(BSP *bsp,float *c);


typedef struct {
	byte VisClusters[(MAX_MAP_LEAFS+7)>>3];
	byte VisAreas[MAX_MAP_AREAS];	// Should compress this?
} VisInfo;

void BSP_MakeVis(BSP *bsp,float *c,VisInfo *vis,int full_recalc);

