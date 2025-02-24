/* vm.h - v0.1 - public domain data structures - nickscha 2025

A C89 standard compliant, single header, nostdlib (no C Standard Library) vector linear algebra implementation.

This Test class defines cases to verify that we don't break the excepted behaviours in the future upon changes.

LICENSE

  Placed in the public domain and also MIT licensed.
  See end of file for detailed license information.

*/
#include "../vm.h"

#include "test.h" /* Simple Testing framework */

void vm_test_math(void)
{
  assert(vm_rad(180) >= 3.141590f && vm_rad(180) <= 3.141593f);
  assert(vm_max(1.0f, 2.0f) == 2.0f);
  assert(vm_max(1.0f, -2.0f) == 1.0f);
  assert(vm_min(1.0f, 2.0f) == 1.0f);
  assert(vm_min(1.0f, -2.0f) == -2.0f);
  assert(vm_clamp(5.0f, 10.0f, 20.0f) == 10.0f);
  assert(vm_clamp(15.0f, 10.0f, 20.0f) == 15.0f);
  assert(vm_clamp(25.0f, 10.0f, 20.0f) == 20.0f);
}

void vm_test_v2(void)
{
  v2 a = {1.0f, 1.0f};
  v2 b = vm_v2_one();

  assert(vm_v2_equals(a, b));
  assert(vm_v2_data(&a)[0] == 1.0f);
  assert(vm_v2_data(&a)[1] == 1.0f);
  assert(vm_v2_add(a, b).x == 2.0f);
  assert(vm_v2_add(a, b).y == 2.0f);
}

void vm_test_v3(void)
{
  v3 a = {1.0f, 1.0f, 1.0f};
  v3 b = vm_v3_one();

  assert(vm_v3_equals(a, b));
  assert(vm_v3_data(&a)[0] == 1.0f);
  assert(vm_v3_data(&a)[1] == 1.0f);
  assert(vm_v3_data(&a)[2] == 1.0f);
  assert(vm_v3_add(a, b).x == 2.0f);
  assert(vm_v3_add(a, b).y == 2.0f);
  assert(vm_v3_add(a, b).z == 2.0f);
}

void vm_test_v4(void)
{
  v4 a = {1.0f, 1.0f, 1.0f, 1.0f};
  v4 b = vm_v4_one();

  assert(vm_v4_equals(a, b));
  assert(vm_v4_data(&a)[0] == 1.0f);
  assert(vm_v4_data(&a)[1] == 1.0f);
  assert(vm_v4_data(&a)[2] == 1.0f);
  assert(vm_v4_data(&a)[3] == 1.0f);
  assert(vm_v4_add(a, b).x == 2.0f);
  assert(vm_v4_add(a, b).y == 2.0f);
  assert(vm_v4_add(a, b).z == 2.0f);
  assert(vm_v4_add(a, b).w == 2.0f);
}

void vm_test_m4x4(void)
{
  m4x4 a = vm_m4x4_identity();

  assert(a.e[0][0] == 1.0f);
  assert(a.e[1][1] == 1.0f);
  assert(a.e[2][2] == 1.0f);
  assert(a.e[3][3] == 1.0f);
}

void vm_test_quat(void)
{
  quat a = {1.0f, 1.0f, 1.0f, 1.0f};

  assert(a.x == 1.0f);
  assert(a.y == 1.0f);
  assert(a.z == 1.0f);
  assert(a.w == 1.0f);
  assert(vm_quat_data(&a)[0] == 1.0f);
  assert(vm_quat_data(&a)[1] == 1.0f);
  assert(vm_quat_data(&a)[2] == 1.0f);
  assert(vm_quat_data(&a)[3] == 1.0f);
}

void vm_test_frustum(void)
{
}

void vm_test_transformation(void)
{
}

int main(void)
{

  vm_test_math();
  vm_test_v2();
  vm_test_v3();
  vm_test_v4();
  vm_test_m4x4();
  vm_test_quat();
  vm_test_frustum();
  vm_test_transformation();

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
