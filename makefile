objects = main.o
point : $(objects)
	cc -o point $(objects)
$(objects) : main.c
clean:
	-rm point $(objects) *.ppm
install:
	./point
