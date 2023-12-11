/**
 * ProgressBar.h
 *
 * Tank @ War Project
 * April 2011
 */

#ifndef PROGRESSBAR_H_
#define PROGRESSBAR_H_

#include <irrlicht.h>

using namespace irr;

class ProgressBar: public gui::IGUIElement {
public:
	/**
	 * Direction
	 */
	enum Direction {
		/**
		 * Horizontal direction
		 */
		HORIZONTAL,
		/**
		 * Vertical direction
		 */
		VERTICAL
	};
private:
	gui::IGUIFont* font;
	bool showText;
	u32 percentage;
	Direction direction;
	u32 borderSize;

	video::SColor fillColor;
	video::SColor backgroundColor;
	video::SColor borderColor;
	video::SColor textColor;

	video::ITexture* fillTexture;
	video::ITexture* backgroundTexture;
	/**
	 * Render target texture
	 */
	video::ITexture* texture;
	bool update;
public:
	/**
	 * Constructor of ProgressBar
	 * @param environment
	 * @param id
	 */
	ProgressBar(gui::IGUIEnvironment* environment, s32 id = -1);

	/**
	 * Destructor of ProgressBar
	 */
	virtual ~ProgressBar();

	/**
	 * Get percentage
	 * @return percentage
	 */
	u32 getPercentage() const;

	/**
	 * Set progress percentage
	 * @param percentage
	 */
	void setPercentage(const u32 percentage);

	/**
	 * Get direction horizontal or vertical
	 * @return direction
	 */
	ProgressBar::Direction getDirection();

	/**
	 * Set direction horizontal or vertical
	 * @param direction
	 * @see Direction
	 */
	void setDirection(Direction direction);

	/**
	 * Set width of progress bar
	 * @param width
	 */
	void setWidth(const u32 width);

	/**
	 * Set height of progress bar
	 * @param height
	 */
	void setHeight(const u32 height);

	/**
	 * Get dimension
	 * @return dimension
	 */
	core::recti getDimension();

	/**
	 * Set dimensions of progress bar
	 * @param width
	 * @param height
	 */
	void setDimension(const u32 width, const u32 height);

	/**
	 * Set dimensions of progress bar
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
	 * Set border size of progress bar
	 * @param size
	 */
	void setBorderSize(u32 size);

	/**
	 * Get fill color
	 * @return fillColor
	 */
	video::SColor getFillColor();

	/**
	 * Set fill color
	 * @param fillColor
	 */
	void setFillColor(video::SColor fillColor);

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
	 * Set text color
	 * @param textColor
	 */
	void setTextColor(video::SColor textColor);

	/**
	 * Set colors
	 * @param fillColor
	 * @param backgroundColor
	 * @param borderColor
	 * @param textColor
	 */
	void setColors(video::SColor fillColor, video::SColor backgroundColor, video::SColor borderColor, video::SColor textColor);

	/**
	 * Set fill texture
	 * @param texture
	 */
	void setFillTexture(video::ITexture* texture);

	/**
	 * Set fill texture
	 * @param texture
	 */
	void setBackgroundTexture(video::ITexture* texture);

	/**
	 * True when show percentage text
	 * @return showText
	 */
	bool isShowText();

	/**
	 * Show or hide percentage text
	 * @param show
	 */
	void setShowText(bool show);

	/**
	 * Set font of HUD
	 * @param font
	 */
	void setFont(gui::IGUIFont* font);

	/**
	 * Draw progress bar
	 */
	void draw();
private:
	/**
	 * Draw the progress bar
	 */
	void drawProgressBar();

	/**
	 * Update Progress bar
	 */
	void updateProgressBar();

	/**
	 * Update Render target texture
	 */
	void updateRenderTargetTexture();
};

#endif /* PROGRESSBAR_H_ */
