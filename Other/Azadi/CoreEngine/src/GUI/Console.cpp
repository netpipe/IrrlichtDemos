#include "Console.h"

#include <iostream>
#include "Console/Message.h"

#include "SDL/SDL.h"

#include "../Azadi.h"
#include "Drawing.h"
#include "../Fonts/Font.h"

#include "../Input/ConsoleListener.h"
#include <ctime>

extern Input input;

//! Console constructor
GUI::Console::Console()
{
    logFile.open("./log.txt", std::ios::out);
    time_t *curTime = new time_t;
    time(curTime);

    logFile << "Began logging at: " << ctime(curTime) << "\n";

	history_size = 128;
	show = false;
	collapsed = false;
	x = 0;
	y = 0;
	leftPadding=4;
	topPadding=1;
	has_focus = false;
	verbosity_level = medium;
	buffer_type = normal;
	buffer_level = highish;
	width = 1024;
	height = 256;
	start = messages.begin();
	stick_bottom = true;
    messageFont="teen bold.ttf";
	messageFontSize=11;
	bgTexture = "Console.png";
	addMessage(normal,highish,"Initialising Console");
	this->registerObject(*this,"console");

    delete curTime;
}

//! Registers an object to the Console
void GUI::Console::registerObject(ConsoleListener& listener,std::string handle)
{
	listeners[handle] = &listener;
}

//! Console destructor
GUI::Console::~Console()
{
    std::list<Message *>::iterator it = messages.begin();
    while (it != messages.end())
    {
        delete *it;
        it++;
    }
    logFile << "\n" << "End Log\n";
    logFile.close();
}

//! Accepts console commands from the user
std::string GUI::Console::acceptConsoleCommand(std::string command)
{
    recompileText = true;
	ScriptEngine::getInstance()->dostring(command.c_str());

	return "";
}

//! No documentation just yet
void GUI::Console::insertUserCommand(std::string command)
{
    recompileText = true;
	user_commands.push_back(command);
	cycle_back_position = user_commands.end();
	std::size_t dot_pos = command.find(' ',0);
	if(command == "quit")
        Azadi::getInstance()->shutdown();
	else if(command != "help")
	{
		std::map<std::string,ConsoleListener*>::iterator listener = listeners.find(command.substr(0,dot_pos));
		if (listener == listeners.end())
			ScriptEngine::getInstance()->dostring(command.c_str());
		else
			listener->second->acceptConsoleCommand(command.substr(dot_pos+1,command.size()));
	}
	else
		for(std::map<std::string,ConsoleListener*>::iterator it = listeners.begin(); it != listeners.end();it++)
			*this << it->first << " - " << it->second->shortDescr() << endl();
}

//! Need to study this one more
void GUI::Console::parseInput()
{
	if (has_focus)
	{
		if (input.getMouseClick(SDL_BUTTON_LEFT))
			if (!((input.getMouseY()>y+height-15) && (input.getMouseY()<y+height)) || !((input.getMouseX()>x) && (input.getMouseX()<x+width)))
				has_focus = false;
	}
}

//! Initialize the textures for the GUI
void GUI::Console::initTextures()
{
    TextureFactory::getInstance()->loadTexture(bgTexture);
    Font::getInstance()->loadFont(messageFont);
    Font::getInstance()->genPointSize(messageFont, messageFontSize);
    textList = glGenLists(1);
    recompileText = true;
}

void GUI::Console::setDimensions(const unsigned int &X, const unsigned int &Y, const unsigned int &w, const unsigned int &h)
{
    x = X;
    y = Y;
    width = w;
    height = h;
}

//! Toggles the showing of the Console
void GUI::Console::toggleShow()
{
	show = !show;
	if (show)
        input.setKeyCatcher(this);
    else
        input.clearKeyCatcher();
}

//! Increases the verbosity of the Console
void GUI::Console::increaseVerbosityLevel()
{
	if (verbosity_level==highish)
		verbosity_level = high;
	if (verbosity_level==medium)
		verbosity_level = highish;
	if (verbosity_level==lowish)
		verbosity_level = medium;
	if (verbosity_level==low)
		verbosity_level = lowish;
}

