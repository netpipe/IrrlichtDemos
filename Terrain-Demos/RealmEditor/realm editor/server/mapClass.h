//***************************************************************************************
// MAP CLASS
//***************************************************************************************
class mapClass
{
public:
int id, numPlayers, weatherId;
int* locations, spawnPoints;
vector <int> players;
void addPlayer(int socket);
void removePlayer(int socket);
mapClass();
};

mapClass::mapClass()
{
	//start with things set to 0
	id=0; 
	numPlayers=0; 
	weatherId=0;
	players.resize(0);
	printf("%i: ", numPlayers);
}
void mapClass::addPlayer(int socket)
{
	//pushes player socket into list and increments number of players
	players.push_back(socket);
	numPlayers++;
}
void mapClass::removePlayer(int socket)
{
	//remove the given socket
	//make dummy player list
	vector<int> newplayers(0);
	//loop through players
	for(int x=0;x<players.size();x++)
	{
		//if not the socket we are removing
		if(players[x]!=socket)
		{
			//add to dummy list
			newplayers.push_back(players[x]);
		}
	}
	//set players equal to dummy list which now has socket removed
	players=newplayers;
	//decrement number of players on maps
	numPlayers--;
}

//init global map class 3 way vector class (quite confusing yes...)
//basically allows for infinite number of maps, [x][y][z]
vector<vector<vector<mapClass> > > virtualMapSystem;
int middleMapX=0,middleMapY=0,middleMapZ=0;
