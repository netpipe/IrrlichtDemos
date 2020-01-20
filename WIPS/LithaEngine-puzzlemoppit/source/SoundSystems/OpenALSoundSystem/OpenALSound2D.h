
#include "ISound2D.h"
#include "OpenALSound.h"

class OpenALSound2D : public OpenALSound, public ISound2D
{
public:
	OpenALSound2D(OpenALSoundSystem *soundSystem);
	~OpenALSound2D();
	
	void SetPan(f32 panning);
};
