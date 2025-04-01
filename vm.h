/* vm.h - v0.1 - public domain data structures - nickscha 2025

A C89 standard compliant, single header, nostdlib (no C Standard Library) vector linear algebra implementation.

LICENSE

  Placed in the public domain and also MIT licensed.
  See end of file for detailed license information.

*/
#ifndef VM_H
#define VM_H

/* #############################################################################
 * # COMPILER SETTINGS
 * #############################################################################
 */
/* Check if using C99 or later (inline is supported) */
#if __STDC_VERSION__ >= 199901L
#define VM_INLINE inline
#define VM_API extern
#elif defined(__GNUC__) || defined(__clang__)
#define VM_INLINE __inline__
#define VM_API static
#elif defined(_MSC_VER)
#define VM_INLINE __inline
#define VM_API static
#else
#define VM_INLINE
#define VM_API static
#endif

/* #############################################################################
 * # COMMON MATH FUNCTIONS
 * #############################################################################
 */
#define VM_PI 3.14159265358979323846264338327950288
#define VM_PI_2 1.57079632679489661923132169163975144
#define VM_PI_4 0.785398163397448309615660845819875721
#define VM_PIf ((float)VM_PI)
#define VM_PIf_DOUBLED ((float)(2.0f * VM_PIf))

/* Linear Congruential Generator (LCG) constants */
#define VM_LCG_A 1664525U
#define VM_LCG_C 1013904223U
#define VM_LCG_M 4294967296.0f /* 2^32 */

/* Seed for the random number generator */
static unsigned int vm_seed_lcg = 1;

VM_API VM_INLINE unsigned int vm_randi(void)
{
    vm_seed_lcg = (VM_LCG_A * vm_seed_lcg + VM_LCG_C);
    return (vm_seed_lcg);
}

VM_API VM_INLINE float vm_randf(void)
{
    return ((float)vm_randi() / VM_LCG_M);
}

VM_API VM_INLINE float vm_randf_range(float min, float max)
{
    return (min + (max - min) * vm_randf());
}

VM_API VM_INLINE float vm_radf(float degree)
{
    return (degree * (VM_PIf / 180.0f));
}

VM_API VM_INLINE float vm_degf(float radians)
{
    return (radians * (180.0f / VM_PIf));
}

VM_API VM_INLINE float vm_maxf(float a, float b)
{
    return ((a > b) ? a : b);
}

VM_API VM_INLINE int vm_maxi(int a, int b)
{
    return ((a > b) ? a : b);
}

VM_API VM_INLINE float vm_minf(float a, float b)
{
    return ((a < b) ? a : b);
}

VM_API VM_INLINE int vm_mini(int a, int b)
{
    return ((a < b) ? a : b);
}

VM_API VM_INLINE float vm_clampf(float value, float min, float max)
{
    return (vm_maxf(min, vm_minf(max, value)));
}

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#pragma GCC diagnostic ignored "-Wuninitialized"
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4699) /* MSVC-specific aliasing warning */
#endif
VM_API VM_INLINE float vm_invsqrt(float number)
{
    union
    {
        float f;
        long i;
    } conv;

    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    conv.f = number;
    conv.i = 0x5f3759df - (conv.i >> 1); /* Magic number for approximation */
    y = conv.f;
    y = y * (threehalfs - (x2 * y * y)); /* One iteration of Newton's method */

    return (y);
}
#ifdef __GNUC__
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

VM_API VM_INLINE float vm_power(float base, int exp)
{
    float result = 1.0f;
    int i;

    if (exp < 0)
    {
        for (i = 0; i < -exp; ++i)
        {
            result /= base;
        }
    }
    else
    {
        for (i = 0; i < exp; ++i)
        {
            result *= base;
        }
    }

    return (result);
}

VM_API VM_INLINE float vm_fmodf(float x, float y)
{
    int quotient;
    float remainder;

    /* Handle special cases where y is 0 */
    if (y == 0.0f)
    {
        return (0.0f); /* Return 0 as a placeholder for undefined behavior */
    }

    /* Compute the quotient (truncated towards zero) */
    quotient = (int)(x / y);

    /* Compute the remainder: x - (quotient * y) */
    remainder = x - ((float)quotient * y);

    /* Ensure the remainder has the same sign as x */
    if ((remainder > 0 && x < 0) || (remainder < 0 && x > 0))
    {
        remainder += y;
    }

    return (remainder);
}

VM_API VM_INLINE float vm_cosf(float x)
{
    /* Limit the number of terms in the series to 10 for approximation */
    const int terms = 10;
    float result = 1.0f; /* Start with the first term in the series (1) */
    float term = 1.0f;   /* Current term (starting with 1) */
    int sign = -1;       /* Alternating sign for each term */
    int i;

    /* Reduce x to the range [-pi, pi] for better accuracy */
    x = vm_fmodf(x, VM_PIf_DOUBLED);

    if (x > VM_PIf)
    {
        x -= VM_PIf_DOUBLED;
    }
    else if (x < -VM_PIf)
    {
        x += VM_PIf_DOUBLED;
    }

    for (i = 1; i < terms; ++i)
    {
        term *= (x * x) / (float)((2 * i - 1) * (2 * i)); /* Efficient calculation of each term */
        result += (float)sign * (float)term;
        sign = -sign; /* Alternate the sign */
    }

    return (result);
}

