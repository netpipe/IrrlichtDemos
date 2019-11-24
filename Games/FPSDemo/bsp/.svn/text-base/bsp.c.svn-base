/********************************************************************
 BSP.C - BSP support routines
*********************************************************************/

#include "engine.h"

#define R_TEXTURES_POW2			1
#define R_TEXTURES_32BIT		2

int min (int i, int x){
    if (i > x){i=x;};
}

int max (int i, int x){
    if (i > x){i=x;};
}

// forward declarations
void MakeEntities(BSP *bsp, int pos, int size);
void LoadSky(BSP *bsp);
void LoadTextures(BSP *bsp);
void MakeCacheFace(BSP *bsp,int face_idx);
void MakeClipNodes(BSP *bsp);

/*
  Loads a Quake BSP file.
  Return: 1 - success, 0 - failure
*/
int BSP_Load(BSP *bsp,char *name)
{
	char fname[64];
	dheader_t *header;
	lump_t *entry;
	int i, size;

	strcpy(fname,"MAPS/");
	strcat(fname,name);

	bsp->Raw = (unsigned char*)DAT_ReadFile(fname,&size);
        if (!bsp->Raw)
        {
//            MessageBox(hWnd,"Failed to load BSP.","Error",MB_OK|MB_ICONERROR);
            return 0;
        }

// Make pointer to header and check the version
	header=(dheader_t *)bsp->Raw;
	if (header->version!=BSPVERSION)
        {
 //               MessageBox(hWnd,"Wrong version.","Error",MB_OK|MB_ICONERROR);
		BSP_Free(bsp);
		return 0;
	}

// Make verts pointer
	entry=&header->lumps[LUMP_VERTEXES];
	bsp->Verts=(dvertex_t *)(bsp->Raw+entry->fileofs);
	bsp->NumVerts=entry->filelen/sizeof(dvertex_t);
        //sprintf(numberofverts, "%d", bsp->NumVerts);

// Make edge pointer
	entry=&header->lumps[LUMP_EDGES];
	bsp->Edges=(dedge_t *)(bsp->Raw+entry->fileofs);
	bsp->NumEdges=entry->filelen/sizeof(dedge_t);

// Make face pointer
	entry=&header->lumps[LUMP_FACES];
	bsp->Faces=(dface_t *)(bsp->Raw+entry->fileofs);
	bsp->NumFaces=entry->filelen/sizeof(dface_t);

// Make model pointer
	entry=&header->lumps[LUMP_MODELS];
	bsp->Models=(dmodel_t *)(bsp->Raw+entry->fileofs);
	bsp->NumModels=entry->filelen/sizeof(dmodel_t);

// Make plane pointer
	entry=&header->lumps[LUMP_PLANES];
	bsp->Planes=(dplane_t *)(bsp->Raw+entry->fileofs);
	bsp->NumPlanes=entry->filelen/sizeof(dplane_t);

// Make node pointer
	entry=&header->lumps[LUMP_NODES];
	bsp->Nodes=(dnode_t *)(bsp->Raw+entry->fileofs);
	bsp->NumNodes=entry->filelen/sizeof(dnode_t);

// Make leaf pointer
	entry=&header->lumps[LUMP_LEAFS];
	bsp->Leaves=(dleaf_t *)(bsp->Raw+entry->fileofs);
	bsp->NumLeaves=entry->filelen/sizeof(dleaf_t);

// Make brush pointer
	entry=&header->lumps[LUMP_BRUSHES];
	bsp->Brushes=(dbrush_t *)(bsp->Raw+entry->fileofs);
	bsp->NumBrushes=entry->filelen/sizeof(dbrush_t);

// Make brush sides pointer
	entry=&header->lumps[LUMP_BRUSHSIDES];
	bsp->BrushSides=(dbrushside_t *)(bsp->Raw+entry->fileofs);
	bsp->NumBrushSides=entry->filelen/sizeof(dbrushside_t);

// Make areas pointer
	entry=&header->lumps[LUMP_AREAS];
	bsp->Areas=(darea_t *)(bsp->Raw+entry->fileofs);
	bsp->NumAreas=entry->filelen/sizeof(darea_t);

// Make area portals pointer
	entry=&header->lumps[LUMP_AREAPORTALS];
	bsp->AreaPortals=(dareaportal_t *)(bsp->Raw+entry->fileofs);
	bsp->NumAreaPortals=entry->filelen/sizeof(dareaportal_t);

// Make XVis pointer
	entry=&header->lumps[LUMP_VISIBILITY];
	bsp->XVis=(dvis_t *)(bsp->Raw+entry->fileofs);

// Make FaceList pointer
	entry=&header->lumps[LUMP_LEAFFACES];
	bsp->FaceList=(word *)(bsp->Raw+entry->fileofs);

// Make BrushList pointer
	entry=&header->lumps[LUMP_LEAFBRUSHES];
	bsp->BrushList=(word *)(bsp->Raw+entry->fileofs);

// Make EdgeList pointer
	entry=&header->lumps[LUMP_SURFEDGES];
	bsp->EdgeList=(int *)(bsp->Raw+entry->fileofs);

// Make TexInfo pointer
	entry=&header->lumps[LUMP_TEXINFO];
	bsp->TexInfo=(texinfo_t *)(bsp->Raw+entry->fileofs);
	bsp->NumTexInfos=entry->filelen/sizeof(texinfo_t);

// Make TextureList
	bsp->TextureList=(unsigned short*)malloc(bsp->NumTexInfos*sizeof(word));
	if (!bsp->TextureList) {
		BSP_Free(bsp);
		return 0;
	}

// Make light map's pointer
	entry=&header->lumps[LUMP_LIGHTING];
	bsp->LightMaps=(byte *)(bsp->Raw+entry->fileofs);

// Make entities
	entry=&header->lumps[LUMP_ENTITIES];
	MakeEntities(bsp,entry->fileofs,entry->filelen);

// Make portals
	bsp->NumPortals=0;
	for(i=0;i<bsp->NumAreaPortals;i++) {
		if (bsp->AreaPortals[i].portalnum>bsp->NumPortals)
			bsp->NumPortals=bsp->AreaPortals[i].portalnum;
	}
	bsp->NumPortals++;
	bsp->Portals=(BSPPortal*)malloc(bsp->NumPortals*sizeof(BSPPortal));
	if (!bsp->Portals) {
		BSP_Free(bsp);
		return 0;
	}
	for(i=0;i<bsp->NumPortals;i++)
		bsp->Portals[i].is_open=0;

// Make BSP CacheFace's
	bsp->CacheFaces=(CacheFace*)malloc(bsp->NumFaces*sizeof(CacheFace));
	if (!bsp->CacheFaces) {
		BSP_Free(bsp);
		return 0;
	}
	for(i=0;i<bsp->NumFaces;i++)
		MakeCacheFace(bsp,i);

// Make BSP CacheEdge's
	bsp->CacheEdges=(CacheEdge*)malloc(bsp->NumEdges*sizeof(CacheEdge));
	if (!bsp->CacheEdges) {
		BSP_Free(bsp);
		return 0;
	}
	for(i=0;i<bsp->NumEdges;i++)
		bsp->CacheEdges[i].TimeStamp=-1;

// Make BSP CacheVerts's
	bsp->CacheVerts=(CacheVert*)malloc(bsp->NumVerts*sizeof(CacheVert));
	if (!bsp->CacheVerts) {
		BSP_Free(bsp);
		return 0;
	}
	for(i=0;i<bsp->NumVerts;i++)
		bsp->CacheVerts[i].TimeStamp=-1;

// Load all the textures
	LoadTextures(bsp);

// Load sky textures
	LoadSky(bsp);

// Make clip nodes
	MakeClipNodes(bsp);

// That's it...
	return 1;
}

