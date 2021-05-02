#ifndef VM_H
#define VM_H

#include <math.h>

#define VM_PI 3.14159265358979323846264338327950288
#define VM_PI_2 1.57079632679489661923132169163975144
#define VM_PI_4 0.785398163397448309615660845819875721
#define VM_PIf ((float)VM_PI)
#define VM_PI_2f ((float)VM_PI_2)
#define VM_PI_4f ((float)VM_PI_4)

/* #############################################################################
 * # TYPE DEFINITIONS
 * ############################################################################# 
 */
typedef struct vec3 {
    float x;
    float y;
    float z;
} vec3;

typedef struct mat4 {
    float m[4][4];
} mat4;

/* #############################################################################
 * # COMMON MATH FUNCTIONS
 * ############################################################################# 
 */
extern float vm_invsqrt(float number) {

    union {
        float f;
        uint32_t i;
    } conv;

    float x2;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    conv.f = number;
    conv.i = 0x5f3759df - (conv.i >> 1);
    conv.f = conv.f * (threehalfs - (x2 * conv.f * conv.f));
    return conv.f;
}

extern float vm_rad(float degree) {
    return degree * VM_PIf / 180.0f;
}

extern float vm_deg(float radians) {
    return radians * 180.0f / VM_PIf;
}

/* #############################################################################
 * # VECTOR 3 FUNCTIONS
 * ############################################################################# 
 */
extern vec3 vm_vec3_add(vec3 a, vec3 b) {
    vec3 res = {};
    res.x = a.x + b.x;
    res.y = a.y + b.y;
    res.z = a.z + b.z;
    return res;
}

extern vec3 vm_vec3_addf(vec3 a, float b) {
    vec3 res = {};
    res.x = a.x + b;
    res.y = a.y + b;
    res.z = a.z + b;
    return res;
}

extern vec3 vm_vec3_sub(vec3 a, vec3 b) {
    vec3 res = {};
    res.x = a.x - b.x;
    res.y = a.y - b.y;
    res.z = a.z - b.z;
    return res;
}

extern vec3 vm_vec3_subf(vec3 a, float b) {
    vec3 res = {};
    res.x = a.x - b;
    res.y = a.y - b;
    res.z = a.z - b;
    return res;
}

extern vec3 vm_vec3_mul(vec3 a, vec3 b) {
    vec3 res = {};
    res.x = a.x * b.x;
    res.y = a.y * b.y;
    res.z = a.z * b.z;
    return res;
}

extern vec3 vm_vec3_mulf(vec3 a, float b) {
    vec3 res = {};
    res.x = a.x * b;
    res.y = a.y * b;
    res.z = a.z * b;
    return res;
}

extern vec3 vm_vec3_div(vec3 a, vec3 b) {
    vec3 res = {};
    res.x = a.x / b.x;
    res.y = a.y / b.y;
    res.z = a.z / b.z;
    return res;
}

extern vec3 vm_vec3_divf(vec3 a, float b) {
    float inv = 1.0f / b;

    vec3 res = {};
    res.x = a.x * inv;
    res.y = a.y * inv;
    res.z = a.z * inv;
    return res;
}

extern vec3 vm_vec3_normalize(vec3 a) {
    float scalar = vm_invsqrt(fmaf(a.x, a.x, fmaf(a.y, a.y, a.z * a.z)));

    vec3 res = {};
    res.x = a.x * scalar;
    res.y = a.y * scalar;
    res.z = a.z * scalar;

    return res;
}

extern vec3 vm_vec3_cross(vec3 a, vec3 b) {
    vec3 res = {};
    res.x = fmaf(a.y, b.z, -a.z * b.y);
    res.y = fmaf(a.z, b.x, -a.x * b.z);
    res.z = fmaf(a.x, b.y, -a.y * b.x);
    return res;
}

extern vec3 vm_vec3_lerp(vec3 a, vec3 b, float t) {
    vec3 res = {};
    res.x = fmaf(b.x - a.x, t, a.x);
    res.y = fmaf(b.y - a.y, t, a.y);
    res.z = fmaf(b.z - a.z, t, a.z);
    return res;
}

extern float vm_vec3_length(vec3 a) {
    return sqrtf(fmaf(a.x, a.x, fmaf(a.y, a.y, a.z * a.z)));
}

extern float vm_vec3_dot(vec3 a, vec3 b) {
    return fmaf(a.x, b.x, fmaf(a.y, b.y, a.z * b.z));
}

/* #############################################################################
 * # MATRIX 4x4 FUNCTIONS
 * ############################################################################# 
 */

