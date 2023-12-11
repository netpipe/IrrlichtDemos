#include "consolemgr.h"

void initTmt(ConsoleMgr* mgr, TMT* tmt) {
	if (mgr) {
		mgr->terminal = tmt;
	}
}
bool initFont(ConsoleMgr* mgr, const char* fontName, int fontSize) {
	if (mgr) {
		mgr->font = TTF_OpenFont(fontName, fontSize);
		mgr->charactH = 0;
		mgr->charactW = 0;
		return mgr->font != nullptr;
	}
	return false;
}

void setFontStyle(ConsoleMgr* mgr, int renderstyle) {
	if (mgr && mgr->font) {
		TTF_SetFontStyle(mgr->font, renderstyle);
	}
}

void setConsoleRect(ConsoleMgr* mgr, SDL_Rect rect)
{
	if (mgr)
		mgr->consoleRect = rect;
}

void setScreenRect(ConsoleMgr* mgr, SDL_Rect rect)
{
	if (mgr)
		mgr->screenRect = rect;
}

void closeFont(ConsoleMgr* mgr)
{
	if (mgr->font) TTF_CloseFont(mgr->font);
}

SDL_Surface* createConsoleSurface(int width, int height) {
	Uint32 rmask, gmask, bmask, amask;

	/* SDL interprets each pixel as a 32-bit number, so our masks must depend
	on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	return SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32,
		rmask, gmask, bmask, amask);
}

SDL_Color convertTmtColorTo(tmt_color_t color) {
	switch (color)
	{
	case TMT_COLOR_DEFAULT:
		return { 0x00, 0x00, 0x00, 0x00 };
		break;
	case TMT_COLOR_BLACK:
		return { 0x00, 0x00, 0x00, 0 };
		break;
	case TMT_COLOR_RED:
		return { 0xFF, 0x00, 0x00, 0 };
		break;
	case TMT_COLOR_GREEN:
		return { 0x00, 0xFF, 0x00, 0 };
		break;
	case TMT_COLOR_YELLOW:
		return { 0xFF, 0xFF, 0x00, 0 };
		break;
	case TMT_COLOR_BLUE:
		return { 0x00, 0x00, 0xFF, 0 };
		break;
	case TMT_COLOR_MAGENTA:
		return { 0xFF, 0x00, 0xFF, 0 };
		break;
	case TMT_COLOR_CYAN:
		return { 0x00, 0xFF, 0xFF, 0 };
		break;
	case TMT_COLOR_WHITE:
		return { 0xFF, 0xFF, 0xFF, 0 };
		break;
	case TMT_COLOR_MAX:
		return { 0xFF, 0xFF, 0xFF, 0 };
		break;
	}

}

SDL_Surface* renderConsole(ConsoleMgr* mgr)
{
	if (mgr && mgr->surface) {
		SDL_FillRect(mgr->surface, NULL, SDL_MapRGB(mgr->surface->format, 0xFF, 0xFF, 0xFF));

		const TMTSCREEN* s = tmt_screen(mgr->terminal);
		const TMTPOINT* cur = tmt_cursor(mgr->terminal);

		TTF_Font* font = mgr->font;
		if(!mgr->charactH) mgr->charactH = TTF_FontHeight(font);
		int current_x = 0;
		int current_y = 0;
		SDL_Rect rect;
		for (size_t r = 0; r < s->nline; r++) {
			//if (s->lines[r]->dirty)
			{
				for (size_t c = 0; c < s->ncol; c++) {
					TMTATTRS attr = s->lines[r]->chars[c].a;
					int renderstyle = TTF_STYLE_NORMAL;
					if (attr.bold)
						renderstyle |= TTF_STYLE_BOLD;
					if (attr.underline)
						renderstyle |= TTF_STYLE_UNDERLINE;
					char str[2] = { s->lines[r]->chars[c].c & 0xFF,0 };
					setFontStyle(mgr, renderstyle);
					
					SDL_Surface* text = TTF_RenderText_Shaded(font, str, convertTmtColorTo(!(attr.fg==TMT_COLOR_DEFAULT || attr.fg == 0) ? attr.fg : TMT_COLOR_BLACK), convertTmtColorTo(!(attr.bg == TMT_COLOR_DEFAULT || attr.bg == 0) ? attr.bg : TMT_COLOR_WHITE));
					rect.x = current_x;
					rect.y = current_y;
					rect.w = text->w;
					rect.h = text->h;
					SDL_BlitSurface(text, &text->clip_rect, mgr->surface, &rect);
					if (!mgr->charactW) 
						mgr->charactW = text->pitch;
					current_x += mgr->charactW;
					SDL_FreeSurface(text);
				}
			}
			current_y += mgr->charactH;
			current_x = 0;
		}
		return mgr->surface;
	}
	return NULL;	
}

void renderCursor(ConsoleMgr* mgr) {
	if (mgr&&mgr->surface) {
		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - mgr->time);
		if (time_span.count() > 0.7) {
			const TMTPOINT* cur = tmt_cursor(mgr->terminal);
			SDL_Rect rect;
			mgr->cursor = !mgr->cursor;
			mgr->time = std::chrono::high_resolution_clock::now();
			SDL_Surface* text;
			if(mgr->cursor)
				text = TTF_RenderText_Shaded(mgr->font, "|", convertTmtColorTo(TMT_COLOR_BLACK), convertTmtColorTo(TMT_COLOR_WHITE));
			else
				text = TTF_RenderText_Shaded(mgr->font, "|", convertTmtColorTo(TMT_COLOR_WHITE), convertTmtColorTo(TMT_COLOR_WHITE));
			rect.x = cur->c * mgr->charactW;
			rect.y = cur->r * mgr->charactH;
			rect.w = text->w;
			rect.h = text->h;
			SDL_BlitSurface(text, &text->clip_rect, mgr->surface, &rect);
			SDL_FreeSurface(text);
		}
	}
}

