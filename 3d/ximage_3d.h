#ifndef XIMAGE_3D_H
#define XIMAGE_3D_H

#include "../ximage.h"
#include "algebra.h"
#include <stdlib.h>

typedef struct
{
    size_t v1;  size_t uv1;  size_t n1;
    size_t v2;  size_t uv2;  size_t n2;
    size_t v3;  size_t uv3;  size_t n3;
} Face;

typedef struct
{
    size_t vcount;
    v3d *vertices;

    size_t ncount;
    v3d *normales;

    size_t tcount;
    v2d *uv;

    size_t fcount;
    Face *faces;
} Model;

typedef struct
{
    v3d pos;
    v3d tar;
    v3d up;
    double fov;
    double aspect;
    double near;
    double far;
} Camera;

typedef struct
{
    XImage *img;
    double *zbuf;
    Camera *cam;

    v3d f;
    v3d r;
    v3d u;
    double fv;

    double ambient;
    v3d sun;
} XImage3D;

typedef struct
{
    v3d p1; v3d p2; v3d p3;
    v2i s1; v2i s2; v2i s3;
    v3d c1; v3d c2; v3d c3;
    v3d n1; v3d n2; v3d n3;
    v2d uv1; v2d uv2; v2d uv3;
    int left; int right;
    int top; int bottom;
    double D;
} Triangle;


XImage3D xi_3d(XImage *img, Camera *cam);
void xi_update_3d(XImage3D *ctx);

v3d xi_w2c(XImage3D *ctx, v3d v);
v2i xi_c2s(XImage3D *ctx, v3d v);



#define XI_MAX(a, b) (a >= b? a : b)
#define XI_MIN(a, b) (a <= b? a : b)

#define XI_SHADOW(color, intensity) XI_RGB(XI_GET_R(color)*(intensity), XI_GET_G(color)*intensity, XI_GET_B(color)*intensity)
#define XI_RANDOM ( ((uint32_t)0xFFu << 24) | ((uint32_t)rand() & 0x00FFFFFFu) )


#ifdef XIMAGE_3D_IMPLEMENTATION


//**********************************************************************
// Initializing

XImage3D xi_3d(XImage *img, Camera *cam)
{
    v3d f = v3d_normalize(v3d_sub(cam->tar, cam->pos));
    v3d r = v3d_normalize(v3d_cross(f, cam->up));
    v3d u = v3d_cross(r, f);
    double fv = 1.0 / tan(cam->fov * 0.5);

    double *zbuf = malloc((size_t)(img->width*img->height) * sizeof(double));
    if (!zbuf)
    {
        fprintf(stderr, "Z Buffer memory allocation error\n");
        exit(69);
    }
    for (int i = 0; i < img->width*img->height; ++i) zbuf[i] = DBL_MAX;

    return (XImage3D){
        .img = img,
        .cam = cam,
        .zbuf = zbuf,
        .f = f, .r = r, .u = u,
        .fv = fv,
    };
}

void xi_update_3d(XImage3D *ctx)
{
    ctx->f = v3d_normalize(v3d_sub(ctx->cam->tar, ctx->cam->pos));
    ctx->r = v3d_normalize(v3d_cross(ctx->f, ctx->cam->up));
    ctx->u = v3d_cross(ctx->r, ctx->f);
    ctx->fv = 1.0 / tan(ctx->cam->fov * 0.5);
    for (int i = 0; i < ctx->img->width*ctx->img->height; ++i) ctx->zbuf[i] = DBL_MAX;
}


//**********************************************************************
// Transforming

v3d xi_w2c(XImage3D *ctx, v3d v)
{
    v = v3d_sub(v, ctx->cam->pos);
    return v3dd(
        v3d_dot(v, ctx->r),
        v3d_dot(v, ctx->u),
        v3d_dot(v, ctx->f)
    );
}

