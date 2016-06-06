

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <fstream>
#include <iostream>

#include <irrlicht.h>


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#pragma comment(lib, "Irrlicht.lib")

using namespace std;

scene::ISceneManager* smgr=0;
video::IVideoDriver* driver=0;
scene::ISceneNode* node = 0;
scene::ISceneNode* nodebox = 0;
scene::ISceneNode* nodehill = 0;
scene::IAnimatedMesh* meshbox =0;
scene::IAnimatedMesh* TileHillMesh =0;

scene::IAnimatedMesh* meshhill =0;
IrrlichtDevice* device = 0;
IrrlichtDevice* texturedevice = 0;
scene::ICameraSceneNode* camera = 0;
scene::ISceneNode* skyboxNode;
gui::IGUIElement* statusText;
IGUIEnvironment* guienv=0;
IMeshBuffer* buffer=0;
IMeshBuffer* buffer2=0;



int screenwidth=1024;
int screenheight=768;
const int worldx=256;
const int worldy=256;
const int mapw=256;
const int maph=256;
const int viewsizex=42;
const int viewsizez=36;
const float tilesizex=100.0f;
const float tilesizez=80.0f;
bool scenelocked=false;
bool changed =false;
bool menuopen=false;
irr::f32 deltaX = 0.0f;
irr::f32 deltaZ = 0.0f;
irr::core::vector3df cameraPos;
int i,j,n,mapcursor;
int mapx=0;
int mapy=0;
int cursorposx, cursorposz;
bool cursorbool=true;
scene::IAnimatedMesh* tileMesh =0;
scene::IAnimatedMesh* hillmesh =0;
IGUIWindow* window=0;
scene::ISceneNode* waternode;


video::ITexture* watertexture;
video::ITexture* texturetest;
video::ITexture* textures[50];
video::ITexture* seamtextures[50];

video::ITexture* mapimage;

scene::ISceneNode* view[viewsizex][viewsizez];
scene::ISceneNode* view1[viewsizex][viewsizez];

scene::SMesh* TileMesh[viewsizex][viewsizez];

scene::ISceneNode* testnode = 0;
core::position2d<s32> mousepos;
bool leftmousebuttonpressed=false;
int editswitch=0;
int texturesnr=21;
int texturescounter=4;


class World
{
private:
	int WorldMapHeight[worldx][worldy];
	int  VertexColor[worldx][worldy][3];
	int WorldTexture[worldx][worldy];
	unsigned char height,c;
	std::ifstream File;
	FILE *f;

public:
	void LoadHeightMap()
	{

		File.open("media/heightmap.raw", std::ios::in | std::ios::binary );
		for (int i=0;i<worldx;i++)
			for (int j=0;j<worldy;j++)
				{
					height=File.get();
					height=File.get();
					height=File.get();
					WorldMapHeight[i][j]=height;
				};

		File.close();
	}

	void SaveMap()
	{
		f=fopen("media/textures/world1.map", "w");
		if (!f) return;
		for (int i=0;i<worldx;i++)
			for (int j=0;j<worldy;j++)
				{
					fprintf(f,"%3x,",WorldMapHeight[i][j]);
					fprintf(f,"%3x,",WorldTexture[i][j]);
				};
		fclose(f);
	}

	void LoadMap()
	{
		f=fopen("media/textures/world1.map", "r");
		if (!f) return;

		for (int i=0;i<worldx;i++)
			for (int j=0;j<worldy;j++)
				{
					fscanf(f,"%3x,",&WorldMapHeight[i][j]);
					fscanf(f,"%3x,",&WorldTexture[i][j]);
				};
		fclose(f);
	}


	short getHeight(int x, int y)
	{
		return WorldMapHeight[x][y];
	}

	void setHeight(int x, int y, short height)
	{
		if (height<0) height=0;

		WorldMapHeight[x][y]=height;
	}

	short getTexture(int x, int y)
	{

		return WorldTexture[x][y];
	}

	void setTexture(int x, int y, short t)
	{
		if (x>worldx-1) x=x-worldx;
		if (x<0) x=worldx+x;
		if (y>worldy-1) y=y-worldy;
		if (y<0) y=worldy+y;
		WorldTexture[x][y]=t;
	}


