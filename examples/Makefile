# Install
BIN = demo

# Flags
CFLAGS += -std=c99 -pedantic -O2 `pkg-config --cflags gtk+-3.0` -I../src/include -I../src/raylib -DUSE_LIBTYPE_SHARED

SRC = window.c
OBJ = $(SRC:.c=.o)

BIN := $(BIN).exe
LIBS = -Wall -L../src -lgtkraylib `pkg-config --libs gtk+-3.0` -lm

all: clean $(BIN)

clean:
	rm -f $(BIN) $(OBJS)

$(BIN):
	$(CC) $(SRC) $(CFLAGS) -o $(BIN) $(LIBS)
