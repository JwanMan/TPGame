/*
 * player.h -- Implementa el personaje controlado por el jugador.
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
 * \file player.h Implementa el personaje controlado por el jugador.
 */

#ifndef __PLAYER_H
#define __PLAYER_H
#include "screen.h"

/** Ancho del personaje en p&iacute;xeles. */
#define PLAYER_WIDTH 16

/** Representa el estado del personaje. */
typedef enum PlayerState {
	/** Indica que el personaje est&aacute; en estado "peque&ntilde;o". */
	PLAYER_ST_SMALL = 1,

	/** Indica que el personaje est&aacute; en estado "grande". */
	PLAYER_ST_BIG = 2,

	/** Indica que el personaje est&aacute; en estado "flor". */
	PLAYER_ST_FLOWER = 3,

	/** Indica que el personaje est&aacute; muerto. */
	PLAYER_ST_DEAD = 0
} PlayerState;

/** Representa la posici&oacute;n relativa del personaje (de pie, saltando o
 * agachado).
 */
typedef enum PlayerPos {
	/** Indica que el personaje est&aacute; de pie. */
	PLAYER_POS_UPRIGHT,

	/** Indica que el personaje est&aacute; saltando. */
	PLAYER_POS_JUMP,

	/** Indica que el personaje est&aacute; agachado. */
	PLAYER_POS_GROUND
} PlayerPos;

/** Representa al personaje. */
typedef struct Player *Player;

/**
 * \brief Crea una instancia de personaje.
 * \param x La coordenada X de la posici&oacute;n de inicio.
 * \param y La coordenada Y de la posici&oacute;n de inicio.
 * \param lives El n&uacute;mero de vidas con el que empieza.
 * \return El personaje creado.
 * \remarks Al crearse, el personaje est&aacute; de pie, parado y mirando hacia
 * la derecha.
 */
Player player_create(int x, int y, int lives);

/**
 * \brief Libera una instancia de personaje.
 * \param p El personaje a liberar.
 */
void player_free(Player p);

/**
 * \brief Libera los recursos (im&acute;genes) alojados por este TDA de forma
 * global. Debe ser llamado antes de screen_end.
 */
void player_end();

/**
 * \brief Obtiene la coordenada X de la posici&oacute;n actual del personaje.
 * \param p El personaje.
 * \return La coordenada X del personaje.
 */
double player_x(Player p);

/**
 * \brief Establece la coordenada X de la posici&oacute;n del personaje.
 * \param p El personaje.
 * \param x El nuevo valor de la coordenada X.
 */
void player_set_x(Player p, double x);

/**
 * \brief Obtiene la coordenada Y de la posici&oacute;n actual del personaje.
 * \param p El personaje.
 * \return La coordenada Y del personaje.
 */
double player_y(Player p);

/**
 * \brief Establece la coordenada Y de la posici&oacute;n del personaje.
 * \param p El personaje.
 * \param y El nuevo valor de la coordenada Y.
 */
void player_set_y(Player p, double y);

/**
 * \brief Obtiene la velocidad horizontal del personaje.
 * \param p El personaje.
 * \return La componente horizontal de la velocidad actual del personaje.
 */
double player_vx(Player p);

/**
 * \brief Obtiene la velocidad vertical del personaje.
 * \param p El personaje.
 * \return La componente vertical de la velocidad actual del personaje.
 */
double player_vy(Player p);

/**
 * \brief Obtiene el estado actual del personaje.
 * \param p El personaje.
 * \return El estado del personaje. Puede ser PLAYER_ST_SMALL, PLAYER_ST_BIG o
 * PLAYER_ST_FLOWER.
 */
PlayerState player_state(Player p);

/**
 * \brief Obtiene el n&uacute;mero de monedas recogidas por el personaje.
 * \param p El personaje.
 * \return El n&uacute;mero de monedas que tiene.
 */
int player_coins(Player p);

/**
 * \brief Obtiene el n&uacute;mero de vidas que le quedan al personaje,
 * incluyendo la que est&eacute; siendo usada en el momento actual.
 */
int player_lives(Player p);

/**
 * \brief Obtiene la posici&oacute;n (postura) actual del personaje.
 * \param p El personaje.
 * \return La posici&oacute;n del personale. Puede ser PLAYER_POS_UPRIGHT,
 * PLAYER_POS_JUMP o PLAYER_POS_GROUND.
 */
PlayerPos player_pos(Player p);

/**
 * \brief Obtiene la altura del personaje.
 * \param p El personaje.
 * \return La altura actual del personaje en p&iacute;xeles.
 */
int player_height(Player p);

/**
 * \brief Establece la posici&oacute;n actual del personaje.
 * \param p El personaje.
 * \param pos La nueva posici&oacute;n del personaje. Puede ser
 * PLAYER_POS_UPRIGHT, PLAYER_POS_JUMP o PLAYER_POS_GROUND.
 */