	void LoadVertexColorMap()
	{
		File.open("media/heightmap.raw",ios::in | ios::binary);
		for (int i=0;i<worldx;i++)
		{
			for (int j=0;j<worldy;j++)
			{
				for(int colour=0;colour<3;colour++)
				{
					c=File.get();
					VertexColor[i][j][colour]=c;
				}
			}
		};
		File.close();
	}
	video::SColor getColor(int i, int j)
	{
		if (i>worldx-1) i=worldx-i;
		if (j>worldy-1) j=worldy-i;
		return SColor(120,VertexColor[worldx-i-1][worldx-j-1][0],VertexColor[worldx-i-1][worldx-j-1][1],VertexColor[worldx-i-1][worldx-j-1][2]);
	}

	short getRed(int i, int j)
	{
		if (i>worldx-1) i=worldx-i;
		if (j>worldy-1) j=worldy-i;
		return VertexColor[worldx-i-1][worldx-j-1][0];
	}
	short getGreen(int i, int j)
	{
		if (i>worldx-1) i=worldx-i;
		if (j>worldy-1) j=worldy-i;
		return VertexColor[worldx-i-1][worldx-j-1][1];
	}
	short getBlue(int i, int j)
	{
		if (i>worldx-1) i=worldx-i;
		if (j>worldy-1) j=worldy-i;
		return VertexColor[worldx-i-1][worldx-j-1][2];
	}
};

World World1;


class Player
{

private:
	int CurrentX;
	int CurrentY;
	int CurrentZ;
	int Gold;

public:
	int getX()
	{
		return CurrentX;
	}
	int getY()
	{
		return CurrentY;
	}
	int getZ()
	{
		return CurrentZ;
	}
	void setX(int x)
	{
		CurrentX=x;
		if (CurrentX < 0) CurrentX=worldx +x;
		if (CurrentX > worldx-1) CurrentX=x-(worldx+1);
	}
	void setY(int y)
	{
		CurrentY=y;
	}
	void setZ(int z)
	{
		CurrentZ=z;
		if (CurrentZ < 0) CurrentZ=worldy +z;
		if (CurrentZ > worldy-1) CurrentZ=z-(worldy+1);
	}
};


Player Player1;




void mergeTextures(video::ITexture* texture1, video::ITexture* texture2)
{
	s16 *p1 = (s16*)texture1->lock();
	s16 *p2 = (s16*)texture2->lock();

	core::dimension2d<s32> dim;
	dim = texture1->getSize();
	s32 pitch = texture1->getPitch()/2;

	for (s32 x=0; x<dim.Width; ++x)
		for (s32 y=0; y<dim.Height; ++y)
			if ((p1[y*pitch + x] & 0x7FFF) !=0) p2[y*pitch+x]=p1[y*pitch + x];

	texture1->unlock();
	texture2->unlock();
}


void setPixelTexture(video::ITexture* texture, int x, int y, s16 color)
{
	s16 *p = (s16*)texture->lock();

	core::dimension2d<s32> dim;
	dim = texture->getSize();
	s32 pitch = texture->getPitch()/2;

	p[y*pitch+x]=color;

	texture->unlock();
}

void setPixelMap(int x, int y, s16 color)
{
	if (World1.getHeight(x,y)<50) color=200;
	y=maph-y;
	if (y>(maph-1)) y=maph-1;
	setPixelTexture(mapimage, x, y, color);
}

void setPixelMapGrey(int x, int y, s16 color)
{
	if (World1.getHeight(x,y)<50)
		color=200;
	else
		color=color*1024+color*32+color;
	y=maph-y;
	if (y>(maph-1)) y=maph-1;
	setPixelTexture(mapimage, x, y, color);
}


