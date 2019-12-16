
#include "core/GameEngine.h"
#include "core/Game.h"
#include "core/Entity.h"
#include "core/Model.h"
#include "core/Keycode.h"
#include "graphics/GraphicsEngine.h"
#include "graphics/SceneNode.h"

#include <math.h>


class CubeEntity : public peak::Entity
{
	public:
		CubeEntity();
		~CubeEntity();
		
		virtual bool init(std::string type);
		
		virtual void update(float msecs);
		
		void moveLeft(bool move);
		void moveRight(bool move);
		
		virtual void applyCommand(peak::EntityCommand *command);
	private:
		peak::Model *model;
		int testdata;
		unsigned char left;
		unsigned char right;
};

CubeEntity *entity = 0;

CubeEntity::CubeEntity() : Entity()
{
	testdata = 0;
	entity = this;
	left = 0;
	right = 0;
}
CubeEntity::~CubeEntity()
{
}

bool CubeEntity::init(std::string type)
{
	// Load model
	model = new peak::Model;
	if (!model->load("tank"))
	{
		delete model;
		model = 0;
		return false;
	}
	//add(model);
	add(&testdata);
	add(&left);
	add(&right);
	
	Entity::init(type);
	
	return true;
}

void CubeEntity::update(float msecs)
{
	peak::Entity::update(msecs);
	testdata += left;
	testdata -= right;
	peak::SceneNode *camera = peak::GraphicsEngine::get()->getDefaultCamera();
	printf("%d.\n", testdata);
	camera->setPosition(peak::Vector3D(sin((float)testdata / 100 * 3.1415) * 10, 0, cos((float)testdata / 100 * 3.1415) * 10));
	camera->setRotation(peak::Vector3D(0, (float)testdata / 100 * 3.1415, 0));
}

void CubeEntity::moveLeft(bool move)
{
	peak::EntityCommand *command = new peak::EntityCommand();
	command->data.writeByte(move);
	command->data.writeByte(right);
	command->timestamp = peak::Game::get()->getTime() + 1;
	sendCommand(command);
	//left = move;
}
void CubeEntity::moveRight(bool move)
{
	peak::EntityCommand *command = new peak::EntityCommand();
	command->data.writeByte(left);
	command->data.writeByte(move);
	command->timestamp = peak::Game::get()->getTime() + 1;
	sendCommand(command);
	//right = move;
}

void CubeEntity::applyCommand(peak::EntityCommand *command)
{
	//peak::Entity::applyCommand(command);
	command->data.setPosition(0);
	left = command->data.readByte();
	right = command->data.readByte();
}

peak::Entity *createCubeEntity(void)
{
	return new CubeEntity;
}

void OnKeyChanged(peak::Keycode key, bool state)
{
	//printf("Key %d changed.\n", (int)key);
	if (!entity) return;
	if (key == peak::EK_LEFT)
	{
		entity->moveLeft(state);
	}
	else if (key == peak::EK_RIGHT)
	{
		entity->moveRight(state);
	}
}
void OnMouseChanged(int x, int y, int z, int buttons)
{
}

int main(int argc, char **argv)
{
	peak::Game::get()->addEntityType("cube", createCubeEntity);
	peak::GameEngine *engine = peak::GameEngine::get();
	peak::GraphicsEngine::get()->setKeyCallback(OnKeyChanged);
	if (!engine->run("test/data"))
	{
		return -1;
	}
	return 0;
}

