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

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//нужно проверить сенд
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#define MAX_CLIENTS 10

int Clients[MAX_CLIENTS]; // flags of clients connect

IrrlichtDevice *GameDevice;
video::IVideoDriver* driver;          //video driver this game
scene::ISceneManager* smgr;           //scene manager this game
gui::IGUIEnvironment* guienv;         //gui this game
IAnimatedMeshSceneNode* meshnode[10]; //мешноды дл€ клиентов
IAnimatedMesh* mesh;                  //меш - модель клиента
ISceneNode* Parent;                     //сценнод, нужный лишь чтобы удал€ть мешноды клиентов корректным способом

//структура дл€ получени€ сообщений от сервера
struct IncomeStruct{
  f32 posX[10];
  f32 posY[10];
  f32 posZ[10];
  bool isConnected[10];
};

//структура дл€ отправлени€ сообщений серверу
struct ClientStruct{
  f32 posX;
  f32 posY;
  f32 posZ;
  bool isConnected;
};

//структура дл€ получени€ серверного идентификатора клиента дл€ клиента
struct IdStruct
{
	int client_id;
};

char* ip; //ип адрес сервера

IncomeStruct *c_struct; //этот указатель используетьс€ дл€ характеристик всех клиентов
IdStruct     *id_struct;//этот указатель используетс€ дл€ получени€ своего ID от сервера
ClientStruct *my_struct;//этот указатель используетс€ дл€ отправки характеристик клиента на сервер
int my_id=32;           //стартовый ид. ƒанное число использовано так как ноль также может быть id-ом.
                        // ароче чтоб глюков небыло
bool init[10];          //состо€ние инициализации всех клиентов
bool init_my_id=false;  //получен ли мой(клиентский) ид?
bool send=false;

//обработчик клавы и мыши
class MyEventReceiver : public IEventReceiver
{
public:
	bool OnEvent(const SEvent &event)
	{
		if (event.EventType == EET_KEY_INPUT_EVENT &&
		event.KeyInput.Key == KEY_UP &&
		event.KeyInput.PressedDown == false)
		{
			meshnode[my_id]->setPosition(vector3df(meshnode[my_id]->getAbsolutePosition().X,
				                               meshnode[my_id]->getAbsolutePosition().Y+0.5,
								               meshnode[my_id]->getAbsolutePosition().Z));
			send=true;
		}
		else
		if (event.EventType == EET_KEY_INPUT_EVENT &&
			event.KeyInput.Key == KEY_DOWN &&
			event.KeyInput.PressedDown == false)
		{
			meshnode[my_id]->setPosition(vector3df(meshnode[my_id]->getAbsolutePosition().X,
			                                   meshnode[my_id]->getAbsolutePosition().Y-0.5,
								               meshnode[my_id]->getAbsolutePosition().Z));
			send=true;
		}
		else
		if (event.EventType == EET_KEY_INPUT_EVENT &&
			event.KeyInput.Key == KEY_RIGHT &&
			event.KeyInput.PressedDown == false)
		{
			meshnode[my_id]->setPosition(vector3df(meshnode[my_id]->getAbsolutePosition().X+0.5,
			                                   meshnode[my_id]->getAbsolutePosition().Y,
								               meshnode[my_id]->getAbsolutePosition().Z));
			send=true;
		}
		else
		if (event.EventType == EET_KEY_INPUT_EVENT &&
			event.KeyInput.Key == KEY_LEFT &&
			event.KeyInput.PressedDown == false)
		{
			meshnode[my_id]->setPosition(vector3df(meshnode[my_id]->getAbsolutePosition().X-0.5,
				                               meshnode[my_id]->getAbsolutePosition().Y,
								               meshnode[my_id]->getAbsolutePosition().Z));
			send=true;
		}
		return false;
	}
};

MyEventReceiver receiver;

//обработчик сообщений от сервера
void InterpriterStruct(union NL_Variable *var)
{

  if(init_my_id==false)
  {
	  id_struct = (struct IdStruct*)(var[0].bin.data);
	  my_id=id_struct->client_id;
	  meshnode[my_id] = smgr->addAnimatedMeshSceneNode(mesh);
	  init[my_id]=true;
	  init_my_id=true;
  }

	  c_struct = (struct IncomeStruct*)(var[0].bin.data);

	  /*if(my_id != i
		 &&	 c_struct->isConnected[i]==true
		 && init[i]==false)
	  {
		  meshnode[i] = smgr->addAnimatedMeshSceneNode(mesh);
		  init[i]=true;
		  meshnode[i]->setPosition(vector3df(c_struct->posX,c_struct->posY,c_struct->posZ));
	  }
	  */




}

