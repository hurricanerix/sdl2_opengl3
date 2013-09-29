CC=gcc -std=gnu99
CFLAGS=-g -Wall -Werror -O3 -c -I/Library/Frameworks/SDL2.framework/Headers -D_GNU_SOURCE=1 -D_THREAD_SAFE -D_DEBUG
LDFLAGS=-L/usr/local/lib -lSDLmain -lSDL2 -Wl,-framework,Cocoa -framework GLUT -framework OpenGL
EXECUTABLE=demo
SOURCES=main.c 3dmath.c shader.c text.c object.c plyfile.c logger.c
HEADERS=$(SOURCES:%.c=%.h)
OBJECTS=$(EXECUTABLES:%.c=%.o) $(SOURCES:%.c=%.o) 

all: bin/$(EXECUTABLE)

bin/$(EXECUTABLE): $(OBJECTS:%.o=obj/%.o)
	@echo "INFO: Linking '$@'..."
	@$(CC) $(LDFLAGS) $(SOURCES:%.c=obj/%.o) -o bin/$(EXECUTABLE)

$(OBJECTS:%.o=obj/%.o): $(SOURCES:%.c=src/%.c) $(HEADERS:%.h=src/%.h)
	@echo "INFO: Compiling '$(@:obj/%.o=src/%.c)' to '$@'..."
	@$(CC) $(CFLAGS) $(@:obj/%.o=src/%.c) -o $@

clean:
	@echo "INFO: Cleaning workspace..."
	@rm -f obj/*.o && rm -f bin/$(EXECUTABLE)
