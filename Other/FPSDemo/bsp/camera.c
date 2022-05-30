#include "camera.h"

/* 
 Initialize camera
*/
void Camera_Init(Camera *C, float fov,int w,int h,int x,int y)
{
	float s,c,temp[3]={0,0,0};

	C->width=w; C->height=h;
	C->sx=x; C->sy=y;

	C->mid_x=(float)(w>>1)-0.5f;
	C->mid_y=(float)(h>>1)-0.5f;

	s=(float)sin(fov*0.5f);
	c=(float)cos(fov*0.5f);

// Make projection constants
	C->proj=(c*(float)(w>>1))/s;

// Projection ratio (1.0 for square pixels)
	C->proj_ratio=(float)w*0.75f;
	C->proj_ratio=(float)h/C->proj_ratio;

// Init coords and rotate
	FVec_Copy(C->c,temp);
	FMatrix_Identity(C->ToWorld);
	C->fov=fov;
}

