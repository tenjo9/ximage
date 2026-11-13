all: output.mp4

output.mp4: out
	./out | ffmpeg -y -f image2pipe -vcodec ppm -framerate 120 -i pipe:0 -crf 12 -preset veryslow -c:v libx264 -pix_fmt yuv444p output.mp4

out: demo.c
	gcc -Wall -Wextra -Wconversion -O3 -march=native -mavx2 -lm $< -o $@

play: output.mp4
	mpv --loop=inf output.mp4

clean:
	rm out output.mp4
