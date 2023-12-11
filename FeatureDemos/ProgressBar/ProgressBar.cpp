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

#include "ProgressBar.h"

using namespace irr;

ProgressBar::ProgressBar(gui::IGUIEnvironment* environment, s32 id) :
		gui::IGUIElement(gui::EGUIET_ELEMENT, environment, environment->getRootGUIElement(), id, core::rect<s32>(10, 10, 110, 30)) {
	percentage = 100;
	direction = HORIZONTAL;

	borderSize = 1;

	fillColor = video::SColor(255, 0, 255, 255);
	backgroundColor = video::SColor(255, 255, 255, 255);
	borderColor = video::SColor(255, 0, 0, 0);
	textColor = video::SColor(255, 255, 255, 0);

	fillTexture = 0;
	backgroundTexture = 0;

	font = environment->getSkin()->getFont();
	showText = true;
	texture = 0;
	update = true;
}

ProgressBar::~ProgressBar() {
	if (fillTexture) {
		fillTexture->drop();
	}
	if (backgroundTexture) {
		backgroundTexture->drop();
	}
	if (texture) {
		texture->drop();
	}
}

u32 ProgressBar::getPercentage() const {
	return percentage;
}

void ProgressBar::setPercentage(const u32 percentage) {
	if (this->percentage != percentage) {
		this->percentage = core::clamp(percentage, (u32) 0, (u32) 100);
		update = true;
	}
}

ProgressBar::Direction ProgressBar::getDirection() {
	return direction;
}

void ProgressBar::setDirection(Direction direction) {
	this->direction = direction;
	update = true;
}

void ProgressBar::setWidth(const u32 width) {
	setDimension(width, RelativeRect.getHeight());
}

void ProgressBar::setHeight(const u32 height) {
	setDimension(RelativeRect.getWidth(), height);
}

core::recti ProgressBar::getDimension() {
	return RelativeRect;
}

void ProgressBar::setDimension(const u32 width, const u32 height) {
	setRelativePosition(core::recti(RelativeRect.UpperLeftCorner.X, RelativeRect.UpperLeftCorner.Y, RelativeRect.UpperLeftCorner.X + width, RelativeRect.UpperLeftCorner.Y + height));
	update = true;
}

void ProgressBar::setDimension(core::recti dimension) {
	setRelativePosition(dimension);
	update = true;
}

core::vector2di ProgressBar::getPosition() const {
	return RelativeRect.UpperLeftCorner;
}

void ProgressBar::setPosition(core::vector2di position) {
	setRelativePosition(position);
	update = true;
}

void ProgressBar::setBorderSize(u32 size) {
	if (this->borderSize != size) {
		borderSize = size;
		update = true;
	}
}

video::SColor ProgressBar::getFillColor() {
	return fillColor;
}

void ProgressBar::setFillColor(video::SColor fillColor) {
	if (this->fillColor != fillColor) {
		this->fillColor = fillColor;
		update = true;
	}
}

void ProgressBar::setBackgroundColor(video::SColor backgroundColor) {
	if (this->backgroundColor != backgroundColor) {
		this->backgroundColor = backgroundColor;
		update = true;
	}
}

void ProgressBar::setBorderColor(video::SColor borderColor) {
	if (this->borderColor != borderColor) {
		this->borderColor = borderColor;
		update = true;
	}
}

void ProgressBar::setTextColor(video::SColor textColor) {
	if (this->textColor != textColor) {
		this->textColor = textColor;
		update = true;
	}
}

void ProgressBar::setColors(video::SColor fillColor, video::SColor backgroundColor, video::SColor borderColor, video::SColor textColor) {
	this->fillColor = fillColor;
	this->backgroundColor = backgroundColor;
	this->borderColor = borderColor;
	this->textColor = textColor;
	update = true;
}

