#include <unistd.h>
#include "graphics.h"

int main(int argc, char *argv[]) {
	int x,y,m;long c = 0;

	if (argc > 1) 	m = strtoi(argv[1], 16);
	else			m = VGA_640x480x16;

	if (graphics_open(m) != 0)
		return 1;

	for (x = 0; x != SCREENWIDTH; x++)
		for (y = 0; y != SCREENHEIGHT; y++)
			drawpixel(x, y, c++);

	sleep(10);

	graphics_close();
	return 0;
}
