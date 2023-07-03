#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

#include "vec.h"

#define LEN(array) (sizeof(array) / sizeof(array[0]))

const uint32_t CANVAS_WIDTH  = 256;
const uint32_t CANVAS_HEIGHT = 256;
const uint32_t CANVAS_SIZE   = CANVAS_WIDTH * CANVAS_HEIGHT * 3;

const int32_t CANVAS_HALF_WIDTH  = CANVAS_WIDTH / 2;
const int32_t CANVAS_HALF_HEIGHT = CANVAS_HEIGHT / 2;

const float VIEWPORT_WIDTH  = 1.0;
const float VIEWPORT_HEIGHT = 1.0;
const float VIEWPORT_Z      = 2.0;

const Vec3 BACKGROUND_COLOR = { 0.0, 0.0, 0.1 };

uint8_t canvas[CANVAS_SIZE]  = {0};

typedef struct {
    Vec3 center;
    float radius;
    Vec3 color;
} Sphere;

const Sphere spheres[] = {
    { {   0,   0, 4 },   1, { 1, 0, 0 } },
    { { 0.5, 0.5, 3 }, 0.3, { 0, 1, 1 } }
};

Vec3 canvas_to_viewport(int x, int y);
Vec3 raycast(Vec3 from, Vec3 to);
void render();
void putpixel(size_t x, size_t y, Vec3 color);

Vec3
canvas_to_viewport(int x, int y)
{
    Vec3 result = {
        x * VIEWPORT_WIDTH / CANVAS_WIDTH,
        y * VIEWPORT_HEIGHT / CANVAS_HEIGHT,
        VIEWPORT_Z
    };
    return result;
}

Vec3
raycast(Vec3 O, Vec3 D)
{
    float t = 1e10;
    Vec3 color = BACKGROUND_COLOR;

    for (size_t i = 0; i < LEN(spheres); i++) {
        const Sphere *sphere = &spheres[i];

        Vec3 CO = vec_sub(O, sphere->center);
        float a = vec_dot(D, D);
        float b = vec_dot(CO, D);
        float c = vec_dot(CO, CO) - sphere->radius * sphere->radius;

        float delta = b * b - a * c;
        if (delta < 0) {
            continue;
        } else {
            float part1 = -b / a;
            float part2 = sqrtf(delta) / a;
            float t1 = part1 + part2;
            float t2 = part1 - part2;

            if (t1 > 0 && t1 < t) {
                t = t1; color = sphere->color;
            }
            if (t2 > 0 && t2 < t) {
                t = t2; color = sphere->color;
            }
        }
    }

    return color;
}

void
render()
{
    for (int x = -CANVAS_HALF_WIDTH; x < CANVAS_HALF_WIDTH; x++) {
        for (int y = -CANVAS_HALF_HEIGHT; y < CANVAS_HALF_HEIGHT; y++) {
            Vec3 V = canvas_to_viewport(x, y);
            Vec3 color = raycast(V, V);
            putpixel(x, y, color);
        }
    }
}

void
putpixel(size_t x, size_t y, Vec3 color)
{
    size_t _x = x + CANVAS_HALF_WIDTH;
    size_t _y = y + CANVAS_HALF_HEIGHT;
    size_t index = (_y * CANVAS_WIDTH + _x) * 3;

    canvas[index++] = (int)(color.x * 255);
    canvas[index++] = (int)(color.y * 255);
    canvas[index]   = (int)(color.z * 255);
}

int
main()
{
    render();
    stbi_write_bmp("result.bmp", CANVAS_WIDTH, CANVAS_HEIGHT, 3, &canvas);
    return 0;
}