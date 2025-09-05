/* vm.h - v0.2 - public domain data structures - nickscha 2025

A C89 standard compliant, single header, nostdlib (no C Standard Library) vector linear algebra implementation.

This Test class defines cases to verify that we don't break the excepted behaviours in the future upon changes.

LICENSE

  Placed in the public domain and also MIT licensed.
  See end of file for detailed license information.

*/
#define VM_USE_SSE
#include "../vm.h"

#include "../deps/test.h" /* Simple Testing framework */

void vm_test_math(void)
{
  assert(vm_radf(180) >= 3.141590f && vm_radf(180) <= 3.141593f);
  assert(vm_maxf(1.0f, 2.0f) == 2.0f);
  assert(vm_maxf(1.0f, -2.0f) == 1.0f);
  assert(vm_minf(1.0f, 2.0f) == 1.0f);
  assert(vm_minf(1.0f, -2.0f) == -2.0f);
  assert(vm_clampf(5.0f, 10.0f, 20.0f) == 10.0f);
  assert(vm_clampf(15.0f, 10.0f, 20.0f) == 15.0f);
  assert(vm_clampf(25.0f, 10.0f, 20.0f) == 20.0f);
}

/* Small tolerance for floating-point comparisons */
#define EPSILON 1e-4f

int vm_fequal(float a, float b)
{
  return vm_absf(a - b) < EPSILON;
}

void vm_test_absf(void)
{
  assert(vm_absf(vm_absf(-5.0f) - 5.0f) < EPSILON);   /* |-5| = 5 */
  assert(vm_absf(vm_absf(3.0f) - 3.0f) < EPSILON);    /* |3| = 3 */
  assert(vm_absf(vm_absf(0.0f) - 0.0f) < EPSILON);    /* |0| = 0 */
  assert(vm_absf(vm_absf(-1e-6f) - 1e-6f) < EPSILON); /* Tiny negative value */
  assert(vm_absf(vm_absf(1e6f) - 1e6f) < EPSILON);    /* Large positive value */
}

void vm_test_sinf(void)
{
  assert(vm_absf(vm_sinf(0.0f) - 0.0f) < EPSILON);
  assert(vm_absf(vm_sinf(VM_PI / 6) - 0.5f) < EPSILON);        /* sin(30°) = 0.5 */
  assert(vm_absf(vm_sinf(VM_PI / 4) - 0.70710678f) < EPSILON); /* sin(45°) ≈ sqrt(2)/2 */
  assert(vm_absf(vm_sinf(VM_PI / 3) - 0.8660254f) < EPSILON);  /* sin(60°) ≈ sqrt(3)/2 */
  assert(vm_absf(vm_sinf(VM_PI / 2) - 1.0f) < EPSILON);        /* sin(90°) = 1 */
  assert(vm_absf(vm_sinf(VM_PI) - 0.0f) < EPSILON);            /* sin(180°) = 0 */
  assert(vm_absf(vm_sinf(3 * VM_PI / 2) + 1.0f) < EPSILON);    /* sin(270°) = -1 */

  /* negative and multiples of π */
  assert(vm_absf(vm_sinf(-VM_PI / 2) + 1.0f) < EPSILON);     /* sin(-90°) = -1 */
  assert(vm_absf(vm_sinf(-VM_PI) - 0.0f) < EPSILON);         /* sin(-180°) = 0 */
  assert(vm_absf(vm_sinf(-3 * VM_PI / 2) - 1.0f) < EPSILON); /* sin(-270°) = 1 */
  assert(vm_absf(vm_sinf(2 * VM_PI) - 0.0f) < EPSILON);      /* sin(360°) = 0 */
  assert(vm_absf(vm_sinf(3 * VM_PI) + 0.0f) < EPSILON);      /* sin(540°) = 0 */

  assert(vm_absf(vm_sinf(2 * VM_PI + VM_PI / 4) - 0.70710678f) < EPSILON); /* sin(2π + 45°) = sin(45°) */
  assert(vm_absf(vm_sinf(1e-6f) - 1e-6f) < EPSILON);                       /* close to zero */
  assert(vm_absf(vm_sinf(1e6f) - vm_sinf(1e6f)) < EPSILON);                /* large angle (beyond typical cycles) */
}

