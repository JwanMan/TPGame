/*
 * error.c -- Implementa la gestión de errores del programa.
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
#include <errno.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"

#define ERR_ON_SDL -1023
#define ERR_BASE -1024

const char *error_table[] = {
	"Invalid scancode.",
	"Null pointer incorrectly passed as a parameter.",
	"Parameter out of range.",
	"Invalid input."
};

void error_exit(ErrorType type)
{
	if (type < 0 || sizeof(error_table) / sizeof(char*) <= type)
		fputs("Unknown error.\n", stderr);
	else
		fprintf(stderr, "Error: %s\n", error_table[type]);
	exit(ERR_BASE-type);
}

void error_libc_exit()
{
	int err = errno; // perror may change the value of errno
	perror("Error");
	exit(err);
}

void error_sdl_exit()
{
	fputs(SDL_GetError(), stderr);
	exit(ERR_ON_SDL);
}
