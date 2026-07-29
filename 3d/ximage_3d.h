#ifndef XIMAGE_3D_H
#define XIMAGE_3D_H

#include "../ximage.h"
#include "algebra.h"
#include <stdlib.h>

typedef struct
{
    size_t v1;  size_t n1;  size_t uv1;
    size_t v2;  size_t n2;  size_t uv2;
    size_t v3;  size_t n3;  size_t uv3;
} Face;

typedef struct
{
    v3d pos;

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
} XImage3D;

typedef struct
{
    v3d p1; v3d p2; v3d p3;
    v2i s1; v2i s2; v2i s3;
    v3d c1; v3d c2; v3d c3;
    v3d n1; v3d n2; v3d n3;
    v2d uv1; v2d uv2; v2d uv3;
    int left; int right; int top; int bottom;
    double D;
} Triangle;

typedef uint32_t (*xi_color_src)(Triangle *t, double l1, double l2, double l3);


XImage3D xi_3d(XImage *img, Camera *cam);

v3d xi_w2c(XImage3D *ctx, v3d v);
v2i xi_c2s(XImage3D *ctx, v3d v);

void xi_draw_triangle_z(XImage3D *ctx, Triangle *t, uint32_t color);
void xi_draw_triangle_z_source(XImage3D *ctx, Triangle *t, xi_color_src color);

void xi_draw_model(XImage3D *ctx, Model *m, uint32_t color);
void xi_draw_model_source(XImage3D *ctx, Model *m, xi_color_src color);
void xi_draw_model_debug(XImage3D *ctx, Model *m);


void xi_draw_triangle_z_lambert(XImage3D *ctx, Triangle *t, double ambient, v3d sun, uint32_t color);
void xi_draw_triangle_z_lambert_source(XImage3D *ctx, Triangle *t, double ambient, v3d sun, xi_color_src color);

void xi_draw_model_lambert(XImage3D *ctx, Model *m, double ambient, v3d sun, uint32_t color);
void xi_draw_model_lambert_source(XImage3D *ctx, Model *m, double ambient, v3d sun, xi_color_src color);
void xi_draw_model_lambert_debug(XImage3D *ctx, Model *m, double ambient, v3d sun);


#define XI_MAX(a, b) (a >= b? a : b)


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


//**********************************************************************
// Drawing triangles flat

void xi_draw_triangle_z(XImage3D *ctx, Triangle *t, uint32_t color)
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
                double z = 1.0 / (1.0/t->c1.z*l1 + 1.0/t->c2.z*l2 + 1.0/t->c3.z*l3);
                if (z < ctx->zbuf[ctx->img->width*y + x])
                {
                    ctx->zbuf[ctx->img->width*y + x] = z;
                    XI_PUT_PIXEL(ctx->img, x, y, color);
                }
            }
        }
    }
}

void xi_draw_triangle_z_source(XImage3D *ctx, Triangle *t, xi_color_src src)
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
                double z = 1.0 / (1.0/t->c1.z*l1 + 1.0/t->c2.z*l2 + 1.0/t->c3.z*l3);
                if (z < ctx->zbuf[ctx->img->width*y + x])
                {
                    ctx->zbuf[ctx->img->width*y + x] = z;
                    XI_PUT_PIXEL(ctx->img, x, y, src(t, l1, l2, l3));
                }
            }
        }
    }
}


//**********************************************************************
// Drawing models flat

void xi_draw_model(XImage3D *ctx, Model *m, uint32_t color)
{
    Triangle t = {0};

    for (size_t i = 0; i < m->fcount; ++i)
    {
        t.p1 = v3d_fold(m->pos, m->vertices[m->faces[i].v1]);
        t.p2 = v3d_fold(m->pos, m->vertices[m->faces[i].v2]);
        t.p3 = v3d_fold(m->pos, m->vertices[m->faces[i].v3]);

        if (!prepare_triangle(ctx, &t))
            continue;

        xi_draw_triangle_z(ctx, &t, color);
    }
}

void xi_draw_model_source(XImage3D *ctx, Model *m, xi_color_src src)
{
    Triangle t = {0};

    for (size_t i = 0; i < m->fcount; ++i)
    {
        t.p1 = v3d_fold(m->pos, m->vertices[m->faces[i].v1]);
        t.p2 = v3d_fold(m->pos, m->vertices[m->faces[i].v2]);
        t.p3 = v3d_fold(m->pos, m->vertices[m->faces[i].v3]);

        if (!prepare_triangle(ctx, &t))
            continue;

        xi_draw_triangle_z_source(ctx, &t, src);
    }
}

void xi_draw_model_debug(XImage3D *ctx, Model *m)
{
    Triangle t = {0};

    for (size_t i = 0; i < m->fcount; ++i)
    {
        t.p1 = v3d_fold(m->pos, m->vertices[m->faces[i].v1]);
        t.p2 = v3d_fold(m->pos, m->vertices[m->faces[i].v2]);
        t.p3 = v3d_fold(m->pos, m->vertices[m->faces[i].v3]);

        if (!prepare_triangle(ctx, &t))
            continue;

        xi_draw_triangle_z(ctx, &t, XI_RGB(rand()%256, rand()%256, rand()%256));
    }
}

//**********************************************************************
// Drawing triangles lambert


