#include <SDL/SDL.h>
#include "graphics.h"

SDL_Surface *screen;
int SCREENWIDTH,SCREENHEIGHT,VGA;
unsigned long stcolors[16] = {
    0x000000, 0x0000AA, 0x00AA00, 0x00AAAA,
    0xAA0000, 0xAA00AA, 0xAA5500, 0xAAAAAA,
    0x555555, 0x5555FF, 0x55FF55, 0x55FFFF,
    0xFF5555, 0xFF55FF, 0xFFFF55, 0xFFFFFF
}; unsigned short colors;

int graphics_open(int mode) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Couldn't initialize SDL: %s\n",
                    SDL_GetError());
        return -1;
    }

    switch(mode) {
        case VGA_640x480x16:
            SCREENWIDTH =640;
            SCREENHEIGHT=350;
            colors = 16;
            screen = SDL_SetVideoMode(640, 480, 8, SDL_SWSURFACE);
            break;
        case VGA_640x350x16:
            SCREENWIDTH =640;
            SCREENHEIGHT=350;
            colors = 16;
            screen = SDL_SetVideoMode(640, 350, 8, SDL_SWSURFACE);
            break;
        default:
            fprintf(stderr, "Unknown video mode '%d'.\n", mode);
            return -1;
            break;
    }

    if (screen == NULL) {
        fprintf(stderr, "Couldn't set a %dx%d video mode: %s\n", SCREENWIDTH, SCREENHEIGHT, SDL_GetError());
        return -1;
    }

    return 0;
}

void graphics_close(void) {
    SDL_FreeSurface(screen);
}

void __putpixel(int x, int y, Uint32 pixel) {
    int bpp = screen->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

Uint32 __hex2rgb(unsigned long color) {
    Uint8 r,g,b;
    r = (color >> 16) & 0xFF;
    g = (color >>  8) & 0xFF;
    b = color & 0xFF;
    return SDL_MapRGB(screen->format, r, g, b);
}

void drawpixel(int x, int y, int color) {
    if (SDL_MUSTLOCK(screen)) {
        if (SDL_LockSurface(screen) < 0) {
            fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
            return;
        }
    }

    if (colors == 16)
        __putpixel(x, y, __hex2rgb(stcolors[color]));

    if (SDL_MUSTLOCK(screen))
        SDL_UnlockSurface(screen);

    SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void _drawpixel_fast(int x, int y, int color) {
    if (SDL_MUSTLOCK(screen)) {
        if (SDL_LockSurface(screen) < 0) {
            fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
            return;
        }
    }

    if (colors == 16)
        __putpixel(x, y, __hex2rgb(stcolors[color]));

    if (SDL_MUSTLOCK(screen))
        SDL_UnlockSurface(screen);
}

void _drawhline_fast(int x1, int x2, int y, int c) {
    while (x1 <= x2) _drawpixel_fast(x1++, y, c);
}

void fill_rect(int x1, int y1, int x2, int y2, int c) {
    while (y1 <= y2) _drawhline_fast(x1, x2, y1++, c);
    SDL_UpdateRect(screen, 0, 0, 0, 0);
}