#ifndef _IrrConsole_Default_Cmds_h_
#define _IrrConsole_Default_Cmds_h_
#include "includes.h"
#include "utils.h"
#include "console.h"

//
//	Default Command Set
//
class IC_Command_ECHO : public IC_Command
{
public:
	IC_Command_ECHO();
	virtual ~IC_Command_ECHO();
	bool invoke(const array<WideString>& args, IC_Dispatcher* pDispatcher, IC_MessageSink* pOutput);
};

class IC_Command_HELP : public IC_Command
{
public:
	IC_Command_HELP();
	virtual ~IC_Command_HELP();
	bool invoke(const array<WideString>& args, IC_Dispatcher* pDispatcher, IC_MessageSink* pOutput);
};

class IC_Command_LIST : public IC_Command
{
public:
	IC_Command_LIST();
	virtual ~IC_Command_LIST();
	bool invoke(const array<WideString>& args, IC_Dispatcher* pDispatcher, IC_MessageSink* pOutput);
};

class IC_Command_DRIVER_INFO : public IC_Command
{
public:
	IC_Command_DRIVER_INFO(irr::IrrlichtDevice *pDevice);
	virtual ~IC_Command_DRIVER_INFO();
	bool invoke(const array<WideString>& args, IC_Dispatcher* pDispatcher, IC_MessageSink* pOutput);
private:
	irr::IrrlichtDevice *device;
};

class IC_Command_EXIT : public IC_Command
{
public:
	IC_Command_EXIT();
	virtual ~IC_Command_EXIT();
	bool invoke(const array<WideString>& args, IC_Dispatcher* pDispatcher, IC_MessageSink* pOutput);

};

class IC_Command_QUIT : public IC_Command
{
public:
	IC_Command_QUIT();
	virtual ~IC_Command_QUIT();
	bool invoke(const array<WideString>& args, IC_Dispatcher* pDispatcher, IC_MessageSink* pOutput);

};

class IC_Command_CLS : public IC_Command
{
public:
	IC_Command_CLS();
	virtual ~IC_Command_CLS();
	bool invoke(const array<WideString>& args, IC_Dispatcher* pDispatcher, IC_MessageSink* pOutput);

};
#endif