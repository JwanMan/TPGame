/*
 * screen.h -- Abstracción sobre SDL para la entrada y salida gráfica.
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
 * \file screen.h Abstracción sobre SDL para la entrada y salida gráfica.
 */

#ifndef __SCREEN_H
#define __SCREEN_H

#include <SDL2/SDL.h>

/** Representa una imagen. */
typedef struct Picture *Picture;

/**
 * Representa una ventana de tama&ntilde;o fijo sobre la que podemos dibujar.
 */
typedef struct Screen *Screen;

/** Representa el inicio de una pulsaci&oacute;n de tecla. Usado en
 *  screen_get_keyboard_event.
 */
#define SCREEN_KEYDOWN 1

/** Representa el final de una pulsaci&oacute;n de tecla. Usado en
 *  screen_get_keyboard_event.
 */
#define SCREEN_KEYUP 2

/** Representa una imagen. */
typedef struct Picture *Picture;

/** Representa una ventana de tama&ntilde;o fijo sobre la que podemos dibujar.
 */
typedef struct Screen *Screen;

/**
 * \brief Crea una ventana con la que el usuario puede interactuar.
 * \param caption El t&iacute;tulo de la ventana.
 * \param w Ancho de la ventana.
 * \param h Alto de la ventana.
 * \return La instancia del TDA Screen para manejar la ventana.
 */
Screen screen_create(const char *caption, int w, int h);

/**
 * \brief Libera un objeto Screen y cierra la ventana asociada.
 * \param scr El objeto Screen a liberar.
 */
void screen_free(Screen scr);

/**
 * \brief Libera todos los recursos de SDL, que fueron alojados al llamar por
 * primera vez a screen_create.
 */
void screen_end();

/**
 * \brief Devuelve el ancho de la ventana.
 * \param scr La ventana en cuesti&oacute;n.
 * \return El ancho de la ventana.
 */
int screen_width(Screen scr);

/**
 * \brief Devuelve el alto de la ventana.
 * \param src La ventana en cuesti&oactue;n.
 * \return El alto de la ventana.
 */
int screen_height(Screen scr);

/**
 * \brief Pausa el programa durante una cantidad de tiempo determinada.
 * \param ms La cantidad de tiempo a esperar.
 */
void screen_wait(int ms);

/**
 * \brief Actualiza el contenido de la ventana. Esta funci&oactue;n debe ser
 * llamada una vez se ha terminado de dibujar en el objeto Screen para que los
 * resultados sean visibles.
 * \param scr La ventana en cuesti&oacute;n.
 */
void screen_update(Screen scr);

/**
 * \brief Comprueba si se ha pulsado (o dejado de pulsar) una tecla del
 * teclado.
 * \param sc Si es no nulo, se guarda qu&eacute; tecla se ha pulsado.
 * \return SCREEN_KEYDOWN si se ha pulsado una tecla, SCREEN_KEYUP si se
 * ha dejado de pulsar o 0 si no ha ocurrido ninguna de las dos cosas.
 * \remarks Esta funci&oacute;n consume (e ignora) el resto de eventos que
 * puedan ocurrir hasta dicha pulsaci&oacute;n de tecla, salvo la
 * notificaci&oacute;n de cierre de ventana, que termina la aplicaci&oacute;n.
 */
int screen_get_keyboard_event(SDL_Scancode *sc);

/**
 * \brief Comprueba si una tecla del teclado est&aacute; siendo pulsada.
 * \param scancode El c&oacute;digo de la tecla en cuesti&oacute;n, v&eacute;ase
 * <a href="https://wiki.libsdl.org/SDL_Scancode">SDL_Scancode</a> en la
 * documentaci&oacute;n de SDL para m&aacute;s informaci&oacute;n.
 */
int screen_key_pressed(SDL_Scancode scancode);

/**
 * \brief Llena una ventana con un color dado.
 * \param scr La ventana en cuesti&oacute;n.
 * \param r El componente rojo del color, de 0 a 255.
 * \param g El componente verde del color, de 0 a 255.
 * \param b El componente azul del color, de 0 a 255.
 * \param a El nivel de opacidad, de 0 a 255.
 */
void screen_fill(Screen scr, int r, int g, int b, int a);

/**
 * \brief Copia una imagen (o un trozo de una) en una ventana.
 * \param scr La ventana en cuesti&oacute;n.
 * \param pict La imagenn en cuesti&oacute;n.
 * \param x La coordenada X en la pantalla donde situar la esquina superior
 * izquierda del trozo a copiar.
 * \param y La coordenada Y en la pantalla donde situar la esquina superior
 * izquierda del trozo a copiar.
 * \param sx La coordenada X en la imagen de la esquina superior izquierda de
 * lo que se ha de copiar.
 * \param sy La coordenada Y en la imagen de la esquina superior izquierda de
 * lo que se ha de copiar.
 * \param w El ancho del trozo de la imagen a copiar.
 * \param h El alto del trozo de la imagen a copiar.
 * \param flipped Si es distinto de 0, la imagen se copia "en espejo" sobre el
 * eje horizontal. De lo contrario se copia "normal".
 * \remarks Si el cuadrado donde se ha de copiar la imagen queda fuera del
 * &aacute;rea visible, la imagen no se muestra. Parte de la imagen puede salir
 * del &aacute;rea visible, y entonces se copia solo la parte visible.
 */
void screen_place(Screen scr, Picture pict, int x, int y, int sx, int sy,
			int w, int h, int flipped);

/**
 * \brief Carga una imagen desde un fichero.
 * \param file La ruta del fichero.
 * \return La imagen le&iacute;da.
 * \remarks El fichero debe ser una imagen BMP no comprimida.
 */
Picture screen_get_picture(const char *file);

/**
 * \brief Libera la memoria ocupada por una imagen.
 * \param La imagen en cuesti&oacute;n.
 */
void screen_free_picture(Picture pict);

/**
 * \brief Imprime un texto en pantalla.
 * \param scr La ventana en la que imprimir el texto.
 * \param x La coordenada X en la pantalla donde ir&aacute; la esquina superior
 * izquierda del texto.
 * \param y La coordenada Y en la pantalla donde ir&aacute; la esquina superior
 * izquierda del texto.
 * \param w El ancho en n&uacute;mero de caracteres del recuadro donde
 * ir&aacute; el texto. Si el texto es m&aacute;s largo que esto, se
 * ocupar&aacute;n m&aacute;s l&iacute;neas en la pantalla seg&uacute;n sea
 * necesario. Si este par&aacute;metro es 0, se escribir&aacute; todo en la
 * misma l&iacute;nea.
 * \param str El texto a imprimir.
 * \remarks El texto puede salir del &aacute;rea visible de la pantalla. El
 * caracter . ser&aacute; sustituido por el signo de copyright. Algunos
 * caracteres no est&aacute;n soportados.
 */
void screen_text_print(Screen scr, int x, int y, int w, const char *str);

/**
 * \brief Imprime un texto en pantalla, centrado en una cierta &aacute;rea.
 * \param scr La ventana en la que imprimir el texto.
 * \param x La coordenada horizontal en la que centrar el texto.
 * \param y La coordenada vertical de lo que ser&aacute; el borde superior del
 * texto en la ventana.
 * \param str El texto a imprimir.
 * \remarks El texto puede salir del &aacute;rea visible de la pantalla. El
 * caracter . ser&aacute; sustituido por el signo de copyright. Algunos
 * caracteres no est&aacute;n soportados.
 */
void screen_text_centered(Screen scr, int x, int y, const char *str);
#endif // __SCREEN_H
