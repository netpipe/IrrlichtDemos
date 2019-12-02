class maps
{ 
public:
	ShTlTerrainSceneNode* terrain;
	irr::scene::ISceneNode* buildings;
	irr::scene::ISceneNode* foliage;
	int mapX, mapY, mapZ, positionX, positionY, positionZ, directionalPos;
	bool playerOnMap, accessible;
	maps();

};
maps::maps(){
	terrain=0;
	buildings=0;
	foliage=0;
	playerOnMap=false;
}