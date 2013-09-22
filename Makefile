CC=gcc -std=gnu99
CFLAGS=-g -Wall -Werror -O3 -c -I/Library/Frameworks/SDL2.framework/Headers -D_GNU_SOURCE=1 -D_THREAD_SAFE -DLOG_DEBUG
LDFLAGS=-L/usr/local/lib -lSDLmain -lSDL2 -Wl,-framework,Cocoa -framework GLUT -framework OpenGL
EXECUTABLES=main.c
SOURCES=3dmath.c shader.c text.c object.c plyfile.c logger.c
HEADERS=$(SOURCES:%.c=%.h)
OBJECTS=$(EXECUTABLES:%.c=%.o) $(SOURCES:%.c=%.o) 

all: bin/demo

bin/demo: $(OBJECTS:%.o=obj/%.o)
	@echo "INFO: Linking '$@'..."
	@$(CC) $(LDFLAGS) $(@:bin/demo%=obj/main%.o) $(SOURCES:%.c=obj/%.o) -o $@

$(OBJECTS:%.o=obj/%.o): $(EXECUTABLES:%.c=src/%.c) $(SOURCES:%.c=src/%.c) $(HEADERS:%.h=src/%.h)
	@echo "INFO: Compiling '$(@:obj/%.o=src/%.c)' to '$@'..."
	@$(CC) $(CFLAGS) $(@:obj/%.o=src/%.c) -o $@

clean:
	@echo "INFO: Cleaning workspace..."
	@rm -f obj/*.o && rm -f bin/demo_glsl*
