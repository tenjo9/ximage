#include <stdio.h>
#include <stdlib.h>
#include "../3rdparty/stb_image_write.h"

#define XI_DEBUG
#define XI_ASSERT(cond, msg)\
    if (!(cond)) { fprintf(stderr, "%s:%d: " msg "\n", __FILE__, __LINE__); exit(-1); }
#define XI_ASSERT_VA(cond, msg, ...)\
    if (!(cond)) { fprintf(stderr, "%s:%d: " msg "\n", __FILE__, __LINE__, __VA_ARGS__); exit(-1); }
#define XIMAGE_IMPLEMENTATION
#include "../ximage.h"


void test(ximage_t *(*testing)(void), char *output)
{
    ximage_t *img = testing();
    stbi_write_png(output, img->width, img->height, 4, img->data, 4*img->width);
    free(img);
}

ximage_t *create_canvas(int width, int height)
{
    ximage_t *xi = malloc(sizeof(ximage_t) + (size_t)(width*height)*sizeof(color_t));
    xi->width = width;
    xi->height = height;
    xi->data = (color_t *)(xi + 1);
    return xi;
}

ximage_t *basic(void)
{
    int WIDHT = 800;
    int HEIGHT = 600;
    int TEX_WIDTH = 100;
    int TEX_HEIGHT = 100;

    ximage_t *xi = create_canvas(WIDHT, HEIGHT);
    ximage_t *tex = create_canvas(TEX_WIDTH, TEX_HEIGHT);

    // Texture
    xi_fill_bg(tex, XI_RGB(250, 10, 235));
    xi_draw_circle(tex, TEX_WIDTH/2, TEX_HEIGHT/2, 30, XI_RGB(250, 215, 10));

    // Background
    xi_fill_bg(xi, XI_RGB(42, 42, 42));

    // Main red cicle in center
    xi_draw_circle(xi, 400, 300, 200, XI_RGB(255, 0, 0));

    // Lines
    //xi_draw_line_thick(xi, 100, 100, 700, 500, 10, XI_RGBA(255, 255, 255, 128));
    //xi_draw_line_thick(xi, 100, 500, 700, 100, 10, XI_RGBA(255, 255, 0, 128));

    // Left green rect
    xi_draw_rect(xi, 100, 200, 200, 200, XI_RGB(0, 255, 0));

    // Blue triangle on purple rect
    xi_draw_rect(xi, 500, 100, 200, 300, XI_RGB(100, 0, 230));
    xi_draw_triangle(xi, 600, 100, 500, 400, 700, 400, XI_RGB(0, 0, 255));

    // Texture
    xi_draw_texture(xi, tex, 400, 300, 200, 200);

    // Yellow cicle on pink rect
    xi_draw_rect(xi, 0, 0, 200, 200, XI_RGB(0, 0, 255));
    xi_draw_circle(xi, 100, 100, 100, XI_RGB(0, 255, 150));

    // Single white pixel
    xi_put_pixel(xi, 400, 300, XI_RGB(255, 255, 255));

    // Right triangle
    xi_draw_triangle(xi, 50, 300, 50, 500, 200, 500, XI_RGB(250, 0, 130));

    free(tex);
    return xi;
}

int main(void)
{
    test(basic, "basic.png");
}
