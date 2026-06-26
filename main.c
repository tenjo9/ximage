#include <math.h>
#include <stdlib.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
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
        int r = 100;
        int x0 = img.width/2.0, y0 = img.height/2.0;
        for (int x = x0-r; x < x0+r; ++x)
        {
            for (int y = y0-r; y < y0+r; ++y)
            {
                double mx = (double)x - (double)x0 + 0.5;
                double my = (double)y - (double)y0 + 0.5;
                int cond = ( mx*mx + my*my - r*r ) <= 0.00001;
                if (cond)
                {
                    unsigned char *p = get_pixel(&img, x, y);
                    if (p)
                    {
                        p[0] = round( (((double)x - x0-r) / (r*2.0) ) * 255.0);
                        p[1] = 50;
                        p[2] = 200;
                    }
                }
            }
        }


        if (1)
        {
            double xscale = r, yscale = -r;
            double xoffset = x0, yoffset = y0;

            double x1f = -sqrt(3.0)/2.0, y1f = -0.5;
            double x2f = 0.0,            y2f = 1.0;
            double x3f = sqrt(3.0)/2.0,  y3f = -0.5;

            int x1 = (x1f) * xscale + xoffset, y1 = (y1f) * yscale + yoffset;
            int x2 = (x2f) * xscale + xoffset, y2 = (y2f) * yscale + yoffset;
            int x3 = (x3f) * xscale + xoffset, y3 = (y3f) * yscale + yoffset;

            int left   = x0-r;
            int right  = x0+r;
            int top    = y0-r;
            int bottom = y0+r;


            if (1)
            for (int x = left; x <= right; ++x)
            {
                for (int y = top; y <= bottom; ++y)
                {
                    double D = (y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3);
                    double l1 = ( (y2 - y3)*(x - x3) + (x3 - x2)*(y - y3) )/D;
                    double l2 = ( (y3 - y1)*(x - x3) + (x1 - x3)*(y - y3) )/D;
                    double l3 = 1 - l1 - l2;

                    if (l1 > 0.0 && l2 > 0.0 && l3 > 0.0)
                    {
                        unsigned char *p = get_pixel(&img, x, y);
                        if (p)
                        {
                            p[0] = 70;
                            p[1] = round( (((double)x - x1) / (x3-x1)) * 255.0);
                            p[2] = 200;
                        }
                    }
                }
            }
        }
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
