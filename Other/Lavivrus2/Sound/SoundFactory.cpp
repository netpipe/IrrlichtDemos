#include "SoundFactory.h"

SoundFactory* SoundFactory::instance = NULL;

//! Sound factory constructor. Take's the working directory as it's initializer
SoundFactory::SoundFactory ( const std::string &workDir )
{
	instance = this;
	refCount = 1;
	sourceRefCount = 1;
	active = false;
	workingDirectory = workDir;

	if ( ! ( soundDevice = alcOpenDevice ( NULL ) ) )
	{
		active = false;
	}
	else
	{
		active = true;
		soundContext = alcCreateContext ( soundDevice, NULL );
		alcMakeContextCurrent ( soundContext );
	}
//active = false;
}

SoundFactory::~SoundFactory()
{
	flush();
}

//! Sets the sound factory as active
void SoundFactory::setActive ( bool flag )
{
	active = flag;
}

//! Checks to see if the sound factory is active
bool SoundFactory::isActive()
{
	return active;
}

bool SoundFactory::loadOgg ( const std::string &soundName )
{
	OggFile *newOgg = new OggFile();
	if ( newOgg->open ( soundName ) < 0 )
	{
		return false;
	}

	while ( files[refCount] != NULL || refCount ==0 )
		refCount++;

	files[refCount] = newOgg;
	sounds[soundName] = refCount;
	fileRefs[soundName] = 1;

	return true;
}

void SoundFactory::unloadOgg ( const std::string &soundName )
{
	if ( fileRefs[soundName] == 1 )
	{
		files[sounds[soundName]]->close();
		files.erase ( sounds[soundName] );
		sounds.erase ( soundName );
	}
	else
		--fileRefs[soundName];
}

//! Loads a sound and returns the SoundID
unsigned long SoundFactory::loadSound ( const std::string &soundFile )
{
	std::string soundName = std::string ( "./" ) + workingDirectory + std::string ( "/Sounds/" ) + soundFile;
	if ( sounds.find ( soundName ) != sounds.end() )
	{
		SoundSource *newSource = new SoundSource();
		while ( sources.find ( sourceRefCount ) != sources.end() || sourceRefCount == 0 )
			sourceRefCount++;
		newSource->attachToSource ( files[sounds[soundName]], sourceRefCount );
		++fileRefs[soundName];
		sources[sourceRefCount] = newSource;
		sourceFiles[sourceRefCount] = soundName;
		return sourceRefCount;
	}

	std::string extension;
	extension = soundName.substr ( soundName.size() - 3, 3 );
	if ( extension == "ogg" )
	{
		if ( loadOgg ( soundName ) )
		{
			SoundSource *newSource = new SoundSource();
			while ( sources.find ( sourceRefCount ) != sources.end() || sourceRefCount == 0 )
				sourceRefCount++;
			newSource->attachToSource ( files[sounds[soundName]], sourceRefCount );
			sources[sourceRefCount] = newSource;
			sourceFiles[sourceRefCount] = soundName;
			return sourceRefCount;
		}
		return 0;
	}

	return 0;
}

//! Unloads a sound
void SoundFactory::unloadSound ( const unsigned long &soundID )
{
	if ( soundID == 0 || !active )
		return;

	sources[soundID]->stop();
	unloadOgg ( sourceFiles[soundID] );
	delete sources[soundID];
	sources.erase ( soundID );
}

//! Plays a sound
void SoundFactory::playSound ( const unsigned long &soundID, const bool &looping, const float &volume, const float &attenuation )
{
	if ( soundID == 0 || !active )
		return;

	sources[soundID]->rewind();
	sources[soundID]->setVolume ( volume );
	sources[soundID]->setAttenuation ( attenuation );
	sources[soundID]->setLooping ( looping );
	sources[soundID]->play();
}

//! Stops a sound
void SoundFactory::stopSound ( const unsigned long &soundID )
{
	if ( soundID == 0 || !active )
		return;

	sources[soundID]->stop();
}

//! Resumes a stopped or paused sound
void SoundFactory::continueSound ( const unsigned long &soundID )
{
	if ( soundID == 0 || !active )
		return;

	sources[soundID]->play();
}

//! Sets the sound volume
void SoundFactory::setVolume ( const unsigned long &soundID, const float &volume )
{
	if ( soundID == 0 || !active )
		return;

	sources[soundID]->setVolume ( volume );
}

//! Sets the sound attenuation
void SoundFactory::setAttenuation ( const unsigned long &soundID, const float &attenuation )
{
	if ( soundID == 0 || !active )
		return;

	sources[soundID]->setAttenuation ( attenuation );
}

//! Sets the sound to loop
void SoundFactory::setLooping ( const unsigned long &soundID, const bool &looping )
{
	if ( soundID == 0 || !active )
		return;

	sources[soundID]->setLooping ( looping );
}

//! Sets the sound's position
void SoundFactory::setPosition ( const unsigned long &soundID, const float &x, const float &y, const float &z )
{
	if ( soundID == 0 || !active )
		return;

	sources[soundID]->setPosition ( x, y, z );
}

//! Checks to see if a sound has finished playing
bool SoundFactory::hasFinished ( const unsigned long &soundID )
{
	if ( soundID == 0 || !active )
		return true;

	return sources[soundID]->hasFinished();
}

//! Checks to see if a sound has stopped (it doesn't mean that the sound has finished playing though)
bool SoundFactory::hasStopped ( const unsigned long &soundID )
{
	if ( soundID == 0 || !active )
		return true;

	return !sources[soundID]->isPlaying();
}

//! Gets the time location of the currently decoded sound
double SoundFactory::getTime ( const unsigned long &soundID )
{
	if ( soundID == 0 || !active )
		return 0;

	return sources[soundID]->getCurrentTime();
}

//! Gets the total duration of the sound
double SoundFactory::getLength ( const unsigned long &soundID )
{
	if ( soundID == 0 || !active )
		return 0;

	return sources[soundID]->getLength();
}

//! Updates the Sound factory (should only be called by the sound factory thread)
void SoundFactory::tick()
{
	if ( active )
	{
		std::map<unsigned long, SoundSource*>::iterator it = sources.begin();
		while ( it != sources.end() )
		{
			if ( ( *it ).second != NULL && ( *it ).second->isPlaying() )
				( *it ).second->update();

			it++;
		}
	}
}

//! The sound factory thread loop
void SoundFactory::run()
{
	while ( 1 )
	{
#ifdef WIN32
		Sleep ( 1 );
#else
		usleep ( 100 );
#endif
		tick();
	}
}

//! Clears out the sounds
void SoundFactory::flush()
{
	std::map<unsigned long, SoundSource*>::iterator st = sources.begin();
	while ( st != sources.end() )
	{
		if ( ( *st ).second != NULL )
		{
			( *st ).second->stop();
			delete ( *st ).second;
		}

		st++;
	}

	std::map<unsigned long, OggFile*>::iterator ft = files.begin();
	while ( ft != files.end() )
	{
		if ( ( *ft ).second != NULL )
		{
			( *ft ).second->close();
			delete ( *ft ).second;
		}

		ft++;
	}

	sources.clear();
	files.clear();
	sounds.clear();
}


void *startSoundFactoryThread ( void * )
{
	printf ( "Initializing Audio thread\n" );
	SoundFactory::getInstance()->run();
	return NULL;
}