//! Decreases the verbosity of the Console
void GUI::Console::decreaseVerbosityLevel()
{
	if (verbosity_level==lowish)
		verbosity_level = low;
	if (verbosity_level==medium)
		verbosity_level = lowish;
	if (verbosity_level==highish)
		verbosity_level = medium;
	if (verbosity_level==high)
		verbosity_level = highish;
}

//! Draws the console on screen
void GUI::Console::draw()
{
	if (!show)
	{
	    scrolledHeight = 0;
	    return;
	}

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

	TextureFactory::getInstance()->applyTexture(bgTexture);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.0f, 1.0f);
        glVertex2i(x, y);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2i(x + width, y);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2i(x + width, y + scrolledHeight);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2i(x, y+scrolledHeight);
    }
    glEnd();

    glTranslatef(0.0f, 0.0f, 1.0f);

	if (!collapsed) {

		glLoadIdentity ();

        float fontAdvance = Font::getInstance()->getFontHeight(messageFont, messageFontSize);
		float thisy = float(scrolledHeight) - (fontAdvance * 2);

        glTranslatef(x + leftPadding, 0.0f, 0.0f);

        if (recompileText)
        {
            thisy = height - (fontAdvance * 2);
            glDeleteLists(textList, 1);
            glNewList(textList, GL_COMPILE);
            for(std::list<Message*>::iterator iter = start; iter != messages.end(); ++iter)
            {
                glPushMatrix();
                glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
                if ((*iter)->getType() == error) glColor4f(1.0,0.0,0.0,1.0);
                if ((*iter)->getType() == warning) glColor4f(1.0,0.6,0.0,1.0);
                if ((*iter)->getType() == normal) glColor4f(1.0,1.0,1.0,1.0);
                if ((*iter)->getType() == user) glColor4f(0.7,0.7,0.2,1.0);
                if ((*iter)->getLevel()<=verbosity_level)
                {
                    glTranslatef(0.0f, thisy, 0.0f);
                    Font::getInstance()->drawText((*iter)->getContent().c_str(), messageFont, messageFontSize, 0);
                    thisy -= fontAdvance;
                }
                glPopMatrix();
                if (thisy > height + topPadding)
                    break;
            }
            glEndList();
            recompileText = false;
        }

        glPushMatrix();
        glTranslatef(0.0f, scrolledHeight - height, 0.0f);
        glCallList(textList);
        glPopMatrix();

		glColor4f(1.0,1.0,1.0,1.0);

		static unsigned int cursor_blinktime = 0; // a global static unsigned integer should do the job --blub
		cursor_blinktime += Drawing::getInstance()->getLastFrameLength();//getLastFrameLength();

        glPushMatrix();
        glTranslatef(1.0f, scrolledHeight - fontAdvance, 0.0f);
        Font::getInstance()->drawText(std::string("> ") + user_command, messageFont, messageFontSize, 0);
        glPopMatrix();
		if( (cursor_blinktime/400) & 1 )
		{ // blink every 400 milliseconds
			glPushMatrix();
			float cursorPos = Font::getInstance()->textWidth(std::string("> ") + user_command.substr(0, cursor), messageFont, messageFontSize);
			glPushMatrix();
			glTranslatef(cursorPos, scrolledHeight - fontAdvance, 0.0f);
			Font::getInstance()->drawText("_", messageFont, messageFontSize, 0);
			glPopMatrix();
		}

		glColor4f(1.0,1.0,1.0,1.0);

	}

    scrolledHeight+= int(360.0f / Drawing::getInstance()->getFps());
    (scrolledHeight > height)?scrolledHeight = height : scrolledHeight=scrolledHeight;

	glColor3f(1.0,1.0,1.0);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}

