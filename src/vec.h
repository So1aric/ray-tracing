typedef struct {
    float x;
    float y;
    float z;
} Vec3;

Vec3 vec_add(Vec3 lhs, Vec3 rhs);
Vec3 vec_sub(Vec3 lhs, Vec3 rhs);
Vec3 vec_mul(Vec3 lhs, Vec3 rhs);
Vec3 vec_div(Vec3 lhs, Vec3 rhs);

float vec_dot(Vec3 lhs, Vec3 rhs);