void ProgressBar::setFillTexture(video::ITexture* texture) {
	if (this->fillTexture) {
		this->fillTexture->drop();
	}
	this->fillTexture = texture;

	if (this->fillTexture) {
		this->fillTexture->grab();
	}
	update = true;
}

void ProgressBar::setBackgroundTexture(video::ITexture* texture) {
	if (this->backgroundTexture) {
		this->backgroundTexture->drop();
	}
	this->backgroundTexture = texture;

	if (this->backgroundTexture) {
		this->backgroundTexture->grab();
	}
	update = true;
}

bool ProgressBar::isShowText() {
	return showText;
}

void ProgressBar::setShowText(bool show) {
	this->showText = show;
	update = true;
}

void ProgressBar::setFont(gui::IGUIFont* font) {
	this->font = font;
	update = true;
}

void ProgressBar::draw() {
	if (!isVisible()) {
		return;
	}

	if (update) {
		updateProgressBar();
		update = false;
	}

	video::IVideoDriver* driver = Environment->getVideoDriver();
	if (driver->queryFeature(video::EVDF_RENDER_TO_TARGET)) {
		const video::SColor color = video::SColor(255, 255, 255, 255);
		const core::vector2di upperLeftCorner = RelativeRect.UpperLeftCorner - borderSize;

		driver->draw2DImage(texture, upperLeftCorner, core::recti(0, 0, texture->getOriginalSize().Width, texture->getOriginalSize().Height), 0, color, true);
	} else {
		drawProgressBar();
	}
}

void ProgressBar::drawProgressBar() {
	video::IVideoDriver* driver = Environment->getVideoDriver();
	core::recti background = RelativeRect;
	core::recti fill = RelativeRect;

	if (direction == HORIZONTAL) {
		const s32 fillX = ((RelativeRect.getWidth() * percentage) / 100) + RelativeRect.UpperLeftCorner.X;
		fill.LowerRightCorner.X = fillX;
	} else {
		const s32 fillY = (RelativeRect.getHeight() - ((RelativeRect.getHeight() * percentage) / 100)) + RelativeRect.UpperLeftCorner.Y;
		fill.UpperLeftCorner.Y = fillY;
	}

	// DirectX fix
	if (driver->getDriverType() == video::EDT_DIRECT3D9 || driver->getDriverType() == video::EDT_DIRECT3D8) {
		background.UpperLeftCorner -= 1;
		fill.UpperLeftCorner -= 1;
	}

	const video::SColor color = video::SColor(255, 255, 255, 255);
	const video::SColor colors[] = { color, color, color, color };

	if (!backgroundTexture) {
		driver->draw2DRectangle(backgroundColor, background);
	} else {
		driver->draw2DImage(backgroundTexture, background, RelativeRect - RelativeRect.UpperLeftCorner, 0, colors, true);
	}
	if (!fillTexture) {
		driver->draw2DRectangle(fillColor, fill);
	} else {
		driver->draw2DImage(fillTexture, fill, RelativeRect - RelativeRect.UpperLeftCorner, 0, colors, true);
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

	if (showText) {
		core::stringw text = L"";
		text += percentage;
		text += " %";
		font->draw(text.c_str(), RelativeRect, textColor, true, true);
	}
}

void ProgressBar::updateProgressBar() {
	video::IVideoDriver* driver = Environment->getVideoDriver();
	core::position2di position = RelativeRect.UpperLeftCorner;

	// Reset position when rendering to target texture
	if (driver->queryFeature(video::EVDF_RENDER_TO_TARGET)) {
		setRelativePosition(core::position2di(borderSize, borderSize));
	}

	// Update RTT
	updateRenderTargetTexture();

	// Reset position when rendering to target texture
	if (driver->queryFeature(video::EVDF_RENDER_TO_TARGET)) {
		setRelativePosition(position);
	}
}

void ProgressBar::updateRenderTargetTexture() {
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
		drawProgressBar();
		driver->setRenderTarget(0, false, false, 0);
	}
}
