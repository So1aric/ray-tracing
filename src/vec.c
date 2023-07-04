#include "vec.h"
#include <math.h>

Vec3
vec_add(Vec3 lhs, Vec3 rhs)
{
    Vec3 result = {
        lhs.x + rhs.x,
        lhs.y + rhs.y,
        lhs.z + rhs.z
    };
    return result;
}

Vec3
vec_sub(Vec3 lhs, Vec3 rhs)
{
    Vec3 result = {
        lhs.x - rhs.x,
        lhs.y - rhs.y,
        lhs.z - rhs.z
    };
    return result;
}

Vec3
vec_mul(Vec3 lhs, Vec3 rhs)
{
    Vec3 result = {
        lhs.x * rhs.x,
        lhs.y * rhs.y,
        lhs.z * rhs.z
    };
    return result;
}

Vec3
vec_div(Vec3 lhs, Vec3 rhs)
{
    Vec3 result = {
        lhs.x / rhs.x,
        lhs.y / rhs.y,
        lhs.z / rhs.z
    };
    return result;
}

Vec3
vec_nadd(float num, Vec3 vec)
{
    Vec3 result = {
        vec.x + num,
        vec.y + num,
        vec.z + num
    };
    return result;
}

Vec3
vec_nsub(float num, Vec3 vec)
{
    Vec3 result = {
        vec.x - num,
        vec.y - num,
        vec.z - num
    };
    return result;
}

Vec3
vec_nmul(float num, Vec3 vec)
{
    Vec3 result = {
        vec.x * num,
        vec.y * num,
        vec.z * num
    };
    return result;
}
Vec3
vec_ndiv(float num, Vec3 vec)
{
    Vec3 result = {
        vec.x / num,
        vec.y / num,
        vec.z / num
    };
    return result;
}
float
vec_dot(Vec3 lhs, Vec3 rhs)
{
    float result = lhs.x * rhs.x
        + lhs.y * rhs.y
        + lhs.z * rhs.z;
    return result;
}

float
vec_norm(Vec3 vec)
{
    return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

Vec3
vec_normalize(Vec3 vec)
{
    float norm = vec_norm(vec);
    Vec3 result = {
        vec.x / norm,
        vec.y / norm,
        vec.z / norm
    };
    return result;
}

Vec3
vec_opp(Vec3 vec)
{
    Vec3 result = {
        -vec.x,
        -vec.y,
        -vec.z
    };
    return result;
}

Vec3
vec_flip(Vec3 L, Vec3 N)
{
    Vec3 result = vec_sub(vec_nmul(2 * vec_dot(N, L), N), L);
    return result;
}
