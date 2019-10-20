
plyfile.o: 3rd/ply-1.1/plyfile.c 3rd/ply-1.1/ply.h
	cc -g -c 3rd/ply-1.1/plyfile.c -I.

objects = main.o plyfile.o
point : $(objects)
	cc -o point $(objects)
$(objects) : main.c

clean:
	-rm point $(objects) *.ppm
install:
	./point
