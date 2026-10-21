#include <stdio.h>
#include <stdlib.h>
#include "../3rdparty/stb_image_write.h"

void throw(char *msg, char *file, int line)
{
    fprintf(stderr, "'%s':%d: '%s'\n", file, line, msg);
    exit(-1);
}

#define XI_ASSERT(cond, msg) if (!(cond)) { throw(msg, __FILE__, __LINE__); }
#define XIMAGE_IMPLEMENTATION
#include "../ximage.h"


#define WIDTH  800
#define HEIGHT 600
color_t canvas[WIDTH*HEIGHT];
ximage_t img = { .width=WIDTH, .height=HEIGHT, .data=canvas };

#define TEX_WIDTH  100
#define TEX_HEIGHT 100
color_t texture[TEX_WIDTH*TEX_HEIGHT];
ximage_t tex = { .width=TEX_WIDTH, .height=TEX_HEIGHT, .data=texture };


int main(void)
{
    xi_fill_bg(&tex, XI_RGB(250, 10, 235));
    xi_draw_circle(&tex, TEX_WIDTH/2, TEX_HEIGHT/2, 30, XI_RGB(250, 215, 10));

    xi_fill_bg(&img, XI_RGB(42, 42, 42));
    xi_draw_circle(&img, 400, 300, 200, XI_RGBA(255, 0, 0, 255));
    xi_draw_line_thick(&img, 100, 100, 700, 500, 10, XI_RGBA(255, 255, 255, 128));
    xi_draw_line_thick(&img, 100, 500, 700, 100, 10, XI_RGBA(255, 255, 0, 128));
    xi_draw_rect(&img, 100, 200, 200, 200, XI_RGBA(0, 255, 0, 128));
    xi_draw_triangle(&img, 600, 100, 500, 400, 700, 400, XI_RGBA(0, 0, 255, 128));
    xi_put_pixel(&img, 400, 300, XI_RGB(0, 0, 0));
    xi_draw_texture(&img, &tex, -100, 0, 200, 600, 255);
    stbi_write_png("output.png", img.width, img.height, 4, (void *)img.data, 4*img.width);
}