void setView(irr::u32 i, irr::u32 j){

	int k,l,k1,l1, km1, lm1,k2,l2;
	bool tflag=false;

	k=(i+Player1.getX())-(viewsizex/2);
	k1=k+1;
	k2=k+2;
	km1=k-1;
	l=j+Player1.getZ();
	l1=l+1;
	l2=l+2;
	lm1=l-1;
	if (k<0) k=worldx+k;
	if (k>(worldx-1)) k=k-worldx;
	if (k1<0) k1=worldx+k1;
	if (k1>(worldx-1)) k1=k1-worldx;
	if (k2>(worldx-1)) k2=k2-worldx;
	if (l<0) l=worldy+l;
	if (l>(worldy-1)) l=l-worldy;
	if (l1<0) l1=worldy+l1;
	if (l1>(worldy-1)) l1=l1-worldy;
	if (l2>(worldy-1)) l2=l2-worldy;

	if (km1<0) km1=worldx+km1;
	if (km1>(worldx-1)) km1=km1-worldx;
	if (lm1<0) lm1=worldy+lm1;
	if (lm1>(worldy-1)) lm1=lm1-worldy;

	core::vector3df ver;

	buffer = TileMesh[i][j]->getMeshBuffer(0);
	video::S3DVertex* v = (video::S3DVertex*)buffer->getVertices();

	ver=v[1].Pos;
	ver.Y=float(World1.getHeight(k,l1)*2);
	v[1].Pos=ver;

	ver=v[0].Pos;
	ver.Y=float(World1.getHeight(k,l)*2);
	v[0].Pos=ver;

	ver=v[3].Pos;
	ver.Y=float(World1.getHeight(k1,l1)*2);
	v[3].Pos=ver;

	ver=v[2].Pos;
	ver.Y=float(World1.getHeight(k1,l)*2);
	v[2].Pos=ver;

	view[i][j]->setMaterialTexture(0, textures[World1.getTexture(k,l)]);




	v[1].Color.setAlpha(0);
	v[0].Color.setAlpha(0);
	v[3].Color.setAlpha(0);
	v[2].Color.setAlpha(0);
	tflag=false;

   if (World1.getTexture(k,l) < 21) {
	if (World1.getTexture(km1,l) < World1.getTexture(k,l))
	{
		tflag=true;
		view[i][j]->setMaterialType(video::EMT_SOLID_2_LAYER );
		view[i][j]->setMaterialTexture(0, textures[World1.getTexture(k,l)]);
		view[i][j]->setMaterialTexture(1, textures[World1.getTexture(km1,l)]);
		v[1].Color.setAlpha(254);
		v[0].Color.setAlpha(254);

	}

	if (World1.getTexture(k1,l) < World1.getTexture(k,l))
	{
		tflag=true;
		view[i][j]->setMaterialType(video::EMT_SOLID_2_LAYER );
		view[i][j]->setMaterialTexture(0, textures[World1.getTexture(k,l)]);
		view[i][j]->setMaterialTexture(1, textures[World1.getTexture(k1,l)]);
		v[3].Color.setAlpha(254);
		v[2].Color.setAlpha(254);
	}

	if (World1.getTexture(k,lm1) < World1.getTexture(k,l))
	{
		tflag=true;
		view[i][j]->setMaterialType(video::EMT_SOLID_2_LAYER );
		view[i][j]->setMaterialTexture(0, textures[World1.getTexture(k,l)]);
		view[i][j]->setMaterialTexture(1, textures[World1.getTexture(k,lm1)]);
		v[0].Color.setAlpha(254);
		v[2].Color.setAlpha(254);
	}

	if (World1.getTexture(k,l1) < World1.getTexture(k,l))
	{
		tflag=true;
		view[i][j]->setMaterialType(video::EMT_SOLID_2_LAYER );
		view[i][j]->setMaterialTexture(0, textures[World1.getTexture(k,l)]);
		view[i][j]->setMaterialTexture(1, textures[World1.getTexture(k,l1)]);
		v[1].Color.setAlpha(254);
		v[3].Color.setAlpha(254);

	}
	if (World1.getTexture(k1,lm1) < World1.getTexture(k,l))
	{
		tflag=true;
		view[i][j]->setMaterialType(video::EMT_SOLID_2_LAYER );
		view[i][j]->setMaterialTexture(0, textures[World1.getTexture(k,l)]);
		view[i][j]->setMaterialTexture(1, textures[World1.getTexture(k1,lm1)]);
		v[2].Color.setAlpha(254);
	}

	if (World1.getTexture(km1,lm1) < World1.getTexture(k,l))
	{
		tflag=true;
		view[i][j]->setMaterialType(video::EMT_SOLID_2_LAYER );
		view[i][j]->setMaterialTexture(0, textures[World1.getTexture(k,l)]);
		v[0].Color.setAlpha(254);
	}

	if (World1.getTexture(k1,l1) < World1.getTexture(k,l))
	{
		tflag=true;
		view[i][j]->setMaterialType(video::EMT_SOLID_2_LAYER );
		view[i][j]->setMaterialTexture(0, textures[World1.getTexture(k,l)]);
		view[i][j]->setMaterialTexture(1, textures[World1.getTexture(k1,l1)]);
		v[3].Color.setAlpha(254);

	}

	if (World1.getTexture(km1,l1) < World1.getTexture(k,l))
	{
		tflag=true;
		view[i][j]->setMaterialType(video::EMT_SOLID_2_LAYER );
		view[i][j]->setMaterialTexture(0, textures[World1.getTexture(k,l)]);
		view[i][j]->setMaterialTexture(1, textures[World1.getTexture(km1,l1)]);
		v[1].Color.setAlpha(254);

	}
   }
	if(tflag==false)
	{
		view[i][j]->setMaterialType(video::EMT_SOLID_2_LAYER );
		view[i][j]->setMaterialTexture(0, textures[World1.getTexture(k,l)]);
	}

}



