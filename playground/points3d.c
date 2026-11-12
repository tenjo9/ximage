#include <math.h>
#include <stdio.h>

#include "../3rdparty/stb_image_write.h"

#define XI_DEBUG
#define XI_ASSERT(cond, msg)\
    if (!(cond)) { fprintf(stderr, "%s:%d: " msg "\n", __FILE__, __LINE__); exit(-1); }
#define XI_ASSERT_VA(cond, msg, ...)\
    if (!(cond)) { fprintf(stderr, "%s:%d: " msg "\n", __FILE__, __LINE__, __VA_ARGS__); exit(-1); }
#define XIMAGE_IMPLEMENTATION
#include "../ximage.h"


#define WIDTH  800
#define HEIGHT 600
color_t canvas[WIDTH*HEIGHT];


static inline float d2r(float deg)
{
    return deg*3.1415f/180.0f;
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
         {  1.0f,  1.0f, -1.0f },
         {  1.0f, -1.0f, -1.0f },
         { -1.0f, -1.0f, -1.0f },
         { -1.0f,  1.0f, -1.0f },

         {  1.0f,  1.0f, 1.0f },
         {  1.0f, -1.0f, 1.0f },
         { -1.0f, -1.0f, 1.0f },
         { -1.0f,  1.0f, 1.0f },
    };
    int points_size = sizeof(points) / sizeof(*points);


    xi_fill_bg(&img, XI_RGB(23, 22, 22));

    float angle = 0.0f;
    float c = cosf(d2r(angle));
    float s = sinf(d2r(angle));
    for (int i = 0; i < points_size; ++i)
    {
        float x = points[i][0];
        float z = points[i][2];

        points[i][0] =  x*c - z*s;
        points[i][2] =  x*s + z*c;
    }

    for (int i = 0; i < points_size; ++i)
    {
        float world[] = { points[i][0]+position[0], points[i][1]+position[1], points[i][2]+position[2] };
        float ndc[] = { world[0]/world[2], world[1]/world[2] };
        int screen[] = {
            (int)(xscale*ndc[0] + xoffset),
            (int)(yscale*ndc[1] + yoffset)
        };

        xi_draw_circle(&img, screen[0], screen[1], 10, XI_RGB(255, 0, 0));
    }

    stbi_write_png("output.png", img.width, img.height, 4, (void *)img.data, 4*img.width);

    return 0;
}
