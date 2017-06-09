/*
 * Copyright 2011
 * Copyright 2009 Mathias Kraus <k.hias@gmx.de>
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

#ifndef XYElement_H
#define XYElement_H

#include "irrlicht.h"

#include "ElementIrrlicht.h"

class ArenaIrrlicht;


/**
 * @brief This class describes the common XYElementistics and behaviour of the game XYElements (Players).
 */
class XYElement: public ElementIrrlicht {

protected:

	/** The Element current x-coordinate */
	irr::f32 m_x;

	/** The Element current y-coordinate */
	irr::f32 m_y;

	/** The Element current z-coordinate */
	irr::f32 m_z;

	/** The XYElement x-speed */
	irr::f32 m_xSpeed;

	/** The XYElement y-speed */
	irr::f32 m_ySpeed;

	/** The XYElement speed */
	irr::f32 m_speed;

	ArenaIrrlicht* m_arena;

public:

	/**
	 * Creates a new XYElement instance.
	 * @param p_x the initial x-coordinate
	 * @param p_y the initial y-coordinate
	 * @param p_arena the Arena the XYElement is on
	 */
	XYElement(irr::f32 p_x, irr::f32 p_y, ArenaIrrlicht* p_arena);

	XYElement();

	/**
	 * Deletes the XYElement instance.
	 */
	~XYElement();

	void updateMove();

	/**
	 * Moves the XYElement function of its current coordinates and speed.
	 * If the XYElement reaches a border, it circles around the arena and continue its way from the other side.
	 */
	void move();

	/**
	 * Gets the XYElement x-speed value.
	 * @return the x-speed value
	 */
	irr::f32 getXSpeed() const;

	/**
	 * Gets the XYElement y-speed value.
	 * @return the y-speed value
	 */
	irr::f32 getYSpeed() const;

	/**
	 * Set the XYElement x-speed value.
	 * @param p_xSpeed the x-speed to set
	 */
	void setXSpeed(irr::f32 p_xSpeed);

	/**
	 * Set the XYElement y-speed value.
	 * @param p_ySpeed the y-speed to set
	 */
	void setYSpeed(irr::f32 p_ySpeed);

	irr::f32 getX() const;
	void setX(irr::f32 x);
	irr::f32 getY() const;
	void setY(irr::f32 y);
	irr::f32 getZ() const;
	void setZ(irr::f32 x);

};

#endif

