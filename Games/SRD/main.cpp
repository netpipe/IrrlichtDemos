// Includes
#include <math.h>
#include <irrlicht.h>
#include "cAsteroid.h"
#include "cShip.h"
#include "cEventReceiver.h"

// Namespaces
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
//using namespace irrklang;

// Consts
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

const bool FULLSCREEN = false;

const int ID_HUD_TAB = 2;
const int ID_HELP_TAB = 1;

// Object types
const int NONE = 0;
const int PLAYER = 1;


const SColor WHITE	(255,255,255,255);
const SColor BLACK	(255,0,	0,0);
const SColor RED	(255,255,0,0);
const SColor GREEN	(255,0,255,0);
const SColor BLUE	(255,0,0,255);
const SColor YELLOW	(255,255,255,0);
const SColor YELLOW2(255,200,200,0);
const SColor YELLOW3(255,150,150,0);

// Irrlicht and irrklang
IrrlichtDevice *device;
IVideoDriver *driver;
IGUIEnvironment *guienv;
//ISoundEngine *sound;

// Font
IGUIFont *font;
IGUIFont *bigFont;

// Flags
bool twoPlayers(false);			// one or two players
bool shieldToggle(false);		// press and hold for shield, or toggle on and off

// Random number generator
//RandGen random;

// Players
cShip player;

cShip player1;			// Player 1
cShip player2;			// Player 2

// Event receiver
cEventReceiver receiver;

// Asteroid list
list<cAsteroid*> Asteroids;


int asteroidCounter;			// adds 1 every time an asteroid is added, used for asteroid id's


bool help;						// Flag for showing help menu


int level;						// Current level

// Global functions
void InitPlayers();
void InitAsteroids();
void ProcessKeyboardInput(int deltaTime);
void ProcessMouseInput(int deltaTime);
void UpdatePlayer(int deltaTime);
void UpdateAsteroids(int deltaTime);
void CheckCollisions(int deltaTime);				// Checks for collisions
void ShowFPS();

void WrapObject(cObject &obj);													// wraps an object around the screen
bool Collision(cObject obj1, cObject obj2);										// Returns true if the two objects have collided
void BounceObjects(cObject &obj1, cObject &obj2, float speed1, float speed2);	// Bounces two objects off eachother with limited speed if wanted


void AddAsteroid(vector2df pos, int size);	// adds an asteroid from list at position

void RemoveAsteroid(int id);			// removes an asteroid by its id
void RemoveAsteroid(cAsteroid *a);		// removes an asteroid


void AddInterface();
void UpdateInterface();


