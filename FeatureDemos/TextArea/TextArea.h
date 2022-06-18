/**
 * TextArea.h
 *
 * Tank @ War Project
 * January 2012
 */

#ifndef TEXTAREA_H_
#define TEXTAREA_H_

#include <irrlicht.h>
#include "Line.h"
#include <vector>

using namespace irr;

/**
 * Struct holding text batch data
 */
struct TextBatch {
	gui::IGUIFont* font;
	gui::IGUISpriteBank* spriteBank;
	video::SColor color;
	core::array<u32> indices;
	core::array<core::position2di> positions;
};

/**
 * Struct holding image batch data
 */
struct ImageBatch {
	video::ITexture* image;
	core::array<core::position2di> positions;
	core::array<core::recti> rectangles;
};

class TextArea: public gui::IGUIElement {
public:
	/**
	 * Alignment
	 */
	enum Alignment {
		/**
		 * Left alignment
		 */
		LEFT,
		/**
		 * Right alignment
		 */
		RIGHT,
		/**
		 * Center alignment
		 */
		CENTER
	};
protected:
	/**
	 * Render target texture
	 */
	video::ITexture* texture;
	/**
	 * Text batches
	 */
	std::vector<TextBatch*> textBatches;
	/**
	 * Image batches
	 */
	std::vector<ImageBatch*> imageBatches;
	/**
	 * All lines added to the textarea
	 */
	std::vector<Line*> lines;
	/**
	 * All visible lines
	 */
	std::vector<Line*> visibleLines;
	/**
	 * Wrapping enabled
	 */
	bool wrapping;
	/**
	 * Auto scroll enabled
	 */
	bool autoScroll;
	/**
	 * Show scroll bar
	 */
	bool showScrollBar;
	/**
	 * Text alignment
	 */
	TextArea::Alignment alignment;
	/**
	 * Text area border size
	 */
	u32 borderSize;
	/**
	 * Text padding (text offset from borders)
	 */
	u32 padding;
	/**
	 * Scroll bar width
	 */
	u32 scrollBarWidth;
	/**
	 * Max lines visible in textarea
	 */
	u32 maxLines;
	/**
	 * Background color
	 */
	video::SColor backgroundColor;
	/**
	 * Border color
	 */
	video::SColor borderColor;
	/**
	 * Background texture
	 */
	video::ITexture* backgroundTexture;
	/**
	 * Textarea font
	 */
	gui::IGUIFont* font;

	/**
	 * Start line index
	 */
	s32 startIndex;

	/**
	 * Line Offset
	 */
	s32 lineOffset;

	/**
	 * Scrollbar
	 */
	gui::IGUIScrollBar* scrollBar;
	/**
	 * Update text area needed
	 */
	bool update;
	/**
	 * True if lines with a lifeTime are added
	 */
	bool temporaryLines;

public:

	/**
	 * Constructor of TextArea
	 * @param environment
	 * @param parent
	 * @param id
	 */
	TextArea(gui::IGUIEnvironment* environment, gui::IGUIElement* parent, s32 id = -1);

	/**
	 * Destructor of TextArea
	 */
	virtual ~TextArea();

	/**
	 * On event
	 * @param event
	 */
	virtual bool OnEvent(const SEvent& event);

	/**
	 * Get lines
	 * @return lines
	 */
	std::vector<Line*> getLines();

	/**
	 * Get lines
	 * @return lines
	 */
	std::vector<Line*> getLines() const;

	/**
	 * Set lines
	 * @param lines
	 */
	void setLines(std::vector<Line*> lines);

	/**
	 * Add new line to the textarea
	 * @param line
	 */
	void addLine(Line* line);

	/**
	 * Add new lines to the textarea
	 * @param newLines
	 */
	void addLines(std::vector<Line*> newLines);

	/**
	 * gets the number of lines to print
	 */
	u32 getNumberOfLines() const;

	/**
	 * Enable or disable text / line wrapping
	 * @param wrapping (default: true)
	 */
	void setWrapping(bool wrapping);

	/**
	 * Enable or disable auto line scrolling when new lines are added
	 * @param autoScroll (default: true)
	 */
	void setAutoScroll(bool autoScroll);

	/**
	 * Enable or disable visiblity of the scroll bar
	 * @param showScrollBar (default: true)
	 */
	void setShowScrollBar(bool showScrollBar);

	/**
	 * Set text alignment
	 * @param alignment (default: LEFT)
	 */
	void setAlignment(TextArea::Alignment alignment);

	/**
	 * Set width of textarea (relative from position)
	 * @param width
	 */
	void setWidth(u32 width);

	/**
	 * Set height of textarea (relative from position)
	 * @param height
	 */
	void setHeight(u32 height);

	/**
	 * Get dimension
	 * @return dimension
	 */
	core::recti getDimension();

	/**
	 * Set dimensions of textarea (relative from position)
	 * @param width
	 * @param height
	 */
	void setDimension(u32 width, u32 height);

	/**
	 * Set dimensions of textarea (relative from position)
	 * @param dimension
	 */
	void setDimension(core::recti dimension);

	/**
	 * Get position (top left corner)
	 * @return position
	 */
	core::vector2di getPosition() const;

	/**
	 * Set position (top left corner)
	 * @param position
	 */
	void setPosition(core::vector2di position);

	/**
	 * Set border size of textarea
	 * @param size
	 */
	void setBorderSize(u32 size);

	/**
	 * Set padding of textarea
	 * @param padding
	 */
	void setPadding(u32 padding);

	/**
	 * Set maximum amount of lines visible
	 * @param maxLines
	 */
	void setMaxLines(u32 maxLines);

	/**
	 * Set background color
	 * @param backgroundColor
	 */
	void setBackgroundColor(video::SColor backgroundColor);

	/**
	 * Set border color
	 * @param borderColor
	 */
	void setBorderColor(video::SColor borderColor);

	/**
	 * Set fill texture
	 * @param texture
	 */
	void setBackgroundTexture(video::ITexture* texture);

	/**
	 * Set font
	 * @param font
	 */
	void setFont(gui::IGUIFont* font);

	/**
	 * Set line spacing offset
	 * @param offset
	 */
	void setLineOffset(u32 offset);

	/**
	 * Set scrollbar with
	 * @param scrollBarWidth
	 */
	void setScrollBarWidth(u32 scrollBarWidth);

	/**
	 * Change start line index
	 * @param increment
	 */
	void changeStartIndex(s32 increment);

	/**
	 * Clears text area
	 */
	void clear();

	/**
	 * Draw the text area
	 */
	void draw();

private:
	/**
	 * Draw the text area
	 */
	void drawTextArea();

	/**
	 * Update TextArea
	 */
	void updateTextArea();

	/**
	 * Update Render target texture
	 */
	void updateRenderTargetTexture();

	/**
	 * Update scroll bar
	 */
	void updateScrollBar();

	/**
	 * Update lines
	 * @param scroll
	 */
	void updateLines();

	/**
	 * Update visible lines
	 * @param scroll
	 */
	void updateVisibleLines(bool scroll = true);

	/**
	 * Create text and image batches
	 */
	void createBatches();

	/**
	 * Get text batch by font and color
	 * @param font
	 * @param color
	 *
	 * @return textBatch
	 */
	TextBatch* getTextBatch(gui::IGUIFont* font, video::SColor color);

	/**
	 * Get text batch by image
	 * @param image
	 *
	 * @return imageBatch
	 */
	ImageBatch* getImageBatch(video::ITexture* image);
};

#endif /* TEXTAREA_H_ */
