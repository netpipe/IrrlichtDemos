/********************************************************************
 COLDET.C - Collision-detection stuff
*********************************************************************/


/*
 Notes on collision detection
-------------------------------
 An object calls a clipping routine from its prog_update. It's up
 to the object to decide against what other objects it wants to be
 tested. Normally, you would want to call clipping code only for 
 objects that actually move (player, projectiles, monsters). 
 Collision routines of both objects participating in a collision
 will be called.

 Clip info of an object is based on its type and additional flags:

	none, model or sprite:
		0     - no checks
		clip  - clip against bound box.
		check - just check bound boxes and pass thru.

	bsp model
		0     - no checks
		clip  - full clip against bsp brushes.
		check - just check bound box and pass thru.
*/



#include "engine.h"

//int RDTSC(void);
//#pragma aux RDTSC = 0x0f 0x31 modify [eax edx] value [eax];

#define MAX_CLIPBOXES		256

typedef struct {
	int obj_idx;
	float mins[3], maxs[3];		// World coords
} ClipBox;

static ClipBox ClipBoxes[MAX_CLIPBOXES];
static int NumClipBoxes;


typedef struct {
	int obj_idx;
	int model_idx;
	int numClipBrushes;
} ClipBSPModel;

static ClipBSPModel ClipBSPModels[MAX_MAP_MODELS];
static int  NumClipBSPModels;

static byte ClipBrushFlag[(MAX_MAP_BRUSHES>>3)+1];
static word ClipBrushes[MAX_MAP_BRUSHES];
static int  NumClipBrushes;
static int  NumClipBrushSides;	// for debug

// General clip stuff
//static BSP *cbsp;		// bsp to work with
static int cobj_idx;		// object to test collision for

// Only for brush/box tests
static float *omins, *omaxs;	// dims of object to test
static float oradius;		// <------- hack

// Only for bounding box tests
static int imins[3], imaxs[3];	// speed box (integers)
static float fmins[3], fmaxs[3];// speed box (floats)
static float check_mins[3], check_maxs[3];	// check box


void check_boundbox(int obj_idx,int flag);
void check_bsp_model(int model_idx,int obj_idx);
void check_node_r(int node_idx);
void check_leaf(int leaf_idx);


extern BSP *cbsp;

void Clip_Begin(BSP *bsp, int obj_idx, float *start, float *delta, 
	float *extra_mins, float *extra_maxs, int flags) 
{
	Object *o=&Objects[obj_idx];
	int i, flag;

	cbsp=bsp;
	cobj_idx=obj_idx;
	omins=o->mins;
	omaxs=o->maxs;

// !!! Hack !!!
	oradius=(omaxs[0]-omins[0])*0.5;

// Make speed-box
	for(i=0;i<3;i++) {
		if (delta[i]>0) {
			check_mins[i]=start[i]+omins[i];
			check_maxs[i]=start[i]+delta[i]+omaxs[i];
		}
		else {
			check_maxs[i]=start[i]+omaxs[i];
			check_mins[i]=start[i]+delta[i]+omins[i];
		}
		fmins[i]=check_mins[i]+extra_mins[i];
		fmaxs[i]=check_maxs[i]+extra_maxs[i];
		imins[i]=fmins[i];
		imaxs[i]=fmaxs[i];
	}

// Init col brush list
	NumClipBoxes=0;
	NumClipBSPModels=0;
	NumClipBrushes=0;
	NumClipBrushSides=0;


// Add world brushes only if we need bsps & clip
	if ( (flags&PF_TYPE_BSP_MODEL) && (flags&PF_COL_CLIP)) {
		memset(ClipBrushFlag,0,(bsp->NumBrushes+7)>>3);
		check_bsp_model(0,-1);
	}

// Add other objects and bsp brushes
	for(i=0;i<NumObjects;i++) {

	// Do not compare to itself
		if (i==cobj_idx) continue;

	// Get only relevant clip flags
		flag=Prog_GetFlags(i)&(PF_COL_CHECK|PF_COL_CLIP|PF_TYPE_MASK_ALL);
		flag&=flags;

	// We do not support this object type or col flags
		if (flag==0) continue;

	// BSP models (only for clip)
		if ((flag&PF_TYPE_BSP_MODEL)&&(flag&PF_COL_CLIP)) {
			check_bsp_model(Objects[i].pob.frame,i);
		}
	// Models and empty fields
		else {
			check_boundbox(i,flag);
		}
	}
}


