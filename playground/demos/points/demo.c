#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define XI_DEBUG
#define XI_ASSERT(cond, msg)\
    if (!(cond)) { fprintf(stderr, "%s:%d: " msg "\n", __FILE__, __LINE__); exit(-1); }
#define XI_ASSERT_VA(cond, msg, ...)\
    if (!(cond)) { fprintf(stderr, "%s:%d: " msg "\n", __FILE__, __LINE__, __VA_ARGS__); exit(-1); }
#define XIMAGE_IMPLEMENTATION
#include "../../../ximage.h"


#define WIDTH  800
#define HEIGHT 600
static color_t canvas[WIDTH*HEIGHT];


static inline float d2r(float deg)
{
    return deg*(float)M_PI/180.0f;
}


int main(void)
{
    ximage_t img = { .width=WIDTH, .height=HEIGHT, .data=canvas };

    float xscale =  (float)WIDTH / 2.0f;
    float yscale = -(float)HEIGHT/ 2.0f;
    float xoffset = (float)WIDTH / 2.0f;
    float yoffset = (float)HEIGHT/ 2.0f;

    float position[] = { 0.0f, 0.0f, 5.0f };
    float points[][3] = {
         // Cube
         {  1.0f,  1.0f, -1.0f },
         {  1.0f, -1.0f, -1.0f },
         { -1.0f, -1.0f, -1.0f },
         { -1.0f,  1.0f, -1.0f },

         {  1.0f,  1.0f, 1.0f },
         {  1.0f, -1.0f, 1.0f },
         { -1.0f, -1.0f, 1.0f },
         { -1.0f,  1.0f, 1.0f },

         // Triangle
         //{  0.0f,  1.0f, 0.0f },
         //{  1.0f, -1.0f, -1.0f },
         //{ -1.0f, -1.0f, -1.0f },
         //{  1.0f, -1.0f,  1.0f },
         //{ -1.0f, -1.0f,  1.0f },
    };
    int points_count = sizeof(points) / sizeof(*points);


    int   fps   = 120;
    float speed = 36.0f;

    float angle = 0.0f;
    float dt    = 1.0f / (float)fps;

    static char header[100] = {0};
    static unsigned char rgb[WIDTH*HEIGHT*3] = {0};

    for (int second = 0; second < 10; ++second)
    {
        for (int frame = 0; frame < fps; ++frame)
        {
            xi_fill_bg(&img, XI_RGB(23, 22, 22));

            angle += speed*dt;
            float c = cosf(d2r(angle));
            float s = sinf(d2r(angle));

            for (int i = 0; i < points_count; ++i)
            {
                float world[] = { points[i][0], points[i][1], points[i][2] };
                float x = world[0];
                float z = world[2];
                world[0] =  x*c - z*s;
                world[2] =  x*s + z*c;

                world[0] += position[0];
                world[1] += position[1];
                world[2] += position[2];

                float ndc[] = { world[0]/world[2], world[1]/world[2] };
                int screen[] = {
                    (int)lroundf(xscale*ndc[0] + xoffset),
                    (int)lroundf(yscale*ndc[1] + yoffset)
                };

                xi_draw_circle(&img, screen[0], screen[1], 10, XI_RGB(255, 0, 0));
            }

            int header_len = snprintf(header, sizeof(header), "P6\n%d %d\n255\n", img.width, img.height);
            fwrite(header, 1, (size_t)header_len, stdout);

            for (int i = 0; i < img.width*img.height; ++i)
            {
                rgb[i*3 + 0] = img.data[i].r;
                rgb[i*3 + 1] = img.data[i].g;
                rgb[i*3 + 2] = img.data[i].b;
            }
            fwrite(rgb, 1, WIDTH*HEIGHT*3, stdout);
        }
    }

    return 0;
}
