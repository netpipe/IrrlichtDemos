#include <irrlicht.h>

using namespace irr;
using namespace irr::core;
using namespace irr::scene;




char int_to_chr(int n,int digit){
			int m;
			char out1=char("0");
			m=0;
			if(n>=100)
				m=n/100;
				if(digit==100)
				out1=char(m+48);
				n=n-m;
			m=0;
			if(n>=10)
				m=n/10;
				if(digit==10)
				out1=char(m+48);
				n=n-m;
			m=n;
			if(digit==1)
			out1=char(m+48);
			return(out1);
			}


class TerraSet{
	public:
	int XCount;
	int YCount;
	int TileSize;
	float TileScale;
	IBillboardSceneNode *grass[1000];
	ITerrainSceneNode *MyTerrains[256][256];

	TerraSet(){
		XCount=0;
		YCount=0;
		TileSize=128;
		TileScale=1;
	}

	void LoadTerrainSet(ISceneManager* smgr ,char *filenameprefix, char *filenamesuffix,int Xcnt,int Ycnt,int tilesize,vector3df scale){
		int ctrx,ctry;
		XCount=Xcnt;
		YCount=Ycnt;

	
		char Xnum[4]="000";
		char Ynum[4]="000";
		char *Xn,*Yn;
		char fname1[1024];
		Xnum[0]=char(0+48);
		Ynum[0]=char(0+48);
		TileSize=tilesize;
		for(ctrx=0;ctrx<Xcnt;ctrx++){
			Xnum[0]=int_to_chr(ctrx,100);
			Xnum[1]=int_to_chr(ctrx,10);
			Xnum[2]=int_to_chr(ctrx,1);
			Xnum[3]=NULL;
			for(ctry=0;ctry<Ycnt;ctry++){
				Ynum[0]=int_to_chr(ctry,100);
				Ynum[1]=int_to_chr(ctry,10);
				Ynum[2]=int_to_chr(ctry,1);
				Ynum[3]=NULL;
				strcpy(fname1,filenameprefix);
				Xn=&Xnum[0];
				strcat(fname1,Xnum);
				
				strcat(fname1,"_");
				Yn=&Ynum[0];
				strcat(fname1,Ynum);
				
				strcat(fname1,filenamesuffix);

				MyTerrains[ctrx][ctry] = smgr->addTerrainSceneNode(fname1);
				if(MyTerrains[ctrx][ctry]){
				MyTerrains[ctrx][ctry]->setScale(core::vector3df(1,1,1));
				MyTerrains[ctrx][ctry]->setMaterialFlag(video::EMF_LIGHTING, false);
				MyTerrains[ctrx][ctry]->setPosition(vector3df(0,0,0));
				MyTerrains[ctrx][ctry]->setMaterialTexture(0, driver->getTexture("media/maze1bmp.bmp"));
				MyTerrains[ctrx][ctry]->setMaterialTexture(1, driver->getTexture(filenameprefix));
				MyTerrains[ctrx][ctry]->setMaterialType(video::EMT_DETAIL_MAP);
				MyTerrains[ctrx][ctry]->scaleTexture(8.0f,8.0f);
				}
				}
			}
		SetScale(scale);
	}	
	
	void MoveBy(vector3df offset){
		int ctrx,ctry;
		for(ctrx=0;ctrx<XCount;ctrx++){
			for(ctry=0;ctry<YCount;ctry++){
			MyTerrains[ctrx][ctry]->setPosition(MyTerrains[ctrx][ctry]->getPosition()+offset);
			}}
		}	

	void Remove(){
		//Delete all Terrain Scene Nodes
		}

	ITerrainSceneNode* TerrainAtPos(vector3df position){
		//return terrain at X,Y world location
		vector3df pos1;
		vector3df pos2;
		int tX,tZ;
		pos1=MyTerrains[0][0]->getPosition();
		pos1=position-pos1;
		tX=pos1.X/((7/8.0)*(TileSize*TileScale));
		tZ=pos1.Z/((7/8.0)*(TileSize*TileScale));
		if(tX>(XCount-1)||tX<0||tZ>(YCount-1)||tZ<0)return NULL;
		return MyTerrains[tX][tZ];
		}

	void SetScale(vector3df scale){
		int ctrx;
		int ctry;
		vector3df pos1;

		pos1=MyTerrains[0][0]->getPosition();

		for(ctrx=0;ctrx<XCount;ctrx++){
			for(ctry=0;ctry<YCount;ctry++){
				MyTerrains[ctrx][ctry]->setScale(scale);
				MyTerrains[ctrx][ctry]->setPosition(vector3df(pos1.X+7/8.0*ctrx*((TileSize)*(scale.X)),pos1.Y,pos1.Z+7/8.0*ctry*((TileSize)*(scale.Z))));
					TileScale=scale.X;
		
				}	
			}
		}

void	ShowVegitationAtPos(vector3df center){

		//move grass to surround center

	}
};