CC = gcc

LDFLAGS += -lX11 

mandelbrot: 
	$(CC) mandelbrot_sw.c $(LDFLAGS) -o mandelbrot_sw.out

simple-drawing:
	$(CC) simple-drawing.c $(LDFLAGS) -o simple-drawing.out

clean: 
	rm *.out 
