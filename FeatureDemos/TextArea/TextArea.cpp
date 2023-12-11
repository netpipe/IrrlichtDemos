/*
 Copyright (C) 2011-2012 Thijs Ferket (RdR)

 This software is provided 'as-is', without any express or implied
 warranty.  In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

 1. The origin of this software must not be misrepresented; you must not
 claim that you wrote the original software. If you use this software
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.
 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original software.
 3. This notice may not be removed or altered from any source distribution.
 */

#include "TextArea.h"

#include <algorithm>
#include "CGUIFont.h"

using namespace irr;
using namespace gui;

TextArea::TextArea(gui::IGUIEnvironment* environment, gui::IGUIElement* parent, s32 id) :
		gui::IGUIElement(gui::EGUIET_ELEMENT, environment, parent, id, core::rect<s32>(10, 10, 110, 50)) {
	wrapping = true;
	autoScroll = true;
	showScrollBar = false;
	alignment = LEFT;

	borderSize = 1;
	padding = 5;
	scrollBarWidth = 20;
	maxLines = 10;

	backgroundColor = video::SColor(100, 0, 0, 0);
	borderColor = video::SColor(255, 0, 0, 0);

	backgroundTexture = 0;

	font = Environment->getSkin()->getFont();
	startIndex = -1;
	lineOffset = 0;
	texture = 0;

	scrollBar = Environment->addScrollBar(false, core::recti(100, 10, 110, 50), this);
	scrollBar->setVisible(showScrollBar);

	update = true;
	temporaryLines = false;

#ifdef _DEBUG
	setDebugName("TextArea");
#endif
}

TextArea::~TextArea() {
	if (backgroundTexture) {
		backgroundTexture->drop();
	}

	clear();
}

bool TextArea::OnEvent(const SEvent& event) {
	// GUI event
	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == gui::EGET_SCROLL_BAR_CHANGED) {
			if (event.GUIEvent.Caller == scrollBar) {
				s32 pos = ((gui::IGUIScrollBar*) event.GUIEvent.Caller)->getPos();
				changeStartIndex(pos);
				return true;
			}
		}
	}
	// Mouse input event
	else if (event.EventType == EET_MOUSE_INPUT_EVENT) {
		core::position2di mousePosition = core::position2di(event.MouseInput.X, event.MouseInput.Y);

		if (getAbsolutePosition().isPointInside(mousePosition)) {
			if (event.MouseInput.Event == EMIE_MOUSE_WHEEL) {
				scrollBar->setPos(scrollBar->getPos() + (s32) event.MouseInput.Wheel * -10);

				changeStartIndex((s32) event.MouseInput.Wheel);
				return true;
			}
		}
	}

	return IGUIElement::OnEvent(event);
}

std::vector<Line*> TextArea::getLines() {
	return lines;
}

std::vector<Line*> TextArea::getLines() const {
	return lines;
}

void TextArea::setLines(std::vector<Line*> lines) {
	clear();
	this->lines = lines;
	update = true;
}

void TextArea::addLine(Line* line) {
	lines.push_back(line);
	update = true;
}

void TextArea::addLines(std::vector<Line*> newLines) {
	lines.insert(lines.begin(), newLines.begin(), newLines.end());
	update = true;
}

u32 TextArea::getNumberOfLines() const {
	return lines.size();
}

void TextArea::setWrapping(bool wrapping) {
	this->wrapping = wrapping;
	update = true;
}

void TextArea::setAutoScroll(bool autoScroll) {
	this->autoScroll = autoScroll;
}

void TextArea::setShowScrollBar(bool showScrollBar) {
	this->showScrollBar = showScrollBar;
	scrollBar->setVisible(showScrollBar);
	update = true;
}

void TextArea::setAlignment(TextArea::Alignment alignment) {
	this->alignment = alignment;
	update = true;
}

void TextArea::setWidth(u32 width) {
	setDimension(width, RelativeRect.getHeight());
}

void TextArea::setHeight(u32 height) {
	setDimension(RelativeRect.getWidth(), height);
}

core::recti TextArea::getDimension() {
	return RelativeRect;
}

