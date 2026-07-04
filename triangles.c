
#include <stdint.h>
#include <stdlib.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

struct Image
{
    int width, height;
    uint32_t *data;
};

uint32_t *get_pixel(struct Image *img, int x, int y)
{
    if (x < 0 || x >= img->width || y < 0 || y >= img->height) { return NULL; }
    return &img->data[(y*img->width + x)];
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

    left = x1; right = x1;
    if (left  > x2) { left  = x2; }
    if (left  > x3) { left  = x3; }
    if (right < x2) { right = x2; }
    if (right < x3) { right = x3; }

    bottom = y1; top = y1;
    if (bottom < y2)  { bottom = y2; }
    if (bottom < y3)  { bottom = y3; }
    if (top    > y2)  { top    = y2; }
    if (top    > y3)  { top    = y3; }

    for (int x = left; x <= right; ++x)
    {
        for (int y = top; y <= bottom; ++y)
        {
            double D  = (y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3);
            double l1 = ( (y2 - y3)*(x - x3) + (x3 - x2)*(y - y3) )/D;
            double l2 = ( (y3 - y1)*(x - x3) + (x1 - x3)*(y - y3) )/D;
            double l3 = 1.0 - l1 - l2;

            if (l1 >= -1e-14 && l2 >= -1e-14 && l3 >= -1e-14)
            {
                uint32_t *p = get_pixel(img, x, y);
                if (p) { *p = color; }
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
    uint32_t (*color)(double x, double y, double l1, double l2, double l3)
)
{
    int left, right;
    int bottom, top;

    left = x1; right = x1;
    if (left  > x2) { left  = x2; }
    if (left  > x3) { left  = x3; }
    if (right < x2) { right = x2; }
    if (right < x3) { right = x3; }

    bottom = y1; top = y1;
    if (bottom < y2)  { bottom = y2; }
    if (bottom < y3)  { bottom = y3; }
    if (top    > y2)  { top    = y2; }
    if (top    > y3)  { top    = y3; }

    for (int x = left; x <= right; ++x)
    {
        for (int y = top; y <= bottom; ++y)
        {
            double D  = (y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3);
            double l1 = ( (y2 - y3)*(x - x3) + (x3 - x2)*(y - y3) )/D;
            double l2 = ( (y3 - y1)*(x - x3) + (x1 - x3)*(y - y3) )/D;
            double l3 = 1.0 - l1 - l2;

            if (l1 >= -1e-14 && l2 >= -1e-14 && l3 >= -1e-14)
            {
                uint32_t *p = get_pixel(img, x, y);
                if (p) { *p = color(x, y, l1, l2, l3); }
            }
        }
    }
}

uint32_t color1(double x, double y, double l1, double l2, double l3)
{
    (void)x; (void)y;

    double r = l1;
    double g = l2;
    double b = l3;

    return
        (uint32_t)(r * 255)       |
        (uint32_t)(g * 255) << 8  |
        (uint32_t)(b * 255) << 16 |
        (uint32_t)(0xff)    << 24;
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
                uint32_t *p = get_pixel(&img, x, y);
                *p = 0xFF191919;
                //*p = (uint32_t)0x00 | (uint32_t)0x00 << 8 | (uint32_t)0xff << 16 | (uint32_t)0xff << 24;
            }
        }
    }


    if (1)
    {
        double xscale  = 200.0, yscale  = -200.0;
        double xoffset = 400.0, yoffset =  300.0;

        #define w2sx(x) (int)((x)*xscale + xoffset)
        #define w2sy(y) (int)((y)*yscale + yoffset)


        double wxs[] = {1.0,  1.0, -1.0, -1.0};
        double wys[] = {1.0, -1.0, -1.0,  1.0};
        double angle = (0)*M_PI / 180.0;

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


    if (1)
    {
        struct Image buf = {0};
        buf.width = img.width;
        buf.height = img.height;
        buf.data = malloc((size_t)(buf.width * buf.height * 4));

        for (int x = 0; x < img.width; ++x)
        {
            for (int y = 0; y < img.height; ++y)
            {
                uint32_t count = 0;
                uint32_t r = 0;
                uint32_t g = 0;
                uint32_t b = 0;

                for (int i = -1; i <= 1; ++i)
                {
                    for (int j = -1; j <= 1; ++j)
                    {
                        int dx = x + i;
                        int dy = y + j;
                        uint32_t *p = get_pixel(&img, dx, dy);
                        if (p)
                        {
                            ++count;
                            r += (*p)       & 0xFF;
                            g += (*p >> 8)  & 0xFF;
                            b += (*p >> 16) & 0xFF;
                        }
                    }
                }

                r /= count;
                g /= count;
                b /= count;

                uint32_t *p = get_pixel(&buf, x, y);
                *p = (uint32_t)(r) | (uint32_t)(g) << 8 | (uint32_t)(b) << 16 | (uint32_t)(0xff) << 24;
            }
        }

        free(img.data);
        img.data = buf.data;
    }


    stbi_write_png("image.png", img.width, img.height, 4, (void *)img.data, img.width * 4);
    free(img.data);
    return 0;
}