void Update()
{
      for (irr::u32 i =0; i<viewsizex; i++){
         for (irr::u32 j =0; j<viewsizez; j++){
            setView(i,j);
         }
      }

}




void InitializeMap()
{
	Player1.setX(50);
	Player1.setZ(50);

	World1.LoadVertexColorMap();

	dimension2d<irr::f32> Tilesize = irr::core::dimension2d<irr::f32>(tilesizex,tilesizez);
	dimension2d<irr::u32> Tilecount = irr::core::dimension2d<irr::u32>(1,1);
	f32 Hillheight = 0.0f;
	dimension2d<irr::f32> Hillcount = irr::core::dimension2d<irr::f32>(0.0f,0.0f);
	dimension2d<irr::f32> Texturerepeat = irr::core::dimension2d<irr::f32>(2.0f,2.0f);

	TileHillMesh = smgr->addHillPlaneMesh("tile", Tilesize, Tilecount, 0, Hillheight,Hillcount,Texturerepeat);


	hillmesh = smgr->addHillPlaneMesh("myHill",
		core::dimension2d<f32>(float(viewsizex * tilesizex/4.25),float(viewsizez*tilesizez/4.25)),
		core::dimension2d<u32>(4,4), 0, 0,
		core::dimension2d<f32>(5,5),
		core::dimension2d<f32>(10,10));

	smgr->getMeshManipulator()->setVertexColorAlpha(hillmesh->getMesh(0), 200);
	waternode = smgr->addWaterSurfaceSceneNode(hillmesh->getMesh(0), 5.0f, 500.0f, 1000.0f);
	waternode->setPosition(core::vector3df((tilesizex*viewsizex)/2+450,110,(tilesizez*viewsizez)/2+350));
	waternode->setMaterialTexture(0, watertexture);
	waternode->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA  );
	waternode->setMaterialFlag(video::EMF_LIGHTING, false);
	//waternode->setAutomaticCulling;

	for ( i = 0; i<worldx; i++)
		for ( j = 0; j<worldy; j++) World1.setTexture(i,j,texturescounter);

	for ( i = 0; i<viewsizex; i++){
		for ( j = 0; j<viewsizez; j++){


			TileMesh[i][j]=smgr->getMeshManipulator()->createMeshCopy(TileHillMesh->getMesh(0));

			buffer =TileMesh[i][j]->getMeshBuffer(0);
			video::S3DVertex* v =
					(video::S3DVertex*)buffer->getVertices();

			core::vector3df ver;
			ver.X=tilesizex*(i);
			ver.Y=float(World1.getHeight(i+Player1.getX(),j+Player1.getX()+1)*2);
			ver.Z=tilesizez*(j+1);
			v[1].Pos = ver;

			ver.X=tilesizex*(i);
			ver.Y=float(World1.getHeight(i+Player1.getX(),j+Player1.getZ())*2);
			ver.Z=tilesizez*(j);
			v[0].Pos=ver;

			ver.X=tilesizex*(i+1);
			ver.Y=float(World1.getHeight(i+Player1.getX()+1,j+Player1.getZ()+1)*2);
			ver.Z=tilesizez*(j+1);
			v[3].Pos=ver;

			ver.X=tilesizex*(i+1);
			ver.Y=float(World1.getHeight(i+Player1.getX()+1,j+Player1.getZ())*2);
			ver.Z=tilesizez*(j);
			v[2].Pos=ver;

			TileMesh[i][j]->recalculateBoundingBox();
			smgr->getMeshManipulator()->recalculateNormals(TileMesh[i][j]);
			smgr->getMeshManipulator()->setVertexColorAlpha(TileMesh[i][j], 120);

			view[i][j] = smgr->addMeshSceneNode(TileMesh[i][j]);
			view[i][j]->setMaterialType(EMT_SOLID) ;
			view[i][j]->setMaterialTexture(0, textures[texturescounter]);
		//	view[i][j]->setAutomaticCulling(false);
			view[i][j]->setMaterialFlag(video::EMF_LIGHTING, false);

		}
	}

}


