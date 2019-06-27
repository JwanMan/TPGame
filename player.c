/*
 * player.c -- Implementa el personaje controlado por el jugador.
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
#include "collision_type.h"
#include "block.h"
#include "error.h"
#include "player.h"
#include "screen.h"

#define PLAYER_GRAVITY 0.15
#define PLAYER_ACCEL 0.2
#define PLAYER_ACCEL_FLYING 0.02
#define PLAYER_VELOCITY_JUMP 4.5
#define PLAYER_MAX_VELOCITY 1.8
#define PLAYER_ANIM_TICKS 5
#define PLAYER_ANIM_NO 3
#define PLAYER_PICT_FILE "50365.bmp"
#define PLAYER_SMALL_Y 34
#define PLAYER_SMALL_H 16
#define PLAYER_BIG_Y 1
#define PLAYER_BIG_H 32
#define PLAYER_FLOWER_Y 129
#define PLAYER_SHIFT_X 17
#define PLAYER_DEAD_X 182
#define PLAYER_LOOK_X 80
#define PLAYER_JUMP_X 165
#define PLAYER_GROUND_X 182
#define PLAYER_WALK_X 97
#define PLAYER_VULNERABLE_TICKS 60

struct Player {
	double x;
	double y;
	double vx;
	double vy;
	PlayerState state;
	PlayerPos pos;
	int left;
	int counter;
	int coins;
	int lives;
	int vuln;
};

Picture player_pict = NULL;

Player player_create(int x, int y, int lives)
{
	Player p = malloc(sizeof(struct Player));
	if (p == NULL)
		error_libc_exit();

	p->x = x;
	p->y = y;
	p->vx = 0;
	p->vy = 0;
	p->state = PLAYER_ST_SMALL;
	p->pos = PLAYER_POS_UPRIGHT;
	p->counter = 0;
	p->left = 0;
	p->coins = 0;
	p->lives = lives;
	p->vuln = PLAYER_VULNERABLE_TICKS;

	return p;
}

void player_free(Player p)
{
	if(p == NULL)
		error_exit(ERR_NULL_PARAM);
	free(p);
}

void player_end()
{
	if (player_pict != NULL) {
		screen_free_picture(player_pict);
		player_pict = NULL;
	}
}

double player_x(Player p)
{
	return p->x;
}

void player_set_x(Player p, double x)
{
	p->x = x;
}

double player_y(Player p)
{
	return p->y;
}

void player_set_y(Player p, double y)
{
	p->y = y;
}

double player_vx(Player p)
{
	return p->vx;
}

double player_vy(Player p)
{
	return p->vy;
}

PlayerState player_state(Player p)
{
	return p->state;
}

PlayerPos player_pos(Player p)
{
	return p->pos;
}

void player_set_pos(Player p, PlayerPos pos)
{
	if (pos == PLAYER_POS_JUMP && p->pos != PLAYER_POS_JUMP)
		p->vy = PLAYER_VELOCITY_JUMP;
	p->pos = pos;
}

int player_coins(Player p)
{
	return p->coins;
}

int player_lives(Player p)
{
	return p->lives;
}

void player_update(Player p)
{
	p->counter++;
	p->vuln++;
	p->vy -= PLAYER_GRAVITY;
	p->x += p->vx;
	p->y += p->vy;
}

int player_vulnerable(Player p)
{
	return p->vuln >= PLAYER_VULNERABLE_TICKS;
}

void player_correct_on_collision(Player p, int dir)
{
	if (dir & COLLISION_BOTTOM) {
		p->vy = 0;
		p->y = ((int)(p->y + BLOCK_SIZE) / BLOCK_SIZE) * BLOCK_SIZE;
		if (dir & COLLISION_TOP)
			p->pos = PLAYER_POS_GROUND;
		else
			p->pos = PLAYER_POS_UPRIGHT;
	} else if (dir & COLLISION_TOP) {
		p->vy = 0;
		p->y = p->y - 2 * (((int)p->y) % BLOCK_SIZE);
	}
	if (dir & COLLISION_LEFT) {
		p->vx = 0;
		p->x = (((int)(p->x + BLOCK_SIZE)) / BLOCK_SIZE) * BLOCK_SIZE;
	}
	if (dir & COLLISION_RIGHT) {
		p->vx = 0;
		p->x = (((int)(p->x)) / BLOCK_SIZE) * BLOCK_SIZE;
	}
}

void player_accel(Player p, int left)
{
	double amount = p->pos == PLAYER_POS_JUMP && p->vy < 0 ?
				PLAYER_ACCEL_FLYING : PLAYER_ACCEL;
	if (left)
		p->vx = (p->vx > 0 ? -amount : p->vx - amount);
	else
		p->vx = (p->vx < 0 ? amount : p->vx + amount);

	if (p->vx > PLAYER_MAX_VELOCITY)
		p->vx = PLAYER_MAX_VELOCITY;
	else if (p->vx < -PLAYER_MAX_VELOCITY)
		p->vx = -PLAYER_MAX_VELOCITY;
}

void player_jump(Player p)
{
	if (p->pos != PLAYER_POS_JUMP) {
		p->vy = PLAYER_VELOCITY_JUMP;
		p->pos = PLAYER_POS_JUMP;
	}
}


void player_stop(Player p)
{
	if (p->pos != PLAYER_POS_JUMP)
		p->vx = 0;
}

int player_pict_y(Player p)
{
	switch (p->state) {
	case PLAYER_ST_SMALL:
	case PLAYER_ST_DEAD:
		return PLAYER_SMALL_Y;
	case PLAYER_ST_BIG:
		return PLAYER_BIG_Y;
	case PLAYER_ST_FLOWER:
		return PLAYER_FLOWER_Y;
	default:
		error_exit(ERR_OUT_OF_RANGE);
		return 0; // Execution will never reach this point.
	}
}

int player_height(Player p) {
	return p->state == PLAYER_ST_SMALL || p->state == PLAYER_ST_DEAD ?
		PLAYER_SMALL_H : PLAYER_BIG_H;
}

int player_pict_x(Player p)
{
	if (p->state == PLAYER_ST_DEAD)
		return PLAYER_DEAD_X;
	else if (p->pos == PLAYER_POS_JUMP)
		return PLAYER_JUMP_X;
	else if (p->pos == PLAYER_POS_GROUND &&
			p->state != PLAYER_ST_SMALL)
		return PLAYER_GROUND_X;
	else if (p->vx == 0)
		return PLAYER_LOOK_X;
	else // Walking
		return PLAYER_WALK_X +
			((p->counter / PLAYER_ANIM_TICKS) % PLAYER_ANIM_NO)
			* PLAYER_SHIFT_X;
}

void player_render(Screen scr, Player p, int scroll)
{
	int h = player_height(p);

	if (p->vx < 0)
		p->left = 1;
	else if (p->vx > 0)
		p->left = 0;

	if (player_pict == NULL)
		player_pict = screen_get_picture(PLAYER_PICT_FILE);

	screen_place(
		scr,
		player_pict,
		p->x - scroll,
		screen_height(scr) - p->y - h,
		player_pict_x(p),
		player_pict_y(p),
		PLAYER_WIDTH,
		h,
		p->left);
}

void player_downgrade(Player p)
{
	if (p->state != PLAYER_ST_DEAD)
		p->state--;
	p->vuln = 0;
}

void player_upgrade_to(Player p, int state)
{
	if (p->state > PLAYER_ST_FLOWER)
		error_exit(ERR_OUT_OF_RANGE);
	if (p->state < state)
		p->state = state;
}

void player_die(Player p)
{
	p->state = PLAYER_ST_DEAD;
}

void player_add_coin(Player p)
{
	if (++p->coins % 100 == 0)
		++p->lives;
}


int player_restart(Player p)
{
	p->vx = 0;
	p->vy = 0;
	p->pos = PLAYER_POS_UPRIGHT;
	p->left = 0;
	p->counter = 0;
	p->vuln = PLAYER_VULNERABLE_TICKS;

	if (p->state == PLAYER_ST_DEAD) {
		player_upgrade_to(p, PLAYER_ST_SMALL);
		p->coins = 0;
	}

	return --p->lives > 0;
}
