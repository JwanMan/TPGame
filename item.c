/*
 * item.c -- Implementa los objetos (y enemigos) que aparecen en el juego.
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
#include <stdlib.h>
#include "block.h"
#include "collision_type.h"
#include "error.h"
#include "item.h"
#include "player.h"
#include "screen.h"

#define ITEM_GRAVITY 0.15
#define ITEM_VELOCITY_X 1.2
#define ITEM_KOOPA_MAX_STILL 300
#define ITEM_TEMPCOIN_LIFETIME 5
#define ITEM_ANIM_TICKS 3
#define ITEM_ANIM_X_SHIFT 16
#define ITEM_GOOMBA_ANIM_NO 2
#define ITEM_KOOPA_ANIM_NO 4
#define ITEM_FLOWER_ANIM_NO 4

#define ITEM_GOOMBA_X 0
#define ITEM_GOOMBA_Y 16
#define ITEM_GOOMBA_W 16
#define ITEM_GOOMBA_H 16
#define ITEM_KOOPA_GROUND_X 160
#define ITEM_KOOPA_GROUND_Y 24
#define ITEM_KOOPA_GROUND_W 16
#define ITEM_KOOPA_GROUND_H 8
#define ITEM_KOOPA_X 96
#define ITEM_KOOPA_Y 10
#define ITEM_KOOPA_W 15
#define ITEM_KOOPA_H 22
#define ITEM_MUSHROOM_X 0
#define ITEM_MUSHROOM_Y 0
#define ITEM_FLOWER_X 0
#define ITEM_FLOWER_Y 32
#define ITEM_COIN_X 0
#define ITEM_COIN_Y 96
#define ITEM_UPGRADE_W 16
#define ITEM_UPGRADE_H 16

#define ITEM_ENEMIES_FILE "52570.bmp"
#define ITEM_UPGRADES_FILE "52569.bmp"

struct Item {
	ItemType type;
	int dir;
	int cnt;
	double x;
	double y;
	double vy;
};

Picture item_enemies_pict = NULL, item_upgrades_pict = NULL;

Item item_create(ItemType type, double x, double y)
{
	Item i = malloc(sizeof(struct Item));

	if (i == NULL)
		error_libc_exit();

	i->type = type;
	i->x = x;
	i->y = y;
	i->vy = 0;
	if (type == ITEM_TYPE_GOOMBA || type == ITEM_TYPE_KOOPA)
		i->dir = -1;
	else if (type == ITEM_TYPE_MUSHROOM)
		i->dir = 2 * (rand() % 1) - 1;
	else
		i->dir = 0;
	i->cnt = 0;
	return i;
}

void item_free(Item i)
{
	if (i == NULL)
		error_exit(ERR_NULL_PARAM);
	free(i);
}

void item_end()
{
	if (item_enemies_pict != NULL)
		screen_free_picture(item_enemies_pict);
	if (item_upgrades_pict != NULL)
		screen_free_picture(item_upgrades_pict);
}

double item_x(Item i)
{
	return i->x;
}

double item_y(Item i)
{
	return i->y;
}

double item_vx(Item i)
{
	return i->dir * ITEM_VELOCITY_X;
}

double item_vy(Item i)
{
	return i->vy;
}

int item_height(Item i)
{
	switch (i->type) {
	case ITEM_TYPE_GOOMBA:
		return ITEM_GOOMBA_H;
	case ITEM_TYPE_KOOPA:
		return i->dir == 0 ? ITEM_KOOPA_GROUND_H : ITEM_KOOPA_H;
	default:
		return ITEM_UPGRADE_H;
	}
}

int item_width(Item i)
{
	switch (i->type) {
	case ITEM_TYPE_GOOMBA:
		return ITEM_GOOMBA_W;
	case ITEM_TYPE_KOOPA:
		return i->dir == 0 ? ITEM_KOOPA_GROUND_W : ITEM_KOOPA_W;
	default:
		return ITEM_UPGRADE_W;
	}
}

int item_update(Item i)
{
	i->vy -= ITEM_GRAVITY;
	i->y += i->vy;

	i->x += i->dir * ITEM_VELOCITY_X;
	i->cnt++;

	if (i->type == ITEM_TYPE_KOOPA && i->dir == 0 &&
		i->cnt >= ITEM_KOOPA_MAX_STILL) {
		i->cnt = 0;
		i->dir = 1;
	}

	return i->type == ITEM_TYPE_TEMPCOIN && i->cnt > ITEM_TEMPCOIN_LIFETIME;
}

void item_correct_on_block_collision(Item i, CollisionType dir)
{
	if (dir & COLLISION_TOP) {
		i->vy = 0;
		i->y = i->y - 2 * (((int)i->y) % BLOCK_SIZE);
	}
	if (dir & COLLISION_BOTTOM) {
		i->vy = 0;
		i->y = ((int)(i->y + BLOCK_SIZE) / BLOCK_SIZE) * BLOCK_SIZE;
	}
	if (dir & COLLISION_LEFT) {
		i->x = (((int)(i->x + BLOCK_SIZE)) / BLOCK_SIZE) * BLOCK_SIZE;
		if (i->dir != 0)
			i->dir = 1;
	}
	if (dir & COLLISION_RIGHT) {
		i->x = (((int)i->x) / BLOCK_SIZE) * BLOCK_SIZE;
		if (i->dir != 0)
			i->dir = -1;
	}
}

int item_player_collide(Item i, Player p, CollisionType dir)
{
	switch (i->type) {
	case ITEM_TYPE_GOOMBA:
		if (dir & COLLISION_BOTTOM) {
			return 1;
		} else {
			if (player_vulnerable(p))
				player_downgrade(p);
			return 0;
		}
	case ITEM_TYPE_KOOPA:
		if (dir & COLLISION_BOTTOM && i->dir == 0) {
			return 1;
		} else if (dir & COLLISION_BOTTOM) {
			i->dir = 0;
			i->cnt = 0;
			return 0;
		} else {
			if (player_vulnerable(p))
				player_downgrade(p);
			return 0;
		}
	case ITEM_TYPE_MUSHROOM:
		player_upgrade_to(p, PLAYER_ST_BIG);
		return 1;
	case ITEM_TYPE_FLOWER:
		player_upgrade_to(p, PLAYER_ST_FLOWER);
		return 1;
	case ITEM_TYPE_COIN:
		player_add_coin(p);
		return 1;
	case ITEM_TYPE_TEMPCOIN:
		return 0;
	default:
		error_exit(ERR_OUT_OF_RANGE);
		return 0; // Execution will never reach this point.
	}
}


void item_render(Screen scr, Item i, int scroll)
{
	Picture pict;
	int x, y, w = item_width(i), h = item_height(i);

	if (item_enemies_pict == NULL)
		item_enemies_pict = screen_get_picture(ITEM_ENEMIES_FILE);
	if (item_upgrades_pict == NULL)
		item_upgrades_pict = screen_get_picture(ITEM_UPGRADES_FILE);

	switch (i->type) {
	case ITEM_TYPE_GOOMBA:
		pict = item_enemies_pict;
		x = ITEM_GOOMBA_X +
			((i->cnt / ITEM_ANIM_TICKS) % ITEM_GOOMBA_ANIM_NO)
			* ITEM_ANIM_X_SHIFT;
		y = ITEM_GOOMBA_Y;
		break;
	case ITEM_TYPE_KOOPA:
		pict = item_enemies_pict;
		if (i->dir == 0) {
			x = ITEM_KOOPA_GROUND_X;
			y = ITEM_KOOPA_GROUND_Y;
		} else {
			x = ITEM_KOOPA_X +
				((i->cnt / ITEM_ANIM_TICKS) %
					ITEM_KOOPA_ANIM_NO)
				* ITEM_ANIM_X_SHIFT;
			y = ITEM_KOOPA_Y;
		}
		break;
	case ITEM_TYPE_MUSHROOM:
		pict = item_upgrades_pict;
		x = ITEM_MUSHROOM_X;
		y = ITEM_MUSHROOM_Y;
		break;
	case ITEM_TYPE_FLOWER:
		pict = item_upgrades_pict;
		x = ITEM_FLOWER_X +
			((i->cnt / ITEM_ANIM_TICKS) % ITEM_FLOWER_ANIM_NO)
			* ITEM_ANIM_X_SHIFT;
		y = ITEM_FLOWER_Y;
		break;
	case ITEM_TYPE_COIN:
	case ITEM_TYPE_TEMPCOIN:
		pict = item_upgrades_pict;
		x = ITEM_COIN_X;
		y = ITEM_COIN_Y;
		break;
	default:
		error_exit(ERR_OUT_OF_RANGE);
		return; // Execution will never reach this point
	}

	screen_place(
		scr,
		pict,
		i->x - scroll,
		screen_height(scr) - i->y - h,
		x,
		y,
		w,
		h,
		i->dir == 1);
}