void xi_draw_triangle_z_lambert(XImage3D *ctx, Triangle *t, double ambient, v3d sun, uint32_t color)
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
                double q = (1.0/t->c1.z*l1 + 1.0/t->c2.z*l2 + 1.0/t->c3.z*l3);
                double z = 1.0/q;

                if (z < ctx->zbuf[ctx->img->width*y + x])
                {
                    ctx->zbuf[ctx->img->width*y + x] = z;


                    v3d p = v3d_div(
                        v3d_fold( t->p1,
                            v3d_fold( t->p2, t->p3 )
                        ),
                        3
                    );

                    v3d n = v3d_normalize(v3d_cross( v3d_sub(t->p2, t->p1), v3d_sub(t->p3, t->p1) ));

                    v3d to_sun = v3d_normalize(v3d_sub(sun, p));
                    double diffuse = XI_MAX(0.0, v3d_dot(n, to_sun));

                    double intencity = ambient + diffuse;
                    if (intencity > 1.0) intencity = 1.0;


                    XI_PUT_PIXEL(ctx->img, x, y, XI_RGB(
                            XI_GET_R(color)*intencity,
                            XI_GET_G(color)*intencity,
                            XI_GET_B(color)*intencity)
                    );
                }
            }
        }
    }
}

void xi_draw_triangle_z_lambert_source(XImage3D *ctx, Triangle *t, double ambient, v3d sun, xi_color_src src)
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
                double q = (1.0/t->c1.z*l1 + 1.0/t->c2.z*l2 + 1.0/t->c3.z*l3);
                double z = 1.0/q;

                if (z < ctx->zbuf[ctx->img->width*y + x])
                {
                    ctx->zbuf[ctx->img->width*y + x] = z;


                    v3d p = v3d_div(
                        v3d_fold( t->p1,
                            v3d_fold( t->p2, t->p3 )
                        ),
                        3
                    );

                    v3d n = v3d_normalize(v3d_cross( v3d_sub(t->p2, t->p1), v3d_sub(t->p3, t->p1) ));

                    v3d to_sun = v3d_normalize(v3d_sub(sun, p));
                    double diffuse = XI_MAX(0.0, v3d_dot(n, to_sun));

                    double intencity = ambient + diffuse;
                    if (intencity > 1.0) intencity = 1.0;


                    uint32_t color = src(t, l1, l2, l3);
                    XI_PUT_PIXEL(ctx->img, x, y, XI_RGB(
                            XI_GET_R(color)*intencity,
                            XI_GET_G(color)*intencity,
                            XI_GET_B(color)*intencity)
                    );
                }
            }
        }
    }
}
//**********************************************************************
// Drawing models lambert


void xi_draw_model_lambert(XImage3D *ctx, Model *m, double ambient, v3d sun, uint32_t color)
{
    Triangle t = {0};

    for (size_t i = 0; i < m->fcount; ++i)
    {
        t.p1 = v3d_fold(m->pos, m->vertices[m->faces[i].v1]);
        t.p2 = v3d_fold(m->pos, m->vertices[m->faces[i].v2]);
        t.p3 = v3d_fold(m->pos, m->vertices[m->faces[i].v3]);

        if (!prepare_triangle(ctx, &t))
            continue;

        t.n1 = m->normales[m->faces[i].n1];
        t.n2 = m->normales[m->faces[i].n2];
        t.n3 = m->normales[m->faces[i].n3];

        xi_draw_triangle_z_lambert(ctx, &t, ambient, sun, color);
    }
}

void xi_draw_model_lambert_source(XImage3D *ctx, Model *m, double ambient, v3d sun, xi_color_src src)
{
    Triangle t = {0};

    for (size_t i = 0; i < m->fcount; ++i)
    {
        t.p1 = v3d_fold(m->pos, m->vertices[m->faces[i].v1]);
        t.p2 = v3d_fold(m->pos, m->vertices[m->faces[i].v2]);
        t.p3 = v3d_fold(m->pos, m->vertices[m->faces[i].v3]);

        if (!prepare_triangle(ctx, &t))
            continue;

        t.n1 = m->normales[m->faces[i].n1];
        t.n2 = m->normales[m->faces[i].n2];
        t.n3 = m->normales[m->faces[i].n3];

        xi_draw_triangle_z_lambert_source(ctx, &t, ambient, sun, src);
    }
}

void xi_draw_model_lambert_debug(XImage3D *ctx, Model *m, double ambient, v3d sun)
{
    Triangle t = {0};

    for (size_t i = 0; i < m->fcount; ++i)
    {
        t.p1 = v3d_fold(m->pos, m->vertices[m->faces[i].v1]);
        t.p2 = v3d_fold(m->pos, m->vertices[m->faces[i].v2]);
        t.p3 = v3d_fold(m->pos, m->vertices[m->faces[i].v3]);

        if (!prepare_triangle(ctx, &t))
            continue;

        t.n1 = m->normales[m->faces[i].n1];
        t.n2 = m->normales[m->faces[i].n2];
        t.n3 = m->normales[m->faces[i].n3];

        xi_draw_triangle_z_lambert(ctx, &t, ambient, sun, XI_RGB(rand()%256, rand()%256, rand()%256));
    }
}

#endif

#endif
