#include <SDL/SDL.h>
#include "graphics.h"

SDL_Surface *screen;
int SCREENWIDTH,SCREENHEIGHT,VGA;
unsigned short colors,fast,grey,palette;

unsigned int bwcolors[2] = {
	0x000000, 0xFFFFFF
};

unsigned int cgacolors[4][4] = {
    { 0x00000, 0x00AA00, 0xAA0000, 0xAA5500 }, /* Palette 0, Low intensity  */
    { 0x00000, 0x55FF55, 0xFF5555, 0xFFFF55 }, /* Palette 0, High intensity */
    { 0x00000, 0x00AAAA, 0xAA00AA, 0xAAAAAA }, /* Palette 1, Low intensity  */
    { 0x00000, 0x55FFFF, 0xFF55FF, 0xFFFFFF }  /* Palette 1, High intensity */
};

unsigned int fsgcolors[4] = { /* CGA 4 shades of grey */
    0x000000, 0x555555,
    0xAAAAAA, 0xFFFFFF
};

unsigned int stcolors[16] = {
    0x000000, 0x0000AA, 0x00AA00, 0x00AAAA,
    0xAA0000, 0xAA00AA, 0xAA5500, 0xAAAAAA,
    0x555555, 0x5555FF, 0x55FF55, 0x55FFFF,
    0xFF5555, 0xFF55FF, 0xFFFF55, 0xFFFFFF
};

int graphics_open(int mode) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Couldn't initialize SDL: %s\n",
                    SDL_GetError());
        return -1;
    }

    switch(mode) {
        case CGA_320x200x4:
            SCREENWIDTH =320;
            SCREENHEIGHT=200;
            colors = 4; palette = 1;
            screen = SDL_SetVideoMode(320, 200, 8, SDL_SWSURFACE);
            break;
        case CGA_320x200x4G:
            SCREENWIDTH =320;
            SCREENHEIGHT=200;
            colors = 4; grey = 1;
            screen = SDL_SetVideoMode(320, 200, 8, SDL_SWSURFACE);
            break;
        case CGA_640x200x2:
            SCREENWIDTH =640;
            SCREENHEIGHT=200;
            colors = 2;
            screen = SDL_SetVideoMode(640, 200, 8, SDL_SWSURFACE);
            break;
        case VGA_640x480x16:
            SCREENWIDTH =640;
            SCREENHEIGHT=480;
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

    SDL_WM_SetCaption("ecrossvga", NULL);

    return 0;
}

void graphics_close(void){SDL_FreeSurface(screen);}
void vga_init(void){graphics_open(VGA_640x480x16);}

/* Modified from here:
 * https://www.libsdl.org/release/SDL-1.2.15/docs/html/guidevideo.html#AEN93 
 * -vmunix */
void _putpixel(int x, int y, Uint32 pixel) {
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

Uint32 _hex2rgb(unsigned long color) {
    Uint8 r,g,b;
    r = (color >> 16) & 0xFF;
    g = (color >>  8) & 0xFF;
    b = color & 0xFF;
    return SDL_MapRGB(screen->format, r, g, b);
}

void _drawpixel(int x, int y, int color, int f) {
    if (SDL_MUSTLOCK(screen)) {
        if (SDL_LockSurface(screen) < 0) {
            fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
            return;
        }
    }

    switch (colors) {
	    case 2:
		    _putpixel(x, y, _hex2rgb(bwcolors[color%2])); break;
        case 4:
            if (grey == 1) {
                 _putpixel(x, y, _hex2rgb(fsgcolors[color%4]));
                 break;
            } else {
                _putpixel(x, y, _hex2rgb(cgacolors[palette][color%4])); 
                break;
            }
	    case 16:
		    _putpixel(x, y, _hex2rgb(stcolors[color%16])); break;
    }

    if (SDL_MUSTLOCK(screen))
        SDL_UnlockSurface(screen);

    if (f != 1) SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void drawpixel(int x, int y, int color){_drawpixel(x, y, color, 0);}
void _drawpixel_fast(int x, int y, int color){_drawpixel(x, y, color, 1);}

void _drawhline_fast(int x1, int x2, int y, int c) {
    while (x1 <= x2) _drawpixel_fast(x1++, y, c);
}

void _drawvline_fast(int x, int y1, int y2, int c) {
    while (y1 <= y2) _drawpixel_fast(x, y1++, c);
}

void drawvline(int x, int y1, int y2, int c) {
    _drawvline_fast(x, y1, y2, c);
    SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void drawhline(int x1, int x2, int y, int c) {
    _drawhline_fast(x1, x2, y, c);
    SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void fill_rect(int x1, int y1, int x2, int y2, int c) {
    while (y1 <= y2) _drawhline_fast(x1, x2, y1++, c);
    SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void switchpalette(int p){palette = p;}