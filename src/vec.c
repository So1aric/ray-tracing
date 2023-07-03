#include "vec.h"

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

float
vec_dot(Vec3 lhs, Vec3 rhs)
{
	float result = lhs.x * rhs.x
		+ lhs.y * rhs.y
		+ lhs.z * rhs.z;
	return result;
}
