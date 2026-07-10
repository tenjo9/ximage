#ifndef XIMAGE_H
#define XIMAGE_H


#include <stdint.h>
#include <stdbool.h>


#define XI_PUT_PIXEL(img, x, y, color)  (img)->data[(y)*(img)->width + (x)] = (color)
#define XI_GET_PIXEL(img, x, y)        &(img)->data[(y)*(img)->width + (x)]
#define XI_GET_COLOR(img, x, y)         (img)->data[(y)*(img)->width + (x)]

#define XI_RGBA(r, g, b, a) (uint32_t)(r) | (uint32_t)(g) << 8 | (uint32_t)(b) << 16 | (uint32_t)(a) << 24
#define XI_RGB(r, g, b)        (uint32_t)(r) | (uint32_t)(g) << 8 | (uint32_t)(b) << 16 | (uint32_t)(0xff) << 24

#define XI_GET_R(color) ((color) >> 0)  & 0xff
#define XI_GET_G(color) ((color) >> 8)  & 0xff
#define XI_GET_B(color) ((color) >> 16) & 0xff
#define XI_GET_A(color) ((color) >> 24) & 0xff


struct XImage
{
    int width, height;
    uint32_t *data;
};
typedef struct XImage XImage;


uint32_t xi_blend_colors(uint32_t colorA, uint32_t colorB);
bool xi_find_bbox(XImage *img, int x1, int y1, int x2, int y2, int x3, int y3, int *left, int *right, int *bottom, int *top);
bool xi_is_in_triangle(double x, double y, int x1, int y1, int x2, int y2, int x3, int y3, double *D, double *l1, double *l2, double *l3);

void xi_fill_bg(XImage *img, uint32_t color);
void xi_draw_circle(XImage *img, int x0, int y0, int r, uint32_t color);
void xi_draw_triangle(XImage *img, int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);


#ifdef XIMAGE_IMPLEMENTATION

uint32_t xi_blend_colors(uint32_t colorA, uint32_t colorB)
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

    return XI_RGBA(rC, gC, bC, aB);
}

bool xi_find_bbox
(
    XImage *img,
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
    if (*left  < 0)            { *left = 0; }
    if (*right < 0)            { return false; }
    if (*left  > img->width)   { return false; }
    if (*right > img->width-1) { *right = img->width-1; }

    *bottom = y1; *top = y1;
    if (*bottom < y2)  { *bottom = y2; }
    if (*bottom < y3)  { *bottom = y3; }
    if (*top    > y2)  { *top    = y2; }
    if (*top    > y3)  { *top    = y3; }
    if (*top    < 0)             { *top = 0; }
    if (*bottom < 0)             { return false; }
    if (*top    > img->height)   { return false; }
    if (*bottom > img->height-1) { *bottom = img->height-1; }

    return true;
}

bool xi_is_in_triangle
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
    *l1 = (double)( (Y2 - Y3)*(X  - X3) + (X3 - X2)*(Y  - Y3) ) / *D;
    *l2 = (double)( (Y3 - Y1)*(X  - X3) + (X1 - X3)*(Y  - Y3) ) / *D;
    *l3 = 1.0 - *l1 - *l2;

    #define EPS -1e-14
    if (*l1 >= EPS && *l2 >= EPS && *l3 >= EPS)
    {
        return true;
    }
    #undef EPS

    return false;
}

void xi_fill_bg(XImage *img, uint32_t color)
{
    for (int x = 0; x < img->width; ++x)
    {
        for (int y = 0; y < img->height; ++y)
        {
            XI_PUT_PIXEL(img, x, y, color);
        }
    }
}

void xi_draw_circle(XImage *img, int x0, int y0, int r, uint32_t color)
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
            if (cond) { XI_PUT_PIXEL(img, x, y, color); }
        }
    }
}

void xi_draw_triangle
(
    XImage *img,
    int x1, int y1,
    int x2, int y2,
    int x3, int y3,
    uint32_t color
)
{
    int left, right;
    int bottom, top;

    if (xi_find_bbox(img, x1, y1, x2, y2, x3, y3, &left, &right, &bottom, &top))
    for (int x = left; x <= right; ++x)
    {
        for (int y = top; y <= bottom; ++y)
        {
            double D, l1, l2, l3;
            if (xi_is_in_triangle(x, y, x1, y1, x2, y2, x3, y3, &D, &l1, &l2, &l3))
                XI_PUT_PIXEL(img, x, y, color);
        }
    }
}

#endif


#endif
