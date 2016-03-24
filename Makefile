test:
	make build;
	make run;

build:
	gcc -std=c99 -c HaD_Badge.c bh-badge-animate.c;
	gcc HaD_Badge.o bh-badge-animate.o -o HaD_Badge -lSDL2 -lSDL2_gfx;

run:
	./HaD_Badge;

clean:
	-rm HaD_Badge;
	-rm *.o;
	-rm *~
