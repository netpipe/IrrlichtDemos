#ifndef __C_GUI_EDIT_BOX_IRB_H_INCLUDED__
#define __C_GUI_EDIT_BOX_IRB_H_INCLUDED__

#include "IrrCompileConfig.h"
#include "IrrlichtDevice.h"
#include "IGUIEditBox.h"
#include "irrArray.h"
#include "IOSOperator.h"
#include "IGUIScrollBar.h"
#include "IGUIStaticText.h"
#include "IGUIButton.h"
#include "IGUIContextMenu.h"

struct SKeyword {
	irr::core::stringc text;
	irr::video::SColor color;
	bool matchCase;
};

struct SGroupKeyword {
	irr::core::stringc text;
	irr::core::stringc endText;
	irr::video::SColor color;
	bool matchCase;
	bool multiLine;
};


struct SKeywordOverlay {
	irr::core::stringw text;
	irr::video::SColor color;
	irr::core::recti   rect;
};

struct SKeywordInfo {
	int size;
	int type;
};

struct STextAction {
	irr::core::stringw oldText, newText;
	int oldStart, oldEnd, oldLen;
	irr::core::stringw undo(irr::core::stringw target);

	irr::core::stringw redo(irr::core::stringw target);
};

namespace irr
{
	namespace gui
	{
		class CGUIEditBoxIRB : public IGUIEditBox
		{
		public:

			//! constructor
			CGUIEditBoxIRB(const wchar_t* text, bool border, bool lines, IGUIEnvironment* environment,
				IGUIElement* parent, s32 id, const core::rect<s32>& rectangle, IrrlichtDevice* device);

			//! destructor
			virtual ~CGUIEditBoxIRB();

			//! Sets another skin independent font.
			virtual void setOverrideFont(IGUIFont* font=0);

			//! Sets another color for the text.
			virtual void setOverrideColor(video::SColor color);

			//! Sets if the text should use the overide color or the
			//! color in the gui skin.
			virtual void enableOverrideColor(bool enable);

			//! Turns the border on or off
			virtual void setDrawBorder(bool border);

			//! Enables or disables word wrap for using the edit box as multiline text editor.
			virtual void setWordWrap(bool enable);

			//! Checks if word wrap is enabled
			//! \return true if word wrap is enabled, false otherwise
			virtual bool isWordWrapEnabled() const;

			//! Enables or disables newlines.
			/** \param enable: If set to true, the EGET_EDITBOX_ENTER event will not be fired,
			instead a newline character will be inserted. */
			virtual void setMultiLine(bool enable);

			//! Checks if multi line editing is enabled
			//! \return true if mult-line is enabled, false otherwise
			virtual bool isMultiLineEnabled() const;

			//! Enables or disables automatic scrolling with cursor position
			//! \param enable: If set to true, the text will move around with the cursor position
			virtual void setAutoScroll(bool enable);

			//! Checks to see if automatic scrolling is enabled
			//! \return true if automatic scrolling is enabled, false if not
			virtual bool isAutoScrollEnabled() const;

			//! Gets the size area of the text in the edit box
			//! \return Returns the size in pixels of the text
			virtual core::dimension2du getTextDimension();

			//! Sets text justification
			virtual void setTextAlignment(EGUI_ALIGNMENT horizontal, EGUI_ALIGNMENT vertical);

			//! called if an event happened.
			virtual bool OnEvent(const SEvent& event);

			//! draws the element and its children
			virtual void draw();

			//! Sets the new caption of this element.
			virtual void setText(const wchar_t* text);

			//! Sets the maximum amount of characters which may be entered in the box.
			//! \param max: Maximum amount of characters. If 0, the character amount is
			//! infinity.
			virtual void setMax(u32 max);

			//! Returns maximum amount of characters, previously set by setMax();
			virtual u32 getMax() const;

			//! Sets whether the edit box is a password box. Setting this to true will
			/** disable MultiLine, WordWrap and the ability to copy with ctrl+c or ctrl+x
			\param passwordBox: true to enable password, false to disable
			\param passwordChar: the character that is displayed instead of letters */
			virtual void setPasswordBox(bool passwordBox, wchar_t passwordChar = L'*');

			//! Returns true if the edit box is currently a password box.
			virtual bool isPasswordBox() const;

			//! Updates the absolute position, splits text if required
			virtual void updateAbsolutePosition();

			//! Writes attributes of the element.
			virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const;

			//! Reads attributes of the element
			virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);

			//! Set and overide the background color
			void setBackgroundColor(video::SColor color);

			//! Set the linecount background colors;
			void setLineCountColors(video::SColor background, video::SColor line, video::SColor text);
			
			//! Display or hide the Linecount
			void setDisplayLineCount(bool display);

			//! Return the current status of the linecount display GUI
			bool isLineCountDisplayed();

			//! Change the text of the element (for multilanguage support)
			void setElementText(u32 index, core::stringw text);

			//! Change the colors of the text selection
			void setSelectionColors(video::SColor background, video::SColor text, video::SColor background1 = video::SColor(255,130,130,180));

