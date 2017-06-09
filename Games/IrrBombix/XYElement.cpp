/*
 * Copyright 2011
 * Copyright 2007-2008 Thomas Gallinari <tg8187@yahoo.fr>
 * Copyright 2007-2008 Pierre-Benoît Besse <besse.pb@gmail.com>
 * 
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


#include "XYElement.h"
#include "irrlicht.h"


#include "ElementItemIrrlicht.h"

/**
 * Creates a new XYElement instance.
 * @param p_x the initial x-coordinate
 * @param p_y the initial y-coordinate
 * @param p_arena the Arena the XYElement is on
 */
XYElement::XYElement(irr::f32 p_x, irr::f32 p_y, ArenaIrrlicht* p_arena){
	m_x=p_x;
	m_y=p_y;
	m_arena=p_arena;
}

XYElement::XYElement(){
};

/**
 * Deletes the XYElement instance.
 */
XYElement::~XYElement(){};

void XYElement::updateMove(){};

/**
 * Moves the XYElement function of its current coordinates and speed.
 * If the XYElement reaches a border, it circles around the arena and continue its way from the other side.
 */
void XYElement::move(){
	return;
};

/**
 * Gets the XYElement x-speed value.
 * @return the x-speed value
 */
irr::f32 XYElement::getXSpeed() const{
	return m_xSpeed;
};

/**
 * Gets the XYElement y-speed value.
 * @return the y-speed value
 */
irr::f32 XYElement::getYSpeed() const{
	return m_ySpeed;
};

/**
 * Set the XYElement x-speed value.
 * @param p_xSpeed the x-speed to set
 */
void XYElement::setXSpeed(irr::f32 p_xSpeed){
	m_xSpeed=p_xSpeed;
};

/**
 * Set the XYElement y-speed value.
 * @param p_ySpeed the y-speed to set
 */
void XYElement::setYSpeed(irr::f32 p_ySpeed){
	m_ySpeed=p_ySpeed;
};

irr::f32 XYElement::getX() const{
	return m_x;
};
void XYElement::setX(irr::f32 x){
	m_x=x;
};

irr::f32 XYElement::getY() const{
	return m_y;
};
void XYElement::setY(irr::f32 y){
	m_y=y;
};

irr::f32 XYElement::getZ() const{
	return m_z;
};
void XYElement::setZ(irr::f32 y){
	m_z=y;
};





