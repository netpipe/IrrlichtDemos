
#include "Litha.h"

class FinalScenePlayerProxy : public ITransformable
{
public:
	core::vector3df GetPosition()
	{
		// Need to see if player (parent) is below certain height...
		// If so, we remain at that minimum height...
		
		if (GetParent())
		{
			f32 y = 0.0;
			
			if (GetParent()->GetAbsolutePosition().Y < 0.0)
				y += 0.0 - GetParent()->GetAbsolutePosition().Y;
			
			return core::vector3df(0,y,0);
		}
		
		return core::vector3df(0,0,0);
	}
};

