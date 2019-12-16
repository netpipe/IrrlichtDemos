/*
Copyright (C) 2008  Mathias Gottschlag

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef _LISTMODEL_H_
#define _LISTMODEL_H_

#include <guichan.hpp>

namespace peak
{
	/**
	 * \brief List data used for ListBox and ComboBox
	 */
	class ListModel : public gcn::ListModel
	{
		public:
			ListModel()
			{
			}
			virtual ~ListModel()
			{
			}

			/**
			 * \brief Returns the number of entries in the list
			 */
			virtual int getNumberOfElements()
			{
				return elements.size();
			}
			/**
			 * \brief Returns the entry at the given index
			 */
			virtual std::string getElementAt(int i)
			{
				return elements[i];
			}

			/**
			 * \brief Adds an entry to the list
			 */
			virtual void addElement(std::string element)
			{
				elements.push_back(element);
			}
			/**
			 * \brief Deletes the entry at the given index
			 */
			virtual void deleteElementAt(int i)
			{
				elements.erase(elements.begin() + i);
			}
			/**
			 * \brief Clears the list
			 */
			virtual void clear(void)
			{
				elements.clear();
			}
		private:
			std::vector<std::string> elements;
	};
}

#endif

