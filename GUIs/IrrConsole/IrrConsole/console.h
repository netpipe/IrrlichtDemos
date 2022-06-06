#ifndef _IrrConsole_Console_h_
#define _IrrConsole_Console_h_
#include "includes.h"
#include "utils.h"
//=====================================================================================
//advance declarations
class  IC_MessageSink;
class  IC_Command;
class  IC_Dispatcher;
class  IC_Console;
struct IC_IC_ConsoleConfig;
using  std::map;
//=====================================================================================
//!the message sink interface
class IC_MessageSink
{
public:
	//! constructor
	IC_MessageSink();
	//! destructor
	virtual ~IC_MessageSink();

	//! log a UTF-8 message to the sink
	void logMessage_ANSI(irr::ELOG_LEVEL logLevel, const String message);
	//! log a UTF-8 message to the sink at info log level
	void logInfo_ANSI(const String message);
	//! log a UTF-8 message to the sink at warning log level
	void logWarning_ANSI(const String message);
	//! log a UTF-8 message to the sink at error log level
	void logError_ANSI(const String message);

	//! log a UTF-16 message to the sink
	void logMessage(irr::ELOG_LEVEL logLevel,const WideString message);
	//! log a UTF-16 message to the sink at info log level
	void logInfo(const WideString message);
	//! log a UTF-16 message to the sink at warning log level
	void logWarning(const WideString message);
	//! log a UTF-16 message to the sink at error log level
	void logError(const WideString message);


	//! get a string for a log level
	virtual const WideString getLevelTag(irr::ELOG_LEVEL logLevel);
	//! add a UTF-16 message to the sink
	virtual void appendMessage(const WideString message)=0;
	//! clear all the messages in the sink
	virtual void clearMessages() = 0;
	//! set the visibility
	virtual void toggleVisible() = 0;

};
//=====================================================================================
//! the command class
class IC_Command
{
public:
	//! the destructor
	virtual ~IC_Command();

	//! invoke the command with supplied args
	virtual bool invoke(const array<WideString>& args, IC_Dispatcher* pDispatcher, IC_MessageSink* pOutput)=0;

	//! get command name
	const WideString getName();
	//! get the usage string for the command
	const WideString getUsage();
	//! get the description line count
	const irr::u32 getDescLineCount();
	//! get the desc line at
	const WideString getDescLine(const u32 index);

	//! print the usage to the message sink
	void printUsage(IC_MessageSink* pOutput);
	//! print the description to the message sink
	void printDesc(IC_MessageSink* pOutput);

protected:
	//! constructor
	IC_Command(const WideString name);
	//! set usage
	void setUsage(const WideString usage);
	//! append a line to the desc array
	void addDescLine(const WideString line);
	//! set descrition to ""
	void clearDescLines();
private:
	//! the name of the command
	WideString commandName;
	//! the usage string for the command
	WideString commandUsage;
	//! the description lines
	array<WideString> commandDescLines;
};
//=====================================================================================
//! the dispatcher interface
class IC_Dispatcher
{
public:
	//! destructor
	virtual ~IC_Dispatcher();
	//! register a command (the cmd->getName() should not already be registered else an exception is thrown)
	void registerCommand(IC_Command* cmd);
	//! check if a command is already registered
	bool hasCommand(const WideString cmdName);
	//! dispatch a command with the name and the supplied args
	void dispatch(const WideString cmdName, const array<WideString>& args, IC_MessageSink* pOutput);
	//! deregister (remove) a command
	void deregisterCommand(const WideString cmdName);
	//! get the list of registered commands
	u32  getRegisteredCommands(array<WideString>& cmdNames);
	//! deregister all commands
	void deregisterAllCommands();

