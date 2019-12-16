
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

#include "core/Plugin.h"
#include "core/Logger.h"
#include "core/FileSystem.h"

#ifdef __unix__
#include <dlfcn.h>
#endif

namespace peak
{
	Plugin::Plugin()
	{
		#ifdef __unix__
		library = 0;
		#endif
		#if defined(_MSC_VER) || defined(_WINDOWS_) || defined(_WIN32)
		library = 0;
		#endif
	}
	Plugin::~Plugin()
	{
		#ifdef __unix__
		if (library) unload();
		#endif
		#if defined(_MSC_VER) || defined(_WINDOWS_) || defined(_WIN32)
		if (library) unload();
		#endif
	}
	
	bool Plugin::load(std::string path)
	{
		#ifdef __unix__
		// Open shared library
		library = dlopen(FileSystem::get()->getFileExists(path + ".so").c_str(), RTLD_LAZY | RTLD_LOCAL);
		if (!library)
		{
			LERROR("Could not open plugin (%s).\n", (path + ".so").c_str());
			return false;
		}
		
		// Call entry function
		bool (*initplugin)(void);
		initplugin = (bool(*)(void))dlsym(library, "initPlugin");
		if (!initplugin)
		{
			LWARNING("No entry function (%s).\n", (path + ".so").c_str());
			dlclose(library);
			library = 0;
			return false;
		}
		else
		{
			if (!initplugin())
			{
				dlclose(library);
				library = 0;
				return false;
			}
		}
		#endif
		#if defined(_MSC_VER) || defined(_WINDOWS_) || defined(_WIN32)
		// Open DLL
		library = LoadLibrary(FileSystem::get()->getFileExists(path + ".dll").c_str());
		if (!library)
		{
			LERROR("Could not open plugin (%s).\n", (path + ".dll").c_str());
			return false;
		}
		
		// Call entry function
		bool (*initplugin)(void);
		initplugin = (bool(*)(void))GetProcAddress(library, "initPlugin");
		if (!initplugin)
		{
			LWARNING("No entry function (%s).\n", (path + ".dll").c_str());
			FreeLibrary(library);
			library = 0;
			return false;
		}
		else
		{
			if (!initplugin())
			{
				FreeLibrary(library);
				library = 0;
				return false;
			}
		}
		#endif
		return true;
	}
	bool Plugin::unload(void)
	{
		#ifdef __unix__
		if (!library) return true;
		// Call unload function
		bool (*unloadplugin)(void);
		unloadplugin = (bool(*)(void))dlsym(library, "unloadPlugin");
		if (!unloadplugin)
		{
			LWARNING("No unload function.\n");
		}
		else
		{
			unloadplugin();
		}
		// Close library
		dlclose(library);
		library = 0;
		#endif
		#if defined(_MSC_VER) || defined(_WINDOWS_) || defined(_WIN32)
		if (!library) return true;
		// Call unload function
		bool (*unloadplugin)(void);
		unloadplugin = (bool(*)(void))GetProcAddress(library, "unloadPlugin");
		if (!unloadplugin)
		{
			LWARNING("No unload function.\n");
		}
		else
		{
			unloadplugin();
		}
		// Close library
		FreeLibrary(library);
		library = 0;
		#endif
		return true;
	}
	
	bool Plugin::addPlugin(std::string path)
	{
		Plugin *plugin = new Plugin;
		if (!plugin->load(path))
		{
			delete plugin;
			return false;
		}
		plugins.push_back(plugin);
		return true;
	}
	bool Plugin::unloadAll(void)
	{
		// Unload plugins
		for (unsigned int i = 0; i < plugins.size(); i++)
		{
			plugins[i]->unload();
			delete plugins[i];
		}
		return true;
	}
	
	std::vector<Plugin*> Plugin::plugins;
}

