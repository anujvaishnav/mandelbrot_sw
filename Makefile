CC = gcc

LDFLAGS += -lX11 
LDFLAGS += -lm

mandelbrot: 
	$(CC) mandelbrot_sw.c $(LDFLAGS) -o mandelbrot_sw.out

fixed_point: 
	$(CC) mandelbrot_fixed_point_sw.c $(LDFLAGS) -o mandelbrot_fixed_point_sw.out

simple_drawing:
	$(CC) simple-drawing.c $(LDFLAGS) -o simple-drawing.out

clean: 
	rm *.out 
