/**
 * Line.h
 *
 * Tank @ War Project
 * March 2012
 */

#ifndef LINE_H_
#define LINE_H_

#define IMAGE_PLACEHOLDER "[Image_placeholder]"

#include <irrlicht.h>
#include <vector>

class Line {
private:
	irr::u32 lifeTime;
	irr::u32 createTime;
	std::vector<irr::core::stringw> strings;
	std::vector<irr::video::SColor> colors;
	std::vector<irr::video::ITexture*> images;
	irr::gui::IGUIFont* font;

public:
	/**
	 * Constructor of Line
	 * @param lifeTime (optional, 0 = maximum lifeTime)
	 */
	Line(irr::u32 lifeTime = 0);

	/**
	 * Constructor of Line
	 * @param font
	 * @param lifeTime (optional, 0 = maximum lifeTime)
	 */
	Line(irr::gui::IGUIFont* font, irr::u32 lifeTime = 0);

	/**
	 * Destructor
	 */
	virtual ~Line();

	/**
	 * Get lifetime of this line
	 * @return lifetime
	 */
	irr::u32 getLifetime() const;

	/**
	 * Set lifetime in this line
	 * @param lifetime
	 */
	void setLifetime(irr::u32 lifetimes);

	/**
	 * Get creation time of this line
	 * @return time
	 */
	irr::u32 getCreateTime() const;

	/**
	 * Return true if this line is a temporary line
	 * @return bool
	 */
	bool isTemporary();

	/**
	 * Return true if this line is expirede
	 * @return bool
	 */
	bool isExpired();

	/**
	 * Get strings in this line
	 * @return strings
	 */
	std::vector<irr::core::stringw> getStrings() const;

	/**
	 * Set strings in this line
	 * @param strings
	 */
	void setStrings(std::vector<irr::core::stringw> strings);

	/**
	 * Get colors in this line
	 * @return colors
	 */
	std::vector<irr::video::SColor> getColors() const;

	/**
	 * Set colors in this line
	 * @param colors
	 */
	void setColors(std::vector<irr::video::SColor> colors);

	/**
	 * Get images in this line
	 * @return images
	 */
	std::vector<irr::video::ITexture*> getImages() const;

	/**
	 * Set images in this line
	 * @param images
	 */
	void setImages(std::vector<irr::video::ITexture*> images);

	/**
	 * Add string to the line
	 * @param string
	 * @param color (optional)
	 */
	void addString(irr::core::stringw string, irr::video::SColor color = irr::video::SColor(255, 255, 255, 255));

	/**
	 * Add image to the line
	 * @param image
	 */
	void addImage(irr::video::ITexture* image);

	/**
	 * Get font
	 * @return font
	 */
	irr::gui::IGUIFont* getFont() const;

	/**
	 * Set font
	 * @param font
	 */
	void setFont(irr::gui::IGUIFont* font);
};

#endif /* LINE_H_ */