bool Connect()
{
  // подключимс€ к серверу (текущий компьютер), к порту 1234

	ip = new char[16];
	cout<<endl<<"IP: ";
	cin>>ip;                     //вводим ип-адрес сервера
	if(NLC_Init(1234, ip))
		cout<<"Connection Ok\n";
	else
	{
		cout<<"Connection Faild\n";
		return true;
	}

  // ”становим обработчики сообщений от сервера
	cout<<endl<<"start initialising interpreter from server"<<endl;
  NLS_SetInterpriter(MSG_SERVER_STRUCT, InterpriterStruct, 1, NL_BIN8);
    cout<<endl<<"end initialising interpreter from server"<<endl;

  // ”становим формат сообщений клиента
	cout<<endl<<"start initialising interpreter from client"<<endl;
  NLC_SetInterpriter(MSG_CLIENT_STRUCT, NULL, 1, NL_BIN8);
    cout<<endl<<"end initialising interpreter from client"<<endl;

  delete ip;

  return false;
}

int main(int argv, char **argc)
{


	init[0]=false;
	init[1]=false;
	init[2]=false;
	init[3]=false;
	init[4]=false;
	init[5]=false;
	init[6]=false;
	init[7]=false;
	init[8]=false;
	init[9]=false;
	init[10]=false;


	struct NL_TBinary binary;
	my_struct = new struct ClientStruct;
	c_struct  = new struct IncomeStruct;
    id_struct = new struct IdStruct;

	Connect();

	GameDevice = createDevice(EDT_OPENGL,
		dimension2du(1024,768), 32, false, false, false, &receiver);

	driver = GameDevice->getVideoDriver();
	smgr = GameDevice->getSceneManager();
	guienv = GameDevice->getGUIEnvironment();

	GameDevice->setWindowCaption(L"Network library client test application!");

	video::SColor backColor;
	backColor.set(255,90,90,156);

	smgr->addCameraSceneNode();


	mesh = smgr->getMesh("media/ball.3ds");
	/*mesh[0] = smgr->getMesh("media/ball.3ds");
	mesh[1] = smgr->getMesh("media/ball.3ds");
	mesh[2] = smgr->getMesh("media/ball.3ds");
	mesh[3] = smgr->getMesh("media/ball.3ds");
	mesh[4] = smgr->getMesh("media/ball.3ds");
	mesh[5] = smgr->getMesh("media/ball.3ds");
	mesh[6] = smgr->getMesh("media/ball.3ds");
	mesh[7] = smgr->getMesh("media/ball.3ds");
	mesh[8] = smgr->getMesh("media/ball.3ds");
	mesh[9] = smgr->getMesh("media/ball.3ds");
	mesh[10] = smgr->getMesh("media/ball.3ds");
	meshnode[0] = smgr->addAnimatedMeshSceneNode(mesh[0]);
	meshnode[0]->setScale(vector3df(0.1f,0.1f,0.1f));*/

	//meshnode[0] = smgr->addAnimatedMeshSceneNode(mesh);



	// «апустим вечный цикл сервера
	while(GameDevice->run())
	{

		if (GameDevice->isWindowActive())
		{
			u32 time = GameDevice->getTimer()->getTime();
			driver->beginScene(true, true, backColor);
			smgr->drawAll();
			guienv->drawAll();
			driver->endScene();

			NLC_Refresh();


			//посылаем серверу информацию об этом(о нас) клиенте
			if(init_my_id==true)
			{
				my_struct->isConnected=true;
				my_struct->posX=meshnode[my_id]->getPosition().X;
				my_struct->posY=meshnode[my_id]->getPosition().Y;
				my_struct->posZ=meshnode[my_id]->getPosition().Z;
				binary.length = sizeof(struct ClientStruct);
				binary.data = my_struct;
				////////////////////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////////////////////////////////
				//нужно проверить сенд
				////////////////////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////////////////////////////////
				//if(send=true)
				//{
					NLC_Message(MSG_CLIENT_STRUCT, binary);
				//	send=false;
				//}

				////////////////////////////////////////////////////////////////////////////////////
				//инициализаци€ мешей других клиентов
				////////////////////////////////////////////////////////////////////////////////////
				if(c_struct->isConnected[0]==true && init[0]==false && my_id!=0)
				{
					meshnode[0] = smgr->addAnimatedMeshSceneNode(mesh);
					init[0]=true;
				}
				if(c_struct->isConnected[1]==true && init[1]==false && my_id!=1)
				{
					meshnode[1] = smgr->addAnimatedMeshSceneNode(mesh);
					init[1]=true;
				}
				if(c_struct->isConnected[2]==true && init[2]==false && my_id!=2)
				{
					meshnode[2] = smgr->addAnimatedMeshSceneNode(mesh);
					init[2]=true;
				}
				if(c_struct->isConnected[3]==true && init[3]==false && my_id!=3)
				{
					meshnode[3] = smgr->addAnimatedMeshSceneNode(mesh);
					init[3]=true;
				}
				if(c_struct->isConnected[4]==true && init[4]==false && my_id!=4)
				{
					meshnode[4] = smgr->addAnimatedMeshSceneNode(mesh);
					init[4]=true;
				}
				if(c_struct->isConnected[5]==true && init[5]==false && my_id!=5)
				{
					meshnode[5] = smgr->addAnimatedMeshSceneNode(mesh);
					init[5]=true;
				}
				////////////////////////////////////////////////////////////////////////////////////
				//конец инициализации мешей других клиентов
				////////////////////////////////////////////////////////////////////////////////////



				////////////////////////////////////////////////////////////////////////////////////
				//позиционирование мешей других клиентов(выполн€етс€ пока программа работет)
				////////////////////////////////////////////////////////////////////////////////////
				if(init[0]==true && my_id!=0)
				{
					meshnode[0]->setPosition(vector3df(c_struct->posX[0],c_struct->posY[0],c_struct->posZ[0]));
				}
				if(init[1]==true && my_id!=1)
				{
					meshnode[1]->setPosition(vector3df(c_struct->posX[1],c_struct->posY[1],c_struct->posZ[1]));
				}
				if(init[2]==true && my_id!=2)
				{
					meshnode[2]->setPosition(vector3df(c_struct->posX[2],c_struct->posY[2],c_struct->posZ[2]));
				}
				if(init[3]==true && my_id!=3)
				{
					meshnode[3]->setPosition(vector3df(c_struct->posX[3],c_struct->posY[3],c_struct->posZ[3]));
				}
				if(init[4]==true && my_id!=4)
				{
					meshnode[4]->setPosition(vector3df(c_struct->posX[4],c_struct->posY[4],c_struct->posZ[4]));
				}
				if(init[5]==true && my_id!=5)
				{
					meshnode[5]->setPosition(vector3df(c_struct->posX[5],c_struct->posY[5],c_struct->posZ[5]));
				}
				if(init[5]==true && my_id!=5)
				{
					meshnode[5]->setPosition(vector3df(c_struct->posX[5],c_struct->posY[0],c_struct->posZ[5]));
				}
				////////////////////////////////////////////////////////////////////////////////////
				//конец позиционировани€ мешей других клиентов
				////////////////////////////////////////////////////////////////////////////////////



				////////////////////////////////////////////////////////////////////////////////////
				//удаление мешей других клиентов
				////////////////////////////////////////////////////////////////////////////////////
				if(c_struct->isConnected[0]==false && init[0]==true && my_id!=0)
				{
					Parent=meshnode[0]->getParent();
					Parent->removeChild(meshnode[0]);
					init[0]=false;
				}
				if(c_struct->isConnected[1]==false && init[1]==true && my_id!=1)
				{
					Parent=meshnode[1]->getParent();
					Parent->removeChild(meshnode[1]);
					init[1]=false;
				}
				if(c_struct->isConnected[2]==false && init[2]==true && my_id!=2)
				{
					Parent=meshnode[2]->getParent();
					Parent->removeChild(meshnode[2]);
					init[2]=false;
				}
				if(c_struct->isConnected[3]==false && init[3]==true && my_id!=3)
				{
					Parent=meshnode[3]->getParent();
					Parent->removeChild(meshnode[3]);
					init[3]=false;
				}
				if(c_struct->isConnected[4]==false && init[4]==true && my_id!=4)
				{
					Parent=meshnode[4]->getParent();
					Parent->removeChild(meshnode[4]);
					init[4]=false;
				}
				if(c_struct->isConnected[5]==false && init[5]==true && my_id!=5)
				{
					Parent=meshnode[5]->getParent();
					Parent->removeChild(meshnode[5]);
					init[5]=false;
				}
				////////////////////////////////////////////////////////////////////////////////////
				//конец удалени€ мешей других клиентов
				////////////////////////////////////////////////////////////////////////////////////


			}

			//фиксируем фпс чтоб не съедало всю ќ«”
			GameDevice->sleep((1000/100) - (time - GameDevice->getTimer()->getTime()));
		}
	}


	NLC_Close();
	delete id_struct;
	delete c_struct;
	delete my_struct;
	GameDevice->drop();	//граф девайс игры удал€ем
}
