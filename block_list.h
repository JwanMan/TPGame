/*
 * block_list.h -- Implementa las listas de bloques.
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
 * \file block_list.h Implementa las listas de bloques.
 */

#ifndef __BLOCK_LIST_H
#define __BLOCK_LIST_H
#include "block.h"
#include "screen.h"

/** Representa un nodo de la lista de bloques. Usado para iterar sobre esta. */
typedef struct BlockNode *BlockNode;

/** Representa una lista de bloques. */
typedef struct BlockList *BlockList;

/**
 * \brief Crea una lista vac&iacute;a de bloques.
 * \return Una nueva lista vac&iacute;a de bloques.
 */
BlockList block_list_create();

/**
 * \brief Libera una lista de bloques, junto con todos sus elementos.
 * \param bl La lista de bloques a liberar.
 * \remarks Si la lista es liberada, todo iterador sobre ella dejar&aacute; de
 * ser v&aacute;lido (ver block_list_begin).
 */
void block_list_free(BlockList bl);

/**
 * \brief Devuelve un iterador para la lista, que apunta al primer elemento de
 * esta (si existe).
 * \param bl La lista sobre la que iterar.
 * \return Iterador de la lista apuntando al primer elemento. Si la lista
 * est&aacute; vac&iacute;a, el iterador apunta al final (ver block_list_isend).
 */
BlockNode block_list_begin(BlockList bl);

/**
 * \brief Devuelve el bloque apuntado por un iterador de lista de bloques.
 * \param bn El iterador.
 * \return El elemento actual al que apunta el iterador.
 */
Block block_list_current(BlockNode bn);

/**
 * \brief Devuelve un iterador que apunta al elemento siguiente al que apunta
 * este.
 * \param bn El iterador.
 * \return Un iterador al siguiente elemento.
 * \remarks No se debe liberar el apuntador anterior al reemplazarlo por el
 * nuevo (ni en ning&uacute;n otro caso). Si bn apunta al &uacute;ltimo
 * elemento, el iterador devuelto apuntar&aacute; al final (ver
 * block_list_isend).
 */
BlockNode block_list_next(BlockNode bn);

/**
 * \brief Elimina de la lista el elemento actual al que apunta el iterador (el
 * devuelto por block_list_current), liber&aacute;ndolo.
 * \param bn El iterador.
 * \remarks Tras llamar a esta funci&oacute;n, el iterador apuntar&aacute; al
 * elemento siguiente al que apuntaba.
 */
void block_list_delete(BlockNode bn);

/**
 * \brief Elimina de la lista el elemento actual al que apunta el iterador (el
 * devuelto por block_list_current), pero sin liberar la memoria ocupada por
 * el bloque en s&iacute;.
 * \param bn El iterador.
 * \remarks Tras llamar a esta funci&oacute;n, el iterador apuntar&aacute; al
 * elemento siguiente al que apuntaba.
 */
void block_list_delete_preserving(BlockNode bn);

/**
 * \brief Comprueba si el apuntador pasado como par&aacute;metro apunta al
 * final.
 * \param bn El iterador.
 * \return Un valor distinto de 0 si el elemento es el final, o 0 si no lo es.
 * \remarks Es un error llamar a block_list_current, block_list_next o
 * block_list_delete con un iterador que apunta al final, pues es como si
 * apuntase al elemento posterior al &uacute;ltimo.
 */
int block_list_isend(BlockNode bn);

/**
 * \brief Comprueba si la lista de bloques es vac&iacute;a.
 * \param bl La lista a comprobar.
 * \return Un valor distinto de 0 si la lista es vac&iacute;a, o 0 si no lo es.
 */
int block_list_isempty(BlockList bl);

/**
 * \brief A&ntilde;ade un elemento al final de la lista.
 * \param bl La lista en la que a&ntilde;adir el elemento.
 * \param e El elemento a a&ntilde;adir.
 */
void block_list_append(BlockList bl, Block e);

/**
 * \brief Ordena una lista de bloques seg&uacute;n la coordenada horizontal de
 * los bloques.
 * \param bl La lista de bloques a ordenar.
 * \return La lista de bloques ordenada.
 * \remarks Esta funci&oacute;n libera la memoria ocupada por la lista de
 * bloques que se le pasa como par&aacute;metro, salvo la correspondiente a los
 * bloques en s&iacute;, que pasa a ser accesible desde la lista de bloques
 * devuelta. La funci&oacute;n asume que la coordenada horizontal de todos los
 * bloques es mayor o igual a cero; de lo contrario la lista devuelta
 * podr&iacute; no estar bien ordenada.
 */
BlockList block_list_sort(BlockList bl);

/**
 * \brief Dibuja todos los elementos de la lista de bloques, suponiendo que
 * est&aacute;n ordenados por posici&oacute;n en el mundo de izquierda y derecha
 * y eliminando los elementos que quedan a la izquierda del &aacute;rea visible
 * del mundo.
 * \param bl La lista de bloques.
 * \param scr La ventana sobre la que dibujar los bloques.
 * \param scroll La distnacia en p&iacute;xeles desde la parte izquierda del
 * mundo hasta la parte izquierda de la ventana.
 */
void block_list_render(Screen scr, BlockList bl, int scroll);

#endif // __BLOCK_LIST_H