void TextArea::setDimension(u32 width, u32 height) {
	setRelativePosition(core::recti(RelativeRect.UpperLeftCorner.X, RelativeRect.UpperLeftCorner.Y, RelativeRect.UpperLeftCorner.X + width, RelativeRect.UpperLeftCorner.Y + height));
	update = true;
}

void TextArea::setDimension(core::recti dimension) {
	setRelativePosition(dimension);
	update = true;
}

core::vector2di TextArea::getPosition() const {
	return RelativeRect.UpperLeftCorner;
}

void TextArea::setPosition(core::vector2di position) {
	setRelativePosition(position);
	update = true;
}

void TextArea::setBorderSize(u32 size) {
	borderSize = size;
	update = true;
}

void TextArea::setPadding(u32 padding) {
	this->padding = padding;
	update = true;
}

void TextArea::setMaxLines(u32 maxLines) {
	this->maxLines = maxLines;

	changeStartIndex(0);
	update = true;
}

void TextArea::setBackgroundColor(video::SColor backgroundColor) {
	this->backgroundColor = backgroundColor;
	update = true;
}

void TextArea::setBorderColor(video::SColor borderColor) {
	this->borderColor = borderColor;
	update = true;
}

void TextArea::setBackgroundTexture(video::ITexture* texture) {
	if (this->backgroundTexture) {
		this->backgroundTexture->drop();
	}
	this->backgroundTexture = texture;

	if (this->backgroundTexture) {
		this->backgroundTexture->grab();
	}
	update = true;
}

void TextArea::setFont(gui::IGUIFont* font) {
	this->font = font;
	update = true;
}

void TextArea::setLineOffset(u32 offset) {
	this->lineOffset = offset;
	update = true;
}

void TextArea::setScrollBarWidth(u32 scrollBarWidth) {
	this->scrollBarWidth = scrollBarWidth;
	update = true;
}

void TextArea::changeStartIndex(s32 increment) {
	if (lines.size() > maxLines || visibleLines.size() > maxLines) {
		startIndex -= increment;

		if (startIndex < 0) {
			startIndex = 0;
		} else if ((u32) startIndex > lines.size() - maxLines) {
			startIndex = lines.size() - maxLines;
		}
	} else {
		startIndex = -1;
	}
	updateVisibleLines(false);
	update = true;
}

void TextArea::clear() {
	// Delete lines
	for (std::vector<Line*>::iterator it = lines.begin(); it != lines.end(); it++) {
		Line* line = *it;
		delete line;
	}
	lines.clear();

	// Delete visible lines
	for (std::vector<Line*>::iterator it = visibleLines.begin(); it != visibleLines.end(); it++) {
		Line* line = *it;
		delete line;
	}
	visibleLines.clear();

	// Delete text batches
	for (std::vector<TextBatch*>::iterator it = textBatches.begin(); it != textBatches.end(); it++) {
		TextBatch* t = *it;
		delete t;
	}
	textBatches.clear();

	// Delete image batches
	for (std::vector<ImageBatch*>::iterator it = imageBatches.begin(); it != imageBatches.end(); it++) {
		ImageBatch* i = *it;
		delete i;
	}
	imageBatches.clear();

	startIndex = -1;

	update = true;
}

void TextArea::draw() {
	if (!isVisible()) {
		return;
	}

	// Update lines if the textarea has a temporary line
	if (temporaryLines) {
		updateLines();
	}

	if (update) {
		updateTextArea();
		update = false;
	}

	video::IVideoDriver* driver = Environment->getVideoDriver();
	if (driver->queryFeature(video::EVDF_RENDER_TO_TARGET)) {
		const video::SColor color = video::SColor(255, 255, 255, 255);
		const core::vector2di upperLeftCorner = RelativeRect.UpperLeftCorner - borderSize;

		driver->draw2DImage(texture, upperLeftCorner, core::recti(0, 0, texture->getOriginalSize().Width, texture->getOriginalSize().Height), 0, color, true);
	} else {
		drawTextArea();
	}

	// Draw scroll bar
	if (showScrollBar) {
		scrollBar->draw();
	}
}

