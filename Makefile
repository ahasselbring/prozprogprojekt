CC = gcc
CFLAGS = -c -O2 -Wall -Wextra -Werror -std=c99 $(shell pkg-config --cflags sdl2)

SRC = $(shell find -name '*.c')
OBJ = $(SRC:.c=.o)

PROG = game

all: $(PROG)

$(PROG): $(OBJ)
	$(CC) $(shell pkg-config --libs sdl2) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	@rm -f $(OBJ) $(PROG)

.PHONY: all clean
