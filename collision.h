/*
 * collision.h -- Contiene funciones de detección de colisiones.
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
 * \file collision.h -- Contiene funciones de detección de colisiones.
 */

#ifndef __COLLISION_H
#define __COLLISION_H
#include "block_list.h"
#include "collision_type.h"
#include "item_list.h"
#include "player.h"

/**
 * \brief Comprueba las colisiones del personaje con los bloques y realiza las
 * acciones oportunas.
 * \param p El personaje.
 * \param bl La lista de bloques a comprobar.
 * \param il La lista de objetos, por si fuera necesario a&ntilde;adirle
 * elementos.
 */
void collision_player_with_blocks(Player p, BlockList bl, ItemList il);

/**
 * \brief Comprueba las colisiones del personaje con los objetos y realiza las
 * acciones oportunas.
 * \param p El personaje.
 * \param il La lista de objetos.
 */
void collision_player_with_items(Player p, ItemList il);

/**
 * \brief Comprueba las colisiones de los bloques con los objetos y realiza las
 * acciones oportunas.
 * \param bl La lista de bloques.
 * \param il La lista de objetos.
 */
void collision_blocks_with_items(BlockList bl, ItemList il);

#endif // __COLLISION_H
