#include "Squirrel_interface.h"

void	printfunc(HSQUIRRELVM v, const SQChar *s, ...)
{
	va_list arglist;
	va_start(arglist, s);
	scvprintf(s, arglist);
	va_end(arglist);
}
//-------------------------------------------------------------------------------------------------
Squirrel_interface::Squirrel_interface(IrrlichtDevice * dev, Scene_Interface	*si)
{
	// save Irrlicht device pointer
	device = dev;

	// set up Squirrel
	SquirrelVM::Init;
	sqstd_seterrorhandlers(SquirrelVM::GetVMPtr());		// sets the default error handler
	sq_setprintfunc(SquirrelVM::GetVMPtr(), printfunc);	// sets the print function

	// bind class vector3
	SQClassDef<vector3>(_T("vector3")).
		var (&vector3::X, _T("X")).
		var (&vector3::Y, _T("Y")).
		var (&vector3::Z, _T("Z")).
		staticFunc(&vector3::construct,    _T("constructor")       ).
		func(&vector3::operator +,         _T("_add")              ).
		func(&vector3::operator -,         _T("_sub")              ).
		func(&vector3::operator *,         _T("_mul")              ).
		func(&vector3::getLength,          _T("getLength")         ).
		func(&vector3::normalize,          _T("normalize")         ).
		func(&vector3::getHorizontalAngle, _T("getHorizontalAngle"))
		;

	// bind class Scene_Interface
	SQClassDef<Scene_Interface>(_T("Scene_Interface")).
		func(&Scene_Interface::setCurrentNode,    _T("setCurrentNode")   ).
		func(&Scene_Interface::getCurrentNode,    _T("getCurrentNode")   ).
		func(&Scene_Interface::getPosition,       _T("getPosition")      ).
		func(&Scene_Interface::getRotation,       _T("getRotation")      ).
		func(&Scene_Interface::getScale,          _T("getScale")         ).
		func(&Scene_Interface::setPosition,       _T("setPosition")      ).
		func(&Scene_Interface::setRotation,       _T("setRotation")      ).
		func(&Scene_Interface::setScale,          _T("setScale")         ).
		func(&Scene_Interface::setFrameLoop,      _T("setFrameLoop")     ).
		func(&Scene_Interface::setFrameType,      _T("setFrameType")     ).
		func(&Scene_Interface::getFrameNr,        _T("getFrameNr")       ).
		func(&Scene_Interface::getCameraPosition, _T("getCameraPosition")).
		func(&Scene_Interface::getCameraRotation, _T("getCameraRotation")).
		func(&Scene_Interface::getCameraTarget,   _T("getCameraTarget")  ).
		func(&Scene_Interface::setCameraTarget,   _T("setCameraTarget")  ).
		var (&Scene_Interface::CurrentTick,       _T("CurrentTick")      ).
		var (&Scene_Interface::LastTick,          _T("LastTick")         ).
		var (&Scene_Interface::SydVec,            _T("SydVec")           )
		;

	// pass pointer to C++ pointer to Scene_Interface
	SquirrelObject root = SquirrelVM::GetRootTable();
	BindVariable(root, si, "si");
}
//-------------------------------------------------------------------------------------------------
Squirrel_interface::~Squirrel_interface(void)
{
	SquirrelVM::Shutdown();
}
//-------------------------------------------------------------------------------------------------
bool	Squirrel_interface::loadScript(const c8* FileName, const c8* ShortName, const s32 ID)
{
	ScriptLibrary	sl;
	sl.FileName		= FileName;
	sl.ShortName	= ShortName;
	sl.node			= ID;

	io::IReadFile	*rf = device->getFileSystem()->createAndOpenFile(FileName);
	if (!rf)
	{
		printf("Error reading scriptfile <%s>\n", FileName);
		return false;
	}

	// compile script
	c8 *nut = new c8[rf->getSize()];			// create memory buffer, do not delete !
	nut[rf->getSize()] = 0;						// avoid any characters at the end of the string
	rf->read(nut, rf->getSize());				// fill buffer
	sl.SqOb =  SquirrelVM::CompileBuffer(nut);	// compile

	so.push_back(sl);

	return true;
}
//-------------------------------------------------------------------------------------------------
bool	Squirrel_interface::loadGlobalScript(const c8* FileName)
{
	io::IReadFile	*rf = device->getFileSystem()->createAndOpenFile(FileName);
	if (!rf)
	{
		printf("Error reading scriptfile <%s>\n", FileName);
		return false;
	}

	// compile script
	c8 *nut = new c8[rf->getSize()];			// create memory buffer, do not delete !
	nut[rf->getSize()] = 0;						// avoid any characters at the end of the string
	rf->read(nut, rf->getSize());				// fill buffer

	SquirrelObject root = SquirrelVM::GetRootTable();
	root =  SquirrelVM::CompileBuffer(nut);		// compile

	try		{	SquirrelVM::RunScript(root);	}
	catch	(SquirrelError & e)
			{	scprintf(_T("Error: %s, %s\n"), e.desc, "Global script");	}

	return true;
}
//-------------------------------------------------------------------------------------------------
bool	Squirrel_interface::runScript(const c8*  ShortName)
{
	s32 index = findScript(core::stringc(ShortName) );

	if (index < 0)
	{
		printf("Error finding script <%s>\n", ShortName);
		return false;
	}

	try		{	SquirrelVM::RunScript(so[index].SqOb);	}
	catch	(SquirrelError & e)
			{	scprintf(_T("Error: %s, %s\n"), e.desc, ShortName);	}

	return true;
}
//-------------------------------------------------------------------------------------------------
s32		Squirrel_interface::findScript(core::stringc ShortName)
{
	bool Found = false;
	irr::u32 i = 0;
	while (!Found && i < so.size())
	{
		if (so[i].ShortName.equals_ignore_case(ShortName)) Found = true; else i++;
	}
	if (Found)	return i;	else	return -1;
}
//-------------------------------------------------------------------------------------------------
bool	Squirrel_interface::runFunction(const c8*  FunctionName)
{
	// this compiles Okay, but gives a Squirrel error upon execution
	try		{	s32 val = SquirrelFunction<s32>(so[0].SqOb, FunctionName)();	}
	catch	(SquirrelError & e)
			{	scprintf(_T("Error: %s, %s\n"), e.desc, FunctionName);
				return false;
			}

	return true;
}
