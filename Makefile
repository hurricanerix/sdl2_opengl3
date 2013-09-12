CC=gcc
SDLFLAGS=`sdl-config --cflags --libs`
SOURCES=main.c
EXECUTABLE=game

all:
	$(CC) -o $(EXECUTABLE) $(SOURCES) $(SDLFLAGS)
