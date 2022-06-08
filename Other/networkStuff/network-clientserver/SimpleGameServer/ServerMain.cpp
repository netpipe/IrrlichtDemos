/*******************************************************************************
* Simple Game Server - Programm-server for EG-Network(EG-Net)                  *
* Version 1.0                                                                  *
* Autor - Olshevsky Andrey aka Dronchik aka Sepule                             *
* Licensing GPL                                                   *
*******************************************************************************/

#include <iostream>       //include i/o streams header file 
#include "irrlicht.h"
#include "GameNetLib.h"   //include game network library header file
#include "Messages.h"     //include messages header file

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


#define MAX_CLIENTS 10 //макс количество клиентов

int Clients[MAX_CLIENTS]; // флаги подключенности клиентов    

IrrlichtDevice *GameDevice;
video::IVideoDriver* driver;            //video driver this game
scene::ISceneManager* smgr;             //scene manager this game
gui::IGUIEnvironment* guienv;           //gui this game
//IAnimatedMeshSceneNode* meshnode[10];   //мешноды для клиентов
//IAnimatedMesh* mesh;                    //меш - модель клиента
bool connect[10];                       //состояние подключения для каждого клиента
bool wait=false;                        //ждём, создался ли клиент или нет
int *id_client;                         //идентификатор клиента
//ISceneNode* Parent;                     //сценнод, нужный лишь чтобы удалять мешноды клиентов корректным способом
                

//стурктура для получения сообщений от клиента
struct MyStruct 
{  
	f32 posX;
	f32 posY;
	f32 posZ;
	bool isConnected;
};

//структура для отправления сообщений клиентам
struct ServerStruct
{	 
	f32 posX[10];
	f32 posY[10];
	f32 posZ[10];
	bool isConnected[10];
	
};

//структура для отправки серверного идентификатора клиента для клиент
struct IdStruct
{
	int client_id;
};

ServerStruct  *sm_struct;   //этот массив указателей используеться для обозначения характеристик всех игроков 
                            //и отправки этих данных всем игрокам
ServerStruct  *sp_struct;   //этот массив указателей используеться для проверки с предыдущими данными 
MyStruct      *s_struct;    //этот указатель используеться для работы с одним клиентом

IdStruct      *id_struct;   //эта структура используется для отправки клиенту его id


struct NL_TBinary binary;




// обработчик сообщения от клиента
// получаем инфу об клиенте от клиента(этого же)
void InterpriterChar(int id, union NL_Variable *variable)
{	
	if(wait==false)
	{
		s_struct=(struct MyStruct*)variable[0].bin.data;	
		
		sm_struct->posX[id] = s_struct->posX;
		sm_struct->posY[id] = s_struct->posY;
		sm_struct->posZ[id] = s_struct->posZ;  
		
		cout<<"client ["<<id<<"] "<<"posX: "<<sm_struct->posX[id]
		    <<" posY: "<<sm_struct->posY[id]<<" posZ"<<sm_struct->posZ[id]<<endl;
			
		//meshnode[id]->setPosition(vector3df(sm_struct->posX[id],sm_struct->posY[id],sm_struct->posZ[id]));
		
		binary.length = sizeof(struct ServerStruct);
		binary.data = sm_struct;
		
		/*if(Clients[id])
		{
			//cout<<"message for client["<<i<<"] sended"<<endl;
			NLS_Message(id, MSG_SERVER_STRUCT, binary);
		}*/	
	}
}

// обработчик подключения клиента
int cl_Connect(int id, char *host)
{
  cout<<"Client connected. id = '"<<id<<"' host = "<<host<<endl;  
	
  //если клиент подключился но не инициализирован на сервере то
  //для мешнода с ид клиента ставим меш шара 
  //connect с клиентским id делаем true;
  //в серверную структуру записываем что это клиент подключён
  if(connect[id]==false)
  {
	  //meshnode[id] = smgr->addAnimatedMeshSceneNode(mesh);
	  id_struct->client_id = id;
	  wait=true;	  
	  connect[id]=true;
	  sm_struct->isConnected[id]=true;
  }


  
  /*
  struct NL_TBinary binary;
  binary.length = sizeof(struct ServerStruct);
  binary.data = sm_struct;
  
  NLS_Message(id, MSG_SERVER_STRUCT, binary);
  */
  
  
  if(id < MAX_CLIENTS){ // простейшее ограничение на количество клиентов
    Clients[id] = 1;	
    return 1;
  }
  cout<<" Connection refused"<<endl;
  return 0;
}

