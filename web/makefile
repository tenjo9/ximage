Cflags = -Wall -Wextra -Wconversion -pedantic -std=c11

all: build/index.html

build/index.html: index.html build/ximage.b64
	cp -f index.html $@
	sed -i "s|WASM_BLOB|$$(cat build/ximage.b64)|" $@

build/ximage.b64: build/ximage.wasm
	base64 -w 0 build/ximage.wasm > build/ximage.b64

build/ximage.wasm: build/ximage.o
	wasm-ld --no-entry --export=draw_demo --export=canvas --export-memory $< -o $@

build/ximage.o: demo.c ../ximage.h | build
	clang -O3 -ffreestanding -fno-builtin --target=wasm32-unknown-unknown $(Cflags) $< -c -o $@

build:
	mkdir -p build

clean:
	rm -rf build
