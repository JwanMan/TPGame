/*
 * main.c -- Rutina principal del programa.
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

/**
 * \mainpage Juego de plataformas
 * En este juego controlas un personaje que va avanzando por un nivel con
 * el objetivo de llegar hasta el final, mientras esquiva enemigos y golpea
 * bloques para conseguir monedas u objetos.
 * \file main.c Fichero principal del juego.
 */

#include <stdio.h>
#include <string.h>
#include "error.h"
#include "player.h"
#include "screen.h"
#include "world.h"

#define FRAME_MILLISECONDS 20
#define MSG_BG_R 0
#define MSG_BG_G 0
#define MSG_BG_B 0
#define MSG_BG_A 255
#define WIDTH 256
#define HEIGHT 240
#define MSG_MILLISECONDS 3000

WorldState play(Screen scr, World w)
{
	WorldState ws;

	while ((ws = world_play_frame(scr, w)) == WORLD_ST_KEEP_ON)
		screen_wait(FRAME_MILLISECONDS);

	return ws;
}

void show_msg(Screen scr, char *s)
{
	screen_fill(scr, MSG_BG_R, MSG_BG_G, MSG_BG_B, MSG_BG_A);
	screen_text_centered(scr, WIDTH / 2, HEIGHT / 2 - 4, s);
	screen_update(scr);
	screen_wait(MSG_MILLISECONDS);
}

void show_copyright()
{
	printf(
		"Copyright (C) 2018 Juan Marín Noguera\n"
		"This is free software; see the source for copying "
		"conditions. There is NO warranty; not even for "
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
		"\n"
		"The graphics and designs are Copyright (C) 1985 "
		"Nintendo, Inc.\n"
		"All rights reserved.\n"
		"\n");
}

void show_instructions(Screen scr)
{
	SDL_Scancode sc;
	int k;

	do {
		screen_text_print(scr, 8, 72, 30,
			" Use las flechas IZQUIERDA y  "
			"                              "
			"    DERECHA para moverse      "
			"                              "
			"                              "
			"   Salte pulsando la flecha   "
			"                              "
			"            ARRIBA            "
			"                              "
			"                              "
			"                              "
			" Pulse ESPACIO para continuar ");
		screen_update(scr);

		k = screen_get_keyboard_event(&sc);
		if (k == 0)
			screen_wait(20);

	} while (k != SCREEN_KEYDOWN || sc != SDL_SCANCODE_SPACE);
}

int main(int argc, char **argv)
{
	Screen scr;
	World w;
	WorldState ws;
	Player p = NULL;
	FILE *f;
	char c[13];

	scr = screen_create("Super Mario Bros.", 255, 240);

	show_copyright();
	show_instructions(scr);

	do {
		f = fopen("1w", "r");
		if (f == NULL)
			error_libc_exit();

		w = world_create(f);
		fclose(f);

		if (p != NULL)
			world_substitute_player(w, p);

		ws = play(scr, w);

		p = world_player(w);
		player_restart(p);

		if (ws != WORLD_ST_DEAD || player_lives(p) <= 0)
			break;

		sprintf(c, "%i lives left", player_lives(p));
		show_msg(scr, c);
	} while (1);

	switch (ws) {
	case WORLD_ST_DEAD:
		show_msg(scr, "GAME OVER");
		break;
	case WORLD_ST_WON:
		show_msg(scr, "YOU WON");
		break;
	case WORLD_ST_MANY_COINS:
		show_msg(scr, "YOU GOT TO THE MILLION COINS");
		break;
	case WORLD_ST_KEEP_ON:
		show_msg(scr, "Sorry - something wrong happened");
	}

	world_free(w);
	screen_free(scr);
	world_end();
	screen_end();
	return 0;
}