// Main -------------------------------------------------------------------------------------------
int main()
{
	// Create device
	device = createDevice(EDT_OPENGL, dimension2du(SCREEN_WIDTH, SCREEN_HEIGHT), 32, FULLSCREEN);

	// Get pointers
	driver = device->getVideoDriver();
	guienv = device->getGUIEnvironment();

	// Create irrklang

	// Set title
	device->setWindowCaption(L"Space Rock Defense by Micah Welch");

	// Hide cursor
	device->getCursorControl()->setVisible(false);

	// Get font
	font = guienv->getFont("data/fonts/fontDefault.bmp");

	if(font)
	{
		guienv->getSkin()->setFont(font);
		guienv->getSkin()->setColor(EGDC_BUTTON_TEXT, WHITE);
	}
	else
		printf("ERROR: Could not load font.\n");

	// Play background music
//	sound->play2D("Data/dark.ogg", true);

	asteroidCounter = 0;

	InitPlayers();

	// testing animated texture
	ITexture *explosion = driver->getTexture("data/explosion.png");
	ITexture *asteroid = driver->getTexture("data/asteroidanim.png");


	AddInterface();


	// Create event receiver
	device->setEventReceiver(&receiver);

	while(device->run() && driver)
	{
		if (device->isWindowActive())
		{
			// keeps the game updates constant
			int static lastTime = 0;
			int time = device->getTimer()->getTime();
			int deltaTime = time - lastTime;
			lastTime = time;

//			printf("%d, %d, %d\n", lastTime, time, deltaTime);

			ProcessKeyboardInput(deltaTime);
			ProcessMouseInput(deltaTime);

			UpdatePlayer(deltaTime);
			UpdateAsteroids(deltaTime);

			CheckCollisions(deltaTime);

			driver->beginScene(true, true, SColor(255,0,0,0));

			// draw background
			driver->draw2DImage(driver->getTexture("data/space1.jpg"), rect<int>(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), rect<int>(0, 0, 1024, 768), 0, 0, false);

			// Draw player shield
			if(player.shield)
				driver->draw2DImage(driver->getTexture("data/shield.png"), rect<int>(player.pos.X - player.size - 3, player.pos.Y - player.size - 3, player.pos.X + player.size + 3, player.pos.Y + player.size + 3), rect<int>(0, 0, 64, 64), 0, 0, true);

			// Draw main player
			driver->draw2DPolygon(position2d<int>((int)player.pos.X, (int)player.pos.Y), player.size, player.color, 16);

			// Draw laser
			list<cLaser*>::Iterator l;

			for(l = player.Laser.begin(); l != player.Laser.end(); l++)
			{
				cLaser *laser = (*l);
				driver->draw2DPolygon(position2d<int>((int)laser->pos.X, (int)laser->pos.Y), laser->size, YELLOW, 6);// old laser
			}

//			for(int i = 0; i < player.maxShots; i++)
//			{
//				driver->draw2DLine(position2d<int>( (int)player.laser[i].pos.X, (int)player.laser[i].pos.Y), position2d<int>( (int)(player.laser[i].pos.X + cosf(player.laser[i].angle)*(180/PI) * 0.2), (int) (player.laser[i].pos.Y + sinf(player.laser[i].angle)*(180/PI) * 0.2)), GREEN);
//				driver->draw2DPolygon(position2d<int>((int)player.laser[i].pos.X, (int)player.laser[i].pos.Y), player.laser[i].size, GREEN, 6);// old laser
//			}

			// Draw line showing player angle
			driver->draw2DLine(position2d<int>( (int)player.pos.X, (int)player.pos.Y), position2d<int>( (int) (player.pos.X + cosf(player.angle)*(180/PI) * 0.5), (int) (player.pos.Y + sinf(player.angle)*(180/PI) * 0.5)) );

			// Draw asteroids
			for(list<cAsteroid*>::Iterator j = Asteroids.begin(); j != Asteroids.end(); j++)
			{
				cAsteroid *a = (*j);

				if(a->active)
				{
					driver->draw2DImage(driver->getTexture("data/asteroid.png"), rect<int>(a->pos.X - a->size, a->pos.Y - a->size, a->pos.X + a->size, a->pos.Y + a->size), rect<int>(0, 0, 256, 256), 0, 0, true);
					for(int z = a->health - 3; z < a->health + 3; z++)
						driver->draw2DPolygon(position2d<int>((int)a->pos.X, (int)a->pos.Y), z, RED, 16);
				}
				else
				{
					if(explosion)
						driver->draw2DImage(explosion, rect<int>(a->pos.X - a->size - 3, a->pos.Y - a->size - 3, a->pos.X + a->size + 3, a->pos.Y + a->size + 3), a->explosionFrames[a->currentFrame], 0, 0, true);
				}
			}

			// Draw health bar
			driver->draw2DRectangle(SColor(100, 255, 0, 0), rect<int>(5, 5, player.maxHealth, 15), 0);
			driver->draw2DRectangle(SColor(200, 255, 0, 0), rect<int>(5, 5, player.health, 15), 0);

			// Draw energy bar
			driver->draw2DRectangle(SColor(100, 0, 156, 255), rect<int>(5, 20, player.maxEnergy, 30), 0);
			driver->draw2DRectangle(SColor(200, 0, 156, 255), rect<int>(5, 20, player.energy, 30), 0);

			// Draw help text
//			if(font)
//				font->draw(L"F1 = Help", rect<int>(2, 36, SCREEN_WIDTH, 51), WHITE, false, true);

		}

		// Show frames per second
		ShowFPS();

		UpdateInterface();

		// Draw GUI
		guienv->drawAll();

		// End scene
		driver->endScene();
	}

	device->drop();

	return 0;
}