void vm_test_cosf(void)
{
  assert(vm_absf(vm_cosf(0.0f) - 1.0f) < EPSILON);             /* cos(0°) = 1 */
  assert(vm_absf(vm_cosf(VM_PI / 6) - 0.8660254f) < EPSILON);  /* cos(30°) ≈ sqrt(3)/2 */
  assert(vm_absf(vm_cosf(VM_PI / 4) - 0.70710678f) < EPSILON); /* cos(45°) ≈ sqrt(2)/2 */
  assert(vm_absf(vm_cosf(VM_PI / 3) - 0.5f) < EPSILON);        /* cos(60°) = 0.5 */
  assert(vm_absf(vm_cosf(VM_PI / 2)) < EPSILON);               /* cos(90°) = 0 */
  assert(vm_absf(vm_cosf(VM_PI) + 1.0f) < EPSILON);            /* cos(180°) = -1 */
  assert(vm_absf(vm_cosf(3 * VM_PI / 2)) < EPSILON);           /* cos(270°) = 0 */
  assert(vm_absf(vm_cosf(-VM_PI / 2)) < EPSILON);
  assert(vm_absf(vm_cosf(-VM_PI) + 1.0f) < EPSILON);
  assert(vm_absf(vm_cosf(-3 * VM_PI / 2)) < EPSILON);
  assert(vm_absf(vm_cosf(2 * VM_PI) - 1.0f) < EPSILON);
}

void vm_test_tanf(void)
{
  assert(vm_absf(vm_tanf(0.0f) - 0.0f) < EPSILON);             /* tan(0°) = 0 */
  assert(vm_absf(vm_tanf(VM_PI / 6) - 0.57735027f) < EPSILON); /* tan(30°) ≈ 1/sqrt(3) */
  assert(vm_absf(vm_tanf(VM_PI / 4) - 1.0f) < EPSILON);        /* tan(45°) = 1 */
  assert(vm_absf(vm_tanf(VM_PI / 3) - 1.7320508f) < EPSILON);  /* tan(60°) ≈ sqrt(3) */
  assert(vm_absf(vm_tanf(VM_PI / 2)) > 1000.0f);
  assert(vm_absf(vm_tanf(3 * VM_PI / 4) + 1.0f) < EPSILON);
  assert(vm_absf(vm_tanf(VM_PI) - 0.0f) < EPSILON);
  assert(vm_absf(vm_tanf(3 * VM_PI / 2)) > 1000.0f);
  assert(vm_absf(vm_tanf(2 * VM_PI) - 0.0f) < EPSILON);
  assert(vm_absf(vm_tanf(11 * VM_PI / 6) + 0.57735027f) < EPSILON);
}

void vm_test_acosf(void)
{
  assert(vm_absf(vm_acosf(1.0f) - 0.0f) < 0.01f);
  assert(vm_absf(vm_acosf(0.0f) - 1.5707963f) < 0.01f);
  assert(vm_absf(vm_acosf(-1.0f) - 3.1415926f) < 0.01f);
}

void vm_test_powf(void)
{
  /* Test positive exponent */
  float base = 2.0f;
  float exp = 3;
  float expected = 8.0f; /* 2^3 = 8 */
  float actual = vm_powf(base, exp);
  assert(vm_absf(actual - expected) < 0.1f);

  /* Test exponent of zero */
  base = 2.0f;
  exp = 0;
  expected = 1.0f; /* 2^0 = 1 */
  actual = vm_powf(base, exp);
  assert(vm_absf(actual - expected) < 0.001f);

  /* Test negative exponent */
  base = 2.0f;
  exp = -3;
  expected = 0.125f; /* 2^(-3) = 1/8 = 0.125 */
  actual = vm_powf(base, exp);
  assert(vm_absf(actual - expected) < 0.001f);

  /* Test base 1, should always return 1 */
  base = 1.0f;
  exp = 1000;
  expected = 1.0f; /* 1^1000 = 1 */
  actual = vm_powf(base, exp);
  assert(vm_absf(actual - expected) < 0.001f);

  /* Test base 0 with positive exponent */
  base = 0.0f;
  exp = 5;
  expected = 0.0f; /* 0^5 = 0 */
  actual = vm_powf(base, exp);
  assert(vm_absf(actual - expected) < 0.001f);
}

void vm_test_smoothstep(void)
{
  float t = 0.5f;
  float expected = (3.0f * t * t) - (2.0f * t * t * t);
  float actual = vm_smoothstep(t);
  assert(vm_absf(actual - expected) < 0.001f);
}

void vm_test_smootherstep(void)
{
  float t = 0.5f;
  float expected = (10.0f * t * t * t) - (15.0f * t * t * t * t) + (6.0f * t * t * t * t * t);
  float actual = vm_smootherstep(t);
  assert(vm_absf(actual - expected) < 0.001f);
}

void vm_test_smoothstep_edge_cases(void)
{
  float t1 = 0.0f;
  float expected1 = 0.0f;
  float actual1 = vm_smoothstep(t1);

  float t2 = 1.0f;
  float expected2 = 1.0f;
  float actual2 = vm_smoothstep(t2);

  assert(vm_absf(actual1 - expected1) < 0.001f);
  assert(vm_absf(actual2 - expected2) < 0.001f);
}

