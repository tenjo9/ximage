#include <stdio.h>
#include <string.h>
#include <time.h>
#include <float.h>

#include "stb_image_write.h"

#define ALGEBRA_IMPLEMENTATION
#include "algebra.h"

#define XIMAGE_IMPLEMENTATION
#include "../ximage.h"

#define XIMAGE_3D_IMPLEMENTATION
#include "ximage_3d.h"

extern XI_Model utah_teapot;

void point3d(XImage3D *ctx, double x, double y, double z, int r, uint32_t color)
{
    v2i p = xi_c2s(ctx, xi_w2c(ctx, v3dd(x, y, z)));
    xi_draw_circle(ctx->img, p.x, p.y, r, color);
}

int main(void)
{
    XImage img = xi_new(4000, 2000);
    xi_fill_bg(&img, 0xff424242);

    XI_Camera c = {
        .pos = v3dd( 5.0, 5.0, 5.0 ),
        .tar = v3dd( 0.0, 0.0, 0.0 ),
        .up  = v3dd( 0.0, 0.0, 1.0 ),
        .fov = 90.0*3.1415/180.0,
        .aspect = (double)img.width / (double)img.height,
        .near = 0.1,
        .far = 100.0,
    };

    double ambient = 0.2;
    v3d sun = { 10.0, 10.0, 10.0 };


    XImage3D ctx = xi_3d(&img, &c);


    for (int i = -10; i < 10; ++i)
    {
        point3d(&ctx, (double)i, 0.0, 0.0, 10, 0xff0000ff);
        point3d(&ctx, 0.0, (double)i, 0.0, 10, 0xff00ff00);
        point3d(&ctx, 0.0, 0.0, (double)i, 10, 0xffff0000);
    }
    point3d(&ctx, 0.0, 0.0, 0.0, 10, 0xffffffff);


    utah_teapot.pos = v3dd(0.0, 5.0, -1.0);
    xi_draw_model_(&ctx, &utah_teapot, ambient, sun, 0xff0000ff);

    utah_teapot.pos = v3dd(0.0, 0.0, -1.0);
    xi_draw_model_(&ctx, &utah_teapot, ambient, sun, 0xff00ff00);

    utah_teapot.pos = v3dd(0.0, -5.0, -1.0);
    xi_draw_model_(&ctx, &utah_teapot, ambient, sun, 0xffff0000);


    stbi_write_png("ximage.png", img.width, img.height, 4, (void *)img.data, 4*img.width);
    free(img.data);
    return 0;
}
