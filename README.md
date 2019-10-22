# A Super Mario-like game

This is a GPL3-licensed game I've created as a Computer Engineering class
project. Image assets are Copyright (c) 1985 Nintendo Entertainment Inc., and
have been obtained from <https://www.spriters-resource.com>.

## Installing on Linux

This games requires LibSDL2 to be installed. Version 2.0.0 does not work,
but 2.0.8 does. You'll also need `gcc` and `make`. You will most likely need to
indicate the path of the SDL library to `make` with the `SDLLIB` environment 
variable.

Once these requierements are met, you simply type `make` and the game is
built to `./main`.

Note: In Debian-based distributions, you run `apt install libsdl2-dev` and then
use `apt-file libsdl2-2.0-0` (or whatever), having installed `apt-file`, to
get the location of the shared library.

## Installing on Windows

You need MinGW. You'll also need to install SDL2 on MinGW (once you compile the
game, you'll only need the DLL file).

In order to install, you have to options:

* Type `make windows`.

* Open the file `Game.cbp` with CodeBlocks and compile from there. You still
  need to generate the BMP files, which are not handled by CodeBlocks.

## Notes

The documentation for Doxygen (in the header files) is in Spanish as this
was required. The instructions screen is in Spanish as well.

The default level is `1w`, and the format is the one being parsed at `world.c`.
Bugs likely remain. Contributions are welcome, just keep in mind that the main
purpose of this repository is educational.