/*
  Free a BSP.
*/
void BSP_Free(BSP *bsp)
{
	int i;

// Free ClipNodes
	if (!bsp->ClipNodes) return;

	free(bsp->ClipNodes);
	bsp->ClipNodes=0;

// Free sky
	for(i=0;i<6;i++)
		free(bsp->Sky.raw[i]);

// Free textures
	for(i=0;i<bsp->NumTextures;i++)
		free(bsp->Textures[i].miptex);
	bsp->NumTextures=0;
	if (bsp->TextureList) free(bsp->TextureList);

// Free entities
	if (bsp->Entities) free(bsp->Entities);

// Free cache stuff
	if (bsp->CacheFaces) free(bsp->CacheFaces);
	if (bsp->CacheEdges) free(bsp->CacheEdges);
	if (bsp->CacheVerts) free(bsp->CacheVerts);

// Free raw stuff
	if (bsp->Raw) free(bsp->Raw);

}

/*
  Make pointers to entities
*/
void MakeEntities(BSP *bsp, int pos, int size)
{
	int i,num;
	byte *ptr;

// First we need to count entities (count '}')
	ptr=bsp->Raw+pos;
	num=0; i=size;
	while(i>0) {
		if (ptr[0]=='{') num++;
		i--; ptr++;
	}
	bsp->NumEntities=num;
	if (num==0) return;


// Allocate place for entities pointers
	bsp->Entities=(char **)malloc(num*sizeof(char *));

// Find pointers
	ptr=bsp->Raw+pos;
	for(i=0;i<num-1;i++) {
		bsp->Entities[i]=(char *)ptr;
		ptr++;
		while((*ptr)!='{') ptr++;
	}
	bsp->Entities[i]=(char *)ptr;
}


