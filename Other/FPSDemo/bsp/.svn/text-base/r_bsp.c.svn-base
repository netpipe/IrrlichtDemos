/********************************************************************
 R_BSP.C - Render BSP (bit flags)
*********************************************************************/

#define MAX_TEXFACES		4096

typedef struct {
	float *v;
	float s, t;
	float ls, lt;
	float dls, dlt;
} MyVert;

typedef struct {
	int numVerts;
	MyVert verts[16];
} MyFace;

static MyFace* MyFaces = new MyFace[MAX_MAP_FACES];

typedef struct _TexFace {
	int face_idx;
	struct _TexFace *next;
} TexFace;


static VisInfo vis;
static int cached_cam_leaf=-1;
static int cached_bsp_raw=-1;


// Bit array of faces. If bit is 1 - face is already in VisFaces[]
static byte VisFaceFlag[(MAX_MAP_FACES>>3)+1];


// All faces to draw, sorted by material
static TexFace *TexFacesPtr[1024];
static TexFace  TexFaces[MAX_TEXFACES];
static int NumTexFaces=0;

// All faces for the skybox
static TexFace *SkyFacesPtr=0;

// Textures used this frame
static int TexUsed[1024];
static int NumTexUsed=0;


int  BSP_FindLeaf(BSP *bsp, float *p);
void BSP_MakeVis(BSP *bsp,float *c,VisInfo *vis,int full_recalc);
void MakeWorldFaces(void);
int  MakeModelFaces(dmodel_t *M,float angle,float axis);
void DrawFaces(void);
void DrawSkyFaceList(TexFace *tf);
void DrawTexFaceList_tex(TexFace *tf,int tex_idx);
void DrawTexFaces_light(TexFace *tf, int num);

extern int ShowLightmap;

void PreCacheFaces(void)
{
	dface_t *face;
	CacheFace *info;
	texinfo_t *texinfo;
	dedge_t *e;
	float s,t,is,it, *v;
	int i,edge_idx;
	MyFace* myface;
	int face_idx, tex_idx, temp;

	for(face_idx=0;face_idx<cbsp->NumFaces;face_idx++)
        {

		myface=&MyFaces[face_idx];

		face=&cbsp->Faces[face_idx];
		info=&cbsp->CacheFaces[face_idx];
		texinfo=&cbsp->TexInfo[face->texinfo];
		tex_idx=cbsp->TextureList[face->texinfo];

        	// Setup texturing stuff
		is=1.0/(float)cbsp->Textures[tex_idx].miptex->width;
		it=1.0/(float)cbsp->Textures[tex_idx].miptex->height;

		myface->numVerts=face->numedges;
		if (myface->numVerts>16) myface->numVerts=16;

		for(i=0;i<myface->numVerts;i++) {
			edge_idx=face->firstedge+i;
			edge_idx=cbsp->EdgeList[edge_idx];

			if (edge_idx<0) {
				edge_idx=-edge_idx;
				e=&cbsp->Edges[edge_idx];
				myface->verts[i].v=v=cbsp->Verts[e->v[1]].point;
			}
			else {
				e=&cbsp->Edges[edge_idx];
				myface->verts[i].v=v=cbsp->Verts[e->v[0]].point;
			}

		// Tex coords
			s=FVec_DotProduct(v,texinfo->vecs[0])+texinfo->vecs[0][3];
			t=FVec_DotProduct(v,texinfo->vecs[1])+texinfo->vecs[1][3];
			myface->verts[i].s=s*is;
			myface->verts[i].t=t*it;

		// Light coords
			s=s-info->min_s+8.0f;
			t=t-info->min_t+8.0f;

			s/=256.0f; t/=256.0f;

			myface->verts[i].dls=s;
			myface->verts[i].dlt=t;

		// Index inside the bank
			temp=face_idx&(LMAP_FACES-1);

			s=(s+(float)( temp&(LMAP_SIDE-1) ))/(float)LMAP_SIDE;
			t=(t+(float)( temp>>LMAP_SIDE2   ))/(float)LMAP_SIDE;

			myface->verts[i].ls=s;
			myface->verts[i].lt=t;
		}

	}
}


