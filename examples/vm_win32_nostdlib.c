/* vm.h - v0.1 - public domain data structures - nickscha 2025

A C89 standard compliant, single header, nostdlib (no C Standard Library) vector linear algebra implementation.

This example demonstrates a win32 program using vm.h without using and linking to the C standard library.

The resulting executable from the build file has only linking to kernel32 (for VirtualAlloc) and nothing else :)

It also does not uses "windows.h" but defines the windows functions prototypes directly since windows.h is
massivly bloated and reduces the build time significantly.

This example tested with clang and gcc.

Please read build.bat file to see the compiler flags and their description.

LICENSE

  Placed in the public domain and also MIT licensed.
  See end of file for detailed license information.

*/
#ifdef _MSC_VER
#pragma function(memset)
#endif
void *memset(void *dest, int c, unsigned int count)
{
    char *bytes = (char *)dest;
    while (count--)
    {
        *bytes++ = (char)c;
    }
    return dest;
}

#ifdef _MSC_VER
#pragma function(memcpy)
#endif
void *memcpy(void *dest, const void *src, unsigned int count)
{
    char *dest8 = (char *)dest;
    const char *src8 = (const char *)src;
    while (count--)
    {
        *dest8++ = *src8++;
    }
    return dest;
}

/*
Avoid in freestanding build: lld-link: error: undefined symbol: _fltused
*/
int _fltused = 0;

#define assert(expression)      \
    if (!(expression))          \
    {                           \
        *(volatile int *)0 = 0; \
    }

/*
    Simple win32 print function without using the "windows.h" include since it slows
    down build time
*/
#include "win32_print.h"

#include "../vm.h"

#ifdef __clang__
#elif __GNUC__
__attribute((externally_visible))
#endif
#ifdef __i686__
__attribute((force_align_arg_pointer))
#endif
int
mainCRTStartup(void)
{
    int width = 800;
    int height = 600;

    v3 look_at_pos = vm_v3_zero();              /* Where should the camera look at */
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

    v3 cube1_position = vm_v3_zero();
    v3 cube1_dimensions = vm_v3_one(); /* No Scaling */

    v3 cube2_position = vm_v3(100.0f, 0.0f, 0.0f); /* Cube is set far away to the left */
    v3 cube2_dimensions = vm_v3_one();             /* No Scaling */

    /* The cube is rendered inside the camera frustum */
    assert(vm_frustum_is_cube_in(frustum_planes, cube1_position, cube1_dimensions, 0.15f));

    /* The cube is outside of camer frustum ! */
    assert(!vm_frustum_is_cube_in(frustum_planes, cube2_position, cube2_dimensions, 0.15f));

    win32_printf("[win32][nostdlib] vm example finished\n");

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