/*
  Load sky environment map
*/
void LoadSky(BSP *bsp)
{
	char app[6][4]={"ft","bk","lf","rt","up","dn"};
	char fname[64];
	byte *src;
	int i,size, tga_flag=1;//Render_GetFlags()&R_TEXTURES_32BIT;

	for(i=0;i<6;i++) {
		strcpy(fname,"env/unit1_");
		strcat(fname,app[i]);
		if (tga_flag) strcat(fname,".tga");
		else strcat(fname,".pcx");

		src = (unsigned char*)DAT_ReadFile(fname,&size);
		if (src) {

			if (tga_flag)
				bsp->Sky.raw[i]=TGA_Decode(src,&bsp->Sky.width,
					&bsp->Sky.height);
			else
				bsp->Sky.raw[i]=PCX_Decode(src,&bsp->Sky.width,
					&bsp->Sky.height,0);
			free(src);
		}
		bsp->Sky.cache_offsets[i]=0;
	}
}


/*
  Load all textures for this bsp
*/
void LoadTextures(BSP *bsp)
{
	char fname[64];
	BSPTexture *bt;
	texinfo_t *info;
	word *list;
	int i, j;

	bsp->NumTextures=0;
	info = bsp->TexInfo;
	list = bsp->TextureList;
	for(i=0;i<bsp->NumTexInfos;i++,info++,list++) {

	// Search for this texture
		for(j=0;j<bsp->NumTextures;j++) {
		   //! if (stricmp(info->texture,bsp->Textures[j].miptex->name)==0)
			if (strcasecmp(info->texture,bsp->Textures[j].miptex->name)==0)
				break;
		}

	// Texture already loaded
		if (j<bsp->NumTextures) {
			*list=(unsigned short)j;
			continue;
		}

	// Nope...have to load the texture
		bt=&bsp->Textures[bsp->NumTextures];
		strcpy(fname,"textures/");
		strcat(fname,info->texture);
		strcat(fname,".wal");
		bt->miptex=(miptex_t *)DAT_ReadFile(fname,&j);

		if (bt->miptex) {
			strcpy(bt->miptex->name,info->texture);
			*list=(unsigned short)bsp->NumTextures;
			bsp->NumTextures++;
		}
		else {
//!                    MessageBox(hWnd,"bt->miptex failed.","Message",MB_OK|MB_ICONERROR);
			*list=0;
		}
		for(j=0;j<4;j++)
			bt->cache_offsets[j]=-1;
	}
}

/*
  Find extents of a face.
*/
void MakeCacheFace(BSP *bsp,int face_idx)
{
	dface_t *face;
	dplane_t *plane;
	texinfo_t *texinfo;
	dedge_t *edge;
	dvertex_t *v;
	CacheFace *info;
	float s, t;
	float min_s, max_s, min_t, max_t;
	int i,j, temp1, temp2;

	face=&bsp->Faces[face_idx];
	info=&bsp->CacheFaces[face_idx];
	plane=&bsp->Planes[face->planenum];
	texinfo=&bsp->TexInfo[face->texinfo];


// Find extents in tex space

/*
  Go thru all verts in the face computing tex space coords
*/
	min_s=min_t=100000;
	max_s=max_t=-100000;

	for(j=0;j<face->numedges;j++) {
		i=face->firstedge+j;
		i=bsp->EdgeList[i];
		if (i<0) i=-i;
		edge=&bsp->Edges[i];

	// Tex coords for first vert of the edge
		v=&bsp->Verts[edge->v[0]];
		s=FVec_DotProduct(v->point,texinfo->vecs[0])+texinfo->vecs[0][3];
		t=FVec_DotProduct(v->point,texinfo->vecs[1])+texinfo->vecs[1][3];
		min_s=min(s,min_s); max_s=max(s,max_s);
		min_t=min(t,min_t); max_t=max(t,max_t);

	// Tex coords for second vert of the edge
		v=&bsp->Verts[edge->v[1]];
		s=FVec_DotProduct(v->point,texinfo->vecs[0])+texinfo->vecs[0][3];
		t=FVec_DotProduct(v->point,texinfo->vecs[1])+texinfo->vecs[1][3];
		min_s=min(s,min_s); max_s=max(s,max_s);
		min_t=min(t,min_t); max_t=max(t,max_t);
	}

// Compute S extents
	temp1=(int)floor(min_s/16.0f);
	temp2=(int)ceil(max_s/16.0f);

	info->min_s=temp1<<4;
	info->size_s=(temp2-temp1)<<4;

// Compute T extents
	temp1=(int)floor(min_t/16.0f);
	temp2=(int)ceil(max_t/16.0f);

	info->min_t=temp1<<4;
	info->size_t=(temp2-temp1)<<4;


	info->cache_offset=0;

	if (!texinfo->flags) {
		if (info->size_s>240) info->size_s=240;
		if (info->size_t>240) info->size_t=240;
	}
}