void TextArea::drawTextArea() {
	video::IVideoDriver* driver = Environment->getVideoDriver();

	// Draw background
	const video::SColor color = video::SColor(255, 255, 255, 255);
	const video::SColor colorsA[] = { color, color, color, color };

	core::recti background = RelativeRect;
	// DirectX fix
	if (driver->getDriverType() == video::EDT_DIRECT3D9 || driver->getDriverType() == video::EDT_DIRECT3D8) {
		background.UpperLeftCorner -= 1;
	}

	if (!backgroundTexture) {
		driver->draw2DRectangle(backgroundColor, background);
	} else {
		driver->draw2DImage(backgroundTexture, background, core::recti(0, 0, backgroundTexture->getOriginalSize().Width, backgroundTexture->getOriginalSize().Height), 0, colorsA, true);
	}

	// Draw all text batches
	for (std::vector<TextBatch*>::iterator it = textBatches.begin(); it != textBatches.end(); it++) {
		TextBatch* textBatch = *it;
		textBatch->spriteBank->draw2DSpriteBatch(textBatch->indices, textBatch->positions, 0, textBatch->color);
	}

	// Draw images
	for (std::vector<ImageBatch*>::iterator it = imageBatches.begin(); it != imageBatches.end(); it++) {
		ImageBatch* imageBatch = *it;
		driver->draw2DImageBatch(imageBatch->image, imageBatch->positions, imageBatch->rectangles, 0, color, true);
	}

	// Draw border
	for (u32 i = 0; i < borderSize; i++) {
		core::recti border = RelativeRect;
		border.UpperLeftCorner -= i + 1;
		border.LowerRightCorner += i;

		// DirectX fix
		if (driver->getDriverType() == video::EDT_DIRECT3D9 || driver->getDriverType() == video::EDT_DIRECT3D8) {
			border.UpperLeftCorner += 1;
		}

		driver->draw2DRectangleOutline(border, borderColor);

		// DirectX draw2DRectangleOutline() fix
		if (driver->getDriverType() == video::EDT_DIRECT3D9 || driver->getDriverType() == video::EDT_DIRECT3D8) {
			border.LowerRightCorner = border.UpperLeftCorner;
			border.UpperLeftCorner.X -= 1;
			driver->draw2DLine(border.UpperLeftCorner, border.LowerRightCorner, borderColor);
		}
	}
}

void TextArea::updateTextArea() {
	video::IVideoDriver* driver = Environment->getVideoDriver();
	core::position2di position = RelativeRect.UpperLeftCorner;
	core::recti rect = RelativeRect;

	// Reset position when rendering to target texture
	if (driver->queryFeature(video::EVDF_RENDER_TO_TARGET)) {
		setRelativePosition(core::position2di(borderSize, borderSize));
		rect = RelativeRect;
	}

	if (showScrollBar && scrollBar->isVisible()) {
		RelativeRect.LowerRightCorner.X -= scrollBarWidth;
	}

	// Update visible lines
	updateVisibleLines();

	// Create text & image batches from visible lines
	createBatches();

	// Restore original
	RelativeRect = rect;

	// Update RTT
	updateRenderTargetTexture();

	// Reset position when rendering to target texture
	if (driver->queryFeature(video::EVDF_RENDER_TO_TARGET)) {
		setRelativePosition(position);
	}

	// Update scroll bar
	updateScrollBar();
}

void TextArea::updateRenderTargetTexture() {
	video::IVideoDriver* driver = Environment->getVideoDriver();
	if (driver->queryFeature(video::EVDF_RENDER_TO_TARGET)) {
		core::dimension2du dimension = core::dimension2du(RelativeRect.getWidth() + (borderSize * 2), RelativeRect.getHeight() + (borderSize * 2));
		if (!texture || (texture && texture->getOriginalSize() != dimension)) {
			texture = driver->addRenderTargetTexture(dimension, "RTT", video::ECF_A8R8G8B8);
		}
	}

	// Render to texture
	if (texture) {
		driver->setRenderTarget(texture, true, true, video::SColor(0, 0, 0, 0));
		drawTextArea();
		driver->setRenderTarget(0, false, false, 0);
	}
}

