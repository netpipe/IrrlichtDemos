class charData
{
public:
	// NPC Info
	int id, maxhp, currenthp, level, name, type, action, race, mapX, mapY, inMapX, inMapY, changeAction, changePosition, loaded;
	vector3df speed;
	irr::scene::ISceneNode* theCharacter;
	// NPC Model Data
	stringc model, head, lHand, rHand, chest, neck, waist, finger1, finger2, leg, hands;
};
class npcClass
{
public:
	void loadFromServer(stringc serverString);
	void updateFromServer(stringc serverString);
	void remove();
	void buildModelFunc();
	void update(double deltaT);
	int getId();
	bool getBuildModel();
	npcClass();
	int getNodeId();
	void setHealth(int newHealth);
private:
	int id, health, maxhealth, modelId, mapx, mapy, mapz, factionId, level, equipList[20], animation, nodeId;
	double posx, posy, posz, fsecx, fsecy, fsecz, rotation, visibility, lastUpdate, lastMove;
	stringc factionName, name, title;
	bool loaded, buildModel;
	vector3df movVec;
	irr::scene::ISceneNode* npcModel;
};
void npcClass::setHealth(int newHealth)
{
	health=newHealth;
}
int npcClass::getNodeId()
{
	return nodeId;
}
npcClass::npcClass()
{
	loaded=false;
}
bool npcClass::getBuildModel()
{
	return buildModel;
}
int npcClass::getId()
{
	return id;
}
void npcClass::updateFromServer(stringc serverString)
{
	if(loaded)
	{
		stringc brokenString[10];
		breakCode(serverString, brokenString,10);
		health=atoi(brokenString[2].c_str());
		movVec.X=(atof(brokenString[3].c_str())-npcModel->getPosition().X)/5;
		movVec.Y=(atof(brokenString[4].c_str())-npcModel->getPosition().Y)/5;
		movVec.Z=(atof(brokenString[5].c_str())-npcModel->getPosition().Z)/5;
		mapx=atoi(brokenString[6].c_str());
		mapy=atoi(brokenString[7].c_str());
		mapz=atoi(brokenString[8].c_str());
		rotation=atof(brokenString[9].c_str());
		npcModel->setRotation(vector3df(0,rotation,0));
	}
}
void npcClass::update(double deltaT)
{
	if(loaded)
	{
		npcModel->setPosition(npcModel->getPosition()+(movVec*deltaT));
	}
}
void npcClass::buildModelFunc()
{
	printf("building Model for npc %i\n", id);
	buildModel=false;
	stringc meshName="models/";
	char buffer[32];
	meshName.append(itoa(modelId, buffer, 10));
	meshName.append(".b3d");
	npcModel=smgr->addAnimatedMeshSceneNode(smgr->getMesh("models/sydney.md2"),0,globalNodeId);//meshName.c_str()));
	nodeId=globalNodeId;
	globalNodeId++;
	meshName="textures/";
	buffer[32];
	meshName.append(itoa(modelId, buffer, 10));
	meshName.append(".png");
	npcModel->setMaterialTexture(0, driver->getTexture("textures/sydney.bmp"));//meshName.c_str()));
	npcModel->setMaterialFlag(video::EMF_LIGHTING, false);
	//equipment build
	npcModel->setPosition(vector3df(posx,posy,posz));
	npcModel->setRotation(vector3df(sin(rotation),0,cos(rotation)));
	npcModel->setScale(vector3df(.05,.05,.05));
	loaded=true;
}
void npcClass::loadFromServer(irr::core::stringc serverString)
{
	printf("loading npc from server\n");
	//break string and store info
	stringc brokenString[22];
	breakCode(serverString, brokenString,22);
	id=atoi(brokenString[1].c_str());
	health=atoi(brokenString[2].c_str());
	maxhealth=atoi(brokenString[3].c_str());
	modelId=atoi(brokenString[4].c_str());
	posx=atof(brokenString[5].c_str());
	posy=atof(brokenString[6].c_str());
	posz=atof(brokenString[7].c_str());
	fsecx=atof(brokenString[8].c_str());
	fsecy=atof(brokenString[9].c_str());
	fsecz=atof(brokenString[10].c_str());
	mapx=atoi(brokenString[11].c_str());
	mapy=atoi(brokenString[12].c_str());
	mapz=atoi(brokenString[13].c_str());
	factionId=atoi(brokenString[14].c_str());
	factionName=brokenString[15];
	name=brokenString[16];
	title=brokenString[17];
	level=atoi(brokenString[18].c_str());
	rotation=atof(brokenString[19].c_str());
	visibility=atof(brokenString[20].c_str());
	//equipList;
	movVec.X=(fsecx-posx)/5;
	movVec.Y=(fsecy-posy)/5;
	movVec.Z=(fsecy-posy)/5;
	buildModel=true;
}