void testviewNodes()
{
core::dimension2d<s32> dim;
f32 transformedPos[4];
core::matrix4 trans;
video::S3DVertex* v;
core::position2d<s32> pos2d;
core::vector3df pos3d;
dim.Width=screenwidth;
dim.Height=screenheight;
dim.Width /= 2;
dim.Height /= 2;


  for ( i = 0; i<viewsizex; i++){
	for ( j = 0; j<viewsizez; j++){
		buffer = TileMesh[i][j]->getMeshBuffer(0);
		v = (video::S3DVertex*)buffer->getVertices();
		pos3d = v[0].Pos;
		trans = smgr->getActiveCamera()->getProjectionMatrix();
		trans *= smgr->getActiveCamera()->getViewMatrix();

		transformedPos[0] = pos3d.X;
		transformedPos[1] = pos3d.Y;
		transformedPos[2] = pos3d.Z;
		transformedPos[3] = 1.0f;

		trans.multiplyWith1x4Matrix(transformedPos);

		f32 zDiv = transformedPos[1] == 0.0f ? 1.0f : (1.0f / transformedPos[3]);

		pos2d.X = (s32)(dim.Width * transformedPos[0] * zDiv) + dim.Width;
		pos2d.Y = ((s32)(dim.Height - (dim.Height * (transformedPos[1] * zDiv))));

		if (((pos2d.X-mousepos.X)>-30) && ((pos2d.X-mousepos.X)<30) && ((pos2d.Y-mousepos.Y)>-30) && ((pos2d.Y-mousepos.Y)<30))
		{
			switch(editswitch)
			{
				case 0:
				{
					World1.setTexture(i+Player1.getX()-viewsizex/2,j+Player1.getZ(),texturescounter);
				}
				break;
				case 1:
				{
					World1.setHeight(i+Player1.getX()-viewsizex/2,j+Player1.getZ(),World1.getHeight(i+Player1.getX()-viewsizex/2,j+Player1.getZ())-5);
					setPixelMapGrey(i+Player1.getX()-viewsizex/2, j+Player1.getZ(), World1.getHeight(i+Player1.getX()-viewsizex/2,j+Player1.getZ()));
				}
				break;
				case 2:
				{
					World1.setHeight(i+Player1.getX()-viewsizex/2,j+Player1.getZ(),World1.getHeight(i+Player1.getX()-viewsizex/2,j+Player1.getZ())+5);
					setPixelMapGrey(i+Player1.getX()-viewsizex/2, j+Player1.getZ(), World1.getHeight(i+Player1.getX()-viewsizex/2,j+Player1.getZ()));

				}
				break;
				case 3:
				{
					World1.setHeight(i+Player1.getX()-viewsizex/2,j+Player1.getZ(),50);
				}
				break;
			}
		}
	}
  }
}