void AddAsteroid(vector2df pos, int size)
{
	// Define new asteroid
	cAsteroid *temp = new cAsteroid;
		temp->pos = pos;
///		temp->vel = vector2df(random.RandReal(-0.1,0.1),random.RandReal(-0.1,0.1));
temp->vel = vector2df((-0.2,0.1));
		temp->size = size;
		temp->mass = size;
		temp->fragments = 3;
		temp->health = size;
		temp->active = true;
		temp->countdownTimer = 1000;
		temp->currentFrame = 0;
		temp->frameTimer = 0;
		temp->id = asteroidCounter;

	// Add it to the list
	Asteroids.push_back(temp);

	// Add to counter
	asteroidCounter++;
}

void RemoveAsteroid(int id)
{
	list<cAsteroid*>::Iterator i;

	for(i = Asteroids.begin(); i != Asteroids.end(); i++)
	{
		if((*i)->id == id && (*i) != NULL)
		{
			printf("Asteroid with ID %d was removed.\n", (*i)->id);
			Asteroids.erase(i);
			return;
		}
	}
}

void RemoveAsteroid(cAsteroid *a)
{
	list<cAsteroid*>::Iterator i;

	for(i = Asteroids.begin(); i != Asteroids.end(); i++)
	{
		if((*i)== a && (*i) != NULL)
		{
			printf("Asteroid with ID %d was removed.\n", (*i)->id);
			Asteroids.erase(i);
			return;// Must have this or it will continue down the list giving bad error
		}
	}
}

void ProcessKeyboardInput(int deltaTime)
{
	// Quit
	if(receiver.keys[KEY_ESCAPE])
		device->closeDevice();

	// Player 1 - Turn left/right
	if(receiver.keys[KEY_KEY_A])
		player.angle -= player.turnSpeed * deltaTime;
	else if(receiver.keys[KEY_KEY_D])
		player.angle += player.turnSpeed * deltaTime;

	// Player 1 - Accelerate
 	if(receiver.keys[KEY_KEY_W])
	{
		vector2df acc( cosf(player.angle), sinf(player.angle) );
		acc.normalize() *= player.speed * deltaTime;
		player.vel += acc;
	}

	// Player 1 - Slow down (friction generator = updagre)
 	if(receiver.keys[KEY_KEY_S])
	{
		// Get angle of velocity, not ship
		float angle = degToRad(-player.vel.getAngle());

		// add friction
		vector2df acc( cosf(angle), sinf(angle) );
		acc.normalize() *= (player.speed/1) * deltaTime;
		player.vel -= acc;
	}

	// Get numbers
	if(receiver.keys[KEY_KEY_N])
	{
		printf("ASTEROIDS: %d\n", Asteroids.getSize());
		printf("LASER SHOTS: %d\n", player.Laser.getSize());
		receiver.keys[KEY_KEY_N] = false;
	}

	// Add an asteroid
	if(receiver.keys[KEY_KEY_G])
	{
 ///       AddAsteroid(vector2df(random.RandInt(0, SCREEN_WIDTH), random.RandInt(0, SCREEN_HEIGHT)), 48);
  AddAsteroid(vector2df(200,200), 48);
		receiver.keys[KEY_PLUS] = false;
	}

	// Delete an asteroid
	if(receiver.keys[KEY_MINUS])
	{
		// Only delete if the list is not empty
		if(Asteroids.getSize() > 0)
		{
			list<cAsteroid*>::Iterator a;
			a = Asteroids.begin();
			RemoveAsteroid((*a));
		}
		else
			printf("No asteroids to delete.\n");

		receiver.keys[KEY_MINUS] = false;
	}

	// Player 1 - Shield
	if(receiver.keys[KEY_KEY_Q])
	{
		if(shieldToggle)
		{
			player.shield = !player.shield;
			receiver.keys[KEY_KEY_Q] = false;

			if(player.shield == true){
//				sound->play2D("data/grow2.wav", false);
            }
			else{
	//			sound->play2D("data/shrink2.wav", false);
			}
		}
		else
		{
	//		if(player.shield == false)
	//			sound->play2D("data/grow2.wav", false);

			player.shield = true;
		}
	}
	else if(receiver.keys[KEY_KEY_Q] == false)
	{
		if(shieldToggle == false)
		{
			if(player.shield == true){}
//				sound->play2D("data/shrink2.wav", false);

			player.shield = false;
		}
	}

	// Fire laser
	if(receiver.keys[KEY_SPACE])
	{
		// Fire if laser is ready
		if(player.laserReady && !player.shield)
		{
//			sound->play2D("data/soundfx/laser_shoot.wav", false);

			// Add shot
			player.AddLaserShot();

			// Reset timer
			player.fireRateTimer = 0;
			player.laserReady = false;

			// Use energy
			player.SubtractEnergy(player.laserEnergyCost);
		}
	}

	// Help screen toggle
	if(receiver.keys[KEY_F1])
	{
		// Get help tab
		IGUITab *tab = (IGUITab*)guienv->getRootGUIElement()->getElementFromId(ID_HELP_TAB);

		// Toggle visible
		if(tab)
			tab->setVisible(help);

		help = !help;
		receiver.keys[KEY_F1] = false;
	}
}

