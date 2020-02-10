
#include <collision.h>

Collision::Collision()
{
    number = 0;
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
