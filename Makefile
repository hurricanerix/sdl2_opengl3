CC=gcc
CFLAGS=-Wall -c -I/Library/Frameworks/SDL2.framework/Headers -D_GNU_SOURCE=1 -D_THREAD_SAFE -std=gnu99
LDFLAGS=-L/usr/local/lib -lSDLmain -lSDL2 -Wl,-framework,Cocoa -framework GLUT -framework OpenGL
SOURCES=main.c
EXE=demo

all: $(EXE)

$(EXE): main.o
	$(CC) $(LDFLAGS) $< -o $@

main.o: main.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o && rm $(EXE)
