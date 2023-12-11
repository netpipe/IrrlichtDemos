#include "GameGlobals.h"


GameConfig::GameConfig()
{
	// nothing
}


void GameConfig::Load(core::stringc fn, GameStruct *pGame)
{
	int val;
	char buf[1024];
	FILE *fp = fopen(fn.c_str(), "rt");
	
	if (!fp)
	{
		GameLog( "ERROR: \"%s\" not found.", fn.c_str() );
		return;
	}

	while (!feof(fp))
	{
		buf[0] = 0;
		fscanf(fp, "%s", buf);
		if (!strlen(buf)) continue;

		if (!strcmp( buf, "drivertype"))
		{
			fscanf(fp, "%s", buf);

			if (!strcmp(buf, "null")) pGame->DriverType = video::EDT_NULL;
#ifdef _IRR_COMPILE_WITH_SOFTWARE_
			else if (!strcmp(buf, "software"))
				pGame->DriverType = video::EDT_SOFTWARE;
#endif
#ifdef _IRR_COMPILE_WITH_BURNINGSVIDEO_
			else if (!strcmp(buf, "burningsvideo"))
				pGame->DriverType = video::EDT_BURNINGSVIDEO;
#endif
#ifdef _IRR_COMPILE_WITH_OPENGL_
			else if (!strcmp(buf, "opengl"))
				pGame->DriverType = video::EDT_OPENGL;
#endif
#ifdef _IRR_COMPILE_WITH_DIRECT3D_8_
			else if (!strcmp(buf, "direct3d8"))
				pGame->DriverType = video::EDT_DIRECT3D8;
#endif
#ifdef _IRR_COMPILE_WITH_DIRECT3D_9_
			else if (!strcmp(buf, "direct3d9"))
				pGame->DriverType = video::EDT_DIRECT3D9;
#endif
			else GameLog( "ERROR: Unknown driver type used \"%s\"!", buf );
		}
		else if (!strcmp(buf, "width"))
		{
			fscanf(fp, "%s", buf);
			val = atoi(buf);
			if (val > 0) pGame->Width = val;
			else GameLog("ERROR: Width value cannot be \"%s\"!", buf);
		}
		else if (!strcmp(buf, "height"))
		{
			fscanf(fp, "%s", buf);
			val = atoi(buf);
			if (val > 0) pGame->Height = val;
			else GameLog("ERROR: Height value cannot be \"%s\"!", buf);
		}
		else if (!strcmp(buf, "bitsperpixel"))
		{
			fscanf(fp, "%s", buf);
			val = atoi(buf);
			if (val==16 || val==24 || val==32) pGame->BitsPerPixel = val;
			else GameLog("ERROR: BitsPerPixel value can be 16, 24 or 32 only! Not \"%s\".", buf);
		}
		else if (!strcmp( buf, "fullscreen"))
		{
			fscanf(fp, "%s", buf);
			pGame->Fullscreen = atoi(buf)!=0;
		}
		else if (!strcmp(buf, "antialias"))
		{
			fscanf(fp, "%s", buf);
			pGame->AntiAlias = atoi(buf)!=0;
		}
		else if (!strcmp(buf, "vsync"))
		{
			fscanf(fp, "%s", buf);
			pGame->Vsync = atoi(buf)!=0;
		}
		else if (!strcmp(buf, "backgroundrendering"))
		{
			fscanf(fp, "%s", buf);
			pGame->BackgroundRendering = atoi(buf)!=0;
		}
		else if (!strcmp(buf, "screenshotwithoutgui"))
		{
			fscanf(fp, "%s", buf);
			pGame->ScreenshotWithoutGUI = atoi(buf)!=0;
		}
		else if (!strcmp(buf, "screenshotquality"))
		{
			fscanf(fp, "%s", buf);
			val = atoi(buf);
			if (val>=1 && val<=100) pGame->ScreenshotQuality = val;
			else {
				GameLog("ERROR: ScreenshotQuality value must be in range from 1 to 100! Using default value.");
				pGame->ScreenshotQuality = 100;
			}
		}
		// addable
		else
		{
			GameLog("ERROR: Unknown keyword \"%s\"", buf);
		}
	}

	fclose(fp);
}
