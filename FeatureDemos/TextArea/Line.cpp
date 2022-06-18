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

#include "Line.h"

#include <stdio.h>
#include <time.h>

Line::Line(irr::u32 lifeTime) {
	this->lifeTime = lifeTime;
	createTime = 0;
	if (lifeTime > 0) {
		createTime = time(NULL);
	}
	font = 0;
}

Line::Line(irr::gui::IGUIFont* font, irr::u32 lifeTime) {
	this->font = font;
	this->lifeTime = lifeTime;
	createTime = 0;
	if (lifeTime > 0) {
		createTime = time(NULL);
	}
}

Line::~Line() {
	strings.clear();
	colors.clear();

	for (std::vector<irr::video::ITexture*>::iterator it = images.begin(); it != images.end(); it++) {
		(*it)->drop();
	}
	images.clear();
}

std::vector<irr::core::stringw> Line::getStrings() const {
	return strings;
}

void Line::setStrings(std::vector<irr::core::stringw> strings) {
	this->strings = strings;
}

std::vector<irr::video::SColor> Line::getColors() const {
	return colors;
}

void Line::setColors(std::vector<irr::video::SColor> colors) {
	this->colors = colors;
}

irr::u32 Line::getLifetime() const {
	return lifeTime;
}

void Line::setLifetime(irr::u32 lifeTime) {
	this->lifeTime = lifeTime;
	if (lifeTime > 0) {
		createTime = time(NULL);
	}
}

irr::u32 Line::getCreateTime() const {
	return createTime;
}

bool Line::isTemporary() {
	return (lifeTime > 0);
}

bool Line::isExpired() {
	irr::u32 now = time(NULL);
	return (createTime + lifeTime < now);
}

std::vector<irr::video::ITexture*> Line::getImages() const {
	return images;
}

void Line::setImages(std::vector<irr::video::ITexture*> images) {
	this->images = images;
	for (std::vector<irr::video::ITexture*>::iterator it = this->images.begin(); it != this->images.end(); it++) {
		(*it)->grab();
	}
}

void Line::addString(irr::core::stringw string, irr::video::SColor color) {
	strings.push_back(string);
	colors.push_back(color);
}

void Line::addImage(irr::video::ITexture* image) {
	strings.push_back(IMAGE_PLACEHOLDER);
	colors.push_back(irr::video::SColor(255, 255, 255, 255));
	image->grab();
	images.push_back(image);
}

irr::gui::IGUIFont* Line::getFont() const {
	return font;
}

void Line::setFont(irr::gui::IGUIFont* font) {
	this->font = font;
}
