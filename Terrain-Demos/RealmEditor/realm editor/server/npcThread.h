//declare thread
pthread_t npcThread;
//**************************
//Handle for NPC updaters***
//**************************
void *HandleNPCUpdates(void *ptr)
{
	//init some vars
	int counter=0;
	double time=0, oldtime=0;
	timeval newTime;
	gettimeofday(&newTime, NULL);
	oldtime= ((.001*newTime.tv_usec)+(newTime.tv_sec*1000));
	time=oldtime;
	//loop while server is running
	while(runServer)
	{
		//reset old time
		oldtime=time;
		//make sure its been atleast 50 ms
		while(time-oldtime<50)
		{
			//get new time
			gettimeofday(&newTime, NULL);
			time= ((.001*newTime.tv_usec)+(newTime.tv_sec*1000));
		}
		//loop through npcs and update them
		for(counter=0;counter<npcsystem.size();counter++)
		{
			//update each npc
			npcsystem[counter].update();
		}
	}
	//exit thread
	pthread_exit(NULL);
}
