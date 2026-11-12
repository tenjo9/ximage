Cflags = -Wall -Wextra -Wconversion -Werror -pedantic -std=c11
Fsan = -fsanitize=leak,address,undefined

all: build out

out: build/test.o build/stb.o
	gcc $(Fsan) -lm $^ -o $@

build/test.o: test.c ../ximage.h
	gcc $(Cflags) -c -g $< -o $@

build/stb.o: ../3rdparty/stb.c
	gcc $(Fsan) -O3 -c -g $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build out *.png