void vm_test_smootherstep_edge_cases(void)
{
  float t1 = 0.0f;
  float expected1 = 0.0f;
  float actual1 = vm_smootherstep(t1);

  float t2 = 1.0f;
  float expected2 = 1.0f;
  float actual2 = vm_smootherstep(t2);

  assert(vm_absf(actual1 - expected1) < 0.001f);
  assert(vm_absf(actual2 - expected2) < 0.001f);
}

void vm_test_ease_in_bounce(void)
{
  float t = 0.5f;
  float expected = 1.0f - vm_ease_out_bounce(1.0f - t);
  float actual = vm_ease_in_bounce(t);
  assert(vm_absf(actual - expected) < 0.001f);
}

void vm_test_ease_out_bounce(void)
{
  float t = 0.5f;
  float expected;
  float actual;
  if (t < (1.0f / 2.75f))
  {
    expected = 7.5625f * t * t;
  }
  else if (t < (2.0f / 2.75f))
  {
    t -= 1.5f / 2.75f;
    expected = 7.5625f * t * t + 0.75f;
  }
  else if (t < (2.5f / 2.75f))
  {
    t -= 2.25f / 2.75f;
    expected = 7.5625f * t * t + 0.9375f;
  }
  else
  {
    t -= 2.625f / 2.75f;
    expected = 7.5625f * t * t + 0.984375f;
  }
  actual = vm_ease_out_bounce(0.5f);
  assert(vm_absf(actual - expected) < 0.001f);
}

void vm_test_ease_in_out_bounce(void)
{
  float t = 0.5f;
  float expected = 0.5f * vm_ease_out_bounce(0.0f) + 0.5f;
  float actual = vm_ease_in_out_bounce(t);
  assert(vm_absf(actual - expected) < 0.001f);
}

void vm_test_v2(void)
{
  v2 a = {1.0f, 1.0f};
  v2 b = vm_v2_one;
  v2 c = vm_v2f(3.0f);

  assert(vm_v2_equals(a, b));
  assert(vm_v2_equals(c, vm_v2(3.0f, 3.0f)));
  assert(vm_v2_data(&a)[0] == 1.0f);
  assert(vm_v2_data(&a)[1] == 1.0f);
  assert(vm_v2_add(a, b).x == 2.0f);
  assert(vm_v2_add(a, b).y == 2.0f);
  assert(vm_v2_length_manhatten(a, b, 1.0f) == 0.0f);
}

void vm_test_v3(void)
{
  v3 a = {1.0f, 1.0f, 1.0f};
  v3 b = vm_v3_one;
  v3 c = {2.0f, 2.0f, 2.0f};
  v3 d = vm_v3f(3.0f);

  assert(vm_v3_equals(a, b));
  assert(vm_v3_equals(d, vm_v3(3.0f, 3.0f, 3.0f)));
  assert(vm_v3_data(&a)[0] == 1.0f);
  assert(vm_v3_data(&a)[1] == 1.0f);
  assert(vm_v3_data(&a)[2] == 1.0f);
  assert(vm_v3_add(a, b).x == 2.0f);
  assert(vm_v3_add(a, b).y == 2.0f);
  assert(vm_v3_add(a, b).z == 2.0f);
  assert(vm_v3_sub(a, c).x == -1.0f);
  assert(vm_v3_sub(a, c).y == -1.0f);
  assert(vm_v3_sub(a, c).z == -1.0f);
  assert(vm_v3_cross(a, b).x == 0.0f);
  assert(vm_v3_cross(a, b).y == 0.0f);
  assert(vm_v3_cross(a, b).z == 0.0f);
  assert(vm_v3_dot(a, c) == 6.0f);
  assert(vm_v3_length_manhatten(a, c, 1.0f) == 3.0f);
  assert(vm_v3_length_manhatten(a, c, 0.5f) == 6.0f);
}

