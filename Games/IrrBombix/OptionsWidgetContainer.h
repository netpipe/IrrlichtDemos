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

#ifndef OPTIONSWIDGETCONTAINER_H
#define OPTIONSWIDGETCONTAINER_H

#include <string>
#include <irrlicht.h>
class OptionsWidget;

class OptionsWidgetContainer {
public:
	OptionsWidgetContainer();
	virtual ~OptionsWidgetContainer();
	OptionsWidget* getSelectedOptionsWidget();
	int getSelectedPos();
	void addoptionsWidget(OptionsWidget* op);
	OptionsWidget* getOptionsWidget(std::string s);
	OptionsWidget* getOptionsWidget(int p);
	int size();
	void selectnext();
	void selectprev();
	void clear();
protected:
private:
	int pos;
	irr::core::array<OptionsWidget*> optionsWidgets;

};

#endif // OPTIONSWIDGETCONTAINER_H
