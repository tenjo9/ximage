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
    if (*right > img->width) { *right = img->width-1; }

    *bottom = y1; *top = y1;
    if (*bottom < y2)  { *bottom = y2; }
    if (*bottom < y3)  { *bottom = y3; }
    if (*top    > y2)  { *top    = y2; }
    if (*top    > y3)  { *top    = y3; }
    if (*top    < 0)           { *top = 0; }
    if (*bottom < 0)           { return false; }
    if (*top    > img->height) { return false; }
    if (*bottom > img->height) { *bottom = img->height-1; }

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

void draw_circle(struct Image *img, int x0, int y0, int r, uint32_t color)
{
    double left = x0-r, right  = x0+r;
    double top  = y0-r, bottom = y0+r;

    if (right < 0)          { return; }
    if (left  > img->width) { return; }
    if (left  < 0)          { left = 0; }
    if (right > img->width) { right = img->width; }

    if (bottom < 0)             { return; }
    if (top    > img->height)   { return; }
    if (top    < 0)             { top = 0; }
    if (bottom > img->height ) { bottom = img->height; }

    for (int x = (int)left; x < (int)right; ++x)
    {
        for (int y = (int)top; y < (int)bottom; ++y)
        {
            double mx = (double)x - (double)x0 + 0.5;
            double my = (double)y - (double)y0 + 0.5;
            int cond = ( mx*mx + my*my - r*r ) <= 0.00001;
            if (cond) { PUT_PIXEL(img, x, y, color); }
        }
    }
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
    (void)x; (void)y; (void)left; (void)right; (void)bottom; (void)top;

    double r = l1 * 255;
    double g = l2 * 255;
    double b = l3 * 255;

    return RGBA(r, g, b, 255);
}

uint32_t color2(double x, double y, double left, double right, double bottom, double top, double l1, double l2, double l3)
{
    (void)x; (void)y; (void)left; (void)right; (void)bottom; (void)top; (void)l1; (void)l2; (void)l3;

    double r = x / (800.0) * 255.0;
    double g = 50;
    double b = 100;

    return RGBA(r, g, b, 255);
}

typedef struct {int x, y;} v2i;
typedef struct {double x, y;} v2f;


v2f v2ff(double x, double y) { return (v2f){x, y}; }
v2f v2fi(v2i v) { return (v2f){(double)v.x, (double)v.y}; }
v2f v2f_neg(v2f v) { return (v2f){-v.x, -v.y}; }

v2f v2f_fold(v2f v1, v2f v2) { return (v2f){v1.x + v2.x, v1.y + v2.y}; }
v2f v2f_sub (v2f v1, v2f v2) { return (v2f){v1.x - v2.x, v1.y - v2.y}; }
v2f v2f_mul (v2f v1, v2f v2) { return (v2f){v1.x * v2.x, v1.y * v2.y}; }
v2f v2f_div (v2f v1, v2f v2) { return (v2f){v1.x / v2.x, v1.y / v2.y}; }

v2f v2f_mulv (v2f v, double k) { return (v2f){v.x * k, v.y * k}; }
v2f v2f_divv (v2f v, double k) { return (v2f){v.x / k, v.y / k}; }

v2f v2f_norm(v2f v)
{
    double norm = sqrt(v.x*v.x + v.y*v.y);
    return (v2f){ v.x / norm, v.y / norm };
}


v2i v2ii(int x, int y) { return (v2i){x, y}; }
v2i v2if(v2f v) { return (v2i){(int)round(v.x), (int)round(v.y)}; }
v2i v2i_neg(v2i v) { return (v2i){-v.x, -v.y}; }

v2i v2i_fold(v2i v1, v2i v2) { return (v2i){v1.x + v2.x, v1.y + v2.y}; }
v2i v2i_sub (v2i v1, v2i v2) { return (v2i){v1.x - v2.x, v1.y - v2.y}; }
v2i v2i_mul (v2i v1, v2i v2) { return (v2i){v1.x * v2.x, v1.y * v2.y}; }
v2i v2i_div (v2i v1, v2i v2) { return (v2i){v1.x / v2.x, v1.y / v2.y}; }

