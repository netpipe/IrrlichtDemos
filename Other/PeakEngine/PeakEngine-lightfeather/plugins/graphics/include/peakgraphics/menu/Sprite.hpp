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

#ifndef _PEAKGRAPHICS_MENU_SPRITE_HPP_
#define _PEAKGRAPHICS_MENU_SPRITE_HPP_

#include "../Loadable.hpp"
#include <peakengine/support/ScreenPosition.hpp>
#include <peakengine/support/Mutex.hpp>

namespace lf
{
	namespace gui
	{
		class CSprite;
	}
}

namespace peak
{
	class SpriteLayer;

	class Sprite : public Loadable
	{
		public:
			Sprite(SpriteLayer *layer, const std::string &texture,
				const ScreenPosition &pos, const ScreenPosition &size,
				const Vector2I &texturepos = Vector2I(0, 0),
				const Vector2I &texturesize = Vector2I(0, 0));
			virtual ~Sprite();

			virtual bool load();
			virtual bool destroy();

			void setPosition(const ScreenPosition &position);
			ScreenPosition getPosition();
			void setSize(const ScreenSize &size);
			ScreenSize getSize();
			void setTexturePosition(const Vector2I &position);
			Vector2I getTexturePosition();
			void setTextureSize(const Vector2I &size);
			Vector2I getTextureSize();

			void update();
			static void updateAll();

		private:
			SpriteLayer *layer;
			std::string texturefile;
			ScreenPosition position;
			ScreenPosition size;
			Vector2I texturepos;
			Vector2I texturesize;
			bool changed;

			lf::gui::CSprite *sprite;

			Sprite *next;
			Sprite *prev;
			static Mutex mutex;
			static Sprite *first;
	};

	typedef SharedPointer<Sprite> SpritePointer;
}

#endif
