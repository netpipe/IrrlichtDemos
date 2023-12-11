#include <stdlib.h>
//include <conio.h> // for console input on msdos
#include <AL/al.h>
#include <AL/alc.h>
//============================================//
class INIT
{
public:
	 ALboolean InitOpenAL();
};
//====================Sound Class========================//
class Sound
{
public:
	ALbyte *Name;
	ALuint Buffer;
	ALuint Source;
	ALfloat SourcePos[3] ;
	ALfloat SourceVel[3];
	ALfloat ListenerPos[3];
	ALfloat ListenerOri[3];
	ALfloat ListenerVel[3];
	void AddSource();
	ALboolean LoadALData();
	void KillALData();
	void ExitOpenAL();
//==============================//
public:
	Sound();
	Sound(ALbyte *File_Name);
	void SetListenerPos(ALfloat x , ALfloat y , ALfloat z);
	void SetListenerVelo(ALfloat x , ALfloat y , ALfloat z);
	void MovePos(ALfloat x, ALfloat y ,ALfloat z);
	void MoveVelo(ALfloat x, ALfloat y ,ALfloat z);
	void Play() ;
	void Stop() ;
	void changeIntinsity(ALfloat t);
};


