/*
Copyright (C) 2008  Mathias Gottschlag, Lukas Kropatschek

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

#include "core/Util.h"

#include <cstring>
#include <cstdlib>
#if defined(_MSC_VER) || defined(_WINDOWS_) || defined(_WIN32)
#include <cstdio>
#define snprintf _snprintf
#endif

namespace peak
{
	int trimString(std::string *str)
	{
		// Remove whitespaces in front of the string
		char *tmpstr = strdup(str->c_str());
		unsigned int whitespaces = strspn(tmpstr, " \t");
		if (whitespaces == strlen(tmpstr))
		{
			*str = "";
			return strlen(tmpstr);
		}
		for (unsigned int i = strlen(tmpstr) - 1; i > 0; i--)
		{
			if ((tmpstr[i] == ' ') || (tmpstr[i] == '\t'))
			{
				tmpstr[i] = 0;
			}
			else
			{
				break;
			}
		}
		unsigned int deleted = str->size() - strlen(tmpstr);
		*str = tmpstr + whitespaces;
		free(tmpstr);
		return deleted;
	}
	bool splitString(std::string str, char c, std::string *str1, std::string *str2)
	{
		if (str.find(c) == std::string::npos) return false;
		*str1 = str.substr(0, str.find(c));
		*str2 = str.substr(str.find(c) + 1, str.length() - str.find(c) - 1);
		return true;
	}
	
	bool splitString(std::string str, std::string split, std::string *str1, std::string *str2)
	{
		if (str.find_first_of(split) == std::string::npos) return false;
		*str1 = str.substr(0, str.find_first_of(split));
		*str2 = str.substr(str.find_first_of(split) + 1, str.length() - str.find_first_of(split) - 1);
		return true;
	}
	
	float toFloat(std::string str)
	{
		char *end = (char *)str.c_str() + str.length();
		return strtod(str.c_str(), &end);
	}
	int toInt(std::string str)
	{
		char *end = (char *)str.c_str() + str.length();
		return strtol(str.c_str(), &end, 10);
	}
	
	Vector3D toVector3D(std::string str)
	{
		trimString(&str);
		std::string xstr;
		std::string ystr;
		std::string zstr;
		std::string yzstr;
		splitString(str, "/,", &xstr, &yzstr);
		splitString(yzstr, "/,", &ystr, &zstr);
		float x = toFloat(xstr);
		float y = toFloat(ystr);
		float z = toFloat(zstr);
		Vector3D ret(x,y,z);
		return ret;
	}
	
	Vector2D toVector2D(std::string str)
	{	
		trimString(&str);
		std::string xstr;
		std::string ystr;
		splitString(str, "/,x", &xstr, &ystr);
		float x = toFloat(xstr);
		float y = toFloat(ystr);
		Vector2D ret(x,y);
		return ret;
	}
	
	Vector2DI toVector2DI(std::string str)
	{	
		trimString(&str);
		std::string xstr;
		std::string ystr;
		splitString(str, "/,x", &xstr, &ystr);
		int x = toInt(xstr);
		int y = toInt(ystr);
		Vector2DI ret(x,y);
		return ret;
	}
	
	bool toBool(std::string str)
	{
		trimString(&str);
		if(str == "true" || str == "1" || str == "yes")
			return true;
		
		if(str == "false" || str == "0" || str == "no")
			return false;
		else return false;
	}
	
	std::string toString(int v)
	{
		char tmp[30];
		snprintf(tmp, 30, "%d", v);
		return tmp;
	}
	std::string toString(float v)
	{
		char tmp[30];
		snprintf(tmp, 30, "%f", v);
		return tmp;
	}
	std::string toString(Vector3D v)
	{
		char tmp[30];
		snprintf(tmp, 30, "%f/%f/%f", v.x, v.y, v.z);
		return tmp;
	}
	std::string toString(Vector2D v)
	{
		char tmp[30];
		snprintf(tmp, 30, "%f/%f", v.x, v.y);
		return tmp;
	}
	std::string toString(Vector2DI v)
	{
		char tmp[30];
		snprintf(tmp, 30, "%d/%d", v.x, v.y);
		return tmp;
	}
	
	std::wstring toWString(std::string s)
	{
		const char *mbs = s.c_str();
		wchar_t *wchar = new wchar_t[s.size() + 1];
		mbstowcs(wchar, mbs, s.size());
		wchar[s.size()] = 0;
		std::wstring ws;
		ws = wchar;
		delete[] wchar;
		return ws;
	}
	
	std::string trimFilename(std::string filename)
	{
		if (filename.find_last_of("/\\") == std::string::npos) return filename;
		return filename.substr(filename.find_last_of("/\\") + 1, filename.length() - filename.find_last_of("/\\") - 1);
	}
	std::string getExtension(std::string filename)
	{
		if (filename.find_last_of(".") == std::string::npos) return filename;
		return filename.substr(filename.find_last_of(".") + 1, filename.length() - filename.find_last_of(".") - 1);
	}
}