void ProcessMouseInput(int deltaTime)
{
	// no mouse
}

void InitPlayers()
{
	// Ship
	player.pos = vector2df(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	player.size = 16;


}

void UpdatePlayer(int deltaTime)
{
	// wait, limited fire rate
	player.fireRateTimer += deltaTime;

	// Ready laser if enough time has passed
	if(player.fireRateTimer >= 400 - (player.energy/2))
		player.laserReady = true;

	list<cLaser*>::Iterator i;

	for(i = player.Laser.begin(); i != player.Laser.end(); i++)
	{
		cLaser *l = (*i);

		// Update position
		l->pos += l->vel * deltaTime;

		// Add to life
		l->age += deltaTime;

		// Wrap laser around screen
		WrapObject(*l);

		// Check age
		if(l->age >= l->lifeSpan)
			l->remove = true;

		// Remove
		if(l->remove == true)
		{
			player.Laser.erase(i);
			break;
		}
	}

	// Add velocity vector to position
	player.pos.X += player.vel.X * deltaTime;
	player.pos.Y += player.vel.Y * deltaTime;

	// get angle of velocity, not the ship
	float angle = degToRad(-player.vel.getAngle());

	// add friction
	vector2df acc( cosf(angle), sinf(angle) );
	acc.normalize() *= (player.speed/10) * deltaTime;
	player.vel -= acc;

	WrapObject(player);

	// if shield is up
	if(player.shield)
	{
		player.color = GREEN;
	}
	else
	{
		player.color = WHITE;
	}

	// Charge energy
	if(player.energy + deltaTime  * player.energyRechargeRate < player.maxEnergy)
		player.energy += deltaTime * player.energyRechargeRate;
	else
		player.energy = player.maxEnergy;

//	printf("%d / %d\n", (int)player.energy, (int)player.maxEnergy);

}

void UpdateAsteroids(int deltaTime)
{
	list<cAsteroid*>::Iterator i;

	for(i = Asteroids.begin(); i != Asteroids.end(); i++)
	{
		// Get asteroid by pointer
		cAsteroid *a = (*i);

		// add velocity vector to position
		a->pos.X += a->vel.X * deltaTime;
		a->pos.Y += a->vel.Y * deltaTime;

		WrapObject(*a);

		// Asteroid is active
		if(a->active)
		{
			a->frameTimer += deltaTime;

			if(a->frameTimer >= 60)// 30 FPS If you change this, you have to change countdown timer
			{
				a->currentFrame++;
				a->frameTimer = 0;
			}

			// If end of animation reached, start back at 0
			if(a->currentFrame >= 19)
				a->currentFrame = 0;
		}
		// If the asteroid is dying and the counter hasn't reached 0
		else if(a->active == false && a->countdownTimer > 0)
		{
			// Subtrack from countdown clock
			a->countdownTimer -= deltaTime;

//			printf("COUNTDOWN: %d\n", a->countdownTimer);

			// Increase frame timer
			a->frameTimer += deltaTime;

			// Update to next frame if enough time has passed and reset timer
			if(a->frameTimer >= 33)// 30 FPS If you change this, you have to change countdown timer
			{
				a->currentFrame++;
				a->frameTimer = 0;
			}

			// If end of animation reached, start back at 0
			if(a->currentFrame >= 29)
				a->currentFrame = 29;
		}
		// If asteroid is dying and the countdown timer is at 0
		else if(a->active == false && a->countdownTimer <= 0)
		{
			RemoveAsteroid(a);
			break;
		}
	}
}

void BounceObjects(cObject &obj1, cObject &obj2, float speed1, float speed2)
{
	//***********************************************************************************
	// TO DO: - Make it possible to bounce the ojbects at two different speeds
	//		  - Use ratios based on mass instead of set values
	//
	//***********************************************************************************

	// Variables
	vector2df c, d, impulse, impact;
	float impactSpeed;

	impact = obj1.vel - obj2.vel;

//	impact /= speed;// Keeps them from flying all over the screen

	// Limit impact
//	if(impact.getLength() > 0.5)
//	{
//		impact.normalize();
//		impact /= 2;
//	}

//	printf("IMPACT: %f\n", impact.getLength());

	impulse = vector2df(obj1.pos.X - obj2.pos.X, obj1.pos.Y - obj2.pos.Y);
	impulse.normalize();

	if(impulse.getLength() < 0)
		impulse *= -1;

	impactSpeed = impact.dotProduct(impulse);

	if(impactSpeed < 0)
		impactSpeed *= -1;

	impulse *= sqrt(impactSpeed * obj1.mass * obj2.mass);

	// Limit impulse
//	if(impulse.getLength() > 1)
//		impulse.normalize();

//	printf("IMPULSE: %f\n", impulse.getLength());

//	impulse /= speed;// Keeps from boucing all over the screen

	c = obj1.vel + impulse / obj1.mass;
	d = obj2.vel - impulse / obj2.mass;

//	printf("%f, %f\n", c.getLength(), d.getLength());

	// Limit velocity
	if(c.getLength() > 0.5)
	{
		c.normalize();
		c /= 10;
	}

	if(d.getLength() > 0.5)
	{
		d.normalize();
		d /= 10;
	}

	// update new velocity
	obj1.vel = c;
	obj2.vel = d;
}

bool Collision(cObject obj1, cObject obj2)
{
	float dist = obj1.pos.getDistanceFrom(obj2.pos);

	if(dist <= obj1.size + obj2.size)
		return true;
	else
		return false;
}

void CheckCollisions(int deltaTime)
{
	list<cAsteroid*>::Iterator i;
	list<cAsteroid*>::Iterator j;
	list<cAsteroid*>::Iterator k;
	list<cLaser*>::Iterator l;

	// Player vs asteroid
	for(i = Asteroids.begin(); i != Asteroids.end(); i++)
	{
		// Get asteroid by pointer
		cAsteroid *a = (*i);

		if(a->active && Collision(player, *a))
		{
			if(player.shield)
			{
				// Play bounce sound
//				sound->play2D("data/soundfx/shield_bounce.wav");

				// Bounce player and asteroid
				BounceObjects(player, *a, 5, 10);// more bounce if shield is up

				// Lose energy
				player.SubtractEnergy(a->mass);

				// Damage asteroid
				if(a->health - player.energy/10 >= 0)
					a->health -= player.energy/10;
				else
					a->active = false;

				// Break shield
				if(player.energy <= 0)
				{
					printf("Your shield broke!\n");
//					sound->play2D("data/soundfx/shield_break.wav");
				}
			}
			else
			{
//				sound->play2D("data/soundfx/metalclank.wav");

				BounceObjects(player, *a, 8, 10);// less of a bounce without shield
				// lose health
				player.SubtractHealth(a->mass);
				printf("You lose %d health.\n", (int)a->mass);

				if(player.health <= 0)
				{
					printf("You have died!\n");
					player.alive = false;
				}
			}
		}

		// Asteroid vs asteroid
		for(j = Asteroids.begin(); j != Asteroids.end(); j++)
		{
			cAsteroid *a2 = (*j);

			if(a->active && a2->active && a != a2)
				if(Collision(*a, *a2))
					BounceObjects(*a, *a2, 10, 10);
		}

	}


	// Laser vs asteroid
	for(l = player.Laser.begin(); l != player.Laser.end(); l++)
		for(k = Asteroids.begin(); k != Asteroids.end(); k++)
		{
			// Get asteroid and laser pointers
			cAsteroid *a3 = (*k);
			cLaser *laser = (*l);

			if(a3->active && laser->vel != vector2df(0,0) && Collision(*laser,*a3))
			{
				printf("You shot an asteroid!\n");

				// Check remaining energy on laser
				if(laser->dmg - a3->health <= 0)
					laser->remove = true;

				// If damaged enough, break asteroid appart
				if(a3->health - laser->dmg <= 0)
				{
//					sound->play2D("data/soundfx/hit.wav");

					// Break asteroid into smaller ones
					vector2df newPos;

					// Only add more if the asteroids are big enough
					if(a3->size > 16)
					{

						for(int f = 0; f < a3->fragments; f++)
						{
///							newPos = vector2df(random.RandInt(a3->pos.X - a3->size, a3->pos.X + a3->size),
			///								   random.RandInt(a3->pos.Y - a3->size, a3->pos.Y + a3->size));
							AddAsteroid(vector2df(newPos), a3->size - 16);
						}
					}

					// Change active status and reset animation timers
					a3->active = false;
					a3->currentFrame = 0;
					a3->frameTimer = 0;
				}
				else
				{
					// Play impact sound
//					sound->play2D("data/soundfx/asteroid_impact1.wav");

					a3->health -= laser->dmg;
					printf("H: %d\n", a3->health);
				}
			}
		}
}

void ShowFPS()
{
	static int lastFPS = -1;
	int fps = driver->getFPS();
	if(lastFPS != fps)
	{
		stringw str = L"Space Rock Defense by Micah Welch (FPS: ";
		str += fps;
		str += ")";
		device->setWindowCaption(str.c_str());
		lastFPS = fps;
	}
}

void WrapObject(cObject &obj)
{
	if(obj.pos.X < 0)
		obj.pos.X = SCREEN_WIDTH;
	else if(obj.pos.X > SCREEN_WIDTH)
		obj.pos.X = 0;

	if(obj.pos.Y < 0)
		obj.pos.Y = SCREEN_HEIGHT;
	else if(obj.pos.Y > SCREEN_HEIGHT)
		obj.pos.Y = 0;
}
void AddInterface()
{
	// Player 1

	IGUITab *hudTab = guienv->addTab(rect<int>(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), 0, ID_HUD_TAB);


//	guienv->addStaticText(L"Player 1", rect<int>(5, 5, 300, 20), true, false, hudTab, -1, false);
//	guienv->addStaticText(L"Armor", rect<int>(5, 25, 300, 40), true, false, hudTab, -1, false);
//	guienv->addStaticText(L"Energy", rect<int>(5, 45, 300, 60), true, false, hudTab, -1, false);
//	guienv->addStaticText(L"Energy Recharge Rate", rect<int>(5, 65, 300, 80), true, false, hudTab, -1, false);

	// Player 2
//	guienv->addStaticText(L"Player 2", rect<int>(SCREEN_WIDTH-300, 5, SCREEN_WIDTH-5, 20), true, false, hudTab, -1, false);
//	guienv->addStaticText(L"Armor", rect<int>(SCREEN_WIDTH-300, 25, SCREEN_WIDTH-5, 40), true, false, hudTab, -1, false);

//	guienv->addImage(driver->getTexture("data/interface/stats.png"), position2d<int>(0, 0), true);
//	guienv->addImage(driver->getTexture("data/interface/energyIcon.png"), position2d<int>(5, 26), true);




}
void UpdateInterface()
{




}