VM_API VM_INLINE float vm_sinf(float x)
{
    const int terms = 10;
    float result;
    float term;
    int sign = -1;

    int i;

    /* Reduce x to the range [-pi, pi] for better accuracy */
    x = vm_fmodf(x, VM_PIf_DOUBLED);

    if (x > VM_PIf)
    {
        x -= VM_PIf_DOUBLED;
    }
    else if (x < -VM_PIf)
    {
        x += VM_PIf_DOUBLED;
    }

    result = x;
    term = x;

    for (i = 1; i < terms; ++i)
    {
        term *= (x * x) / (float)((2 * i) * (2 * i + 1));
        result += (float)sign * (float)term;
        sign = -sign;
    }

    return (result);
}

VM_API VM_INLINE float vm_tanf(float x)
{
    return (vm_sinf(x) / vm_cosf(x));
}

VM_API VM_INLINE float vm_absf(float x)
{
    return (x < 0.0f ? -x : x);
}

/* #############################################################################
 * # VECTOR 2 FUNCTIONS
 * #############################################################################
 */
#define VM_V2_ELEMENT_COUNT 2

typedef struct v2
{
    float x;
    float y;
} v2;

VM_API VM_INLINE v2 vm_v2(float x, float y)
{
    v2 result;

    result.x = x;
    result.y = y;

    return (result);
}

VM_API VM_INLINE float *vm_v2_data(v2 *a)
{
    return ((float *)a);
}

VM_API VM_INLINE v2 vm_v2_zero(void)
{
    v2 result;

    result.x = 0.0f;
    result.y = 0.0f;

    return (result);
}

VM_API VM_INLINE v2 vm_v2_one(void)
{
    v2 result;

    result.x = 1.0f;
    result.y = 1.0f;

    return (result);
}

VM_API VM_INLINE int vm_v2_equals(v2 a, v2 b)
{
    return (((a.x == b.x) && (a.y == b.y)) ? 1 : 0);
}

VM_API VM_INLINE v2 vm_v2_add(v2 a, v2 b)
{
    v2 result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;

    return (result);
}

VM_API VM_INLINE v2 vm_v2_addf(v2 a, float b)
{
    v2 result;

    result.x = a.x + b;
    result.y = a.y + b;

    return (result);
}

VM_API VM_INLINE v2 vm_v2_sub(v2 a, v2 b)
{
    v2 result;

    result.x = a.x - b.x;
    result.y = a.y - b.y;

    return (result);
}

VM_API VM_INLINE v2 vm_v2_subf(v2 a, float b)
{
    v2 result;

    result.x = a.x - b;
    result.y = a.y - b;

    return (result);
}

VM_API VM_INLINE v2 vm_v2_mul(v2 a, v2 b)
{
    v2 result;

    result.x = a.x * b.x;
    result.y = a.y * b.y;

    return (result);
}

VM_API VM_INLINE v2 vm_v2_mulf(v2 a, float b)
{
    v2 result;

    result.x = a.x * b;
    result.y = a.y * b;

    return (result);
}

VM_API VM_INLINE v2 vm_v2_div(v2 a, v2 b)
{
    v2 result;

    result.x = a.x / b.x;
    result.y = a.y / b.y;

    return (result);
}

VM_API VM_INLINE v2 vm_v2_divf(v2 a, float b)
{
    v2 result;

    result.x = a.x / b;
    result.y = a.y / b;

    return (result);
}

VM_API VM_INLINE float vm_v2_length_manhatten(v2 start, v2 end, float unit)
{
    return ((vm_absf(start.x - end.x) + vm_absf(start.y - end.y)) / (unit == 0.0f ? 1.0f : unit));
}

/* #############################################################################
 * # VECTOR 3 FUNCTIONS
 * #############################################################################
 */
#define VM_V3_ELEMENT_COUNT 3

typedef struct v3
{
    float x;
    float y;
    float z;
} v3;

VM_API VM_INLINE v3 vm_v3(float x, float y, float z)
{
    v3 result;

    result.x = x;
    result.y = y;
    result.z = z;

    return (result);
}

VM_API VM_INLINE float *vm_v3_data(v3 *a)
{
    return ((float *)a);
}

VM_API VM_INLINE v3 vm_v3_zero(void)
{
    v3 result;

    result.x = 0.0f;
    result.y = 0.0f;
    result.z = 0.0f;

    return (result);
}

VM_API VM_INLINE v3 vm_v3_one(void)
{
    v3 result;

    result.x = 1.0f;
    result.y = 1.0f;
    result.z = 1.0f;

    return (result);
}

VM_API VM_INLINE int vm_v3_equals(v3 a, v3 b)
{
    return (((a.x == b.x) && (a.y == b.y) && (a.z == b.z)) ? 1 : 0);
}