extern mat4 vm_mat4_zero() {
    mat4 res;
    res.m[0][0] = 0.0f;
    res.m[0][1] = 0.0f;
    res.m[0][2] = 0.0f;
    res.m[0][3] = 0.0f;

    res.m[1][0] = 0.0f;
    res.m[1][1] = 0.0f;
    res.m[1][2] = 0.0f;
    res.m[1][3] = 0.0f;

    res.m[2][0] = 0.0f;
    res.m[2][1] = 0.0f;
    res.m[2][2] = 0.0f;
    res.m[2][3] = 0.0f;

    res.m[3][0] = 0.0f;
    res.m[3][1] = 0.0f;
    res.m[3][2] = 0.0f;
    res.m[3][3] = 0.0f;
    return res;
}

extern mat4 vm_mat4_identity() {
    mat4 res;
    res.m[0][0] = 1.0f;
    res.m[0][1] = 0.0f;
    res.m[0][2] = 0.0f;
    res.m[0][3] = 0.0f;

    res.m[1][0] = 0.0f;
    res.m[1][1] = 1.0f;
    res.m[1][2] = 0.0f;
    res.m[1][3] = 0.0f;

    res.m[2][0] = 0.0f;
    res.m[2][1] = 0.0f;
    res.m[2][2] = 1.0f;
    res.m[2][3] = 0.0f;

    res.m[3][0] = 0.0f;
    res.m[3][1] = 0.0f;
    res.m[3][2] = 0.0f;
    res.m[3][3] = 1.0f;
    return res;
}

extern mat4 vm_mat4_mul(mat4 a, mat4 b) {
    mat4 res = {};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            res.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j] + a.m[i][3] * b.m[3][j];
        }
    }
    return res;
}

extern mat4 vm_mat4_perspective(float fov, float aspectRatio, float zNear, float zFar) {
    float f = 1.0f / tanf(fov * 0.5f);
    float fn = 1.0f / (zNear - zFar);

    mat4 res = vm_mat4_zero();
    res.m[0][0] = f / aspectRatio;
    res.m[1][1] = f;
    res.m[2][2] = (zNear + zFar) * fn;
    res.m[3][2] = -1.0f;
    res.m[2][3] = 2.0f * zNear * zFar * fn;
    return res;
}

extern mat4 vm_mat4_translate(mat4 src, vec3 b) {
    mat4 res = src;
    res.m[0][3] = b.x;
    res.m[1][3] = b.y;
    res.m[2][3] = b.z;
    return res;
}

extern mat4 vm_mat4_rotate(mat4 src, float angle, vec3 axis) {
    float c = cosf(angle);

    vec3 axisn = vm_vec3_normalize(axis);
    vec3 v = vm_vec3_mulf(axisn, 1.0f - c);
    vec3 vs = vm_vec3_mulf(axisn, sinf(angle));

    mat4 rot = vm_mat4_zero();

    vec3 a = vm_vec3_mulf(axisn, v.x);
    rot.m[0][0] = a.x;
    rot.m[1][0] = a.y;
    rot.m[2][0] = a.z;

    vec3 b = vm_vec3_mulf(axisn, v.y);
    rot.m[0][1] = b.x;
    rot.m[1][1] = b.y;
    rot.m[2][1] = b.z;

    vec3 f = vm_vec3_mulf(axisn, v.z);
    rot.m[0][2] = f.x;
    rot.m[1][2] = f.y;
    rot.m[2][2] = f.z;

    rot.m[0][0] += c;
    rot.m[0][1] -= vs.z;
    rot.m[0][2] += vs.y;
    rot.m[1][0] += vs.z;
    rot.m[1][1] += c;
    rot.m[1][2] -= vs.x;
    rot.m[2][0] -= vs.y;
    rot.m[2][1] += vs.x;
    rot.m[2][2] += c;

    rot.m[3][0] = 0.0f;
    rot.m[3][1] = 0.0f;
    rot.m[3][2] = 0.0f;
    rot.m[0][3] = 0.0f;
    rot.m[1][3] = 0.0f;
    rot.m[2][3] = 0.0f;
    rot.m[3][3] = 1.0f;

    return vm_mat4_mul(src, rot);
}

extern mat4 vm_mat4_lookAt(vec3 eye, vec3 target, vec3 up) {
    vec3 f = vm_vec3_normalize(vm_vec3_sub(target, eye));
    vec3 s = vm_vec3_normalize(vm_vec3_cross(f, up));
    vec3 u = vm_vec3_cross(s, f);

    mat4 res = {};
    res.m[0][0] = s.x;
    res.m[0][1] = s.y;
    res.m[0][2] = s.z;
    res.m[0][3] = -vm_vec3_dot(s, eye);
    res.m[1][0] = u.x;
    res.m[1][1] = u.y;
    res.m[1][2] = u.z;
    res.m[1][3] = -vm_vec3_dot(u, eye);
    res.m[2][0] = -f.x;
    res.m[2][1] = -f.y;
    res.m[2][2] = -f.z;
    res.m[2][3] = vm_vec3_dot(f, eye);
    res.m[3][3] = 1.0f;

    return res;
}

#endif /* VM_H */
