/********************************************************************
 PROG.C - Support code for PROGS
*********************************************************************/

	
#include "engine.h"

// Registration functions. They must be called in InitProgs()
//void Player_Register(Prog *);
//void ExtCamera_Register(Prog *);
//void Missile_Register(Prog *);
//void Explosion_Register(Prog *);
void Door1_Register(Prog *);
//void Rotate_Register(Prog *);
void Button1_Register(Prog *);
//void Portal_Register(Prog *);
//void Test_Register(Prog *);

#define MAX_PROGS	1024

// All the objects in the world
extern Object Objects[MAX_OBJECTS];
extern int NumObjects;

static Prog Progs[MAX_PROGS];
static int register_counter=0;

/********************************************************************
			INTERFACE TO OBJECT PROGRAMS
*********************************************************************/


Prog *Prog_Alloc(void)
{
	register_counter++;
	return(&Progs[register_counter-1]);
}

void InitProgs(void)
{
//	Player_Register(Prog_Alloc());
//	ExtCamera_Register(Prog_Alloc());
//	Missile_Register(Prog_Alloc());
//	Explosion_Register(Prog_Alloc());
	Door1_Register(Prog_Alloc());
//	Rotate_Register(Prog_Alloc());
	Button1_Register(Prog_Alloc());
//	Portal_Register(Prog_Alloc());
//	Test_Register(Prog_Alloc());

/* Add your progs after this line */

}

void FreeProgs() 
{
	int i;
	for(i=0;i<register_counter;i++) {
		Progs[i].free();
	}
}

int Prog_GetProgByName(char *prog_name)
{
	int i;

	for(i=0;i<register_counter;i++) {
		if (strcmp(Progs[i].prog_name,prog_name)==0)
			return(i);
	}
	return -1;
}

int Prog_GetFlags(int obj_idx)
{
	return(Progs[Objects[obj_idx].pob.prog_idx].flags);
}

int Prog_GetProgFlags(int prog_idx)
{
	return(Progs[prog_idx].flags);
}


/*
 Server side PROG functions.
*/
int Prog_Init(int obj_idx, char *params)
{
	return(Progs[Objects[obj_idx].pob.prog_idx].init(obj_idx,params));
}

int Prog_Event(int obj_idx,int event,int param1,int param2)
{
	return(Progs[Objects[obj_idx].pob.prog_idx].event(obj_idx,event,param1,param2));
}


/*
 Client side PROG functions
*/

void Prog_Process(POB *pob) 
{
	Progs[pob->prog_idx].process(pob);
}

void Prog_Render(POB *pob) 
{
	Progs[pob->prog_idx].render(pob);
}




/********************************************************************
			TAG SUPPORT STUFF
*********************************************************************/


static char TagList[1024][16];
static int  NumTags=0;

int Tag_Register(char *tag_str)
{
	int i;

	for(i=0;i<NumTags;i++) {
		if (strcmp(TagList[i],tag_str)==0) 
			return i;
	}
	strcpy(TagList[NumTags],tag_str);
	NumTags++;
	return(NumTags-1);
}

void Tag_Activate(int tag_idx, int tag_param) 
{
	int i;

	if (tag_idx<0) return;

	for(i=0;i<NumObjects;i++) {
		if (Objects[i].tag==tag_idx)
			Prog_Event(i,PE_TAG,tag_idx,tag_param);
	}
}

/********************************************************************
			TEAM SUPPORT STUFF
*********************************************************************/


static char TeamList[1024][16];
static int  TeamHead[1024];
static int  NumTeams=0;

void Team_AddObject(char *team_str, int obj_idx)
{
	Object *head;
	int i;

// Maybe disconnect it from prev team???
	if (Objects[obj_idx].team_next>=0) return;

	for(i=0;i<NumTeams;i++) {
		if (strcmp(TeamList[i],team_str)==0) 
			break;
	}
	if (i==NumTeams) {
		strcpy(TeamList[NumTeams],team_str);
		TeamHead[NumTeams]=obj_idx;
		Objects[obj_idx].team_next=obj_idx;
		NumTeams++;
	}
	head=&Objects[TeamHead[i]];
	Objects[obj_idx].team_next=head->team_next;
	head->team_next=obj_idx;
}

void Team_RemoveObject(int obj_idx)
{
	int idx, next, i;

	idx=Objects[obj_idx].team_next;

	while(idx>=0&&idx!=obj_idx) {
		next=Objects[idx].team_next;
		if (next==obj_idx) {
			Objects[idx].team_next=Objects[obj_idx].team_next;
			Objects[obj_idx].team_next=-1;
			for(i=0;i<NumTeams;i++) {
				if (TeamHead[i]==obj_idx) {
					TeamHead[i]=idx;
					break;
				}
			}
			break;
		}
		idx=next;
	}
}

void Team_Notify(int src_idx, int notify_param) 
{
	int idx;

	idx=Objects[src_idx].team_next;

	while(idx>=0&&idx!=src_idx) {
		Prog_Event(idx,PE_TEAM,src_idx,notify_param);
		idx=Objects[idx].team_next;
	}
}


/********************************************************************
			OBJ LISTENERS
*********************************************************************/

struct {
	int listener, target;
} Watches[1024];

static int NumWatches=0;


void Watch_Register(int listener, int target)
{
	if (listener<0||target<0) return;

	Watches[NumWatches].listener=listener;
	Watches[NumWatches].target=target;
	NumWatches++;
}

void Watch_Notify(int old_idx, int new_idx) 
{
	int i;

	if (new_idx<0) {
		for(i=0;i<NumWatches;i++) {
		// If old_idx was a target, remove the watch
			if (Watches[i].target==old_idx) {
				Prog_Event(Watches[i].listener,PE_OBJ_WATCH,old_idx,new_idx);
				Watches[i]=Watches[NumWatches-1];
				NumWatches--; i--;
			}
		// If old_idx was a listener, remove the watch
			if (Watches[i].listener==old_idx) {
				Watches[i]=Watches[NumWatches-1];
				NumWatches--; i--;
			}
		}
	}
	else {
		for(i=0;i<NumWatches;i++) {
		// Change the watch if old_idx was a listener
			if (Watches[i].listener==old_idx)
				Watches[i].listener=new_idx;
		// Notify the listener if old_idx was a target
			if (Watches[i].target==old_idx) {
				Watches[i].target=new_idx;
				Prog_Event(Watches[i].listener,PE_OBJ_WATCH,old_idx,new_idx);
			}
		}
	}
}
