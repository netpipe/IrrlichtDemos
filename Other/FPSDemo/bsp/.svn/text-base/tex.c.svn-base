#define MAX_DLIGHTS			256
#define MAX_SURFLIGHTS		16
#define BASE_BSP_TEX		1
#define BASE_SKY_TEX		190
#define BASE_MDL_TEX		200
#define BASE_SPR_TEX		300
#define BASE_LMAP_TEX		400

PFNGLCOLORTABLEEXTPROC glColorTableEXT;

typedef struct {
	float c[3];
	float rgb[3];
	float radius;
} DLight;

typedef struct {
	int x, y;
	int radius;
	int rgb[3];
} SurfLight;

static DLight VisDLights[MAX_DLIGHTS];
int NumVisDLights=0;
static SurfLight SurfLights[MAX_SURFLIGHTS];
static int NumSurfLights=0;
static byte DynamicLightMap[17*17*4];
static byte texpal[768];
static byte tex[256*256*4];
//static int texpal16[256];
static dword mdls[100], sprs[100];
static int numMdls, numSprs;


void InitGLTextures()
{
	int i,a;

// Get ColorTable extension
	glColorTableEXT = (PFNGLCOLORTABLEEXTPROC)
	wglGetProcAddress ("glColorTableEXT");

// Misc GL tex params
	glPixelStorei(GL_PACK_ALIGNMENT,1);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);


// Init lightmap textures
	memset(DynamicLightMap,128,16*16*3);

	glBindTexture(GL_TEXTURE_2D,0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
			16, 16, 0,
			GL_RGB, GL_UNSIGNED_BYTE, DynamicLightMap);

	numMdls=numSprs=0;


// Tetxure gamma hack
	for(i=0;i<768;i++) {
		a=TexPalette[i];
		a*=2;
		if (a>255) a=255;
		texpal[i]=(byte)a;
	}

}


void GL_AddDLight(float *coord,float *rgb,float /*intensity*/,float radius)
{
	DLight *l=&VisDLights[NumVisDLights];

	if (NumVisDLights>=MAX_DLIGHTS) return;
	NumVisDLights++;

	FVec_Copy(coord,l->c);
	FVec_Copy(rgb,l->rgb);
	l->radius=radius;
}


int CheckSurfLights(CacheFace *info, texinfo_t *texinfo, dplane_t *plane)
{
	DLight *pl=VisDLights;
	float u, v, n;
	int i;

	if (NumVisDLights<=0) return 0;

	for(i=0;i<NumVisDLights;i++,pl++) {

		n=FVec_DotProduct(pl->c,plane->normal)-plane->dist;
		n=pl->radius-fabs(n);
		if (n<=1.0) continue;

		u=FVec_DotProduct(pl->c,texinfo->vecs[0])+
				texinfo->vecs[0][3]-info->min_s+8.0f;
		if (u<(-pl->radius) || u>(((float)info->size_s)+pl->radius))
			continue;
		v=FVec_DotProduct(pl->c,texinfo->vecs[1])+
				texinfo->vecs[1][3]-info->min_t+8.0f;
		if (v<(-pl->radius) || v>(((float)info->size_t)+pl->radius))
			continue;

		return 1;
	}

	return 0;
}


void MakeSurfLights(CacheFace *info, texinfo_t *texinfo, dplane_t *plane)
{
	DLight *pl=VisDLights;
	SurfLight *sl=SurfLights;
	float u, v, n;
	int i;

	NumSurfLights=0;


	for(i=0;i<NumVisDLights;i++,pl++) {

		n=FVec_DotProduct(pl->c,plane->normal)-plane->dist;
		n=pl->radius-fabs(n);
		if (n<=1.0) continue;

		u=FVec_DotProduct(pl->c,texinfo->vecs[0])+
				texinfo->vecs[0][3]-info->min_s+8.0f;
		if (u<(-pl->radius) || u>(((float)info->size_s)+pl->radius))
			continue;

		v=FVec_DotProduct(pl->c,texinfo->vecs[1])+
				texinfo->vecs[1][3]-info->min_t+8.0f;
		if (v<(-pl->radius) || v>(((float)info->size_t)+pl->radius))
			continue;

		sl->x=(int)u; sl->y=(int)v;
		sl->radius=(int)n;
		sl->rgb[0]=(int)(pl->rgb[0]*255);
		sl->rgb[1]=(int)(pl->rgb[1]*255);
		sl->rgb[2]=(int)(pl->rgb[2]*255);

		NumSurfLights++; sl++;
	}
}