class MyEventReceiver : public IEventReceiver
{
public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{

		if(event.EventType == irr::EET_LOG_TEXT_EVENT)
		{
			return true;
		}


		if (! menuopen && event.EventType == EET_MOUSE_INPUT_EVENT)
		{
			mousepos= device->getCursorControl()->getPosition();
			if (leftmousebuttonpressed)
			{
				testviewNodes();
				Update();
			}
			if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
			{
				if (mousepos.X >mapx && mousepos.Y >mapy && mousepos.X <(mapx+mapw) && mousepos.Y < (mapy+maph))
				{
					cursorposx=mousepos.X-mapx;
					cursorposx=cursorposx*worldx/mapw;
					cursorposz=maph-mousepos.Y-mapy;
					cursorposz=cursorposz*worldy/maph;
					Player1.setX(cursorposx);
					Player1.setZ(cursorposz);
					Update();
				}
				else
				{
					leftmousebuttonpressed=true;
					testviewNodes();
					Update();
				}
			}
			if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
			{
				leftmousebuttonpressed=false;
			}
		}

		if (!menuopen && view[0][0] != 0 && event.EventType == irr::EET_KEY_INPUT_EVENT)
		{
			switch(event.KeyInput.Key)
			{
			case KEY_ESCAPE:
				{

					device->closeDevice();

					return true;
				}
				case KEY_KEY_Y:
				{
					cameraPos = camera->getPosition();
					camera->setPosition(irr::core::vector3df(cameraPos.X,cameraPos.Y+10,cameraPos.Z));
					if (cameraPos.Y>700)
					{
						skyboxNode->setVisible(false);
					}
					return true;
				}
				case KEY_KEY_X:
				{
					cameraPos = camera->getPosition();
					camera->setPosition(irr::core::vector3df(cameraPos.X,cameraPos.Y-10,cameraPos.Z));
					if (cameraPos.Y<700)
					{
						skyboxNode->setVisible(true);
					}
					return true;
				}
				case KEY_KEY_C:
				{
					cameraPos = camera->getTarget();
					camera->setTarget(irr::core::vector3df(cameraPos.X,cameraPos.Y+10,cameraPos.Z));
					return true;
				}
				case KEY_KEY_V:
				{
					cameraPos = camera->getTarget();
					camera->setTarget(irr::core::vector3df(cameraPos.X,cameraPos.Y-10,cameraPos.Z));
					return true;
				}
				case KEY_KEY_B:
				{
					cameraPos = camera->getPosition();
					camera->setPosition(irr::core::vector3df(cameraPos.X,cameraPos.Y,cameraPos.Z-10));
					return true;
				}
				case KEY_KEY_N:
				{
					cameraPos = camera->getPosition();
					camera->setPosition(irr::core::vector3df(cameraPos.X,cameraPos.Y,cameraPos.Z+10));
					return true;
				}
				case KEY_KEY_L:
				{
					editswitch=1;
					return true;
				}
				case KEY_KEY_H:
				{
					editswitch=2;
					return true;
				}
				case KEY_KEY_R:
				{
					editswitch=3;
					return true;
				}

				case KEY_KEY_K:
				{
					World1.SaveMap();
					return true;
				}
				case KEY_KEY_O:
				{
					World1.LoadMap();
					return true;
				}
				case KEY_KEY_Z :
				{
					if (editswitch > 0 )
					{
						editswitch=0;
						return true;
					}
					else
					{
						if (event.KeyInput.PressedDown)
						{
						texturescounter--;
						if (texturescounter < 0) texturescounter=texturesnr;
						return true;
						}
					}
					return true;
				}
				case KEY_KEY_T :
				{
					if (editswitch > 0 )
					{
						editswitch=0;
						return true;
					}
					else
					{
						if (event.KeyInput.PressedDown)
						{
						texturescounter++;
						if (texturescounter > texturesnr) texturescounter=0;
						return true;
						}
					}
					return true;
				}
				case KEY_LEFT:
				case KEY_KEY_A:
				{
					n=Player1.getX();
					Player1.setX(--n);
					Update();
					return true;
				}
				case KEY_RIGHT:
				case KEY_KEY_D:
				{
					n=Player1.getX();
					Player1.setX(++n);
					Update();
					return true;
				}
				case KEY_UP:
				case KEY_KEY_W:
				{
					n=Player1.getZ();
					Player1.setZ(++n);
					Update();
					return true;
				}
				case KEY_DOWN:
				case KEY_KEY_S:
				{
					n=Player1.getZ();
					Player1.setZ(--n);
					Update();
					return true;
				}

			}
		}

		return false;
	}
};


