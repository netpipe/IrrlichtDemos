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

#include "peakgraphics/menu/Sprite.hpp"
#include "peakgraphics/menu/SpriteLayer.hpp"
#include "peakgraphics/Graphics.hpp"
#include <peakengine/support/ScopedLock.hpp>

#include <lf/Lightfeather.h>
using namespace lf;

namespace peak
{
	Sprite::Sprite(SpriteLayer *layer,const std::string &texture,
		const ScreenPosition &pos, const ScreenPosition &size,
		const Vector2I &texturepos, const Vector2I &texturesize) : layer(layer),
		texturefile(texture), position(pos), size(size), texturepos(texturepos),
		texturesize(texturesize), changed(false), sprite(0), next(0), prev(0)
	{
		// Load sprite
		Graphics *graphics = layer->getGraphics();
		graphics->registerLoading(this);
	}
	Sprite::~Sprite()
	{
		ScopedLock lock(mutex);
		if (next || prev || (first == this))
		{
			if (next)
				next->prev = prev;
			if (prev)
				prev->next = next;
			if (first == this)
				first = next;
		}
	}

	bool Sprite::load()
	{
		// Load texture
		CResourceManager *resmgr = CResourceManager::getInstancePtr();
		res::CTexture *texture = resmgr->loadTexture(texturefile.c_str(),
			texturefile.c_str());
		if (!texture)
			return false;
		// Get absolute position/size in pixels
		Graphics *graphics = layer->getGraphics();
		Vector2I screensize = graphics->getWindowSize();
		Vector2I abspos = position.getAbsolute(screensize);
		Vector2I abssize = size.getAbsolute(screensize);
		// Get the correct size
		if (texturesize == Vector2I(0, 0))
		{
			core::vector2di texsize = texture->getSize();
			this->texturesize = texturesize = Vector2I(texsize.X, texsize.Y);
			
		}
		// Create sprite
		gui::CGUIManager *guimgr = layer->getGUIManager();
		sprite = guimgr->drawImage(texture, core::vector2di(abspos.x, abspos.y),
			core::vector2di(abssize.x, abssize.y),
			core::vector2di(texturepos.x, texturepos.y),
			core::vector2di(texturesize.x, texturesize.y),
			gui::ETT_TEXTURE_ALPHA);
		if (!sprite)
			return false;
		// Add the sprite to the sprite list
		ScopedLock lock(mutex);
		if (first)
			first->prev = this;
		next = first;
		prev = 0;
		first = this;
		return true;
	}
	bool Sprite::destroy()
	{
		if (!sprite)
			return false;
		layer->getGUIManager()->removeImage(sprite);
		sprite = 0;
		return true;
	}

	void Sprite::setPosition(const ScreenPosition &position)
	{
		this->position = position;
		changed = true;
	}
	ScreenPosition Sprite::getPosition()
	{
		return position;
	}
	void Sprite::setSize(const ScreenSize &size)
	{
		this->size = size;
		changed = true;
	}
	ScreenSize Sprite::getSize()
	{
		return size;
	}
	void Sprite::setTexturePosition(const Vector2I &position)
	{
		texturepos = position;
		changed = true;
	}
	Vector2I Sprite::getTexturePosition()
	{
		return texturepos;
	}
	void Sprite::setTextureSize(const Vector2I &size)
	{
		texturesize = size;
		changed = true;
	}
	Vector2I Sprite::getTextureSize()
	{
		return texturesize;
	}

	void Sprite::update()
	{
		if (changed && sprite)
		{
			// Get absolute position/size in pixels
			Graphics *graphics = layer->getGraphics();
			Vector2I screensize = graphics->getWindowSize();
			Vector2I abspos = position.getAbsolute(screensize);
			Vector2I abssize = size.getAbsolute(screensize);
			// Update position and size
			sprite->setPosition(core::vector2di(abspos.x, abspos.y));
			sprite->setSize(core::vector2di(abssize.x, abssize.y));
			sprite->setClipPosition(core::vector2di(texturepos.x,
				texturepos.y));
			sprite->setClipSize(core::vector2di(texturesize.x,
				texturesize.y));
			changed = false;
		}
	}
	void Sprite::updateAll()
	{
		ScopedLock lock(mutex);
		Sprite *sprite = first;
		while (sprite)
		{
			sprite->update();
			sprite = sprite->next;
		}
	}

	Mutex Sprite::mutex;
	Sprite *Sprite::first = 0;
}
