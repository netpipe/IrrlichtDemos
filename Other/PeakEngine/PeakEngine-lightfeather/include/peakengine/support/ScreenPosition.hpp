/*
Copyright (c) 2009, Mathias Gottschlag

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef _PEAKENGINE_SUPPORT_SCREENPOSITION_HPP_
#define _PEAKENGINE_SUPPORT_SCREENPOSITION_HPP_

#include "Vector2.hpp"

namespace peak
{
	/**
	 * Class to hold the position on the screen for menu or overlay layouts.
	 * Positions are stored in two parts: A relative part (with 2 components
	 * going from 0 to 1 where 0/0 is the top left and 1/1 the bottom right
	 * screen corner) and an absolute part in pixels which is added onto the
	 * relative position.
	 */
	class ScreenPosition
	{
		public:
			/**
			 * Constructor.
			 */
			ScreenPosition()
			{
			}
			/**
			 * Constructor.
			 */
			ScreenPosition(const Vector2F &relative,
				const Vector2I &absolute = Vector2I(0, 0))
			{
				rel = relative;
				abs = absolute;
			}
			/**
			 * Destructor.
			 */
			~ScreenPosition()
			{
			}

			ScreenPosition operator+(const ScreenPosition &sp) const
			{
				return ScreenPosition(rel + sp.rel, abs + sp.abs);
			}
			ScreenPosition operator-(const ScreenPosition &sp) const
			{
				return ScreenPosition(rel - sp.rel, abs - sp.abs);
			}
			ScreenPosition &operator=(const ScreenPosition &sp)
			{
				rel = sp.rel;
				abs = sp.abs;
				return *this;
			}
			ScreenPosition &operator+=(const ScreenPosition &sp)
			{
				rel += sp.rel;
				abs += sp.abs;
				return *this;
			}
			ScreenPosition &operator-=(const ScreenPosition &sp)
			{
				rel -= sp.rel;
				abs -= sp.abs;
				return *this;
			}

			/**
			 * Computes the absolute screen position based on the current
			 * screen size.
			 */
			Vector2I getAbsolute(const Vector2I &screensize) const
			{
				return abs + Vector2I(rel.x * screensize.x,
					rel.y * screensize.y);
			}

			/**
			 * Relative part of the position (1.0 means screen width/height).
			 */
			Vector2F rel;
			/**
			 * Absolute part of the position in pixels.
			 */
			Vector2I abs;
	};

	typedef ScreenPosition ScreenSize;
}

#endif