void player_set_pos(Player p, PlayerPos pos);

/**
 * \brief Actualiza la posici&oacute;n del personaje por un frame.
 * \param p El personaje.
 * \remarks La funci&oacute;n act&uacute;a como si no hubiera nada alrededor del
 * personaje (cae). Para evitarlo, se debe llamar posteriormente a
 * player_correct_on_collision.
 */
void player_update(Player p);

/**
 * \brief Comprueba si el personaje es vulnerable a da&ntilde;o por parte de
 * enemigos (como Goombas y Koopas). El personaje se vuelve invulnerable tras
 * chocar con uno y bajar de estado, durante un corto periodo de tiempo, y
 * no se deber&iacute;a llamar a player_downgrade si esta funci&oacute;n
 * devuelve 0.
 * \param p El personaje.
 * \return Devuelve 0 si el personaje es invulnerable, u otro valor en caso
 * contrario.
 */
int player_vulnerable(Player p);

/**
 * \brief Corrige la posici&oacute;n del personaje despu&eacute;s de llamar
 * a player_update en caso de haber una colisi&oacute;n.
 * \param p El personaje.
 * \param dir El tipo de colisi&oacute;n por el que corregir la posici&oacute;n.
 * Esta par&acute;metro puede valer COLLISION_TOP, COLLISION_BOTTOM,
 * COLLISION_LEFT o COLLISION_RIGHT (definidos en collision.h), o el resultado
 * de hacer la disyunci&oacute;n l&oacute;gica bit a bit (operador '|') para
 * corregir varios tipos de colisi&oacute;n en la misma llamada a la
 * funci&oacute;n.
 */
void player_correct_on_collision(Player p, int dir);

/**
 * \brief Acelera al personaje a la izquierda o a la derecha seg&uacute;n
 * valores fijos para la aceleraci&oacute;n y la velocidad m&aacute;xima.
 * \param p El personaje.
 * \param left Si es distinto de 0, la aceleraci&oacute;n ser&aacute; hacia la
 * izquierda. De lo contrario ser&aacute; hacia la derecha.
 * \remarks Si el personaje est&aacute; avanzando en direcci&oacute;n contraria,
 * frena en seco antes de acelerar en esta direcci&oacute;n.
 */
void player_accel(Player p, int left);

/**
 * \brief Hace saltar al personaje.
 * \param p El personaje.
 */
void player_jump(Player p);

/**
 * \brief Establece a cero la componente horizontal de la velocidad del
 * personaje, salvo si este est&aacute; saltando.
 * \param p El personaje.
 */
void player_stop(Player p);

/**
 * \brief Dibuja al personaje en la posici&oacute;n de la pantalla que le
 * corresponde.
 * \param scr La ventana donde dibujar al personaje.
 * \param p El personaje.
 * \param scroll El scroll, la coordenada X de la parte izquierda de la ventana
 * respecto de la parte izquierda del mundo.
 */
void player_render(Screen scr, Player p, int scroll);

/**
 * \brief Pasa al personaje al estado inferior al actual.
 * \param p El personaje.
 * \remarks Pasa de estado "flor" a "grande", de "grande" a "peque&ntilde;o" y
 * de "peque&ntilde;o" a "muerto".
 */
void player_downgrade(Player p);

/**
 * \brief Pasa al personaje a un estado dado si est&aacute; en un estado
 * inferior.
 * \param p El personaje.
 * \param state El nuevo estado, entre PLAYER_ST_DEAD, PLAYER_ST_SMALL,
 * PLAYER_ST_BIG o PLAYER_ST_FLOWER.
 */
void player_upgrade_to(Player p, int state);

/**
 * \brief "Mata" a un personaje (cambia su estado a "muerto").
 * \param p El personaje.
 */
void player_die(Player p);

/**
 * \brief A&ntilde;ade una moneda al contador de monedas recogidas por el
 * personaje.
 * \param p El personaje.
 * \remarks Si se llega a un n&uacute;mero de monedas m&uacute;ltiplo de 100,
 * el personaje gana una vida extra.
 */
void player_add_coin(Player p);

/**
 * \brief Resetea el personaje para empezar un mundo de nuevo.
 * \param p El personaje.
 * \return Un n&uacute;mero distinto de 0 si quedan vidas como para empezar una
 * nueva partida. De lo contrario devuelve 0.
 * \remarks Al resetear, el personaje queda parado, mirando hacia la derecha.
 * Si el personaje estaba en estado "muerto", se resetean las monedas y el
 * personaje pasa a estado "peque&ntilde;o", y de lo contrario el personaje
 * conserva su estado. Esta funci&oacute;n tambi&eacute;n disminuye el
 * n&uacute;mero de vidas en 1. Sin embargo, el personaje queda en las mismas
 * coordenadas del mundo que ten&iacute;a antes.
 */
int player_restart(Player p);

#endif // __PLAYER_H

