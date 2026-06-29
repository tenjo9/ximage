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
        5, 5, 0,
        (unsigned char [])
        {
            0, 1, 1, 1, 0,
            0, 0, 0, 0, 1,
            0, 1, 1, 1, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 1,
        }
    },

    ['b'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            1, 0, 0, 0, 0,
            1, 0, 0, 0, 0,
            1, 0, 1, 1, 0,
            1, 1, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 1, 1, 1, 0,
        }
    },


    ['c'] =
    {
        5, 5, 0,
        (unsigned char [])
        {
            0, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 0,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 0,
        }
    },

    ['d'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            0, 0, 0, 0, 1,
            0, 0, 0, 0, 1,
            0, 1, 1, 0, 1,
            1, 0, 0, 1, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 1,
        }
    },

    ['e'] =
    {
        5, 5, 0,
        (unsigned char [])
        {
            0, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 1, 1, 1, 1,
            1, 0, 0, 0, 0,
            0, 1, 1, 1, 1,
        }
    },

    ['f'] =
    {
        4, 7, 0,
        (unsigned char [])
        {
            0, 0, 1, 1,
            0, 1, 0, 0,
            1, 1, 1, 1,
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 1, 0, 0,
        }
    },

    ['g'] =
    {
        5, 6, 1,
        (unsigned char [])
        {
            0, 1, 1, 1, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 1,
            0, 0, 0, 0, 1,
            1, 1, 1, 1, 0,
        }
    },

    ['h'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            1, 0, 0, 0, 0,
            1, 0, 0, 0, 0,
            1, 0, 1, 1, 0,
            1, 1, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
        }
    },

    ['i'] =
    {
        1, 7, 0,
        (unsigned char [])
        {
            1,
            0,
            1,
            1,
            1,
            1,
            1,
        }
    },

    ['j'] =
    {
        5, 8, 1,
        (unsigned char [])
        {
            0, 0, 0, 0, 1,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 1,
            0, 0, 0, 0, 1,
            0, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 0,
        }
    },

    ['k'] =
    {
        4, 7, 0,
        (unsigned char [])
        {
            1, 0, 0, 0,
            1, 0, 0, 0,
            1, 0, 0, 1,
            1, 0, 1, 0,
            1, 1, 0, 0,
            1, 0, 1, 0,
            1, 0, 0, 1,
        }
    },

    ['l'] =
    {
        2, 7, 0,
        (unsigned char [])
        {
            1, 0,
            1, 0,
            1, 0,
            1, 0,
            1, 0,
            1, 0,
            0, 1,
        }
    },

    ['m'] =
    {
        5, 5, 0,
        (unsigned char [])
        {
            1, 1, 0, 1, 0,
            1, 0, 1, 0, 1,
            1, 0, 1, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
        }
    },

    ['n'] =
    {
        5, 5, 0,
        (unsigned char [])
        {
            1, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
        }
    },

    ['o'] =
    {
        5, 5, 0,
        (unsigned char [])
        {
            0, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 0,
        }
    },

    ['p'] =
    {
        5, 6, 1,
        (unsigned char [])
        {
            1, 0, 1, 1, 0,
            1, 1, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 1, 1, 1, 0,
            1, 0, 0, 0, 0,
            1, 0, 0, 0, 0,
        }
    },

    ['q'] =
    {
        5, 6, 1,
        (unsigned char [])
        {
            0, 1, 1, 0, 1,
            1, 0, 0, 1, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 1,
            0, 0, 0, 0, 1,
            0, 0, 0, 0, 1,
        }
    },

    ['r'] =
    {
        5, 5, 0,
        (unsigned char [])
        {
            1, 0, 1, 1, 0,
            1, 1, 0, 0, 1,
            1, 0, 0, 0, 0,
            1, 0, 0, 0, 0,
            1, 0, 0, 0, 0,
        }
    },

    ['s'] =
    {
        5, 5, 0,
        (unsigned char [])
        {
            0, 1, 1, 1, 1,
            1, 0, 0, 0, 0,
            0, 1, 1, 1, 0,
            0, 0, 0, 0, 1,
            1, 1, 1, 1, 0,
        }
    },


    ['t'] =
    {
        3, 7, 0,
        (unsigned char [])
        {
            0, 1, 0,
            0, 1, 0,
            1, 1, 1,
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,
            0, 0, 1,
        }
    },

    ['u'] =
    {
        5, 5, 0,
        (unsigned char [])
        {
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 1,
        }
    },

    ['v'] =
    {
        5, 5, 0,
        (unsigned char [])
        {
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 0, 1, 0,
            0, 0, 1, 0, 0,
        }
    },

    ['w'] =
    {
        5, 5, 0,
        (unsigned char [])
        {
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 1, 0, 1,
            1, 0, 1, 0, 1,
            0, 1, 1, 1, 1,
        }
    },

    ['x'] =
    {
        5, 5, 0,
        (unsigned char [])
        {
            1, 0, 0, 0, 1,
            0, 1, 0, 1, 0,
            0, 0, 1, 0, 0,
            0, 1, 0, 1, 0,
            1, 0, 0, 0, 1,
        }
    },

    ['y'] =
    {
        5, 6, 1,
        (unsigned char [])
        {
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 1,
            0, 0, 0, 0, 1,
            1, 1, 1, 1, 0,
        }
    },

    ['z'] =
    {
        5, 5, 0,
        (unsigned char [])
        {
            1, 1, 1, 1, 1,
            0, 0, 0, 1, 0,
            0, 0, 1, 0, 0,
            0, 1, 0, 0, 0,
            1, 1, 1, 1, 1,
        }
    },


    ['A'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            0, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 1, 1, 1, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
        }
    },

    ['B'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            1, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 1, 1, 1, 0,
        }
    },

    ['C'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            0, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 0,
            1, 0, 0, 0, 0,
            1, 0, 0, 0, 0,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 0,
        }
    },

    ['D'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            1, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 1, 1, 1, 0,
        }
    },

    ['E'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            1, 1, 1, 1, 1,
            1, 0, 0, 0, 0,
            1, 1, 1, 0, 0,
            1, 0, 0, 0, 0,
            1, 0, 0, 0, 0,
            1, 0, 0, 0, 0,
            1, 1, 1, 1, 1,
        }
    },

    ['F'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            1, 1, 1, 1, 1,
            1, 0, 0, 0, 0,
            1, 1, 1, 0, 0,
            1, 0, 0, 0, 0,
            1, 0, 0, 0, 0,
            1, 0, 0, 0, 0,
            1, 0, 0, 0, 0,
        }
    },

    ['G'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            0, 1, 1, 1, 1,
            1, 0, 0, 0, 0,
            1, 0, 1, 1, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 0,
        }
    },

    ['H'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 1, 1, 1, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
        }
    },

    ['I'] =
    {
        3, 7, 0,
        (unsigned char [])
        {
            1, 1, 1,
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,
            1, 1, 1,
        }
    },

    ['J'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            0, 0, 0, 0, 1,
            0, 0, 0, 0, 1,
            0, 0, 0, 0, 1,
            0, 0, 0, 0, 1,
            0, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 0,
        }
    },

    ['K'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            1, 0, 0, 0, 1,
            1, 0, 0, 1, 0,
            1, 1, 1, 0, 0,
            1, 0, 0, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
        }
    },

    ['L'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            1, 0, 0, 0, 0,
            1, 0, 0, 0, 0,
            1, 0, 0, 0, 0,
            1, 0, 0, 0, 0,
            1, 0, 0, 0, 0,
            1, 0, 0, 0, 0,
            1, 1, 1, 1, 1,
        }
    },

    ['M'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            1, 0, 0, 0, 1,
            1, 1, 0, 1, 1,
            1, 0, 1, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
        }
    },

    ['N'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            1, 0, 0, 0, 1,
            1, 1, 0, 0, 1,
            1, 0, 1, 0, 1,
            1, 0, 0, 1, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
        }
    },

    ['O'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            0, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 0,
        }
    },

    ['P'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            1, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 1, 1, 1, 0,
            1, 0, 0, 0, 0,
            1, 0, 0, 0, 0,
            1, 0, 0, 0, 0,
            1, 0, 0, 0, 0,
        }
    },

    ['Q'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            0, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 1, 0,
            0, 1, 1, 0, 1,
        }
    },

    ['R'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            1, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
        }
    },

    ['S'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            0, 1, 1, 1, 1,
            1, 0, 0, 0, 0,
            0, 1, 1, 1, 0,
            0, 0, 0, 0, 1,
            0, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 0,
        }
    },

    ['T'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            1, 1, 1, 1, 1,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
        }
    },

    ['U'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 0,
        }
    },

    ['V'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            1, 0, 0, 0, 1,
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
        5, 7, 0,
        (unsigned char [])
        {
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 1, 0, 1,
            1, 1, 0, 1, 1,
            1, 0, 0, 0, 1,
        }
    },

    ['X'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            1, 0, 0, 0, 1,
            0, 1, 0, 1, 0,
            0, 0, 1, 0, 0,
            0, 1, 0, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
        }
    },

    ['Y'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            1, 0, 0, 0, 1,
            0, 1, 0, 1, 0,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
        }
    },

    ['Z'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            1, 1, 1, 1, 1,
            0, 0, 0, 0, 1,
            0, 0, 0, 1, 0,
            0, 0, 1, 0, 0,
            0, 1, 0, 0, 0,
            1, 0, 0, 0, 0,
            1, 1, 1, 1, 1,
        }
    },


    ['1'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            0, 0, 1, 0, 0,
            0, 1, 1, 0, 0,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
            1, 1, 1, 1, 1,
        }
    },

    ['2'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            0, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            0, 0, 0, 0, 1,
            0, 0, 1, 1, 0,
            0, 1, 0, 0, 0,
            1, 0, 0, 0, 0,
            1, 1, 1, 1, 1,
        }
    },

    ['3'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            0, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            0, 0, 0, 0, 1,
            0, 0, 1, 1, 0,
            0, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 0,
        }
    },

    ['4'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            0, 0, 0, 1, 1,
            0, 0, 1, 0, 1,
            0, 1, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 1, 1, 1, 1,
            0, 0, 0, 0, 1,
            0, 0, 0, 0, 1,
        }
    },

    ['5'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            1, 1, 1, 1, 1,
            1, 0, 0, 0, 0,
            1, 1, 1, 1, 0,
            0, 0, 0, 0, 1,
            0, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 0,
        }
    },

    ['6'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            0, 0, 1, 1, 0,
            0, 1, 0, 0, 0,
            1, 0, 0, 0, 0,
            1, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 0,
        }
    },

    ['7'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            1, 1, 1, 1, 1,
            1, 0, 0, 0, 1,
            0, 0, 0, 0, 1,
            0, 0, 0, 1, 0,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
        }
    },

    ['8'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            0, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 0,
        }
    },

    ['9'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            0, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 1,
            0, 0, 0, 0, 1,
            0, 0, 0, 1, 0,
            0, 1, 1, 0, 0,
        }
    },

    ['0'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            0, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 1, 1,
            1, 0, 1, 0, 1,
            1, 1, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 0,
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
        1, 2, 0,
        (unsigned char [])
        {
            1,
            1,
        }
    },

    [','] =
    {
        1, 3, 1,
        (unsigned char [])
        {
            1,
            1,
            1,
        }
    },

    ['\''] =
    {
        1, 7, 0,
        (unsigned char [])
        {
            1,
            1,
            0,
            0,
            0,
            0,
            0,
        }
    },

    [':'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            0, 0, 0, 0, 0,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
            0, 0, 0, 0, 0,
        }
    },

    ['_'] =
    {
        5, 1, 0,
        (unsigned char [])
        {
            1, 1, 1, 1, 1,
        }
    },

    ['+'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
            1, 1, 1, 1, 1,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
        }
    },

    ['-'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            1, 1, 1, 1, 1,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
        }
    },

    ['*'] =
    {
        5, 6, 0,
        (unsigned char [])
        {
            0, 1, 0, 1, 0,
            0, 0, 1, 0, 0,
            0, 1, 0, 1, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
        }
    },

    ['/'] =
    {
        5, 7, 0,
        (unsigned char [])
        {
            0, 0, 0, 0, 1,
            0, 0, 0, 1, 0,
            0, 0, 0, 1, 0,
            0, 0, 1, 0, 0,
            0, 1, 0, 0, 0,
            0, 1, 0, 0, 0,
            1, 0, 0, 0, 0,
        }
    },

    ['('] =
    {
        3, 7, 0,
        (unsigned char [])
        {
            0, 0, 1,
            0, 1, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            0, 1, 0,
            0, 0, 1,
        }
    },

    [')'] =
    {
        3, 7, 0,
        (unsigned char [])
        {
            1, 0, 0,
            0, 1, 0,
            0, 0, 1,
            0, 0, 1,
            0, 0, 1,
            0, 1, 0,
            1, 0, 0,
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
            max_height = 0;
        }
    }
}

