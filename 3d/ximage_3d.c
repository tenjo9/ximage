/*
    ffmpeg -i part_%03d.png -r 60 -crf 18 demo.mp4
    mpv --loop=inf demo.mp4
*/

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

static inline double d2r(double deg)
{
    return deg*3.1415/180.0;
}

int main(void)
{
    XImage img = xi_new(4096, 2160);

    double r = 5.0;
    double fi = 1.0;

    XI_Camera c = {
        .pos = v3dd( r*cos(d2r(fi)), r*sin(d2r(fi)), 0.0 ),
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



    int tex_side = 16;
    XImage tex = xi_new(tex_side, tex_side);
    xi_fill_bg(&tex, 0xff424242);
    int cell = 1;
    for (int y = 0; y < tex.height; y += cell)
    {
        for (int x = 0; x < tex.width; x += cell)
        {
            uint32_t color = ((x / cell) + (y / cell)) % 2 ? 0xffff00ff : 0xff000000;
            xi_draw_rect(&tex, x, y, cell, cell, color);
        }
    }
    stbi_write_png("ximage.png", tex.width, tex.height, 4, (void *)tex.data, 4*tex.width);


    for (int i = 0; i < 360; ++i)
    {
        xi_update_3d(&ctx);

        c.pos.x = r*cos(d2r(fi));
        c.pos.y = r*sin(d2r(fi));
        fi += 1.0;


        xi_fill_bg(&img, 0xff424242);


        /*
        for (int i = -10; i < 10; ++i)
        {
            point3d(&ctx, (double)i, 0.0, 0.0, 5, 0xff0000ff);
            point3d(&ctx, 0.0, (double)i, 0.0, 5, 0xff00ff00);
            point3d(&ctx, 0.0, 0.0, (double)i, 5, 0xffff0000);
        }
        point3d(&ctx, 0.0, 0.0, 0.0, 5, 0xffffffff);
        */


        //utah_teapot.pos = v3dd(0.0, 5.0, -1.0);
        //xi_draw_model(&ctx, &utah_teapot, ambient, sun, 0xffff0000);

        utah_teapot.pos = v3dd(0.0, 0.0, -1.0);
        xi_draw_model_tex(&ctx, &utah_teapot, &tex, ambient, sun);

        //utah_teapot.pos = v3dd(0.0, -5.0, -1.0);
        //xi_draw_model(&ctx, &utah_teapot, ambient, sun, 0xff0000ff);




        char filename[32];
        sprintf(filename, "part_%03d.png", i);
        stbi_write_png(filename, img.width, img.height, 4, (void *)img.data, 4*img.width);

        printf("Writed %s\n", filename);
    }

    free(img.data);
    return 0;
}
