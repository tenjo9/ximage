#include <math.h>
#include <stdlib.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define FONT_WIDTH 5
#define FONT_HEIGHT 6

unsigned char glyphs[][FONT_WIDTH * FONT_HEIGHT * 3] =
{
    ['a'] =
    {
        0, 0, 0, 0, 0,
        0, 1, 1, 0, 0,
        0, 0, 0, 1, 0,
        0, 1, 1, 1, 0,
        1, 0, 0, 1, 0,
        0, 1, 1, 1, 0,
    },

    ['A'] =
    {
        0, 0, 1, 0, 0,
        0, 1, 0, 1, 0,
        1, 0, 0, 0, 1,
        1, 1, 1, 1, 1,
        1, 0, 0, 0, 1,
        1, 0, 0, 0, 1,
    },
};

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

void draw_glyph(
                struct Image *img,
                int gwidth, int gheight,
                int x1, int y1,
                int width, int height,
                unsigned char *glyph,
                unsigned char *color
)
{
    int xside = width / gwidth;
    int yside = height / gheight;

    for (int i = 0; i < gwidth; ++i)
    {
        for (int j = 0; j < gheight; ++j)
        {
            if (glyph[j*gwidth + i])
                draw_rect(img, x1 + i*xside, y1 + j*yside, xside, yside, color);
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
        double scale = 3.0;
        unsigned char white[] = {255, 255, 255};
        int xcursor = 100;
        int ycursor = 100;

        draw_glyph(&img,
                FONT_WIDTH, FONT_HEIGHT,
                xcursor, ycursor,
                FONT_WIDTH*scale, FONT_HEIGHT*scale,
                glyphs['A'],
                white);
        xcursor += FONT_WIDTH*scale + scale;

        draw_glyph(&img,
                FONT_WIDTH, FONT_HEIGHT,
                xcursor, ycursor,
                FONT_WIDTH*scale, FONT_HEIGHT*scale,
                glyphs['a'],
                white);
    }


    stbi_write_png("image.png", img.width, img.height, 3, (void *)img.data, img.width * 3);
    return 0;
}
