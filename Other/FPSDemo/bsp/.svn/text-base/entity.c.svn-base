/********************************************************************
 ENTITY.C - Entity management
*********************************************************************/

extern BSP WorldBSP;
extern char WorldBSPName[64];
extern Object Objects[MAX_OBJECTS];
extern int NumObjects;

char *Parse_GetKeyValue(char *obj_params, char *key);
int World_AddObject(int prog_idx,char *params);
void World_ObjectMoved(int obj_idx);


void Entity_NewWorld(BSP *bsp)
{
	char *obj_params, *prog_name;
	int i, prog_idx, idx;

// Init the entities
	for(i=0;i<bsp->NumEntities;i++) {
		obj_params=bsp->Entities[i];
		prog_name=Parse_GetKeyValue(obj_params,"classname");

	// Got classname for the entity
		if (prog_name) {
			prog_idx=Prog_GetProgByName(prog_name);

		// We support the entity
			if (prog_idx>=0) {
				idx=World_AddObject(prog_idx,obj_params);
				World_ObjectMoved(idx);
			}
		}
	}
}


void World_ObjectMoved(int obj_idx)
{
	Object *o=&Objects[obj_idx];
	float coords[3];
	int idx;

	if (Prog_GetFlags(obj_idx)&PF_TYPE_BSP_MODEL) {
		FVec_AddFVec(WorldBSP.Models[o->pob.frame].maxs,
			WorldBSP.Models[o->pob.frame].mins,coords);
		FVec_MulScalar(coords,0.5,coords);
		FVec_AddFVec(coords,WorldBSP.Models[o->pob.frame].origin /*o->pob.c*/,coords);

		idx=BSP_FindLeaf(&WorldBSP,coords);
	}
	else {
		idx=BSP_FindLeaf(&WorldBSP,o->pob.c);
	}

	if (idx>=0) o->leaf_idx=idx;
}


int World_AddObject(int prog_idx,char *params)
{
	Object *o=&Objects[NumObjects];
	int i;

	o->pob.prog_idx=prog_idx;
	for(i=0;i<3;i++) o->pob.c[i]=0;
	o->pob.angle=0;
	o->pob.vangle=0;
	o->pob.frame=0;
	o->pob.flags=0;
	for(i=0;i<4;i++) o->speed[i]=0;
	for(i=0;i<3;i++) o->mins[i]=o->maxs[i]=0;

	o->is_gravity=0;
	o->tag=-1;
	o->team_next=-1;

	NumObjects++;

	if (Prog_Init(NumObjects-1,params)) {
		NumObjects--;
		return -1;
	}

	return(NumObjects-1);
}


void World_RemoveObject(int idx)
{
	if (idx<0||idx>=NumObjects) return;

// Dec number of objects
	NumObjects--;
	Objects[idx]=Objects[NumObjects];

// Notify the watches
	Watch_Notify(idx,-1);
	Watch_Notify(NumObjects,idx);
}


void Entity_PlayerStartCoords(BSP *bsp,float *p)
{
	char *prog_name, *origin;
	int i;

	for(i=0;i<bsp->NumEntities;i++) {
		prog_name=Parse_GetKeyValue(bsp->Entities[i],"classname");
		if (prog_name&&strcmp(prog_name,"info_player_start")==0) {
			origin=Parse_GetKeyValue(bsp->Entities[i],"origin");
			sscanf(origin,"%f %f %f",p,p+1,p+2);
			p[2]+=0.01;
			return;
		}
	}
	for (i=0;i<3;i++) p[i]=0;
}





/********************************************************************
  Quake entity lexical scanner
*********************************************************************/

static char buf_value[1024];


char *GetToken(char *start_ptr,char *token);


/*
 Returns: value as string or NULL if key is not found
*/
char *Parse_GetKeyValue(char *obj_params, char *key)
{
	char *ptr;
	char buf_key[32];

	ptr=obj_params;
	while(ptr) {
		ptr=GetToken(ptr,buf_key);
		if (ptr) {
			ptr=GetToken(ptr,buf_value);
			if (strcmp(key,buf_key)==0)
				return buf_value;
		}
	}
	return NULL;
}


/*
 Returns ptr to the next char after the token.
 If '}' is found, returns NULL.
*/
char *GetToken(char *start_ptr,char *token)
{
	char c;

// Find first  '"'
	while((*start_ptr)!='\"'&&(*start_ptr)!='}'&&(*start_ptr)!='\0')
		start_ptr++;
	if (*start_ptr == '}' || *start_ptr == '\0' )
        return NULL;

// Advance ptr past '"'
	start_ptr++;

// Copy token from buffer to 'token' until '"' or '}' or NULL
	while(1) {
		c=*start_ptr; start_ptr++;
		if (c=='"') break;
		if (c=='}'||c=='\0') {
			start_ptr=NULL;
			break;
		}
		*token=c;
		token++;
	}
	*token='\0';
	return(start_ptr);
}