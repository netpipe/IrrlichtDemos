/*
 * Copyright 2011
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OptionsWidgetObject_H
#define OptionsWidgetObject_H
#include "irrlicht.h"
#include <iostream>
#include <string>
#include  "OptionsWidget.h"

template<class T>
class OptionsWidgetObject: public OptionsWidget {
public:
	OptionsWidgetObject(std::string s) :
		OptionsWidget(s) {
	}
	;
	virtual ~OptionsWidgetObject() {
		clear();
	}
	;
	void addoptions(std::string op, T obj, std::string* ic) {
		// std::cout << "OptionsWidgetObject add" << op << " obj: " << obj << "ic: "<< ic->c_str()<< "\n";
		OptionsWidget::addoptions(op);
		objects.push_back(obj);
		icon.push_back(ic);
	}
	;

	T getObject() {
		if (pos == -2) {
			std::cout << "OptionsWidget getValue on empty options\n";
			return 0;
		}
		return objects[pos];
	}
	;
	void clear() {
		OptionsWidget::clear();
		objects.clear();
	}
	;

	std::string* getIcon() {
		if (pos == -2) {
			std::cout << "OptionsWidget getValue on empty options\n";
			return 0;
		}
		return icon[pos];
	}

protected:
	irr::core::array<T> objects;
	irr::core::array<std::string*> icon;
};

#endif // OptionsWidgetObject_H