/**********************************************************************
			DRAW TEXTURED BSP
***********************************************************************/

/*
  Render BSP to camera
*/
void GL_BeginBSP(BSP *bsp)
{
	int temp;

// Check if this bsp has been cached
	if (cached_bsp_raw!=(int)(bsp->NumVerts)) {
		cbsp=bsp;
		cached_bsp_raw=(int)(bsp->NumVerts);
		cached_cam_leaf=-1;
		Tex_PreCacheBSPTextures();
		PreCacheFaces();
	}

// Find all visible leaves of model #0
	temp=BSP_FindLeaf(cbsp,&camera[currentCamera].Position.x);
        if (temp!=cached_cam_leaf) {
		cached_cam_leaf=temp;
	// Full vis calc
		BSP_MakeVis(cbsp,&camera[currentCamera].Position.x,&vis,2);
	}
// Areas/portals only
	else {
		BSP_MakeVis(cbsp,&camera[currentCamera].Position.x,&vis,0);
	}

// Make vis faces
	MakeWorldFaces();

// Render normal faces
	DrawFaces();

// Render sky
	DrawSkyFaceList(SkyFacesPtr);
	SkyFacesPtr=0;

}


void GL_DrawBSPModel(POB *pob)
{

	dmodel_t *M=&cbsp->Models[pob->frame];

	FVec_Copy(pob->c,M->origin);
	if (MakeModelFaces(M,pob->angle,pob->vangle)) return;

	glPushMatrix();

	glTranslatef(M->origin[0],M->origin[1],M->origin[2]);

	if (pob->vangle==0) glRotatef(pob->angle*180.0/PI,1,0,0);
	else if (pob->vangle==1) glRotatef(pob->angle*180.0/PI,0,1,0);
	else glRotatef(pob->angle*180.0/PI,0,0,1);

	DrawFaces();

	glPopMatrix();
}


/******************************************************************
			3D CULLING
*******************************************************************/

static dplane_t frustum[5];
static int fix_planes[5][4];
extern FMatrix con;

/*
  Setup frustum planes in world coords
*/
void FrustumSetup(void)
{
  GLfloat ProjectionMatrix[16];
  GLfloat ModelViewMatrix[16];
  glGetFloatv(GL_PROJECTION_MATRIX, ProjectionMatrix);
  glGetFloatv(GL_MODELVIEW_MATRIX, ModelViewMatrix);

//!
  glMultMatrix(ProjectionMatrix, ModelViewMatrix);
  con[0][0] = ProjectionMatrix[0];
  con[0][1] = ProjectionMatrix[1];
  con[0][2] = ProjectionMatrix[2];
  con[1][0] = ProjectionMatrix[4];
  con[1][1] = ProjectionMatrix[5];
  con[1][2] = ProjectionMatrix[6];
  con[2][0] = ProjectionMatrix[8];
  con[2][1] = ProjectionMatrix[9];
  con[2][2] = ProjectionMatrix[10];
	float s,c;
    s=(float)sin((PI/2)*0.5f);
	c=(float)cos((PI/2)*0.5f);
    GLfloat proj=(c*(float)(800>>1))/s;
	GLfloat proj_ratio=800.0f;//*0.75f;
	proj_ratio=600.0f/proj_ratio;
	float v[3];
	float a;
	int idx = 0;

// Min_Z plane
	v[0]=0; v[1]=0; v[2]=-1.0;
        FMatrix_MulFVec(con,v,frustum[idx].normal);
	frustum[idx].dist=FVec_DotProduct(frustum[idx].normal,&camera[currentCamera].Position.x);
	idx++;

// Horizontal FOV
    a=(float)atan2((double)(800>>1),proj);
    c=(float)cos(a);
    s=(float)sin(a);

// Left plane
	v[0]=-c; v[1]=0; v[2]=-s;
	FMatrix_MulFVec(con,v,frustum[idx].normal);
	frustum[idx].dist=FVec_DotProduct(frustum[idx].normal,&camera[currentCamera].Position.x);
	idx++;

// Right plane
	v[0]=c; v[1]=0; v[2]=-s;
	FMatrix_MulFVec(con,v,frustum[idx].normal);
	frustum[idx].dist=FVec_DotProduct(frustum[idx].normal,&camera[currentCamera].Position.x);
	idx++;

// Vertical FOV
	a=(float)atan2((double)(600>>1),proj*proj_ratio);
        c=(float)cos(a);
	s=(float)sin(a);

// Bottom plane
	v[0]=0; v[1]=-c; v[2]=-s;
	FMatrix_MulFVec(con,v,frustum[idx].normal);
	frustum[idx].dist=FVec_DotProduct(frustum[idx].normal,&camera[currentCamera].Position.x);
	idx++;

// Top plane
	v[0]=0; v[1]=c; v[2]=-s;
	FMatrix_MulFVec(con,v,frustum[idx].normal);
	frustum[idx].dist=FVec_DotProduct(frustum[idx].normal,&camera[currentCamera].Position.x);
	idx++;

	for(idx=0;idx<5;idx++) {
		fix_planes[idx][0]=(int)(frustum[idx].normal[0]*65536.0);
		fix_planes[idx][1]=(int)(frustum[idx].normal[1]*65536.0);
		fix_planes[idx][2]=(int)(frustum[idx].normal[2]*65536.0);
		fix_planes[idx][3]=(int)(frustum[idx].dist*65536.0);
	}
}

