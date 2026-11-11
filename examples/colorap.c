#include <unistd.h>
#include "graphics.h"

int main(int argc, char *argv[]) {
	int x,y;long c = 0;

	graphics_open(VGA_640x480x16);

	for (x = 0; x != SCREENWIDTH; x++)
		for (y = 0; y != SCREENHEIGHT; y++)
			drawpixel(x, y, c++);

	sleep(10);

	graphics_close();
	return 0;
}
