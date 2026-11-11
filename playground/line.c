#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../3rdparty/stb_image_write.h"

void throw(char *msg, char *file, int line)
{
    fprintf(stderr, "'%s':%d: '%s'\n", file, line, msg);
    exit(-1);
}

#define XI_ASSERT(cond, msg) if (!(cond)) { throw(msg, __FILE__, __LINE__); }
#define XIMAGE_IMPLEMENTATION
#include "../ximage.h"


// ========================== AI SLOP ========================== //
void xi_draw_line(ximage_t *xi, int x1, int y1, int x2, int y2, color_t color)
{
    XI_ASSERT(xi != XI_NULL, "xi pointer must not be NULL");

    int dx = xi_absi(x2 - x1);
    int dy = xi_absi(y2 - y1);

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    for (;;)
    {
        xi_put_pixel(xi, x1, y1, color);

        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;

        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void xi_draw_line_thick(ximage_t *xi, int x1, int y1, int x2, int y2, int thickness, color_t color)
{
    XI_ASSERT(xi != XI_NULL, "xi pointer must not be NULL");

    int dx = xi_absi(x2 - x1);
    int dy = xi_absi(y2 - y1);

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    for (;;)
    {
        xi_draw_circle(xi, x1, y1, thickness, color);

        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;

        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}
// ========================== AI SLOP ========================== //


#define WIDTH  100
#define HEIGHT 100
color_t canvas[WIDTH*HEIGHT];
ximage_t img = { .width=WIDTH, .height=HEIGHT, .data=canvas };


int main(void)
{
    xi_fill_bg(&img, XI_RGB(42, 42, 42));

    int x1 =  0, y1 =  20;
    int x2 = 20, y2 = HEIGHT-20;

    int dx = x2 - x1;
    int dy = y2 - y1;

    float d = (float)dy / (float)dx;

    int y = 0;
    for (int x = 0; x <= x2; ++x)
    {
        float y_ = d*((float)x - (float)x1) + (float)y1;
        while ((float)y < y_)
        {
            ++y;
            xi_put_pixel(&img, x, y, XI_RGB(255, 255, 255));
        }
        xi_put_pixel(&img, x, y, XI_RGB(255, 255, 255));
    }

    stbi_write_png("output.png", img.width, img.height, 4, (void *)img.data, 4*img.width);
}
