#define XIMAGE_IMPLEMENTATION
#include "../ximage.h"

#define WIDTH 800
#define HEIGHT 600

#define RADIUS 10
#define START_SPEED 300.0

color_t canvas[WIDTH*HEIGHT];
ximage_t img = { .width=WIDTH, .height=HEIGHT, .data=canvas };


typedef struct { float x; float y; } v2f;

typedef struct
{
    v2f pos;
    v2f speed;
    color_t color;
} ball_t;


ball_t ball_a = {
    .pos   = (v2f){ WIDTH/2.0, HEIGHT/2.0 },
    .speed = (v2f){ START_SPEED, START_SPEED },
    .color = (color_t){ 255, 0, 0, 255 },
};

ball_t ball_b = {
    .pos   = (v2f){ WIDTH/2.0, HEIGHT/2.0 },
    .speed = (v2f){ START_SPEED, START_SPEED },
    .color = (color_t){ 0, 255, 0, 255 },
};

void draw_demo_a(float dt)
{
    xi_fill_bg(&img, XI_RGB(42, 42, 42));
    xi_draw_circle(&img, (int)ball_a.pos.x, (int)ball_a.pos.y, 10, ball_a.color);

    ball_a.pos.x += ball_a.speed.x*dt;
    ball_a.pos.y += ball_a.speed.y*dt;

    if (ball_a.pos.x - RADIUS <= 0.0 || ball_a.pos.x + RADIUS >= WIDTH)
        ball_a.speed.x = -ball_a.speed.x;
    if (ball_a.pos.y - RADIUS <= 0.0 || ball_a.pos.y + RADIUS >= HEIGHT)
        ball_a.speed.y = -ball_a.speed.y;
}

void draw_demo_b(float dt)
{
    xi_fill_bg(&img, XI_RGB(42, 42, 42));
    xi_draw_circle(&img, (int)ball_b.pos.x, (int)ball_b.pos.y, 10, ball_b.color);

    ball_b.pos.x += ball_b.speed.x*dt;
    ball_b.pos.y += ball_b.speed.y*dt;

    if (ball_b.pos.x - RADIUS <= 0.0 || ball_b.pos.x + RADIUS >= WIDTH)
        ball_b.speed.x = -ball_b.speed.x;
    if (ball_b.pos.y - RADIUS <= 0.0 || ball_b.pos.y + RADIUS >= HEIGHT)
        ball_b.speed.y = -ball_b.speed.y;
}
