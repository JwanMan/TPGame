/*
 * world.h -- Representación del estado del juego y carga del nivel.
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
 * \file world.h Representación del estado del juego y carga del nivel.
 */

#ifndef __WORLD_H
#define __WORLD_H

#include <stdio.h>
#include "screen.h"

/** Resultados de la llamada a world_play_frame, que indican el estado del
 *  mundo.
 */
typedef enum WorldState {

	/** Indica que la partida contin&uacute;a normalmente. */
	WORLD_ST_KEEP_ON,

	/** Indica que el personaje ha muerto. */
	WORLD_ST_DEAD,

	/** Indica que el nivel ha sido superado. */
	WORLD_ST_WON,

	/** Indica que el juego ha sido superado por llegar al mill&oacute;n de
	 *  monedas.
	 */
	WORLD_ST_MANY_COINS
} WorldState;

/** Representa un mundo en el juego, con bloques, items, etc. */
typedef struct World *World;

/**
 * \brief Crea un mundo del juego.
 * \param f Flujo del fichero con la informaci&oacute;n del mundo.
 * \return El mundo creado.
 * \remarks El flujo debe haber sido abierto en un modo que permita la lectura.
 * Una vez ejecutada esta funci&oacute;n no es necesario mantener el flujo de
 * fichero abierto.
 */
World world_create(FILE *f);

/**
 * \brief Libera un mundo del juego.
 * \param w El mundo en cuesti&oacute;n.
 */
void world_free(World w);

/**
 * \brief Libera los recursos globales ocupados por el m&oacute;dulo, como las
 * im&aacute;genes.
 * \remarks Requiere ser llamada antes que a screen_end.
void world_end();
 */
void world_end();

/**
 * \brief Obtiene el personaje que juega en el nivel.
 * \param w El nivel.
 * \return El personaje.
 */
Player world_player(World w);

/**
 * \brief Sustituye el personaje actual en el mundo por el personaje dado, que
 * establece en la posici&oacute;n ocupada por el anterior.
 * \param w El mundo.
 * \param p El nuevo personaje.
 * \remarks Esta funci&oacute;n libera la memoria ocupada por el personaje
 * anterior del mundo.
 */
void world_substitute_player(World w, Player p);

/**
 * \brief Ejecuta un frame del mundo, en el que lee los eventos de teclado,
 * actualiza el estado del mundo y lo dibuja en pantalla.
 * \param scr La ventana donde dibujar el mundo.
 * \param w El mundo en cuesti&oacute;n.
 * \return Devuelve el estado actual de la partida.
 */
WorldState world_play_frame(Screen scr, World w);

#endif // __WORLD_H
