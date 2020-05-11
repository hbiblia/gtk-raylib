# Flags
CFLAGS += -std=c99 -pedantic -O2 `pkg-config --cflags gtk+-3.0` -I../src/include -I../src/raylib -DUSE_LIBTYPE_SHARED

OBJ = $(SRC:.c=.o)

BIN := $(BIN).exe
LIBS = -Wall -L../src -lgtkraylib `pkg-config --libs gtk+-3.0` -lm

all: clean window input

clean:
	rm -f *.exe

window:
	$(CC) window.c $(CFLAGS) -o window.exe $(LIBS)

input:
	$(CC) input_key.c $(CFLAGS) -o input_key.exe $(LIBS)
	$(CC) input_mouse.c $(CFLAGS) -o input_mouse.exe $(LIBS)
	$(CC) input_mouse_wheel.c $(CFLAGS) -o input_mouse_wheel.exe $(LIBS)
