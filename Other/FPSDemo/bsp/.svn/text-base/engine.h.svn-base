#ifndef _ENGINE_H_
#define _ENGINE_H_

//#include <windows.h>
#include <stdio.h>
//#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FALSE           	0
#define TRUE            	1
#define PI			3.141592f
#define TIMER_SPEED		100
#define MAX_VIS_OBJECTS		256
#define MAX_VIS_DLIGHTS		16
#define MAX_VIS_SOUNDS		8

typedef unsigned char   byte;
typedef unsigned char   BYTE;
typedef unsigned short  word;
typedef unsigned short  WORD;
typedef unsigned long   dword;
typedef unsigned long   DWORD;
typedef float           FMatrix[3][3];

int TotalFrames=0;
int TotalTicks=0, FrameTicks=0;
//static int StartTicks=0;

typedef struct {
	int prog_idx;	// PROG index
	float c[3];		// Coordinates of the object
	float angle;	// Angle (yaw)
	float vangle;	// V Angle (pitch) or rotation axis (bspmodel)
	int frame;		// Frame (Model, Sprite) or BSPModel (bspmodel)
	int flags;		// Internal rendering flags (passed to client)
} POB;


typedef struct {
	char prog_name[16];
	int  flags;
	int  (*init)(int obj_idx,char *params);
	int  (*event)(int obj_idx,int event,int param1,int param2);
	void (*process)(POB *pob);
	void (*render)(POB *pob);
	void (*free)(void);
} Prog;


typedef struct {		// Used only in server
	POB pob;
	float speed[4];
	float mins[3], maxs[3];	// bounding box for this object
	int next_frame_tick;
	int is_gravity;
	int leaf_idx;
	int tag;			// Same as 'targetname' in Quake
	int team_next;		// Next object in the team
	byte vars[32];		// Later on there'll be dynamic mem alloc
} Object;


// forward declarationsvoid
void   FindPlane(float *,float *,float *,float *);
int    FindPlaneType(float *);
void   FVec_Copy(float *,float *);
void   FVec_AddFVec(float *,float *,float *);
void   FVec_SubFVec(float *,float *,float *);
float  FVec_DotProduct(float *,float *);
void   FVec_CrossProduct(float *,float *,float *);
void   FVec_MulScalar(float *,float,float *);
float  FVec_Normalize(float *);
void   FMatrix_Identity(FMatrix);
void   FMatrix_Copy(FMatrix,FMatrix);
void   FMatrix_MakeRotation(FMatrix,float *);
void   FMatrix_PreXRot(FMatrix,float);
void   FMatrix_PreYRot(FMatrix,float);
void   FMatrix_PreZRot(FMatrix,float);
void   FMatrix_MulFVec(FMatrix,float *,float *);
void   FMatrix_InvMulFVec(FMatrix,float *,float *);
void   FMatrix_MulFMatrix(FMatrix,FMatrix,FMatrix);
void   FMatrix_InvMulFMatrix(FMatrix,FMatrix,FMatrix);

#endif