void TextArea::updateScrollBar() {
	core::vector2di scrollBarPosition = core::vector2di(RelativeRect.getWidth() - scrollBarWidth, 0);
	scrollBar->setRelativePosition(core::recti(scrollBarPosition, core::dimension2di(scrollBarWidth, RelativeRect.getHeight())));
}

void TextArea::updateLines() {
	std::vector<Line*>::iterator it = lines.begin();
	while (it != lines.end()) {
		Line* line = *it;

		if (line->isTemporary() && line->isExpired()) {
			it = lines.erase(it);
			delete line;
			update = true;
		} else {
			it++;
		}
	}
}

void TextArea::updateVisibleLines(bool scroll) {
//	scrollBar->setVisible(false);
//	if (visibleLines.size() > maxLines) {
//		scrollBar->setVisible(true);
//	}

// Auto scroll to newest line when autoScroll is enabled
	if (autoScroll && scroll && (lines.size() > maxLines || visibleLines.size() > maxLines)) {
		startIndex = lines.size() - maxLines;
	}

	// Remove current visible lines
	for (std::vector<Line*>::iterator it = visibleLines.begin(); it != visibleLines.end(); it++) {
		Line* line = *it;
		delete line;
	}
	visibleLines.clear();

	core::dimension2d<u32> space = font->getDimension(L" ");

	core::recti lastDimension = core::recti(padding, padding, padding, padding) + RelativeRect.UpperLeftCorner;

	gui::IGUIFont* font;

	temporaryLines = false;

	// Update if temporary lines exist
	for (std::vector<Line*>::iterator it = lines.begin(); it != lines.end(); it++) {
		Line* line = *it;

		if (line->isTemporary() && !line->isExpired()) {
			temporaryLines = true;
		}
	}

	u32 start = 0;
	u32 end = maxLines;
	if (startIndex >= 0) {
		start = startIndex;
		end = startIndex + maxLines;
	}
	if (end > lines.size()) {
		end = lines.size();
	}

	u32 lineHeight = lastDimension.UpperLeftCorner.Y;
	for (u32 l = start; l < end; l++) {
		const Line* line = lines.at(l);
		Line* tempLine = new Line();

		// Get font of line else use textarea default font
		if (line->getFont()) {
			font = line->getFont();
			tempLine->setFont(font);
		} else {
			font = this->font;
		}
		space = font->getDimension(L" ");

		std::vector<core::stringw> strings = line->getStrings();
		std::vector<video::SColor> colors = line->getColors();
		std::vector<video::ITexture*> images = line->getImages();
		u32 currentImageIndex = 0;

		core::dimension2d<u32> stringDimension;

		if (wrapping) {
			for (u32 i = 0; i < strings.size(); i++) {
				std::vector<core::stringw> words;
				strings.at(i).split(words, L" ", 1, true, true);

				for (u32 w = 0; w < words.size(); w++) {
					core::stringw word = words.at(w);

					bool isImage = word.equals_ignore_case(IMAGE_PLACEHOLDER);

					// Image
					if (isImage && images.size() > currentImageIndex) {
						video::ITexture* image = images.at(currentImageIndex);
						lastDimension.LowerRightCorner.X += image->getOriginalSize().Width;
					}
					// Word
					else {
						stringDimension = font->getDimension(word.c_str());
						lastDimension.LowerRightCorner.X += stringDimension.Width;

						//If word is wider than the textarea width, chop word
						if (stringDimension.Width > (u32) RelativeRect.getWidth()) {
							s32 charAmount = (s32) (((f32) RelativeRect.getWidth() / (f32) stringDimension.Width) * (f32) word.size());
							word = word.subString(0, charAmount);
						}
					}

					if (lastDimension.LowerRightCorner > RelativeRect.LowerRightCorner) {
						// Next line
						lastDimension = core::recti(padding, padding, padding, padding) + RelativeRect.UpperLeftCorner;
						lastDimension.UpperLeftCorner.Y = lineHeight;
						lastDimension.LowerRightCorner.X += stringDimension.Width;

						//Create now line
						visibleLines.push_back(tempLine);
						tempLine = new Line();
						tempLine->setFont(line->getFont());

						lineHeight += space.Height + lineOffset;

						// Trim spaces
						word.trim(" ");
					}

					// Add image to temp line
					if (isImage) {
						tempLine->addImage(images.at(currentImageIndex));
						currentImageIndex++;
					}
					// Add string to temp line
					else {
						tempLine->addString(word, colors.at(i));
					}

					lastDimension.UpperLeftCorner.X = lastDimension.LowerRightCorner.X;
				}
			}

			// Next line
			lastDimension = core::recti(padding, padding, padding, padding) + RelativeRect.UpperLeftCorner;
			lastDimension.UpperLeftCorner.Y = lineHeight;
		}
		// No wrapping
		else {
			tempLine->setStrings(strings);
			tempLine->setColors(colors);
			tempLine->setImages(images);
		}

		// Add line to visible lines
		visibleLines.push_back(tempLine);
		lineHeight += space.Height + lineOffset;
	}
}

