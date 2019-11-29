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

#ifndef _LUASCRIPT_H_
#define _LUASCRIPT_H_

#include <string>
#include <list>

extern "C"
{
#include "lua.h"
}

struct lua_State;

//tolua_begin
namespace peak
{
	//tolua_end
	class UserClass
	{
		public:
			template<class T> UserClass(std::string name, T *value)
			{
				this->name = name;
				this->value = (void*)value;
			}
			std::string name;
			void *value;
	};
	//tolua_begin
	class Parameter
	{
		public:
			Parameter(double value);
			Parameter(std::string name, void *ptr);
			
			bool isPointer(void);
			double getValue(void);
			std::string getClassName(void);
			void *getPointer(void);
			//tolua_end
		private:
			bool isptr;
			void *ptr;
			std::string classname;
			double value;
			//tolua_begin
	};
	
	class ParameterList
	{
		public:
			ParameterList();
			~ParameterList();
			
			void add(Parameter &parameter);
			//tolua_end
			std::list<Parameter> &getParameterList(void);
		private:
			std::list<Parameter> parameters;
			//tolua_begin
	};
	
	/**
	 * \brief Class capsulating a Lua script.
	 */
	class Script
	{
		public:
			Script();
			~Script();
			
			/**
			 * \brief Runs a lua script file
			 *
			 * The functions of the file will be available via callFunction* afterwards.
			 * \param filename Path to the script file
			 * \return Returns false if the script could not be run.
			 */
			bool run(std::string filename);
			
			void callFunction(std::string name);
			void callFunction(std::string name, ParameterList &parameters);
			void callFunctionI(std::string name, int arg1);
			void callFunctionII(std::string name, int arg1, int arg2);
			void callFunctionIII(std::string name, int arg1, int arg2, int arg3);
			void callFunctionFFF(std::string name, float arg1, float arg2, float arg3);
			int callIntFunction(std::string name);
			float callFloatFunction(std::string name);
			bool isFunction(std::string name);
			
			//tolua_end
			// Functions without return value
			template <typename A> void callFunction(std::string name, A arg1)
			{
				lua_getglobal(state, name.c_str());
				push(arg1);
				lua_call(state, 1, 0);
			};
			template <typename A1, typename A2> void callFunction(std::string name, A1 arg1, A2 arg2)
			{
				lua_getglobal(state, name.c_str());
				push(arg1);
				push(arg2);
				lua_call(state, 2, 0);
			};
			template <typename A1, typename A2, typename A3> void callFunction(std::string name, A1 arg1, A2 arg2, A3 arg3)
			{
				lua_getglobal(state, name.c_str());
				push(arg1);
				push(arg2);
				push(arg3);
				lua_call(state, 3, 0);
			};
			
			// Functions with return value
			template <typename R> R callFunction(std::string name)
			{
				lua_getglobal(state, name.c_str());
				lua_call(state, 0, 1);
				R value;
				pop(value);
				return value;
			};
			template <typename R> R callFunction(std::string name, ParameterList &parameters)
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
				lua_call(state, params.size(), 1);
				R value;
				pop(value);
				return value;
			}
			template <typename R, typename A> R callFunction(std::string name, A arg1)
			{
				lua_getglobal(state, name.c_str());
				push(arg1);
				lua_call(state, 1, 1);
				R value;
				pop(value);
				return value;
			};
			template <typename R, typename A1, typename A2> R callFunction(std::string name, A1 arg1, A2 arg2)
			{
				lua_getglobal(state, name.c_str());
				push(arg1);
				push(arg2);
				lua_call(state, 2, 1);
				R value;
				pop(value);
				return value;
			};
			template <typename R, typename A1, typename A2, typename A3> R callFunction(std::string name, A1 arg1, A2 arg2, A3 arg3)
			{
				lua_getglobal(state, name.c_str());
				push(arg1);
				push(arg2);
				push(arg3);
				lua_call(state, 3, 1);
				R value;
				pop(value);
				return value;
			};
			
		private:
			void push(int value);
			void push(float value);
			void push(double value);
			void push(std::string value);
			void push(void *value);
			void push(UserClass value);
			
			void pop(int &value);
			void pop(float &value);
			void pop(double &value);
			void pop(std::string &value);
			
			lua_State *state;
			//tolua_begin
	};
}
//tolua_end

#endif

