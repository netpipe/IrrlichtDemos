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

#ifndef _FILE_H_
#define _FILE_H_

#include "core/FileSystem.h"

//tolua_begin
namespace peak
{
	class Buffer;
	
	/**
	 * \brief Class for file access
	 */
	class File
	{
		public:
			File(std::string filename, EFileMode mode, bool binary = false);
			virtual ~File();
			
			/**
			 * \brief Reads some data into allocated memory.
			 * \param buffer Memory to read into
			 * \param length maximal data length to be read
			 * \return Number of bytes read
			 */
			virtual int read(char *buffer, int length);
			/**
			 * \brief Reads some data into a Buffer.
			 * \param buffer Buffer to read into
			 * \param length maximal data length to be read
			 * \return Number of bytes read
			 */
			virtual int read(Buffer *buffer, int length);
			/**
			 * \brief Reads a line and returns it as a string.
			 * \return Line which was read
			 */
			virtual std::string readLine(void);
			/**
			 * \brief Write some data from allocated memory.
			 * \param buffer Memory to read data from
			 * \param length maximal data length to be written
			 * \return Number of bytes written
			 */
			virtual int write(const char *buffer, int length);
			/**
			 * \brief Write some data from a Buffer.
			 * \param buffer Buffer to read data from
			 * \param length maximal data length to be written
			 * \return Number of bytes written
			 */
			virtual int write(Buffer *buffer, int length);
			/**
			 * \brief Returns whether the end of the file was reached.
			 */
			virtual bool eof(void);
			
			/**
			 * \brief Returns the current read/write position.
			 * \return Position
			 */
			virtual int getPosition(void);
			/**
			 * \brief Sets the current read/write position.
			 * \param pos New position
			 * \param end If set to true, the position is set relative to the
			 * end of the file.
			 * \return Position (can differ from the desired position)
			 */
			virtual int setPosition(int pos, bool end = false);
			virtual int movePosition(int pos);
		private:
			//tolua_end
			FILE *file;
			//tolua_begin
	};
}
//tolua_end

#endif

