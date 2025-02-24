# vm
A C89 standard compliant, single header, nostdlib (no C Standard Library) vector linear algebra implementation.

For more information please look at the "vm.h" file or take a look at the "examples" or "tests" folder.

## Quick Start

Download or clone vm.h and include it in your project.

```C
#include "vm.h"

int main() {

    float dstProjection[VM_M4X4_ELEMENT_COUNT] = {0};
    float dstView[VM_M4X4_ELEMENT_COUNT] = {0};

    int width = 800;
    int height = 600;

    v3 look_at_pos = vm_v3_zero();              /* Where should the camera look at */
    v3 up = vm_v3(0.0f, 1.0f, 0.0f);            /* World/Camera up */
    v3 cam_position = vm_v3(0.0f, 0.0f, 13.0f); /* Camera set a little bit back */
    float cam_fov = 90.0f;

    m4x4 projection = vm_m4x4_perspective(vm_rad(cam_fov), (float)width / (float)height, 0.1f, 1000.0f);
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

    /*
      OpenGl need to know
    */
    /* IMPORTANT: Swap to column major order for OpenGl glUniformMatrix4fv */
    vm_m4x4_swap(projection, dstProjection);

    /* IMPORTANT: Swap to column major order for OpenGl glUniformMatrix4fv */
    vm_m4x4_swap(view, dstView);

    return 0;
}
```

## Run Example: nostdlib, freestsanding

In this repo you will find the "examples/vm_win32_nostdlib.c" with the corresponding "build.bat" file which
creates an executable only linked to "kernel32" and is not using the C standard library and executes the program afterwards.
