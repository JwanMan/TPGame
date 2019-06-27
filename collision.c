/*
 * collision.c -- Comntiene funciones de detección de colisiones.
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
#include <math.h>
#include "block.h"
#include "block_list.h"
#include "collision.h"
#include "collision_type.h"
#include "item.h"
#include "item_list.h"
#include "player.h"

CollisionType collision_data(double x1, double y1, double w1, double h1,
		double x2, double y2, double w2, double h2)
{
	double dw = (w1+w2)/2, dh = (h1+h2)/2;
	double dx = x1 - x2 + dw - w2, dy = y1 - y2 + dh - h2;
	double dxp, dyp;
	CollisionType c = 0;
	if (dx < dw && -dx < dw && dy < dh && -dy < dh) {
		dxp = dx * 0.75 - dy;
		dyp = dx * 0.75 + dy;
		if (dxp > 0) {
			if (dyp > 0)
				c |= COLLISION_RIGHT;
			else
				c |= COLLISION_BOTTOM;
		} else {
			if (dyp > 0)
				c |= COLLISION_TOP;
			else
				c |= COLLISION_LEFT;
		}
	}
	return c;
}

void collision_hit_if_top_and_advance(Player p, BlockNode *bn,
					CollisionType c, ItemList il)
{
	Item i = NULL;
	if (c & COLLISION_TOP) {
		if (block_hit(p, block_list_current(*bn), &i))
			block_list_delete(*bn);
		else
			*bn = block_list_next(*bn);

		if (i != NULL)
			item_list_append(il, i);
	} else {
		*bn = block_list_next(*bn);
	}
}

void collision_player_with_blocks(Player p, BlockList bl, ItemList il)
{
	BlockNode bn = block_list_begin(bl);
	Block b;
	double px = player_x(p), py = player_y(p), ph = player_height(p);
	int pxb = (int)player_x(p) / BLOCK_SIZE;
	CollisionType total_collisions = 0, c;

	while (!block_list_isend(bn) &&
		block_x(block_list_current(bn)) < pxb)
		bn = block_list_next(bn);

	while (!block_list_isend(bn) &&
		block_x(b = block_list_current(bn)) < pxb + 2)
		if (!block_can_pass_through(b) &&
			(c = collision_data(
				block_x(b) * BLOCK_SIZE,
				block_y(b) * BLOCK_SIZE,
				BLOCK_SIZE,
				BLOCK_SIZE,
				px,
				py,
				PLAYER_WIDTH,
				ph))
			!= 0) {
			collision_hit_if_top_and_advance(p, &bn, c, il);
			total_collisions |= c;
		} else {
			bn = block_list_next(bn);
		}

	player_correct_on_collision(p, total_collisions);
}

void collision_player_with_items(Player p, ItemList il)
{
	ItemNode in = item_list_begin(il);
	Item i;
	double px = player_x(p), py = player_y(p), ph = player_height(p);
	CollisionType c;

	while (!item_list_isend(in)) {
		i = item_list_current(in);
		c = collision_data(
			item_x(i), item_y(i), item_width(i), item_height(i),
			px, py, PLAYER_WIDTH, ph);

		if (c != 0 && item_player_collide(i, p, c))
			item_list_delete(in);
		else
			item_list_advance(in);
	}

	item_list_free_node(in);
}

void collision_blocks_with_items(BlockList bl, ItemList il)
{
	ItemNode in;
	Item i;
	BlockNode bn;
	Block b;
	CollisionType c;
	double ix, iy, iw, ih;

	for (in = item_list_begin(il); !item_list_isend(in);
		item_list_advance(in)) {
		i = item_list_current(in);
		ix = item_x(i);
		iy = item_y(i);
		iw = item_width(i);
		ih = item_height(i);
		c = 0;
		for (bn = block_list_begin(bl); !block_list_isend(bn);
			bn = block_list_next(bn)) {
			b = block_list_current(bn);
			if (!block_can_pass_through(b))
				c |= collision_data(
					block_x(b) * BLOCK_SIZE,
					block_y(b) * BLOCK_SIZE,
					BLOCK_SIZE,
					BLOCK_SIZE,
					ix,
					iy,
					iw,
					ih);
		}
		if (c != 0)
			item_correct_on_block_collision(i, c);
	}

	item_list_free_node(in);
}
