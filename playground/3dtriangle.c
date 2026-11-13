#include <math.h>
#include <stdio.h>
#include <float.h>

#include "../3rdparty/stb_image_write.h"

#define XI_DEBUG
#define XI_ASSERT(cond, msg)\
    if (!(cond)) { fprintf(stderr, "%s:%d: " msg "\n", __FILE__, __LINE__); exit(-1); }
#define XI_ASSERT_VA(cond, msg, ...)\
    if (!(cond)) { fprintf(stderr, "%s:%d: " msg "\n", __FILE__, __LINE__, __VA_ARGS__); exit(-1); }
#define XIMAGE_IMPLEMENTATION
#include "../ximage.h"


#define WIDTH  800
#define HEIGHT 600
color_t canvas[WIDTH*HEIGHT];
float buffer[WIDTH*HEIGHT];


float d2r(float deg)
{
    return deg*3.1415f/180.0f;
}

typedef struct
{
    float x;
    float y;
    float z;
} v3f;

typedef struct
{
    float x;
    float y;
} v2f;

typedef struct
{
    int x;
    int y;
} v2i;


float xscale;
float yscale;
float xoffset;
float yoffset;


v2f to_ndc(v3f world)
{
    return (v2f){ world.x / world.z, world.y / world.z };
}

v2i to_screen(v2f ndc)
{
    return (v2i){ (int)(xscale*ndc.x + xoffset), (int)(yscale*ndc.y + yoffset) };
}


void xi_draw_triangle_3c(ximage_t *xi, float *zbuffer, int x1, int y1, int x2, int y2, int x3, int y3, float z1, float z2, float z3, color_t color)
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
            float l2 = ((X1-X)*(Y3-Y) - (X3-X)*(Y1-Y)) * recD;
            float l3 = 1.0f - l1 - l2;
            float inv_z = l1/z1 + l2/z2 + l3/z3;
            float z = 1.0f / inv_z;
            float *zb = zbuffer + y*xi->width + x;
            if (l1 >= 0.0f && l2 >= 0.0f && l3 >= 0.0f && z < *zb)
            {
                *XI_PIXEL(xi, x, y) = blend_colors(color, *XI_PIXEL(xi, x, y));
                *zb = z;
            }
        }
    }
}


int main(void)
{
    ximage_t img = { .width=WIDTH, .height=HEIGHT, .data=canvas };

    xscale =  (float)WIDTH / 2.0f;
    yscale = -(float)HEIGHT/ 2.0f;
    xoffset = (float)WIDTH / 2.0f;
    yoffset = (float)HEIGHT/ 2.0f;

    for (int i = 0; i < WIDTH*HEIGHT; ++i)
    {
        buffer[i] = FLT_MAX;
    }


    xi_fill_bg(&img, XI_RGB(23, 22, 22));

    {
        v3f world1 = {  0.0f,  1.0f, 3.0f };
        v3f world2 = {  1.0f, -1.0f, 4.0f };
        v3f world3 = { -1.0f, -1.0f, 2.0f };

        v2i screen1 = to_screen(to_ndc(world1));
        v2i screen2 = to_screen(to_ndc(world2));
        v2i screen3 = to_screen(to_ndc(world3));

        xi_draw_triangle_3c(&img, buffer,
            screen1.x, screen1.y,
            screen2.x, screen2.y,
            screen3.x, screen3.y,
            world1.z, world2.z, world3.z,
            XI_RGB(128, 0, 128)
        );
    }

    {
        v3f world1 = {  0.0f,  1.0f, 3.0f };
        v3f world2 = {  1.0f, -1.0f, 2.0f };
        v3f world3 = { -1.0f, -1.0f, 4.0f };

        v2i screen1 = to_screen(to_ndc(world1));
        v2i screen2 = to_screen(to_ndc(world2));
        v2i screen3 = to_screen(to_ndc(world3));

        xi_draw_triangle_3c(&img, buffer,
            screen1.x, screen1.y,
            screen2.x, screen2.y,
            screen3.x, screen3.y,
            world1.z, world2.z, world3.z,
            XI_RGB(0, 0, 128)
        );
    }

    stbi_write_png("output.png", img.width, img.height, 4, (void *)img.data, 4*img.width);

    return 0;
}
