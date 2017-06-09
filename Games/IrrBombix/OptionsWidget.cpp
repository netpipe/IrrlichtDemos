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

#include "OptionsWidget.h"
#include <iostream>


OptionsWidget::OptionsWidget(std::string s) {
	pos = -2;
	name = s;
	//ctor
}

OptionsWidget::~OptionsWidget() {
	//dtor
}

void OptionsWidget::clear(){
	options.clear();
}

std::string OptionsWidget::getValue() {
	if (pos == -2) {
		std::cout << "OptionsWidget getValue on empty options\n";
	}
	return options[pos];
}

void OptionsWidget::addoptions(std::string op) {
	if (pos == -2) {
		pos = 0;
	}
	options.push_back(op);
}
void OptionsWidget::selectnext() {
	pos = (pos + 1) % options.size();
}

void OptionsWidget::selectprev() {
	//pos = (pos - 1) % options.size();
	pos = (pos - 1);
	if  (pos ==-1){
		pos = options.size()-1;
	}
}

std::string OptionsWidget::getName(){
	return name;
}

void OptionsWidget::setName(std::string s){
	name = s;
}
