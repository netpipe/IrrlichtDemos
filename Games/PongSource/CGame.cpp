#include "CGame.h"

//author : tigrang

CGame *haaa;

int main (){
haaa= new CGame();
while (1)
{
haaa->render();
}



}

CGame::CGame(  ) : CEventReceiver()
{
	this->device = createDevice(video::EDT_OPENGL, dimension2d<u32>(640, 480), 32, 0, false, false, this);

	Controls controls;
	controls.up = KEY_UP;
	controls.down = KEY_DOWN;

	p1 = new CPlayer(0, 200, 15, 100, controls, this);
	p1->setPlayerId(1);

	p2 = new CPlayer(625, 200, 15, 100, controls, this);
	p2->setPlayerId(2);

	ball = new CPongBall(200, 200, 10, 10, p1, p2);

	render();
}

CGame::~CGame()
{
}

/*bool CGame::onEvent(SEvent event)
{
	return false;
}
*/

void CGame::render()
{
	IGUIFont *font = device->getGUIEnvironment()->getBuiltInFont();

	while (device->run())
	{
		device->getVideoDriver()->beginScene();

		drawMap();

		p1->update();
		p1->draw();

		p2->update();
		p2->draw();

		ball->checkCollision(p1, p2);
		ball->update();
		ball->draw();


		if (ball->getX() < -20)
			p2->setScore( p2->getScore() + 1 );

		if (ball->getX() > 660)
			p1->setScore( p1->getScore() + 1 );

		stringw p1Score = "";
		p1Score += p1->getScore();

		stringw p2Score = "";
		p1Score += p2->getScore();

		font->draw(p1Score, rect<s32>(50, 50, 100, 100), SColor(255, 255, 255, 255));
		font->draw(p2Score, rect<s32>(590, 50, 100, 100), SColor(255, 255, 255, 255));

		device->getGUIEnvironment()->drawAll();

		device->getVideoDriver()->endScene();

		stringw cap = "Pong";
		cap += " [FPS: ";
		cap += device->getVideoDriver()->getFPS();
		cap += "]";
		device->setWindowCaption(cap.c_str());

		device->sleep(5,0);
	}
}

void CGame::drawMap()
{
	int width = device->getVideoDriver()->getScreenSize().Width;
	int height = device->getVideoDriver()->getScreenSize().Height;

	int mid = width / 2;

	for (int y = 5; y < height; y++)
	{
		if (y % 5 == 0)
		{
			device->getVideoDriver()->draw2DRectangle(SColor(255, 255, 255, 255),
			rect<s32>(mid, y, mid + 5, y + 10));
			y += 15;
		}
	}
}
