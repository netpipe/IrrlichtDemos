#include "Level.h"

#define MAX_SIZEX 2048
#define MAX_SIZEY 20
#define MAX_BUFF (MAX_SIZEX * MAX_SIZEY)

#define WIN32

#ifdef WIN32
	#define LBOFFSET 1
#else
	#define LBOFFSET 2
#endif

LevelManager::LevelManager(Game* pCore, Enemies* enem) : core(pCore), enemies(enem) 
{
};

bool LevelManager::loadLevel(std::string levelName)
{
	enemies->EnemyArray.clear();
	Tiles.clear();
	
	std::string helperString = "media/levels/";
	helperString += levelName;
	helperString += ".map";
	
	FILE* fp = fopen((char*)helperString.c_str(),"r");
	
	if(!fp)
	{
		printf(helperString.c_str());
		printf("fp NULL, exiting.");
		exit(0);
	}
	
	char C = 0;
	int i = 0;
	int z = 0;
	
	char tileSetName[256] = {};
	
	while((C = fgetc(fp)) != '\n')
	{
		tileSetName[i] = C;
		i++;
	}
	
	std::string levelPath = "media/tilesets/";
	levelPath += (char*)tileSetName;
	levelPath += "/";
	
	char sizeX[16] = {};
	char sizeY[16] = {};

	// Reset i to 0.
	i = 0;

	while((C = fgetc(fp)) != ' ')
	{
		sizeY[i] = C;
		i++;
	}

	// Reset i to 0.
	i = 0;

	while((C = fgetc(fp)) != '\n')
	{
		sizeX[i] = C;
		i++;
	}
	
	SizeX = atoi(sizeX);
	SizeY = atoi(sizeY);
	
	// Consts for (hopefully) better performance in the upcoming loops
	const int cSizeX = SizeX;
	const int cSizeY = SizeY;

	
	char col1r[16] = {};
	char col1g[16] = {};
	char col1b[16] = {};
	
	// Reset i to 0.
	i = 0;

	while((C = fgetc(fp)) != ' ')
	{
		col1r[i] = C;
		i++;
	}
	
	// Reset i to 0.
	i = 0;

	while((C = fgetc(fp)) != ' ')
	{
		col1g[i] = C;
		i++;
	}
	
	// Reset i to 0.
	i = 0;

	while((C = fgetc(fp)) != '\n')
	{
		col1b[i] = C;
		i++;
	}
	
	char col2r[16] = {};
	char col2g[16] = {};
	char col2b[16] = {};
	
	// Reset i to 0.
	i = 0;

	while((C = fgetc(fp)) != ' ')
	{
		col2r[i] = C;
		i++;
	}
	
	// Reset i to 0.
	i = 0;

	while((C = fgetc(fp)) != ' ')
	{
		col2g[i] = C;
		i++;
	}
	
	// Reset i to 0.
	i = 0;

	while((C = fgetc(fp)) != '\n')
	{
		col2b[i] = C;
		i++;
	}
	
	SkyCol1 = Color(atof(col1r),atof(col1g),atof(col1b),1.0f);
	SkyCol2 = Color(atof(col2r),atof(col2g),atof(col2b),1.0f);

	// Reset i to 0.
	i = 0;
	
	char buffer[MAX_BUFF];
	char levMap[MAX_SIZEX][MAX_SIZEY];

	while((C = fgetc(fp)) != EOF && i < MAX_BUFF)
	{
		buffer[i] = C;
		i++;
	}
	
	for(i = 0;i < MAX_SIZEX;++i)
		for(z = 0;z < MAX_SIZEY;++z)
			levMap[i][z] = ETT_EMPTY;


	/*
	// Find all of the tiles in the level
	for(z = 0;z < SizeY;++z)
	{
		for(i = 0;i < SizeX;++i)
		{
			if(buffer[(z * (int(SizeX) + LBOFFSET)) + i] == 'X')
				levMap[i][SizeY - z - 1] = ETT_UNDEF;
		}
	}*/
	
	// New way to read level.
	i = 0;
	int g = 0;
	
	for(z = 0;z < cSizeY;++z)
	{
	C = 0;
	g = 0;
		while(C != '\n')
		{
			C = buffer[i]; 
			++i; 
			if(C == 'X')
				levMap[g][cSizeY - z - 1] = ETT_UNDEF;
			else
			{
				for(int f = 0;f < enemies->EnemyTypes.size();++f)	
					if(C == enemies->EnemyTypes[f].iden)
						levMap[g][cSizeY - z - 1] = ETT_COUNT + 1 + f;		
			}
				
			++g;
		}
		
	}
	
	// Define tile types
	for(z = 0;z < cSizeY;++z)
	{
		for(i = 0;i < cSizeX;++i)
		{
			if(levMap[i][z] == ETT_UNDEF)
			{
				if(z < cSizeY - 1)
				if(levMap[i][z + 1] == ETT_EMPTY || levMap[i][z + 1] > ETT_COUNT)
				{	
					levMap[i][z] = ETT_TOP;	
					
					if(i > 0)
					if(levMap[i - 1][z] == ETT_EMPTY || levMap[i - 1][z] > ETT_COUNT)
						levMap[i][z] = ETT_TOPLEFT;
					
					if(i < cSizeX - 1)
					if(levMap[i + 1][z] == ETT_EMPTY || levMap[i + 1][z] > ETT_COUNT)
						levMap[i][z] = ETT_TOPRIGHT;
				}
				else
				{
					levMap[i][z] = ETT_MIDDLE;
				}
				
			}			
		}
	}
	
	// Load textures.
	helperString = levelPath;
	helperString += "topleft.png";
	texTopLeft = core->getRoot()->getVideoDriver()->createTexture((char*)helperString.c_str());
	
	helperString = levelPath;
	helperString += "topright.png";
	texTopRight = core->getRoot()->getVideoDriver()->createTexture((char*)helperString.c_str());
	
	helperString = levelPath;
	helperString += "top.png";
	texTop = core->getRoot()->getVideoDriver()->createTexture((char*)helperString.c_str());
	
	helperString = levelPath;
	helperString += "mid.png";
	texMiddle = core->getRoot()->getVideoDriver()->createTexture((char*)helperString.c_str());
	
	#ifdef USE_CRACKS
	helperString = levelPath;
	helperString += "crack.png";
	texCrack = core->getRoot()->getVideoDriver()->createTexture((char*)helperString.c_str());
	#endif
	
	ISprite* tempTile = NULL;
	Tile tmpTile;
	
	// Create tiles.
	for(z = 0;z < cSizeY;++z)
	{
		for(i = 0;i < cSizeX;++i)
		{	
			tmpTile.enemy = 0;
			tmpTile.isEnemy = 0;
		
			if(levMap[i][z] == ETT_EMPTY)
				continue;				
			else if(levMap[i][z] == ETT_TOP)
				tempTile = core->getRoot()->getSceneManager()->createSprite(texTop);
			else if(levMap[i][z] == ETT_TOPLEFT)
				tempTile = core->getRoot()->getSceneManager()->createSprite(texTopLeft);
			else if(levMap[i][z] == ETT_TOPRIGHT)
				tempTile = core->getRoot()->getSceneManager()->createSprite(texTopRight);
			// This means its an enemy
			else if(levMap[i][z] > ETT_COUNT)
			{
				tmpTile.isEnemy = 1;
			
				Enemy* enem = new Enemy();
				int index = levMap[i][z] - 1 - ETT_COUNT;
				
				// This should be ok as long as EnemyTypes is not modified later
				enem->type = &enemies->EnemyTypes[index];
				
				enem->sprite = core->getRoot()->getSceneManager()->createSprite(enemies->EnemyTypes[index].Textures[0]);
				tempTile = enem->sprite;
				enem->HitPoints = enemies->EnemyTypes[index].defHitPoints;
				enem->currAnim = rand()%(enemies->EnemyTypes[index].Textures.size());
				
				enem->dead = false;
				enem->dieing = false;
				enem->dieProg = 0;
				enem->posCycle = float(rand()%100) / 100;
				
				enemies->EnemyArray.push_back(enem);
				tmpTile.enemy = enem;
			}
			else
			{
			#ifdef USE_CRACKS
				if(i > 0 && i < cSizeX - 1 && z > 0 && z < cSizeY - 1)
				{
				if(
				levMap[i][z] == ETT_MIDDLE
				&&
				levMap[i - 1][z] == ETT_MIDDLE
				&&
				levMap[i][z - 1] == ETT_MIDDLE
				&&
				levMap[i - 1][z - 1] == ETT_MIDDLE
				&&
				levMap[i + 1][z] == ETT_MIDDLE
				&&
				levMap[i][z + 1] == ETT_MIDDLE
				&&
				levMap[i + 1][z + 1] == ETT_MIDDLE
				&&
				levMap[i + 1][z - 1] == ETT_MIDDLE
				&&
				levMap[i - 1][z + 1] == ETT_MIDDLE
				)
				{
					if(rand()%20 > 16)
					{
					tempTile = core->getRoot()->getSceneManager()->createSprite(texCrack);
					tempTile->setPosition(Vector((i * 64.0f) + 64.0f,z * 64.0f));
					}
				}
				}
			#endif
				
				tempTile = core->getRoot()->getSceneManager()->createSprite(texMiddle);
			}
				
			
			
				
			
			if(tmpTile.isEnemy)
			{
				tempTile->setPosition(Vector((i * 64.0f) + 32.0f,z * 64.0f - 16.0f));
				tempTile->setLayer(4);
			}
			else
			{
				tempTile->setPosition(Vector((i * 64.0f) + 32.0f,z * 64.0f));
				tempTile->setLayer(5);
			}
			
			tmpTile.sprite = tempTile;
			tmpTile.type = levMap[i][z];
			Tiles.push_back(tmpTile);
		}
	}
	
	return true;
};
