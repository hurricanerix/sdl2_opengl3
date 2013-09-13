CC=gcc
CFLAGS=-Wall -c -I/Library/Frameworks/SDL2.framework/Headers -D_GNU_SOURCE=1 -D_THREAD_SAFE -std=gnu99
LDFLAGS=-L/usr/local/lib -lSDLmain -lSDL2 -Wl,-framework,Cocoa -framework GLUT -framework OpenGL
SOURCES=src/main.c
EXE=bin/demo

all: $(EXE)

$(EXE): obj/main.o
	$(CC) $(LDFLAGS) $< -o $@

obj/main.o: src/main.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm obj/*.o && rm $(EXE)
