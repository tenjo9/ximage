#include <stdio.h>

#include "stb_image_write.h"

#define XIMAGE_IMPLEMENTATION
#include "ximage.h"

int main(void)
{
    XImage *img = &(XImage){0};
    img->width = 800;
    img->height = 600;
    img->data = malloc((size_t)(img->width * img->height) * 4);
    if (!img->data)
    {
        fprintf(stderr, "Image memory allocation error\n");
        exit(69);
    }

    xi_fill_bg(img, 0xff424242);
    xi_draw_circle(img, 200, 300, 100, 0xff0000ff);
    xi_draw_triangle(img, 500, 400, 600, 100, 700, 400, 0xffff0000);

    stbi_write_png("ximage.png", img->width, img->height, 4, (void *)img->data, 4*img->width);
    free(img->data);
    return 0;
}
