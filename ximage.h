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
void xi_draw_texture(ximage_t *xi, ximage_t *texture, int x0, int y0, int width, int height, unsigned char alpha);

#define XI_RGBA(R, G, B, A) (color_t){ .r=(R), .g=(G), .b=(B), .a=(A) }
#define XI_RGB(R, G, B) (color_t){ .r=(R), .g=(G), .b=(B), .a=255 }

#ifndef XI_ASSERT
    #define XI_ASSERT(cond, msg)
#endif // #ifndef XI_ASSERT


#ifdef XIMAGE_IMPLEMENTATION

#define XI_NULL ((void *)0)

#define XI_PUT_PIXEL(xi, x, y, color)  (xi)->data[(y)*(xi)->width + (x)] = (color)
#define XI_GET_PIXEL(xi, x, y)        &(xi)->data[(y)*(xi)->width + (x)]
#define XI_GET_COLOR(xi, x, y)         (xi)->data[(y)*(xi)->width + (x)]

static inline color_t blend_colors(color_t fg, color_t bg)
{
    unsigned char k = 255 - fg.a;
    color_t color = {
        .r = (unsigned char)((fg.r*fg.a + bg.r*k)/255),
        .b = (unsigned char)((fg.b*fg.a + bg.b*k)/255),
        .g = (unsigned char)((fg.g*fg.a + bg.g*k)/255),
        .a = 255
    };
    return color;
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

void xi_fill_bg(ximage_t *xi, color_t color)
{
    XI_ASSERT(xi != XI_NULL, "xi pointer must not be NULL");

    for (int i = 0; i < xi->width*xi->height; ++i)
        xi->data[i] = color;
}

void xi_put_pixel(ximage_t *xi, int x, int y, color_t color)
{
    XI_ASSERT(xi != XI_NULL, "xi pointer must not be NULL");

    if (x >= 0 && x < xi->width && y >= 0 && y < xi->height)
    {
        XI_PUT_PIXEL(xi, x, y, blend_colors(color, XI_GET_COLOR(xi, x, y)));
    }
}

void xi_draw_rect(ximage_t *xi, int x0, int y0, int width, int height, color_t color)
{
    XI_ASSERT(xi != XI_NULL, "xi pointer must not be NULL");

    int xstart = x0 >= 0? x0 : 0;
    int xend   = x0+width < xi->width? x0+width : xi->width;
    int ystart = y0 >= 0? y0 : 0;
    int yend   = y0+height < xi->height? y0+height : xi->height;

    for (int y = ystart; y < yend; ++y)
    {
        for (int x = xstart; x < xend; ++x)
        {
            XI_PUT_PIXEL(xi, x, y, blend_colors(color, XI_GET_COLOR(xi, x, y)));
        }
    }
}

void xi_draw_circle(ximage_t *xi, int x0, int y0, int r, color_t color)
{
    XI_ASSERT(xi != XI_NULL, "xi pointer must not be NULL");

    int xstart = x0-r >= 0? x0-r : 0;
    int xend   = x0+r < xi->width? x0+r+1 : xi->width;
    int ystart = y0-r >= 0? y0-r : 0;
    int yend   = y0+r < xi->height? y0+r+1 : xi->height;
    double rsq = (double)(r*r);

    for (int y = ystart; y < yend; ++y)
    {
        for (int x = xstart; x < xend; ++x)
        {
            double mx = (double)x - (double)x0 + 0.5;
            double my = (double)y - (double)y0 + 0.5;
            if (mx*mx + my*my <= rsq)
            {
                XI_PUT_PIXEL(xi, x, y, blend_colors(color, XI_GET_COLOR(xi, x, y)));
            }
        }
    }
}

void xi_draw_triangle(ximage_t *xi, int x1, int y1, int x2, int y2, int x3, int y3, color_t color)
{
    XI_ASSERT(xi != XI_NULL, "xi pointer must not be NULL");

    float X1 = (float)x1;
    float Y1 = (float)y1;
    float X2 = (float)x2;
    float Y2 = (float)y2;
    float X3 = (float)x3;
    float Y3 = (float)y3;

    int xstart = (int)xi_maxf(            0.0f,  xi_minf(X1, xi_minf(X2, X3)));
    int xend   = (int)xi_minf((float)xi->width,  xi_maxf(X1, xi_maxf(X2, X3)));
    int ystart = (int)xi_maxf(            0.0f,  xi_minf(Y1, xi_minf(Y2, Y3)));
    int yend   = (int)xi_minf((float)xi->height, xi_maxf(Y1, xi_maxf(Y2, Y3)));

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
            if (l1 > 0.0f && l2 > 0.0f && l3 > 0.0f)
                XI_PUT_PIXEL(xi, x, y, blend_colors(color, XI_GET_COLOR(xi, x, y)));
                //XI_PUT_PIXEL(xi, x, y,
                    //XI_RGB(
                        //(unsigned char)(l1*255.0f),
                        //(unsigned char)(l2*255.0f),
                        //(unsigned char)(l3*255.0f)
                    //)
                //);
        }
    }
}

void xi_draw_texture(ximage_t *xi, ximage_t *texture, int x0, int y0, int width, int height, unsigned char alpha)
{
    XI_ASSERT(xi != XI_NULL, "xi pointer must not be NULL");
    XI_ASSERT(texture != XI_NULL, "texture pointer must not be NULL");

    int xstart = x0 >= 0? x0 : 0;
    int xend   = x0+width < xi->width? x0+width : xi->width;
    int ystart = y0 >= 0? y0 : 0;
    int yend   = y0+height < xi->height? y0+height : xi->height;

    for (int y = ystart; y < yend; ++y)
    {
        for (int x = xstart; x < xend; ++x)
        {
            color_t fg = XI_GET_COLOR(texture,
                (int)((float)texture->width *(float)(x-x0)/(float)width),
                (int)((float)texture->height*(float)(y-y0)/(float)height)
            );
            color_t bg = XI_GET_COLOR(xi, x, y);
            unsigned char k = 255 - alpha;
            color_t color = {
                .r = (unsigned char)((fg.r*alpha + bg.r*k)/255),
                .b = (unsigned char)((fg.b*alpha + bg.b*k)/255),
                .g = (unsigned char)((fg.g*alpha + bg.g*k)/255),
                .a = 255
            };
            XI_PUT_PIXEL(xi, x, y, color);
        }
    }
}

#endif // #ifdef XIMAGE_IMPLEMENTATION


#endif // #ifndef XIMAGE_H