void check_boundbox(int obj_idx,int flag) 
{
	ClipBox *clip;
	Object *o=&Objects[obj_idx];
	float *c=o->pob.c;
	int i;

// For check only, consider check_box and bounding box
	if (flag&PF_COL_CHECK) {
		for(i=0;i<3;i++) {
			if (check_mins[i]>o->maxs[i]+c[i]||
			    check_maxs[i]<o->mins[i]+c[i]) 
				return;
		}
		Prog_Event(cobj_idx,PE_COLLISION,obj_idx,0);
		Prog_Event(obj_idx,PE_COLLISION,cobj_idx,0);
		return;
	}


// Check bounding box and speed_box
	for(i=0;i<3;i++) {
		if (fmins[i]>o->maxs[i]+c[i]||fmaxs[i]<o->mins[i]+c[i]) 
			return;
	}

// Setup a new ClipBox
	clip=&ClipBoxes[NumClipBoxes++];
	clip->obj_idx=obj_idx;
	FVec_AddFVec(c,o->mins,clip->mins);
	FVec_AddFVec(c,o->maxs,clip->maxs);
}


void check_bsp_model(int model_idx,int obj_idx) 
{
	dmodel_t *M=&cbsp->Models[model_idx];
	ClipBSPModel *cm;
	int i, num;

// Move speed box by model's -origin
	for(i=0;i<3;i++) {
		imins[i]-=M->origin[i];
		imaxs[i]-=M->origin[i];
	}

// Add all model brushes to the list
	num=NumClipBrushes;

// Recurse into node bound boxes
	check_node_r(M->headnode);

// Move speed box back
	for(i=0;i<3;i++) {
		imins[i]+=M->origin[i];
		imaxs[i]+=M->origin[i];
	}

// Brushes from the model has been added
	if (num<NumClipBrushes) {
		cm=&ClipBSPModels[NumClipBSPModels];
		cm->obj_idx=obj_idx;
		cm->model_idx=model_idx;
		cm->numClipBrushes=NumClipBrushes-num;
		NumClipBSPModels++;
	}
}


void check_node_r(int node_idx)
{
	dnode_t *node=&cbsp->Nodes[node_idx];
	int i;

// Check bounding box
	for(i=0;i<3;i++) {
		if (imins[i]>node->maxs[i]||imaxs[i]<node->mins[i])
			return;
	}

// Descent into the tree
	node_idx=node->children[0];
	if (node_idx<0) check_leaf(-(node_idx+1));
	else check_node_r(node_idx);

	node_idx=node->children[1];
	if (node_idx<0) check_leaf(-(node_idx+1));
	else check_node_r(node_idx);
}

void check_leaf(int leaf_idx)
{
	dleaf_t *leaf=&cbsp->Leaves[leaf_idx];
	int i, n;
	int brush_idx, idx;
	byte bit;

// Check bounding box
	for(i=0;i<3;i++) {
		if (imins[i]>leaf->maxs[i]||imaxs[i]<leaf->mins[i])
			return;
	}

// Mark leaf brushes for col-det
	i=(int)leaf->firstleafbrush;
	n=i+(int)leaf->numleafbrushes;
	while(i<n) {
		brush_idx=cbsp->BrushList[i];
		idx=brush_idx>>3;
		bit=(unsigned char)(1<<(brush_idx&7));
		if ((ClipBrushFlag[idx]&bit)==0) {
			ClipBrushFlag[idx]|=bit;
			ClipBrushes[NumClipBrushes]=(float)brush_idx;
			NumClipBrushes++;
			NumClipBrushSides+=cbsp->Brushes[brush_idx].numsides;
//printf("   %d %d %d\n",brush_idx,cbsp->Brushes[brush_idx].firstside,cbsp->Brushes[brush_idx].numsides);
		}
		i++;
	}
}




static float col_end[3];
static float col_dist;
static int col_bside;
static int col_start_solid;
static int col_object;


