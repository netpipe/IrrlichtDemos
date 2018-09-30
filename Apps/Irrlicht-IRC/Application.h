#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <string>
#include <irrlicht.h>
#include "gui.h"
#include "server.h"
#include <pthread.h>

class Application : public irr::IEventReceiver
{

public:

	Application();

	~Application();

	bool init();

	void run() const;

	bool OnEvent(const irr::SEvent& event);


	Gui* gui;

private:

	irr::IrrlichtDevice* device;

	Server *server;

    pthread_t thread;

	bool running;

	bool sendMessage(const std::string& msg) const;

};


extern Application *g_app;

#endif /* APPLICATION_H_ */
