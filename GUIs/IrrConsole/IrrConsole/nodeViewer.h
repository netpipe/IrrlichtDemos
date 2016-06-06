#ifndef _IrrConsole_Test_NodeViewer_h_
#define _IrrConsole_Test_NodeViewer_h_
#include "console.h"
//! a simple node viewer interface
class NodeViewer
{
public:
	NodeViewer(){}
	virtual ~NodeViewer(){}

	virtual void showModel()=0;
	virtual void showMap()=0;
	virtual void showTestNode()=0;
	virtual void hideNode()=0;
};
//! a command for showing nodes
class TestCommand_SHOWNODE : public IC_Command
{
public:
	TestCommand_SHOWNODE(NodeViewer *pNV);
	virtual ~TestCommand_SHOWNODE();
	bool invoke(const array<WideString>& args, IC_Dispatcher *pDispatcher, IC_MessageSink* pOutput);
private:
	NodeViewer* nv;

};
//! a command to hide the node
class TestCommand_HIDENODE : public IC_Command
{
public:
	TestCommand_HIDENODE(NodeViewer *pNV);
	virtual ~TestCommand_HIDENODE();
	bool invoke(const array<WideString>& args, IC_Dispatcher *pDispatcher, IC_MessageSink* pOutput);
private:
	NodeViewer* nv;

};
#endif
