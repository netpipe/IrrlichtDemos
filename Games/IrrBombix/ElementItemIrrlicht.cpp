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
#include "ElementItemIrrlicht.h"
#include "ElementIrrlicht.h"
#include "GameItemIrrlicht.h"
#include <iostream>

GameSceneIrrlicht* ElementItemIrrlicht::g_scene = 0;

ElementItemIrrlicht::ElementItemIrrlicht(ElementIrrlicht* p_model)
{
    m_model = p_model;
}

ElementItemIrrlicht::~ElementItemIrrlicht()
{
    delete m_model;

}

ElementIrrlicht* ElementItemIrrlicht::getModel() const
{
    return m_model;
}

void ElementItemIrrlicht::setModel(ElementIrrlicht* model){
	m_model = model;
}

void ElementItemIrrlicht::init(){
 std::cout << "init in ElementItemIrrlicht!!";
}

void ElementItemIrrlicht::setVisible(bool yorn){
 std::cout << "setVisible in ElementItemIrrlicht!!";
}

void ElementItemIrrlicht::update(){
 std::cout << "update in ElementItemIrrlicht!!";
}
