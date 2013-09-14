CC=gcc
CFLAGS=-Wall -c -I/Library/Frameworks/SDL2.framework/Headers -D_GNU_SOURCE=1 -D_THREAD_SAFE -std=gnu99
LDFLAGS=-L/usr/local/lib -lSDLmain -lSDL2 -Wl,-framework,Cocoa -framework GLUT -framework OpenGL

all: bin/demo_glsl120 bin/demo_glsl150

run: run-120

run-120: clean bin/demo_glsl120
	./bin/demo_glsl120

run-150: clean bin/demo_glsl120
	./bin/demo_glsl120

bin/demo_glsl120: obj/main_glsl120.o
	$(CC) $(LDFLAGS) $< -o $@

obj/main_glsl120.o: src/main_glsl120.c
	$(CC) $(CFLAGS) $< -o $@

bin/demo_glsl150: obj/main_glsl150.o
	$(CC) $(LDFLAGS) $< -o $@

obj/main_glsl150.o: src/main_glsl150.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f obj/*.o && rm -f $(EXE)
