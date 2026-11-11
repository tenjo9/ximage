#define XIMAGE_IMPLEMENTATION
#include "../ximage.h"

#include "../3rdparty/stb_image_write.h"


#define WIDTH  8
#define HEIGHT 8
color_t canvas[WIDTH*HEIGHT];
ximage_t img = { .width=WIDTH, .height=HEIGHT, .data=canvas };


int main(void)
{
    xi_fill_bg(&img, XI_RGB(0, 0, 0));

    for (int x = 0; x < WIDTH; ++x)
    {
        for (int y = 0; y < HEIGHT; ++y)
        {
            if ((x+y) % 2 == 0)
            {
                xi_put_pixel(&img, x, y, XI_RGB(255, 0, 255));
            }
        }
    }

    stbi_write_png("output.png", img.width, img.height, 4, (void *)img.data, 4*img.width);
}
