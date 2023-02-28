//стандартный заголовок :)
#include <irrlicht.h>
#include <iostream>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#pragma comment(lib, "Irrlicht.lib")

void entity_translate(ISceneNode *entity,float x,float y,float z)
{
	vector3df v = entity->getPosition();
	v.X += x;
	v.Y += y;
	v.Z += z;
	entity->setPosition(v);
}

//задаем общие переменые - чтобы непарится !
ISceneManager* smgr=0;
IrrlichtDevice *device=0;
IVideoDriver* driver=0;
ISceneNode* player_1=0;

class Cubics
{
public:
	IAnimatedMesh *Cubic_mesh;
	IAnimatedMeshSceneNode *Cubic;

	float x;
	float y;
	float z;

	void Create()
	{
		Cubic_mesh = smgr->getMesh("models/Box.x");
		Cubic = smgr->addAnimatedMeshSceneNode( Cubic_mesh );
		Cubic->setMaterialFlag(EMF_LIGHTING, false);
		Cubic->setScale(vector3df(0,2,2));
		Cubic->setRotation(vector3df(90,0,0));
	}

};

//типа главная функция :)
int main()
{
	std::cout<<"My first game on Irrlicht\n";
	std::cout<<"by jimon\n";
	std::cout<<"============================\n";
	std::cout<<"Irrlicht debug : \n";
	std::cout<<"============================\n";

	//создаем окошко
	device = createDevice(EDT_OPENGL,dimension2d<u32>(640, 480),16,0,0,0,0);
	device->setWindowCaption(L"test");

	//получаем хендлы девайсов
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();

	//ето камера
	smgr->addCameraSceneNode(0,vector3df(30,0,0),vector3df(0,0,0),-1);
	ICameraSceneNode* camera = smgr->getActiveCamera();

	//создаем главный кубик
	Cubics main;
	main.Create();
	ISceneNode* main_1=main.Cubic;
	entity_translate(main_1,0,-18,0);

	//игрок
	Cubics player;
	player.Create();
	player_1=player.Cubic;
	entity_translate(player_1,0,-20,0);
	player_1->setScale(vector3df(0,8,1));

	//масив сраных кубиков :D
	Cubics Cubs [14][9];

	//создаем кубики
	int counts_x;
	int counts_y;

	for (counts_y = 0 ; counts_y < 9 ; counts_y ++)
	for (counts_x = 0 ; counts_x < 14 ; counts_x ++)
	{
		Cubics Cubs_create;
		Cubs_create.Create();
		Cubs_create.Cubic->setPosition(vector3df(0,20-counts_y*2.5f,-20+counts_x*3));
		Cubs_create.x=0;
		Cubs_create.y=20-counts_y*2.5f;
		Cubs_create.z=-20+counts_x*3;
		Cubs[counts_x][counts_y]=Cubs_create;
	};

	int lastfps=0;

	//ускорение по 3 осям
	float n2=0.032f*800;
	float n3=0.02f*800;

	//буферные переменые
	float a2;
	float a3;

	//размер ящика :)
	float razmer=20.0f;

	//сам цыкл, стоит пока незакроем окошко
	while(device->run())
	{

		//МЕГА ЛОЛОВСКАЯ ФИЗИКА :D
		if (lastfps!=0)
		entity_translate(main_1,0,n2/lastfps,n3/lastfps);

		//ето мы так щитаем позицию кубика
		vector3df v = main_1->getPosition();
		a2=v.Y;
		a3=v.Z;
		main_1->setPosition(vector3df(0,a2,a3));

		bool otr;
		otr=0;

		//колозии с кубиками
		float y;
		float z;
		for (counts_y = 0 ; counts_y < 9 ; counts_y ++)
		for (counts_x = 0 ; counts_x < 14 ; counts_x ++)
		{

			y=Cubs[counts_x][counts_y].y;
			z=Cubs[counts_x][counts_y].z;

			if (a2>(y-2)&&a2<(y+2)&&a3>(z-2)&&a3<(z+2))
			{
			n2=-n2;
			otr=1;
			Cubs[counts_x][counts_y].Cubic->remove();
			Cubs[counts_x][counts_y].y=1000;
			Cubs[counts_x][counts_y].z=1000;
			};

		};

		//колозии с боковыми стенками
		if (a2>razmer)
		{
			n2=-n2;
			otr=1;
		};

		if (a3>razmer||a3<-razmer)
		n3=-n3;

		if (a2<-razmer)
		{
			std::cout<<"YOU LOSE\n";
			n2=-n2;
		}

		//колозии с игроком
		vector3df pz = player_1->getPosition();
		y=pz.Y;
		z=pz.Z;

		if (a2>(y-1)&&a2<(y+1)&&a3>(z-0)&&a3<(z+8)&&otr==0)
			n2=-n2;

		//ставим позицию игроку
		position2d<s32> mouse = device->getCursorControl()->getPosition();
		float mx;
		mx=mouse.X-400;
		mx=mx/20+mx/27+3.6f;
		player_1->setPosition(vector3df(0,-20,mx));


		//получаем fps
		int fps = driver->getFPS();

		if (lastfps != fps)
		{

			core::stringw str = L"test ";
			str += " FPS:";
			str += fps;

			device->setWindowCaption(str.c_str());
			lastfps = fps;
		}


		//************************************************************************
		driver->beginScene(true, true, SColor(255,100,101,140));
		smgr->drawAll();
		driver->endScene();
		//************************************************************************

device->sleep(100);
	}

	//удаляем девайс, тоесть закрываем окошко
    device->drop();

	//ну есно что выходим из функции :)
	return 0;
}

