// Q2View (Quake2 BSP viewer using OpenGL/Win32)
// Alan Baylis 2001

// BSP source adapted from Alexey Goloshubin's Poly Engine (http://www.ii.uib.no/~alexey/poly/)

#define PF_TYPE_NONE		1
#define PF_TYPE_BSP_MODEL	2
#define PF_TYPE_MODEL		4
#define PF_TYPE_SPRITE		8
#define PF_TYPE_MASK_ALL	15
#define PF_COL_CHECK		32	// check box and pass (don't change)
#define PF_COL_CLIP		64	// check and clip (don't change)
#define PE_UPDATE		1	// called each frame
#define PE_COLLISION		2	// param1 - other_obj
#define PE_HURT			3	// param1 - attacker, param2 - damage
#define PE_ATTACK		4	// attack (player only)
#define PE_DEATH		5	// die (player only)
#define PE_TAG			6	// param1 - tag_idx, param2 - tag_param
#define PE_TEAM			7	// param1 - src_idx, param2 - notify_param
#define PE_OBJ_WATCH		8	// param1 - old_idx, param2 - new_idx
#define MAX_OBJECTS		2048
#define MAX_PLAYERS		16
#define MAX_PROGS		1024
#define R_TEXTURES_POW2		1
#define R_TEXTURES_32BIT	2
#define MAX_VIS_OBJECTS		256
#define MAX_VIS_DLIGHTS		16
#define MAX_VIS_SOUNDS		8
#define MAX_VIS_OBJECTS		256
#define MAX_VIS_DLIGHTS		16
#define MAX_VIS_SOUNDS		8

#include "bsp/float.c"
#include "bsp/pcx.c"
#include "bsp/datfile.c"
#include "bsp/camera.c"
#include "bsp/qmdl.h"
#include "bsp/model.h"
#include "bsp/qbsp.h"
#include "bsp/mybsp.h"
#include "bsp/bsp.c"
#include "bsp/pal.c"
#include "bsp/prog.c"
#include "bsp/entity.c"
#include "bsp/engine.h"
#include "bsp/coldet.c"
#include "bsp/rogl.h"
#include "bsp/r_bsp.c"
#include "bsp/tex.c"
#include "bsp/render.c"
#include "bsp/button1.c"
#include "bsp/door1.c"

typedef struct
{
	float cam_pos[3];
	float cam_angle;
	float head_yaw, head_pitch, head_roll;
	float body_yaw, body_roll;
	float cam_height;
	int player_action;
	int player_flags;
	int player_req;
	int req;
	BSP *MainBSP; // The main BSP to render
	POB VisObjects[MAX_VIS_OBJECTS]; // Visible objects
	int NumVisObjects;
} ClientFrame;

ClientFrame *cl = new ClientFrame;
char pakPath[64];
int ShowObject = 1;
int ShowLightmap = 0;
BSP *cbsp=0;
BSP WorldBSP;
BSP *MainBSP;
char WorldBSPName[64];
char PakName[64];
char PalName[64];
Object Objects[MAX_OBJECTS];
int NumObjects=0;
float cam_pos[3];
float cam_angle;
float head_yaw, head_pitch, head_roll;
float body_yaw, body_roll;
float cam_height;
int player_action;
int player_flags;
int player_req;
int req;
FMatrix con;
POB VisObjects[MAX_VIS_OBJECTS];
int NumVisObjects;
static Camera MainCamera;

int LoadWorld(int Width, int Height)
{
	int i, loaded;
	for(i=0;i<NumObjects;i++)
		Objects[i].pob.flags=0;
	cbsp = &WorldBSP;
	BSP_Free(&WorldBSP);
    	if (!DAT_Open(PakName))
                return 0;
    	loaded = BSP_Load(&WorldBSP, WorldBSPName);
    	if(!loaded)
		return 0;
	MainBSP = &WorldBSP;
	Camera *C=&MainCamera;
	Camera_Init(C,PI/2.0,Width,Height,0,0);
	ccam = C;
	cbsp=&WorldBSP;
	loaded = InitPalette(PalName);
    	if(!loaded)
		return 0;
	TexPalette=RGBPalette;
	GL_Init(Width, Height, 0);
	InitProgs();
	Entity_NewWorld(&WorldBSP);
        return 1;
}

void GetStartPosition(GLfloat* camerapos)
{
	Entity_PlayerStartCoords(&WorldBSP, camerapos);
}

GLvoid DrawWorld(int Width, int Height, GLfloat* camerapos)
{
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glCullFace(GL_FRONT);

	ccam->c[0] = camerapos[0];
	ccam->c[1] = camerapos[1];
	ccam->c[2] = camerapos[2];
	GL_BeginBSP(&WorldBSP);
	Object *cam=NULL, *src;
	POB *pob;
	POB *dest;
	BSP *bsp=&WorldBSP;
	int i,type, cam_idx, c;//, vol, pan;
	cl->cam_pos[0] = camerapos[0];
	cl->cam_pos[1] = camerapos[1];
	cl->cam_pos[2] = camerapos[2];
	cl->cam_angle=0;
	cl->body_roll=0;
	cl->cam_height=100;
	cl->player_flags=0;
	cl->head_yaw=0;
	cl->head_pitch=0;
	cl->head_roll=0;
	cl->NumVisObjects=0;
	src=Objects; dest=cl->VisObjects;

	if(ShowObject)
	{
		for(i=0;i<NumObjects;i++,src++)
		{
			c=WorldBSP.Leaves[src->leaf_idx].cluster;
	 		if (/*i==cam_idx||*/c<0)
				continue;
			type=Prog_GetFlags(i);
			if (type&PF_TYPE_NONE)
				continue;
			if (vis.VisClusters[c>>3]&(1<<(c&7)))
			{
				*dest=src->pob;
				dest++;
				cl->NumVisObjects++;
				if (cl->NumVisObjects>=MAX_VIS_OBJECTS) break;
			}
		}

        	// Pre-process all visible objects
	    	pob=cl->VisObjects;
	    	for(i=0;i<cl->NumVisObjects;i++,pob++)
        	{
			Prog_Process(pob);
		}
        	// Render all visible bsp models
	    	pob=cl->VisObjects;
		for(i=0;i<cl->NumVisObjects;i++,pob++)
		{
			if (Prog_GetProgFlags(pob->prog_idx)&PF_TYPE_BSP_MODEL)
			{
				Prog_Render(pob);
			}
		}
	}
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glCullFace(GL_BACK);
}

