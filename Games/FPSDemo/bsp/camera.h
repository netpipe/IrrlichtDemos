#define MIN_Z		4.0

typedef struct {
	float c[3];
	float pitch, roll, yaw;
	FMatrix ToWorld;
	float fov;
	int sx, sy, width, height;
	float mid_x, mid_y;
	float proj, proj_ratio;
} Camera;

void Camera_Init(Camera *,float,int,int,int,int);
