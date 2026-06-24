all: ./a.out run

./a.out: ./main.c ./stb_image_write.h
	gcc -g ./main.c -lm

run:
	./a.out

rm:
	rm ./a.out ./image.png
