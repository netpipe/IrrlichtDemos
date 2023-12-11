
#include "openal_stuff.h"
#include "litha_internal.h"

// We stop logging after a while, since it will slow the app down.
#define MAX_OPENAL_ERROR_REPORTS 500

u32 errorCount = 0;

bool check_openal_error()
{
	int alError = alGetError();
	
	if (alError != AL_NO_ERROR)
	{
		errorCount ++;
		
		if (errorCount < MAX_OPENAL_ERROR_REPORTS)
			NOTE << "OpenALSoundSystem error: AL error code " << alError;
		else if (errorCount == MAX_OPENAL_ERROR_REPORTS)
			WARN << "Too many OpenAL errors, will disable reporting.";
		
		return true;
	}
	
	return false;
}
