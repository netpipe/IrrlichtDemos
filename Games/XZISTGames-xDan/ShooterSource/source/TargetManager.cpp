
#include "TargetManager.h"
#include "Person.h"
#include "Duck.h"
#include "UFO.h"
#include "Demon.h"

TargetManager::TargetManager(IrrlichtDevice *devicesengine)
{
    this->device = device;
//    this->sengine = sengine;
    this->smgr = device->getSceneManager();
    level = 1.0;
    mode = 0;

    // create a few ducks to start with...
    int count = 1 + rand()%3;
    for (int i = 0; i < count; i ++)
    {
        targets.push_back(new Duck(device));
    }
}

TargetManager::~TargetManager()
{
    for (int i = 0; i < targets.size(); i ++)
    {
        delete targets[i];
    }
}

bool TargetManager::timeStepped(int id, double gameTime, double step, double stepMaxRandGreater)
{
    for (int i = 0; i <= id; i ++)
    {
        if (i >= lastTargetTimes.size()) lastTargetTimes.push_back(-1);
    }

    if (lastTargetTimes[id] == -1)
    {
        // create new stepper
        lastTargetTimes[id] = 0;
        return false;
    }
    else
    {
        // refresh old one

        if (lastTargetTimes[id] == 0) // first step is not used
        {
            lastTargetTimes[id] = gameTime;
            return false;
        }

        double timeSinceLastTarget = gameTime - lastTargetTimes[id];

        if (timeSinceLastTarget > step)
        {
            if (stepMaxRandGreater < 1.0) stepMaxRandGreater = 1.0;
            lastTargetTimes[id] = gameTime + ((double)(rand()%((int)stepMaxRandGreater*1000))) / 1000.0;
            return true;
        }
        else
        {
            return false;
        }
    }
}

void TargetManager::update(double gameTime, double dt)
{
    enum {MODE_NORMAL = 0, MODE_FRANTIC = 1, MODE_DUCKFEST = 2, MODE_DEMONS = 3};
    const static int MODE_COUNT = 4;

    switch (mode)
    {
    case MODE_NORMAL:
        // person
        if (timeStepped(0, gameTime, 1.4/level, 1.0))
        {
            targets.push_back(new Person(device));
        }
        // duck
        if (timeStepped(1, gameTime, 7.0/level, 2.0))
        {
            int count = 1 + rand()%2;
            for (int i = 0; i < count; i ++)
            {
                targets.push_back(new Duck(device));
            }
        }
        // demon
        if (timeStepped(2, gameTime, 40.0/level, 4.0))
        {
            targets.push_back(new Demon(device));
        }
        break;

    case MODE_FRANTIC:
        // person
        if (timeStepped(0, gameTime, 0.2 * 1.4/level, 1.0))
        {
            targets.push_back(new Person(device));
        }
        // duck
        if (timeStepped(1, gameTime, 0.25 * 7.0/level, 2.0))
        {
            int count = 1 + rand()%2;
            for (int i = 0; i < count; i ++)
            {
                targets.push_back(new Duck(device));
            }
        }
        // demon
        if (timeStepped(2, gameTime, 0.1 * 40.0/level, 4.0))
        {
            targets.push_back(new Demon(device));
        }
        break;

    case MODE_DUCKFEST:
        // person
        if (timeStepped(0, gameTime, 1.4/level, 1.0))
        {
            targets.push_back(new Person(device));
        }
        // duck
        if (timeStepped(1, gameTime, 1.0/level, 2.0))
        {
            int count = 1 + rand()%2;
            for (int i = 0; i < count; i ++)
            {
                targets.push_back(new Duck(device));
            }
        }
        // demon
        if (timeStepped(2, gameTime, 40.0/level, 4.0))
        {
            targets.push_back(new Demon(device));
        }
        break;

    case MODE_DEMONS:
        // person
        if (timeStepped(0, gameTime, 0.5 * 1.4/level, 1.0))
        {
            targets.push_back(new Person(device));
        }
        // duck
        if (timeStepped(1, gameTime, 12.0/level, 2.0))
        {
            int count = 1 + rand()%2;
            for (int i = 0; i < count; i ++)
            {
                targets.push_back(new Duck(device));
            }
        }
        // demon
        if (timeStepped(2, gameTime, 4.0/level, 4.0))
        {
            targets.push_back(new Demon(device));
        }
        break;
    }

    // difficulty increase
    if (timeStepped(3, gameTime, 40.0, 0))
    {
        level += 0.4;
        printf("Level Increased: %f\n", level);
    }

    if (timeStepped(4, gameTime, 20.0, 10.0))
    {
        mode = (mode+1+rand()%(MODE_COUNT-1))%MODE_COUNT;
        printf("Mode Changed: %i\n", mode);
    }

    /*
    - timeStepper for difficulty increase
        - therefore all values are modulated by this value
    - timeStepper for "mode change"
        - several different modes. for each mode there is a different set
            of critter creation time steppers.
            e.g. mode == MODE_NORMAL, MODE_FRANTIC, MODE_DUCKFEST...?
    */
    for (int i = 0; i < targets.size(); i ++)
    {
        if (!targets[i]->update(dt)) // if doesn't update, remove it
        {
            delete targets[i];
            targets.erase(targets.begin()+i);
            i --;
        }
    }
}

bool TargetManager::shootTargets(core::line3df ray)
{
    bool hit = false;
    for (int i = 0; i < targets.size(); i ++)
    {
        if (targets[i]->shoot(ray)) hit = true;
    }
    return hit;
}

int TargetManager::getScoreIncrements()
{
    int score = 0;
    for (int i = 0; i < targets.size(); i ++)
    {
        score += targets[i]->takeScore();
    }
    return score;
}

int TargetManager::takeUnhitCount()
{
    int count = 0;
    for (int i = 0; i < targets.size(); i ++)
    {
        count += targets[i]->takeUnhitFlag();
    }
    return count;
}

int TargetManager::takeShellsGained()
{
    int count = 0;
    for (int i = 0; i < targets.size(); i ++)
    {
        count += targets[i]->takeShellsGiven();
    }
    return count;
}

int TargetManager::takeFullnessIncreases()
{
    int count = 0;
    for (int i = 0; i < targets.size(); i ++)
    {
        count += targets[i]->takeFullnessIncrease();
    }
    return count;
}

std::vector <Splatter> TargetManager::getNewSplatters()
{
    std::vector <Splatter> splatters;
    for (int i = 0; i < targets.size(); i ++)
    {
        std::vector <Splatter> moreSplatters = targets[i]->getNewSplatters();
        for (int j = 0; j < moreSplatters.size(); j ++)
        {
            splatters.push_back(moreSplatters[j]);
        }
    }
    return splatters;
}