int main(void)
{
    struct Image img = {0};

    img.width = 1920;
    img.height = 1080;
    img.data = malloc(img.width * img.height * 3);

    unsigned char white[] = {255, 255, 255};


    if (1)
    {
        for (int x = 0; x < img.width; ++x)
        {
            for (int y = 0; y < img.height; ++y)
            {
                unsigned char *p = get_pixel(&img, x, y);
                p[0] = 0;
                p[1] = 0;
                p[2] = 240;
            }
        }
    }


    if (0)
    {
        char lorem[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
        char abc[] = "Aa Bb Cc Dd Ee Ff Gg Hh Ii Jj Kk Ll Mm Nn Oo Pp Qq Rr Ss Tt Uu Vv Ww Xx Yy Zz";
        draw_text(&img, lorem, 100, 100, 3, img.width-100, white);
    }


    if (1)
    {
        int scale = 4;
        int left = 100;
        int right = img.width - 100;
        int ycursor = 100;
        int block_ystep = 70;
        int line_ystep = 40;

        draw_text(&img,
            "A problem has been detected and GNU/Linux has been shut down to prevent",
            left, ycursor, scale, right, white);
        ycursor += line_ystep;

        draw_text(&img,
            "damage to your computer.",
            left, ycursor, scale, right, white);
        ycursor += block_ystep;


        draw_text(&img,
            "NFSW_FOLDER_OVERFLOW",
            left, ycursor, scale, right, white);
        ycursor += block_ystep;


        draw_text(&img,
            "If this if the first time you've seen this Stop error screen, restart your",
            left, ycursor, scale, right, white);
        ycursor += line_ystep;

        draw_text(&img,
            "computer. If this screen appears again, follow these steps:",
            left, ycursor, scale, right, white);
        ycursor += block_ystep;


        draw_text(&img,
            "Check to make sure any new hardware or software is properly installed. If this",
            left, ycursor, scale, right, white);
        ycursor += line_ystep;

        draw_text(&img,
            "is a new installation, ask your hardware or software manufacturer for any",
            left, ycursor, scale, right, white);
        ycursor += line_ystep;

        draw_text(&img,
            "GNU/Linux updates you might need.",
            left, ycursor, scale, right, white);
        ycursor += block_ystep;


        draw_text(&img,
            "If problems continue, disable or remove any new installed hardware of software.",
            left, ycursor, scale, right, white);
        ycursor += line_ystep;

        draw_text(&img,
            "Disable BIOS memory options such as caching of shadowing. If you need to use",
            left, ycursor, scale, right, white);
        ycursor += line_ystep;

        draw_text(&img,
            "Safe Mode to remove or disable components, restart your computer, press F8",
            left, ycursor, scale, right, white);
        ycursor += line_ystep;

        draw_text(&img,
            "to select Advansed Startup Options, and then select Safe Mode.",
            left, ycursor, scale, right, white);
        ycursor += block_ystep;


        draw_text(&img,
            "Technical information:",
            left, ycursor, scale, right, white);
        ycursor += line_ystep;

        draw_text(&img,
            "*** STOP: 0x00000069 (0x00000042, 0x00000228, 0x00001377, 0x00000067)",
            left, ycursor, scale, right, white);
        ycursor += line_ystep;
    }


    stbi_write_png("image.png", img.width, img.height, 3, (void *)img.data, img.width * 3);
    return 0;
}
