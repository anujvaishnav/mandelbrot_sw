#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>

void drawpixel(Display* di, Window wi, GC gc, int x, int y, int color)
{
	XSetForeground(di, gc, color);
	XDrawPoint(di, wi, gc, x, y);
}

int main() 
{
	//Open Display
	Display *di = XOpenDisplay(getenv("DISPLAY"));
	if (di == NULL) {
		printf("Couldn't open display.\n");
		return -1;
	}

	//Create Window
	int const x = 0, y = 0, width = 640, height = 480, border_width = 1;
	int sc    = DefaultScreen(di);
	Window ro = DefaultRootWindow(di);
	Window wi = XCreateSimpleWindow(di, ro, x, y, width, height, border_width, 
                                BlackPixel(di, sc), WhitePixel(di, sc));
	XMapWindow(di, wi); //Make window visible
	XStoreName(di, wi, "Window sample"); // Set window title
	
	//Prepare the window for drawing
	GC gc = XCreateGC(di, ro, 0, NULL);

	//Select what events the window will listen to
	XSelectInput(di, wi, KeyPressMask | ExposureMask);
	XEvent ev;
	int quit = 0;
	
	while (!quit) {
		int a = XNextEvent(di, &ev);
		if (ev.type == KeyPress)
			quit = 1; // quit if someone presses a key
		if (ev.type == Expose) {
		  for(int i = 0; i < width; i++)
		  {
/*		    int j = 10;*/
		    for(int j = 0; j < height; j++)
			    drawpixel(di, wi, gc, i, j, 0x00FF00); //green
			}
		}
	}
	
	XFreeGC(di, gc);
	XDestroyWindow(di, wi);
	XCloseDisplay(di);
	return 0;
}
