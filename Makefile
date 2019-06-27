# Makefile for the project. The commented parts show how the images have been
# obtained via wget and ImageMagick.
CFLAGS=-Os -Wall -pedantic
OFILES=main.o screen.o error.o player.o item.o collision.o item_list.o block.o block_list.o world.o
LINUX_EXEC=main
WINDOWS_EXEC=main.exe
SDLLIB ?= /usr/lib/x86_64-linux-gnu/libSDL2.so
WINLIB=libSDL2.dll
#BMPFILES=50365.bmp 52570.bmp 52569.bmp 52571.bmp 56929.bmp
#PNGFILES=50365.png 52570.png 52569.png 52571.png 56929.png

.PHONY=all windows base clean

all: base $(LINUX_EXEC)

windows: base $(WINDOWS_EXEC) libSDL2.dll

base: #$(BMPFILES)

$(LINUX_EXEC): $(OFILES)
	gcc -o $(LINUX_EXEC) $(CFLAGS) $(SDLLIB) $(OFILES)

$(WINDOWS_EXEC): $(OFILES)
	gcc -o $(WINDOWS_EXEC) $(CFLAGS) $(WINLIB) $(OFILES)

%.o : %.c
	gcc -c $(CFLAGS) $<

clean:
	rm -f $(OFILES) $(LINUX_EXEC) $(WINDOWS_EXEC) #$(BMPFILES) $(PNGFILES)

#50365.png:
#	wget https://www.spriters-resource.com/resources/sheets/47/50365.png
#
#52570.png:
#	wget https://www.spriters-resource.com/resources/sheets/49/52570.png
#
#52569.png:
#	wget https://www.spriters-resource.com/resources/sheets/49/52569.png
#
#52571.png:
#	wget https://www.spriters-resource.com/resources/sheets/49/52571.png
#
#56929.png:
#	wget https://www.spriters-resource.com/resources/sheets/54/56929.png
#
#56929.bmp: 56929.png
#	convert 56929.png -transparent "#5c94fc" 56929.bmp
#
#%.bmp: %.png
#	convert $< $@

