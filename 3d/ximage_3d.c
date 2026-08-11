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

extern Model utah_teapot;

static inline double d2r(double deg)
{
    return deg*3.1415/180.0;
}

int main(void)
{
    XImage img = xi_new(1280, 720);

    double r = 10.0;
    double fi = 1.0;

    Camera c = {
        .pos = v3dd( r*cos(d2r(fi)), r*sin(d2r(fi)), 0.0 ),
        .tar = v3dd( 0.0, 0.0, 0.0 ),
        .up  = v3dd( 0.0, 0.0, 1.0 ),
        .fov = 90.0*3.1415/180.0,
        .aspect = (double)img.width / (double)img.height,
        .near = 0.1,
        .far = 100.0,
    };


    XImage3D ctx = xi_3d(&img, &c);
    ctx.ambient = 0.2;
    ctx.sun = (v3d){ 10.0, 10.0, 10.0 };


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
        xi_draw_model_tex(&ctx, &utah_teapot, v3dd(0.0, 0.0, -1.0), &tex);
        xi_draw_model(&ctx, &utah_teapot, v3dd(0.0, 5.0, -1.0), 0xff0062fc);

        char filename[32];
        sprintf(filename, "part_%03d.png", i);
        stbi_write_png(filename, img.width, img.height, 4, (void *)img.data, 4*img.width);

        printf("Writed %s\n", filename);
    }

    free(img.data);
    return 0;
}
