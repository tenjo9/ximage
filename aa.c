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

#define PUT_PIXEL(img, x, y, color) (img)->data[(y)*(img)->width + (x)] = (color)
#define GET_PIXEL(img, x, y)        (img)->data[(y)*(img)->width + (x)]

#define RGBA(r, g, b, a) (uint32_t)(r) | (uint32_t)(g) << 8 | (uint32_t)(b) << 16 | (uint32_t)(a) << 24
#define RGB(r, g, b) (uint32_t)(r) | (uint32_t)(g) << 8 | (uint32_t)(b) << 16 | (uint32_t)(0xff) << 24

#define GET_RED(color) ((color) >> 0)  & 0xff
#define GET_GREEN(color) ((color) >> 8)  & 0xff
#define GET_BLUE(color) ((color) >> 16) & 0xff
#define GET_APLPHA(color) ((color) >> 24) & 0xff

uint32_t blend_colors(uint32_t colorA, uint32_t colorB)
{
    uint32_t aA = (colorA >> 24) & 0xff;
    uint32_t aB = (colorB >> 24) & 0xff;

    uint32_t rA = (colorA >> 0)  & 0xff;
    uint32_t gA = (colorA >> 8)  & 0xff;
    uint32_t bA = (colorA >> 16) & 0xff;

    uint32_t rB = (colorB >> 0)  & 0xff;
    uint32_t gB = (colorB >> 8)  & 0xff;
    uint32_t bB = (colorB >> 16) & 0xff;

    uint32_t rC = (rB*(255 - aA) + rA*aA) / 255; if (rC > 255) rC = 255;
    uint32_t gC = (gB*(255 - aA) + gA*aA) / 255; if (gC > 255) gC = 255;
    uint32_t bC = (bB*(255 - aA) + bA*aA) / 255; if (bC > 255) bC = 255;

    return RGBA(rC, gC, bC, aB);
}

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
    double x, double y,
    int x1, int y1,
    int x2, int y2,
    int x3, int y3,
    double *D, double *l1, double *l2, double *l3
)
{
    double X  = (double)x, Y =  (double)y;
    double X1 = (double)x1, Y1 = (double)y1;
    double X2 = (double)x2, Y2 = (double)y2;
    double X3 = (double)x3, Y3 = (double)y3;

    *D  = (double)( (Y2 - Y3)*(X1 - X3) + (X3 - X2)*(Y1 - Y3) );
    *l1 = (double)( (Y2 - Y3)*(X  - X3) + (X3 - X2)*(Y  - Y3) );
    *l2 = (double)( (Y3 - Y1)*(X  - X3) + (X1 - X3)*(Y  - Y3) );
    *l3 = *D - *l1 - *l2;

    #define EPS -1e-14
    if (*l1 >= EPS && *l2 >= EPS && *l3 >= EPS)
    {
        return true;
    } else return false;
    #undef EPS
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
            double xf = (double)x + 0.5;
            double yf = (double)y + 0.5;

            if (is_in_triangle(xf, yf, x1, y1, x2, y2, x3, y3, &D, &l1, &l2, &l3))
                PUT_PIXEL(img, x, y, blend_colors(color, GET_PIXEL(img, x, y)));
        }
    }
}

void draw_triangle_aa
(
    struct Image *map,
    int x1, int y1,
    int x2, int y2,
    int x3, int y3
)
{
    int left, right;
    int bottom, top;

    if (find_bbox(map, x1, y1, x2, y2, x3, y3, &left, &right, &bottom, &top))
    for (int x = left; x <= right; ++x)
    {
        for (int y = top; y <= bottom; ++y)
        {
            double D, l1, l2, l3;
            double xf = (double)x;
            double yf = (double)y;

            uint32_t alpha = 0;

            if (is_in_triangle(xf+0.5, yf+0.5, x1, y1, x2, y2, x3, y3, &D, &l1, &l2, &l3))
                ++alpha;
            if (is_in_triangle(xf+0.5, yf-0.5, x1, y1, x2, y2, x3, y3, &D, &l1, &l2, &l3))
                ++alpha;
            if (is_in_triangle(xf-0.5, yf-0.5, x1, y1, x2, y2, x3, y3, &D, &l1, &l2, &l3))
                ++alpha;
            if (is_in_triangle(xf-0.5, yf+0.5, x1, y1, x2, y2, x3, y3, &D, &l1, &l2, &l3))
                ++alpha;

            /*
            if (is_in_triangle(xf+0.25, yf+0.25, x1, y1, x2, y2, x3, y3, &D, &l1, &l2, &l3))
                ++alpha;
            if (is_in_triangle(xf+0.25, yf-0.25, x1, y1, x2, y2, x3, y3, &D, &l1, &l2, &l3))
                ++alpha;
            if (is_in_triangle(xf-0.25, yf-0.25, x1, y1, x2, y2, x3, y3, &D, &l1, &l2, &l3))
                ++alpha;
            if (is_in_triangle(xf-0.25, yf+0.25, x1, y1, x2, y2, x3, y3, &D, &l1, &l2, &l3))
                ++alpha;
            */

            uint32_t *p = &(map->data[y*map->width + x]);
            *p += alpha;
            if (*p > 4) *p = 4;
        }
    }
}

