#ifndef ALGEBRA_H
#define ALGEBRA_H

#include <math.h>

typedef struct { double x; double y; double z; } v3d;
typedef struct { double x; double y; } v2d;
typedef struct { int x; int y; } v2i;

//********************************************************
// v3d

v3d v3dd(double x, double y, double z);
v3d v3d2d(v2d v);

v3d v3d_fold(v3d a, v3d b);
v3d v3d_sub (v3d a, v3d b);

v3d v3d_mult(v3d v, double k);
v3d v3d_div (v3d v, double k);

v3d v3d_neg(v3d v);
v3d v3d_normalize(v3d v);
double v3d_dot(v3d a, v3d b);
v3d v3d_cross(v3d a, v3d b);


//********************************************************
// v2d

v2d v2dd(double x, double y);
v2d v2di(v2i v);

v3d v3d_fold(v3d a, v3d b);
v3d v3d_sub (v3d a, v3d b);

//********************************************************
// v2i

v2i v2ii(int x, int y);


#ifdef ALGEBRA_IMPLEMENTATION


//********************************************************
// v3d

v3d v3dd(double x, double y, double z)
{
    return (v3d){x, y, z};
}

v3d v3d2d(v2d v)
{
    return (v3d){v.x, v.y, 1.0};
}

v3d v3d_fold(v3d a, v3d b)
{
    return v3dd(a.x+b.x, a.y+b.y, a.z+b.z);
}

v3d v3d_sub (v3d a, v3d b)
{
    return v3dd(a.x-b.x, a.y-b.y, a.z-b.z);
}

v3d v3d_mult(v3d v, double k)
{
    return v3dd(v.x*k, v.y*k, v.z*k);
}

v3d v3d_div (v3d v, double k)
{
    return v3dd(v.x/k, v.y/k, v.z/k);
}

v3d v3d_neg(v3d v)
{
    return v3dd(-v.x, -v.y, -v.z);
}

v3d v3d_normalize(v3d v)
{
    return v3d_div(v, sqrt(v3d_dot(v, v)));
}

double v3d_dot(v3d a, v3d b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

v3d v3d_cross(v3d a, v3d b)
{
    return v3dd(
        a.y*b.z - b.y*a.z,
        a.z*b.x - b.z*a.x,
        a.x*b.y - b.x*a.y
    );
}


//********************************************************
// v2d

v2d v2dd(double x, double y)
{
    return (v2d){x, y};
}

v2d v2di(v2i v)
{
    return (v2d){(double)v.x, (double)v.y};
}

v2d v2d_fold(v2d a, v2d b)
{
    return (v2d){a.x+b.x, a.y+b.y};
}

v2d v2d_sub (v2d a, v2d b)
{
    return (v2d){a.x-b.x, a.y-b.y};
}


//********************************************************
// v2i

v2i v2ii(int x, int y)
{
    return (v2i){x, y};
}


#endif

#endif
