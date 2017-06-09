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

#ifndef ELEMENTITEM_H
#define ELEMENTITEM_H


class ElementIrrlicht;
class GameSceneIrrlicht;

/**
 * @brief This class is the graphical representation of a game Element.
 */
class ElementItemIrrlicht
{


private:

    /** The instance of Element the ElementItem will represent */
    ElementIrrlicht* m_model;

public:
    /**
     * The Scene where all elements are drawn on
     */
    static GameSceneIrrlicht* g_scene;

    /**
      * Creates a new ElementItem instance.
      * @param p_model the Element model
      * @param renderer the KGameRenderer
      */
    ElementItemIrrlicht(ElementIrrlicht* p_model);

    /**
      * Deletes the ElementItem instance.
      */
    ~ElementItemIrrlicht();

    /**
      * Gets the Element model.
      * @return the model
      */
    ElementIrrlicht* getModel() const;
    void setModel(ElementIrrlicht* model);

    /**
     *  inits the modell
     */
    virtual void init()=0;
    virtual void setVisible(bool yorn);
    virtual void update();

};

#endif

