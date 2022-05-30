/********************************************************************
 DOOR1.C - A door (bsp model)
*********************************************************************/


#define MODE_STOP	0
#define MODE_MOVE	1
#define MODE_WAIT	2
#define MODE_RETURN	3
#define MODE_FINISHED	4


typedef struct {
	byte mode;		// Current mode
	byte dir;		// Dir to move (forward)
	signed char wait;	// Wait before return (in seconds)
	byte dummy;
	float speed;		// Speed of move (per tick)
	short time_count;	// Counter for an action
	short total_time;	// Time to do full movement
	short target_tag;
} door_local_vars;


static float door_move[][3]={
	{ 1, 0, 0},	// 0
	{ 0, 1, 0},	// 90
	{-1, 0, 0},	// 180
	{ 0,-1, 0},	// 270
	{ 0, 0, 1},	// up
	{ 0, 0,-1},	// down
};



int Door1_Init(int obj_idx,char *params)
{
	Object *o=&Objects[obj_idx];
	door_local_vars *vars=(door_local_vars *)o->vars;
	dmodel_t *bspmodel;
	int model_idx,a=0;
	float dist;
	char *val;

// No params - should not happen
	if (!params) return 1;

// Find BSP model idx
	val=Parse_GetKeyValue(params,"model");
	if (!val) return 1;

	sscanf(val+1,"%d",&model_idx);
	if (model_idx<1||model_idx>=WorldBSP.NumModels) return 1;

// Setup bsp model info
	o->pob.frame=model_idx;
	WorldBSP.ModelObjectIndex[model_idx]=obj_idx;
	bspmodel=&WorldBSP.Models[model_idx];
	FVec_Copy(bspmodel->origin,o->pob.c);
	FVec_Copy(bspmodel->mins,o->mins);
	FVec_Copy(bspmodel->maxs,o->maxs);

// Setup move direction (forward)
	val=Parse_GetKeyValue(params,"angle");
	if (val) sscanf(val,"%d",&a);

	switch(a) {
		default:
		case 0:
			vars->dir=0;
			dist=bspmodel->maxs[0]-bspmodel->mins[0];
			break;
		case 90:
			vars->dir=1;
			dist=bspmodel->maxs[1]-bspmodel->mins[1];
			break;
		case 180:
			vars->dir=2;
			dist=bspmodel->maxs[0]-bspmodel->mins[0];
			break;
		case 270:
			vars->dir=3;
			dist=bspmodel->maxs[1]-bspmodel->mins[1];
			break;
		case -1:
			vars->dir=4;
			dist=bspmodel->maxs[2]-bspmodel->mins[2];
			break;
		case -2:
			vars->dir=5;
			dist=bspmodel->maxs[2]-bspmodel->mins[2];
			break;
	}

// Move speed (per tick)
	val=Parse_GetKeyValue(params,"speed");
	if (val) sscanf(val,"%f",&vars->speed);
	else vars->speed=200.0;
	vars->speed/=(float)TIMER_SPEED;

// Total time (in ticks) for full move
	vars->total_time=((int)dist)/vars->speed;

// Wait time (in seconds)
	val=Parse_GetKeyValue(params,"wait");
	if (val) sscanf(val,"%d",&a);
	else a=3;
	vars->wait=(signed char)a;

// Setup target-name (tag by which the door will be open)
	val=Parse_GetKeyValue(params,"targetname");
	if (val) o->tag=Tag_Register(val);

// Setup target (tag that will be activated by this door)
	val=Parse_GetKeyValue(params,"target");
	if (val) vars->target_tag=(short)Tag_Register(val);
	else vars->target_tag=-1;

// Setup the team
	val=Parse_GetKeyValue(params,"team");
	if (val) Team_AddObject(val,obj_idx);

// The default mode is MODE_STOP
	vars->mode=MODE_STOP;
	vars->time_count=0;

//	printf("door: %d %d %d\n",a,vars->dir,o->tag);
	return 0;
}

void Door1_Collision(int obj_idx,int /*obj_other*/)
{
	Object *o=&Objects[obj_idx];
	door_local_vars *vars=(door_local_vars *)o->vars;

	if (vars->mode==MODE_STOP) {
		if (o->tag<0) {
			vars->mode=MODE_MOVE;
			vars->time_count=vars->total_time;
			Team_Notify(obj_idx,1);
			Tag_Activate(vars->target_tag,1);
		}
	}

// !!! Check for head on collisions !!!
}


void Door1_Update(int obj_idx) 
{
	Object *o=&Objects[obj_idx];
	door_local_vars *vars=(door_local_vars *)o->vars;
	float delta[3], dist;
	int ticks=0;

	if (vars->mode==MODE_MOVE) {
		ticks=FrameTicks;
		vars->time_count-=(short)ticks;
		if (vars->time_count<=0) {
			ticks+=vars->time_count;
			if (vars->wait<0) {
				vars->mode=MODE_FINISHED;
				vars->time_count=0;
			}
			else {
				vars->mode=MODE_WAIT;
				vars->time_count=vars->wait*TIMER_SPEED;
			}
		}
	}
	else if (vars->mode==MODE_RETURN) {
		ticks=FrameTicks;
		vars->time_count-=(short)ticks;
		if (vars->time_count<=0) {
			ticks+=vars->time_count;
			vars->mode=MODE_STOP;
			vars->time_count=0;
			Tag_Activate(vars->target_tag,0);
		}
		ticks=-ticks;
	}
	else if (vars->mode==MODE_WAIT) {
		vars->time_count-=(short)FrameTicks;
		if (vars->time_count<=0) {
			//ticks=vars->time_count;
			vars->mode=MODE_RETURN;
			vars->time_count=vars->total_time;
		}
	}

	if (ticks) {
		dist=(float)(ticks*vars->speed);
		FVec_MulScalar(door_move[vars->dir],dist,delta);
		ClipMoveBSPModel(&WorldBSP,obj_idx,delta,vars->dir==4||vars->dir==5);
		World_ObjectMoved(obj_idx);
	}
}

int Door1_Event(int obj_idx, int event, int param1, int /*param2*/)
{
	Object *o=&Objects[obj_idx];
	door_local_vars *vars=(door_local_vars *)o->vars;

	switch(event) {
		case PE_TEAM:
		case PE_TAG: 
			if (vars->mode==MODE_STOP) {
				vars->mode=MODE_MOVE;
				vars->time_count=vars->total_time;
				if (event!=PE_TEAM) 
					Team_Notify(obj_idx,1);
				Tag_Activate(vars->target_tag,1);
			}
			break;
		case PE_COLLISION:
			Door1_Collision(obj_idx,param1);
			break;
		case PE_UPDATE:
			Door1_Update(obj_idx);
			break;
	}

	return 0;
}



void Door1_Process(POB */*pob*/) {}

void Door1_Render(POB *pob)
{
	GL_DrawBSPModel(pob);
}

void Door1_Free(void) {}


/********************************************************************
			REGISTRATION
*********************************************************************/

void Door1_Register(Prog *p)
{
	strcpy(p->prog_name,"func_door");
	p->flags=	PF_TYPE_BSP_MODEL | PF_COL_CLIP;
	p->init=	Door1_Init;
	p->event=	Door1_Event;
	p->process=	Door1_Process;
	p->render=	Door1_Render;
	p->free=	Door1_Free;
}
