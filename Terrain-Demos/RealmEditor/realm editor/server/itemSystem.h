//***********************************
//controls items
//***********************************
class itemSystem
{
public:
int findItem(int uid);
itemClass getData(int position);
void sendItem(int uid, int sock);
void update();
private:
vector<itemClass> items;
};

itemClass itemSystem::getData(int position)
{
	//returns the item in position
	return items[position];
}

int itemSystem::findItem(int uid)
{
	//find item in the vector of items
	int x=-1;
	//loop through items
	for(x=0;x<items.size();x++)
	{
		//if item id is the id given to function
		if(items[x].getId()==uid)
		{
			//return the position
			return x;
		}
	}
	//item does not exist in list add new item and refind
	cout <<"new item"<<endl;
	//make new item
	itemClass newItem;
	//load item from id given
	newItem.loadFromFile(uid);
	//add it to the list
	items.push_back(newItem);
	//loop through items from back to front
	for(x=items.size();x>0;x--)
	{
		//if item id is same as id given
		if(items[x-1].getId()==uid)
		{
			//return the position
			return x-1;
		}
	}
	//big issue return -1
	return -1;
}

void itemSystem::sendItem(int uid, int sock)
{
	//send item to sock
	//find item location
	int loc=findItem(uid);
	//send item in loc to sock
	items[loc].sendData(sock);
}

void itemSystem::update()
{
	//get time
	timeval newTime;
	gettimeofday(&newTime, NULL);
	double time= ((.001*newTime.tv_usec)+(newTime.tv_sec*1000));
	//loop through items
	for(int x=0;x<items.size();x++)
	{
		//if last time item was used is more than 10 mins
		if(items[x].getLastUsed()+600000<time)
		{
			//unload after 10 mins
			items[x].unloadItem();
		}
	}
}
//declare item system
itemSystem itemSys;

