
#include "Application.h"
typedef unsigned long DWORD;

#include <fstream>
#include <stdlib.h>

#define test
#ifdef test
	const std::string botName("aBot");
#else if
    const std::string serverName("irc.freenode.net");
	const unsigned short serverPort = 6667;
	const std::string botName("aBot");
#endif


Application::Application() :    device(0), gui(0), running(false) {	}

Application::~Application()
{

	//sendMessage("#tutorial", "QUIT :Bye\r\n");

	delete gui;
	device->drop();
}

#ifdef WIN32
DWORD WINAPI runServer(LPVOID param)
{
	Server* server = (Server*)param;

	server->connectToServer();
	return 0;
}
#else
void *runServer(void *param)
{
	Server* server = (Server*)param;

	server->connectToServer();

}
#endif


bool Application::init()
{
	this->device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(800, 450), 16, false);
	if (!this->device)
		return false;
	this->device->setEventReceiver(this);
	this->device->setWindowCaption(L"Irrlicht Help Bot");

	//todo: read configuration
	gui = new Gui(device->getGUIEnvironment());

	return true;
}

void Application::run() const
{
	irr::gui::IGUIEnvironment* const guienv = device->getGUIEnvironment();
	irr::video::IVideoDriver* const driver = device->getVideoDriver();

	while (device->run())
	{
	//	if (device->isWindowActive())
	//	{
			driver->beginScene(true, false, irr::video::SColor());
			guienv->drawAll();
			driver->endScene();
		//}
		device->sleep(22,0);
	}
}

bool Application::sendMessage(const std::string channel, std::string msg) {
	u32 cur = gui->getSelectedNodeIdx();
	serverList[cur]->message(channel, msg);

	return true;
}

bool Application::sendMe(const std::string channel, const std::string msg)
{
	u32 cur = gui->getSelectedNodeIdx();
	serverList[cur]->me(channel, msg);

	return true;
}

bool Application::sendCommand(std::string cmd, std::string param)
{
	if (cmd.compare("join") == 0) {
		joinChannel(param);
	}
	else if (cmd.compare("part") == 0) {
		leaveChannel(param);
	}
	else if (cmd.compare("me") == 0) {
		std::string channel(gui->getActiveTabName().c_str());
		sendMe(channel, param);
		gui->addLine(irr::core::stringw(botName.c_str()) + " : ", param.c_str());
	}


	return true;
}

bool Application::parsingCmd(std::string msg, std::string& cmd, std::string& param)
{
	if (msg.substr(0, 1).compare("/") == 0){
		cmd = msg.substr(1, msg.find_first_of(" ")-1);
		param = msg.substr(msg.find_first_of(" ") + 1, msg.length());
		return true;
	}

	return false;
}

void Application::joinChannel(std::string channel)
{
	if (!channel.empty()) {
		if (channel.substr(0, 1).compare("#"))
			channel = "#" + channel;

		if (gui->addChannelTab(irr::core::stringw(channel.c_str()))) {
			u32 cur = gui->getSelectedNodeIdx();
			serverList[cur]->joinToChannel(channel);
		}
	}
}

void Application::leaveChannel(std::string channel)
{
	if (!channel.empty()) {
		if (channel.substr(0, 1).compare("#"))
			channel = "#" + channel;

		gui->removeChannelTab(irr::core::stringw(channel.c_str()));

		u32 cur = gui->getSelectedNodeIdx();
		serverList[cur]->leaveToChannel(channel);
	}

}

bool Application::OnEvent(const irr::SEvent& event)
{
	switch(event.EventType)
	{
		case EET_KEY_INPUT_EVENT:
		{
			if (!event.KeyInput.PressedDown)
			{
				switch (event.KeyInput.Key)
				{

				case KEY_ESCAPE:
				{
					running = false;
					device->closeDevice();
					return true;
				}

				case KEY_RETURN:
				{
					if (gui->getJoinChannelDlg()->isVisible())
						return true;

					if (this->gui->getChatBox() == NULL) {
						return true;
					}
					const std::string message(irr::core::stringc(this->gui->getChatBox()->getText()).c_str());
					if (message.empty())
						return true;

					std::string cmd, param;

					if (parsingCmd(message, cmd, param)) {
						this->gui->getChatBox()->setText(L"");
						sendCommand(cmd, param);
					}
					else {
						std::string channel(gui->getActiveTabName().c_str());
						sendMessage(channel, message);
						gui->addLine(irr::core::stringw(botName.c_str()) + " : ", message.c_str());
					}

					if(this->gui->getChatBox() != NULL)
						this->gui->getChatBox()->setText(L"");

					return true;

				}


				default: return false;
				}
			}
		}
		case EET_GUI_EVENT:
		{

			if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
				u32 id = event.GUIEvent.Caller->getID();
				switch (id)
				{
				case GUI_ID_JOIN_CHANNEL_BUTTON:
				{
					gui->ShowJoinChannel();

					return true;
				}
				case GUI_ID_CONNECT_SERVER:
				{
					gui->ShowConnectServer();
					return true;
				}
				case GUI_ID_JOIN_CHANNEL_OK:
				{
					std::string channel = (irr::core::stringc(this->gui->getChannelNameEdit()->getText()).c_str());

					joinChannel(channel);

					gui->getJoinChannelDlg()->setVisible(false);
					return true;
				}
				case GUI_ID_SERVER_CONNECT_OK:
				{
					std::string server = (irr::core::stringc(this->gui->getServerAddressEdit()->getText()).c_str());
					std::string port = (irr::core::stringc(this->gui->getServerPortEdit()->getText()).c_str());

					CreateServer(server, port);

					std::string str = server + ":" + port;
					gui->addServerChild(irr::core::stringw(str.c_str()));

					gui->getConnectServerDlg()->setVisible(false);
					return true;
				}
				default:
					break;
				}

			}
			else if (event.GUIEvent.EventType == EGET_TREEVIEW_NODE_SELECT) {
				IGUITreeViewNode* node = gui->getSelectedNode();
				if (node->hasChildren())
					return true;

				u32 idx = gui->getSelectedNodeIdx();
				gui->SetVisibleTabeServer(idx);
			}

		}

	default:
		return false;
	}

	return false;
}

void Application::CreateServer(std::string serverName, std::string serverPort)
{
	//configure network
	ServerConfig config;
	config.host = serverName;
	config.port = atoi(serverPort.c_str());

	config.nickName = botName;

	Server* server = new Server(config);
	serverList.push_back(server);

	DWORD thread;
#ifdef WIN32
	CreateThread(0, 0, runServer, server, 0, &thread);
#else
	int rc = pthread_create(&thread, 0, runServer, (void *)server);
	if (rc) {
	printf("Error:unable to create thread\n");
//	return false;
	}
#endif


	//device->sleep(6220, 0); // allow server to auth non ident'd client

}
