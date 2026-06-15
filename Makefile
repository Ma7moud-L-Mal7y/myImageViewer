CC = gcc
CFLAGS = $(shell sdl2-config --cflags)
LIBS = $(shell sdl2-config --libs)
SRC = iv.c utils/readFormats.c

all:
	$(CC) $(SRC) -o iv $(CFLAGS) $(LIBS)

run:
	__NV_PRIME_RENDER_OFFLOAD=1 __GLX_VENDOR_LIBRARY_NAME=nvidia ./iv

clean:
	rm -f iv