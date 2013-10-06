#The MIT License (MIT)
#
#Copyright (c) 2013 Richard Hawkins
#
#Permission is hereby granted, free of charge, to any person obtaining a copy
#of this software and associated documentation files (the "Software"), to deal
#in the Software without restriction, including without limitation the rights
#to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#copies of the Software, and to permit persons to whom the Software is
#furnished to do so, subject to the following conditions:
#
#The above copyright notice and this permission notice shall be included in
#all copies or substantial portions of the Software.
#
#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#THE SOFTWARE.

EXECUTABLE=demo
EXECUTABLE_EXT=
SOURCES=main.c 3dmath.c shader.c text.c object.c plyfile.c logger.c config.c texture.c status.c
HEADERS=$(SOURCES:%.c=%.h)
OBJECTS=$(EXECUTABLES:%.c=%.o) $(SOURCES:%.c=%.o) 

ifeq ($(OS),Windows_NT)
    CC=echo "???"
    CFLAGS_ALL=
    LDFLAGS_ALL=
    EXECUTABLE_EXT=.exe
else
    CC=gcc -std=gnu99
    UNAME_S := $(shell uname -s)
    CFLAGS_ALL=-g -Wall -Werror -O3 -c `sdl2-config --cflags`
    LDFLAGS_ALL=-L/usr/local/lib -lm `sdl2-config --libs`

    ifeq ($(UNAME_S),Darwin)
        CFLAGS=$(CFLAGS_ALL) -I/usr/include/GL/gl.h -I/Library/Frameworks/SDL2.framework/Headers -D_GNU_SOURCE=1 -D_THREAD_SAFE -D_DEBUG
        LDFLAGS=$(LDFLAGS_ALL) -Wl,-framework,Cocoa -framework GLUT -framework OpenGL
    endif
    ifeq ($(UNAME_S),Linux)
        CFLAGS=$(CFLAGS_ALL) -I/usr/include
        LDFLAGS=$(LDFLAGS_ALL) -lGL
    endif
endif

all: bin/$(EXECUTABLE)$(EXECUTABLE_EXT)

bin/$(EXECUTABLE)$(EXECUTABLE_EXT): $(OBJECTS:%.o=obj/%.o)
	@echo "INFO: Linking '$@'..."
	$(CC) $(SOURCES:%.c=obj/%.o) $(LDFLAGS) -o bin/$(EXECUTABLE)$(EXECUTABLE_EXT)

$(OBJECTS:%.o=obj/%.o): $(SOURCES:%.c=src/%.c) $(HEADERS:%.h=src/%.h)
	@echo "INFO: Compiling '$(@:obj/%.o=src/%.c)' to '$@'..."
	$(CC) $(CFLAGS) $(@:obj/%.o=src/%.c) -o $@

clean:
	@echo "INFO: Cleaning workspace..."
	rm -f obj/*.o && rm -f bin/$(EXECUTABLE)
