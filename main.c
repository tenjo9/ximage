#include <math.h>
#include <stdlib.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

struct Image
{
    int width, height;
    unsigned char *data;
};

int main(void)
{
    struct Image img = {0};

    img.width = 800;
    img.height = 600;
    img.data = malloc(img.width * img.height * 3);

    for (int x = 0; x < img.width; ++x)
    {
        for (int y = 0; y < img.height; ++y)
        {
            unsigned char *p = &img.data[(y * img.width + x) * 3];
            p[0] = ((double)(x + y) / (double)(img.width + img.height)) * 255;
            p[1] = ((double)x / img.width) * 255;
            p[2] = 100;
        }
    }

    int r = 100;
    int x0 = img.width/2, y0 = img.height/2;
    for (int x = x0-r; x < x0+r; ++x)
    {
        for (int y = y0-r; y < y0+r; ++y)
        {
            double mx = (double)x - (double)x0 + 0.5;
            double my = (double)y - (double)y0 + 0.5;
            int cond = ( mx*mx + my*my - r*r ) <= 0.00001;
            if (cond)
            {
                unsigned char *p = &img.data[(y * img.width + x) * 3];
                p[0] = round( (((double)x - x0-r) / (r*2.0) ) * 255.0);
                p[1] = 50;
                p[2] = 200;
            }
        }
    }

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

                    if ( (dx < 0 || dx >= img.width) || (dy < 0 || dy >= img.height) ) { continue; }

                    ++count;
                    unsigned char *p = &img.data[(dy * img.width + dx) * 3];
                    r += p[0];
                    g += p[1];
                    b += p[2];
                }
            }

            r /= count;
            g /= count;
            b /= count;

            unsigned char *p = &img.data[(y * img.width + x) * 3];
            p[0] = r;
            p[1] = g;
            p[2] = b;
        }
    }

    stbi_write_png("image.png", img.width, img.height, 3, (void *)img.data, img.width * 3);
    return 0;
}
