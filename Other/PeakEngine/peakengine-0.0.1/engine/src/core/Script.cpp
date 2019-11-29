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

#include "core/Script.h"
#include "core/Logger.h"

extern "C"
{
#include "lualib.h"
#include "lauxlib.h"
#include "tolua++.h"
}

int tolua_peak_open(lua_State* L);

namespace peak
{
	Parameter::Parameter(double value)
	{
		isptr = false;
		this->value = value;
	}
	Parameter::Parameter(std::string name, void *ptr)
	{
		isptr = true;
		classname = name;
		this->ptr = ptr;
	}

	bool Parameter::isPointer(void)
	{
		return isptr;
	}
	double Parameter::getValue(void)
	{
		return value;
	}
	std::string Parameter::getClassName(void)
	{
		return classname;
	}
	void *Parameter::getPointer(void)
	{
		return ptr;
	}

	ParameterList::ParameterList()
	{
	}
	ParameterList::~ParameterList()
	{
	}

	void ParameterList::add(Parameter &parameter)
	{
		parameters.push_back(parameter);
	}

	std::list<Parameter> &ParameterList::getParameterList(void)
	{
		return parameters;
	}

	Script::Script()
	{
		state = lua_open();
		luaopen_base(state);
		luaopen_string(state);
		luaopen_math(state);
		tolua_peak_open(state);
	}
	Script::~Script()
	{
		lua_close(state);
	}

	bool Script::run(std::string filename)
	{
		int error = luaL_dofile(state, filename.c_str());
		if (error)
		{
			LERROR("Error while executing %s:\n", filename.c_str());
			LERROR("%s\n", lua_tostring(state, -1));
			return false;
		}
		return true;
	}

	void Script::callFunction(std::string name)
	{
		lua_getglobal(state, name.c_str());
		lua_call(state, 0, 0);
	}
	void Script::callFunction(std::string name, ParameterList &parameters)
	{
		lua_getglobal(state, name.c_str());
		std::list<Parameter> params = parameters.getParameterList();
		std::list<Parameter>::iterator it = params.begin();
		while (it != params.end())
		{
			if ((*it).isPointer())
			{
				push(UserClass((*it).getClassName(), (*it).getPointer()));
			}
			else
			{
				push((*it).getValue());
			}
			it++;
		}
		lua_call(state, params.size(), 0);
	}

	void Script::callFunctionI(std::string name, int arg1)
	{
		callFunction(name, arg1);
	}
	void Script::callFunctionII(std::string name, int arg1, int arg2)
	{
		callFunction(name, arg1, arg2);
	}
	void Script::callFunctionIII(std::string name, int arg1, int arg2, int arg3)
	{
		callFunction(name, arg1, arg2, arg3);
	}
	void Script::callFunctionFFF(std::string name, float arg1, float arg2, float arg3)
	{
		callFunction(name, arg1, arg2, arg3);
	}
	int Script::callIntFunction(std::string name)
	{
		return callFunction<int>(name);
	}
	float Script::callFloatFunction(std::string name)
	{
		return callFunction<float>(name);
	}

	bool Script::isFunction(std::string name)
	{
		lua_getglobal(state, name.c_str());
		bool exists = lua_isfunction(state, -1);
		lua_pop(state, 1);
		return exists;
	}

	void Script::push(int value)
	{
		lua_pushinteger(state, value);
	}
	void Script::push(float value)
	{
		lua_pushnumber(state, value);
	}
	void Script::push(double value)
	{
		lua_pushnumber(state, value);
	}
	void Script::push(std::string value)
	{
		lua_pushstring(state, value.c_str());
	}
	void Script::push(void *value)
	{
	}
	void Script::push(UserClass value)
	{
		tolua_pushusertype(state, value.value, value.name.c_str());
	}

	void Script::pop(int &value)
	{
		value = (int)lua_tointeger(state, -1);
		lua_pop(state, 1);
	}
	void Script::pop(float &value)
	{
		value = (float)lua_tonumber(state, -1);
		lua_pop(state, 1);
	}
	void Script::pop(double &value)
	{
		value = (double)lua_tonumber(state, -1);
		lua_pop(state, 1);
	}
	void Script::pop(std::string &value)
	{
		// TODO: Pop string.
	}
}

