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

#ifndef _UTIL_H_
#define _UTIL_H_

#include <string>

#include "core/Vector3D.h"
#include "core/Vector2D.h"
#include "core/Vector2DI.h"

//tolua_begin
namespace peak
{
	/**
	 * \brief Removes all whitespaces from the beginning and the end of the specified string.
	 * \param str Pointer to the string
	 * \return Number of deleted characters
	 */
	int trimString(std::string *str);
	/**
	 * \brief Splits a string in two.
	 *
	 * The function searches for the character c and puts the string before into
	 * str1 and the string afterwards into str2.
	 *
	 * Example:
	 * \code
	 * splitString("abcdefgh", 'd', &str1, &str2);
	 * \endcode
	 * This would create the strings "abc" and "efgh", not including the 'd'.
	 * \param str String to be splitted
	 * \param c Character to look for
	 * \param str1 Pointer to the string variable for the first part
	 * \param str2 Pointer to the string variable for the second part
	 * \return Returns false if c could not be found.
	 */
	bool splitString(std::string str, char c, std::string *str1, std::string *str2);
	/**
	 * \brief Splits a string in two.
	 *
	 * The function searches for the characters in split and puts the string
	 * before into str1 and the string afterwards into str2.
	 * \param str String to be splitted
	 * \param split String holding the characters to look for
	 * \param str1 Pointer to the string variable for the first part
	 * \param str2 Pointer to the string variable for the second part
	 * \return Returns false if no character in split could be found.
	 */
	bool splitString(std::string str, std::string split, std::string *str1, std::string *str2);
	
	/**
	 * \brief Converts a string to a float.
	 * \param str String to be converted
	 * \return Value of the extracted number
	 */
	float toFloat(std::string str);
	/**
	 * \brief Converts a string to a int.
	 * \param str String to be converted
	 * \return Value of the extracted number
	 */
	int toInt(std::string str);

	/**
	 * \brief Converts a string to a Vector3D
	 * \param str String to be converted.
	 * \return string as Vector3D
	 */
	Vector3D toVector3D(std::string str);

	/**
	 * \brief Converts a string to a Vector2D
	 * \param str String to be converted.
	 * \return string as Vector2D
	 */
	Vector2D toVector2D(std::string str);
	
	/**
	 * \brief Converts a string to a Vector2DI
	 * \param str String to be converted.
	 * \return string as Vector2DI
	 */
	Vector2DI toVector2DI(std::string str);
	
	/**
	 * \brief Converts a string to a bool.
	 * \param str String to be converted
	 * \return string as bool.
	 */	
	bool toBool(std::string str);
	
	/**
	 * \brief Creates a string from an int.
	 * \param v Value of the number
	 * \return Created string
	 */
	std::string toString(int v);
	/**
	 * \brief Creates a string from a float.
	 * \param v Value of the number
	 * \return Created string
	 */
	std::string toString(float v);
	/**
	 * \brief Creates a string from a Vector3D.
	 * \param v Value of the number
	 * \return Created string
	 */
	std::string toString(Vector3D v);
	/**
	 * \brief Creates a string from a Vector2D.
	 * \param v Value of the number
	 * \return Created string
	 */
	std::string toString(Vector2D v);
	/**
	 * \brief Creates a string from a Vector2DI.
	 * \param v Value of the number
	 * \return Created string
	 */
	std::string toString(Vector2DI v);
	
	/**
	 * \brief Converts a string to a wide string.
	 */
	std::wstring toWString(std::string s);
	
	/**
	 * \brief Strips the directory from a file name
	 */
	std::string trimFilename(std::string filename);
	/**
	 * \brief Returns the extension of a file name
	 */
	std::string getExtension(std::string filename);
}
//tolua_end

#endif

