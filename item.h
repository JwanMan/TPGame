/*
 * item.h -- Implementa los objetos (y enemigos) que aparecen en el juego.
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
 * \file item.h Implementa los objetos (y enemigos) que aparecen en el juego.
 */

#ifndef __ITEM_H
#define __ITEM_H
#include "collision_type.h"
#include "player.h"
#include "screen.h"

/** Representa un tipo de objeto. */
typedef enum ItemType {
	/** Indica que el objeto es un goomba. */
	ITEM_TYPE_GOOMBA,

	/** Indica que el objeto es un koopa. */
	ITEM_TYPE_KOOPA,

	/** Indica que el objeto es un champi&ntilde;&oacute;n. */
	ITEM_TYPE_MUSHROOM,

	/** Indica que el objeto es una flor. */
	ITEM_TYPE_FLOWER,

	/** Indica que el objeto es una moneda. */
	ITEM_TYPE_COIN,

	/** Indica que el objeto es una "moneda temporal", es decir, una que
	 * se muestra al golpear un bloque que devuelve una moneda pero que se
	 * contabiliza en el momento de golpear el bloque, y no al coger la
	 * moneda.
	 */
	ITEM_TYPE_TEMPCOIN
} ItemType;

/** Tipo de dato objeto. */
typedef struct Item *Item;

/**
 * \brief Crea un objeto.
 * \param type Tipo de objeto, uno de los definidos como ITEM_TYPE_*.
 * \param x Coordenada X de la posici&oacute;n inicial del objeto, en
 * p&iacute;xeles respecto del borde izquierdo del mundo.
 * \param y Coordenada Y de la posici&oacute;n inicial del objeto, en
 * p&iacute;xeles respecto de la parte inferior del mundo.
 */
Item item_create(ItemType type, double x, double y);

/**
 * \brief Libera un objeto.
 * \param i El objeto a liberar.
 */
void item_free(Item i);

/**
 * \brief Libera los recursos utilizados por el TDA de forma global (las
 * im&aacute;genes.
 */
void item_end();

/**
 * \brief Obtiene la coordenada X del objeto.
 * \param i El objeto.
 * \return La coordenada X del objeto, en p&iacute;xeles desde la parte
 * izquierda del mundo.
 */
double item_x(Item i);

/**
 * \brief Obtiene la coordenada Y del objeto.
 * \param i El objeto.
 * \return La coordenada Y del objeto, en p&iacute;xeles desde la parte inferior
 * del mundo.
 */
double item_y(Item i);

/**
 * \brief Devuelve la velocidad horizontal del objeto.
 * \param i El objeto.
 * \return La velocidad horizontal del objeto (hacia la derecha) en
 * p&iacute;xeles por frame.
 */
double item_vx(Item i);

/**
 * \brief Devuelve la velocidad vertical del objeto.
 * \param i El objeto.
 * \return La velocidad vertical del objeto (hacia arriba) en p&iacute;xeles
 * por frame.
 */
double item_vy(Item i);

/**
 * \brief Devuelve el ancho del objeto.
 * \param i El objeto.
 * \return El ancho del objeto en p&iacute;xeles.
 */
int item_width(Item i);

/**
 * \brief Devuelve el alto del objeto.
 * \param i El objeto.
 * \return El alto del objeto en p&iacute;xeles.
 */
int item_height(Item i);

/**
 * \brief Actualiza la posici&oacute;n y propiedades de un objeto despu&eacute;s
 * de un frame.
 * \param i El objeto en cuesti&oacute;n.
 * \return Un valor distinto de 0 si el objeto debe ser eliminado, o 0 si no
 * debe serlo.
 * \remarks Esta funci&oacute;n act&uacute;a como si el objeto no chocara con
 * nada. Puede ser necesario llamar posteriormente a
 * item_correct_on_block_collision o a item_player_collide.
 */
int item_update(Item i);

/**
 * \brief Corrige la posici&oacute;n de un objeto tras colisionar con un bloque.
 * \param i El objeto en cuesti&oacute;n.
 * \param dir La direcci&oacute;n relativa del bloque con el que choca el objeto
 * respecto de la posici&oacute;n del objeto.
 */
void item_correct_on_block_collision(Item i, CollisionType dir);

/**
 * \brief Realiza las acciones necesarias cuando el objeto colisiona con el
 * personaje.
 * \param i El objeto en cuesti&oacute;n.
 * \param p El personaje.
 * \param dir La posici&oacute;n relativa del objeto respecto del personaje.
 * \return Valor distinto de 0 si deber&iacute;a liberarse y eliminarse el
 * objeto, o 0 si no deber&iacute;a.
 */
int item_player_collide(Item i, Player p, CollisionType dir);

/**
 * \brief Dibuja el objeto en la pantalla, siempre que est&eacute; en el
 * &aacute;rea visible.
 * \param scr La ventana donde dibujar el objeto.
 * \param i El objeto.
 * \param scroll La distancia en p&iacute;xeles desde la parte izquierda del
 * mundo hasta la parte izquierda de la ventana.
 */
void item_render(Screen scr, Item i, int scroll);

#endif // __ITEM_H