/*
  Check for 3d culling of a box (shorts) against frustum
  Returns true if culled
*/
int FrustumCullBox_short(short *mins, short *maxs)
{

	int dot, a, i,j;

	for(i=0;i<5;i++) {
		dot=-fix_planes[i][3];
		for(j=0;j<3;j++) {
			a=fix_planes[i][j];
			if (a>0) dot+=a*(int)mins[j];
			else dot+=a*(int)maxs[j];
		}
		if (dot>0) return 1;
	}

	return 0;
}

/*
  Check for 3d culling of a box (floats) against frustum
  Returns true if culled
*/
int FrustumCullBox_float(float *mins, float *maxs)
{
	float *normal, dot, a;
	int i,j;

	for(i=0;i<5;i++) {
		dot=-frustum[i].dist;
		normal=frustum[i].normal;
		for(j=0;j<3;j++) {
			a=normal[j];
			if (a>0) dot+=a*mins[j];
			else dot+=a*maxs[j];
		}
		if (dot>0) return 1;
	}

	return 0;
}


/******************************************************************
			VISIBLE LISTS
*******************************************************************/

/*
  Add leaf's faces that are not on the list yet.
*/
void AddFaceList(int firstleafface,int numleaffaces)
{
	dface_t *face;
	texinfo_t *texinfo;
	dplane_t *plane;
	TexFace *tf;
	int i,face_idx,idx, first, last;
	float dot;
	byte bit;

	first=cbsp->Models[0].firstface;
	last=first+cbsp->Models[0].numfaces;

	for(i=0;i<numleaffaces;i++) {

	// Check if this face has already been seen
		face_idx=firstleafface+i;
		face_idx=cbsp->FaceList[face_idx];

	// Does not belong to world model
		if (face_idx<first||face_idx>=last) continue;

		idx=face_idx>>3;
		bit=(unsigned char)(1<<(face_idx&7));

	// Already seen
		if ((VisFaceFlag[idx]&bit)!=0) continue;

		face=&cbsp->Faces[face_idx];

	// Does poly face camera?
		plane=&cbsp->Planes[face->planenum];
		dot=FVec_DotProduct(plane->normal,&camera[currentCamera].Position.x
)-plane->dist;
		if (!face->side) { if (dot<0.01) continue; }
		else { if (dot>-0.01) continue; }

	// Sky face, do not draw
		texinfo=&cbsp->TexInfo[face->texinfo];

	// New visible face!
		VisFaceFlag[idx]|=bit;

	// Allocate a new TexFaces
		if (NumTexFaces>=MAX_TEXFACES) return;

		tf=&TexFaces[NumTexFaces++];

	// If sky add face to skybox list
		if (texinfo->flags&SURF_SKY) {
			tf->face_idx=face_idx;
			tf->next=SkyFacesPtr;
			SkyFacesPtr=tf;
		}
	// Add TexFace to material list
		else {
			idx=cbsp->TextureList[face->texinfo];
			if (!TexFacesPtr[idx]) {
				if (texinfo->flags&(SURF_TRANS66|SURF_TRANS33))
					TexUsed[NumTexUsed++]=-idx;
				else
					TexUsed[NumTexUsed++]=idx;
			}

			tf->face_idx=face_idx;
			tf->next=TexFacesPtr[idx];
			TexFacesPtr[idx]=tf;
		}
	}
}



