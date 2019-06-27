/*
 * collision_type.h -- Define los tipos de colisión.
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
 * \file collision_type.h Define los tipos de colisión.
 */

#ifndef __COLLISION_TYPE_H
#define __COLLISION_TYPE_H

/** Representa un tipo de colisi&oacute;n. Este tipo funciona por m&aacute;scara
 * de bits, por lo que el tipo de colisi&oacute;n puede ser la
 * combinaci&oacute;n de varios tipos.
 */
typedef enum CollisionType {
	/** Indica que una colisi&oacute;n es por arriba. */
	COLLISION_TOP = 1,

	/** Indica que una colisi&oacute;n es por abajo. */
	COLLISION_BOTTOM = 2,

	/** Indica que una colisi&oacute;n es por la izquierda. */
	COLLISION_LEFT = 4,

	/** Indica que una colisi&oacute;n es por la derecha. */
	COLLISION_RIGHT = 8
} CollisionType;

#endif // __COLLISION_TYPE_H
