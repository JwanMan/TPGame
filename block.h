/*
 * block.h -- Implementa los bloques que aparecen en el juego.
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
 * \file block.h Implementa los bloques que aparecen en el juego.
 */

#ifndef __BLOCK_H
#define __BLOCK_H
#include "collision_type.h"
#include "item.h"
#include "player.h"
#include "screen.h"

/** El tama&ntilde;o de un bloque en p&iacute;xeles. */
#define BLOCK_SIZE 16

/** Representa un tipo de bloque. */
typedef enum BlockType {
	/** Indica que se trata de decoraci&oacute;n de fondo, por tanto
         *  atravesable sin ning&uacute;n efecto.
	 */
	BLOCK_TYPE_PASSTHROUGH,

	/** Indica un bloque opaco, no atravesable pero que no hace nada. */
	BLOCK_TYPE_OPAQUE,

	/** Indica un bloque que al golpearlo devuelve una moneda y se
	 *  convierte en opaco.
	 */
	BLOCK_TYPE_COIN,

	/** Indica un bloque que al golpearlo devuelve una moneda y al hacerlo
	 *  es destru&iacute;do.
	 */
	BLOCK_TYPE_DESTROYCOIN,

	/** Indica un bloque que devuelve una moneda y despu&eacute;s sigue
	 *  devolviendo monedas durante un intervalo aproximado de tiempo, tras
	 *  el cual se convierte en bloque opaco normal.
	 */
	BLOCK_TYPE_MULTICOIN,

	/** Indica un bloque del tipo BLOCK_TYPE_MULTICOIN que ya ha sido
	 *  golpeado.
	 */
	BLOCK_TYPE_HIT_MULTICOIN,

	/** Indica un bloque que devuelve un champi&ntilde;&oacute;n, o una
	 *  flor si el personaje que lo golpea ya est&aacute; en estado
	 * "grande" o "flor".
	 */
	BLOCK_TYPE_UPGRADE
} BlockType;

/** Representa un bloque (fijo) del mundo. */
typedef struct Block *Block;

/**
 * \brief Crea un bloque del mapa.
 * \param type El tipo de bloque.
 * \param x La posici&oacute;n X del bloque, en bloques desde la izquierda del
 * mundo.
 * \param y La posici&oacute;n Y del bloque, en bloques desde la parte inferior
 * del mundo.
 * \param ix La coordenada X en la imagen de bloques en la que se encuentra el
 * bloque, en bloques.
 * \param iy La coordenada Y en la imagen de bloques en la que se encuentra el
 * bloque, en bloques.
 * \param nanim El n&uacute;mero de pasos de la animaci&oacute;n del bloque, que
 * en la imagen de bloques se almacenan de forma consecutiva en horizontal a
 * partir de las coordenadas dadas. Si el bloque no tiene animaci&oacute;n, este
 * valor debe ser 1.
 * \return El bloque creado.
 */
Block block_create(BlockType type, int x, int y, int ix, int iy, int nanim);

/**
 * \brief Libera un bloque.
 * \param b El bloque a liberar.
 */
void block_free(Block b);

/**
 * \brief Libera los recursos ocupados por el m&oacute;dulo que no pertenecen a ning&uacute;n bloque en concreto, como son las im&aacute;genes de
 * estos.
 * \remarks Debe ser llamada antes que a screen_end.
 */
void block_end();

/**
 * \brief Obtiene la coordenada X del bloque.
 * \param b El bloque.
 * \return La coordenada X del bloque, en bloques desde la parte izquierda del
 * mundo.
 */
int block_x(Block b);

/**
 * \brief Obtiene la coordenada Y del bloque.
 * \param b El bloque.
 * \return La coordenada Y del bloque, en bloques desde la parte inferior del
 * mundo.
 */
int block_y(Block b);

/**
 * \brief Dibuja un bloque en la pantalla.
 * \param scr La ventana donde dibujar el bloque.
 * \param b El bloque a dibujar.
 * \param scroll La distancia en p&iacute;xeles desde la parte izquierda del
 * mundo hasta la parte izquierda de la ventana.
 * \return Un valor distinto de 0 si el bloque est&aacute; dentro del
 * &aacute;rea visible del mundo (solo comprueba la coordenada horizontal),
 * o 0 si queda fuera de dicha &aacute;rea.
 */
int block_render(Screen scr, Block b, int scroll);

/**
 * \brief Compruba si el bloque es atravesable (por objetos o personajes).
 * \param b El bloque.
 * \return Un valor distinto de 0 si el bloque es atravesable, o 0 si no lo es.
 */
int block_can_pass_through(Block b);

/**
 * \brief Realiza las acciones necesiarias cuando un personaje golpea un bloque.
 * \param p El personaje.
 * \param b El bloque golpeado por el personaje.
 * \param item Par&aacute;metro de salida. Si no es NULL, la funci&oacute;n
 * deposita aqu&iacute; el objeto devuelto por el bloque al ser golpeado, o
 * NULL si no devuelve ning&uacute;n objeto.
 * \return Un valor distinto de 0 si el bloque debe ser eliminado, o 0 si no
 * debe serlo.
 */
int block_hit(Player p, Block b, Item *item);

#endif // __BLOCK_H
