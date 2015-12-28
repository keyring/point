objects = point.o
point : $(objects)
	cc -o point point.o
point.o : point.c

clean:
	-rm point $(objects) *.ppm