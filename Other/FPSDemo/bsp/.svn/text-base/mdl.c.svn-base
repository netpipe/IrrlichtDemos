/********************************************************************f
 MDL.C - Model support routines
*********************************************************************/

#include "engine.h"

void FixTexCoords(MDL *mdl);


int MDL_Load(MDL *mdl,char *name)
{
	char fname[64];
	dmdl_t *head;
	int size,i;
	byte *ptr, *skin_names;

// Open model file
	strcpy(fname,name);

// Read the file
	mdl->Raw=(byte *)DAT_ReadFile(fname,&size);

	if (!mdl->Raw) return 0;

	ptr=mdl->Raw;

// Make Header pointer and fill out nums
	mdl->Header=(dmdl_t *)ptr; ptr+=sizeof(dmdl_t);
	head=mdl->Header;
	mdl->NumSkins=0;//head->num_skins;
	mdl->NumVerts=head->num_xyz;
	mdl->NumTexVerts=head->num_st;
	mdl->NumTris=head->num_tris;
	mdl->NumFrames=head->num_frames;
	mdl->NumGLCommands=head->num_glcmds;

// Check file type and version
	if (head->ident!=IDALIASHEADER||
	    head->version!=ALIAS_VERSION) {
		MDL_Free(mdl);
		return 0;
	}

// Set model name
	strcpy(mdl->Name,name);

// Allocate and load skins
	ptr=mdl->Raw+head->ofs_skins;
	skin_names=ptr;

// Make pointer to TexVerts
	ptr=mdl->Raw+head->ofs_st;
	mdl->TexVerts=(dstvert_t *)ptr;

// Make pointers to Tris
	ptr=mdl->Raw+head->ofs_tris;
	mdl->Tris=(dtriangle_t *)ptr;

// Make pointer to gl commands
	ptr=mdl->Raw+head->ofs_glcmds;
	mdl->glCommands=ptr;


// Make pointers to Frames
	ptr=mdl->Raw+head->ofs_frames;

	for(i=0;i<mdl->NumFrames;i++,ptr+=head->framesize)
		mdl->Frames[i]=(daliasframe_t *)ptr;

// Fix tex coords
//	if (Render_GetFlags()&R_TEXTURES_POW2)
		FixTexCoords(mdl);

// Load and fix textures
	ptr=skin_names;
	for(i=0;i<head->num_skins;i++,ptr+=MAX_SKINNAME) {
		memcpy(fname,ptr,64);
		fname[63]='\0';
		MDL_LoadSkin(mdl,fname);
	}

	return 1;
}


void FixTexCoords(MDL *mdl)
{
	dmdl_t *head;
	dstvert_t *v;
	int old_w, old_h, w, h, i;
	float s1, s2;

	head = mdl->Header;

	w = old_w = head->skinwidth;
	h = old_h = head->skinheight;

// Find power-of-two width
	if (w!=256&&w!=128&&w!=64&&w!=32&&w!=16&&
		w!=8&&w!=4&&w!=2&&w!=1) {
	// Find nearest power-of-two size, less than the original
		w=1;
		while(w<old_w) w<<=1;
		w>>=1;
		head->skinwidth=w;
	}

// Find power-of-two height
	if (h!=256&&h!=128&&h!=64&&h!=32&&h!=16&&
		h!=8&&h!=4&&h!=2&&h!=1) {
	// Find nearest power-of-two size, less than the original
		h=1;
		while(h<old_h) h<<=1;
		h>>=1;
		head->skinheight=h;
	}


// Nothing rescaled???
	if (w==old_w&&h==old_h)
		return;

// Go thru all tex verts and adjust U,V coords
	s1 = (float)w/(float)old_w;
	s2 = (float)w/(float)old_h;

	v = mdl->TexVerts;
	for(i=0;i<mdl->NumVerts;i++,v++) {
		v->s = (int)((s1*(float)v->s)+0.5);
		v->t = (int)((s2*(float)v->t)+0.5);
	}
}




void FixModelSkin_x(MDL *mdl,int skin_idx)
{
	dmdl_t *head=mdl->Header;
	MDLSkin *skin = &mdl->Skins[skin_idx];
	byte *buf, *dest, *src;
	int u, du, i, j;

	if (skin->width == head->skinwidth) return;

// Allocate temp buffer
	buf = (unsigned char*)malloc(skin->width*head->skinheight);
	dest = skin->pic;
	src = buf;
	memcpy(src,dest,skin->width*head->skinheight);
	du=(skin->width<<16)/head->skinwidth;
	for(j=0;j<head->skinheight;j++,src+=skin->width) {
		for(i=0,u=0;i<head->skinwidth;i++,dest++,u+=du) {
			dest[0]=src[u>>16];
		}
	}

	free(buf);

	skin->width=head->skinwidth;
}

void FixModelSkin_y(MDL *mdl,int skin_idx)
{
	dmdl_t *head=mdl->Header;
	MDLSkin *skin=&mdl->Skins[skin_idx];
	byte *buf, *dest, *src;
	int v, dv, j;

	head = mdl->Header;

	if (skin->height==head->skinheight) return;

// Allocate temp buffer
	buf = (unsigned char*)malloc(skin->width*skin->height);
	dest = skin->pic;
	memcpy(buf,dest,skin->width*skin->height);

	v=0;
	dv=(skin->height<<16)/head->skinheight;
	for(j=0,v=0;j<head->skinheight;j++,v+=dv) {
		src = buf+(v>>16)*skin->width;
		memcpy(dest,src,skin->width);
		dest+=skin->width;
	}
	free(buf);

	skin->height=head->skinheight;
}


int MDL_LoadSkin(MDL *mdl,char *fname)
{
	MDLSkin *skin=&mdl->Skins[mdl->NumSkins];
	byte *pcx;
	int w, h, size;
	
	if (mdl->NumSkins>=MAX_MD2SKINS) return -1;

	pcx = (unsigned char*)DAT_ReadFile(fname,&size);
	if (!pcx) return -1;

	skin->pic=PCX_Decode(pcx,&w,&h,skin->pal);
	free(pcx);

	if (!skin->pic) return -1;

	skin->width=w;
	skin->height=h;
	skin->is_rgba=0;

//	if (Render_GetFlags()&R_TEXTURES_POW2) {
		FixModelSkin_y(mdl,mdl->NumSkins);
		FixModelSkin_x(mdl,mdl->NumSkins);
//	}

	mdl->NumSkins++;
	return(mdl->NumSkins-1);
}



void MDL_Free(MDL *mdl)
{
	int i;

	if (mdl->Raw) {
		for(i=0;i<mdl->NumSkins;i++) {
			if (mdl->Skins[i].pic) free(mdl->Skins[i].pic);
		}
		free(mdl->Raw);
		mdl->Raw=0;
	}
}

int MDL_FirstFrame(MDL *mdl,char *name)
{
	int i;

	for(i=0;i<mdl->NumFrames;i++) {
		if (strnicmp(mdl->Frames[i]->name,name,strlen(name))==0) {
			return i;
		}
	}
	return -1;
}

int MDL_LastFrame(MDL *mdl,char *name)
{
	int i;

	i=MDL_FirstFrame(mdl,name);
	if (i<0) return -1;

	i++;
	for(;i<mdl->NumFrames;i++) {
		if (strnicmp(mdl->Frames[i]->name,name,strlen(name))!=0) {
			break;
		}
	}
	return i-1;
}