void MakeClipNodes(BSP *bsp)
{
	BSPClipNode *clip;
	dnode_t *node;
	dplane_t *plane;
	dleaf_t *leaf;
	int i, j, idx;

	bsp->ClipNodes=(BSPClipNode*)malloc(bsp->NumNodes*sizeof(BSPClipNode));

	clip=bsp->ClipNodes;
	node=bsp->Nodes;

	for(i=0;i<bsp->NumNodes;i++,clip++,node++) {
		plane=&bsp->Planes[node->planenum];

		clip->type=plane->type;
		clip->dist=plane->dist;
		FVec_Copy(plane->normal,clip->normal);

		for(j=0;j<2;j++) {
			idx = node->children[j];
			if (idx<0) {
				leaf=&bsp->Leaves[-(idx+1)];
				if (leaf->contents&CONTENTS_SOLID)
					idx=-1;
				else
					idx=-2;
			}
			clip->children[j]=idx;
		}
	}
}





/*
  Find a bsp leaf for given coords

int BSP_FindLeaf(BSP *bsp, float *p)
{
	dnode_t  *node;
	dplane_t *plane;
	int idx;
	float dot;


	idx=bsp->Models[0].headnode;

	while(idx>=0) {
		node=&bsp->Nodes[idx];
		plane=&bsp->Planes[node->planenum];
		dot=FVec_DotProduct(plane->normal,p)-plane->dist;
		if (dot>=0) idx=node->children[0];
		else idx=node->children[1];
	}

	return(-(idx+1));
}
*/



/********************************************************************
			VISIBILITY STUFF
*********************************************************************/
/*
static BSP *vis_bsp;
static byte *vis_dest;


void vis_areas(int area_idx)
{
	dareaportal_t *ap;
	darea_t *area;
	int i;

// Mark this area as visible
	vis_dest[area_idx]=1;

// Walk thru all portals of this area
	area=&vis_bsp->Areas[area_idx];

	ap=&vis_bsp->AreaPortals[area->firstareaportal];
	for(i=0;i<area->numareaportals;i++,ap++) {
		if (vis_bsp->Portals[ap->portalnum].is_open) {
			if (vis_dest[ap->otherarea]==0)
				vis_areas(ap->otherarea);
		}
	}
}



void BSP_MakeVis(BSP *bsp,float *c,VisInfo *vis,int full_recalc)
{
	dleaf_t *leaf;
	int leaf_idx,num;
	byte *src, *dest, a;


	leaf_idx=BSP_FindLeaf(bsp,c);

	leaf=&bsp->Leaves[leaf_idx];

	if (leaf->cluster<0) return;

	if (full_recalc) {

	// Uncompress list of vis clusters
		num=(bsp->XVis->numclusters+7)>>3;
		src=(byte *)bsp->XVis+bsp->XVis->bitofs[leaf->cluster][DVIS_PVS];
		dest=vis->VisClusters;

		while(num>0) {
			a=src[0];
			if (a) {
				dest[0]=a;
				src++; dest++; num--;
			}
			else {
				a=src[1];
				if (num<a) memset(dest,0,num);
				else memset(dest,0,a);
				num-=a;
				dest+=a;
				src+=2;
			}
		}
	}


// Area portals
	if (leaf->area<0) {
	// All areas visible
		memset(vis->VisAreas,1,bsp->NumAreas);
		return;
	}

// Hide all areas
	vis_dest=vis->VisAreas;
	vis_bsp=bsp;
	memset(vis_dest,0,bsp->NumAreas);

// Do recursive flood-fill
	vis_areas(leaf->area);
}
*/