/*
  Make list of all visible faces.
  Decompress leaf vislist, and add all _new_ faces of a visible leaf
  to the list of visible faces.
*/
void MakeWorldFaces()
{
	dleaf_t *leaf;
	int i, vis_byte, vis_mask;

	memset(VisFaceFlag,0,(cbsp->NumFaces+7)>>3);

	FrustumSetup();

// Find visible faces
	leaf=&cbsp->Leaves[0];
	for(i=0;i<cbsp->NumLeaves;i++,leaf++) {

		vis_byte=vis.VisClusters[leaf->cluster>>3];
		vis_mask=1<<(leaf->cluster&7);

		if ( (vis_byte&vis_mask) && vis.VisAreas[leaf->area] ) {

			if (!FrustumCullBox_short(leaf->mins,leaf->maxs)) {
				AddFaceList(leaf->firstleafface,
						leaf->numleaffaces);
			}
		}

	}
}




/*
  Make BSP MODEL faces (no VisFaces calcs).
	Cull the model. Process all model faces
  returns true if culled
*/
int MakeModelFaces(dmodel_t *M,float angle,float axis)
{
	FMatrix xform;
	dface_t *face;
	dplane_t *plane;
	texinfo_t *texinfo;
	TexFace *tf;
	float dot, mins[3], maxs[3], normal[3];
	int face_idx, idx;

// Cull the model
	FVec_AddFVec(M->mins,M->origin,mins);
	FVec_AddFVec(M->maxs,M->origin,maxs);
	if (FrustumCullBox_float(mins,maxs)) return 1;

	FMatrix_Identity(xform);
	if (axis==0) FMatrix_PreXRot(xform,angle);
	else if (axis==1) FMatrix_PreYRot(xform,angle);
	else FMatrix_PreZRot(xform,angle);


	for(face_idx=M->firstface;face_idx<M->firstface+M->numfaces;face_idx++) {

		face=&cbsp->Faces[face_idx];

	// Sky face, do not draw (should not happen)
		texinfo=&cbsp->TexInfo[face->texinfo];
		if (texinfo->flags&SURF_SKY) {
			//draw_sky=1;
			continue;
		}

	// Does poly face camera?
		plane=&cbsp->Planes[face->planenum];
		FMatrix_MulFVec(xform,plane->normal,normal);
		dot=FVec_DotProduct(normal,ccam->c)-plane->dist;
		dot-=FVec_DotProduct(normal,M->origin);
		if (!face->side) { if (dot<0.01) continue; }
		else { if (dot>-0.01) continue; }

	// Add face to the material list
		if (NumTexFaces>=MAX_TEXFACES) break;

		tf=&TexFaces[NumTexFaces++];

		idx=cbsp->TextureList[face->texinfo];
		if (!TexFacesPtr[idx]) {
			if (texinfo->flags&(SURF_TRANS66|SURF_TRANS33))
				TexUsed[NumTexUsed++]=-idx;
			else
				TexUsed[NumTexUsed++]=idx;
		}

		tf->face_idx=face_idx;
		tf->next=TexFacesPtr[idx];
		TexFacesPtr[idx]=tf;
	}
	return 0;
}


