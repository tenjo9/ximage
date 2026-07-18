#include <math.h>
#include <stdlib.h>

#include "stb_image_write.h"

struct Image
{
    int width, height;
    unsigned char *data;
};

unsigned char *get_pixel(struct Image *img, int x, int y)
{
    if (x < 0 || x >= img->width || y < 0 || y >= img->height) { return NULL; }
    return &img->data[(y * img->width + x) * 3];
}

#define draw_triangle_macro(img, x1, y1, x2, y2, x3, y3, r, g, b)\
do\
{\
    int left, right;\
    int bottom, top;\
\
    left = x1;\
    right = x1;\
    if (left > x2)  { left = x2; }\
    if (left > x3)  { left = x3; }\
    if (right < x2) { right = x2; }\
    if (right < x3) { right = x3; }\
\
    bottom = y1;\
    top = y1;\
    if (bottom < y2)  { bottom = y2; }\
    if (bottom < y3)  { bottom = y3; }\
    if (top > y2)     { top = y2; }\
    if (top > y3)     { top = y3; }\
\
    for (int x = left; x <= right; ++x)\
    {\
        for (int y = top; y <= bottom; ++y)\
        {\
            double D = (y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3);\
            double l1 = ( (y2 - y3)*(x - x3) + (x3 - x2)*(y - y3) )/D;\
            double l2 = ( (y3 - y1)*(x - x3) + (x1 - x3)*(y - y3) )/D;\
            double l3 = 1.0 - l1 - l2;\
\
            if (l1 >= -1e-14 && l2 >= -1e-14 && l3 >= -1e-14)\
            {\
                unsigned char *p = get_pixel(img, x, y);\
                if (p)\
                {\
                    p[0] = r;\
                    p[1] = g;\
                    p[2] = b;\
                }\
            }\
        }\
    }\
}\
while (0);

int main(void)
{
    struct Image img = {0};

    img.width = 800;
    img.height = 600;
    img.data = malloc(img.width * img.height * 3);


    if (1)
    {
        for (int x = 0; x < img.width; ++x)
        {
            for (int y = 0; y < img.height; ++y)
            {
                unsigned char *p = get_pixel(&img, x, y);
                p[0] = ((double)(x + y) / (double)(img.width + img.height)) * 255;
                p[1] = ((double)x / img.width) * 255;
                p[2] = 100;
            }
        }
    }


    if (1)
    {
        double r = (0.3 * (double)img.width);
        double x0 = (double)img.width/2.0, y0 = (double)img.height/2.0;

        if (1)
        {
            double left = floor(x0-r), right = ceil(x0+r);
            double top = floor(y0-r), bottom = ceil(y0+r);

            for (int x = (int)left; x < (int)right; ++x)
            {
                for (int y = (int)top; y < (int)bottom; ++y)
                {
                    double mx = (double)x - (double)x0 + 0.5;
                    double my = (double)y - (double)y0 + 0.5;
                    int cond = ( mx*mx + my*my - r*r ) <= 0.00001;
                    if (cond)
                    {
                        unsigned char *p = get_pixel(&img, x, y);
                        if (p)
                        {
                            p[0] = round( (((double)x - (x0-r)) / (r*2.0) ) * 255.0);
                            p[1] = 50;
                            p[2] = 200;
                        }
                    }
                }
            }
        }


        if (1)
        {
            double xscale = r, yscale = -r;
            double xoffset = x0, yoffset = y0;
            #define c2sx(x) ((x)*xscale + xoffset)
            #define c2sy(y) ((y)*yscale + yoffset)

            double x1f = -sqrt(3.0)/2.0, y1f = -0.5;
            double x2f = 0.0,            y2f = 1.0;
            double x3f = sqrt(3.0)/2.0,  y3f = -0.5;

            double x1 = c2sx(x1f), y1 = c2sy(y1f);
            double x2 = c2sx(x2f), y2 = c2sy(y2f);
            double x3 = c2sx(x3f), y3 = c2sy(y3f);

            draw_triangle_macro(&img, x1, y1, x2, y2, x3, y3,
                70,
                255 - round( (((double)x - x1) / (x3-x1)) * 255.0),
                200
            );

            double x4f = (x1f+x2f)*0.5, y4f = (y1f+y2f)*0.5;
            double x5f = (x2f+x3f)*0.5, y5f = (y2f+y3f)*0.5;
            double x6f = (x3f+x1f)*0.5, y6f = (y3f+y1f)*0.5;

            double x4 = c2sx(x4f), y4 = c2sy(y4f);
            double x5 = c2sx(x5f), y5 = c2sy(y5f);
            double x6 = c2sx(x6f), y6 = c2sy(y6f);

            draw_triangle_macro(&img, x4, y4, x5, y5, x6, y6,
                (unsigned char)(l1 * 255.0),
                (unsigned char)(l2 * 255.0),
                (unsigned char)(l3 * 255.0)
            );
        }
    }


    if (0)
    {
        draw_triangle_macro(&img, 400, 100, 700, 300, 400, 500,
            (unsigned char)(l1 * 255),
            (unsigned char)(l2 * 255),
            (unsigned char)(l3 * 255)
        );

        draw_triangle_macro(&img, 400, 500, 100, 300, 400, 100,
            (unsigned char)(l1 * 255),
            (unsigned char)(l2 * 255),
            (unsigned char)(l3 * 255)
        );


        draw_triangle_macro(&img, 0, 0, 20, 0, 0, 20,
            (unsigned char)(l1 * 255),
            (unsigned char)(l2 * 255),
            (unsigned char)(l3 * 255)
        );

        draw_triangle_macro(&img, 0, 20, 20, 20, 20, 0,
            (unsigned char)(l1 * 255),
            (unsigned char)(l2 * 255),
            (unsigned char)(l3 * 255)
        );
    }


    if (1)
    for (int x = 0; x < img.width; ++x)
    {
        for (int y = 0; y < img.height; ++y)
        {
            int count = 0;
            int r = 0;
            int g = 0;
            int b = 0;

            for (int i = -1; i <= 1; ++i)
            {
                for (int j = -1; j <= 1; ++j)
                {
                    int dx = x + i;
                    int dy = y + j;
                    unsigned char *p = get_pixel(&img, dx, dy);
                    if (p)
                    {
                        ++count;
                        r += p[0];
                        g += p[1];
                        b += p[2];
                    }
                }
            }

            r /= count;
            g /= count;
            b /= count;

            unsigned char *p = get_pixel(&img, x, y);
            p[0] = r;
            p[1] = g;
            p[2] = b;
        }
    }


    stbi_write_png("image.png", img.width, img.height, 3, (void *)img.data, img.width * 3);
    return 0;
}
