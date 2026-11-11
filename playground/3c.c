#include <stdio.h>

#include "../3rdparty/stb_image_write.h"

#define XI_DEBUG
#define XI_ASSERT(cond, msg)\
    if (!(cond)) { fprintf(stderr, "%s:%d: " msg "\n", __FILE__, __LINE__); exit(-1); }
#define XI_ASSERT_VA(cond, msg, ...)\
    if (!(cond)) { fprintf(stderr, "%s:%d: " msg "\n", __FILE__, __LINE__, __VA_ARGS__); exit(-1); }
#define XIMAGE_IMPLEMENTATION
#include "../ximage.h"


void xi_draw_triangle_3c(ximage_t *xi, int x1, int y1, int x2, int y2, int x3, int y3, color_t c1, color_t c2, color_t c3)
{
    XI_ASSERT( (xi) != XI_NULL, "xi is NULL");

    int xstart = xi_maxi(         0,  xi_mini(x1, xi_mini(x2, x3)));
    int xend   = xi_mini( xi->width,  xi_maxi(x1, xi_maxi(x2, x3)));
    int ystart = xi_maxi(         0,  xi_mini(y1, xi_mini(y2, y3)));
    int yend   = xi_mini(xi->height,  xi_maxi(y1, xi_maxi(y2, y3)));

    float X1 = (float)x1;
    float Y1 = (float)y1;
    float X2 = (float)x2;
    float Y2 = (float)y2;
    float X3 = (float)x3;
    float Y3 = (float)y3;

    float D = (X3-X1)*(Y2-Y1) - (X2-X1)*(Y3-Y1);
    if (xi_absf(D) < 1.0e-12f)
        return;
    float recD = 1.0f/D;

    for (int y = ystart; y < yend; ++y)
    {
        for (int x = xstart; x < xend; ++x)
        {
            float X = (float)x + 0.5f;
            float Y = (float)y + 0.5f;
            float l1 = ((X3-X)*(Y2-Y) - (X2-X)*(Y3-Y)) * recD;
            float l2 = ((X2-X)*(Y1-Y) - (X1-X)*(Y2-Y)) * recD;
            float l3 = 1.0f - l1 - l2;
            if (l1 >= 0.0f && l2 >= 0.0f && l3 >= 0.0f)
            {
                color_t color = {
                    (unsigned char) (l1*(float)c1.r + l2*(float)c2.r + l3*(float)c3.r),
                    (unsigned char) (l1*(float)c1.g + l2*(float)c2.g + l3*(float)c3.g),
                    (unsigned char) (l1*(float)c1.b + l2*(float)c2.b + l3*(float)c3.b),
                    (unsigned char) (l1*(float)c1.a + l2*(float)c2.a + l3*(float)c3.a),
                };
                *XI_PIXEL(xi, x, y) = blend_colors(color, *XI_PIXEL(xi, x, y));
            }
        }
    }
}


#define WIDTH  800
#define HEIGHT 600
color_t canvas[WIDTH*HEIGHT];


int main(void)
{
    ximage_t img = { .width=WIDTH, .height=HEIGHT, .data=canvas };

    xi_fill_bg(&img, XI_RGB(32, 32, 32));
    xi_draw_circle(&img, WIDTH/2, HEIGHT/2, 150, XI_RGB(42, 42, 42));
    xi_draw_triangle_3c(&img,
        300, 450,
        450, 100,
        650, 550,
        XI_RGB(255, 0, 0),
        XI_RGB(0, 255, 0),
        XI_RGB(0, 0, 255)
    );

    stbi_write_png("output.png", img.width, img.height, 4, (void *)img.data, 4*img.width);
    return 0;
}