v2i xi_c2s(XImage3D *ctx, v3d v)
{
    v2d ndc = {
        (v.x * ctx->fv) / (v.z * ctx->cam->aspect),
        (v.y * ctx->fv) / v.z
    };

    return (v2i) {
        (int)( ndc.x*ctx->img->width /2.0 + ctx->img->width /2.0),
        (int)(-ndc.y*ctx->img->height/2.0 + ctx->img->height/2.0)
    };
}


//**********************************************************************
// Utils

double clampd(double value, double min, double max)
{
    return (value < min)? min : (value > max)? max : value;
}

double baricent(v2i a, v2i b, v2i c)
{
    v2d A = v2d_fold( v2di(a), v2dd(0.5, 0.5) );
    v2d B = v2d_fold( v2di(b), v2dd(0.5, 0.5) );
    v2d C = v2d_fold( v2di(c), v2dd(0.5, 0.5) );

    v2d AB = v2d_sub(B, A);
    v2d AC = v2d_sub(C, A);

    v3d area = v3d_cross( v3d2d(AB), v3d2d(AC) );

    return area.z;
}

Triangle get_triangle(Model *m, size_t i)
{
    Triangle t = {0};

    t.p1 = m->vertices[m->faces[i].v1];
    t.p2 = m->vertices[m->faces[i].v2];
    t.p3 = m->vertices[m->faces[i].v3];

    t.n1 = m->normales[m->faces[i].n1];
    t.n2 = m->normales[m->faces[i].n2];
    t.n3 = m->normales[m->faces[i].n3];

    t.uv1 = m->uv[m->faces[i].uv1];
    t.uv2 = m->uv[m->faces[i].uv2];
    t.uv3 = m->uv[m->faces[i].uv3];

    return t;
}

bool prepare_triangle(XImage3D *ctx, Triangle *t)
{
    t->c1 = xi_w2c(ctx, t->p1);
    t->c2 = xi_w2c(ctx, t->p2);
    t->c3 = xi_w2c(ctx, t->p3);

    t->s1 = xi_c2s(ctx, t->c1);
    t->s2 = xi_c2s(ctx, t->c2);
    t->s3 = xi_c2s(ctx, t->c3);

    int xs[] = { t->s1.x, t->s2.x, t->s3.x };
    int ys[] = { t->s1.y, t->s2.y, t->s3.y };
    if (!xi_find_bbox(ctx->img, xs, ys, 3, &t->left, &t->right, &t->top, &t->bottom))
        return false;

    if (
        (t->c1.z <= ctx->cam->near || t->c2.z <= ctx->cam->near || t->c3.z <= ctx->cam->near) ||
        (t->c1.z >= ctx->cam->far  || t->c2.z >= ctx->cam->far  || t->c3.z >= ctx->cam->far )
    )
        return false;

    t->D = baricent(t->s1, t->s2, t->s3);
    if (t->D >= 0.0)
        return false;

    return true;
}

double phong_light(XImage3D *ctx, v3d p, v3d n)
{
    double i = ctx->ambient + clampd(v3d_dot(n, v3d_normalize(v3d_sub(ctx->sun, p))), 0.0, 1.0);
    if (i > 1.0) i = 1.0;
    return i;
}


//**********************************************************************
// Triangles

void xi_draw_triangle_z_c3(XImage3D *ctx, Triangle *t, uint32_t col1, uint32_t col2, uint32_t col3)
{
    for (int x = t->left; x <= t->right; ++x)
    {
        for (int y = t->top; y <= t->bottom; ++y)
        {
            double l1 = baricent(v2ii(x, y), t->s2, t->s3) / t->D;
            double l2 = baricent(t->s1, v2ii(x, y), t->s3) / t->D;
            double l3 = baricent(t->s1, t->s2, v2ii(x, y)) / t->D;

            if (l1 >= -1e-14 && l2 >= -1e-14 && l3 >= -1e-14)
            {
                double w1 = 1.0 / t->c1.z*l1;
                double w2 = 1.0 / t->c2.z*l2;
                double w3 = 1.0 / t->c3.z*l3;
                double w = (w1 + w2 + w3);
                double z = 1.0 / w;
                if (z < ctx->zbuf[ctx->img->width*y + x])
                {
                    ctx->zbuf[ctx->img->width*y + x] = z;

                    double r = XI_GET_R(col1)*l1 + XI_GET_R(col2)*l2 + XI_GET_R(col3)*l3; if (r > 255.0) r = 255.0;
                    double g = XI_GET_G(col1)*l1 + XI_GET_G(col2)*l2 + XI_GET_G(col3)*l3; if (g > 255.0) g = 255.0;
                    double b = XI_GET_B(col1)*l1 + XI_GET_B(col2)*l2 + XI_GET_B(col3)*l3; if (b > 255.0) b = 255.0;

                    XI_PUT_PIXEL(ctx->img, x, y, XI_RGB(r, g, b));
                }
            }
        }
    }
}

