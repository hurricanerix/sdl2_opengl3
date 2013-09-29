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

CC=gcc -std=gnu99
CFLAGS=-g -Wall -Werror -O3 -c -I/Library/Frameworks/SDL2.framework/Headers -D_GNU_SOURCE=1 -D_THREAD_SAFE -D_DEBUG
LDFLAGS=-L/usr/local/lib -lSDLmain -lSDL2 -Wl,-framework,Cocoa -framework GLUT -framework OpenGL
EXECUTABLE=demo
SOURCES=main.c 3dmath.c shader.c text.c object.c plyfile.c logger.c config.c
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
