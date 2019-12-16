
/*
Copyright (C) 2008   Mathias Gottschlag

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

#ifndef _PLUGIN_H_
#define _PLUGIN_H_

#include <string>
#include <vector>
#if defined(_MSC_VER) || defined(_WINDOWS_) || defined(_WIN32)
#include <windows.h>
#endif

//tolua_begin
namespace peak
{
	class Plugin
	{
		public:
			Plugin();
			~Plugin();
			
			bool load(std::string path);
			bool unload(void);
			
			/**
			 * \brief Loads a plugin into the engine.
			 *
			 * Note: .so and .dll are appended to the path automatically!
			 * \param path Relative path to the shared library
			 */
			static bool addPlugin(std::string path);
			static bool unloadAll(void);
			//tolua_end
		private:
			#ifdef __unix__
			void *library;
			#endif
			#if defined(_MSC_VER) || defined(_WINDOWS_) || defined(_WIN32)
			HMODULE library;
			#endif
			static std::vector<Plugin*> plugins;
			//tolua_begin
	};
}
//tolua_end

#endif

