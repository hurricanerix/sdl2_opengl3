CC=gcc
CFLAGS=-Wall -c -I/Library/Frameworks/SDL2.framework/Headers -D_GNU_SOURCE=1 -D_THREAD_SAFE -std=gnu99
LDFLAGS=-L/usr/local/lib -lSDLmain -lSDL2 -Wl,-framework,Cocoa -framework GLUT -framework OpenGL
EXECUTABLES=main_glsl120.c #main_glsl150.c
SOURCES=sdl.c shader.c cube.c
HEADERS=$(SOURCES:%.c=%.h)
OBJECTS=$(EXECUTABLES:%.c=%.o) $(SOURCES:%.c=%.o) 

all: bin/demo_glsl120 bin/demo_glsl150

bin/demo_glsl120: $(OBJECTS:%.o=obj/%.o)
	@echo "INFO: Linking '$@'..."
	$(CC) $(LDFLAGS) $(@:bin/demo%=obj/main%.o) $(SOURCES:%.c=obj/%.o) -o $@

#bin/demo_glsl150: $(OBJECTS:%.o=obj/%.o)
#	@echo "INFO: Linking '$@'..."
#	$(CC) $(LDFLAGS) $(@:bin/demo%=obj/main%.o) $(SOURCES:%.c=obj/%.o) -o $@

$(OBJECTS:%.o=obj/%.o): $(@:obj/%.o=src/%.c) $(HEADERS:%.h=src/%.h)
	@echo "INFO: Compiling '$(@:obj/%.o=src/%.c)' to '$@'..."
	$(CC) $(CFLAGS) $(@:obj/%.o=src/%.c) -o $@

clean:
	@echo "INFO: Cleaning workspace..."
	rm -f obj/*.o && rm -f bin/demo_glsl*
