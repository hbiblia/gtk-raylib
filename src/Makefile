# Install
BIN = libgtkraylib

# Flags
CFLAGS += -std=c99 -fPIC -Wall -Wextra -O2 -g -shared `pkg-config --cflags gtk+-3.0` -Iinclude

SRC = gtkraylib.c
OBJ = $(SRC:.c=.o)

BIN := $(BIN).so
LIBS = `pkg-config --libs gtk+-3.0` -lm -L../src/

all: $(BIN)

clean:
	rm -f $(BIN) $(OBJS)

$(BIN):
	rm -f $(BIN) $(OBJS)
	$(CC) $(SRC) $(CFLAGS) -o $(BIN) $(LIBS)