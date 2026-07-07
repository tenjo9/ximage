#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "stb_image_write.h"

#define M_PI 3.14159

struct Image
{
    int width, height;
    uint32_t *data;
};

#define PUT_PIXEL(img, x, y, color) *(&(img)->data[(y)*(img)->width + (x)]) = (color)
#define GET_PIXEL(img, x, y) &(img)->data[(y)*(img)->width + (x)]

#define RGBA(r, g, b, a) (uint32_t)(r) | (uint32_t)(g) << 8 | (uint32_t)(b) << 16 | (uint32_t)(a) << 24
#define RGB(r, g, b) (uint32_t)(r) | (uint32_t)(g) << 8 | (uint32_t)(b) << 16 | (uint32_t)(0xff) << 24

bool find_bbox
(
    struct Image *img,
    int x1, int y1,
    int x2, int y2,
    int x3, int y3,
    int *left, int *right,
    int *bottom, int *top
)
{
    *left = x1; *right = x1;
    if (*left  > x2) { *left  = x2; }
    if (*left  > x3) { *left  = x3; }
    if (*right < x2) { *right = x2; }
    if (*right < x3) { *right = x3; }
    if (*left  < 0)          { *left = 0; }
    if (*right < 0)          { return false; }
    if (*left  > img->width) { return false; }
    if (*right > img->width) { *right = img->width - 1; }

    *bottom = y1; *top = y1;
    if (*bottom < y2)  { *bottom = y2; }
    if (*bottom < y3)  { *bottom = y3; }
    if (*top    > y2)  { *top    = y2; }
    if (*top    > y3)  { *top    = y3; }
    if (*top    < 0)           { *top = 0; }
    if (*bottom < 0)           { return false; }
    if (*top    > img->height) { return false; }
    if (*bottom > img->height) { *bottom = img->height - 1; }

    return true;
}

bool is_in_triangle
(
    int x, int y,
    int x1, int y1,
    int x2, int y2,
    int x3, int y3,
    double *D, double *l1, double *l2, double *l3
)
{
    *D  = (double)( (y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3) );
    *l1 = (double)( (y2 - y3)*(x  - x3) + (x3 - x2)*(y  - y3) ) / *D;
    *l2 = (double)( (y3 - y1)*(x  - x3) + (x1 - x3)*(y  - y3) ) / *D;
    *l3 = 1.0 - *l1 - *l2;

    #define EPS -1e-14
    if (*l1 >= EPS && *l2 >= EPS && *l3 >= EPS)
    {
        return true;
    }
    #undef EPS

    return false;
}

void draw_triangle
(
    struct Image *img,
    int x1, int y1,
    int x2, int y2,
    int x3, int y3,
    uint32_t color
)
{
    int left, right;
    int bottom, top;

    if (find_bbox(img, x1, y1, x2, y2, x3, y3, &left, &right, &bottom, &top))
    for (int x = left; x <= right; ++x)
    {
        for (int y = top; y <= bottom; ++y)
        {
            double D, l1, l2, l3;
            if (is_in_triangle(x, y, x1, y1, x2, y2, x3, y3, &D, &l1, &l2, &l3))
            {
                PUT_PIXEL(img, x, y, color);
            }
        }
    }
}

void draw_triangle_source
(
    struct Image *img,
    int x1, int y1,
    int x2, int y2,
    int x3, int y3,
    uint32_t (*color)
    (
        double x, double y,
        double left, double right,
        double bottom, double top,
        double l1, double l2, double l3
    )
)
{
    int left, right;
    int bottom, top;

    if (find_bbox(img, x1, y1, x2, y2, x3, y3, &left, &right, &bottom, &top))
    for (int x = left; x <= right; ++x)
    {
        for (int y = top; y <= bottom; ++y)
        {
            double D, l1, l2, l3;
            if (is_in_triangle(x, y, x1, y1, x2, y2, x3, y3, &D, &l1, &l2, &l3))
            {
                PUT_PIXEL(img, x, y, color(x, y, left, right, bottom, top, l1, l2, l3));
            }
        }
    }
}

uint32_t color1(double x, double y, double left, double right, double bottom, double top, double l1, double l2, double l3)
{
    (void)x; (void)y; (void)left; (void)right; (void)bottom; (void)top; (void)l1; (void)l2; (void)l3;

    double r = x/800.0*255.0;
    double g = 100;
    double b = y/600.0*255.0;

    return RGBA(r, g, b, 255);
}

uint32_t color2(double x, double y, double left, double right, double bottom, double top, double l1, double l2, double l3)
{
    (void)x; (void)y; (void)left; (void)right; (void)bottom; (void)top;

    double r = l1 * 255;
    double g = l2 * 255;
    double b = l3 * 255;

    return RGBA(r, g, b, 255);
}

int main(void)
{
    struct Image img = {0};

    img.width = 800;
    img.height = 600;
    img.data = malloc((size_t)(img.width * img.height * 4));

    if (1)
    {
        for (int x = 0; x < img.width; ++x)
        {
            for (int y = 0; y < img.height; ++y)
            {
                PUT_PIXEL(&img, x, y, RGB(42, 42, 42));
            }
        }
    }


    if (1)
    {
        draw_triangle_source(&img, 0, 300, 400, 290, 799, 300, color1);
    }


    if (0)
    {
        double xscale  = 200.0, yscale  = -200.0;
        double xoffset = 400.0, yoffset =  300.0;

        #define w2sx(x) (int)((x)*xscale + xoffset)
        #define w2sy(y) (int)((y)*yscale + yoffset)


        double wxs[] = {1.0,  1.0, -1.0, -1.0};
        double wys[] = {1.0, -1.0, -1.0,  1.0};
        double angle = (30)*M_PI / 180.0;

        int sxs[4];
        int sys[4];

        for (int i = 0; i < 4; ++i)
        {
            double x = wxs[i], y = wys[i];
            wxs[i] = x*cos(angle) - y*sin(angle);
            wys[i] = x*sin(angle) + y*cos(angle);
        }
        for (int i = 0; i < 4; ++i) { sxs[i] = w2sx(wxs[i]); sys[i] = w2sy(wys[i]); }

        draw_triangle_source(&img, sxs[0], sys[0], sxs[1], sys[1], sxs[2], sys[2], color1);
        draw_triangle_source(&img, sxs[2], sys[2], sxs[3], sys[3], sxs[0], sys[0], color1);
    }


    stbi_write_png("image.png", img.width, img.height, 4, (void *)img.data, img.width * 4);
    free(img.data);
    return 0;
}
