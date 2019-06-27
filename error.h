/*
 * error.h -- Implementa la gestión de errores del programa.
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
 * \file error.h Implementa la gestión de errores del programa.
 */

#ifndef __ERROR_H
#define __ERROR_H

/** Representa un tipo de error propio del programa (no de SDL ni de la
 * biblioteca est&aacute;ndar de C.
 */
typedef enum ErrorType {
	/** El c&oacute;digo de escaneo de tecla especificado no es
	 *  v&aacute;lido.
	 */
	ERR_INVALID_SCANCODE = 0,

	/** Se ha pasado un puntero nulo a una funci&oacute;n incorrectamente.
	 */
	ERR_NULL_PARAM = 1,

	/** Un par&aacute;metro de funci&oacute;n est&aacute; fuera de rango. */
	ERR_OUT_OF_RANGE = 2,

	/** El fichero de entrada no es v&aacute;lido o se ha producido un error
	 *  de lectura.
	 */
	ERR_INVALID_INPUT = 3
} ErrorType;

/**
 * \brief Informa de un error del propio programa y lo finaliza.
 * \param type El tipo de error.
 */
void error_exit(ErrorType type);

/**
 * \brief Informa del &uacute;ltimo error que ha tenido lugar en la biblioteca
 * est&acute;ndar de C y finaliza el programa.
 */
void error_libc_exit();

/**
 * \brief Informa del &uactute;ltimo error que ha tenido lugar en la
 * biblioteca SDL y finaliza el programa.
 */
void error_sdl_exit();

#endif // __ERROR_H