v2i v2i_mulv (v2i v, int k) { return (v2i){v.x * k, v.y * k}; }
v2i v2i_divv (v2i v, int k) { return (v2i){v.x / k, v.y / k}; }

v2i v2i_norm(v2i v)
{
    v2f vf = v2fi(v);
    double norm = sqrt(vf.x*vf.x + vf.y*vf.y);
    return (v2i){ (int)round(vf.x / norm), (int)round(vf.y / norm) };
}


v2f *points = NULL;
size_t points_size = 0;

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


    #if 1
        double left = -10.0, right = 10.0;
        double bottom = -10.0, top = 10.0;

        v2f scale =
        {
             (double)img.width  / (right - left),
            -(double)img.height / (top - bottom),
        };

        v2f offset =
        {
            (double)img.width /2.0 - scale.x*(left + right)/2.0,
            (double)img.height/2.0 - scale.y*(bottom + top)/2.0,
        };

        #define w2s(v) v2f_fold(v2f_mul(v, scale), offset)

        points_size = 100;
        points = malloc(sizeof(v2f) * points_size);

        #if 0
        double x = -5.0;
        double dx = (5.0 - (-5.0)) / (double)points_size;
        #else
        double x = left;
        double dx = (right - left) / ((double)points_size-1);
        #endif

        for (size_t i = 0; i < points_size; ++i)
        {
            double y = sin(x);
            points[i] = w2s(v2ff(x, y));
            x += dx;
        }

        v2i v = v2if(w2s(v2ff(10.0, -10.0)));
        draw_circle(&img, v.x, v.y, 5, 0xffff0000);
    #endif


    #if 1
        int k = 20;
        k /= 2;

        size_t normales_size = points_size * 2;
        v2i *normales = malloc(normales_size * sizeof(v2i));
        printf("%lu\n", normales_size);

        for (size_t i = 0; i < points_size-1; ++i)
        {
            v2f t  = v2f_sub(points[i+1], points[i]);
            v2f n1 = v2f_mulv(v2f_norm(v2ff( t.y, -t.x)), k);
            v2f n2 = v2f_mulv(v2f_norm(v2ff(-t.y,  t.x)), k);

            normales[i*2]     = v2if(v2f_fold(n1, points[i]));
            normales[i*2 + 1] = v2if(v2f_fold(n2, points[i]));
        }

        {
            v2f t  = v2f_sub(points[points_size-2], points[points_size-1]);
            v2f n1 = v2f_mulv(v2f_norm(v2ff( t.y, -t.x)), k);
            v2f n2 = v2f_mulv(v2f_norm(v2ff(-t.y,  t.x)), k);

            normales[normales_size-2] = v2if(v2f_fold(n2, points[points_size-1]));
            normales[normales_size-1] = v2if(v2f_fold(n1, points[points_size-1]));
        }

        #if 0
        for (size_t i = 0; i < points_size; ++i)
        {
            v2i c  = v2if(points[i]);
            v2i n1 = normales[i*2];
            v2i n2 = normales[i*2+1];

            printf("(%d %d) (%d %d)\n", n1.x, n1.y, n2.x, n2.y);

            draw_circle(&img,  c.x,  c.y, 1, 0xFF00FF00);
            draw_circle(&img, n1.x, n1.y, 1, 0xFF0000FF);
            draw_circle(&img, n2.x, n2.y, 1, 0xFF0000FF);
        }

        #else
        for (size_t i = 0; i < normales_size-2; ++i)
        {
            v2i v1 = normales[i];
            v2i v2 = normales[i+1];
            v2i v3 = normales[i+2];

            #if 1
                draw_triangle_source(&img,
                    v1.x, v1.y,
                    v2.x, v2.y,
                    v3.x, v3.y,
                    color1);
            #else
                draw_triangle(&img,
                    v1.x, v1.y,
                    v2.x, v2.y,
                    v3.x, v3.y,
                    0xff0000ff);
            #endif
        }
        #endif
    #endif


    stbi_write_png("image.png", img.width, img.height, 4, (void *)img.data, img.width * 4);
    free(img.data);
    return 0;
}
