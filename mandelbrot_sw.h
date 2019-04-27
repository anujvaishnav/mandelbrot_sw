#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <sys/wait.h> 
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

Display* dis;
Window win;
GC gc;