void TextArea::createBatches() {
	// Remove old text batches
	for (std::vector<TextBatch*>::iterator it = textBatches.begin(); it != textBatches.end(); it++) {
		TextBatch* t = *it;
		delete t;
	}
	textBatches.clear();

	// Remove old image batches
	for (std::vector<ImageBatch*>::iterator it = imageBatches.begin(); it != imageBatches.end(); it++) {
		ImageBatch* i = *it;
		delete i;
	}
	imageBatches.clear();

	core::dimension2d<u32> space = font->getDimension(L" ");

	// Start dimension
	core::recti lastDimension;
	if (alignment == LEFT) {
		lastDimension = core::recti(padding, padding, padding, padding) + RelativeRect.UpperLeftCorner;
	} else if (alignment == RIGHT) {
		lastDimension = core::recti(RelativeRect.getWidth() - padding, padding, RelativeRect.getWidth() - padding, padding) + RelativeRect.UpperLeftCorner;
	} else if (alignment == CENTER) {
		lastDimension = core::recti(padding, padding, padding, padding) + RelativeRect.UpperLeftCorner;
	}

	// Draw visible lines
	u32 start = 0;
	u32 end = maxLines;
	if (visibleLines.size() > maxLines && (startIndex > 0 || startIndex == -1)) {
		start = visibleLines.size() - maxLines;
		end = visibleLines.size();
	}
	if (end > visibleLines.size()) {
		end = visibleLines.size();
	}

	TextBatch* textBatch;
	ImageBatch* imageBatch;
	core::position2di lastPosition = lastDimension.UpperLeftCorner;
	irr::core::stringw word;
	irr::core::stringw character;
	irr::core::stringw prevCharacter;
	gui::IGUIFont* font;
	u32 lineHeight = lastDimension.UpperLeftCorner.Y;
	for (u32 l = start; l < end; l++) {
		Line* line = visibleLines.at(l);

		// Get font of line else use textarea default font
		if (line->getFont()) {
			font = line->getFont();
		} else {
			font = this->font;
		}
		space = font->getDimension(L" ");

		std::vector<core::stringw> strings = line->getStrings();
		std::vector<video::SColor> colors = line->getColors();
		std::vector<video::ITexture*> images = line->getImages();
		u32 currentImageIndex = 0;

		core::dimension2d<u32> stringDimension;

		lastDimension.UpperLeftCorner.Y = lineHeight;
		lastDimension.LowerRightCorner.Y = lastDimension.UpperLeftCorner.Y + space.Height;

		// Text left aligned
		if (alignment == LEFT) {
			for (u32 i = 0; i < strings.size(); i++) {
				// Replace image place holder with image
				if (images.size() > currentImageIndex && strings.at(i).equals_ignore_case(IMAGE_PLACEHOLDER)) {
					video::ITexture* image = images.at(currentImageIndex);
					core::recti imageDimension = lastDimension;
					imageDimension.LowerRightCorner.X = imageDimension.UpperLeftCorner.X + image->getOriginalSize().Width;
					imageDimension.LowerRightCorner.Y = imageDimension.UpperLeftCorner.Y + image->getOriginalSize().Height;

					// Batching image
					imageBatch = getImageBatch(image);
					imageBatch->positions.push_back(imageDimension.UpperLeftCorner);
					imageBatch->rectangles.push_back(core::recti(0, 0, imageBatch->image->getOriginalSize().Width, imageBatch->image->getOriginalSize().Height));

					currentImageIndex++;
					lastDimension.LowerRightCorner.X += image->getOriginalSize().Width;
				}
				// Add plain text to batch
				else {
					stringDimension = font->getDimension(strings.at(i).c_str());
					lastDimension.LowerRightCorner.X += stringDimension.Width;

					// Batching text
					textBatch = getTextBatch(font, colors.at(i));
					lastPosition = lastDimension.UpperLeftCorner;
					word = strings.at(i);

					for (u32 i = 0; i < word.size(); i++) {
						character = word.subString(i, 1);
						textBatch->indices.push_back(((gui::CGUIFont*) font)->getSpriteNoFromChar(character.c_str()));
						textBatch->positions.push_back(lastPosition);

						lastPosition.X += font->getDimension(character.c_str()).Width;
						if (i > 0) {
							lastPosition.X += font->getKerningWidth(character.c_str(), prevCharacter.c_str());
						}
						prevCharacter = character;
					}
				}
				lastDimension.UpperLeftCorner.X = lastDimension.LowerRightCorner.X;
			}

			// Next line
			lastDimension = core::recti(padding, padding, padding, padding) + RelativeRect.UpperLeftCorner;
		}
		// Text right aligned
		else if (alignment == RIGHT) {
			for (u32 i = strings.size() - 1; i + 1 > 0; i--) {
				// Replace image place holder with image
				if (images.size() > currentImageIndex && strings.at(i).equals_ignore_case(IMAGE_PLACEHOLDER)) {
					video::ITexture* image = images.at(currentImageIndex);
					core::recti imageDimension = lastDimension;
					imageDimension.UpperLeftCorner.X = imageDimension.LowerRightCorner.X - image->getOriginalSize().Width;
					imageDimension.LowerRightCorner.Y = imageDimension.UpperLeftCorner.Y + image->getOriginalSize().Height;

					// Batching image
					imageBatch = getImageBatch(image);
					imageBatch->positions.push_back(imageDimension.UpperLeftCorner);
					imageBatch->rectangles.push_back(core::recti(0, 0, imageBatch->image->getOriginalSize().Width, imageBatch->image->getOriginalSize().Height));
					currentImageIndex++;

					lastDimension.UpperLeftCorner.X -= image->getOriginalSize().Width;
				}
				// Add plain text to batch
				else {
					stringDimension = font->getDimension(strings.at(i).c_str());

					lastDimension.UpperLeftCorner.X -= stringDimension.Width;

					// Batching text
					textBatch = getTextBatch(font, colors.at(i));
					lastPosition = lastDimension.UpperLeftCorner;
					word = strings.at(i);

					for (u32 i = 0; i < word.size(); i++) {
						character = word.subString(i, 1);
						textBatch->indices.push_back(((gui::CGUIFont*) font)->getSpriteNoFromChar(character.c_str()));
						textBatch->positions.push_back(lastPosition);

						lastPosition.X += font->getDimension(character.c_str()).Width;
						if (i > 0) {
							lastPosition.X += font->getKerningWidth(character.c_str(), prevCharacter.c_str());
						}
						prevCharacter = character;
					}
				}
				lastDimension.LowerRightCorner.X = lastDimension.UpperLeftCorner.X;
			}

			// Next line
			lastDimension = core::recti(RelativeRect.getWidth() - padding, padding, RelativeRect.getWidth() - padding, padding) + RelativeRect.UpperLeftCorner;
		}
		// Text center aligned
		else if (alignment == CENTER) {
			// Calculate total line length
			core::dimension2d<u32> totalDimension;
			for (u32 i = 0; i < strings.size(); i++) {
				// Calculate image size
				if (images.size() > currentImageIndex && strings.at(i).equals_ignore_case(IMAGE_PLACEHOLDER)) {
					video::ITexture* image = images.at(currentImageIndex);
					totalDimension.Width += image->getOriginalSize().Width;
					currentImageIndex++;
				}
				// Calculate text size
				else {
					stringDimension = font->getDimension(strings.at(i).c_str());
					totalDimension.Width += stringDimension.Width;
					if (totalDimension.Height < stringDimension.Height) {
						totalDimension.Height = stringDimension.Height;
					}
				}
			}

			currentImageIndex = 0;
			lastDimension = core::recti((RelativeRect.getWidth() / 2) - (totalDimension.Width / 2), padding, (RelativeRect.getWidth() / 2) - (totalDimension.Width / 2), padding) + RelativeRect.UpperLeftCorner;
			lastDimension.UpperLeftCorner.Y = lineHeight;

			for (u32 i = 0; i < strings.size(); i++) {
				// Replace image place holder with image
				if (images.size() > currentImageIndex && strings.at(i).equals_ignore_case(IMAGE_PLACEHOLDER)) {
					video::ITexture* image = images.at(currentImageIndex);
					core::recti imageDimension = lastDimension;
					imageDimension.LowerRightCorner.X = imageDimension.UpperLeftCorner.X + image->getOriginalSize().Width;
					imageDimension.LowerRightCorner.Y = imageDimension.UpperLeftCorner.Y + image->getOriginalSize().Height;

					// Batching image
					imageBatch = getImageBatch(image);
					imageBatch->positions.push_back(imageDimension.UpperLeftCorner);
					imageBatch->rectangles.push_back(core::recti(0, 0, imageBatch->image->getOriginalSize().Width, imageBatch->image->getOriginalSize().Height));

					currentImageIndex++;
					lastDimension.LowerRightCorner.X += image->getOriginalSize().Width;
				}
				// Add plain text to batch
				else {
					stringDimension = font->getDimension(strings.at(i).c_str());
					lastDimension.LowerRightCorner.X += stringDimension.Width;

					// Batching text
					textBatch = getTextBatch(font, colors.at(i));
					lastPosition = lastDimension.UpperLeftCorner;
					word = strings.at(i);

					for (u32 i = 0; i < word.size(); i++) {
						character = word.subString(i, 1);
						textBatch->indices.push_back(((gui::CGUIFont*) font)->getSpriteNoFromChar(character.c_str()));
						textBatch->positions.push_back(lastPosition);

						lastPosition.X += font->getDimension(character.c_str()).Width;
						if (i > 0) {
							lastPosition.X += font->getKerningWidth(character.c_str(), prevCharacter.c_str());
						}
						prevCharacter = character;
					}
				}
				lastDimension.UpperLeftCorner.X = lastDimension.LowerRightCorner.X;
			}
		}
		lineHeight += space.Height + lineOffset;
	}
}

