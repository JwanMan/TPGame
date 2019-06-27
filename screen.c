/*
 * screen.c -- Abstracción sobre SDL para la entrada y salida gráfica.
 * Copyright (C) 2018 Juan Marín Noguera
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <malloc.h>
#include <SDL2/SDL.h>
#include <string.h>
#include "error.h"
#include "screen.h"

#define TEXT_X 3
#define TEXT_Y 460
#define TEXT_FILE "56929.bmp"
#define TEXT_H 8
#define TEXT_W 8
#define TEXT_PER_ROW 16

struct Picture {
	SDL_Surface *s;
	SDL_Surface *f;
};

struct Screen {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface *surface;
};

Picture screen_text_bmp = NULL;

static void screen_init()
{
	if (!SDL_WasInit(SDL_INIT_VIDEO) && SDL_Init(SDL_INIT_VIDEO) < 0)
		error_sdl_exit();
}

Screen screen_create(const char *caption, int w, int h)
{
	Screen scr;

	if (w <= 0 || h <= 0)
		error_exit(ERR_OUT_OF_RANGE);

	scr = malloc(sizeof(struct Screen));
	if (scr == NULL)
		error_libc_exit();

	screen_init();

	scr->window = SDL_CreateWindow(
		caption,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		w,
		h,
		0);
	if (scr->window == NULL)
		error_sdl_exit();

	scr->surface = SDL_GetWindowSurface(scr->window);
	if (scr->surface == NULL)
		error_sdl_exit();

	scr->renderer = SDL_CreateSoftwareRenderer(scr->surface);
	if (scr->renderer == NULL)
		error_sdl_exit();

	if (SDL_SetRenderDrawBlendMode(scr->renderer, SDL_BLENDMODE_BLEND) ==
		-1)
		error_sdl_exit();

	return scr;
}

void screen_free(Screen scr)
{
	SDL_DestroyWindow(scr->window);
	SDL_DestroyRenderer(scr->renderer);
}

void screen_end()
{
	if (screen_text_bmp != NULL)
		screen_free_picture(screen_text_bmp);
	SDL_Quit();
}


int screen_width(Screen scr)
{
	return scr->surface->w;
}

int screen_height(Screen scr)
{
	return scr->surface->h;
}

void screen_wait(int ms)
{
	SDL_Delay(ms);
}

void screen_update(Screen scr)
{
	if (SDL_UpdateWindowSurface(scr->window) == -1)
		error_sdl_exit();
}

int screen_get_keyboard_event(SDL_Scancode *sc)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
		switch (event.type) {
		case SDL_KEYDOWN:
			if (event.key.repeat)
				break;
			if (sc != NULL)
				*sc = event.key.keysym.scancode;
			return SCREEN_KEYDOWN;
		case SDL_KEYUP:
			if (event.key.repeat)
				break;
			if (sc != NULL)
				*sc = event.key.keysym.scancode;
			return SCREEN_KEYUP;
		case SDL_QUIT:
			exit(0);
		}

	return 0;
}

int screen_key_pressed(SDL_Scancode scancode)
{
	int len;
	const uint8_t *state;

	SDL_PumpEvents();
	state = SDL_GetKeyboardState(&len);
	if (len <= scancode || scancode < 0)
		error_exit(ERR_INVALID_SCANCODE);
	return state[scancode];
}

void screen_fill(Screen scr, int r, int g, int b, int a)
{
	if (SDL_SetRenderDrawColor(scr->renderer, r, g, b, a) == -1)
		error_sdl_exit();
	if (SDL_RenderClear(scr->renderer) == -1)
		error_sdl_exit();
}

void screen_place(Screen scr, Picture pict, int x, int y, int sx, int sy,
			int w, int h, int flipped)
{
	SDL_Rect src, dst;
	int total_width = screen_width(scr), total_height = screen_height(scr);
	SDL_Surface *surface;

	if (flipped) {
		sx = pict->f->w - sx - w;
		surface = pict->f;
	} else {
		surface = pict->s;
	}

	if (x > total_width || y > total_height || x + w <= 0 || y + h <= 0)
		return; // Nothing to do here

	src.x = x < 0 ? sx - x : sx;
	src.y = y < 0 ? sy - y : sy;
	src.w = x < 0 ? w + x :
		x + w > total_width ? total_width - x :
		w;
	src.h = y < 0 ? h + y :
		y + h > total_height ? total_height - y:
		h;
	dst.x = x < 0 ? 0 : x;
	dst.y = y < 0 ? 0 : y;
	dst.w = src.w;
	dst.h = src.h;

	if (SDL_BlitSurface(surface, &src, scr->surface, &dst) == -1)
		error_sdl_exit();
}

Picture screen_get_picture(const char *file)
{
	SDL_Surface *raw, *converted, *flipped;
	SDL_PixelFormat *pxf;
	Picture pict = malloc(sizeof(struct Picture));

	if (pict == NULL)
		error_libc_exit();

	raw = SDL_LoadBMP(file);
	if (raw == NULL)
		error_sdl_exit();

	pxf = SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888);
	if (pxf == NULL)
		error_sdl_exit();

	converted = SDL_ConvertSurface(raw, pxf, 0);
	if (converted == NULL)
		error_sdl_exit();

	SDL_FreeFormat(pxf);
	SDL_FreeSurface(raw);

	flipped = SDL_CreateRGBSurface(0, raw->w, raw->h, 32,
			pxf->Rmask, pxf->Gmask, pxf->Bmask, pxf->Amask);
	if (flipped == NULL)
		error_sdl_exit();
	for (int y = 0; y < raw->h; y++)
		for (int x = 0; x < raw->w; x++)
			((int*)flipped->pixels)[(y + 1) * raw->w - x - 1] =
				((int*)converted->pixels)[y * raw->w + x];
	pict->s = converted;
	pict->f = flipped;
	return pict;
}

void screen_free_picture(Picture pict)
{
	SDL_FreeSurface(pict->f);
	SDL_FreeSurface(pict->s);
	free(pict);
}

void screen_print_char(Screen scr, int x, int y, char c)
{
	int i;
	if (c >= '0' && c <= '9')
		i = c - '0';
	else if (c >= 'a' && c <= 'z')
		i = c - 'a' + 10;
	else if (c >= 'A' && c <= 'Z')
		i = c - 'A' + 10;
	else if (c == '.')
		i = 36; // This is the copyright mark.
	else if (c == '-')
		i = 40;
	else if (c == '*')
		i = 41;
	else if (c == '!')
		i = 43;
	else
		return;
	screen_place(scr, screen_text_bmp, x, y,
			TEXT_X + (i % TEXT_PER_ROW) * TEXT_W,
			TEXT_Y + (i / TEXT_PER_ROW) * TEXT_H,
			TEXT_W,
			TEXT_H,
			0);
}

void screen_text_print(Screen scr, int x, int y, int w, const char *str)
{
	if (screen_text_bmp == NULL)
		screen_text_bmp = screen_get_picture(TEXT_FILE);

	for (int i = 0; str[i] != '\0'; i++)
		screen_print_char(
			scr,
			x + (w != 0 ? i % w : i) * TEXT_W,
			y + (w != 0 ? i / w : 0) * TEXT_H,
			str[i]);
}

void screen_text_centered(Screen scr, int x, int y, const char *str)
{
	int len = strlen(str);
	screen_text_print(scr, x - len * TEXT_W / 2, y, 0, str);
}
