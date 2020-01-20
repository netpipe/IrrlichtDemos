
#ifndef SOUND_REG_H
#define SOUND_REG_H

#include <vector>

class OpenALSound;
class OpenALSound2D;
class OpenALSound3D;

extern std::vector<OpenALSound *> allSounds;
extern std::vector<OpenALSound2D *> allSounds2D;
extern std::vector<OpenALSound3D *> allSounds3D;

void register_sound2d(OpenALSound2D *sound);
void unregister_sound2d(OpenALSound2D *sound);

void register_sound3d(OpenALSound3D *sound);
void unregister_sound3d(OpenALSound3D *sound);

#endif