void Clip_CheckBrushes(float *start, float *end)
{
	dmodel_t *M;
	ClipBSPModel *cm;
	dbrush_t *brush;
	dbrushside_t *bside;
	dplane_t *plane;
	float p1[3], p2[3], p[3], d1, d2, d;
	float dot1, dot2;
	int i, j, n, num, start_solid;
	word *pClipBrush;


	FVec_Copy(end,col_end);	// end will not be used anywhere after this
	col_dist=1.0;
	col_bside=-1;
	col_object=-1;
	col_start_solid=0;

	cm=ClipBSPModels;
	pClipBrush=ClipBrushes;
	for(n=0;n<NumClipBSPModels;n++,cm++) {
		M=&cbsp->Models[cm->model_idx];

		FVec_SubFVec(start,M->origin,start);
		FVec_SubFVec(col_end,M->origin,col_end);

		for(j=0;j<cm->numClipBrushes;j++,pClipBrush++) {

			d1=0.0; d2=col_dist;
			FVec_Copy(start,p1);
			FVec_Copy(col_end,p2);

			start_solid=1;

			brush=&cbsp->Brushes[*pClipBrush];

			if ((brush->contents&CONTENTS_SOLID)==0) continue;

			bside=&cbsp->BrushSides[brush->firstside];
			num=brush->numsides;

			for(i=0;i<num;i++,bside++) {

				plane=&cbsp->Planes[bside->planenum];

				dot1=FVec_DotProduct(plane->normal,p1)
					-plane->dist-oradius;
				dot2=FVec_DotProduct(plane->normal,p2)
					-plane->dist-oradius;

				if (dot1>=0&&dot2>=0) {
					break;
				}

				if (dot1<0&&dot2<0) {
					continue;
				}

				d = dot1/(dot1-dot2);
				FVec_SubFVec(p2,p1,p);
				FVec_MulScalar(p,d,p);
				FVec_AddFVec(p,p1,p);
				d = d1+(d2-d1)*d;

				if (dot1>=0) {
					FVec_Copy(p,p1); d1=d;
					col_bside=i+brush->firstside;
					start_solid=0;
				}
				else {
					FVec_Copy(p,p2); d2=d;
				}
			}

			if (i==num) {

				if (start_solid) {
					FVec_AddFVec(start,M->origin,start);
					FVec_Copy(start,col_end);
					col_dist=0.0;
					col_bside=-1;
					col_start_solid=1;
					col_object=cm->obj_idx;
					return;
				}

				FVec_Copy(p1,col_end);
				col_dist=d1;
				col_object=cm->obj_idx;
			}
		}

		FVec_AddFVec(start,M->origin,start);
		FVec_AddFVec(col_end,M->origin,col_end);
	}
}

int Clip_MoveSlide(float *pos, float *speed)
{
	dplane_t *plane;
	float safe[3], start[3], end[3], delta[3], v[3], dot;
	int i, clip_flag=0;


	FVec_Copy(pos,start);
	FVec_Copy(start,safe);
	FVec_Copy(speed,delta);
	FVec_AddFVec(start,delta,end);

	for(i=0;i<4;i++) {
		Clip_CheckBrushes(start,end);

		if (col_object>=0) {
			Prog_Event(cobj_idx,PE_COLLISION,col_object,0);
			Prog_Event(col_object,PE_COLLISION,cobj_idx,0);
		}

		if (col_start_solid) {

//			printf("stuck %d %d %d\n",col_object,i,TotalFrames);
//			FVec_Copy(safe,pos);

		// Really stuck
			if (i==0) return 4;

		// We are stuck due to previous slide
			FVec_Copy(safe,start);
			FVec_AddFVec(start,delta,end);
			continue;
		}

	// New safe position (slightly away from the wall)
		FVec_SubFVec(end,start,v);
		if (FVec_Normalize(v)>1.0) {
			FVec_MulScalar(v,0.1,v);
		}
		else {
			FVec_SubFVec(end,start,v);
			FVec_MulScalar(v,0.5,v);
		}
		FVec_SubFVec(col_end,v,safe);
//		FVec_Copy(col_end,safe);

	// If moved all the way, break
		if (col_dist==1.0) break;

	// Find new delta to move along
		plane=&WorldBSP.Planes[WorldBSP.BrushSides[col_bside].planenum];

		if (plane->normal[2]>0.8) clip_flag|=1;
		if (plane->normal[2]==0.0) clip_flag|=2;

		FVec_MulScalar(delta,1.0-col_dist,delta);
		dot=FVec_DotProduct(plane->normal,delta);
		FVec_MulScalar(plane->normal,dot,v);
		FVec_SubFVec(delta,v,delta);

		if (delta[0]>-0.1&&delta[0]<0.1) delta[0]=0.0;
		if (delta[1]>-0.1&&delta[1]<0.1) delta[1]=0.0;
		if (delta[2]>-0.1&&delta[2]<0.1) delta[2]=0.0;

		if (FVec_DotProduct(speed,delta)<0) {
			delta[0]=delta[1]=delta[2]=0;
		}

		FVec_MulScalar(plane->normal,0.1,v);
		FVec_AddFVec(col_end,v,start);
		FVec_AddFVec(start,delta,end);
	}


// Move to the last safe position
//	if (!col_start_solid)
		FVec_Copy(safe,pos);

	return clip_flag;
}



/*
 Moves from start to end in bsp world, Checks for col-det.
 Slides along walls. start is updated to the new position
 Returns: true for free fall
*/
int ClipMovePlayer(BSP */*bsp*/, int obj_idx, float *speed)
{
	Object *o=&Objects[obj_idx];
	float extra_mins[3]={-10,-10,-10};	// Because of possible slide
	float extra_maxs[3]={10,10,40};		//   and stairs checks
	float start[3], start2[3];
	int clip_flag, temp;

//	count=RDTSC();

	FVec_Copy(o->pob.c,start);
	FVec_Copy(o->pob.c,start2);

// Prepare for col-det
	Clip_Begin(&WorldBSP,obj_idx,start,speed,extra_mins,extra_maxs,
		PF_TYPE_MASK_ALL|PF_COL_CHECK|PF_COL_CLIP);

// Do move slide
	clip_flag=Clip_MoveSlide(start,speed);
	FVec_Copy(start,o->pob.c);

// We had a possible step ahead (wall and ground)
	if ((clip_flag&3)==3) {
		start2[2]+=30;
		speed[2]-=30;
		temp=Clip_MoveSlide(start2,speed);
		speed[2]+=30;

	// Step successfull (not start_solid and ground after)
		if (temp!=4&&(temp&1)==1) {
			clip_flag=temp;
			FVec_Copy(start2,o->pob.c);
		}
	}

	return ((clip_flag&1)==0);
}





