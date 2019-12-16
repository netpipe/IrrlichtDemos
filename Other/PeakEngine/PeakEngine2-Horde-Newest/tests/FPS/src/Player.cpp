
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

PlayerEntity::PlayerEntity() : Entity()
{
}
PlayerEntity::~PlayerEntity()
{
}

bool PlayerEntity::load(std::string name, peak::Buffer *data)
{
	printf("Loading player entity.\n");
	// Load entity file
	if (!Entity::load(name, data)) return false;
	getScript()->registerVariable(peak::UserClass("PlayerEntity", this), "entity");
	return true;
}
void PlayerEntity::update(float msecs)
{
	peak::Body *body = getBody("playerbody");
	// Foot raycasting
	peak::Vector3D pos = getProperty("position")->getVector3D();
	peak::CollisionInfo collision = peak::Game::get()->getWorld()->castRay(pos, pos + peak::Vector3D(0.0, -2.0, 0.0));
	if (collision.object != 0)
	{
		float distance = collision.lambda;
		
		if (distance < 0.5)
		{
			// Reset player position
			pos.y = pos.y + (0.5 - distance) * 2;
			getProperty("position")->set(pos);
			peak::Vector3D vel = body->getLinearVelocity();
			vel.y = 0;
			//body->setLinearVelocity(vel);
				
			bool onground = true;
				
			// Move player
			// TODO: Don't walk through walls. Directly setting the velocity is bad.
			/*if sprint == 0 then
				setriflespeed = 30;
				newvel = peak.Vector3D((right - left) * 10, 0, (back - forward) * 10);
			else
				setriflespeed = 20;
				newvel = peak.Vector3D((right - left) * 10, 0, (-forward * sprint * 2) * 10);
			end*/
			vel = peak::Vector3D((right - left) * 10, 0, (back - forward) * 10);
			vel.rotateXZ(-yaw);
			body->setLinearVelocity(vel);
			body->setAngularVelocity(peak::Vector3D(0, 0, 0));
		}
	}
	
	Entity::update(msecs);
}

void PlayerEntity::setMovement(int forward, int back, int left, int right)
{
	printf("setMovement!\n");
	this->forward = forward;
	this->back = back;
	this->left = left;
	this->right = right;
}

void PlayerEntity::setRotation(float yaw)
{
	this->yaw = yaw;
}

PlayerFactory PlayerFactory::factory;