int FastDist(int x1, int y1, int x2, int y2)
{
	if ((x2-=x1)<0) x2=-x2;
	if ((y2-=y1)<0) y2=-y2;
	return(x2+y2-( ((x2>y2)?y2:x2)>>1 ));
}


void ApplySurfLights(int light_width, int light_height)
{
	SurfLight *sl;
	byte *light;
	int n, i, j, x, y, a, r, g, b;

	light=DynamicLightMap;

	for(j=0,y=0;j<light_height;j++,y+=16) {
		for(i=0,x=0;i<light_width;i++,x+=16,light+=3) {
			r=light[0];
			g=light[1];
			b=light[2];

			sl=SurfLights;
			for(n=0;n<NumSurfLights;n++,sl++) {
				a=sl->radius-FastDist(x,y,sl->x,sl->y);
				if (a<0) continue;
				r+=(a*sl->rgb[0])>>6;
				g+=(a*sl->rgb[1])>>6;
				b+=(a*sl->rgb[2])>>6;
			}

			if (r>255) r=255;
			if (g>255) g=255;
			if (b>255) b=255;

			light[0]=(byte)r;
			light[1]=(byte)g;
			light[2]=(byte)b;
		}
		//light+=(16-light_width)*3;
	}
}


void InitBSPTexture(int);
void InitLightmapBank(int);


void Tex_PreCacheBSPTextures(void)
{
	int i;

// Pre-cache all bsp textures
	for(i=0;i<cbsp->NumTextures;i++) {
		InitBSPTexture(i);
	}

// Pre-cache all sky textures
	for(i=0;i<6;i++) {
		Tex_SetSkyTexture(i);
	}

// Pre-cache all lightmaps
	for(i=0;i<((cbsp->NumFaces>>LMAP_FACES2)+1);i++) {
		InitLightmapBank(i);
	}
}


