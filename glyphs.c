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

void draw_rect(struct Image *img, int x0, int y0, int width, int height, unsigned char *color)
{
    for (int x = x0; x < x0+width; ++x)
    {
        for (int y = y0; y < y0+height; ++y)
        {
            unsigned char *p = get_pixel(img, x, y);
            p[0] = color[0];
            p[1] = color[1];
            p[2] = color[2];
        }
    }
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
                p[0] = 0;
                p[1] = 0;
                p[2] = 0;
            }
        }
    }


    if (1)
    {
        unsigned char glyph[] =
        {
            0, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 1, 1, 1, 0,
            1, 0, 0, 0, 0,
            0, 1, 1, 1, 1,
        };
    
        int glyph_width = 5;
        int glyph_height = 6;

        int scale = 10;

        int x0 = 100, y0 = 100;
        int x1 = x0+glyph_width*scale, y1 = y0+glyph_height*scale;

        int xside = (x1-x0) / glyph_width;
        int yside = (y1-y0) / glyph_height;

        unsigned char white[] = {255, 255, 255};

        for (int i = 0; i < glyph_width; ++i)
        {
            for (int j = 0; j < glyph_height; ++j)
            {
                if (glyph[j*glyph_width + i])
                    draw_rect(&img, x0 + i*xside, y0 + j*yside, xside, yside, white);
            }
        }
    }


    stbi_write_png("image.png", img.width, img.height, 3, (void *)img.data, img.width * 3);
    return 0;
}
