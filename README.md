# vm
A C89 standard compliant, single header, nostdlib (no C Standard Library) vector linear algebra implementation.

For more information please look at the "vm.h" file or take a look at the "examples" or "tests" folder.

## Quick Start

Download or clone vm.h and include it in your project.

```C
#include "vm.h"

int main() {

    int width = 800;
    int height = 600;

    v3 look_at_pos = vm_v3_zero;                /* Where should the camera look at */
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

    v3 cube1_position = vm_v3_zero;
    v3 cube1_dimensions = vm_v3_one; /* No Scaling */

    v3 cube2_position = vm_v3(100.0f, 0.0f, 0.0f); /* Cube is set far away to the left */
    v3 cube2_dimensions = vm_v3_one;               /* No Scaling */

    /* The cube is rendered inside the camera frustum */
    assert(vm_frustum_is_cube_in(frustum_planes, cube1_position, cube1_dimensions, 0.15f));

    /* The cube is outside of camer frustum ! */
    assert(!vm_frustum_is_cube_in(frustum_planes, cube2_position, cube2_dimensions, 0.15f));

    return 0;
}
```

### Enable SIMD (SSE)

By default explicit SIMD (SSE) is disabled.
If your environment supports SSE you can either use the compile flag `-DVM_USE_SSE` or define it before including the header.

```C
#define VM_USE_SSE
#include "vm.h"
```

### Switch Row/Column major layout
By default the m4x4 (Matrix 4x4) uses a **column major** order for storing data (used by OpenGL).
If you want to change to a row major order you can use the following define before including the header.

```C
#define VM_M4X4_ROW_MAJOR_ORDER
#include "vm.h"
```

### Switch Coordinate orientation

By default a right hand layout (like OpenGL uses) is used.
You can switch the layout to lefthand like this:

```C
#define VM_LEFT_HAND_LAYOUT
#include "vm.h"
```

## Run Example: nostdlib, freestsanding

In this repo you will find the "examples/vm_win32_nostdlib.c" with the corresponding "build.bat" file which
creates an executable only linked to "kernel32" and is not using the C standard library and executes the program afterwards.

## "nostdlib" Motivation & Purpose

nostdlib is a lightweight, minimalistic approach to C development that removes dependencies on the standard library. The motivation behind this project is to provide developers with greater control over their code by eliminating unnecessary overhead, reducing binary size, and enabling deployment in resource-constrained environments.

Many modern development environments rely heavily on the standard library, which, while convenient, introduces unnecessary bloat, security risks, and unpredictable dependencies. nostdlib aims to give developers fine-grained control over memory management, execution flow, and system calls by working directly with the underlying platform.

### Benefits

#### Minimal overhead
By removing the standard library, nostdlib significantly reduces runtime overhead, allowing for faster execution and smaller binary sizes.

#### Increased security
Standard libraries often include unnecessary functions that increase the attack surface of an application. nostdlib mitigates security risks by removing unused and potentially vulnerable components.

#### Reduced binary size
Without linking to the standard library, binaries are smaller, making them ideal for embedded systems, bootloaders, and operating systems where storage is limited.

#### Enhanced performance
Direct control over system calls and memory management leads to performance gains by eliminating abstraction layers imposed by standard libraries.

#### Better portability
By relying only on fundamental system interfaces, nostdlib allows for easier porting across different platforms without worrying about standard library availability.
