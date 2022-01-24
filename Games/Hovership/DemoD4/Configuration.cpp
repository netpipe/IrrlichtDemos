#include "Configuration.h"
#include <irrlicht.h>

Configuration::Configuration() :
	//setup name and versioning
	projectName(L"FOS Project"),
	majorVersion(0),
	minorversion(1),

#ifdef _SOUND
	//sound
	soundEnabled(true),
#endif

	//gui
	guiTransparency(255),

	//player
	playerName(L"Jack"),
	shipColor(255, 0, 0, 255), //ARGB - default color is blue

	cursorSensitivity(1.0f)
{
	//setup sane default values for irrlicht device in case of invalid / not readable configuration file
	this->params.DriverType = irr::video::EDT_OPENGL;
	this->params.WindowSize.Width = 1024;
	this->params.WindowSize.Height = 768;
	this->params.Bits = 32;
	this->params.Fullscreen = false;
	this->params.Stencilbuffer = true;
	this->params.Vsync = false;

	this->params.AntiAlias = false;
}

const void Configuration::read()
{
	irr::IrrlichtDevice* device = irr::createDevice(irr::video::EDT_NULL);
	//create xml reader
	irr::io::IXMLReader* xml = 0;
	xml = device->getFileSystem()->createXMLReader("config/config.xml"); //create xmlreader

	if (xml == 0)
	{
		device->getLogger()->log(L"Cannot find configuration file. Falling back to defaults.");
		device->drop();

		return;
	}

	device->getLogger()->log(L"Reading configuration file...");

	while(xml && xml->read())
	{
		if (irr::core::stringw("graphic") == xml->getNodeName())
		{
			const irr::u32 driver = xml->getAttributeValueAsInt(L"driver");
			irr::video::E_DRIVER_TYPE driverType;

			switch (driver)
			{
				case 0: driverType = irr::video::EDT_OPENGL; break;
				case 1: driverType = irr::video::EDT_BURNINGSVIDEO; break;
				default: driverType = irr::video::EDT_SOFTWARE; break;
			}

			this->params.DriverType = driverType;
			this->params.Fullscreen = 0;//xml->getAttributeValueAsInt(L"fullscreen");
			this->params.Stencilbuffer = xml->getAttributeValueAsInt(L"stencilbuffer");
			this->params.Vsync = xml->getAttributeValueAsInt(L"vsync");
		}

		else if (irr::core::stringw("resolution") == xml->getNodeName())
		{
			this->params.WindowSize.Width = xml->getAttributeValueAsInt(L"xres");
			this->params.WindowSize.Height = xml->getAttributeValueAsInt(L"yres");
			///\todo Check resolution

			this->params.Bits = xml->getAttributeValueAsInt(L"bits");

			//Check color depth.
			if ( (this->params.Bits != 16) || (this->params.Bits != 24))
				this->params.Bits = 32;
		}
#ifdef _SOUND
		else if (irr::core::stringw("sound") == xml->getNodeName())
		{
			this->soundEnabled = xml->getAttributeValueAsInt(L"enabled");
		}
#endif
		else if (irr::core::stringw("gui") == xml->getNodeName())
		{
			this->guiTransparency = xml->getAttributeValueAsInt(L"transparency");

			//check for sane values
			if (this->guiTransparency > 255) this->guiTransparency = 255;
			else if (this->guiTransparency < 0) this->guiTransparency = 0;
		}

		else if (irr::core::stringw("player") == xml->getNodeName())
		{
			this->playerName = xml->getAttributeValue(L"name");

			//check for sane values
			if (this->playerName == L"" || this->playerName == L"." || this->playerName == L"..") //etc
				this->playerName = L"Jack";
		}

		else if (irr::core::stringw("ship") == xml->getNodeName())
		{
			//const irr::u32 shipType = xml->getAttributeValueAsInt(L"type");

			irr::u32 alpha = xml->getAttributeValueAsInt(L"alpha");
			irr::u32 red = xml->getAttributeValueAsInt(L"red");
			irr::u32 green = xml->getAttributeValueAsInt(L"green");
			irr::u32 blue = xml->getAttributeValueAsInt(L"blue");

			if (alpha > 255) alpha = 255;
			if (red > 255) red = 255;
			if (green > 255) green = 255;
			if (blue > 255) blue = 255;

			this->shipColor.set(alpha, red, green, blue);
		}

	}

	xml->drop();
	device->drop();
}

