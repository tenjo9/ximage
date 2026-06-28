#include <math.h>
#include <stdlib.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

struct Glyph
{
    int width, height, base;
    unsigned char *bytes;
};

struct Glyph glyphs[] =
{
    ['a'] =
    {
        4, 5, 0,
        (unsigned char [])
        {
            0, 1, 1, 0,
            0, 0, 0, 1,
            0, 1, 1, 1,
            1, 0, 0, 1,
            0, 1, 1, 1,
        }
    },

    ['b'] =
    {
        4, 6, 0,
        (unsigned char [])
        {
            1, 0, 0, 0,
            1, 0, 0, 0,
            1, 1, 1, 0,
            1, 0, 0, 1,
            1, 0, 0, 1,
            1, 1, 1, 0,
        }
    },


    ['c'] =
    {
        4, 6, 0,
        (unsigned char [])
        {
            0, 0, 0, 0,
            0, 1, 1, 1,
            1, 0, 0, 0,
            1, 0, 0, 0,
            1, 0, 0, 0,
            0, 1, 1, 1,
        }
    },

    ['d'] =
    {
        4, 6, 0,
        (unsigned char [])
        {
            0, 0, 0, 1,
            0, 0, 0, 1,
            0, 1, 1, 1,
            1, 0, 0, 1,
            1, 0, 0, 1,
            0, 1, 1, 1,
        }
    },

    ['e'] =
    {
        4, 5, 0,
        (unsigned char [])
        {
            0, 1, 1, 0,
            1, 0, 0, 1,
            1, 1, 1, 0,
            1, 0, 0, 0,
            0, 1, 1, 1,
        }
    },

    ['f'] =
    {
        3, 5, 0,
        (unsigned char [])
        {
            0, 1, 1,
            1, 0, 0,
            1, 1, 1,
            1, 0, 0,
            1, 0, 0,
        }
    },

    ['g'] =
    {
        4, 7, 3,
        (unsigned char [])
        {
            0, 1, 1, 1,
            1, 0, 0, 1,
            1, 0, 0, 1,
            0, 1, 1, 1,
            0, 0, 0, 1,
            0, 0, 0, 1,
            1, 1, 1, 0,
        }
    },

    ['h'] =
    {
        4, 6, 0,
        (unsigned char [])
        {
            1, 0, 0, 0,
            1, 0, 0, 0,
            1, 1, 1, 0,
            1, 0, 0, 1,
            1, 0, 0, 1,
            1, 0, 0, 1,
        }
    },

    ['i'] =
    {
        3, 6, 0,
        (unsigned char [])
        {
            0, 1, 0,
            0, 0, 0,
            1, 1, 0,
            0, 1, 0,
            0, 1, 0,
            1, 1, 1,
        }
    },

    ['j'] =
    {
        2, 9, 3,
        (unsigned char [])
        {
            0, 1,
            0, 0,
            0, 1,
            0, 1,
            0, 1,
            0, 1,
            0, 1,
            0, 1,
            1, 0,
        }
    },

    ['k'] =
    {
        3, 6, 0,
        (unsigned char [])
        {
            1, 0, 0,
            1, 0, 1,
            1, 0, 1,
            1, 1, 0,
            1, 1, 0,
            1, 0, 1,
        }
    },

    ['l'] =
    {
        3, 6, 0,
        (unsigned char [])
        {
            1, 1, 0,
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,
            1, 1, 1,
        }
    },

    ['m'] =
    {
        5, 4, 0,
        (unsigned char [])
        {
            1, 1, 0, 1, 0,
            1, 0, 1, 0, 1,
            1, 0, 1, 0, 1,
            1, 0, 1, 0, 1,
        }
    },

    ['n'] =
    {
        3, 4, 0,
        (unsigned char [])
        {
            1, 1, 0,
            1, 0, 1,
            1, 0, 1,
            1, 0, 1,
        }
    },

    ['o'] =
    {
        4, 5, 0,
        (unsigned char [])
        {
            0, 1, 1, 0,
            1, 0, 0, 1,
            1, 0, 0, 1,
            1, 0, 0, 1,
            0, 1, 1, 0,
        }
    },

    ['p'] =
    {
        4, 7, 3,
        (unsigned char [])
        {
            1, 0, 1, 0,
            1, 1, 0, 1,
            1, 0, 0, 1,
            1, 1, 1, 0,
            1, 0, 0, 0,
            1, 0, 0, 0,
            1, 0, 0, 0,
        }
    },

    ['q'] =
    {
        4, 7, 3,
        (unsigned char [])
        {
            0, 1, 1, 0,
            1, 0, 0, 1,
            1, 0, 0, 1,
            0, 1, 1, 1,
            0, 0, 0, 1,
            0, 0, 0, 1,
            0, 0, 0, 1,
        }
    },

    ['r'] =
    {
        4, 5, 0,
        (unsigned char [])
        {
            1, 0, 1, 1,
            1, 1, 0, 0,
            1, 0, 0, 0,
            1, 0, 0, 0,
            1, 0, 0, 0,
        }
    },