void vm_test_v3_cross_dot_normalize(void)
{
  v3 a = {1.0f, 0.0f, 0.0f};
  v3 b = {0.0f, 1.0f, 0.0f};
  v3 r = vm_v3_cross(a, b);
  float rf;
  float epsilon = 1e-2f;

  assert(r.x == 0.0f);
  assert(r.y == 0.0f);
  assert(r.z == 1.0f);

  a = vm_v3(1.0f, 0.0f, 0.0f);
  b = vm_v3(0.0f, -1.0f, 0.0f);
  r = vm_v3_cross(a, b);

  assert(r.x == 0.0f);
  assert(r.y == 0.0f);
  assert(r.z == -1.0f);

  a = vm_v3(1.0f, 2.0f, 3.0f);
  b = vm_v3(2.0f, 4.0f, 6.0f);
  r = vm_v3_cross(a, b);

  assert(r.x == 0.0f);
  assert(r.y == 0.0f);
  assert(r.z == 0.0f);

  a = vm_v3(2.0f, 3.0f, 4.0f);
  b = vm_v3(5.0f, 6.0f, 7.0f);
  r = vm_v3_cross(a, b);

  assert(r.x == -3.0f);
  assert(r.y == 6.0f);
  assert(r.z == -3.0f);

  a = vm_v3(1.0f, 2.0f, 3.0f);
  b = vm_v3(4.0f, -5.0f, 6.0f);
  rf = vm_v3_dot(a, b);
  assert(rf == 12.0f);

  a = vm_v3(1.0f, 1.0f, 1.0f);
  b = vm_v3(0.0f, 0.0f, 0.0f);
  rf = vm_v3_dot(a, b);
  assert(rf == 0.0f);

  a = vm_v3(1.0f, 0.0f, 0.0f);
  b = vm_v3(0.0f, 1.0f, 0.0f);
  rf = vm_v3_dot(a, b);
  assert(rf == 0.0f);

  a = vm_v3(1.0f, 2.0f, 3.0f);
  b = vm_v3(-1.0f, -2.0f, -3.0f);
  rf = vm_v3_dot(a, b);
  assert(rf == -14.0f);

  a = vm_v3(3.0f, 4.0f, 0.0f);
  r = vm_v3_normalize(a);
  assert(vm_absf(r.x - 0.6f) < epsilon);
  assert(vm_absf(r.y - 0.8f) < epsilon);
  assert(vm_absf(r.z - 0.0f) < epsilon);

  a = vm_v3(1.0f, 0.0f, 0.0f);
  r = vm_v3_normalize(a);
  assert(vm_absf(r.x - 1.0f) < epsilon);
  assert(vm_absf(r.y - 0.0f) < epsilon);
  assert(vm_absf(r.z - 0.0f) < epsilon);

  a = vm_v3(-3.0f, 4.0f, 0.0f);
  r = vm_v3_normalize(a);
  assert(vm_absf(r.x + 0.6f) < epsilon);
  assert(vm_absf(r.y - 0.8f) < epsilon);
  assert(vm_absf(r.z - 0.0f) < epsilon);

  a = vm_v3(0.0f, 0.0f, 0.0f);
  r = vm_v3_normalize(a);
  assert(vm_absf(r.x - 0.0f) < epsilon);
  assert(vm_absf(r.y - 0.0f) < epsilon);
  assert(vm_absf(r.z - 0.0f) < epsilon);

  a = vm_v3(10.0f, 0.0f, 0.0f);
  r = vm_v3_normalize(a);
  assert(vm_absf(r.x - 1.0f) < epsilon);
  assert(vm_absf(r.y - 0.0f) < epsilon);
  assert(vm_absf(r.z - 0.0f) < epsilon);
}

void vm_test_v3_reflect_project_angle(void)
{
  v3 incident = vm_v3(1.0f, -1.0f, 0.0f);
  v3 normal = vm_v3(0.0f, 1.0f, 0.0f);
  v3 reflected = vm_v3_reflect(incident, normal);
  v3 a = vm_v3(2.0f, 3.0f, 0.0f);
  v3 b = vm_v3(1.0f, 0.0f, 0.0f);
  v3 projected = vm_v3_project(a, b);
  v3 angle_a = vm_v3(1.0f, 0.0f, 0.0f);
  v3 angle_b = vm_v3(0.0f, 1.0f, 0.0f);
  float angle = vm_v3_angle(angle_a, angle_b);

  assert(vm_v3_equals(reflected, vm_v3(1.0f, 1.0f, 0.0f)));
  assert(projected.x == 2.0f);
  assert(projected.y == 0.0f);
  assert(projected.z == 0.0f);
  assert(vm_absf(angle - (3.14159265f / 2.0f)) < 0.004f); /* 90 degrees */
}

void vm_test_v3_distance(void)
{
  v3 a = vm_v3(1.0f, 0.0f, 0.0f);
  v3 b = vm_v3(0.0f, 1.0f, 0.0f);

  float d = vm_v3_distance(a, b);

  assert(vm_absf(d - 1.41421356f) < 0.001f); /* sqrt(2) */
}

