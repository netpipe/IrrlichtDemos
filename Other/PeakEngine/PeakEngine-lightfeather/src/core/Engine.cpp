/*
Copyright (c) 2009, Mathias Gottschlag

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "peakengine/core/Engine.hpp"
#include "peakengine/core/Game.hpp"
#include "peakengine/core/Client.hpp"
#include "peakengine/core/Server.hpp"

#include <enet/enet.h>

namespace peak
{
	Engine::Engine() : game(0), server(0), client(0)
	{
		// Initialize networking
		enet_initialize();
		// FIXME: This limits us to one existing instance of Engine.
	}
	Engine::~Engine()
	{
		// Shutdown networking
		enet_deinitialize();
	}

	bool Engine::loadGame(std::string filename)
	{
		// TODO
		return false;
	}
	bool Engine::loadGame(Game *game)
	{
		this->game = game;
		game->setEngine(this);
		return true;
	}
	Game *Engine::getGame()
	{
		return game;
	}

	void Engine::setGameDirectory(std::string directory)
	{
		this->directory = directory;
	}
	std::string Engine::getGameDirectory()
	{
		return directory;
	}

	bool Engine::run()
	{
		// Check game
		if (!game)
			return false;
		if (directory == "")
			return false;
		// Initialize engine
		stopping.lock();
		if (!game->init())
		{
			stopping.unlock();
			return false;
		}
		// TODO
		// Wait for engine to be closed
		stopping.wait();
		stopping.unlock();
		// Clean up again
		stopClient();
		stopServer();
		game->shutdown();
		// TODO
		return true;
	}
	bool Engine::runFromEditor()
	{
		// Check game
		if (!game)
			return false;
		if (directory == "")
			return false;
		// Initialize engine
		// TODO
		// Start game
		// TODO
		return false;
	}
	bool Engine::runServer(BufferPointer serverdata)
	{
		// Check game
		if (!game)
			return false;
		if (directory == "")
			return false;
		// Initialize engine
		stopping.lock();
		// TODO
		// Create server
		Server *server = createServer(serverdata);
		if (!server)
		{
			stopping.unlock();
			return false;
		}
		// Wait for engine to be closed
		stopping.wait();
		stopping.unlock();
		// Clean up again
		stopClient();
		stopServer();
		// TODO
		return false;
	}
	void Engine::stop()
	{
		stopping.lock();
		stopping.signal();
		stopping.unlock();
	}

	Server *Engine::createServer(BufferPointer serverdata)
	{
		// Stop existing server
		if (server)
			stopServer();
		// Create new server
		server = game->createServer(serverdata);
		if (!server)
			return 0;
		// Start thread
		server->startThread();
		return server;
	}
	void Engine::stopServer()
	{
		if (!server)
			return;
		server->shutdown();
		delete server;
	}
	Server *Engine::getServer()
	{
		return server;
	}
	Client *Engine::createClient(std::string address, unsigned int port,
		unsigned int timeout)
	{
		// Stop existing server
		if (client)
			stopClient();
		// Create new client
		client = game->createClient(address, port, timeout);
		if (!client)
			return 0;
		// Start thread
		client->startThread();
		return client;
	}
	Client *Engine::createLocalClient()
	{
		if (!server)
			return 0;
		// Stop existing server
		if (client)
			stopClient();
		// Create new client
		client = game->createClient(server);
		if (!client)
			return 0;
		// Start thread
		client->startThread();
		return client;
	}
	void Engine::stopClient()
	{
		if (!client)
			return;
		client->shutdown();
		delete client;
	}
	Client *Engine::getClient()
	{
		return client;
	}
}
