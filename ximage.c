#include <stdio.h>

#include "stb_image_write.h"

#define XIMAGE_IMPLEMENTATION
#include "ximage.h"

int main(void)
{
    XImage img = xi_new(800, 600);

    xi_fill_bg(&img, 0xff424242);
    xi_draw_circle(&img, 400, 300, 200, 0xff0000ff);
    xi_draw_rect(&img, 100, 200, 200, 200, 0xaa00ff00);
    xi_draw_triangle(&img, 500, 400, 600, 100, 700, 400, 0xaaff0000);



    XImage *spr = &(XImage){0};
    spr->width = 100;
    spr->height = 100;
    spr->data = malloc((size_t)(spr->width * spr->height) * 4);
    if (!spr->data)
    {
        fprintf(stderr, "Image memory allocation error\n");
        exit(69);
    }

    xi_draw_circle(spr, 50, 50, 50, 0xffff00ff);
    xi_draw_sprite(&img, spr, 250, 250);

    xi_draw_line(&img, 0, 0, img.width, img.height, 0xffffffff);


    stbi_write_png("ximage.png", img.width, img.height, 4, (void *)img.data, 4*img.width);
    xi_delete(&img);
    return 0;
}