void vm_test_v4(void)
{
  v4 a = {1.0f, 1.0f, 1.0f, 1.0f};
  v4 b = vm_v4_one;
  v4 c = vm_v4f(3.0f);

  assert(vm_v4_equals(a, b));
  assert(vm_v4_equals(c, vm_v4(3.0f, 3.0f, 3.0f, 3.0f)));
  assert(vm_v4_data(&a)[0] == 1.0f);
  assert(vm_v4_data(&a)[1] == 1.0f);
  assert(vm_v4_data(&a)[2] == 1.0f);
  assert(vm_v4_data(&a)[3] == 1.0f);
  assert(vm_v4_add(a, b).x == 2.0f);
  assert(vm_v4_add(a, b).y == 2.0f);
  assert(vm_v4_add(a, b).z == 2.0f);
  assert(vm_v4_add(a, b).w == 2.0f);
  assert(vm_v4_sub(a, b).x == 0.0f);
  assert(vm_v4_sub(a, b).y == 0.0f);
  assert(vm_v4_sub(a, b).z == 0.0f);
  assert(vm_v4_sub(a, b).w == 0.0f);
  assert(vm_v4_mul(a, b).x == 1.0f);
  assert(vm_v4_mul(a, b).y == 1.0f);
  assert(vm_v4_mul(a, b).z == 1.0f);
  assert(vm_v4_mul(a, b).w == 1.0f);
  assert(vm_v4_divf(a, 0.5f).x == 2.0f);
  assert(vm_v4_divf(a, 0.5f).y == 2.0f);
  assert(vm_v4_divf(a, 0.5f).z == 2.0f);
  assert(vm_v4_divf(a, 0.5f).w == 2.0f);
}

void vm_test_m4x4(void)
{
  m4x4 a = vm_m4x4_identity;
  m4x4 b = vm_m4x4_zero;

  assert(a.e[VM_M4X4_AT(0, 0)] == 1.0f);
  assert(a.e[VM_M4X4_AT(1, 1)] == 1.0f);
  assert(a.e[VM_M4X4_AT(2, 2)] == 1.0f);
  assert(a.e[VM_M4X4_AT(3, 3)] == 1.0f);
  assert(b.e[VM_M4X4_AT(0, 0)] == 0.0f);
  assert(b.e[VM_M4X4_AT(1, 1)] == 0.0f);
  assert(b.e[VM_M4X4_AT(2, 2)] == 0.0f);
  assert(b.e[VM_M4X4_AT(3, 3)] == 0.0f);
}

void vm_test_m4x4_perspective(void)
{
  /* Perspective Projection */
  m4x4 p1, p2, p3;
  m4x4 p = vm_m4x4_perspective(VM_PI / 4, 16.0f / 9.0f, 0.1f, 100.0f);
  float f = 1.0f / vm_tanf((VM_PI / 4) * 0.5f);
  float fn = 1.0f / (0.1f - 100.0f);

  assert(vm_fequal(p.e[VM_M4X4_AT(0, 0)], f / (16.0f / 9.0f)));
  assert(vm_fequal(p.e[VM_M4X4_AT(0, 1)], 0.0f));
  assert(vm_fequal(p.e[VM_M4X4_AT(0, 2)], 0.0f));
  assert(vm_fequal(p.e[VM_M4X4_AT(0, 3)], 0.0f));

  assert(vm_fequal(p.e[VM_M4X4_AT(1, 0)], 0.0f));
  assert(vm_fequal(p.e[VM_M4X4_AT(1, 1)], f));
  assert(vm_fequal(p.e[VM_M4X4_AT(1, 2)], 0.0f));
  assert(vm_fequal(p.e[VM_M4X4_AT(1, 3)], 0.0f));

  assert(vm_fequal(p.e[VM_M4X4_AT(2, 0)], 0.0f));
  assert(vm_fequal(p.e[VM_M4X4_AT(2, 1)], 0.0f));
  assert(vm_fequal(p.e[VM_M4X4_AT(2, 2)], (0.1f + 100.0f) * fn));
  assert(vm_fequal(p.e[VM_M4X4_AT(2, 3)], 2.0f * 0.1f * 100.0f * fn));

  assert(vm_fequal(p.e[VM_M4X4_AT(3, 0)], 0.0f));
  assert(vm_fequal(p.e[VM_M4X4_AT(3, 1)], 0.0f));
  assert(vm_fequal(p.e[VM_M4X4_AT(3, 2)], -1.0f));
  assert(vm_fequal(p.e[VM_M4X4_AT(3, 3)], 0.0f));

  /* 45-degree FOV, 16:9 aspect ratio, near=0.1, far=100 */
  p1 = vm_m4x4_perspective(VM_PI / 4, 16.0f / 9.0f, 0.1f, 100.0f);
  assert(vm_fequal(p1.e[VM_M4X4_AT(0, 0)], 1.3579952f));  /* 1 / (tan(45°/2) * (16/9)) */
  assert(vm_fequal(p1.e[VM_M4X4_AT(1, 1)], 2.4142137f));  /* 1 / tan(45°/2) */
  assert(vm_fequal(p1.e[VM_M4X4_AT(2, 2)], -1.002002f));  /* (far+near) / (near-far) */
  assert(vm_fequal(p1.e[VM_M4X4_AT(2, 3)], -0.2002002f)); /* (2*far*near) / (near-far) */
  assert(vm_fequal(p1.e[VM_M4X4_AT(3, 2)], -1.0f));

  /* 60-degree FOV, 4:3 aspect ratio, near=0.1, far=100 */
  p2 = vm_m4x4_perspective(VM_PI / 3, 4.0f / 3.0f, 0.1f, 100.0f);
  assert(vm_fequal(p2.e[VM_M4X4_AT(0, 0)], 1.2990381f)); /* 1 / (tan(60°/2) * (4/3)) */
  assert(vm_fequal(p2.e[VM_M4X4_AT(1, 1)], 1.7320508f)); /* 1 / tan(60°/2) */
  assert(vm_fequal(p2.e[VM_M4X4_AT(2, 2)], -1.002002f));
  assert(vm_fequal(p2.e[VM_M4X4_AT(2, 3)], -0.2002002f));
  assert(vm_fequal(p2.e[VM_M4X4_AT(3, 2)], -1.0f));

  /* 90-degree FOV, 1:1 aspect ratio, near=0.1, far=50 */
  p3 = vm_m4x4_perspective(VM_PI / 2, 1.0f, 0.1f, 50.0f);
  assert(vm_fequal(p3.e[VM_M4X4_AT(0, 0)], 1.0f)); /* 1 / tan(90°/2) */
  assert(vm_fequal(p3.e[VM_M4X4_AT(1, 1)], 1.0f));
  assert(vm_fequal(p3.e[VM_M4X4_AT(2, 2)], -1.004008f));  /* (far+near) / (near-far) */
  assert(vm_fequal(p3.e[VM_M4X4_AT(2, 3)], -0.2004008f)); /* (2*far*near) / (near-far) */
  assert(vm_fequal(p3.e[VM_M4X4_AT(3, 2)], -1.0f));
}