    ['s'] =
    {
        4, 5, 0,
        (unsigned char [])
        {
            0, 1, 1, 1,
            1, 0, 0, 0,
            0, 1, 1, 0,
            0, 0, 0, 1,
            1, 1, 1, 0,
        }
    },


    ['t'] =
    {
        4, 5, 0,
        (unsigned char [])
        {
            0, 1, 0, 0,
            1, 1, 1, 0,
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
        }
    },

    ['u'] =
    {
        4, 5, 0,
        (unsigned char [])
        {
            1, 0, 0, 1,
            1, 0, 0, 1,
            1, 0, 0, 1,
            1, 0, 0, 1,
            0, 1, 1, 1,
        }
    },

    ['v'] =
    {
        3, 5, 0,
        (unsigned char [])
        {
            1, 0, 1,
            1, 0, 1,
            1, 0, 1,
            1, 0, 1,
            0, 1, 0,
        }
    },

    ['w'] =
    {
        5, 4, 0,
        (unsigned char [])
        {
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 1, 0, 1,
            0, 1, 0, 1, 0,
        }
    },

    ['x'] =
    {
        3, 5, 0,
        (unsigned char [])
        {
            1, 0, 1,
            1, 0, 1,
            0, 1, 0,
            1, 0, 1,
            1, 0, 1,
        }
    },

    ['y'] =
    {
        3, 5, 2,
        (unsigned char [])
        {
            1, 0, 1,
            1, 0, 1,
            0, 1, 1,
            0, 0, 1,
            1, 1, 0,
        }
    },

    ['z'] =
    {
        4, 4, 0,
        (unsigned char [])
        {
            1, 1, 1, 1,
            0, 0, 1, 0,
            0, 1, 0, 0,
            1, 1, 1, 1,
        }
    },


    ['A'] =
    {
        5, 6, 0,
        (unsigned char [])
        {
            0, 0, 1, 0, 0,
            0, 1, 0, 1, 0,
            1, 0, 0, 0, 1,
            1, 1, 1, 1, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
        }
    },

    ['B'] =
    {
        5, 6, 0,
        (unsigned char [])
        {
            1, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 1, 1, 1, 1,
        }
    },

    ['C'] =
    {
        5, 6, 0,
        (unsigned char [])
        {
            0, 1, 1, 1, 1,
            1, 0, 0, 0, 0,
            1, 0, 0, 0, 0,
            1, 0, 0, 0, 0,
            1, 0, 0, 0, 0,
            0, 1, 1, 1, 1,
        }
    },

    ['D'] =
    {
        5, 6, 0,
        (unsigned char [])
        {
            1, 1, 1, 0, 0,
            1, 0, 0, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 1, 0,
            1, 1, 1, 0, 0,
        }
    },

    ['E'] =
    {
        4, 6, 0,
        (unsigned char [])
        {
            1, 1, 1, 1,
            1, 0, 0, 0,
            1, 1, 1, 0,
            1, 0, 0, 0,
            1, 0, 0, 0,
            1, 1, 1, 1,
        }
    },

    ['F'] =
    {
        4, 6, 0,
        (unsigned char [])
        {
            1, 1, 1, 1,
            1, 0, 0, 0,
            1, 1, 1, 0,
            1, 0, 0, 0,
            1, 0, 0, 0,
            1, 0, 0, 0,
        }
    },

    ['G'] =
    {
        5, 6, 0,
        (unsigned char [])
        {
            0, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 0,
            1, 0, 1, 1, 1,
            1, 0, 0, 0, 1,
            1, 1, 1, 1, 0,
        }
    },

    ['H'] =
    {
        4, 6, 0,
        (unsigned char [])
        {
            1, 0, 0, 1,
            1, 0, 0, 1,
            1, 0, 0, 1,
            1, 1, 1, 1,
            1, 0, 0, 1,
            1, 0, 0, 1,
        }
    },

    ['I'] =
    {
        3, 6, 0,
        (unsigned char [])
        {
            1, 1, 1,
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,
            1, 1, 1,
        }
    },

    ['J'] =
    {
        3, 6, 0,
        (unsigned char [])
        {
            1, 1, 1,
            0, 0, 1,
            0, 0, 1,
            0, 0, 1,
            1, 0, 1,
            1, 1, 0,
        }
    },

    ['K'] =
    {
        4, 6, 0,
        (unsigned char [])
        {
            1, 0, 0, 1,
            1, 0, 0, 1,
            1, 0, 1, 0,
            1, 1, 0, 0,
            1, 0, 1, 0,
            1, 0, 0, 1,
        }
    },

    ['L'] =
    {
        3, 6, 0,
        (unsigned char [])
        {
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 1, 1,
        }
    },

    ['M'] =
    {
        5, 6, 0,
        (unsigned char [])
        {
            1, 0, 0, 0, 1,
            1, 1, 0, 1, 1,
            1, 0, 1, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
        }
    },

    ['N'] =
    {
        5, 6, 0,
        (unsigned char [])
        {
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 1, 0, 0, 1,
            1, 0, 1, 0, 1,
            1, 0, 0, 1, 1,
            1, 0, 0, 0, 1,
        }
    },