// обработчик отключения клиента
void cl_Disconnect(int id)
{	
	//также как и операция подключения, только в точности наоборот
	cout<<"Client disconnected. id = "<<id<<endl;
	Clients[id] = 0;
	//Parent=meshnode[id]->getParent();
	//Parent->removeChild(meshnode[id]);
	wait=false;	  
	connect[id]=false;
	sm_struct->isConnected[id]=false;
}


int main(int argv, char **argc)
{
	int clients;

	//коннект клиентов в false
	//connects of clients to false
	connect[0]  = false;
	connect[1]  = false;
	connect[2]  = false;
	connect[3]  = false;
	connect[4]  = false;
	connect[5]  = false;
	connect[6]  = false;
	connect[7]  = false;
	connect[8]  = false;
	connect[9]  = false;
	connect[10] = false;

	//создаём нужные структуры
	s_struct = new MyStruct;
	sm_struct = new ServerStruct;
	sp_struct = new ServerStruct;
	id_struct = new IdStruct;

	//обнуляем позиции клиентов в структуре
	for(int i=0;i<MAX_CLIENTS;i++)
	{		
		sm_struct->posX[i] = 0;
		sm_struct->posY[i] = 0;
		sm_struct->posZ[i] = 0;
	}
	for(int i=0;i<MAX_CLIENTS;i++)
	{		
		sp_struct->posX[i] = 0;
		sp_struct->posY[i] = 0;
		sp_struct->posZ[i] = 0;
	}

	// Запустиим сервер на порту 1234, установив обработчики событий подключения клиентов
	clients = NLS_Init(1234, cl_Connect, cl_Disconnect);
	if(!clients)
	{ // максимальное количество клиентов 0, сервер запустить не удалось
		if(NL_Error())
			cout<<"Error: "<<NL_ErrorDescription()<<endl;
		else
			cout<<"Unknown critical error"<<endl;
		return 0;
	}

	// Установим обработчики сообщений от клиента
	NLC_SetInterpriter(MSG_CLIENT_STRUCT, InterpriterChar, 1, NL_BIN8);

	// Установим формат сообщений сервера
	NLS_SetInterpriter(MSG_SERVER_STRUCT, NULL, 1, NL_BIN8);

	GameDevice = createDevice(EDT_OPENGL, 
		dimension2d<s32>(1024,768), 32, false, false, false);
	
	driver = GameDevice->getVideoDriver();
	smgr = GameDevice->getSceneManager();
	guienv = GameDevice->getGUIEnvironment();
	
	GameDevice->setWindowCaption(L"Network library server test application!");
	
	video::SColor backColor;
	backColor.set(255,90,90,156);

	smgr->addCameraSceneNode();

	//mesh = smgr->getMesh("media/ball.3ds");
	//meshnode[0] = smgr->addAnimatedMeshSceneNode(mesh);
	//meshnode[0]->setScale(vector3df(0.1f,0.1f,0.1f));

	// Запустим вечный цикл сервера
	while(GameDevice->run())
	{
		
		//if (GameDevice->isWindowActive())
		//{
			u32 time = GameDevice->getTimer()->getTime();			
			driver->beginScene(true, true, backColor);			
			smgr->drawAll();			
			guienv->drawAll();
			NLS_Refresh();						
			driver->endScene();
			//NLS_Refresh();

			//если клиент присоединился, но на своей(читай клиентской) части, то посылаем ему его ид на сервере
			//и возвращаем wait в false
			if(wait==true)
			{
				binary.length = sizeof(struct IdStruct);
				binary.data = id_struct;
				NLS_Message(id_struct->client_id, MSG_SERVER_STRUCT, binary);
				wait=false;
			}

			//шлём сообщения клиентам
			//if(sm_struct != sp_struct)
			//{
			if(connect[0]==true)
			{				
				NLS_Message(0, MSG_SERVER_STRUCT, binary);				
			}
			if(connect[1]==true)
			{
				NLS_Message(0, MSG_SERVER_STRUCT, binary);				
			}
			if(connect[2]==true)
			{				
				NLS_Message(0, MSG_SERVER_STRUCT, binary);				
			}
			if(connect[3]==true)
			{
				NLS_Message(0, MSG_SERVER_STRUCT, binary);				
			}	
			//sm_struct = sp_struct;
			//}
			//дальше продолжу этот же код, с изменением числа элемента массива connect

			//фиксируем фпс чтоб не съедало всю ОЗУ
			GameDevice->sleep((1000/100) - (time - GameDevice->getTimer()->getTime()));			
		//}		
	}	
	
	delete id_struct;
	delete s_struct;
	delete sp_struct;
	delete sm_struct;
	NLS_Close();
	GameDevice->drop();	//граф девайс игры удаляем
}
