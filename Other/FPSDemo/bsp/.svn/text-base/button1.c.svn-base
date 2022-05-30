/********************************************************************
 BUTTON1.C - PROG for a button (bsp model)
*********************************************************************/


typedef struct {
	short target_tag;
} button_local_vars;


int Button1_Init(int obj_idx,char *params)
{
	Object *o=&Objects[obj_idx];
	button_local_vars *vars=(button_local_vars *)o->vars;
	dmodel_t *bspmodel;
	int model_idx;//,a=0;
	char *val;

	if (!params) return 1;

// Find BSP model idx
	val=Parse_GetKeyValue(params,"model");
	if (!val) return 1;

	sscanf(val+1,"%d",&model_idx);
	if (model_idx<1||model_idx>=WorldBSP.NumModels) return 1;

// Setup bsp model info
	o->pob.frame=model_idx;
	bspmodel=&WorldBSP.Models[model_idx];
	WorldBSP.ModelObjectIndex[model_idx]=obj_idx;
	FVec_Copy(bspmodel->origin,o->pob.c);
	FVec_Copy(bspmodel->mins,o->mins);
	FVec_Copy(bspmodel->maxs,o->maxs);

// Setup target (which tag will be activated by button)
	val=Parse_GetKeyValue(params,"target");
	if (val) vars->target_tag=(short)Tag_Register(val);
	else vars->target_tag=-1;

	return 0;
}


void Button1_Collision(int obj_idx,int /*obj_other*/)
{
	Object *o=&Objects[obj_idx];
	button_local_vars *vars=(button_local_vars *)o->vars;

	if (vars->target_tag>=0)
		Tag_Activate(vars->target_tag,0);
}

int Button1_Event(int obj_idx, int event, int param1, int /*param2*/)
{
	switch(event) {
		case PE_COLLISION:
			Button1_Collision(obj_idx,param1);
			break;
	}

	return 0;
}

void Button1_Process(POB */*pob*/) {}

void Button1_Render(POB *pob)
{
	/*render->*/GL_DrawBSPModel(pob);
}


void Button1_Free(void) {}


/********************************************************************
			REGISTRATION
*********************************************************************/

void Button1_Register(Prog *p)
{
	strcpy(p->prog_name,"func_button");
	p->flags=	PF_TYPE_BSP_MODEL | PF_COL_CLIP;
	p->init=	Button1_Init;
	p->event=	Button1_Event;
	p->free=	Button1_Free;
	p->process=	Button1_Process;
	p->render=	Button1_Render;
}
