#include <stdio.h>

#include "../3rdparty/stb_image_write.h"

#define XI_DEBUG
#define XI_ASSERT(cond, msg)\
    if (!(cond)) { fprintf(stderr, "%s:%d: " msg "\n", __FILE__, __LINE__); exit(-1); }
#define XI_ASSERT_VA(cond, msg, ...)\
    if (!(cond)) { fprintf(stderr, "%s:%d: " msg "\n", __FILE__, __LINE__, __VA_ARGS__); exit(-1); }
#define XIMAGE_IMPLEMENTATION
#include "../ximage.h"


#define WIDTH  800
#define HEIGHT 600
color_t canvas[WIDTH*HEIGHT];


void xi_draw_ellipse(ximage_t *xi, int x0, int y0, int a, int b, color_t color)
{
    XI_ASSERT( (xi) != XI_NULL, "xi is NULL");

    int xstart = 0;
    int xend   = xi->width;
    int ystart = 0;
    int yend   = xi->height;
    double A = 1.0 / (double)a;
    double B = 1.0 / (double)b;

    for (int y = ystart; y < yend; ++y)
    {
        for (int x = xstart; x < xend; ++x)
        {
            double mx = (double)x - (double)x0 + 0.5;
            double my = (double)y - (double)y0 + 0.5;
            if (mx*mx*A*A + my*my*B*B <= 1.0)
            {
                *XI_PIXEL(xi, x, y) = blend_colors(color, *XI_PIXEL(xi, x, y));
            }
        }
    }
}


int main(void)
{
    ximage_t img = { .width=WIDTH, .height=HEIGHT, .data=canvas };

    xi_fill_bg(&img, XI_RGB(42, 42, 42));
    xi_draw_ellipse(&img, 400, 300, 100, 50, XI_RGB(60, 60, 60));

    stbi_write_png("output.png", img.width, img.height, 4, (void *)img.data, 4*img.width);
}
