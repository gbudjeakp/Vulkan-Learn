CC = gcc
CFLAGS = $(shell pkg-config --cflags glfw3 vulkan)
LDFLAGS = $(shell pkg-config --libs glfw3 vulkan)

SRC = $(wildcard week-*.c)
BINS = $(patsubst %.c, build/%, $(SRC))

all: build $(BINS)

# Ensure build directory exists
build:
	mkdir -p build

# Compile source files into build/ directory
build/%: %.c
	$(CC) $< -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f build/*
