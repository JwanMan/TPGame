/*
 * item_list.c -- Implementa las listas de objetos.
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
#include <stdlib.h>
#include "collision_type.h"
#include "error.h"
#include "item.h"
#include "item_list.h"
#include "screen.h"

struct ItemNode {
	ItemList l;
	int n;
};

struct ItemList {
	int len;
	int capacity;
	Item *elems;
};

ItemList item_list_create()
{
	ItemList il;

	il = malloc(sizeof(struct ItemList));

	if (il == NULL)
		error_libc_exit();

	il->len = 0;
	il->capacity = 1;

	il->elems = malloc(sizeof(Item));

	if (il->elems == NULL)
		error_libc_exit();

	return il;
}

void item_list_free(ItemList il)
{
	int i;

	for (i = il->len - 1; i >= 0; i--)
		item_free(il->elems[i]);

	free(il->elems);
	free(il);
}

ItemNode item_list_begin(ItemList il)
{
	ItemNode in = malloc(sizeof(struct ItemNode));

	if (in == NULL)
		error_libc_exit();

	in->l = il;
	in->n = 0;

	return in;
}

Item item_list_current(ItemNode in)
{
	return in->l->elems[in->n];
}

void item_list_advance(ItemNode in)
{
	in->n++;
}

void item_list_delete(ItemNode in)
{
	item_free(in->l->elems[in->n]);
	in->l->elems[in->n] = in->l->elems[--in->l->len];

	if (in->l->len > 0 && in->l->len * 2 <= in->l->capacity) {
		in->l->capacity /= 2;
		in->l->elems = realloc (in->l->elems,
					in->l->capacity * sizeof(Item));

		if (in->l->elems == NULL)
			error_libc_exit();
	}
}

int item_list_isend(ItemNode in)
{
	return in->n >= in->l->len;
}

void item_list_free_node(ItemNode in)
{
	free(in);
}

void item_list_append(ItemList il, Item e)
{
	if (il->len == il->capacity) {
		il->capacity *= 2;
		il->elems = realloc(il->elems, il->capacity * sizeof(Item));

		if (il->elems == NULL)
			error_libc_exit();
	}

	il->elems[il->len++] = e;
}

void item_list_update(ItemList il, int scroll, int w)
{
	ItemNode in = item_list_begin(il);
	Item i;

	while (!item_list_isend(in)) {
		if (item_x(i = item_list_current(in)) < scroll + w &&
			(item_update(i) ||
			 item_y(i) + item_height(i) <= 0 ||
			 item_x(i) + item_width(i) <= scroll))
			item_list_delete(in);
		else
			item_list_advance(in);
	}

	item_list_free_node(in);
}

void item_list_render(Screen scr, ItemList il, int scroll)
{
	ItemNode in;

	for (in = item_list_begin(il); !item_list_isend(in);
			item_list_advance(in))
		item_render(scr, item_list_current(in), scroll);

	item_list_free_node(in);
}
