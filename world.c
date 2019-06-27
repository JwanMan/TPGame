/*
 * world.c -- Representación del estado del juego y carga del nivel.
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
#include <stdio.h>
#include "block.h"
#include "block_list.h"
#include "collision.h"
#include "collision_type.h"
#include "error.h"
#include "item.h"
#include "item_list.h"
#include "screen.h"
#include "world.h"

#define WORLD_BGCOLOR_R 128
#define WORLD_BGCOLOR_G 128
#define WORLD_BGCOLOR_B 255
#define WORLD_BGCOLOR_A 255

struct World {
	BlockList bl;
	ItemList il;
	Player p;
	int scroll;
	int dir;
	int max_scroll;
};

struct WorldBlockDec {
	BlockType type;	// Block type
	int ix;		// X coordinate of the block in the picture (tiles)
	int iy;		// Y coordinate of the block in the picture (tiles)
	int nanim;	// Number of animation steps
	int dx;		// In world_block_dec, (dx,dy) is the size of the
	int dy;		// structure. In world_block_conts, this is the
			// position of the block relative to the position of
			// the first block defined in the structure.
	int next;	// Index of the next block in world_block_conts,
			// or -1.
};

const struct WorldBlockDec world_block_dec[] = {
	{BLOCK_TYPE_UPGRADE, 24, 0, 3, 1, 1, -1}, //a: Upgrade ? block
	{BLOCK_TYPE_PASSTHROUGH, 2, 0, 1, 5, 5, 16}, //b: Castle
	{BLOCK_TYPE_COIN, 2, 0, 1, 1, 1, -1}, //c: Simple coin block
	{BLOCK_TYPE_DESTROYCOIN, 2, 0, 1, 1, 1, -1}, //d: Destroy coin block
	{BLOCK_TYPE_COIN, 24, 0, 3, 1, 1, -1}, //e: Coin ? block
	{BLOCK_TYPE_OPAQUE, 0, 0, 1, 1, 1, -1}, //f: Floor block
	{BLOCK_TYPE_MULTICOIN, 2, 0, 1, 1, 1, -1}, //g: Multiple coin block
	{BLOCK_TYPE_PASSTHROUGH, 8, 8, 1, 5, 3, 0}, //h: Hill
	{BLOCK_TYPE_PASSTHROUGH, 8, 8, 1, 3, 2, 11}, //i: Small hill
	{0, 16, 8, 1, 1, 1, -1}, //j: Nothing
	{0, 16, 8, 1, 1, 1, -1}, //k: Nothing
	{0, 16, 8, 1, 1, 1, -1}, //l: Nothing
	{BLOCK_TYPE_PASSTHROUGH, 11, 9, 1, 1, 1, -1}, //m: Bush (left)
	{BLOCK_TYPE_PASSTHROUGH, 12, 9, 1, 1, 1, -1}, //n: Bush (middle)
	{BLOCK_TYPE_PASSTHROUGH, 13, 9, 1, 1, 1, -1}, //o: Bush (right)
	{0, 16, 8, 1, 1, 1, -1}, //p: Nothing
	{0, 16, 8, 1, 1, 1, -1}, //q: Nothing
	{0, 16, 8, 1, 1, 1, -1}, //r: Nothing
	{BLOCK_TYPE_OPAQUE, 0, 1, 1, 1, 1, -1}, //s: Stairs
	{BLOCK_TYPE_OPAQUE, 0, 8, 1, 2, 1, 14}, //t: Tube (upper part)
	{BLOCK_TYPE_OPAQUE, 0, 9, 1, 2, 1, 15}, //u: Tube (lower part)
	{0, 16, 8, 1, 1, 1, -1}, //v: Nothing
	{0, 16, 8, 1, 1, 1, -1}, //w: Nothing
	{BLOCK_TYPE_PASSTHROUGH, 0, 21, 1, 1, 2, 8}, //x: Cloud (left)
	{BLOCK_TYPE_PASSTHROUGH, 1, 21, 1, 1, 2, 9}, //y: Cloud (middle)
	{BLOCK_TYPE_PASSTHROUGH, 2, 21, 1, 1, 1, 10}, //z: Cloud (right)
}, world_block_conts[] = {
	{BLOCK_TYPE_PASSTHROUGH, 8, 8, 1, 1, 1, 1}, // Hill
	{BLOCK_TYPE_PASSTHROUGH, 8, 9, 1, 1, 0, 2},
	{BLOCK_TYPE_PASSTHROUGH, 10, 9, 1, 3, 0, 3},
	{BLOCK_TYPE_PASSTHROUGH, 8, 9, 1, 2, 1, 4},
	{BLOCK_TYPE_PASSTHROUGH, 9, 9, 1, 2, 0, 5},
	{BLOCK_TYPE_PASSTHROUGH, 9, 8, 1, 2, 2, 6},
	{BLOCK_TYPE_PASSTHROUGH, 10, 8, 1, 3, 1, 7},
	{BLOCK_TYPE_PASSTHROUGH, 10, 8, 1, 4, 0, -1},
	{BLOCK_TYPE_PASSTHROUGH, 0, 20, 1, 0, 1, -1}, // Cloud (left)
	{BLOCK_TYPE_PASSTHROUGH, 1, 20, 1, 0, 1, -1}, // Cloud (middle)
	{BLOCK_TYPE_PASSTHROUGH, 2, 20, 1, 0, 1, -1}, // Cloud (right)
	{BLOCK_TYPE_PASSTHROUGH, 8, 9, 1, 1, 0, 12}, // Small hill
	{BLOCK_TYPE_PASSTHROUGH, 10, 8, 1, 2, 0, 13},
	{BLOCK_TYPE_PASSTHROUGH, 9, 8, 1, 1, 1, -1},
	{BLOCK_TYPE_OPAQUE, 1, 8, 1, 1, 0, -1}, // Tube (upper part)
	{BLOCK_TYPE_OPAQUE, 1, 9, 1, 1, 0, -1}, // Tube (lower part)
	{BLOCK_TYPE_PASSTHROUGH, 2, 0, 1, 1, 0, 17}, // Castle
	{BLOCK_TYPE_PASSTHROUGH, 2, 0, 1, 0, 1, 18},
	{BLOCK_TYPE_PASSTHROUGH, 2, 0, 1, 1, 1, 19},
	{BLOCK_TYPE_PASSTHROUGH, 2, 0, 1, 3, 0, 20},
	{BLOCK_TYPE_PASSTHROUGH, 2, 0, 1, 3, 1, 21},
	{BLOCK_TYPE_PASSTHROUGH, 2, 0, 1, 4, 0, 22},
	{BLOCK_TYPE_PASSTHROUGH, 2, 0, 1, 4, 1, 23},
	{BLOCK_TYPE_PASSTHROUGH, 13, 1, 1, 2, 0, 24},
	{BLOCK_TYPE_PASSTHROUGH, 12, 1, 1, 2, 1, 25},
	{BLOCK_TYPE_PASSTHROUGH, 11, 0, 1, 0, 2, 26},
	{BLOCK_TYPE_PASSTHROUGH, 11, 1, 1, 1, 2, 27},
	{BLOCK_TYPE_PASSTHROUGH, 11, 1, 1, 2, 2, 28},
	{BLOCK_TYPE_PASSTHROUGH, 11, 1, 1, 3, 2, 29},
	{BLOCK_TYPE_PASSTHROUGH, 11, 0, 1, 4, 2, 30},
	{BLOCK_TYPE_PASSTHROUGH, 14, 0, 1, 1, 3, 31},
	{BLOCK_TYPE_PASSTHROUGH, 2, 0, 1, 2, 3, 32},
	{BLOCK_TYPE_PASSTHROUGH, 14, 0, 1, 3, 3, 33},
	{BLOCK_TYPE_PASSTHROUGH, 11, 0, 1, 1, 4, 34},
	{BLOCK_TYPE_PASSTHROUGH, 11, 0, 1, 2, 4, 35},
	{BLOCK_TYPE_PASSTHROUGH, 11, 0, 1, 3, 4, -1}
};

Player world_read_player(FILE *f)
{
	int x, y, lives;

	if (fscanf(f, " %i %i %i", &x, &y, &lives) != 3)
		error_exit(ERR_INVALID_INPUT);

	return player_create(x * BLOCK_SIZE, y * BLOCK_SIZE, lives);
}

Block world_block_from_dec(const struct WorldBlockDec *wbd, int x, int y)
{
	return block_create(wbd->type, x, y, wbd->ix, wbd->iy, wbd->nanim);
}

void world_parse_one_block(char type, int x, int y, BlockList bl)
{
	const struct WorldBlockDec *wbd;

	wbd = &(world_block_dec[type - 'a']);
	block_list_append(bl, world_block_from_dec(wbd, x, y));

	while (wbd->next != -1) {
		wbd = &world_block_conts[wbd->next];
		block_list_append(bl,
			world_block_from_dec(wbd, x + wbd->dx, y + wbd->dy));
	}
}

void world_parse_block(char type, int x, int y, int nx, int ny,
			BlockList bl)
{
	int i, j, dx, dy;
	const struct WorldBlockDec *wbd;

	if (type < 'a' || type > 'z')
		error_exit(ERR_INVALID_INPUT);

	wbd = &(world_block_dec[type - 'a']);
	dx = wbd->dx;
	dy = wbd->dy;

	for (i = 0; i < nx; i++)
		for (j = 0; j < ny; j++)
			world_parse_one_block(
				type,
				x + dx*i,
				y + dy*j,
				bl);
}

BlockList world_read_blocks(FILE *f)
{
	BlockList bl = block_list_create();
	char type;
	int x, y, nx, ny, read;
	while ((read = fscanf(f, " %c %i %i %i %i", &type, &x, &y, &nx, &ny))
			>= 3) {
		if (read < 5) {
			ny = 1;
			if (read == 3)
				nx = 1;
		}
		world_parse_block(type, x, y, nx, ny, bl);
	}
	return block_list_sort(bl);
}

ItemType world_item_type(char c)
{
	switch (c) {
	case 'g':
		return ITEM_TYPE_GOOMBA;
	case 'k':
		return ITEM_TYPE_KOOPA;
	case 'c':
		return ITEM_TYPE_COIN;
	default:
		error_exit(ERR_INVALID_INPUT);
		return 0; // Execution will never reach this point
	}
}

ItemList world_read_items(FILE *f)
{
	ItemList il = item_list_create();
	char type;
	int x, y, read;

	while ((read = fscanf(f, " %c %i %i", &type, &x, &y)) == 3)
		item_list_append(il,
			item_create(world_item_type(type),
					x * BLOCK_SIZE,
					y * BLOCK_SIZE));

	return il;
}

World world_create(FILE *f)
{
	World w = malloc(sizeof(struct World));

	if (w == NULL)
		error_libc_exit();

	w->p = world_read_player(f);

	if (fscanf(f, " %i", &w->max_scroll) != 1)
		error_libc_exit();

	w->max_scroll *= BLOCK_SIZE;

	w->bl = world_read_blocks(f);
	w->il = world_read_items(f);
	w->scroll = 0;

	return w;
}

void world_free(World w)
{
	player_free(w->p);
	block_list_free(w->bl);
	item_list_free(w->il);
	free(w);
}

void world_end()
{
	block_end();
	item_end();
	player_end();
}

Player world_player(World w)
{
	return w->p;
}

void world_substitute_player(World w, Player p)
{
	int x = player_x(w->p), y = player_y(w->p);
	player_free(w->p);
	w->p = p;
	player_set_x(w->p, x);
	player_set_y(w->p, y);
}

WorldState world_play_frame(Screen scr, World w)
{
	SDL_Scancode sc;
	int k;
	char coins[7];

	while ((k = screen_get_keyboard_event(&sc)) != 0)
		if (k == SCREEN_KEYDOWN)
			switch (sc) {
			case SDL_SCANCODE_UP:
				player_jump(w->p);
				break;
			case SDL_SCANCODE_LEFT:
				w->dir = -1;
				break;
			case SDL_SCANCODE_RIGHT:
				w->dir = 1;
				break;
			default:
				;
			}
		else if (k == SCREEN_KEYUP)
			switch (sc) {
			case SDL_SCANCODE_LEFT:
				w->dir = screen_key_pressed(SDL_SCANCODE_RIGHT)
					? 1 : 0;
				break;
			case SDL_SCANCODE_RIGHT:
				w->dir = screen_key_pressed(SDL_SCANCODE_LEFT)
					? -1 : 0;
				break;
			default:
				;
			}

	switch(w->dir) {
	case -1:
		player_accel(w->p, 1);
		break;
	case 0:
		player_stop(w->p);
		break;
	case 1:
		player_accel(w->p, 0);
	}

	player_update(w->p);
	if (player_x(w->p) < w->scroll)
		player_set_x(w->p, w->scroll);
	if ((player_x(w->p) - w->scroll) * 2 > screen_width(scr))
		w->scroll = player_x(w->p) - screen_width(scr) / 2;
	if ((player_y(w->p) + player_height(w->p)) < 0)
		player_die(w->p);

	item_list_update(w->il, w->scroll, screen_width(scr));
	collision_player_with_blocks(w->p, w->bl, w->il);
	collision_player_with_items(w->p, w->il);
	collision_blocks_with_items(w->bl, w->il);

	screen_fill(scr, WORLD_BGCOLOR_R, WORLD_BGCOLOR_G, WORLD_BGCOLOR_B,
			WORLD_BGCOLOR_A);
	block_list_render(scr, w->bl, w->scroll);
	item_list_render(scr, w->il, w->scroll);
	player_render(scr, w->p, w->scroll);

	if (player_coins(w->p) >= 1000000)
		return WORLD_ST_MANY_COINS;
	sprintf(coins, "%06d", player_coins(w->p));
	screen_text_print(scr, 0, 0, 0, coins);

	screen_update(scr);

	return player_state(w->p) == PLAYER_ST_DEAD ? WORLD_ST_DEAD :
		w->scroll >= w->max_scroll ? WORLD_ST_WON : WORLD_ST_KEEP_ON;
}