//! Adds a message to the console
void GUI::Console::addMessage(Console::MessageType type, Console::MessageLevel level, std::string content)
{
    recompileText = true;
    if (type==log || type==error || type==warning)
    {
        logFile << content << "\n";
    }

    if (level!=silent)
    {
        printf("%s\n",content.c_str());
        messages.push_front(new Message(type,level, content));
        if (stick_bottom)
            start = messages.begin();
    }
}

//! Sets the verbosity level to level
void GUI::Console::setVerbosityLevel(MessageLevel level)
{
	verbosity_level = level;
}

//! Puts a MessageType into the console
GUI::Console& GUI::Console::operator<<(Console::MessageType right)
{
	buffer_type = right;
	return *this;
}

//! Puts a MessageLevel into the console
GUI::Console& GUI::Console::operator<<(Console::MessageLevel right)
{
	buffer_level = right;
	return *this;
}

//! Puts a String into the console
GUI::Console& GUI::Console::operator<<(const std::string& right) // use reference, don't copy --blub
{
	buffer << right;
	return *this;
}


//! Puts a char into the console
GUI::Console& GUI::Console::operator<<(char right)
{
	if (right=='\n') {
		addMessage(buffer_type,buffer_level,buffer.str());
		buffer_type = normal;
		buffer_level = medium;
		buffer.str("");
		//cursor = 0;
	} else {
		buffer << right;
		//buffer.str().insert(cursor++, 1, right);
	}
	return *this;
}

/*
//! Puts an int into the console
GUI::Console& GUI::Console::operator<<(int right)
{
	buffer << right;
	return *this;
}

//! Puts a float into the console
GUI::Console& GUI::Console::operator<<(float right)
{
	buffer << right;
	return *this;
}

//! Puts a double into the console
GUI::Console& GUI::Console::operator<<(double right)
{
	buffer << right;
	return *this;
}

//! Puts an unsigned into the console
GUI::Console& GUI::Console::operator<<(unsigned right)
{
	buffer << right;
	return *this;
}

//! Puts an long unsigned int into the console
GUI::Console& GUI::Console::operator<<(long unsigned int right)
{
	buffer << right;
	return *this;
	}*/

//! Puts an endline into the console
char GUI::Console::endl()
{
	return '\n';
}

void GUI::Console::keyPressed(const SDLKey &sym, const short &ch)
{
    recompileText = true;
    if (sym == SDLK_TAB)
    {
        toggleShow();
        return;
    }
	if (sym == SDLK_BACKSPACE)
	{
		if (user_command.size() && cursor > 0) {
			user_command.erase(--cursor, 1);
		}
		return;
	}
	if (sym == SDLK_DELETE)
	{
		if(user_command.size() > cursor) {
			user_command.erase(cursor, 1);
		}
		return;
	}
	if (sym == SDLK_LEFT)
	{
		if(cursor)
			--cursor;
		return;
	}
	if (sym == SDLK_RIGHT)
	{
		if(cursor < user_command.size())
			++cursor;
		return;
	}
	if (sym == SDLK_KP_ENTER || sym == SDLK_RETURN )
	{
		if(!history_size)
			history_size = 32;
		if(user_command.size())
		{
			command_history.push_back(user_command);
                        insertUserCommand(user_command);
		}
		if(command_history.size() > history_size)
		{
			command_history.erase(
				command_history.begin(),
				command_history.end() - history_size);
		}
		user_command = "";
		history_pos = cursor = 0;
		return;
	}
	if(command_history.size())
	{
		if (sym == SDLK_DOWN)
		{
			history_pos = (history_pos+1) % command_history.size();
			user_command = command_history[history_pos];
			return;
		}
		if (sym == SDLK_UP)
		{
			if(history_pos == 0)
				history_pos = command_history.size()-1;
			else
				history_pos = (history_pos-1) % command_history.size();
			user_command = command_history[history_pos];
			return;
		}
	}
	if (sym == SDLK_END)
	{
		cursor = user_command.size();
		return;
	}
	if (sym == SDLK_HOME)
	{
		cursor = 0;
		return;
	}
	if(ch)
		user_command.insert(cursor++, 1, ch);
}
