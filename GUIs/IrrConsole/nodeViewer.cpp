#include "IrrConsole/nodeViewer.h"

TestCommand_SHOWNODE::TestCommand_SHOWNODE(NodeViewer *pNV) : IC_Command(L"show_node")
{
	nv = pNV;
	if(nv == 0)
	{
		throw IC_Error(L"Valid node viewer pointer not supplied");
	}
	setUsage(L"show_node [model | map | level |test]");
	addDescLine(L"this command shows a node on screen");
	addDescLine(L"the node can be a model, a map or a test node (a cube)");
	addDescLine(L"  mode - shows an animated MD2 model");
	addDescLine(L"  map - shows a Quake III Level");
	addDescLine(L"  level - shows a Quake III Level");
	addDescLine(L"  test - shows a test cube node");
}
TestCommand_SHOWNODE::~TestCommand_SHOWNODE()
{
	nv = 0;
}
bool TestCommand_SHOWNODE::invoke(const array<WideString>& args, IC_Dispatcher *pDispatcher, IC_MessageSink* pOutput)
{
	if(args.size() == 0)
	{
		//this is one way of handling error
		//we print the usage to the console
		pOutput->logError_ANSI("no view mode supplied");
		printUsage(pOutput);
		return false;
	}
	else
	{
		WideString mode = args[0];
		if(mode == L"model")
		{
			nv->showModel();
		}
		else if(mode == L"map" || mode == L"level")
		{
			nv->showMap();
		}
		else if(mode == L"test")
		{
			nv->showTestNode();
		}
		else
		{
			//this is another way of error intimation
			//we throw an exception and the console takes
			//care of showing the message
			throw IC_Error("Invalid mode for node viewer specified. see usage for valid modes");
		}

	}
	return true;
}

TestCommand_HIDENODE::TestCommand_HIDENODE(NodeViewer *pNV) : IC_Command(L"hide_node")
{
	nv = pNV;
	if(nv == 0)
	{
		throw IC_Error(L"Valid node viewer pointer not supplied");
	}
	setUsage(L"hide_node");
	addDescLine(L"this command hides node on screen");
}
TestCommand_HIDENODE::~TestCommand_HIDENODE()
{
	nv = 0;
}
bool TestCommand_HIDENODE::invoke(const array<WideString>& args, IC_Dispatcher *pDispatcher, IC_MessageSink* pOutput)
{
	
	nv->hideNode();
	return true;
}