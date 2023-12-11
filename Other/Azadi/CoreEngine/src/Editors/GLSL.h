#ifndef GLSLEDITOR_H
#define GLSLEDITOR_H

#include <vector>
#include <string>

#include "../GUI/Button.h"
#include "../Input/Input.h"

class GLSLEditor : public KeyCatcher
{
    public:
        GLSLEditor(const unsigned int &x, const unsigned int &y, const unsigned int &w, const unsigned int &h);
        ~GLSLEditor();

        void tick(Input &input);
        void draw();

        inline bool hasFocus() { return Focus; }
        inline bool isVisible() { return Visible; }
        inline void toggleFocus() { Focus = !Focus; }
        inline void toggleVisible() { Visible = !Visible; }
        inline void activate() { Focus = true; Visible = true; }
        inline void deactivate() { Focus = false; Visible = false; }

        void keyPressed(const SDLKey &sym, const short &ch);
    private:
        unsigned int x, y, w, h;
        unsigned int textWidth, textHeight;

        bool Focus;
        bool Visible;
        bool mouseHeld;
        bool resizing;
        bool highlighting;
        unsigned int mouseHitX, mouseHitY;

        std::string fontname;
        float pointSize;
        unsigned int lineMaxWidth;

        std::vector<std::string>::iterator currentLine;
        unsigned long int cursorPos;
        std::vector<std::string> textBuffer;
};

#endif
