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

#include "OptionsWidgetContainer.h"
#include "OptionsWidget.h"
#include <string>

OptionsWidgetContainer::OptionsWidgetContainer() {
	//ctor
	pos = -2;
}

OptionsWidgetContainer::~OptionsWidgetContainer() {
	//dtor
}

void OptionsWidgetContainer::clear(){
	optionsWidgets.clear();
}

OptionsWidget* OptionsWidgetContainer::getSelectedOptionsWidget() {
	return optionsWidgets[pos];
}
void OptionsWidgetContainer::addoptionsWidget(OptionsWidget* op){
	if (pos = -2){
		pos = 0;
	}
	optionsWidgets.push_back(op);
}
OptionsWidget* OptionsWidgetContainer::getOptionsWidget(std::string s) {
	for (int i = 0; i < optionsWidgets.size(); i++) {
		if (s == optionsWidgets[i]->getName()) {
			return optionsWidgets[i];
		}
	}
}

OptionsWidget* OptionsWidgetContainer::getOptionsWidget(int p) {
			return optionsWidgets[p];
}

int OptionsWidgetContainer::size() {
	return optionsWidgets.size();
}

int OptionsWidgetContainer::getSelectedPos(){
	return pos;
}

void OptionsWidgetContainer::selectnext() {
	pos = (pos + 1) % optionsWidgets.size();
}

void OptionsWidgetContainer::selectprev() {
	if (pos == 0){
		pos =optionsWidgets.size()-1;
	}else{
		pos = (pos - 1) % optionsWidgets.size();
	}
}
