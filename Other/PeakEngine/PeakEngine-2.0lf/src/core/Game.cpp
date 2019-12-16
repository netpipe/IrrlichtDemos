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

#include "peakengine/core/Game.hpp"
#include "peakengine/entity/EntityFactory.hpp"

namespace peak
{
	Game::Game()
	{
	}
	Game::~Game()
	{
	}

	void Game::setEngine(Engine *engine)
	{
		this->engine = engine;
	}
	Engine *Game::getEngine()
	{
		return engine;
	}

	void Game::addEntityFactory(EntityFactory *factory, std::string name)
	{
		factories.insert(std::pair<std::string, EntityFactory*>(name, factory));
	}
	EntityFactory *Game::getEntityFactory(std::string name)
	{
		std::map<std::string, EntityFactory*>::iterator it = factories.find(name);
		if (it == factories.end())
			return 0;
		return it->second;
	}
}