int CheckRay(BSP */*bsp*/,float */*start*/,float */*dir*/,int /*skip_obj*/)
{
	return -1;
}


/*
 Moves from start to end in bsp world, Checks for col-det.
 start is updated to the new position
*/
void ClipMoveProjectile(BSP */*bsp*/,int obj_idx,float *delta,int /*skip_idx*/)
{
	float *start, end[3];
	float zeros[3]={0,0,0};
//	float extra_mins[3]={-5,-5,-5}, extra_maxs[3]={5,5,5};
//	int i;

	start=Objects[obj_idx].pob.c;
	FVec_AddFVec(start,delta,end);

// Prepare for col-det
	Clip_Begin(&WorldBSP,obj_idx,start,delta,zeros,zeros, //extra_mins,extra_maxs,
		PF_TYPE_MASK_ALL|PF_COL_CLIP);

	Clip_CheckBrushes(start,end);

	FVec_Copy(col_end,start);

// Call collision code
	if (col_start_solid||col_dist<1.0) {
		Prog_Event(obj_idx,PE_COLLISION,col_object,0);
		if (col_object>=0)
			Prog_Event(col_object,PE_COLLISION,obj_idx,0);
	}

	return;
}



/*
 Here we just have to check if some objects became stuck in the
 model after its move. If so, either push objects or block bsp move.
*/
void ClipMoveBSPModel(BSP *bsp,int obj_idx,float *delta,int do_push)
{
	Object *model_object=&Objects[obj_idx], *o;
	dmodel_t *bspmodel=&bsp->Models[model_object->pob.frame];
	float mmins[3], mmaxs[3], *c;
	int flags,i;

// Move the model to the new location
	FVec_AddFVec(bspmodel->origin,delta,bspmodel->origin);
	FVec_AddFVec(model_object->pob.c,delta,model_object->pob.c);

// Find abs bound box for the model
	FVec_AddFVec(bspmodel->mins,bspmodel->origin,mmins);
	FVec_AddFVec(bspmodel->maxs,bspmodel->origin,mmaxs);

// Basicly the same stuff as Clip_Begin, just from the other side
	cbsp=bsp;

// Init col brush list
	NumClipBoxes=0;
	NumClipBSPModels=0;
	NumClipBrushes=0;
	NumClipBrushSides=0;
	memset(ClipBrushFlag,0,(bsp->NumBrushes+7)>>3);

// Simulate speed_box - integers (the same size as model)
	for(i=0;i<3;i++) {
		imins[i]=mmins[i];
		imaxs[i]=mmaxs[i];
	}

// Add brushes from the model to the list
	check_bsp_model(model_object->pob.frame,obj_idx);


// Go thru all the non-bsp objects and push 'em
	o=Objects;
	for(i=0;i<NumObjects;i++,o++) {

	// If it's a bsp or does not do real clip, go to another object
		flags=Prog_GetFlags(i);
		if ((flags&PF_TYPE_BSP_MODEL)||
		    (flags&PF_COL_CLIP)==0) continue;

		c=o->pob.c;

	// Check if object bound box intersects with model's bound box
		if (mmins[0]>o->maxs[0]+c[0]||
		    mmaxs[0]<o->mins[0]+c[0]) continue;
		if (mmins[1]>o->maxs[1]+c[1]||
		    mmaxs[1]<o->mins[1]+c[1]) continue;
		if (mmins[2]>o->maxs[2]+c[2]||
		    mmaxs[2]<o->mins[2]+c[2]) continue;

	// Setup object info for the test
		cobj_idx=i;
		omins=o->mins;
		omaxs=o->maxs;
		oradius=(omaxs[0]-omins[0])*0.5;

	// Check object for collision with bsp model
		Clip_CheckBrushes(c,c);

	// Object got stuck after this bsp model has moved, do something
		if (col_start_solid) {
		// Push the object
			if (do_push) {
				FVec_AddFVec(c,delta,c);
			}
		// Block model move
			else {
				FVec_SubFVec(bspmodel->origin,delta,bspmodel->origin);
				FVec_SubFVec(model_object->pob.c,delta,model_object->pob.c);
				return;
			}
		}
	}
}

