#include <math.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

#include "vec.h"

#define LEN(array) (sizeof(array) / sizeof(array[0]))

const uint32_t CANVAS_WIDTH  = 512;
const uint32_t CANVAS_HEIGHT = 512;
const uint32_t CANVAS_SIZE   = CANVAS_WIDTH * CANVAS_HEIGHT * 3;

const int32_t CANVAS_HALF_WIDTH  = CANVAS_WIDTH / 2;
const int32_t CANVAS_HALF_HEIGHT = CANVAS_HEIGHT / 2;

const float VIEWPORT_WIDTH  = 1.2;
const float VIEWPORT_HEIGHT = 1.2;
const float VIEWPORT_Z      = 2.0;

const Vec3 BACKGROUND_COLOR = { 0.0, 0.0, 0.1 };

/* There is a warning. I don't know how to deal with it. */
uint8_t canvas[CANVAS_SIZE]  = {0};

/* scene */
typedef struct {
    Vec3 center;
    float radius;
    /* 0 is for black, 1 is for white. */
    Vec3 color;
} Sphere;

const Sphere spheres[] = {
    /*    center        radius       color   */
    { {   0,   0,  4 },      1,   { 1, 0, 0 } },
    { { 0.5, 0.5,  3 },    0.3,   { 0, 1, 1 } }
};

typedef struct {
    Vec3 direction;
    float intensity;
} Light;

const Light lights[] = {
    { { -0.1, 0.1, 0.1 }, 0.5 }
};

/* function declarations */
Vec3 canvas_to_viewport(int x, int y);
Vec3 clamp_color(Vec3 color);
/*
 * @param O: the start of the ray
 * @param D: the direction of the ray
 */
bool has_intersection(Vec3 O, Vec3 D);
/*
 * @param O: the start of the ray
 * @param D: the direction of the ray
 */
Vec3 raycast(Vec3 O, Vec3 D, int level);
void render();
void putpixel(size_t x, size_t y, Vec3 color);

/* function definitions */
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
clamp_color(Vec3 color)
{
    if (color.x > 1) color.x = 1;
    if (color.y > 1) color.y = 1;
    if (color.z > 1) color.z = 1;
    return color;
}

bool
has_intersection(Vec3 O, Vec3 D)
{
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

            if (t1 > 0 || t2 > 0) return true;
        }
    }
    return false;
}

/*
 * TODO: split this function into smaller
 * parts. It's too long now.
 */
Vec3
raycast(Vec3 O, Vec3 D, int level)
{
    if (level > 3) {
        Vec3 zero = { 0, 0, 0 };
        return zero;
    }
    
    /* We want the closest intersection. */
    float t = 1e10;
    const Sphere *sph = NULL;

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

            /* 't' must be greater than 0.
             * Otherwise, the intersection is behind the viewport.
             */
            if (t1 > 0 && t1 < t) {
                t = t1; sph = sphere;
            }
            if (t2 > 0 && t2 < t) {
                t = t2; sph = sphere;
            }
        }
    }

    /* calculate lights */
    if (sph) {
        Vec3 P = vec_add(O, vec_nmul(t, D));
        Vec3 N = vec_sub(P, sph->center);
        N = vec_normalize(N);

        /*
         * Here we need to move P away from the sphere
         * a little bit, otherwise 'has_intersection'
         * will find a intersection at P.
         */
        Vec3 _P = vec_add(P, vec_nmul(0.001, N));

        float I = 0.2;
        for (size_t i = 0; i < LEN(lights); i++) {
            const Light *light = &lights[i];
            const Vec3 L = vec_opp(light->direction);

            /* is in shadow? */
            bool in_shadow = has_intersection(_P, L);
            if (in_shadow) continue;

            float rate1 = vec_dot(N, L);
            rate1 /= vec_norm(L);
            if (rate1 < 0) rate1 = 0;

            Vec3 R = vec_flip(L, N);
            Vec3 V = vec_opp(D);
            float rate2 = powf(vec_dot(R, V) / vec_norm(R) / vec_norm(V), 8);
            if (rate2 < 0) rate2 = 0;
            I += (rate1 + rate2) * light->intensity;
        }
        if (I > 1) I = 1;

        Vec3 R = vec_flip(vec_opp(D), N);
        /* reflections */
        return clamp_color(
            vec_add(vec_nmul(I, sph->color),
            vec_nmul(0.5, raycast(_P, R, level+1)))
        );
    } else {
        return BACKGROUND_COLOR;
    }
}

void
render()
{
    for (int x = -CANVAS_HALF_WIDTH; x < CANVAS_HALF_WIDTH; x++) {
        for (int y = -CANVAS_HALF_HEIGHT; y < CANVAS_HALF_HEIGHT; y++) {
            Vec3 V = canvas_to_viewport(x, y);
            /* The direction is 'vec_sub(V, O)' */
            Vec3 color = raycast(V, V, 0);
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

    /* color: [0, 1] -> [0, 255] */
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