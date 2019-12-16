/*
Copyright (C) 2008   Lukas Kropatschek, Mathias Gottschlag

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

#ifndef _COLOR_H_
#define _COLOR_H_

//tolua_begin
namespace peak
{
	/**
	 * \brief Class representing a 32bit color
	 */
	class Color
	{
		public:
			Color();
			/**
			 * \brief Create a instance of the Color class
			 * \param color the color 
			 */
			Color(unsigned int color);
			
			/**
			 * \brief Create a instance of the Color class
			 * \param r red
			 * \param g green
			 * \param b blue
			 * \param a alpha (default 255)
			 */
			Color(unsigned char r, unsigned char g,
				  unsigned char b, unsigned char a = 255);
			
			/**
			 * \brief Returns the red component of the color.
			 * \return red component of the color between 0 - 255
			 */
			unsigned char getRed();
			/**
			 * \brief Returns the green component of the color.
			 * \return green component of the color between 0 - 255
			 */
			unsigned char getGreen();
			/**
			 * \brief Returns the blue component of the color.
			 * \return blue component of the color between 0 - 255
			 */
			unsigned char getBlue();
			/**
			 * \brief Returns the alpha component of the color. Alpha specifies how transparent the color is.
			 * \return alpha component of the color between 0 - 255.
			 */
			unsigned char getAlpha();
			
			/**
			 * \brief Sets the red component of the color.
			 * \param red red component
			 */
			void setRed(unsigned char red);
			/**
			 * \brief Sets the green component of the color.
			 * \param green green component
			 */			
			void setGreen(unsigned char green);
			/**
			 * \brief Sets the blue component of the color.
			 * \param blue blue component
			 */
			void setBlue(unsigned char blue);
			/**
			 * \brief Sets the alpha component of the color.
			 * \param alpha alpha component
			 */
			void setAlpha(unsigned char alpha);

			/**
			 * \brief Creates a color from it's ARGB values
			 * \param r red
			 * \param g green
			 * \param b blue
			 * \param a alpha (default 255)
			 */
			void set(unsigned char r, unsigned char g,
					 unsigned char b, unsigned char a = 255);
			/**
			 * \brief Creates a color from it's ARGB values
			 * \param color the color
			 */
			void set(unsigned int color);
			
			/**
			 * \brief Returns the color
			 */
			unsigned int get(void);
			//tolua_end
			unsigned char color[4];
			//tolua_begin
	};

}
//tolua_end

#endif /*COLOR_H_*/