    ['O'] =
    {
        5, 6, 0,
        (unsigned char [])
        {
            0, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 0,
        }
    },

    ['P'] =
    {
        4, 6, 0,
        (unsigned char [])
        {
            1, 1, 1, 0,
            1, 0, 0, 1,
            1, 0, 0, 1,
            1, 1, 1, 0,
            1, 0, 0, 0,
            1, 0, 0, 0,
        }
    },

    ['Q'] =
    {
        5, 6, 1,
        (unsigned char [])
        {
            0, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 1, 0,
            0, 1, 1, 0, 1,
        }
    },

    ['R'] =
    {
        4, 6, 0,
        (unsigned char [])
        {
            1, 1, 1, 0,
            1, 0, 0, 1,
            1, 0, 0, 1,
            1, 1, 1, 0,
            1, 0, 0, 1,
            1, 0, 0, 1,
        }
    },

    ['S'] =
    {
        4, 6, 0,
        (unsigned char [])
        {
            0, 1, 1, 1,
            1, 0, 0, 0,
            0, 1, 1, 0,
            0, 0, 0, 1,
            0, 0, 0, 1,
            1, 1, 1, 0,
        }
    },

    ['T'] =
    {
        5, 6, 0,
        (unsigned char [])
        {
            1, 1, 1, 1, 1,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
        }
    },

    ['U'] =
    {
        4, 6, 0,
        (unsigned char [])
        {
            1, 0, 0, 1,
            1, 0, 0, 1,
            1, 0, 0, 1,
            1, 0, 0, 1,
            1, 0, 0, 1,
            0, 1, 1, 0,
        }
    },

    ['V'] =
    {
        5, 6, 0,
        (unsigned char [])
        {
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 0, 1, 0,
            0, 1, 0, 1, 0,
            0, 0, 1, 0, 0,
        }
    },

    ['W'] =
    {
        7, 6, 0,
        (unsigned char [])
        {
            1, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 1,
            0, 1, 0, 1, 0, 1, 0,
            0, 1, 0, 1, 0, 1, 0,
            0, 0, 1, 0, 1, 0, 0,
        }
    },

    ['X'] =
    {
        5, 6, 0,
        (unsigned char [])
        {
            1, 0, 0, 0, 1,
            0, 1, 0, 1, 0,
            0, 0, 1, 0, 0,
            0, 1, 0, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
        }
    },

    ['Y'] =
    {
        5, 6, 0,
        (unsigned char [])
        {
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 0, 1, 0,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
        }
    },

    ['Z'] =
    {
        4, 6, 0,
        (unsigned char [])
        {
            1, 1, 1, 1,
            0, 0, 0, 1,
            0, 0, 1, 0,
            0, 1, 0, 0,
            1, 0, 0, 0,
            1, 1, 1, 1,
        }
    },

    [' '] =
    {
        5, 1, 0,
        (unsigned char [])
        {
            0, 0, 0, 0, 0,
        }
    },

    ['.'] =
    {
        1, 1, 0,
        (unsigned char [])
        {
            1,
        }
    },

    [','] =
    {
        1, 2, 1,
        (unsigned char [])
        {
            1,
            1,
        }
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
            if (p)
            {
                p[0] = color[0];
                p[1] = color[1];
                p[2] = color[2];
            }
        }
    }
}

void draw_glyph(
                struct Image *img,
                struct Glyph *glyph,
                int x1, int y1,
                int width, int height,
                unsigned char *color
)
{
    int xside = width / glyph->width;
    int yside = height / glyph->height;

    for (int i = 0; i < glyph->width; ++i)
    {
        for (int j = 0; j < glyph->height; ++j)
        {
            if (glyph->bytes[j*glyph->width + i])
                draw_rect(img, x1 + i*xside, y1 + j*yside, xside, yside, color);
        }
    }
}

void draw_text(struct Image *img, char *text, int xpos, int ypos, int scale, int right, unsigned char *color)
{
    int xcursor = xpos;
    int ycursor = 0;
    int base = ypos;
    int max_height = 0;

    while (*text)
    {
        struct Glyph *g = &glyphs[*text];
        ycursor = base - g->height*scale + g->base*scale;
        draw_glyph(img, g, xcursor, ycursor, g->width*scale, g->height*scale, color);
        xcursor += g->width*scale + scale;
        ++text;

        max_height = g->height > max_height? g->height : max_height;

        if (xcursor > right)
        {
            xcursor = xpos;
            base = base + 2*scale + max_height*scale;
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
        char lorem[] = "To Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
        char abc[] = "Aa Bb Cc Dd Ee Ff Gg Hh Ii Jj Kk Ll Mm Nn Oo Pp Qq Rr Ss Tt Uu Vv Ww Xx Yy Zz";
        unsigned char white[] = {255, 255, 255};
        draw_text(&img, abc, 100, 100, 4, img.width-100, white);
    }


    stbi_write_png("image.png", img.width, img.height, 3, (void *)img.data, img.width * 3);
    return 0;
}