void vm_test_m4x4_rotation(void)
{
  /* Identity rotation (axes aligned with world space) */
  v3 forward = vm_v3(0.0f, 0.0f, 1.0f);
  v3 up = vm_v3(0.0f, 1.0f, 0.0f);
  v3 right = vm_v3(1.0f, 0.0f, 0.0f);
  m4x4 r = vm_m4x4_rotation(forward, up, right);
  assert(r.e[VM_M4X4_AT(0, 0)] == 1.0f);
  assert(r.e[VM_M4X4_AT(0, 1)] == 0.0f);
  assert(r.e[VM_M4X4_AT(0, 2)] == 0.0f);
  assert(r.e[VM_M4X4_AT(0, 3)] == 0.0f);
  assert(r.e[VM_M4X4_AT(1, 0)] == 0.0f);
  assert(r.e[VM_M4X4_AT(1, 1)] == 1.0f);
  assert(r.e[VM_M4X4_AT(1, 2)] == 0.0f);
  assert(r.e[VM_M4X4_AT(1, 3)] == 0.0f);
  assert(r.e[VM_M4X4_AT(2, 0)] == 0.0f);
  assert(r.e[VM_M4X4_AT(2, 1)] == 0.0f);
  assert(r.e[VM_M4X4_AT(2, 2)] == 1.0f);
  assert(r.e[VM_M4X4_AT(2, 3)] == 0.0f);
  assert(r.e[VM_M4X4_AT(3, 0)] == 0.0f);
  assert(r.e[VM_M4X4_AT(3, 1)] == 0.0f);
  assert(r.e[VM_M4X4_AT(3, 2)] == 0.0f);
  assert(r.e[VM_M4X4_AT(3, 3)] == 1.0f);

  /* 90-degree rotation around Y-axis (Z -> X, X -> -Z) */
  forward = vm_v3(1.0f, 0.0f, 0.0f);
  up = vm_v3(0.0f, 1.0f, 0.0f);
  right = vm_v3(0.0f, 0.0f, -1.0f);
  r = vm_m4x4_rotation(forward, up, right);
  assert(r.e[VM_M4X4_AT(0, 0)] == 0.0f);
  assert(r.e[VM_M4X4_AT(0, 1)] == 0.0f);
  assert(r.e[VM_M4X4_AT(0, 2)] == -1.0f);
  assert(r.e[VM_M4X4_AT(0, 3)] == 0.0f);
  assert(r.e[VM_M4X4_AT(1, 0)] == 0.0f);
  assert(r.e[VM_M4X4_AT(1, 1)] == 1.0f);
  assert(r.e[VM_M4X4_AT(1, 2)] == 0.0f);
  assert(r.e[VM_M4X4_AT(1, 3)] == 0.0f);
  assert(r.e[VM_M4X4_AT(2, 0)] == 1.0f);
  assert(r.e[VM_M4X4_AT(2, 1)] == 0.0f);
  assert(r.e[VM_M4X4_AT(2, 2)] == 0.0f);
  assert(r.e[VM_M4X4_AT(2, 3)] == 0.0f);
  assert(r.e[VM_M4X4_AT(3, 0)] == 0.0f);
  assert(r.e[VM_M4X4_AT(3, 1)] == 0.0f);
  assert(r.e[VM_M4X4_AT(3, 2)] == 0.0f);
  assert(r.e[VM_M4X4_AT(3, 3)] == 1.0f);

  /* 180-degree rotation around Y-axis (Z -> -Z, X -> -X) */
  forward = vm_v3(0.0f, 0.0f, -1.0f);
  up = vm_v3(0.0f, 1.0f, 0.0f);
  right = vm_v3(-1.0f, 0.0f, 0.0f);
  r = vm_m4x4_rotation(forward, up, right);
  assert(r.e[VM_M4X4_AT(0, 0)] == -1.0f);
  assert(r.e[VM_M4X4_AT(0, 1)] == 0.0f);
  assert(r.e[VM_M4X4_AT(0, 2)] == 0.0f);
  assert(r.e[VM_M4X4_AT(0, 3)] == 0.0f);
  assert(r.e[VM_M4X4_AT(1, 0)] == 0.0f);
  assert(r.e[VM_M4X4_AT(1, 1)] == 1.0f);
  assert(r.e[VM_M4X4_AT(1, 2)] == 0.0f);
  assert(r.e[VM_M4X4_AT(1, 3)] == 0.0f);
  assert(r.e[VM_M4X4_AT(2, 0)] == 0.0f);
  assert(r.e[VM_M4X4_AT(2, 1)] == 0.0f);
  assert(r.e[VM_M4X4_AT(2, 2)] == -1.0f);
  assert(r.e[VM_M4X4_AT(2, 3)] == 0.0f);
  assert(r.e[VM_M4X4_AT(3, 0)] == 0.0f);
  assert(r.e[VM_M4X4_AT(3, 1)] == 0.0f);
  assert(r.e[VM_M4X4_AT(3, 2)] == 0.0f);
  assert(r.e[VM_M4X4_AT(3, 3)] == 1.0f);
}

