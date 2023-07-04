#ifndef VEC
#define VEC
typedef struct {
    float x;
    float y;
    float z;
} Vec3;

Vec3 vec_add(Vec3 lhs, Vec3 rhs);
Vec3 vec_sub(Vec3 lhs, Vec3 rhs);
Vec3 vec_mul(Vec3 lhs, Vec3 rhs);
Vec3 vec_div(Vec3 lhs, Vec3 rhs);

Vec3 vec_nadd(float num, Vec3 vec);
Vec3 vec_nsub(float num, Vec3 vec);
Vec3 vec_nmul(float num, Vec3 vec);
Vec3 vec_ndiv(float num, Vec3 vec);

float vec_dot(Vec3 lhs, Vec3 rhs);
float vec_norm(Vec3 vec);
Vec3 vec_normalize(Vec3 vec);
Vec3 vec_opp(Vec3 vec);
Vec3 vec_flip(Vec3 L, Vec3 N);
#endif