			//! copies text between markers
			void copy();

			//! cuts text between markers
			void cut();

			//! pastes text between markers
			void paste();

			//! deletes text between markers
			void deleteText();

			//! add a keyword to highlight
			void addKeyword(irr::core::stringc keyword, irr::video::SColor color, bool matchCase = true);

			//! add a keyword to highlight an entire line
			void addLineKeyword(irr::core::stringc keyword, irr::video::SColor color, bool matchCase = true);

			//! add a keyword to highlight a group
			void addGroupKeyword(irr::core::stringc keyword, irr::core::stringc endKeyword, irr::video::SColor color, bool matchCase = true, bool multiLine = false);

			//! adds keyword info
			void addKeywordInfo(int size, int type);

			//! clears all keywords
			void clearKeywords();

			//! adds C++ style keywords
			void addCppKeywords(irr::video::SColor keywordColor = irr::video::SColor(255,0,0,200), irr::video::SColor stringColor = irr::video::SColor(255,170,0,0), irr::video::SColor commentColor = irr::video::SColor(255,0,150,0));
			
			//! adds LUA style keywords
			void addLUAKeywords(irr::video::SColor keywordColor = irr::video::SColor(255,0,0,200), irr::video::SColor stringColor = irr::video::SColor(255,170,0,0), irr::video::SColor commentColor = irr::video::SColor(255,0,150,0));
			
			//! Replace text for easy undo/redo and line updates
			void replaceText(int start, int end, irr::core::stringw newText);
			void undo();
			void redo();
			//! add a keyword to highlight a group
			
			// Enums for the elements that compose the code editor
			enum GUI
			{
				CM_CUT = 0,
				CM_COPY,
				CM_PASTE,
				CM_DELETE,
				CM_UNDO,
				CM_REDO,
				BT_LINECOUNT,
				GUI_COUNT,
			};

		protected:
			//! Breaks the single text line.
			void breakText();
			//! sets the area of the given line
			void setTextRect(s32 line);
			//! returns the line number that the cursor is on
			s32 getLineFromPos(s32 pos);
			//! returns whether a point is within the selection
			bool isPointInSelection(core::position2di pos);
			//! adds a letter to the edit box
			void inputChar(wchar_t c);
			//! calculates the current scroll position
			void calculateScrollPos();
			//! send some gui event to parent
			void sendGuiEvent(EGUI_EVENT_TYPE type);
			//! set text markers
			void setTextMarkers(s32 begin, s32 end);


			bool processKey(const SEvent& event);
			bool processMouse(const SEvent& event);
			s32 getCursorPos(s32 x, s32 y);

			bool MouseMarking;
			bool Border;
			bool OverrideColorEnabled;
			bool LineNumbering;
			bool Inserting;

			s32 MarkBegin;
			s32 MarkEnd;

			video::SColor OverrideColor;
			gui::IGUIFont *OverrideFont, *LastBreakFont;
			IOSOperator* Operator;
			IrrlichtDevice* IRRdevice;

			IGUIScrollBar * Scrollbar;
			IGUIScrollBar * ScrollbarH;
			IGUIButton *   LineToggle;
			irr::gui::IGUIContextMenu* menu;

			core::stringw  linenumber; //contain the line numbering

			u32 BlinkStartTime;
			s32 CursorPos;
			s32 HScrollPos, VScrollPos; // scroll position in characters
			s32 lineCount;
			u32 Max;
			s32 RightSpace;
			s32 LeftSpace;
			s32 UpperSpace;
			s32 LowerSpace;

			s32  OldMarkBegin, OldMarkEnd;
			bool InMenu;
			bool WordWrap, MultiLine, AutoScroll, PasswordBox;
			bool SearchingForEndOfGroup;
			video::SColor GroupColor;

			// Override color for the interface
			video::SColor BackgroundColor;
			video::SColor LinecountBackColor;
			video::SColor LinecountLineColor;
			video::SColor LinecountTextColor;
			video::SColor SelectionBackColor;
			video::SColor SelectionBackColor1;
			video::SColor SelectionTextColor;

			bool DoubleClicked;
			irr::core::stringc EndOfGroupText;
			wchar_t PasswordChar;
			EGUI_ALIGNMENT HAlign, VAlign;

			core::array< core::stringw > BrokenText;
			core::array< s32 > BrokenTextPositions;

			core::rect<s32> CurrentTextRect, FrameRect; // temporary values

			irr::core::recti myRect(int x, int y, int w, int h);

			// Arrays for the keywords informations
			irr::core::array<SKeyword> Keywords;
			irr::core::array<SKeyword> LineKeywords;
			irr::core::array<SGroupKeyword> GroupKeywords;
			irr::core::array<SKeywordInfo> KeywordList;
			irr::core::array<STextAction> RedoList;
			irr::core::array<STextAction> UndoList;

			//Store the text information for the menus
			irr::core::stringw menustring[GUI_COUNT];

			
		};


	} // end namespace gui
} // end namespace irr

#endif // __C_GUI_EDIT_BOX_IRB_H_INCLUDED__