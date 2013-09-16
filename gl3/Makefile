CC=gcc -std=gnu99
CFLAGS=-g -Wall -Werror -O3 -c -I/Library/Frameworks/SDL2.framework/Headers -D_GNU_SOURCE=1 -D_THREAD_SAFE 
LDFLAGS=-L/usr/local/lib -lSDLmain -lSDL2 -Wl,-framework,Cocoa -framework GLUT -framework OpenGL
EXECUTABLES=main.c
SOURCES=textfile.c
HEADERS=$(SOURCES:%.c=%.h)
OBJECTS=$(EXECUTABLES:%.c=%.o) $(SOURCES:%.c=%.o) 

all: demo

demo: $(OBJECTS)
	@echo "INFO: Linking '$@'..."
	$(CC) $(LDFLAGS) $(@:demo%=main%.o) $(SOURCES:%.c=%.o) -o $@

$(OBJECTS): $(@:%.o=%.c) $(HEADERS)
	@echo "INFO: Compiling '$(@:%.o=%.c)' to '$@'..."
	$(CC) $(CFLAGS) $(@:%.o=%.c) -o $@

clean:
	@echo "INFO: Cleaning workspace..."
	@rm -f *.o && rm -f demo
