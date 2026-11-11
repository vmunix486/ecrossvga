#include <unistd.h>
#include "graphics.h"

int main(void) {
    unsigned short i;

    if (graphics_open(CGA_320x200x4) != 0)
        return -1;

    for (i = 0; i <= 3; i++) {
        switchpalette(i);
        fill_rect(0, 0, SCREENWIDTH/4, SCREENHEIGHT, 0);
        fill_rect(SCREENWIDTH/4, 0, (SCREENWIDTH/4)*2, SCREENHEIGHT, 1);
        fill_rect((SCREENWIDTH/4)*2, 0, (SCREENWIDTH/4)*3, SCREENHEIGHT, 2);
        fill_rect((SCREENWIDTH/4)*3, 0, SCREENWIDTH, SCREENHEIGHT, 3);
        sleep(5);
    }

    graphics_close();

    return 0;
}