const void Configuration::write() const
{
	irr::IrrlichtDevice* device = 0;
	device = irr::createDevice(irr::video::EDT_NULL);

	irr::io::IXMLWriter* xmlw = 0;
	if (device->getFileSystem()->existFile("config/config.xml"))
		xmlw = device->getFileSystem()->createXMLWriter("config/config.xml");
	else
		device->getLogger()->log(L"Configuration file does not exist. Please create an empty file 'config/config.xml'.");

	if (xmlw == 0)
	{	//could not open file for writing
		device->drop();
		return;
	}

	device->getLogger()->log(L"Writing configuration file...");

	xmlw->writeXMLHeader();
	xmlw->writeLineBreak();

	xmlw->writeElement(L"configuration", false);
	xmlw->writeLineBreak();
	xmlw->writeLineBreak();

	//write grafic driver config
	xmlw->writeComment(L"Irrlicht device settings: driver can be 0 = OPENGL, 1 = Burning's Video or 2 = Software");
	xmlw->writeLineBreak();

	irr::core::stringw driver(L"");
	switch(this->params.DriverType)
	{
		case irr::video::EDT_OPENGL: driver = L"0"; break;
		case irr::video::EDT_BURNINGSVIDEO: driver = L"1"; break;
		default: driver = L"2";
	}

	xmlw->writeElement(L"graphic", true,
		L"driver", driver.c_str(),
		L"fullscreen", irr::core::stringw(this->params.Fullscreen).c_str(),
		L"stencilbuffer", irr::core::stringw(this->params.Stencilbuffer).c_str(),
		L"vsync", irr::core::stringw(this->params.Vsync).c_str()
	);

	xmlw->writeLineBreak();
	xmlw->writeLineBreak();

	//write resolution and color depth
	xmlw->writeComment(L"Resolution and color depth settings");
	xmlw->writeLineBreak();
	xmlw->writeElement(L"resolution", true,
		L"xres", irr::core::stringw(this->params.WindowSize.Width).c_str(),
		L"yres", irr::core::stringw(this->params.WindowSize.Height).c_str(),
		L"bits", irr::core::stringw(this->params.Bits).c_str()
	);

	xmlw->writeLineBreak();
	xmlw->writeLineBreak();

#ifdef _SOUND
	//write sound config
	xmlw->writeComment(L"Sound settings: enabled=<0|1>");
	xmlw->writeLineBreak();
	xmlw->writeElement(L"sound", true, L"enabled", irr::core::stringw(this->soundEnabled).c_str());

	xmlw->writeLineBreak();
	xmlw->writeLineBreak();
#endif

	//write gui configuration
	xmlw->writeComment(L"Gui settings: transparency=<0-255>");
	xmlw->writeLineBreak();
	xmlw->writeElement(L"gui", true, L"transparency", irr::core::stringw(this->guiTransparency).c_str());

	xmlw->writeLineBreak();
	xmlw->writeLineBreak();

	//write player config
	xmlw->writeComment(L"Player settings: name=<Your name>");
	xmlw->writeLineBreak();
	xmlw->writeElement(L"player", true, L"name", irr::core::stringw(this->playerName).c_str());

	xmlw->writeLineBreak();
	xmlw->writeLineBreak();

	//write ship config
	xmlw->writeComment(L"Ship settings: type=<ignored>");
	xmlw->writeLineBreak();
	xmlw->writeElement(
		L"ship", true,
		L"type", irr::core::stringw(L"0").c_str(),
		L"alpha", irr::core::stringw(this->shipColor.getAlpha()).c_str(),
		L"red", irr::core::stringw(this->shipColor.getRed()).c_str(),
		L"green", irr::core::stringw(this->shipColor.getGreen()).c_str(),
		L"blue", irr::core::stringw(this->shipColor.getBlue()).c_str()
	);

	xmlw->writeLineBreak();
	xmlw->writeLineBreak();

	xmlw->writeClosingTag(L"configuration");

	//clean up writer
	xmlw->drop();

	device->getLogger()->log(L"Done.");
	device->drop();
}
