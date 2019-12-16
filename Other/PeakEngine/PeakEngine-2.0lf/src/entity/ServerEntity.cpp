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

#include "peakengine/entity/ServerEntity.hpp"
#include "peakengine/core/Server.hpp"

namespace peak
{
	ServerEntity::ServerEntity(Server *server) : Entity(server), owner(0)
	{
	}
	ServerEntity::~ServerEntity()
	{
	}

	void ServerEntity::sendMessage(Buffer *buffer, bool reliable)
	{
		((Server*)getManager())->sendEntityMessage(this, buffer, reliable);
	}
	void ServerEntity::receiveMessage(Buffer *buffer)
	{
	}

	void ServerEntity::setOwner(unsigned int owner)
	{
		this->owner = owner;
	}
	unsigned int ServerEntity::getOwner()
	{
		return owner;
	}

	void ServerEntity::onUpdate()
	{
	}
}
