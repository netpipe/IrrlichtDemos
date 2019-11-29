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

#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <string>

#include "core/Vector3D.h"
#include "core/Vector2D.h"
#include "core/Quaternion.h"

//tolua_begin
namespace peak
{
	/**
	 * \brief Dynamically sized buffer for network data etc.
	 */
	class Buffer
	{
		public:
			Buffer();
			Buffer(unsigned int size);
			Buffer(void *data, unsigned int size);
			Buffer(const Buffer &buf);
			~Buffer();

			/**
			 * \brief Sets buffer size.
			 *
			 * If there is more data than the new size, the data is truncated.
			 * \param size New size
			 */
			void setSize(unsigned int size);
			/**
			 * \brief Returns the buffer size.
			 * \return Buffer size
			 */
			unsigned int getSize(void);
			/**
			 * \brief Returns the size of the data stored in the buffer.
			 * \return Data size
			 */
			unsigned int getDataSize(void);

			/**
			 * \brief Returns the current write/read position.
			 * \return Position
			 */
			unsigned int getPosition(void);
			/**
			 * \brief Sets the current write/read position.
			 * \param position New position
			 * \param relative If set, add the current position to the new position
			 */
			void setPosition(unsigned int position, bool relative = false);

			/**
			 * \brief Reads raw data from the current position.
			 * 
			 * Might read less than size bytes if there is not enough data.
			 * \param dest Memory to hold the data. Must be at least size bytes long.
			 * \param size Size to be read
			 * \return Number of bytes read
			 */
			int readData(void *dest, unsigned int size);
			/**
			 * \brief Write data to the buffer.
			 * 
			 * Might write less then size bytes if resize is not set.
			 * \param source Pointer to the data.
			 * \param size Size to be written
			 * \param resize If set, resize the buffer if it is not big enougth to hold the data
			 * \return Number of bytes written
			 */
			int writeData(const void *source, unsigned int size,
					bool resize = true);

			/**
			 * \brief Write integer to the buffer.
			 * \param value Number to be written
			 */
			void writeInt(int value);
			/**
			 * \brief Read integer from the buffer.
			 * \return Value of the integer
			 */
			int readInt(void);
			/**
			 * \brief Write float to the buffer.
			 * \param value Number to be written
			 */
			void writeFloat(float value);
			/**
			 * \brief Read float from the buffer.
			 * \return Value of the float
			 */
			float readFloat(void);
			/**
			 * \brief Write string to the buffer.
			 * \param str String to be written
			 */
			void writeString(std::string str);
			/**
			 * \brief Read string from the buffer.
			 * \return String which was read
			 */
			std::string readString(void);
			/**
			 * \brief Write single byte to the buffer.
			 * \param value Byte to be written
			 */
			void writeByte(unsigned char value);
			/**
			 * \brief Read single byte from the buffer.
			 * \return Value of the byte
			 */
			unsigned char readByte(void);
			/**
			 * \brief Write word (2 bytes) to the buffer.
			 * \param value Word to be written
			 */
			void writeWord(short value);
			/**
			 * \brief Read word (2 bytes) from the buffer.
			 * \return Value of the word
			 */
			short readWord(void);
			/**
			 * \brief Write a bool variable to the buffer.
			 * \param flag bool variable to be written.
			 */
			void writeBool(bool flag);
			/**
			 * \brief Read a bool variable from the buffer.
			 * \return Value of the bool variable.
			 */
			bool readBool(void);
			/**
			 * \brief Write 2-dimensional vector to the buffer.
			 * \param v Vector to be written
			 */
			void writeVector2D(Vector2D v);
			/**
			 * \brief Read 2-dimensional vector from the buffer.
			 * \return Value of the vector.
			 */
			Vector2D readVector2D(void);
			/**
			 * \brief Write vector to the buffer.
			 * \param v Vector to be written
			 */
			void writeVector3D(Vector3D v);
			/**
			 * \brief Read vector from the buffer.
			 * \return Value of the vector
			 */
			Vector3D readVector3D(void);
			/**
			 * \brief Write quaternion to the buffer.
			 * \param q Quaternion to be written
			 */
			void writeQuaternion(Quaternion q);
			/**
			 * \brief Read quaternion from the buffer.
			 * \return Value of the quaternion
			 */
			Quaternion readQuaternion(void);

			/**
			 * \brief Returns pointer to the raw buffer data.
			 * \return Pointer to buffer data
			 */
			char *getData(void);

			/**
			 * \brief Clear the buffer and free the data.
			 */
			void clear(void);

			//tolua_end
			Buffer &operator=(const Buffer &buf);
			Buffer &operator+=(const Buffer &buf);
			Buffer &operator<<(const Buffer &buf);
			Buffer &operator<<(int &data);
			Buffer &operator<<(float &data);
		private:
			char *data;
			unsigned int size;
			unsigned int datasize;
			unsigned int position;
			//tolua_begin
	};
}
//tolua_end

#endif
