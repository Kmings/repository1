#include "vector_math.h"

vector3f vector3f_create(float x, float y, float z) {
    vector3f v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

vector3f vector3f_add(const vector3f* a, const vector3f* b) {
    return vector3f_create(a->x + b->x, a->y + b->y, a->z + b->z);
}

vector3f vector3f_subtract(const vector3f* a, const vector3f* b) {
    return vector3f_create(a->x - b->x, a->y - b->y, a->z - b->z);
}

float vector3f_dot(const vector3f* a, const vector3f* b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

vector3f vector3f_cross(const vector3f* a, const vector3f* b) {
    return vector3f_create(
        a->y * b->z - a->z * b->y,
        a->z * b->x - a->x * b->z,
        a->x * b->y - a->y * b->x
    );
}

float vector3f_magnitude(const vector3f* v) {
    return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

vector3f vector3f_normalize(const vector3f* v) {
    float mag = vector3f_magnitude(v);
    if (mag < 1e-6f) { // 避免除零
        return vector3f_create(0.0f, 0.0f, 0.0f);
    }
    return vector3f_multiply_scalar(v, 1.0f / mag);
}

vector3f vector3f_multiply_scalar(const vector3f* v, float scalar) {
    return vector3f_create(v->x * scalar, v->y * scalar, v->z * scalar);
}

void vector3f_print(const vector3f* v, const char* name) {
    printf("    %s = (%.2f, %.2f, %.2f)\n", name, v->x, v->y, v->z);
}

void point3f_print(const vector3f* v, const char* name) {
    printf("    %s(%.2f, %.2f, %.2f)\n", name, v->x, v->y, v->z);
}
