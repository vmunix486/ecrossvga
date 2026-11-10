PLAT=sdl12

CC=gcc
AR=ar
CFLAGS=-O3 -Wall -Wextra -ansi -pedantic
ifeq ($(PLAT), sdl12)
	CFLAGS += $(shell sdl-config --cflags)
endif
ifeq ($(PLAT), sdl12)
	LDFLAGS=$(shell sdl-config --libs)
endif

all: plat_$(PLAT) strtoi
	$(AR) rcs build/libecrossvga.a build/plat_$(PLAT).o build/strtoi.o

plat_sdl12: src/plat_sdl12.c
	$(CC) $(CFLAGS) -c src/plat_sdl12.c -o build/plat_sdl12.o

strtoi: src/strtoi.c 
	$(CC) $(CFLAGS) -c src/strtoi.c -o build/strtoi.o 

clean:
	rm -fv build/*

examples: vgatest

vgatest: examples/vgatest.c
	$(CC) $(CFLAGS) -Isrc examples/vgatest.c -o build/vgatest -Lbuild -lecrossvga $(LDFLAGS)