void Tex_SetBSPLightmapBank(int bank)
{
	char str[64];

	if (bank>=MAX_LMAP_BANKS) {
		sprintf(str,"too many lmap banks %d\n", bank);
//!		MessageBox(NULL,str,"info",MB_OK);
		return;
	}

	glBindTexture(GL_TEXTURE_2D,BASE_LMAP_TEX+bank);
        //sprintf(texturename, "%d", BASE_LMAP_TEX+bank);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}


int Tex_IsDynamicLightmap(int face_idx)
{
	dface_t *face;
	texinfo_t *texinfo;
	CacheFace *info;
//	byte *src;
//	int i,lw,lh,dynamic,key, bank;

	face=&cbsp->Faces[face_idx];
	info=&cbsp->CacheFaces[face_idx];
	texinfo=&cbsp->TexInfo[face->texinfo];

// Special surface
	if (texinfo->flags) return 0;

// Check if lightmap is dynamic
	if (CheckSurfLights(info,texinfo,&cbsp->Planes[face->planenum])) return 1;

// Nope, normal boring lightmap
	return -1;
}


void Tex_BeginDynamicLighting(void)
{
	glBindTexture(GL_TEXTURE_2D,0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}


void Tex_SetDynamicLightmap(int face_idx)
{
	dface_t *face;
	texinfo_t *texinfo;
	CacheFace *info;
	byte *src;
	int i,lw,lh;//,dynamic,key, bank;

	face=&cbsp->Faces[face_idx];
	info=&cbsp->CacheFaces[face_idx];
	texinfo=&cbsp->TexInfo[face->texinfo];

// Setup surf lights for this face
	MakeSurfLights(info,texinfo,&cbsp->Planes[face->planenum]);

// Make the lightmap
	src=cbsp->LightMaps+face->lightofs;
	lw=(info->size_s>>4)+1;
	lh=(info->size_t>>4)+1;

// Copy the original lightmap
	for(i=0;i<lh;i++)
		memcpy(DynamicLightMap+(i*lw)*3,src+(i*lw)*3,lw*3);

// Apply lights
	ApplySurfLights(lw,lh);
	src=DynamicLightMap;

// Set GL texture
	glTexSubImage2D(GL_TEXTURE_2D, 0,
		0, 0, lw,lh,
		GL_RGB, GL_UNSIGNED_BYTE, src);
}


void InitLightmapBank(int bank)
{
	dface_t *face;
	texinfo_t *texinfo;
	CacheFace *info;
	byte *src;
	int lw,lh;//,i,dynamic,key;
	int face_idx, last, temp;


	Tex_SetBSPLightmapBank(bank);

// Init lightmap banks
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
			LMAP_SIZE, LMAP_SIZE, 0,
			GL_RGB, GL_UNSIGNED_BYTE, tex);

// Paste all the faces lightmaps
	face_idx=bank<<LMAP_FACES2;
	last=face_idx+LMAP_FACES;
	if (last>cbsp->NumFaces) last=cbsp->NumFaces;

	for(;face_idx<last;face_idx++) {

		face=&cbsp->Faces[face_idx];
		info=&cbsp->CacheFaces[face_idx];
		texinfo=&cbsp->TexInfo[face->texinfo];

	// Special surface
		if (texinfo->flags) continue;

	// Make the lightmap
		src=cbsp->LightMaps+face->lightofs;
		lw=(info->size_s>>4)+1;
		lh=(info->size_t>>4)+1;

	// Index inside the bank
		temp=face_idx&(LMAP_FACES-1);

		glTexSubImage2D(GL_TEXTURE_2D, 0,
			(temp&(LMAP_SIDE-1))<<4, (temp>>LMAP_SIDE2)<<4, lw,lh,
			GL_RGB, GL_UNSIGNED_BYTE, src);
	}
}



void Tex_SetBSPTexture(int tex_idx)
{
	glBindTexture(GL_TEXTURE_2D,BASE_BSP_TEX+tex_idx);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}


void InitBSPTexture(int tex_idx)
{
	BSPTexture *bt;
	miptex_t *miptex;
	int i,j,w,h,a;//,key;
	byte *src;

	Tex_SetBSPTexture(tex_idx);

	bt=&cbsp->Textures[tex_idx];
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);// GL_NEAREST_MIPMAP_NEAREST);
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

// I'll remove this stuff, once I figure out how to do paletted textures
	miptex=bt->miptex;

	w=miptex->width;
	h=miptex->height;
	src=(byte *)miptex+miptex->offsets[0];

	for(j=0;j<h;j++) {
		for(i=0;i<w;i++) {
			a=src[j*w+i];
			tex[(j*w+i)*3+0]=texpal[a*3+0];
			tex[(j*w+i)*3+1]=texpal[a*3+1];
			tex[(j*w+i)*3+2]=texpal[a*3+2];
		}
	}

	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,w,h,GL_RGB,GL_UNSIGNED_BYTE,tex);
}


void Tex_SetModelSkin(MDL *mdl,int skin_idx)
{
	int a,i,j,w,h;//,key;
	MDLSkin *skin = &mdl->Skins[skin_idx];
	byte *pal;


// Try to find the texture
	for(i=0;i<numMdls;i++) {
		if (mdls[i]==skin_idx+(dword)mdl) {
			glBindTexture(GL_TEXTURE_2D,BASE_MDL_TEX+i);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			return;
		}
	}

// Nope, allocate a new name for the texture
	mdls[numMdls]=skin_idx+(dword)mdl; numMdls++;
	glBindTexture(GL_TEXTURE_2D,BASE_MDL_TEX+numMdls-1);

// Setup texture params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


// I'll remove this stuff, once I figure out how to do paletted textures
	w=skin->width;
	h=skin->height;

	pal = skin->pal;

	for(j=0;j<h;j++) {
		for(i=0;i<w;i++) {
			a=skin->pic[j*w+i];
			tex[(j*w+i)*3+0]=pal[a*3+0];
			tex[(j*w+i)*3+1]=pal[a*3+1];
			tex[(j*w+i)*3+2]=pal[a*3+2];
		}
	}


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
			w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, tex);
}

void Tex_SetSkyTexture(int idx)
{
//	int i, j, a;

	glBindTexture(GL_TEXTURE_2D,BASE_SKY_TEX+idx);

	if (cbsp->Sky.cache_offsets[idx]!=BASE_SKY_TEX+idx) {
		cbsp->Sky.cache_offsets[idx]=BASE_SKY_TEX+idx;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//NEAREST
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, cbsp->Sky.raw[idx]);
	}
}

