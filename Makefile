CC = gcc
CFLAGS = -c -O2 -Wall -Wextra -std=c99 $(shell pkg-config --cflags sdl2)

SRC = $(shell find ./src -name '*.c')
OBJ = $(SRC:.c=.o)

PROG = ponging

all: $(PROG)

$(PROG): $(OBJ)
	$(CC) -o $@ $^ $(shell pkg-config --libs sdl2) -lSDL2_ttf

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	@rm -f $(OBJ) $(PROG)

.PHONY: all clean
