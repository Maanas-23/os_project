CC = gcc
CFLAGS = -Iinclude
DEPS = $(wildcard include/*.h)
SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.o)
EXEC = build/main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^

build/%.o: src/%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf build/*.o $(EXEC)

.PHONY: all clean