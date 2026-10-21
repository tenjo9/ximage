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


#define WIDTH  800
#define HEIGHT 600
color_t canvas[WIDTH*HEIGHT];
ximage_t img = { .width=WIDTH, .height=HEIGHT, .data=canvas };


int main(void)
{
    int x1, x2, x, dx, y1, y2, y, dy, e;
    xi_fill_bg(&img, XI_RGB(42, 42, 42));

    ////////////////////////////////////////////
    x1 = 100;       y1 = 100;
    x2 = WIDTH-100; y2 = HEIGHT-100;

    dx = x2 - x1;
    dy = y2 - y1;

    e = 2*dy-dx;

    y = y1;
    for (x = x1; x < x2; ++x)
    {
        xi_draw_circle(&img, x, y, 10, XI_RGB(255, 255, 255));
        //xi_put_pixel(&img, x, y, XI_RGB(255, 255, 255));
        if (e >= 0) {
            e -= 2*dx;
            y += 1;
        }
        e += 2*dy;
    }
    ////////////////////////////////////////////
    x1 = 100;       y1 = HEIGHT-100;
    x2 = WIDTH-100; y2 = 100;

    dx = x2 - x1;
    dy = y1 - y2;

    e = 2*dy-dx;

    y = y2;
    for (x = x2; x > x1; --x)
    {
        xi_draw_circle(&img, x, y, 10, XI_RGB(255, 255, 0));
        //xi_put_pixel(&img, x, y, XI_RGB(255, 255, 0));
        if (e >= 0) {
            e -= 2*dx;
            y += 1;
        }
        e += 2*dy;
    }
    ////////////////////////////////////////////
    x1 = 100;       y1 = 100;
    x2 = WIDTH-100; y2 = 100;

    dx = x2 - x1;
    dy = y2 - y1;

    e = 2*dy-dx;

    y = y1;
    for (x = x1; x < x2; ++x)
    {
        xi_draw_circle(&img, x, y, 10, XI_RGB(255, 0, 255));
        //xi_put_pixel(&img, x, y, XI_RGB(255, 0, 255));
        if (e >= 0) {
            e -= 2*dx;
            y += 1;
        }
        e += 2*dy;
    }
    ////////////////////////////////////////////
    x1 = 100; y1 = 100;
    x2 = 100; y2 = HEIGHT-100;

    dx = x2 - x1;
    dy = y2 - y1;

    e = 2*dy-dx;

    x = x1;
    for (y = y1; y < y2; ++y)
    {
        xi_draw_circle(&img, x, y, 10, XI_RGB(0, 255, 150));
        //xi_put_pixel(&img, x, y, XI_RGB(0, 255, 150));
        if (e >= 0) {
            e -= 2*dy;
            x += 1;
        }
        e += 2*dx;
    }
    ////////////////////////////////////////////

    stbi_write_png("output.png", img.width, img.height, 4, (void *)img.data, 4*img.width);
}
