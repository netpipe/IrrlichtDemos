/*
Copyright (c) 2009, Mathias Gottschlag

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef _PEAKGRAPHICS_MENU_MENUELEMENT_HPP_
#define _PEAKGRAPHICS_MENU_MENUELEMENT_HPP_

#include <peakengine/support/ScreenPosition.hpp>
#include <peakengine/support/Mutex.hpp>
#include "peakgraphics/Loadable.hpp"

#include <vector>

namespace lf
{
	namespace gui
	{
		class CGUIWidget;
	}
}

namespace peak
{
	class Graphics;
	class Menu;

	class MenuElement : public Loadable
	{
		public:
			MenuElement(Graphics *graphics, Menu *menu, MenuElement *parent = 0);
			virtual ~MenuElement();

			virtual void queueForLoading();

			virtual bool load();
			virtual bool destroy();

			void setPosition(ScreenPosition position);
			ScreenPosition getPosition();

			void setSize(ScreenSize size);
			ScreenSize getSize();

			virtual void setVisible(bool visible);
			virtual bool isVisible();

			virtual void updateParent();
			virtual void updatePosition();
			virtual void update();

			lf::gui::CGUIWidget *getWidget();
		protected:
			void removeChild(MenuElement *child);

			Graphics *graphics;
			Menu *menu;
			MenuElement *newparent;
			MenuElement *parent;
			std::vector<SharedPointer<MenuElement> > children;

			lf::gui::CGUIWidget *widget;

			ScreenPosition position;
			ScreenPosition size;
			bool visible;

			bool changed;

			Mutex mutex;
	};

	typedef SharedPointer<MenuElement> MenuElementPointer;
}

#endif
