#define XIMAGE_IMPLEMENTATION
#include "../ximage.h"

#define WIDTH 800
#define HEIGHT 600

#define RADIUS 10
#define START_SPEED 300.0

color_t canvas[WIDTH*HEIGHT];
ximage_t img = { .width=WIDTH, .height=HEIGHT, .data=canvas };

double x_pos = WIDTH/2.0;
double y_pos = HEIGHT/2.0;
double x_speed = START_SPEED;
double y_speed = START_SPEED;

void draw_demo(float dt)
{
    xi_fill_bg(&img, XI_RGB(42, 42, 42));
    xi_draw_circle(&img, (int)x_pos, (int)y_pos, 10, XI_RGB(255, 0, 0));

    x_pos += x_speed*dt;
    y_pos += y_speed*dt;

    if (x_pos-RADIUS <= 0.0 || x_pos+RADIUS >= WIDTH)
        x_speed = -x_speed;
    if (y_pos-RADIUS <= 0.0 || y_pos+RADIUS >= HEIGHT)
        y_speed = -y_speed;
}
