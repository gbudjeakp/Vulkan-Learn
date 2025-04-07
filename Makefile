# This will help in compiling any file we specify
CC = gcc
CFLAGS = $(shell pkg-config --cflags glfw3 vulkan)
LDFLAGS = $(shell pkg-config --libs glfw3 vulkan)
SRC = $(wildcard week-*.c)
OUT = $(basename $(SRC))

all: $(OUT)

%: %.c
	$(CC) $< -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(OUT)