void vm_test_m4x4_lookAt(void)
{
  v3 eye = vm_v3(0.0f, 0.0f, 5.0f);
  v3 target = vm_v3(0.0f, 0.0f, 0.0f);
  v3 up = vm_v3(0.0f, 1.0f, 0.0f);

  float tolerance = 1e-9f;

  m4x4 m = vm_m4x4_lookAt(eye, target, up);

  assert(vm_absf(m.e[VM_M4X4_AT(0, 0)]) - 1.0f <= tolerance);
  assert(vm_absf(m.e[VM_M4X4_AT(0, 1)]) - 0.0f <= tolerance);
  assert(vm_absf(m.e[VM_M4X4_AT(0, 2)]) - 0.0f <= tolerance);
  assert(vm_absf(m.e[VM_M4X4_AT(0, 3)]) - 0.0f <= tolerance);

  assert(vm_absf(m.e[VM_M4X4_AT(1, 0)]) - 0.0f <= tolerance);
  assert(vm_absf(m.e[VM_M4X4_AT(1, 1)]) - 1.0f <= tolerance);
  assert(vm_absf(m.e[VM_M4X4_AT(1, 2)]) - 0.0f <= tolerance);
  assert(vm_absf(m.e[VM_M4X4_AT(1, 3)]) - 0.0f <= tolerance);

  assert(vm_absf(m.e[VM_M4X4_AT(2, 0)]) - 0.0f <= tolerance);
  assert(vm_absf(m.e[VM_M4X4_AT(2, 1)]) - 0.0f <= tolerance);
  assert(vm_absf(m.e[VM_M4X4_AT(2, 2)]) - 1.0f <= tolerance);
  assert(vm_absf(m.e[VM_M4X4_AT(2, 3)]) - 5.0f <= tolerance);

  assert(vm_absf(m.e[VM_M4X4_AT(3, 0)]) - 0.0f <= tolerance);
  assert(vm_absf(m.e[VM_M4X4_AT(3, 1)]) - 0.0f <= tolerance);
  assert(vm_absf(m.e[VM_M4X4_AT(3, 2)]) - 0.0f <= tolerance);
  assert(vm_absf(m.e[VM_M4X4_AT(3, 3)]) - 1.0f <= tolerance);
}

