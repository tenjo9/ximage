#ifndef XIMAGE_H
#define XIMAGE_H


typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} color_t;

typedef struct
{
    int width, height;
    color_t *data;
} ximage_t;


void xi_fill_bg(ximage_t *xi, color_t color);
void xi_put_pixel(ximage_t *xi, int x, int y, color_t color);
void xi_draw_rect(ximage_t *xi, int x0, int y0, int width, int height, color_t color);
void xi_draw_circle(ximage_t *xi, int x0, int y0, int r, color_t color);
void xi_draw_triangle(ximage_t *xi, int x1, int y1, int x2, int y2, int x3, int y3, color_t color);
void xi_draw_texture(ximage_t *xi, ximage_t *texture, int x0, int y0, int width, int height);

//#define XI_RGBA(R, G, B, A) (color_t){ .r=(R), .g=(G), .b=(B), .a=(A) }
#define XI_RGB(R, G, B) (color_t){ .r=(R), .g=(G), .b=(B), .a=255 }


#ifdef XIMAGE_IMPLEMENTATION


#define XI_NULL ((void *)0)

#ifndef XI_ASSERT
    #define XI_ASSERT(cond, msg)
#endif // #ifndef XI_ASSERT

#ifndef XI_ASSERT_VA
    #define XI_ASSERT_VA(cond, msg, ...)
#endif // #ifndef XI_ASSERT_VA

#ifdef XI_DEBUG
    color_t *xi_pixel_debug(ximage_t *xi, int x, int y, char *file, int line)
    {
        XI_ASSERT_VA((x) >= 0 && (x) < (xi)->width, "x out of bounce: %d [%d, %d), in %s:%d", x, 0, xi->width, file, line);
        XI_ASSERT_VA((y) >= 0 && (y) < (xi)->height, "y out of bounce: %d [%d, %d), in %s:%d", y, 0, xi->height, file, line);
        return xi->data + y*xi->width + x;
    }
    #define XI_PIXEL(xi, x, y) xi_pixel_debug(xi, x, y, __FILE__, __LINE__)
#else
    #define XI_PIXEL(xi, x, y) ((xi)->data + (y)*(xi)->width + (x))
#endif


static inline color_t blend_colors(color_t fg, color_t bg)
{
    color_t color = {
        .r = (unsigned char)((fg.r*fg.a + bg.r*(255 - fg.a))/255),
        .b = (unsigned char)((fg.b*fg.a + bg.b*(255 - fg.a))/255),
        .g = (unsigned char)((fg.g*fg.a + bg.g*(255 - fg.a))/255),
        .a = bg.a
    };
    return color;
}



// ==================== MATH ==================== //

static inline int xi_maxi(int a, int b)
{
    return a > b? a : b;
}

static inline int xi_mini(int a, int b)
{
    return a < b? a : b;
}

static inline float xi_maxf(float a, float b)
{
    return a > b? a : b;
}

static inline float xi_minf(float a, float b)
{
    return a < b? a : b;
}

static inline int xi_absi(int a)
{
    return a >= 0? a : -a;
}

static inline float xi_absf(float a)
{
    return a >= 0.0f? a : -a;
}


// ==================== 2D PRIMITIVES ==================== //

void xi_fill_bg(ximage_t *xi, color_t color)
{
    for (int i = 0; i < xi->width*xi->height; ++i)
        xi->data[i] = color;
}

void xi_put_pixel(ximage_t *xi, int x, int y, color_t color)
{
    if (x >= 0 && x < xi->width && y >= 0 && y < xi->height)
    {
        *XI_PIXEL(xi, x, y) = color;
    }
}

void xi_draw_rect(ximage_t *xi, int x0, int y0, int width, int height, color_t color)
{
    int xstart = x0 >= 0? x0 : 0;
    int xend   = x0+width < xi->width? x0+width : xi->width;
    int ystart = y0 >= 0? y0 : 0;
    int yend   = y0+height < xi->height? y0+height : xi->height;

    for (int y = ystart; y < yend; ++y)
    {
        for (int x = xstart; x < xend; ++x)
        {
            *XI_PIXEL(xi, x, y) = color;
        }
    }
}

void xi_draw_circle(ximage_t *xi, int x0, int y0, int r, color_t color)
{
    if (r == 0)
    {
        xi_put_pixel(xi, x0, y0, color);
        return;
    }

    int xstart = x0-r >= 0? x0-r : 0;
    int xend   = x0+r < xi->width? x0+r+1 : xi->width;
    int ystart = y0-r >= 0? y0-r : 0;
    int yend   = y0+r < xi->height? y0+r+1 : xi->height;
    double rsq = (double)r * (double)r;

    for (int y = ystart; y < yend; ++y)
    {
        for (int x = xstart; x < xend; ++x)
        {
            double mx = (double)x - (double)x0 + 0.5;
            double my = (double)y - (double)y0 + 0.5;
            if (mx*mx + my*my <= rsq)
            {
                *XI_PIXEL(xi, x, y) = color;
            }
        }
    }
}

void xi_draw_triangle(ximage_t *xi, int x1, int y1, int x2, int y2, int x3, int y3, color_t color)
{
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
                *XI_PIXEL(xi, x, y) = color;
        }
    }
}

void xi_draw_texture(ximage_t *xi, ximage_t *texture, int x0, int y0, int width, int height)
{
    int xstart = x0 >= 0? x0 : 0;
    int xend   = x0+width < xi->width? x0+width : xi->width;
    int ystart = y0 >= 0? y0 : 0;
    int yend   = y0+height < xi->height? y0+height : xi->height;

    for (int y = ystart; y < yend; ++y)
    {
        for (int x = xstart; x < xend; ++x)
        {
            color_t color = *XI_PIXEL(texture,
                (int)((float)texture->width *(float)(x-x0)/(float)width),
                (int)((float)texture->height*(float)(y-y0)/(float)height)
            );
            *XI_PIXEL(xi, x, y) = color;
        }
    }
}

#endif // #ifdef XIMAGE_IMPLEMENTATION


#endif // #ifndef XIMAGE_H
