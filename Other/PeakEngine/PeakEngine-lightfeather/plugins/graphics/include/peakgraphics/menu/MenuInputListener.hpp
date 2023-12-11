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

#ifndef _PEAKGRAPHICS_MENU_MENUINPUTLISTENER_HPP_
#define _PEAKGRAPHICS_MENU_MENUINPUTLISTENER_HPP_

#include "Menu.hpp"
#include "MenuElement.hpp"
#include <peakengine/support/ScopedLock.hpp>

#include <lf/Lightfeather.h>
#include <map>

namespace peak
{
	class MenuInputListener : public lf::gui::IGUIActionListener,
		public lf::gui::IGUISelectionListener
	{
		public:
			MenuInputListener(Menu *menu) : menu(menu)
			{
			}

			void registerInput(MenuElement *element)
			{
				ScopedLock lock(mutex);
				elements.insert(std::pair<lf::gui::CGUIWidget*, MenuElement*>(
					element->getWidget(), element));
			}
			void deregisterInput(MenuElement *element)
			{
				ScopedLock lock(mutex);
				std::map<lf::gui::CGUIWidget*, MenuElement*>::iterator it;
				it = elements.find(element->getWidget());
				if (it == elements.end())
					return;
				elements.erase(it);
			}
			MenuElement *getElement(lf::gui::CGUIWidget *widget)
			{
				ScopedLock lock(mutex);
				std::map<lf::gui::CGUIWidget*, MenuElement*>::iterator it;
				it = elements.find(widget);
				if (it == elements.end())
					return 0;
				return it->second;
			}

			virtual void performAction(lf::gui::CGUIActionEvent &event)
			{
				MenuElement *element = getElement(event.getSource());
				if (!element)
					return;
				menu->onAction(element);
			}
			void selectionChanged(lf::gui::CGUISelectionEvent &event)
			{
			}
		private:
			Menu *menu;
			std::map<lf::gui::CGUIWidget*, MenuElement*> elements;
			Mutex mutex;
	};
}

#endif
