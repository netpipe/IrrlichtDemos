
#define LINE_COUNT		5
#define MARGIN_X		10
#define MARGIN_Y		10
#define MAX_LINE_WIDTH	500
#define FONT_HEIGHT		16
#define LINE_SPACING	16

// Displays lines of text on the screen
class ScreenLog
{
	gui::IGUIStaticText *lines[LINE_COUNT];
	
public:
	
	ScreenLog()
	{
		int x = MARGIN_X;
		int y = engine.getVideoDriver()->getViewPort().getHeight() - LINE_COUNT*LINE_SPACING - MARGIN_Y;
		
		for (int i = 0; i < LINE_COUNT; i ++)
		{
			gui::IGUIStaticText *text = engine.getGUIEnvironment()->addStaticText(L"",
					core::rect<s32>(x,y,x+MAX_LINE_WIDTH,y+FONT_HEIGHT),
					false, false, NULL, -1, false
					);
			
			text->setOverrideColor(video::SColor(160, 0,0,0));
			
			lines[i] = text;
			
			y += LINE_SPACING;
		}
	}
	
	void writeLine(const wchar_t *str)
	{
		// Move all lines down one, and add the new one to the top.
		for (int i = LINE_COUNT-1; i > 0; i --)
		{
			lines[i]->setText( lines[i-1]->getText() );
		}
		lines[0]->setText(str);
	}
	
	void clear()
	{
		for (int i = 0; i < LINE_COUNT; i ++)
			lines[i]->setText(L"");
	}
};
