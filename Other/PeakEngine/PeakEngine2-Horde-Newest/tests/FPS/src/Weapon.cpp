
/*
Copyright (C) 2008  Mathias Gottschlag

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "Player.h"

#include "core/Script.h"
#include "core/Property.h"
#include "core/Game.h"
#include "physics/World.h"
#include "physics/Body.h"

WeaponEntity::WeaponEntity() : Entity()
{
}
WeaponEntity::~WeaponEntity()
{
}

bool WeaponEntity::load(std::string name, peak::Buffer *data)
{
	printf("Loading weapon entity.\n");
	// Load entity file
	if (!Entity::load(name, data)) return false;
	getScript()->registerVariable(peak::UserClass("WeaponEntity", this), "entity");
	return true;
}
void WeaponEntity::update(float msecs)
{
	Entity::update(msecs);
}

WeaponFactory WeaponFactory::factory;

