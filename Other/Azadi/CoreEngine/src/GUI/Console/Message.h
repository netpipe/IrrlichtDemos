#ifndef CONSOLE_MESSAGE_H_02112006
#define CONSOLE_MESSAGE_H_02112006

#include "../Console.h"

#include <string>

namespace GUI {
class Console::Message {
	private:
		std::string content;
		MessageLevel level;
		MessageType type;
	public:
		Message(MessageType type, MessageLevel level, std::string content);
		MessageType getType();
		std::string getContent();
		MessageLevel getLevel();
};
}

#endif
		