void DrawFaces(void)
{
        int i, tex_idx;

	if (ShowLightmap)
	{
        // Fake texture rendering
		for(i=0;i<NumTexUsed;i++) {
			tex_idx=abs(TexUsed[i]);
			TexFacesPtr[tex_idx]=0;
		}
		NumTexUsed=0;
	// Render lightmaps
		DrawTexFaces_light(TexFaces,NumTexFaces);
		NumTexFaces=0;
		return;
	}

// Render solid faces
	for(i=0;i<NumTexUsed;i++) {
		tex_idx=TexUsed[i];
		if (tex_idx>=0) {
			DrawTexFaceList_tex(TexFacesPtr[tex_idx],tex_idx);
			TexFacesPtr[tex_idx]=0;
		}
	}

// Setup blending mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_ZERO,GL_SRC_COLOR);

// Render lightmaps
	if (!ShowLightmap)
		DrawTexFaces_light(TexFaces,NumTexFaces);

// Render translucent faces
	for(i=0;i<NumTexUsed;i++) {
		tex_idx=-TexUsed[i];
		if (tex_idx>0) {
			        DrawTexFaceList_tex(TexFacesPtr[tex_idx],tex_idx);
	TexFacesPtr[tex_idx]=0;
		}
	}

	NumTexUsed=0;

	glDisable(GL_BLEND);
	NumTexFaces=0;
}


void DrawTexFaceList_tex(TexFace *tf,int tex_idx)
{
	MyFace *myface;
	MyVert *myvert;
	int i;

	Tex_SetBSPTexture(tex_idx);

	while(tf) {

	// Get all necessary pointers
		myface=&MyFaces[tf->face_idx];

		if (myface->numVerts==3) glBegin(GL_TRIANGLES);
		else if (myface->numVerts==4) glBegin(GL_QUADS);
		else glBegin(GL_POLYGON);

		myvert=myface->verts;
		for(i=0;i<myface->numVerts;i++,myvert++) {
			glTexCoord2f(myvert->s,myvert->t);
			glVertex3fv(myvert->v);
		}
		glEnd();

		tf=tf->next;
	}
}


static int DLightFaces[1024];
static int NumDLightFaces;


void DrawTexFaces_light(TexFace *tf_start, int tf_num)
{
	MyFace *myface;
	MyVert *myvert;
	TexFace *tf;
	int i,bank,numbanks,num, bank_inited, temp;
    //int edge_idx;
	numbanks=((cbsp->NumFaces>>LMAP_FACES2)+1);

	NumDLightFaces=0;

	for(bank=0;bank<numbanks;bank++) {

		bank_inited=0;

		for(num=tf_num,tf=tf_start;num>0;num--,tf++) {

			if ((tf->face_idx>>LMAP_FACES2)!=bank) continue;

			if (!bank_inited) {
				Tex_SetBSPLightmapBank(bank);
				bank_inited=1;
			}

		// Set lightmap source
			temp=Tex_IsDynamicLightmap(tf->face_idx);

		// Dynamic lightmap or no lightmap at all
			if (temp>=0) {
				if (temp>0&&NumDLightFaces<1024) {
					DLightFaces[NumDLightFaces]=tf->face_idx;
					NumDLightFaces++;
				}
				continue;
			}

		// Render the face
			myface=&MyFaces[tf->face_idx];

			if (myface->numVerts==3) glBegin(GL_TRIANGLES);
			else if (myface->numVerts==4) glBegin(GL_QUADS);
			else glBegin(GL_POLYGON);

			myvert=myface->verts;
			for(i=0;i<myface->numVerts;i++,myvert++) {
				glTexCoord2f(myvert->ls,myvert->lt);
				glVertex3fv(myvert->v);
			}
			glEnd();
		}
	}

// Render dynamic lightmaps (if any)

	if (NumDLightFaces>0) {
		Tex_BeginDynamicLighting();
		for(num=0;num<NumDLightFaces;num++) {

			Tex_SetDynamicLightmap(DLightFaces[num]);
			myface=&MyFaces[DLightFaces[num]];

			if (myface->numVerts==3) glBegin(GL_TRIANGLES);
			else if (myface->numVerts==4) glBegin(GL_QUADS);
			else glBegin(GL_POLYGON);

			myvert=myface->verts;
			for(i=0;i<myface->numVerts;i++,myvert++) {
				glTexCoord2f(myvert->dls,myvert->dlt);
				glVertex3fv(myvert->v);
			}
			glEnd();

		}
	}
}



