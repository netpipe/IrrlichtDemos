#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <string>
#include <vector>
#include <irrlicht.h>
#include "gui.h"
#include "server.h"

#ifdef WIN32
	#include <windows.h>
#else
	#include <pthread.h>
#endif


class Application : public irr::IEventReceiver
{

public:

	Application();

	~Application();

	bool init();

	void run() const;

	bool OnEvent(const irr::SEvent& event);
	
	Gui* gui;

	void CreateServer(std::string serverName, std::string serverPort);

	
private:

	irr::IrrlichtDevice* device;

	std::vector<Server*> serverList;
		
#ifdef WIN32
	std::vector<DWORD> threadList;
#else
	std::vector<pthread_t> threadList;
#endif
    

	bool running;

	bool sendMessage(const std::string channel, const std::string msg);

	bool sendMe(const std::string channel, const std::string msg);

	bool sendCommand(std::string cmd, std::string param);

	

	bool parsingCmd(std::string msg, std::string& cmd, std::string& param);

	void joinChannel(std::string channel);

	void leaveChannel(std::string channel);

	
};


extern Application *g_app;

#endif /* APPLICATION_H_ */
