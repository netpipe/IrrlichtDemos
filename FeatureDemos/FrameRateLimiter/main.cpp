more of a placement file



static DWORD then = 0;
DWORD now = GetTickCount();

if (now - then < 15)
{
   Sleep(15 - (now - then));
}

then = GetTickCount();




orr




irr::u32 timeThisFrame = Timer->getTime();

// Lock FPS at around 60
while((timeThisFrame - timePreviousFrame) <= 16) {
  Timer->tick();
  timeThisFrame = Timer->getTime();
}

timePreviousFrame = timeThisFrame; 
