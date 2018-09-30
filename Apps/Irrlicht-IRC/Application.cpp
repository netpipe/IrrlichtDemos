//author randomMesh

#include "Application.h"


#include <fstream>

//#define test
#ifdef test
	const std::string serverName("127.0.0.1");
	const unsigned short serverPort = 6697;
	const std::string channelName("#ming");
	int aha = rand();
	const std::string botName("aBot");
#else if
    const std::string serverName("irc.freenode.net");
	const unsigned short serverPort = 6667;
	const std::string channelName("#irrlicht2");
	int aha = rand();
	const std::string botName("aBot2");
#endif


Application::Application() :    device(0), gui(0), server(0), running(false) {	}

Application::~Application()
{

	sendMessage("QUIT :Bye\r\n");

	delete gui;
	device->drop();
}


void *runServer(void *param)
{
    Server* server = (Server*) param;

    server->connectToServer();

}

bool Application::init()
{
	this->device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(800, 600), 16, false);
	if (!this->device)
		return false;
	this->device->setEventReceiver(this);
	this->device->setWindowCaption(L"Irrlicht Help Bot");

	//todo: read configuration
	gui = new Gui(device->getGUIEnvironment());

	//configure network
    ServerConfig config;
    config.host = serverName;
    config.port = serverPort;

    config.nickName = botName;
    config.channel = channelName;

    server = new Server(config);


    int rc = pthread_create(&thread, 0, runServer, (void *)server);
    if(rc){
        printf("Error:unable to create thread\n");
        return false;
    }
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

bool Application::sendMessage(const std::string& msg) const
{

	server->message(msg);

	return true;
}

bool Application::OnEvent(const irr::SEvent& event)
{
	switch(event.EventType)
	{
	case irr::EET_KEY_INPUT_EVENT:
	{
		if (!event.KeyInput.PressedDown)
		{
			switch (event.KeyInput.Key)
			{

			case irr::KEY_ESCAPE: running = false; device->closeDevice(); return true;

			case irr::KEY_RETURN:
			{
				const std::string message(irr::core::stringc(this->gui->getBox()->getText()).c_str());
				if (message.compare("") != 0)
				{
					sendMessage(message);
					gui->addLine(irr::core::stringw( botName.c_str() ) + " : ", message.c_str());
				}
				this->gui->getBox()->setText(L"");

			}
			return true;

			default: return false;
			}
		}
	}
	break;

	default: return false;
	}

	return false;
}
