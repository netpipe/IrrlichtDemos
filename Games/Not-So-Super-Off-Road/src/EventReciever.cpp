#include "EventReciever.h"

EventReciever::EventReciever()
{
	wheels.X = 0.0f;
	wheels.Y = 260.0f;
	wheels.Z = 0.0f;

	velocity.X = 0.0f;
	velocity.Y = 0.0f;
	velocity.Z = 0.0f;

	speed = 0.0f;

	key = new vector< pair<EKEY_CODE, bool> * >();
	key->push_back(new pair<EKEY_CODE, bool>(KEY_UP, false));
	key->push_back(new pair<EKEY_CODE, bool>(KEY_DOWN, false));
	key->push_back(new pair<EKEY_CODE, bool>(KEY_LEFT, false));
	key->push_back(new pair<EKEY_CODE, bool>(KEY_RIGHT, false));

	// Sound
//	FMOD::System_Create(&system);
//	system->init(32, FMOD_INIT_NORMAL, NULL);
//	system->createDSPByType(FMOD_DSP_TYPE_OSCILLATOR, &dsp);
//	dsp->setParameter(FMOD_DSP_OSCILLATOR_RATE, 150.0f);
//	system->playDSP(FMOD_CHANNEL_REUSE, dsp, true, &channel);
//    channel->setVolume(0.08f);
//    dsp->setParameter(FMOD_DSP_OSCILLATOR_TYPE, 1);
//	channel->setPaused(false);
}

bool EventReciever::OnEvent(const SEvent &event)
{
	bool success = false;
	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
	{
		switch (current_state)
		{
			case GS_RUNNING:
			{
				next_state = GS_RUNNING;
				vector< pair<EKEY_CODE, bool> * >::iterator i = key->begin();
				for (; i != key->end(); i++)
				{
					if ((*i)->first == event.KeyInput.Key)
					{
						(*i)->second = event.KeyInput.PressedDown;
					}
				}
				success = true;
				break;
			}
			case GS_GAME_OVER:
			{
				if (event.KeyInput.Key == KEY_ESCAPE)
				{
					next_state = GS_QUIT;
				}
				else if (event.KeyInput.Key == KEY_RETURN)
				{
					next_state = GS_START;

					wheels.X = 0.0f;
					wheels.Y = 260.0f;
					wheels.Z = 0.0f;

					velocity.X = 0.0f;
					velocity.Y = 0.0f;
					velocity.Z = 0.0f;

					speed = 0.0f;

					vector< pair<EKEY_CODE, bool> * >::iterator i = key->begin();
					for (; i != key->end(); i++)
					{
						(*i)->second = false;
					}
				}
				success = true;
				break;
			}
		}
	}


	return success;
}

void EventReciever::setSteer(ISceneNode *n)
{
	node = n;
}

void EventReciever::dispatch()
{
	vector< pair<EKEY_CODE, bool> * >::iterator i = key->begin();
	for (; i != key->end(); i++)
	{
		if((*i)->second)
		{
			switch ((*i)->first)
			{
				case KEY_UP:
				{
					speed += 0.32f;
					break;
				}
				case KEY_DOWN:
				{
					speed -= 0.05f;
					break;
				}
				case KEY_LEFT:
				{
					wheels.Y -= 1.0f;
					if (wheels.Y < 0.0f)
						wheels.Y = 360.0f + wheels.Y;

					break;
				}
				case KEY_RIGHT:
				{
					wheels.Y += 1.0f;
					if (wheels.Y > 360.0f)
						wheels.Y = wheels.Y - 360.0f;

					break;
				}
			}
		}
	}

	speed *= 0.97f;

	vector3df v;
	v.X = sin(wheels.Y * 3.14/180.0f);
	v.Z = cos(wheels.Y * 3.14/180.0f);
	v = v * speed;
	v.Y = 0.0f;

	v = (v + velocity)/2.0f;
	velocity = v;
	//list<ISceneNodeAnimator*> anim = node->getAnimators();
	//ISceneNodeAnimator *t = (*anim.begin());
	//ITriangleSelector *selector = static_cast<ISceneNodeAnimatorCollisionResponse*>(t)->getWorld();
	//triangle3df triangle[100];
	//s32 count;
	//selector->getTriangles(triangle, 100, count, node->getTransformedBoundingBox(), NULL);

	node->setPosition(node->getPosition() + v);
	node->setRotation(wheels);

//	dsp->setParameter(FMOD_DSP_OSCILLATOR_RATE, 150.0f + (speed * 20.0f));
}

void EventReciever::setCurrentState(GAME_STATE state)
{
	current_state = state;
}

GAME_STATE EventReciever::getNextState()
{
	return next_state;
}