int main()
{
	int fps, lastfps;
	MyEventReceiver receiver;

	device = createDevice(video::EDT_OPENGL , core::dimension2d<s32>(800, 600),
		16, false, false, &receiver);

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();
	statusText = device->getGUIEnvironment()->addStaticText(L"Loading...",rect<int>(10,10,400,30),	true);

	IGUISkin* skin = guienv->getSkin();
	IGUIFont* font = guienv->getFont("../../media/fonthaettenschweiler.bmp");
	if (font) skin->setFont(font);
	skin->setColor(EGDC_BUTTON_TEXT ,SColor(255,255,255,255));

	watertexture = driver->getTexture("media/SkyBox5.bmp");

	textures[0] = driver->getTexture("media/textures/black.bmp");
	textures[1] = driver->getTexture("media/SkyBox5.bmp");
	textures[2] = driver->getTexture("media/textures/water.bmp");
	textures[3] = driver->getTexture("media/textures/waterdark.bmp");
	textures[4] = driver->getTexture("media/textures/grass1.bmp");
	textures[5] = driver->getTexture("media/textures/grass2.bmp");
	textures[6] = driver->getTexture("media/textures/grass3.bmp");
	textures[7] = driver->getTexture("media/textures/grass4.bmp");
	textures[8] = driver->getTexture("media/textures/grassstone.bmp");
	textures[9] = driver->getTexture("media/textures/rock.bmp");
	textures[10] = driver->getTexture("media/textures/rock1.bmp");
	textures[11] = driver->getTexture("media/textures/rock2.bmp");
	textures[12] = driver->getTexture("media/textures/rock3.bmp");
	textures[13] = driver->getTexture("media/textures/sand1.bmp");
	textures[14] = driver->getTexture("media/textures/sand2.bmp");
	textures[15] = driver->getTexture("media/textures/sand3.bmp");
	textures[16] = driver->getTexture("media/textures/sand4.bmp");
	textures[17] = driver->getTexture("media/textures/sandgrass1.bmp");
	textures[18] = driver->getTexture("media/textures/snow1.bmp");
	textures[19] = driver->getTexture("media/textures/snow2.bmp");
	textures[20] = driver->getTexture("media/textures/snow3.bmp");
	textures[21] = driver->getTexture("media/textures/road1.bmp");

	mapcursor=3;

	driver->makeColorKeyTexture(texturetest, core::position2d<s32>(65,65));
	camera = smgr->addCameraSceneNode(0,core::vector3df(tilesizex*(viewsizex/2),600.0f,tilesizez*(viewsizez/10)),core::vector3df(tilesizex*(viewsizex/2),-200,tilesizez*(viewsizez/2)),-1);
	//camera = smgr->addCameraSceneNodeFPS();

	InitializeMap();
	World1.LoadMap();

	skyboxNode = smgr->addSkyBoxSceneNode(
	driver->getTexture("media/irrlicht2_up.bmp"),
	driver->getTexture("media/irrlicht2_dn.bmp"),
	driver->getTexture("media/irrlicht2_lf.bmp"),
	driver->getTexture("media/irrlicht2_rt.bmp"),
	driver->getTexture("media/irrlicht2_ft.bmp"),
	driver->getTexture("media/irrlicht2_bk.bmp"));
	lastfps = -1;
	World1.LoadMap();
	Update();
	Player1.setX(220);
	Player1.setZ(98);

device->setEventReceiver(&receiver);

World1.LoadHeightMap();
	while(device->run())
	{
		scenelocked=true;
		Update();
		scenelocked=false;
		if (scenelocked==false)
		{
		driver->beginScene(true, true, video::SColor(150,0,0,0));
		smgr->drawAll();


		guienv->drawAll();
		driver->endScene();
		}


		fps = driver->getFPS();
			wchar_t tmp[255];
			swprintf(tmp, 255, L"%s fps:%d polys:%d ",
				driver->getName(),
				driver->getFPS(),
				driver->getPrimitiveCountDrawn());
			statusText->setText(tmp);
			lastfps = fps;


	}


	for(i=0;i<viewsizex;i++)
		for(j=0;j<viewsizez;j++)
			TileMesh[i][j]->drop();

	device->drop();

	return 0;
}