#define SKYSIZE			2000.0f
#define SKY_FRONT		1
#define SKY_BACK		2
#define SKY_LEFT		4
#define SKY_RIGHT		8
#define SKY_UP			16
#define SKY_DOWN		32


void DrawSkyFaceList(TexFace *tf)
{
	dface_t *face;
	dplane_t *plane;
	dedge_t *e;
	float *v, d[3];
	int i, edge_idx, sky_flag=0;

	while(tf) {

		face=&cbsp->Faces[tf->face_idx];
		plane=&cbsp->Planes[face->planenum];

		if (plane->type==0||plane->type==3) {
			if ((plane->normal[0]<0&&face->side)||
			    (plane->normal[0]>0&&!face->side))
				sky_flag|=SKY_LEFT;
			else
				sky_flag|=SKY_RIGHT;
		}
		else if (plane->type==1||plane->type==4) {
			if ((plane->normal[1]<0&&face->side)||
			    (plane->normal[1]>0&&!face->side))
				sky_flag|=SKY_BACK;
			else
				sky_flag|=SKY_FRONT;
		}
		else {
			if ((plane->normal[2]<0&&face->side)||
			    (plane->normal[2]>0&&!face->side))
				sky_flag|=SKY_DOWN;
			else
				sky_flag|=SKY_UP;
		}



		for(i=0;i<face->numedges;i++) {
			edge_idx=face->firstedge+i;
			edge_idx=cbsp->EdgeList[edge_idx];
			if (edge_idx<0) {
				edge_idx=-edge_idx;
				e=&cbsp->Edges[edge_idx];
				v=cbsp->Verts[e->v[1]].point;
			}
			else {
				e=&cbsp->Edges[edge_idx];
				v=cbsp->Verts[e->v[0]].point;
			}

			FVec_SubFVec(v,ccam->c,d);

			if (fabs(d[0])>fabs(d[1])) {
				if (fabs(d[0])>fabs(d[2])) {
					if (d[0]<0) sky_flag|=SKY_LEFT;
					else sky_flag|=SKY_RIGHT;
				}
				else {
					if (d[2]<0) sky_flag|=SKY_DOWN;
					else sky_flag|=SKY_UP;
				}
			}
			else {
				if (fabs(d[1])>fabs(d[2])) {
					if (d[1]<0) sky_flag|=SKY_BACK;
					else sky_flag|=SKY_FRONT;
				}
				else {
					if (d[2]<0) sky_flag|=SKY_DOWN;
					else sky_flag|=SKY_UP;
				}
			}
		}

		tf=tf->next;
	}

// FRONT
	if (sky_flag&SKY_FRONT) {
		Tex_SetSkyTexture(0);

		glBegin(GL_QUADS);

		glTexCoord2f(1,0);
		glVertex3f(ccam->c[0]-SKYSIZE,ccam->c[1]+SKYSIZE,ccam->c[2]+SKYSIZE);

		glTexCoord2f(0,0);
		glVertex3f(ccam->c[0]+SKYSIZE,ccam->c[1]+SKYSIZE,ccam->c[2]+SKYSIZE);

		glTexCoord2f(0,1);
		glVertex3f(ccam->c[0]+SKYSIZE,ccam->c[1]+SKYSIZE,ccam->c[2]-SKYSIZE);

		glTexCoord2f(1,1);
		glVertex3f(ccam->c[0]-SKYSIZE,ccam->c[1]+SKYSIZE,ccam->c[2]-SKYSIZE);

		glEnd();
	}

// BACK
	if (sky_flag&SKY_BACK) {
		Tex_SetSkyTexture(1);

		glBegin(GL_QUADS);

		glTexCoord2f(0,0);
		glVertex3f(ccam->c[0]-SKYSIZE,ccam->c[1]-SKYSIZE,ccam->c[2]+SKYSIZE);

		glTexCoord2f(0,1);
		glVertex3f(ccam->c[0]-SKYSIZE,ccam->c[1]-SKYSIZE,ccam->c[2]-SKYSIZE);

		glTexCoord2f(1,1);
		glVertex3f(ccam->c[0]+SKYSIZE,ccam->c[1]-SKYSIZE,ccam->c[2]-SKYSIZE);

		glTexCoord2f(1,0);
		glVertex3f(ccam->c[0]+SKYSIZE,ccam->c[1]-SKYSIZE,ccam->c[2]+SKYSIZE);

		glEnd();
	}

// LEFT
	if (sky_flag&SKY_LEFT) {
		Tex_SetSkyTexture(2);

		glBegin(GL_QUADS);

		glTexCoord2f(1,0);
		glVertex3f(ccam->c[0]-SKYSIZE,ccam->c[1]-SKYSIZE,ccam->c[2]+SKYSIZE);

		glTexCoord2f(0,0);
		glVertex3f(ccam->c[0]-SKYSIZE,ccam->c[1]+SKYSIZE,ccam->c[2]+SKYSIZE);

		glTexCoord2f(0,1);
		glVertex3f(ccam->c[0]-SKYSIZE,ccam->c[1]+SKYSIZE,ccam->c[2]-SKYSIZE);

		glTexCoord2f(1,1);
		glVertex3f(ccam->c[0]-SKYSIZE,ccam->c[1]-SKYSIZE,ccam->c[2]-SKYSIZE);

		glEnd();
	}

// RIGHT
	if (sky_flag&SKY_RIGHT) {
		Tex_SetSkyTexture(3);

		glBegin(GL_QUADS);

		glTexCoord2f(1,0);
		glVertex3f(ccam->c[0]+SKYSIZE,ccam->c[1]+SKYSIZE,ccam->c[2]+SKYSIZE);

		glTexCoord2f(0,0);
		glVertex3f(ccam->c[0]+SKYSIZE,ccam->c[1]-SKYSIZE,ccam->c[2]+SKYSIZE);

		glTexCoord2f(0,1);
		glVertex3f(ccam->c[0]+SKYSIZE,ccam->c[1]-SKYSIZE,ccam->c[2]-SKYSIZE);

		glTexCoord2f(1,1);
		glVertex3f(ccam->c[0]+SKYSIZE,ccam->c[1]+SKYSIZE,ccam->c[2]-SKYSIZE);

		glEnd();
	}

// UP
	if (sky_flag&SKY_UP) {
		Tex_SetSkyTexture(4);

		glBegin(GL_QUADS);

		glTexCoord2f(0,1);
		glVertex3f(ccam->c[0]+SKYSIZE,ccam->c[1]-SKYSIZE,ccam->c[2]+SKYSIZE);

		glTexCoord2f(1,1);
		glVertex3f(ccam->c[0]+SKYSIZE,ccam->c[1]+SKYSIZE,ccam->c[2]+SKYSIZE);

		glTexCoord2f(1,0);
		glVertex3f(ccam->c[0]-SKYSIZE,ccam->c[1]+SKYSIZE,ccam->c[2]+SKYSIZE);

		glTexCoord2f(0,0);
		glVertex3f(ccam->c[0]-SKYSIZE,ccam->c[1]-SKYSIZE,ccam->c[2]+SKYSIZE);

		glEnd();
	}

// DOWN
	if (sky_flag&SKY_DOWN) {
		Tex_SetSkyTexture(5);

		glBegin(GL_QUADS);

		glTexCoord2f(0,1);
		glVertex3f(ccam->c[0]-SKYSIZE,ccam->c[1]-SKYSIZE,ccam->c[2]-SKYSIZE);

		glTexCoord2f(1,1);
		glVertex3f(ccam->c[0]-SKYSIZE,ccam->c[1]+SKYSIZE,ccam->c[2]-SKYSIZE);

		glTexCoord2f(1,0);
		glVertex3f(ccam->c[0]+SKYSIZE,ccam->c[1]+SKYSIZE,ccam->c[2]-SKYSIZE);

		glTexCoord2f(0,0);
		glVertex3f(ccam->c[0]+SKYSIZE,ccam->c[1]-SKYSIZE,ccam->c[2]-SKYSIZE);

		glEnd();
	}
}



/********************************************************************
			BSP VISIBILITY STUFF
*********************************************************************/


/*
  Find a bsp leaf for given coords
*/
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


