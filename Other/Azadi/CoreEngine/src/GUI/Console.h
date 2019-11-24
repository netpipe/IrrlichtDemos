#ifndef CONSOLE_H_01112006
#define CONSOLE_H_01112006

#include <list>
#include <sstream>
#include <fstream>
#include <map>

#ifndef __APPLE__
#include <GL/glew.h>
#include <GL/gl.h>
#else
#include <GL/glew.h>
#include <OpenGL/gl.h>
#endif

#include "../Input/ConsoleListener.h"
#include "../Input/Input.h"
#include "../Scripting/ScriptEngine.h"

class Input;
extern Input input;

//! GUI framework that contains all the GUI related code (Console, FPS counter, etc). The console is implemented in the global namespace with:  console
namespace GUI {
	//! Console class that handles displaying of all messages
	class Console : public ConsoleListener,KeyCatcher
	{
		friend class cmdparser;
        public:
		enum MessageType {error,warning,normal,user,log};
		enum MessageLevel {silent,low,lowish,medium,highish,high};
		class Message;
        private:
		int x;
		int y;
		int width;
		int height;
		int scrolledHeight;
		unsigned int leftPadding, topPadding;
		std::string bgTexture;
		std::string messageFont;
		unsigned long messageFontSize;
		unsigned long cursor; // cursor position --
		unsigned long history_pos; // pos in history --
		unsigned long history_size; // max stored strings --blub
		std::stringstream buffer;
		MessageType buffer_type;
		MessageLevel buffer_level;
		std::list<Message*> messages;
		MessageLevel verbosity_level;
		bool show;
		bool collapsed;
		bool has_focus;
		bool stick_bottom;
		std::list<Message*>::iterator start;
		std::list<std::string> user_commands;
		std::list<std::string>::iterator cycle_back_position;
		std::string user_command;
		std::vector<std::string> command_history;
		std::map<std::string,ConsoleListener*> listeners;
		std::ofstream logFile;

		ScriptEngine *script_engine;

        public:
		Console();
		Console(Console&);
		Console& operator<<(MessageType right);
		Console& operator<<(MessageLevel right);
		Console& operator<<(const std::string& right); // use reference instead --blub
		Console& operator<<(char); // special type
		/*Console& operator<<(std::string right); // replace those with a template --blub
		Console& operator<<(int);
		Console& operator<<(char);
		Console& operator<<(double);
		Console& operator<<(float);
		Console& operator<<(unsigned);
		Console& operator<<(long unsigned int);*/
		template<class T>
			Console& operator<<(const T t) // all unknown, not specially treated vars:
		{
			buffer << t;
			return (*this);
		}
		~Console();
		static char endl();
		void toggleShow();
		void draw();
		void initTextures();
		void setDimensions(const unsigned int &x, const unsigned int &y, const unsigned int &w, const unsigned int &h);
		inline bool isVisible() { return show; }
		bool recompileText;
		GLuint textList;

		void keyPressed(const SDLKey &keysym, const short &ch);
		void addMessage(MessageType,MessageLevel,std::string);
		void setVerbosityLevel(MessageLevel level);
		void increaseVerbosityLevel();
		void decreaseVerbosityLevel();
		void parseInput();
		/*! \brief Executes a registered Console command
		  \param command: The command, including its arguments, that you want to execute
		*/
		void insertUserCommand(std::string command);
		/*! \brief Register a new ConsoleListener
		  \param listener: Pointer to an inhereited ConsoleListener instance. Its acceptConsoleCommand functions will be called if the cmd was entered. Use splitArgs to get the single arguments
		  \param handle: The command which should be registered.
		*/
		void registerObject(ConsoleListener&,std::string);
		std::string acceptConsoleCommand(std::string);
	};

}

extern GUI::Console console;

#endif
