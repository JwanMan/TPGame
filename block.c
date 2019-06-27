/*
 * block.c -- Implementa los bloques que aparecen en el juego.
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
#include <malloc.h>
#include "block.h"
#include "error.h"
#include "item.h"
#include "player.h"

#define BLOCK_FILE "52571.bmp"
#define BLOCK_TICKS_PER_MOVE 10
#define BLOCK_MOVE_FRAMES 3
#define BLOCK_TICKS_MULTICOIN_THRESHOLD 300
#define BLOCK_END_X 3
#define BLOCK_END_Y 0
#define BLOCK_ANIM_TICKS 3

struct Block {
	BlockType type;
	int x;
	int y;
	int ix;
	int iy;
	int cnt;
	int nanim;
};

Picture block_pict = NULL;

Block block_create(BlockType type, int x, int y, int ix, int iy, int nanim)
{
	Block b = malloc(sizeof(struct Block));
	if (b == NULL)
		error_libc_exit();

	if (nanim <= 0)
		error_exit(ERR_OUT_OF_RANGE);

	b->x = x;
	b->y = y;
	b->type = type;
	b->ix = ix;
	b->iy = iy;
	b->cnt = 0;
	b->nanim = nanim;

	return b;
}

void block_free(Block b)
{
	free(b);
}

void block_end()
{
	screen_free_picture(block_pict);
}

int block_x(Block b)
{
	return b->x;
}

int block_y(Block b)
{
	return b->y;
}

int block_render(Screen scr, Block b, int scroll)
{
	int x = b->ix + ((b->cnt++ / BLOCK_ANIM_TICKS) % b->nanim);

	if (BLOCK_SIZE*(b->x+1) <= scroll ||
		BLOCK_SIZE*b->x >= scroll + screen_width(scr))
		return 0; // Don't need to render

	if (block_pict == NULL)
		block_pict = screen_get_picture(BLOCK_FILE);

	screen_place(
		scr,
		block_pict,
		b->x * BLOCK_SIZE - scroll,
		screen_height(scr) - (b->y + 1) * BLOCK_SIZE,
		BLOCK_SIZE * x,
		BLOCK_SIZE * b->iy,
		BLOCK_SIZE,
		BLOCK_SIZE,
		0);

	return 1;
}

int block_can_pass_through(Block b)
{
	return b->type == BLOCK_TYPE_PASSTHROUGH;
}

void block_make_opaque(Block b)
{
	b->type = BLOCK_TYPE_OPAQUE;
	b->ix = BLOCK_END_X;
	b->iy = BLOCK_END_Y;
	b->nanim = 1;
}


void block_return_coin(Player p, Block b, Item *item)
{
	if (item != NULL)
		*item = item_create(
			ITEM_TYPE_TEMPCOIN,
			b->x * BLOCK_SIZE,
			(b->y + 1) * BLOCK_SIZE);
	player_add_coin(p);
}

int block_hit(Player p, Block b, Item *item)
{
	switch (b->type) {
	case BLOCK_TYPE_PASSTHROUGH:
		if (item != NULL)
			*item = NULL;
		return 0;
	case BLOCK_TYPE_OPAQUE:
		if (item != NULL)
			*item = NULL;
		return 0;
	case BLOCK_TYPE_COIN:
		block_return_coin(p, b, item);
		block_make_opaque(b);
		return 0;
	case BLOCK_TYPE_DESTROYCOIN:
		block_return_coin(p, b, item);
		return 1;
	case BLOCK_TYPE_MULTICOIN:
		// Put counter in some small number without altering the
		// animation.
		b->cnt %= BLOCK_TICKS_PER_MOVE * BLOCK_MOVE_FRAMES;
		b->type = BLOCK_TYPE_HIT_MULTICOIN;
	case BLOCK_TYPE_HIT_MULTICOIN:
		block_return_coin(p, b, item);
		if (b->cnt >= BLOCK_TICKS_MULTICOIN_THRESHOLD)
			block_make_opaque(b);
		return 0;
	case BLOCK_TYPE_UPGRADE:
		if (item != NULL)
			*item = item_create(
				player_state(p) == PLAYER_ST_BIG ||
				player_state(p) == PLAYER_ST_FLOWER ?
				ITEM_TYPE_FLOWER : ITEM_TYPE_MUSHROOM,
				b->x * BLOCK_SIZE,
				(b->y + 1) * BLOCK_SIZE);
		block_make_opaque(b);
		return 0;
	default:
		error_exit(ERR_OUT_OF_RANGE);
		return 0; // Execution will never reach this point.
	}
}

