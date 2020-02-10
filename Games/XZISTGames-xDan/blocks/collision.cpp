
#include "collision.h"

Collision::Collision()
{
    type = 0;
    number = 0;
    data = NULL;
    surfaceSet = false;
}

void Collision::add()
{
    number ++;
}

void Collision::reset()
{
    number = 0;
}

int Collision::getAmount()
{
    return number;
}

void Collision::setRayContact(dContactGeom contact)
{
    rayContact = contact;
}

dContactGeom Collision::getRayContact()
{
    return rayContact;
}