void xi_draw_triangle_z_uv(XImage3D *ctx, Triangle *t, XImage *tex, double i1, double i2, double i3)
{
    for (int x = t->left; x <= t->right; ++x)
    {
        for (int y = t->top; y <= t->bottom; ++y)
        {
            double l1 = baricent(v2ii(x, y), t->s2, t->s3) / t->D;
            double l2 = baricent(t->s1, v2ii(x, y), t->s3) / t->D;
            double l3 = baricent(t->s1, t->s2, v2ii(x, y)) / t->D;

            if (l1 >= -1e-14 && l2 >= -1e-14 && l3 >= -1e-14)
            {
                double w1 = 1.0 / t->c1.z*l1;
                double w2 = 1.0 / t->c2.z*l2;
                double w3 = 1.0 / t->c3.z*l3;
                double w = (w1 + w2 + w3);
                double z = 1.0 / w;
                if (z < ctx->zbuf[ctx->img->width*y + x])
                {
                    ctx->zbuf[ctx->img->width*y + x] = z;

                    double uv_x = (t->uv1.x*w1 + t->uv2.x*w2 + t->uv3.x*w3) / w;
                    double uv_y = (t->uv1.y*w1 + t->uv2.y*w2 + t->uv3.y*w3) / w;

                    int tex_x = (int)((double)tex->width * uv_x);
                    int tex_y = (int)((double)tex->height * uv_y);
                    uint32_t out = tex->data[tex_y*tex->width + tex_x];

                    double i = (i1*w1 + i2*w2 + i3*w3) / w;
                    out = XI_SHADOW(out, i);

                    XI_PUT_PIXEL(ctx->img, x, y, out);
                }
            }
        }
    }
}


//**********************************************************************
// Models

void xi_draw_model(XImage3D *ctx, Model *m, v3d pos, uint32_t color)
{

    for (size_t i = 0; i < m->fcount; ++i)
    {
        Triangle t = get_triangle(m, i);

        t.p1 = v3d_fold(pos, t.p1);
        t.p2 = v3d_fold(pos, t.p2);
        t.p3 = v3d_fold(pos, t.p3);

        if (!prepare_triangle(ctx, &t))
            continue;

        double i1 = phong_light(ctx, t.p1, t.n1);
        double i2 = phong_light(ctx, t.p2, t.n2);
        double i3 = phong_light(ctx, t.p3, t.n3);

        xi_draw_triangle_z_c3(ctx, &t, XI_SHADOW(color, i1), XI_SHADOW(color, i2), XI_SHADOW(color, i3));
    }
}

void xi_draw_model_tex(XImage3D *ctx, Model *m, v3d pos, XImage *tex)
{
    for (size_t i = 0; i < m->fcount; ++i)
    {
        Triangle t = get_triangle(m, i);

        t.p1 = v3d_fold(pos, t.p1);
        t.p2 = v3d_fold(pos, t.p2);
        t.p3 = v3d_fold(pos, t.p3);

        if (!prepare_triangle(ctx, &t))
            continue;

        double i1 = phong_light(ctx, t.p1, t.n1);
        double i2 = phong_light(ctx, t.p2, t.n2);
        double i3 = phong_light(ctx, t.p3, t.n3);

        xi_draw_triangle_z_uv(ctx, &t, tex, i1, i2, i3);
    }
}

#endif

#endif
