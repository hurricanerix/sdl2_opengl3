CC=gcc
SDLFLAGS=`sdl-config --cflags --libs` -I/opt/X11/include -framework GLUT -framework OpenGL 
SOURCES=main.c
EXECUTABLE=game

all:
	$(CC) -o $(EXECUTABLE) $(SOURCES) $(SDLFLAGS)
