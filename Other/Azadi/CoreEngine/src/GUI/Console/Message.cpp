#include "Message.h"

GUI::Console::Message::Message(MessageType type, MessageLevel level, std::string content) {
	this->level = level;
	this->type = type;
	this->content = content;
}

GUI::Console::MessageType GUI::Console::Message::getType() {
	return type;
}

std::string GUI::Console::Message::getContent() {
	return content;
}

GUI::Console::MessageLevel GUI::Console::Message::getLevel() {
	return level;
}

