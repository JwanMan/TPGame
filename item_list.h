/*
 * item_list.h -- Implementa las listas de objetos.
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
 * \file item_list.h Implementa las listas de objetos.
 */

#ifndef __ITEM_LIST_H
#define __ITEM_LIST_H
#include "item.h"
#include "screen.h"

/** Representa un nodo de la lista de objetos. Usado para iterar sobre esta. */
typedef struct ItemNode *ItemNode;

/** Representa una lista de objetos. */
typedef struct ItemList *ItemList;

/**
 * \brief Crea una lista vac&iacute;a de objetos.
 * \return Una nueva lista vac&iacute;a de objetos.
 */
ItemList item_list_create();

/**
 * \brief Libera una lista de objetos, junto con todos sus elementos.
 * \param il La lista de objetos a liberar.
 * \remarks Si la lista es liberada, todo iterador sobre ella dejar&aacute; de
 * ser v&aacute;lido (ver item_list_begin).
 */
void item_list_free(ItemList il);

/**
 * \brief Devuelve un iterador para la lista, que apunta al primer elemento de
 * esta (si existe).
 * \param il La lista sobre la que iterar.
 * \return Iterador de la lista apuntando al primer elemento. Si la lista
 * est&aacute; vac&iacute;a, el iterador apunta al final (ver item_list_isend).
 */
ItemNode item_list_begin(ItemList il);

/**
 * \brief Devuelve el objeto apuntado por un iterador de lista de objetos.
 * \param in El iterador.
 * \return El elemento actual al que apunta el iterador.
 */
Item item_list_current(ItemNode in);

/**
 * \brief Avanza el iterador para que apunte al elemento siguiente al actual.
 * \param in El iterador.
 */
void item_list_advance(ItemNode in);

/**
 * \brief Elimina de la lista el elemento actual al que apunta el iterador (el
 * devuelto por item_list_current), liber&aacute;ndolo.
 * \param in El iterador.
 * \remarks Tras llamar a esta funci&oacute;n, el iterador apuntar&aacute; a
 * un elemento posterior. No se garantiza que se mantenga el orden de los
 * elementos de la lista tras llamar a esta funci&oacute;n.
 */
void item_list_delete(ItemNode in);

/**
 * \brief Comprueba si el apuntador pasado como par&aacute;metro apunta al
 * final.
 * \param in El iterador.
 * \return Un valor distinto de 0 si el elemento es el final, o 0 si no lo es.
 * \remarks Es un error llamar a item_list_current, item_list_next o
 * item_list_delete con un iterador que apunta al final, pues es como si
 * apuntase al elemento posterior al &uacute;ltimo.
 */
int item_list_isend(ItemNode in);

/**
 * \brief Libera de memoria un iterador.
 * \param in El iterador a liberar.
 */
void item_list_free_node(ItemNode in);

/**
 * \brief A&ntilde;ade un elemento al final de la lista.
 * \param il La lista en la que a&ntilde;adir el elemento.
 * \param e El elemento a a&ntilde;adir.
 */
void item_list_append(ItemList il, Item e);

/**
 * \brief Actualiza los elementos de la lista de objetos, eliminando los que han
 * quedado a la izquierda del &aacute;rea visible del mundo y los que han
 * dejado de ser visibles por haber ca&iacute;do por debajo de este &aacute;rea,
 * y teniendo en cuenta las posibles colisiones.
 * \param il La lista de objetos.
 * \param scroll La distancia en p&iacute;xeles desde el borde izquierdo del
 * mundo hasta el borde izquierdo del &aacute;rea visible de este.
 * \param w El ancho del &aacute;rea visible del mundo, en p&iacute;xeles.
 * \remarks Esta funci&oacute;n no tiene en cuenta posibles colisiones, por
 * lo que es necesario manejar las colisiones aparte.
 */
void item_list_update(ItemList il, int scroll, int w);

/**
 * \brief Dibuja todos los elementos de la lista de objetos.
 * \param il La lista de objetos.
 * \param scr La ventana sobre la que dibujar los objetos.
 * \param scroll La distnacia en p&iacute;xeles desde la parte izquierda del
 * mundo hasta la parte izquierda de la ventana.
 */
void item_list_render(Screen scr, ItemList il, int scroll);

#endif // __ITEM_LIST_H