VM_API VM_INLINE v3 vm_v3_add(v3 a, v3 b)
{
    v3 result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;

    return (result);
}

VM_API VM_INLINE v3 vm_v3_addf(v3 a, float b)
{
    v3 result;

    result.x = a.x + b;
    result.y = a.y + b;
    result.z = a.z + b;

    return (result);
}

VM_API VM_INLINE v3 vm_v3_sub(v3 a, v3 b)
{
    v3 result;

    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;

    return (result);
}

VM_API VM_INLINE v3 vm_v3_subf(v3 a, float b)
{
    v3 result;

    result.x = a.x - b;
    result.y = a.y - b;
    result.z = a.z - b;

    return (result);
}

VM_API VM_INLINE v3 vm_v3_mul(v3 a, v3 b)
{
    v3 result;

    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;

    return (result);
}

VM_API VM_INLINE v3 vm_v3_mulf(v3 a, float b)
{
    v3 result;

    result.x = a.x * b;
    result.y = a.y * b;
    result.z = a.z * b;

    return (result);
}

VM_API VM_INLINE v3 vm_v3_div(v3 a, v3 b)
{
    v3 result;

    result.x = a.x / b.x;
    result.y = a.y / b.y;
    result.z = a.z / b.z;

    return (result);
}

VM_API VM_INLINE v3 vm_v3_divf(v3 a, float b)
{
    v3 result;

    result.x = a.x / b;
    result.y = a.y / b;
    result.z = a.z / b;

    return (result);
}

VM_API VM_INLINE v3 vm_v3_cross(v3 a, v3 b)
{
    v3 result;

    result.x = (a.y * b.z) + (-a.z * b.y);
    result.y = (a.z * b.x) + (-a.x * b.z);
    result.z = (a.x * b.y) + (-a.y * b.x);

    return (result);
}

VM_API VM_INLINE float vm_v3_dot(v3 a, v3 b)
{
    return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

VM_API VM_INLINE v3 vm_v3_normalize(v3 a)
{
    float scalar = vm_invsqrt((a.x * a.x) + (a.y * a.y) + (a.z * a.z));

    v3 result;

    result.x = a.x * scalar;
    result.y = a.y * scalar;
    result.z = a.z * scalar;

    return (result);
}

VM_API VM_INLINE v3 vm_v3_lerp(v3 a, v3 b, float t)
{
    v3 result;

    result.x = ((b.x - a.x) * t) + a.x;
    result.y = ((b.y - a.y) * t) + a.y;
    result.z = ((b.z - a.z) * t) + a.z;

    return (result);
}

VM_API VM_INLINE float vm_v3_length_manhatten(v3 start, v3 end, float unit)
{
    return ((vm_absf(start.x - end.x) + vm_absf(start.y - end.y) + vm_absf(start.z - end.z)) / (unit == 0.0f ? 1.0f : unit));
}

/* #############################################################################
 * # VECTOR 4 FUNCTIONS
 * #############################################################################
 */
#define VM_V4_ELEMENT_COUNT 4

typedef struct v4
{
    float x;
    float y;
    float z;
    float w;
} v4;

VM_API VM_INLINE v4 vm_v4(float x, float y, float z, float w)
{
    v4 result;

    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;

    return (result);
}

VM_API VM_INLINE float *vm_v4_data(v4 *a)
{
    return ((float *)a);
}

VM_API VM_INLINE v4 vm_v4_zero(void)
{
    v4 result;

    result.x = 0.0f;
    result.y = 0.0f;
    result.z = 0.0f;
    result.w = 0.0f;

    return (result);
}

VM_API VM_INLINE v4 vm_v4_one(void)
{
    v4 result;

    result.x = 1.0f;
    result.y = 1.0f;
    result.z = 1.0f;
    result.w = 1.0f;

    return (result);
}

VM_API VM_INLINE int vm_v4_equals(v4 a, v4 b)
{
    return (((a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w)) ? 1 : 0);
}

VM_API VM_INLINE v4 vm_v4_add(v4 a, v4 b)
{
    v4 result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    result.w = a.w + b.w;

    return (result);
}

VM_API VM_INLINE v4 vm_v4_addf(v4 a, float b)
{
    v4 result;

    result.x = a.x + b;
    result.y = a.y + b;
    result.z = a.z + b;
    result.w = a.w + b;

    return (result);
}

VM_API VM_INLINE v4 vm_v4_sub(v4 a, v4 b)
{
    v4 result;

    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    result.w = a.w - b.w;

    return (result);
}

VM_API VM_INLINE v4 vm_v4_subf(v4 a, float b)
{
    v4 result;

    result.x = a.x - b;
    result.y = a.y - b;
    result.z = a.z - b;
    result.w = a.w - b;

    return (result);
}

VM_API VM_INLINE v4 vm_v4_mul(v4 a, v4 b)
{
    v4 result;

    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    result.w = a.w * b.w;

    return (result);
}

VM_API VM_INLINE v4 vm_v4_mulf(v4 a, float b)
{
    v4 result;

    result.x = a.x * b;
    result.y = a.y * b;
    result.z = a.z * b;
    result.w = a.w * b;

    return (result);
}

VM_API VM_INLINE v4 vm_v4_div(v4 a, v4 b)
{
    v4 result;

    result.x = a.x / b.x;
    result.y = a.y / b.y;
    result.z = a.z / b.z;
    result.w = a.w / b.w;

    return (result);
}

VM_API VM_INLINE v4 vm_v4_divf(v4 a, float b)
{
    v4 result;

    result.x = a.x / b;
    result.y = a.y / b;
    result.z = a.z / b;
    result.w = a.w / b;

    return (result);
}

VM_API VM_INLINE float vm_v4_dot(v4 v1, v4 v2)
{
    return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w));
}