void vm_test_m4x4_inverse(void)
{
  /* 90-degree Y rotation matrix (Z -> X, X -> -Z) */
  v3 forward = vm_v3(1.0f, 0.0f, 0.0f);
  v3 up = vm_v3(0.0f, 1.0f, 0.0f);
  v3 right = vm_v3(0.0f, 0.0f, -1.0f);
  m4x4 m = vm_m4x4_rotation(forward, up, right);

  m4x4 inv = vm_m4x4_inverse(m);
  m4x4 identity = vm_m4x4_mul(m, inv);

  int row;
  int col;

  /* Assert the result is approximately identity */
  for (row = 0; row < 4; ++row)
  {
    for (col = 0; col < 4; ++col)
    {
      float expected = (row == col) ? 1.0f : 0.0f;
      float actual = identity.e[VM_M4X4_AT(row, col)];
      assert(vm_absf(actual - expected) < 0.0001f);
    }
  }
}

void vm_test_quat(void)
{
  quat a = {1.0f, 1.0f, 1.0f, 1.0f};
  quat b = vm_quatf(3.0f);

  assert(a.x == 1.0f);
  assert(a.y == 1.0f);
  assert(a.z == 1.0f);
  assert(a.w == 1.0f);
  assert(vm_quat_equals(b, vm_quat(3.0f, 3.0f, 3.0f, 3.0f)));
  assert(vm_quat_data(&a)[0] == 1.0f);
  assert(vm_quat_data(&a)[1] == 1.0f);
  assert(vm_quat_data(&a)[2] == 1.0f);
  assert(vm_quat_data(&a)[3] == 1.0f);
}

void vm_test_frustum(void)
{
  int width = 800;
  int height = 600;

  v3 look_at_pos = vm_v3_zero;                /* Where should the camera look at */
  v3 up = vm_v3(0.0f, 1.0f, 0.0f);            /* World/Camera up */
  v3 cam_position = vm_v3(0.0f, 0.0f, 13.0f); /* Camera set a little bit back */
  float cam_fov = 90.0f;

  m4x4 projection = vm_m4x4_perspective(vm_radf(cam_fov), (float)width / (float)height, 0.1f, 1000.0f);
  m4x4 view = vm_m4x4_lookAt(cam_position, look_at_pos, up);
  m4x4 projection_view = vm_m4x4_mul(projection, view);

  /*
    Frustum Culling Example
  */
  frustum frustum_planes = vm_frustum_extract_planes(projection_view);

  v3 cube1_position = vm_v3_zero;
  v3 cube1_dimensions = vm_v3_one; /* No Scaling */

  v3 cube2_position = vm_v3(100.0f, 0.0f, 0.0f); /* Cube is set far away to the left */
  v3 cube2_dimensions = vm_v3_one;               /* No Scaling */

  float sphere_radius = 10.0f;
  v3 sphere1_position = vm_v3_zero;
  v3 sphere2_position = vm_v3(100.0f, 0.0f, 0.0f);

  /* The cube is rendered inside the camera frustum */
  assert(vm_frustum_is_cube_in(frustum_planes, cube1_position, cube1_dimensions, 0.15f));

  /* The cube is outside of camer frustum ! */
  assert(!vm_frustum_is_cube_in(frustum_planes, cube2_position, cube2_dimensions, 0.15f));

  assert(vm_frustum_is_sphere_in(frustum_planes, sphere1_position, sphere_radius));
  assert(!vm_frustum_is_sphere_in(frustum_planes, sphere2_position, sphere_radius));

  /* Sphere center is technically outside, but the radius is so big, that it reaches 0.0f location so inside the frustum*/
  assert(vm_frustum_is_sphere_in(frustum_planes, sphere2_position, 100.0f));
}

int main(void)
{

  vm_test_math();
  vm_test_absf();
  vm_test_sinf();
  vm_test_cosf();
  vm_test_tanf();
  vm_test_acosf();
  vm_test_powf();
  vm_test_smoothstep();
  vm_test_smootherstep();
  vm_test_smoothstep_edge_cases();
  vm_test_smootherstep_edge_cases();
  vm_test_ease_in_bounce();
  vm_test_ease_out_bounce();
  vm_test_ease_in_out_bounce();
  vm_test_v2();
  vm_test_v3();
  vm_test_v3_cross_dot_normalize();
  vm_test_v3_reflect_project_angle();
  vm_test_v3_distance();
  vm_test_v4();
  vm_test_m4x4();
  vm_test_m4x4_perspective();
  vm_test_m4x4_rotation();
  vm_test_m4x4_lookAt();
  vm_test_m4x4_inverse();
  vm_test_quat();
  vm_test_frustum();

  return 0;
}

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
