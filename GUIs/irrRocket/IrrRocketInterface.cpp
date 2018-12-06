/*
  Copyright (C) 2011 Daniel Sudmann

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Daniel Sudmann suddani@googlemail.com
*/
#include "IrrRocketInterface.h"
#include <ITimer.h>

IrrRocketInterface::IrrRocketInterface(irr::ITimer* timer) : IrrTimer(timer)
{
    //ctor
}

IrrRocketInterface::~IrrRocketInterface()
{
    //dtor
}

float IrrRocketInterface::GetElapsedTime()
{
    return (float)IrrTimer->getTime()/1000.f;
}