/* #############################################################################
 * # MATRIX 4x4 FUNCTIONS
 * #############################################################################
 */
#define VM_M4X4_ELEMENT_COUNT 16

typedef struct m4x4
{
    float e[4][4];
} m4x4;

VM_API VM_INLINE m4x4 vm_m4x4_zero(void)
{
    m4x4 result;

    result.e[0][0] = 0.0f;
    result.e[0][1] = 0.0f;
    result.e[0][2] = 0.0f;
    result.e[0][3] = 0.0f;

    result.e[1][0] = 0.0f;
    result.e[1][1] = 0.0f;
    result.e[1][2] = 0.0f;
    result.e[1][3] = 0.0f;

    result.e[2][0] = 0.0f;
    result.e[2][1] = 0.0f;
    result.e[2][2] = 0.0f;
    result.e[2][3] = 0.0f;

    result.e[3][0] = 0.0f;
    result.e[3][1] = 0.0f;
    result.e[3][2] = 0.0f;
    result.e[3][3] = 0.0f;

    return (result);
}

VM_API VM_INLINE m4x4 vm_m4x4_identity(void)
{
    m4x4 result;

    result.e[0][0] = 1.0f;
    result.e[0][1] = 0.0f;
    result.e[0][2] = 0.0f;
    result.e[0][3] = 0.0f;

    result.e[1][0] = 0.0f;
    result.e[1][1] = 1.0f;
    result.e[1][2] = 0.0f;
    result.e[1][3] = 0.0f;

    result.e[2][0] = 0.0f;
    result.e[2][1] = 0.0f;
    result.e[2][2] = 1.0f;
    result.e[2][3] = 0.0f;

    result.e[3][0] = 0.0f;
    result.e[3][1] = 0.0f;
    result.e[3][2] = 0.0f;
    result.e[3][3] = 1.0f;

    return (result);
}

VM_API VM_INLINE m4x4 vm_m4x4_mul(m4x4 a, m4x4 b)
{
    m4x4 result;

    int i = 0;
    for (; i < 4; ++i)
    {
        result.e[i][0] = a.e[i][0] * b.e[0][0] + a.e[i][1] * b.e[1][0] + a.e[i][2] * b.e[2][0] + a.e[i][3] * b.e[3][0];
        result.e[i][1] = a.e[i][0] * b.e[0][1] + a.e[i][1] * b.e[1][1] + a.e[i][2] * b.e[2][1] + a.e[i][3] * b.e[3][1];
        result.e[i][2] = a.e[i][0] * b.e[0][2] + a.e[i][1] * b.e[1][2] + a.e[i][2] * b.e[2][2] + a.e[i][3] * b.e[3][2];
        result.e[i][3] = a.e[i][0] * b.e[0][3] + a.e[i][1] * b.e[1][3] + a.e[i][2] * b.e[2][3] + a.e[i][3] * b.e[3][3];
    }

    return (result);
}

VM_API VM_INLINE int vm_m4x4_equals(m4x4 a, m4x4 b)
{
    return (
        a.e[0][0] == b.e[0][0] &&
        a.e[0][1] == b.e[0][1] &&
        a.e[0][2] == b.e[0][2] &&
        a.e[0][3] == b.e[0][3] &&

        a.e[1][0] == b.e[1][0] &&
        a.e[1][1] == b.e[1][1] &&
        a.e[1][2] == b.e[1][2] &&
        a.e[1][3] == b.e[1][3] &&

        a.e[2][0] == b.e[2][0] &&
        a.e[2][1] == b.e[2][1] &&
        a.e[2][2] == b.e[2][2] &&
        a.e[2][3] == b.e[2][3] &&

        a.e[3][0] == b.e[3][0] &&
        a.e[3][1] == b.e[3][1] &&
        a.e[3][2] == b.e[3][2] &&
        a.e[3][3] == b.e[3][3]);
}

VM_API VM_INLINE m4x4 vm_m4x4_perspective(float fov, float aspectRatio, float zNear, float zFar)
{
    float f = 1.0f / vm_tanf(fov * 0.5f);
    float fn = 1.0f / (zNear - zFar);

    m4x4 result = vm_m4x4_zero();

    result.e[0][0] = f / aspectRatio;
    result.e[1][1] = f;
    result.e[2][2] = (zNear + zFar) * fn;
    result.e[2][3] = (2.0f * zNear * zFar) * fn;
    result.e[3][2] = -1.0f;

    return (result);
}

