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

#ifndef OPTIONSWIDGET_H
#define OPTIONSWIDGET_H
#include "irrlicht.h"
#include <string>


class OptionsWidget
{
    public:
        OptionsWidget(std::string s);
        virtual ~OptionsWidget();
        std::string getValue();
        void addoptions(std::string op);
        void selectnext();
        void selectprev();
        std::string getName();
        void setName(std::string s);
        void clear();
    protected:
    std::string name;
    int pos;
    irr::core::array<std::string> options;
};

#endif // OPTIONSWIDGET_H
