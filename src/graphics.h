/* graphics.h */
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_
/* supported graphics modes in graphics_open */
#define CGA_320x200x4	    0x04
#define CGA_320x200x4G	    0x05 /* Grayscale */
#define CGA_640x200x2	    0x06 /* B/W */

#define VGA_640x350x16      0x10        /* 640x350 16 color/4bpp */
#define VGA_640x480x16      0x12        /* 640x480 16 color/4bpp */
#define PAL_320x200x256     0x13        /* 320x200 256 color/8bpp */
#define TEXT_MODE           0x03        /* 80x25 text mode */

extern int SCREENWIDTH;                 /* set by graphics_open */
extern int SCREENHEIGHT;
extern int VGA;                         /* VGA vs PAL mode */

enum {
    CGA_PAL0_LOWINT,
    CGA_PAL0_HIGHINT,
    CGA_PAL1_LOWINT,
    CGA_PAL1_HIGHINT
};

/* start/stop graphics mode */
int graphics_open(int mode);
void graphics_close(void);
void drawpixel(int x,int y, int color);
void drawhline(int x1, int x2, int y, int c);
void drawvline(int x, int y1, int y2, int c);
void fill_rect(int x1, int y1, int x2, int y2, int c);
void switchpalette(int palette);

/* VGA 16 color, 4bpp routines */
#ifdef _ELKS_
void vga_init(void);
void vga_drawpixel(int x, int y, int c);
void vga_drawhline(int x1, int x2, int y, int c);
void vga_drawvline(int x, int y1, int y2, int c);
int vga_readpixel(int x, int y);
void vga_fill_rect(int x1, int y1, int x2, int y2, int c);

/* PAL 256 color 8bpp routines */
void pal_drawpixel(int x,int y, int color);
void pal_fill_rect(int x1, int y1, int x2, int y2, int c);
#else
void vga_init(void);
#define vga_drawpixel drawpixel
#define pal_drawpixel drawpixel
#define vga_drawhline drawhline
#define vga_drawvline drawvline
#define vga_fill_rect fill_rect
#define pal_fill_rect fill_rect
#endif

int digit(char c, int base);
unsigned int strtoi(const char *s, int base);
#endif
