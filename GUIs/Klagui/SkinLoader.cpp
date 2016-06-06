#include "SkinLoader.h"
#include <IFileSystem.h>
#include "CConfigMap.h"
#include "CConfigReader.h"
#include <IReadFile.h>
#include <IVideoDriver.h>

namespace irr
{

core::stringc GetContextPathFromFilename( const core::stringc& filename )
{
	core::stringc path;
	s32 i=filename.size()-1;
	while ( i >= 0 && filename[i] != '/' )
	{
		i--;
	}
	path = filename.subString( 0, i+1 );
	return path;
}

CConfigMap* LoadConfigFromFile( io::IFileSystem* fileSystem, const core::stringc& filename )
{
	io::IReadFile* file = fileSystem->createAndOpenFile( filename.c_str() );

	if ( !file )
		return 0;

	CConfigMap* map = new CConfigMap();

	CConfigReader reader;
	reader.loadReadFile(file);

	file->drop();

	while ( reader.nextConfig() )
	{
		core::stringc name = reader.getConfigName();
		core::stringc value = reader.getConfigValue();

		map->setConfig(name,value);
	}

	return map;
}

static void ParseGUIElementStyle( video::IVideoDriver* driver, CConfigMap* cfg, gui::SImageGUIElementStyle& elem, const core::stringc& name, const core::stringc& context )
{
	core::stringc s = cfg->getConfig(name+".SrcBorder");
	sscanf( s.c_str(), "%d , %d , %d , %d", &elem.SrcBorder.Top, &elem.SrcBorder.Left, &elem.SrcBorder.Bottom, &elem.SrcBorder.Right );

	s = cfg->getConfig(name+".DstBorder");
	sscanf( s.c_str(), "%d , %d , %d , %d", &elem.DstBorder.Top, &elem.DstBorder.Left, &elem.DstBorder.Bottom, &elem.DstBorder.Right );

	s = cfg->getConfig(name+".Texture");
	elem.Texture = driver->getTexture((context + s).c_str());

	s = cfg->getConfig(name+".Color");
	if ( s != "" )
	{
		s32 a,r,g,b;
		sscanf( s.c_str(), "%d , %d , %d , %d", &a, &r, &g, &b );
		elem.Color = video::SColor(a,r,g,b);
	}
}

gui::SImageGUISkinConfig LoadGUISkinFromFile( io::IFileSystem* fileSystem, video::IVideoDriver* driver, const core::stringc& filename )
{
	gui::SImageGUISkinConfig skin;

	CConfigMap* cfg = LoadConfigFromFile(fileSystem, filename);
	if ( !cfg )
		return skin;

	core::stringc context = GetContextPathFromFilename(filename);

	ParseGUIElementStyle( driver, cfg, skin.Button, "Button", context );
	ParseGUIElementStyle( driver, cfg, skin.ButtonPressed, "ButtonPressed", context );
	ParseGUIElementStyle( driver, cfg, skin.SunkenPane, "SunkenPane", context );
	ParseGUIElementStyle( driver, cfg, skin.Window, "Window", context );
	ParseGUIElementStyle( driver, cfg, skin.ProgressBar, "ProgressBar", context );
	ParseGUIElementStyle( driver, cfg, skin.ProgressBarFilled, "ProgressBarFilled", context );

	delete cfg;

	return skin;
}


} // end namespace irr