VM_API VM_INLINE m4x4 vm_m4x4_orthographic(float left, float right, float bottom, float top, float near, float far)
{
    float width = right - left;
    float height = top - bottom;
    float depth = far - near;

    m4x4 result = vm_m4x4_zero();

    result.e[0][0] = 2.0f / width;
    result.e[0][3] = -(right + left) / width;
    result.e[1][1] = 2.0f / height;
    result.e[1][3] = -(top + bottom) / height;
    result.e[2][2] = -2.0f / depth;
    result.e[2][3] = -(far + near) / depth;
    result.e[3][3] = 1.0f;

    return (result);
}

VM_API VM_INLINE m4x4 vm_m4x4_rotation(v3 forward, v3 up, v3 right)
{
    m4x4 result = vm_m4x4_zero();

    result.e[0][0] = right.x;
    result.e[0][1] = right.y;
    result.e[0][2] = right.z;
    result.e[0][3] = 0.0f;

    result.e[1][0] = up.x;
    result.e[1][1] = up.y;
    result.e[1][2] = up.z;
    result.e[1][3] = 0.0f;

    result.e[2][0] = forward.x;
    result.e[2][1] = forward.y;
    result.e[2][2] = forward.z;
    result.e[2][3] = 0.0f;

    result.e[3][0] = 0.0f;
    result.e[3][1] = 0.0f;
    result.e[3][2] = 0.0f;
    result.e[3][3] = 1.0f;

    return (result);
}

VM_API VM_INLINE m4x4 vm_m4x4_translate(m4x4 src, v3 b)
{
    m4x4 result = src;

    result.e[0][3] = b.x;
    result.e[1][3] = b.y;
    result.e[2][3] = b.z;

    return (result);
}

VM_API VM_INLINE m4x4 vm_m4x4_scale(m4x4 src, v3 factor)
{
    m4x4 result = src;

    result.e[0][0] = factor.x;
    result.e[1][1] = factor.y;
    result.e[2][2] = factor.z;
    result.e[3][3] = 1.0f;

    return (result);
}

VM_API VM_INLINE m4x4 vm_m4x4_scalef(m4x4 src, float factor)
{
    m4x4 result = src;

    result.e[0][0] = factor;
    result.e[1][1] = factor;
    result.e[2][2] = factor;
    result.e[3][3] = 1.0f;

    return (result);
}

VM_API VM_INLINE void vm_m4x4_swap(m4x4 src, float *dst)
{
    /* column-major-order */
    dst[0] = src.e[0][0];
    dst[1] = src.e[1][0];
    dst[2] = src.e[2][0];
    dst[3] = src.e[3][0];
    dst[4] = src.e[0][1];
    dst[5] = src.e[1][1];
    dst[6] = src.e[2][1];
    dst[7] = src.e[3][1];
    dst[8] = src.e[0][2];
    dst[9] = src.e[1][2];
    dst[10] = src.e[2][2];
    dst[11] = src.e[3][2];
    dst[12] = src.e[0][3]; /* transform x component */
    dst[13] = src.e[1][3]; /* transform z component */
    dst[14] = src.e[2][3]; /* transform y component */
    dst[15] = src.e[3][3];
}

VM_API VM_INLINE m4x4 vm_m4x4_rotate(m4x4 src, float angle, v3 axis)
{
    float c = vm_cosf(angle);

    v3 axisn = vm_v3_normalize(axis);
    v3 v = vm_v3_mulf(axisn, 1.0f - c);
    v3 vs = vm_v3_mulf(axisn, vm_sinf(angle));

    m4x4 rot = vm_m4x4_zero();

    v3 a;
    v3 b;
    v3 f;

    a = vm_v3_mulf(axisn, v.x);
    rot.e[0][0] = a.x;
    rot.e[1][0] = a.y;
    rot.e[2][0] = a.z;

    b = vm_v3_mulf(axisn, v.y);
    rot.e[0][1] = b.x;
    rot.e[1][1] = b.y;
    rot.e[2][1] = b.z;

    f = vm_v3_mulf(axisn, v.z);
    rot.e[0][2] = f.x;
    rot.e[1][2] = f.y;
    rot.e[2][2] = f.z;

    rot.e[0][0] += c;
    rot.e[0][1] -= vs.z;
    rot.e[0][2] += vs.y;
    rot.e[1][0] += vs.z;
    rot.e[1][1] += c;
    rot.e[1][2] -= vs.x;
    rot.e[2][0] -= vs.y;
    rot.e[2][1] += vs.x;
    rot.e[2][2] += c;

    rot.e[3][0] = 0.0f;
    rot.e[3][1] = 0.0f;
    rot.e[3][2] = 0.0f;
    rot.e[0][3] = 0.0f;
    rot.e[1][3] = 0.0f;
    rot.e[2][3] = 0.0f;
    rot.e[3][3] = 1.0f;

    return (vm_m4x4_mul(src, rot));
}

