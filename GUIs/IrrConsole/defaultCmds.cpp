
#include "IrrConsole/defaultCmds.h"

IC_Command_ECHO::IC_Command_ECHO() : IC_Command(L"echo")
{
	setUsage(L"echo <string>");
	addDescLine(L"This command echoes the given string to console");
}

IC_Command_ECHO::~IC_Command_ECHO()     {   }

bool IC_Command_ECHO::invoke(const array<WideString>& args, IC_Dispatcher* pDispatcher, IC_MessageSink* pOutput)
{
	if(args.size() > 1)
	{
		WideString wstr = args[0];
		for(u32 i = 1; i < args.size(); i++)
		{
			wstr += L" ";
			wstr += args[i];
		}
		pOutput->appendMessage(wstr);
	}
	return true;
}

IC_Command_HELP::IC_Command_HELP() : IC_Command(L"help")
{
	setUsage(L"help <cmd-name>");
	addDescLine(L"this command prints the help available for console commands");
	addDescLine(L"if cmd-name is not supplied a list of commands is printed with usage");
	addDescLine(L"any command has to be preceded with the backslash character to execute");
	addDescLine(L"e.g. \\help");
}


IC_Command_HELP::~IC_Command_HELP()     {   }


bool IC_Command_HELP::invoke(const array<WideString>& args, IC_Dispatcher* pDispatcher, IC_MessageSink* pOutput)
{
	if(args.size() == 0)
	{
		pDispatcher->printCommandList(pOutput,true);
	}
	else
	{
		WideString wstr = args[0];
		for(u32 i = 1; i < args.size(); i++)
		{
			wstr += L" ";
			wstr += args[i];
		}
		if(pDispatcher->hasCommand(wstr))
		{
			pDispatcher->printCommandDesc(wstr,pOutput);
		}
		else
		{
			WideString msg = " No help available for command ";
			msg+= wstr;
			pOutput->appendMessage(msg);
		}
	}
	return true;
}


IC_Command_LIST::IC_Command_LIST(): IC_Command(L"list")
{
	setUsage("list <detailed>");
	addDescLine("This command lists the available commands");
	addDescLine("If an additional paramter is specified then");
	addDescLine("the command returns usage information for the commands");
}


IC_Command_LIST::~IC_Command_LIST()    {   }


bool IC_Command_LIST::invoke(const array<WideString>& args, IC_Dispatcher* pDispatcher, IC_MessageSink* pOutput)
{
	if(args.size() > 0)
	{
		pDispatcher->printCommandList(pOutput,true);
	}
	else
	{
		pDispatcher->printCommandList(pOutput,false);
	}
	return true;
}


IC_Command_DRIVER_INFO::IC_Command_DRIVER_INFO(irr::IrrlichtDevice *pDevice) : IC_Command("driver_info"),device(pDevice)
{
	setUsage("driver_info");
	addDescLine("This command prints some info about the engine");

}


IC_Command_DRIVER_INFO::~IC_Command_DRIVER_INFO()   {	device = 0;  }


bool IC_Command_DRIVER_INFO::invoke(const array<WideString>& args, IC_Dispatcher* pDispatcher, IC_MessageSink* pOutput)
{
	if(device)
	{
		irr::video::IVideoDriver* driver = device->getVideoDriver();
		WideString wstr = L" Irrlicht Version : ";
		wstr+= device->getVersion();
		pOutput->appendMessage(wstr);

		wstr = L" OS Version : ";
		wstr+= device->getOSOperator()->getOperationSystemVersion();
		pOutput->appendMessage(wstr);

		wstr = L" Display Driver : ";
		wstr+= device->getVideoDriver()->getName();
		pOutput->appendMessage(wstr);

		wstr=L"";
		return true;
	}
	else
	{
		throw IC_Error(L"No valid irrlicht device detected!!");
	}
}



//! Exit Codes
IC_Command_EXIT::IC_Command_EXIT() : IC_Command(L"exit")
{
	setUsage("exit");
	addDescLine("sets the console to invisible");
}

IC_Command_EXIT::~IC_Command_EXIT() {   }

bool IC_Command_EXIT::invoke(const array<WideString>& args, IC_Dispatcher* pDispatcher, IC_MessageSink* pOutput)
{
	pOutput->toggleVisible();
	return true;
}

IC_Command_QUIT::IC_Command_QUIT() : IC_Command(L"quit")
{
	setUsage("exit");
	addDescLine("sets the console to invisible");
}

IC_Command_QUIT::~IC_Command_QUIT() {}

bool IC_Command_QUIT::invoke(const array<WideString>& args, IC_Dispatcher* pDispatcher, IC_MessageSink* pOutput)
{
	pOutput->toggleVisible();
	return true;
}

IC_Command_CLS::IC_Command_CLS() : IC_Command(L"cls")
{
	setUsage("exit");
	addDescLine("clears the console messages");
}

IC_Command_CLS::~IC_Command_CLS()   {   }

bool IC_Command_CLS::invoke(const array<WideString>& args, IC_Dispatcher* pDispatcher, IC_MessageSink* pOutput)
{
	pOutput->toggleVisible();
	return true;
}
