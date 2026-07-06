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

void draw_lines(struct Image *img, int lines, int y1, int y2, unsigned char *colors)
{
    double side = (double)img->width/(double)lines;
    for (int i = 0; i < lines; ++i)
    {
        double x1f = i*side;
        double x2f = x1f + side;
        int x1 = floor(x1f);
        int x2 = ceil(x2f);
        for (int x = x1; x < x2; ++x)
        {
            for (int y = y1; y < y2; ++y)
            {
                unsigned char *p = get_pixel(img, x, y);
                if (p)
                {
                    unsigned char *color = colors + (i*3);
                    p[0] = color[0];
                    p[1] = color[1];
                    p[2] = color[2];
                }
            }
        }
    }
}

int main(void)
{
    struct Image img = {0};

    img.width = 1920;
    img.height = 1200;
    img.data = malloc(img.width * img.height * 3);


    if (1)
    {
        for (int x = 0; x < img.width; ++x)
        {
            for (int y = 0; y < img.height; ++y)
            {
                unsigned char *p = get_pixel(&img, x, y);
                p[0] = 32;
                p[1] = 32;
                p[2] = 32;
            }
        }
    }


    if (1)
    {
        int lines_a = 7;
        int lines_b = 7;
        int lines_c = 6;

        double side_a = (double)img.width/(double)lines_a;
        double side_b = (double)img.width/(double)lines_b;
        double side_c = (double)img.width/(double)lines_c;

        int deviders[] = {900, 1000};

        unsigned char colors_a[] =
        {
            242,  242,  242,
            230,  242,   25,
             25,  240,  180,
              0,  255,   70,
            180,   20,  255,
            255,   20,   20,
             20,   20,  255,
        };

        unsigned char colors_b[] =
        {
             20,   20,  255,
             35,   35,   35,
            180,   20,  255,
             30,   30,   30,
             25,  240,  180,
             10,   10,   10,
            242,  242,  242,
        };

        unsigned char colors_c[] =
        {
             10,   10,   10,
             20,   20,   20,
             30,   30,   30,
             40,   40,   40,
             50,   50,   50,
             60,   60,   60,
        };

        draw_lines(&img, lines_a, 0, deviders[0], colors_a);
        draw_lines(&img, lines_b, deviders[0], deviders[1], colors_b);
        draw_lines(&img, lines_c, deviders[1], img.height, colors_c);
    }


    stbi_write_png("image.png", img.width, img.height, 3, (void *)img.data, img.width * 3);
    return 0;
}