VM_API VM_INLINE m4x4 vm_m4x4_lookAt(v3 eye, v3 target, v3 up)
{
    v3 f = vm_v3_normalize(vm_v3_sub(target, eye));
    v3 s = vm_v3_normalize(vm_v3_cross(f, up));
    v3 u = vm_v3_cross(s, f);

    m4x4 result = vm_m4x4_zero();

    result.e[0][0] = s.x;
    result.e[0][1] = s.y;
    result.e[0][2] = s.z;
    result.e[0][3] = -vm_v3_dot(s, eye);
    result.e[1][0] = u.x;
    result.e[1][1] = u.y;
    result.e[1][2] = u.z;
    result.e[1][3] = -vm_v3_dot(u, eye);
    result.e[2][0] = -f.x;
    result.e[2][1] = -f.y;
    result.e[2][2] = -f.z;
    result.e[2][3] = vm_v3_dot(f, eye);
    result.e[3][3] = 1.0f;

    return (result);
}

/* #############################################################################
 * # Quaternion FUNCTIONS
 * ######################################################################## #####
 */
#define VM_QUAT_ELEMENT_COUNT 4

typedef v4 quat;

VM_API VM_INLINE quat vm_quat(float x, float y, float z, float w)
{
    quat result;

    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;

    return (result);
}

VM_API VM_INLINE float *vm_quat_data(quat *a)
{
    return ((float *)a);
}

VM_API VM_INLINE quat vm_quat_rotate(v3 axis, float angle)
{
    quat result;

    const float halfAngle = angle * 0.5f;
    const float sinHalfAngle = vm_sinf(halfAngle);
    const float cosHalfAngle = vm_cosf(halfAngle);

    result.x = axis.x * sinHalfAngle;
    result.y = axis.y * sinHalfAngle;
    result.z = axis.z * sinHalfAngle;
    result.w = cosHalfAngle;

    return (result);
}

VM_API VM_INLINE quat vm_quat_normalize(quat a)
{
    float scalar = vm_invsqrt((a.x * a.x) + (a.y * a.y) + (a.z * a.z) + (a.w * a.w));

    quat result;

    result.x = a.x * scalar;
    result.y = a.y * scalar;
    result.z = a.z * scalar;
    result.w = a.w * scalar;

    return (result);
}

VM_API VM_INLINE quat vm_quat_conjugate(quat a)
{
    quat result;

    result.x = -a.x;
    result.y = -a.y;
    result.z = -a.z;
    result.w = a.w;

    return (result);
}

VM_API VM_INLINE quat vm_quat_mul(quat a, quat b)
{
    quat result;

    result.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
    result.x = a.x * b.w + a.w * b.x + a.y * b.z - a.z * b.y;
    result.y = a.y * b.w + a.w * b.y + a.z * b.x - a.x * b.z;
    result.z = a.z * b.w + a.w * b.z + a.x * b.y - a.y * b.x;

    return (result);
}

VM_API VM_INLINE quat vm_quat_mulf(quat a, float b)
{
    quat result;

    result.x = a.x * b;
    result.y = a.y * b;
    result.z = a.z * b;
    result.w = a.w * b;

    return (result);
}

VM_API VM_INLINE quat vm_quat_mulv3(quat a, v3 b)
{
    quat result;

    result.w = -a.x * b.x - a.y * b.y - a.z * b.z;
    result.x = a.w * b.x + a.y * b.z - a.z * b.y;
    result.y = a.w * b.y + a.z * b.x - a.x * b.z;
    result.z = a.w * b.z + a.x * b.y - a.y * b.x;

    return (result);
}

VM_API VM_INLINE quat vm_quat_sub(quat a, quat b)
{
    quat result;

    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    result.w = a.w - b.w;

    return (result);
}

VM_API VM_INLINE quat vm_quat_add(quat a, quat b)
{
    quat result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    result.w = a.w + b.w;

    return (result);
}

VM_API VM_INLINE m4x4 vm_quat_to_rotation_matrix(quat a)
{
    v3 forward = vm_v3(
        2.0f * (a.x * a.z - a.w * a.y),
        2.0f * (a.y * a.z + a.w * a.x),
        1.0f - 2.0f * (a.x * a.x + a.y * a.y));

    v3 up = vm_v3(
        2.0f * (a.x * a.y + a.w * a.z),
        1.0f - 2.0f * (a.x * a.x + a.z * a.z),
        2.0f * (a.y * a.z - a.w * a.x));

    v3 right = vm_v3(
        1.0f - 2.0f * (a.y * a.y + a.z * a.z),
        2.0f * (a.x * a.y - a.w * a.z),
        2.0f * (a.x * a.z + a.w * a.y));

    return (vm_m4x4_rotation(forward, up, right));
}

