/*
 * block_list.c -- Implementa las listas de bloques.
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
#include "error.h"
#include "block.h"
#include "block_list.h"
#include "screen.h"

struct BlockNode {
	Block e;
	BlockNode nx;
};

struct BlockList {
	BlockNode first;
	BlockNode last;
};

BlockList block_list_create()
{
	BlockList bl;
	BlockNode bn;

	if ((bl = malloc(sizeof(struct BlockList))) == NULL)
		error_libc_exit();

	if ((bn = malloc(sizeof(struct BlockNode))) == NULL)
		error_libc_exit();

	bn->e = NULL;
	bn->nx = NULL;

	bl->first = bn;
	bl->last = bn;

	return bl;
}

void block_list_free(BlockList bl)
{
	BlockNode bn = bl->first->nx, nx;

	while (bn != NULL) {
		nx = bn->nx;
		block_free(bn->e);
		free(bn);
		bn = nx;
	}

	free(bl);
}

BlockNode block_list_begin(BlockList bl)
{
	return bl->first;
}

Block block_list_current(BlockNode bn)
{
	return bn->nx->e;
}

BlockNode block_list_next(BlockNode bn)
{
	return bn->nx;
}

void block_list_delete(BlockNode bn)
{
	BlockNode nx = bn->nx->nx;
	block_free(bn->nx->e);
	free(bn->nx);
	bn->nx = nx;
}

void block_list_delete_preserving(BlockNode bn)
{
	BlockNode nx = bn->nx->nx;
	free(bn->nx);
	bn->nx = nx;
}

int block_list_isend(BlockNode bn)
{
	return bn->nx == NULL;
}

int block_list_isempty(BlockList bl)
{
	return block_list_isend(block_list_begin(bl));
}

void block_list_append(BlockList bl, Block e)
{
	BlockNode bn;

	if (bl == NULL || e == NULL)
		error_exit(ERR_NULL_PARAM);

	bn = malloc(sizeof(struct BlockNode));
	bn->e = e;
	bn->nx = NULL;
	bl->last->nx = bn;
	bl->last = bn;
}

BlockList block_list_sort(BlockList bl)
{
	BlockList dst = block_list_create();
	BlockNode src;
	Block b;
	int i = 0, nx = -1;

	while (!block_list_isempty(bl)) {
		src = block_list_begin(bl);
		while (!block_list_isend(src)) {
			b = block_list_current(src);
			if (block_x(b) == i) {
				block_list_append(dst, b);
				block_list_delete_preserving(src);
			} else {
				nx = nx == -1 || block_x(b) < nx ?
					block_x(b) : nx;
				src = block_list_next(src);
			}
		}
		i = nx;
		nx = -1;
	}

	block_list_free(bl);
	return dst;
}

void block_list_render(Screen scr, BlockList bl, int scroll)
{
	BlockNode bn = block_list_begin(bl);
	Block b;
	int w = screen_width(scr);

	while (!block_list_isend(bn) &&
		(block_x(block_list_current(bn)) + 1) * BLOCK_SIZE <= scroll)
		block_list_delete(bn);

	while (!block_list_isend(bn) &&
		block_x(b = block_list_current(bn)) * BLOCK_SIZE < scroll + w) {
		block_render(scr, b, scroll);
		bn = block_list_next(bn);
	}
}
