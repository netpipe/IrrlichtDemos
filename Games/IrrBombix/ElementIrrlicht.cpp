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

#include "ElementIrrlicht.h"
#include "ElementItemIrrlicht.h"
#include <iostream>

ElementIrrlicht::ElementIrrlicht(ElementItemIrrlicht* view) {
		this->view=view;
}

ElementIrrlicht::ElementIrrlicht() {
	view=0;
}

ElementIrrlicht::~ElementIrrlicht() {
	// TODO Auto-generated destructor stub
}


ElementItemIrrlicht* ElementIrrlicht::getView()
  {

	//std::cout << "this is " << this <<"\n";
	//std::cout << "view is " << view <<"\n";
	if (view ==0){
		std::cout << "null view??";
	}
      return view;
  }

  void ElementIrrlicht::setView(ElementItemIrrlicht *view)
  {
	  //std::cout << "view set to " << view <<"\n";
	  //std::cout << "this is " << this <<"\n";
      this->view = view;
  }