	//! print a list of commands
	void printCommandList(IC_MessageSink* pOutput, bool bUsage);
	//! print the command usage
	void printCommandUsage(const WideString cmdName, IC_MessageSink* pOutput);
	//! print detailed description
	void printCommandDesc(const WideString cmdName, IC_MessageSink* pOutput);
protected:
	//! constructor
	IC_Dispatcher();
private:
	//! the map of command name to command pointer
	map<WideString,IC_Command*> commandTable;
};
//=====================================================================================
//! an alignment enumeration for vertical alignment
enum IC_VerticalAlignment
{
	//! top
	VAL_TOP = 0,
	//! middle
	VAL_MIDDLE = 1,
	//! bottom
	VAL_BOTTOM = 2
};
//=====================================================================================
//! an alignment enumeration for horizontal alignment
enum IC_HorizontalAlignment
{
	//! left
	HAL_LEFT = 0,
	//! center
	HAL_CENTER = 1,
	//! right
	HAL_RIGHT = 2
};
//=====================================================================================
//! the console config structure
struct IC_ConsoleConfig
{
public:
	//! constructor
	IC_ConsoleConfig(){ setDefaults(); }
	//! set the defaults on the console config
	void setDefaults()
	{
		dimensionRatios.X = 1.0f;
		dimensionRatios.Y = 0.6f;
		lineSpacing = 2;
		indent = 1;
		valign= VAL_TOP;
		halign= HAL_LEFT;
		bShowBG = true;
		bgColor = irr::video::SColor(150,10,10,70);
		fontColor = irr::video::SColor(200,200,200,200);
		fontName = "data/font/console.bmp";
		prompt = "console";
		commandHistorySize = 10;
	}

	//! this contains the Width and Height ratios to the main view port (0 - 1)
	vector2df dimensionRatios;

	//! this is the spacing between two lines in pixels (Default / Min : 2)
	u32 lineSpacing;

	//! this is the indentation for each line in pixels (Default / Min : 1)
	u32 indent;

	//! this is the alignment flag for the vertical placement of the console
	IC_VerticalAlignment valign;

	//! this is the alignment flag for the horizontal alignment of the console
	IC_HorizontalAlignment halign;

	//! this is the flag indicating if the console BG should be drawn
	bool bShowBG;

	//! this is the color for the console BG
	irr::video::SColor bgColor;

	//! this is the font color for the console
	irr::video::SColor fontColor;

	//! this is the font name
	String fontName;

	//! this is the prompt string displayed as prompt$>
	String prompt;

	//! this is the command history length (defaults to 10)
	u32 commandHistorySize;

};
//=====================================================================================
//! A Quake Like console class
class IC_Console : public IC_Dispatcher, public IC_MessageSink
{
public:
	static const wchar_t IC_KEY_TILDE;
	//! constructor
	IC_Console();
	//! destructor
	virtual ~IC_Console();

	//! get the console config reference
	IC_ConsoleConfig& getConfig();
	//! (re)initialize the console with current config
	void initializeConsole(irr::gui::IGUIEnvironment* guienv, const irr::core::dimension2d<u32>& screenSize);

	//! loads a few default commands into the console
	void loadDefaultCommands(irr::IrrlichtDevice* device);
	//! should console be visible
	bool isVisible();
	//! set the visibility of the console
	void setVisible(bool bV);
	//! toggle the visibility of the console
	void toggleVisible();

	//
	//	Message Sink implementation
	//
	//! add a UTF-16 message to the sink
	void appendMessage(const WideString message);
	//! clear all the messages in the sink
	void clearMessages();

	//
	//	console rendering
	//

	//! render the console (it internally checks if the console is visible)
	void renderConsole(irr::gui::IGUIEnvironment* guienv, irr::video::IVideoDriver *videoDriver, const u32 deltaMillis);

	//
	//	console message handling
	//

	//! handles a key press when console is active/visible
	void handleKeyPress(wchar_t keyChar, irr::EKEY_CODE keyCode, bool bShiftDown, bool bControlDown);

private:
	//! parses and handles the current command string
	void handleCommandString(WideString& wstr);
	//! add a line to history
	void addToHistory(WideString& line);
	//! calculate the whole console rect
	void calculateConsoleRect(const irr::core::dimension2d<u32>& screenSize);
	//! calculate the messages rect and prompt / shell rect
	void calculatePrintRects(rect<s32>& textRect,rect<s32>& shellRect);
	//! calculate the various limits of the console
	bool calculateLimits(u32& maxLines, u32& lineHeight,s32& fontHeight);
	//! resize the message count
	void resizeMessages();
	//! do a tab completion
	void tabComplete();

	//! console config data
	IC_ConsoleConfig consoleConfig;

	//! visibility flag
	bool bVisible;

	//! the font of the console
	irr::gui::IGUIFont* guiFont;

	//! the console rectangle
	irr::core::rect<s32> consoleRect;

	//! the console messages
	array<WideString> consoleMessages;

	//! the command history
	array<WideString> consoleHistory;

	//! the history pointer / index
	u32 consoleHistoryIndex;

	//! the current command string
	WideString currentCommand;
};
//=====================================================================================
#endif