VM_API VM_INLINE float vm_quat_dot(quat a, quat b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

VM_API VM_INLINE v3 vm_v3_rotate(v3 a, quat rotation)
{
    v3 result;

    quat conjugate = vm_quat_conjugate(rotation);
    quat w = vm_quat_mulv3(rotation, a);
    w = vm_quat_mul(w, conjugate);

    result.x = w.x;
    result.y = w.y;
    result.z = w.z;

    return (result);
}

VM_API VM_INLINE v3 vm_quat_forward(quat rotation)
{
    return (vm_v3_rotate(vm_v3(0.0f, 0.0f, 1.0f), rotation));
}

VM_API VM_INLINE v3 vm_quat_back(quat rotation)
{
    return (vm_v3_rotate(vm_v3(0.0f, 0.0f, -1.0f), rotation));
}

VM_API VM_INLINE v3 vm_quat_up(quat rotation)
{
    return (vm_v3_rotate(vm_v3(0.0f, 1.0f, 0.0f), rotation));
}

VM_API VM_INLINE v3 vm_quat_down(quat rotation)
{
    return (vm_v3_rotate(vm_v3(0.0f, -1.0f, 0.0f), rotation));
}

VM_API VM_INLINE v3 vm_quat_right(quat rotation)
{
    return (vm_v3_rotate(vm_v3(1.0f, 0.0f, 0.0f), rotation));
}

VM_API VM_INLINE v3 vm_quat_left(quat rotation)
{
    return (vm_v3_rotate(vm_v3(-1.0f, 0.0f, 0.0f), rotation));
}

#define VM_FRUSTUM_PLANE_SIZE 6

typedef struct frustum
{

    v4 leftPlane;
    v4 rightPlane;
    v4 bottomPlane;
    v4 topPlane;
    v4 nearPlane;
    v4 farPlane;

} frustum;

VM_API VM_INLINE v4 *vm_frustum_data(frustum *a)
{
    return ((v4 *)a);
}

VM_API VM_INLINE frustum vm_frustum_extract_planes(m4x4 projection_view)
{
    int i;

    frustum result;
    v4 *frustum_data;

    /* Left plane */
    result.leftPlane.x = projection_view.e[3][0] + projection_view.e[0][0];
    result.leftPlane.y = projection_view.e[3][1] + projection_view.e[0][1];
    result.leftPlane.z = projection_view.e[3][2] + projection_view.e[0][2];
    result.leftPlane.w = projection_view.e[3][3] + projection_view.e[0][3];

    /* Right plane */
    result.rightPlane.x = projection_view.e[3][0] - projection_view.e[0][0];
    result.rightPlane.y = projection_view.e[3][1] - projection_view.e[0][1];
    result.rightPlane.z = projection_view.e[3][2] - projection_view.e[0][2];
    result.rightPlane.w = projection_view.e[3][3] - projection_view.e[0][3];

    /* Bottom plane */
    result.bottomPlane.x = projection_view.e[3][0] + projection_view.e[1][0];
    result.bottomPlane.y = projection_view.e[3][1] + projection_view.e[1][1];
    result.bottomPlane.z = projection_view.e[3][2] + projection_view.e[1][2];
    result.bottomPlane.w = projection_view.e[3][3] + projection_view.e[1][3];

    /* Top plane */
    result.topPlane.x = projection_view.e[3][0] - projection_view.e[1][0];
    result.topPlane.y = projection_view.e[3][1] - projection_view.e[1][1];
    result.topPlane.z = projection_view.e[3][2] - projection_view.e[1][2];
    result.topPlane.w = projection_view.e[3][3] - projection_view.e[1][3];

    /* Near plane */
    result.nearPlane.x = projection_view.e[3][0] + projection_view.e[2][0];
    result.nearPlane.y = projection_view.e[3][1] + projection_view.e[2][1];
    result.nearPlane.z = projection_view.e[3][2] + projection_view.e[2][2];
    result.nearPlane.w = projection_view.e[3][3] + projection_view.e[2][3];

    /* Far plane */
    result.farPlane.x = projection_view.e[3][0] - projection_view.e[2][0];
    result.farPlane.y = projection_view.e[3][1] - projection_view.e[2][1];
    result.farPlane.z = projection_view.e[3][2] - projection_view.e[2][2];
    result.farPlane.w = projection_view.e[3][3] - projection_view.e[2][3];

    frustum_data = vm_frustum_data(&result);

    /* Normalize planes */
    for (i = 0; i < VM_FRUSTUM_PLANE_SIZE; ++i)
    {
        float scalar = vm_invsqrt((frustum_data[i].x * frustum_data[i].x) + (frustum_data[i].y * frustum_data[i].y) + (frustum_data[i].z * frustum_data[i].z));

        frustum_data[i].x = frustum_data[i].x * scalar;
        frustum_data[i].y = frustum_data[i].y * scalar;
        frustum_data[i].z = frustum_data[i].z * scalar;
        frustum_data[i].w = frustum_data[i].w * scalar;
    }

    return (result);
}

VM_API VM_INLINE int vm_frustum_is_point_in(frustum frustum, v3 point)
{
    int i;
    v4 *frustum_data = vm_frustum_data(&frustum);

    for (i = 0; i < VM_FRUSTUM_PLANE_SIZE; ++i)
    {
        v4 pointTarget = vm_v4(point.x, point.y, point.z, 1.0f);
        if (vm_v4_dot(frustum_data[i], pointTarget) < 0)
        {
            return (0); /* Point is outside */
        }
    }
    return (1); /* Point is inside */
}

VM_API VM_INLINE int vm_frustum_is_cube_in(frustum frustum, v3 center, v3 dimensions, float epsilon)
{
    v4 *frustum_data = vm_frustum_data(&frustum);

    /* Calculate the half extents of the object */
    v3 half_extents = vm_v3_addf(vm_v3_mulf(dimensions, 0.5f), epsilon);

    float subHalfX = center.x - half_extents.x;
    float addHalfX = center.x + half_extents.x;
    float subHalfY = center.y - half_extents.y;
    float addHalfY = center.y + half_extents.y;
    float subHalfZ = center.z - half_extents.z;
    float addHalfZ = center.z + half_extents.z;

    int i;

    /* Define the eight corners of the cube in local space */
    v3 corners[8];
    corners[0] = vm_v3(subHalfX, subHalfY, subHalfZ);
    corners[1] = vm_v3(addHalfX, subHalfY, subHalfZ);
    corners[2] = vm_v3(subHalfX, addHalfY, subHalfZ);
    corners[3] = vm_v3(addHalfX, addHalfY, subHalfZ);
    corners[4] = vm_v3(subHalfX, subHalfY, addHalfZ);
    corners[5] = vm_v3(addHalfX, subHalfY, addHalfZ);
    corners[6] = vm_v3(subHalfX, addHalfY, addHalfZ);
    corners[7] = vm_v3(addHalfX, addHalfY, addHalfZ);

    /* Check each plane of the frustum */
    for (i = 0; i < VM_FRUSTUM_PLANE_SIZE; ++i)
    {
        const v4 *plane = &frustum_data[i];

        /* Check if all corners of the object are on the outside of the plane */
        int outside = 1;
        int j;
        for (j = 0; j < 8; ++j)
        {
            /* Compute the distance from the corner to the plane */
            float distance = plane->x * corners[j].x + plane->y * corners[j].y + plane->z * corners[j].z + plane->w;

            /* If the distance is greater than or equal to zero, the corner is inside or on the plane */
            if (distance >= 0)
            {
                outside = 0; /* At least one corner is inside */
                break;
            }
        }

        /* If all corners are outside this plane, the object is outside the frustum */
        if (outside)
        {
            return (0);
        }
    }

    /* If we passed all planes, the object is inside or intersects the frustum */
    return (1);
}

typedef struct transformation
{
    v3 position;
    quat rotation;
    v3 scale;

    struct transformation *parent;

} transformation;

VM_API VM_INLINE transformation vm_tranformation_init(void)
{
    transformation result = {0};
    result.position = vm_v3_zero();
    result.rotation = vm_quat(0.0f, 0.0f, 0.0f, 1.0f);
    result.scale = vm_v3_one();
    return (result);
}

VM_API VM_INLINE m4x4 vm_transformation_matrix(transformation *t)
{
    m4x4 translation_matrix = vm_m4x4_translate(vm_m4x4_identity(), t->position);
    m4x4 rotation_matrix = vm_quat_to_rotation_matrix(t->rotation);
    m4x4 scale_matrix = vm_m4x4_identity();
    m4x4 parent_matrix;

    if (vm_v3_equals(t->scale, vm_v3_zero()))
    {
        t->scale = vm_v3_one();
    }

    scale_matrix.e[0][0] = t->scale.x;
    scale_matrix.e[1][1] = t->scale.y;
    scale_matrix.e[2][2] = t->scale.z;

    if (t->parent)
    {
        parent_matrix = vm_transformation_matrix(t->parent);
    }
    else
    {
        parent_matrix = vm_m4x4_identity();
    }

    return (vm_m4x4_mul(parent_matrix, vm_m4x4_mul(translation_matrix, vm_m4x4_mul(rotation_matrix, scale_matrix))));
}

VM_API VM_INLINE void vm_tranformation_rotate(transformation *t, v3 axis, float angle)
{
    t->rotation = vm_quat_rotate(axis, angle);
}

#endif /* VM_H */

/*
   ------------------------------------------------------------------------------
   This software is available under 2 licenses -- choose whichever you prefer.
   ------------------------------------------------------------------------------
   ALTERNATIVE A - MIT License
   Copyright (c) 2025 nickscha
   Permission is hereby granted, free of charge, to any person obtaining a copy of
   this software and associated documentation files (the "Software"), to deal in
   the Software without restriction, including without limitation the rights to
   use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
   of the Software, and to permit persons to whom the Software is furnished to do
   so, subject to the following conditions:
   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
   ------------------------------------------------------------------------------
   ALTERNATIVE B - Public Domain (www.unlicense.org)
   This is free and unencumbered software released into the public domain.
   Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
   software, either in source code form or as a compiled binary, for any purpose,
   commercial or non-commercial, and by any means.
   In jurisdictions that recognize copyright laws, the author or authors of this
   software dedicate any and all copyright interest in the software to the public
   domain. We make this dedication for the benefit of the public at large and to
   the detriment of our heirs and successors. We intend this dedication to be an
   overt act of relinquishment in perpetuity of all present and future rights to
   this software under copyright law.
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
   WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
   ------------------------------------------------------------------------------
*/