void draw_sprite
(
    struct Image *img,
    struct Image *sprite,
    int x0, int y0
)
{
    int left, right;
    int bottom, top;

    left = x0, right = x0+sprite->width;
    if (left < 0)           { left = 0; }
    if (right < 0)          { return; }
    if (left > img->width)  { return; }
    if (right > img->width) { right = img->width-1; }

    top = y0, bottom = y0+sprite->height;
    if (top < 0)              { top = 0; }
    if (bottom < 0)           { return; }
    if (top > img->height)    { return; }
    if (bottom > img->height) { bottom = img->height-1; }

    for (int x = 0; x < sprite->width; ++x)
    {
        for (int y = 0; y < sprite->height; ++y)
        {
            PUT_PIXEL(img, left+x, top+y, GET_PIXEL(sprite, x, y));
        }
    }
}

void fill_bg(struct Image *img, uint32_t color)
{
    for (int x = 0; x < img->width; ++x)
    {
        for (int y = 0; y < img->height; ++y)
        {
            PUT_PIXEL(img, x, y, color);
        }
    }
}

int main(void)
{
    struct Image img = {0};

    img.width = 800;
    img.height = 600;
    img.data = malloc((size_t)(img.width * img.height * 4));

    fill_bg(&img, RGB(42, 42, 42));


    #if 1
        double xscale  = 100.0, yscale  = -100.0;
        double xoffset = 300.0, yoffset = 200.0;

        #define w2sx(x) (int)((x)*xscale + xoffset)
        #define w2sy(y) (int)((y)*yscale + yoffset)


        double wxs[] = {1.0,  1.0, -1.0, -1.0};
        double wys[] = {1.0, -1.0, -1.0,  1.0};
        double angle = (60)*M_PI / 180.0;

        int sxs[4], sys[4];

        for (int i = 0; i < 4; ++i)
        {
            double x = wxs[i], y = wys[i];
            wxs[i] = x*cos(angle) - y*sin(angle);
            wys[i] = x*sin(angle) + y*cos(angle);
        }
        for (int i = 0; i < 4; ++i) { sxs[i] = w2sx(wxs[i]); sys[i] = w2sy(wys[i]); }

        struct Image map = {0};
        map.width = 600;
        map.height = 400;
        map.data = malloc((size_t)(map.width * map.height) * 4);

        draw_triangle_aa(&map, sxs[0], sys[0], sxs[1], sys[1], sxs[2], sys[2]);
        draw_triangle_aa(&map, sxs[2], sys[2], sxs[3], sys[3], sxs[0], sys[0]);

        for (int x = 0; x < map.width; ++x)
        {
            for (int y = 0; y < map.height; ++y)
            {
                uint32_t alpha = GET_PIXEL(&map, x, y);
                if (alpha > 0)
                {
                    uint32_t color = RGBA(255, 0, 0, (uint32_t)((255*alpha) / 4));
                    PUT_PIXEL(&map, x, y, blend_colors(color, GET_PIXEL(&img, x, y)));
                }
                else
                {
                    PUT_PIXEL(&map, x, y, RGBA(42, 42, 42, 255));
                }
            }
        }

        draw_sprite(&img, &map, 100, 100);
    #endif


    stbi_write_png("image.png", img.width, img.height, 4, (void *)img.data, img.width * 4);
    free(img.data);
    return 0;
}