TextBatch* TextArea::getTextBatch(gui::IGUIFont* font, video::SColor color) {
	TextBatch* textBatch = 0;

	// Search if text batch with this font already exists
	for (std::vector<TextBatch*>::iterator it = textBatches.begin(); it != textBatches.end(); it++) {
		TextBatch* t = *it;
		if (t->font == font && t->color == color) {
			textBatch = t;
			break;
		}
	}

	// Add text batch to map
	if (!textBatch) {
		textBatch = new TextBatch();
		textBatch->font = font;
		textBatch->spriteBank = ((gui::CGUIFont*) font)->getSpriteBank();
		textBatch->color = color;
		textBatches.push_back(textBatch);
	}

	return textBatch;
}

ImageBatch* TextArea::getImageBatch(video::ITexture* image) {
	ImageBatch* imageBatch = 0;

	// Search if image batch with this font already exists
	for (std::vector<ImageBatch*>::iterator it = imageBatches.begin(); it != imageBatches.end(); it++) {
		ImageBatch* i = *it;
		if (i->image == image) {
			imageBatch = i;
			break;
		}
	}

	// Add image batch to map
	if (!imageBatch) {
		imageBatch = new ImageBatch();
		imageBatch->image = image;
		imageBatches.push_back(imageBatch);
	}

	return imageBatch;
}
