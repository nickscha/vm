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

## Functions Documentation

This document provides an overview of the C89 functions included in the source code. The functions are grouped by their purpose, such as random number generation, math operations, and vector operations.

### Random Number Generation Functions

### `unsigned int vm_randi(void)`
Generates a random unsigned integer using a Linear Congruential Generator (LCG).

### `float vm_randf(void)`
Generates a random float between 0.0 and 1.0.

### `float vm_randf_range(float min, float max)`
Generates a random float within a specified range `[min, max]`.

### Mathematical Functions

### `float vm_rad(float degree)`
Converts an angle from degrees to radians.

### `float vm_deg(float radians)`
Converts an angle from radians to degrees.

### `float vm_max(float a, float b)`
Returns the maximum of two float values.

### `int vm_maxi(int a, int b)`
Returns the maximum of two integer values.

### `float vm_min(float a, float b)`
Returns the minimum of two float values.

### `float vm_clamp(float value, float min, float max)`
Clamps a value between a specified `min` and `max`.

### `float vm_invsqrt(float number)`
Computes the inverse square root of a given number using the fast inverse square root method.

### `float vm_power(float base, int exp)`
Calculates the power of a base raised to an exponent.

### `float vm_fmodf(float x, float y)`
Computes the floating-point remainder of `x` divided by `y`.

### `float vm_cosf(float x)`
Approximates the cosine of an angle `x` (in radians) using a truncated Taylor series.

### `float vm_sinf(float x)`
Approximates the sine of an angle `x` (in radians) using a truncated Taylor series.

### `float vm_tanf(float x)`
Calculates the tangent of an angle `x` (in radians) by dividing the sine by the cosine of `x`.

### `float vm_absf(float x)`
Returns the absolute value of a float `x`.

### Vector 2D Functions

### `v2 vm_v2(float x, float y)`
Creates a 2D vector with the given `x` and `y` components.

### `float *vm_v2_data(v2 *a)`
Returns a pointer to the data array of a 2D vector `a`.

### `v2 vm_v2_zero(void)`
Returns a 2D zero vector `(0.0f, 0.0f)`.

### `v2 vm_v2_one(void)`
Returns a 2D vector `(1.0f, 1.0f)`.

### `int vm_v2_equals(v2 a, v2 b)`
Checks if two 2D vectors `a` and `b` are equal.

### `v2 vm_v2_add(v2 a, v2 b)`
Adds two 2D vectors `a` and `b`.

### `v2 vm_v2_addf(v2 a, float b)`
Adds a scalar `b` to each component of the 2D vector `a`.

### `v2 vm_v2_sub(v2 a, v2 b)`
Subtracts vector `b` from vector `a`.

### `v2 vm_v2_subf(v2 a, float b)`
Subtracts scalar `b` from each component of the 2D vector `a`.

### `v2 vm_v2_mul(v2 a, v2 b)`
Multiplies two 2D vectors `a` and `b` component-wise.

### `v2 vm_v2_mulf(v2 a, float b)`
Multiplies each component of the 2D vector `a` by the scalar `b`.

### `v2 vm_v2_div(v2 a, v2 b)`
Divides each component of vector `a` by the corresponding component of vector `b`.

### `v2 vm_v2_divf(v2 a, float b)`
Divides each component of the 2D vector `a` by the scalar `b`.

### `float vm_v2_length_manhatten(v2 start, v2 end, float unit)`
Computes the Manhattan length between two 2D vectors `start` and `end`, with an optional `unit` factor.

### Vector 3D Functions

### `v3 vm_v3(float x, float y, float z)`
Creates a 3D vector with the given `x`, `y`, and `z` components.

### `float *vm_v3_data(v3 *a)`
Returns a pointer to the data array of a 3D vector `a`.

### `v3 vm_v3_zero(void)`
Returns a 3D zero vector `(0.0f, 0.0f, 0.0f)`.

### `v3 vm_v3_one(void)`
Returns a 3D vector `(1.0f, 1.0f, 1.0f)`.

### `int vm_v3_equals(v3 a, v3 b)`
Checks if two 3D vectors `a` and `b` are equal.

### `v3 vm_v3_add(v3 a, v3 b)`
Adds two 3D vectors `a` and `b`.

### `v3 vm_v3_addf(v3 a, float b)`
Adds a scalar `b` to each component of the 3D vector `a`.

### `v3 vm_v3_sub(v3 a, v3 b)`
Subtracts vector `b` from vector `a`.

### `v3 vm_v3_subf(v3 a, float b)`
Subtracts scalar `b` from each component of the 3D vector `a`.

### `v3 vm_v3_mul(v3 a, v3 b)`
Multiplies two 3D vectors `a` and `b` component-wise.

### `v3 vm_v3_mulf(v3 a, float b)`
Multiplies each component of the 3D vector `a` by the scalar `b`.

### `v3 vm_v3_div(v3 a, v3 b)`
Divides each component of vector `a` by the corresponding component of vector `b`.

### `v3 vm_v3_divf(v3 a, float b)`
Divides each component of the 3D vector `a` by the scalar `b`.

### `v3 vm_v3_cross(v3 a, v3 b)`
Computes the cross product of two 3D vectors `a` and `b`.

### `float vm_v3_dot(v3 a, v3 b)`
Computes the dot product of two 3D vectors `a` and `b`.

### `v3 vm_v3_normalize(v3 a)`
Normalizes a 3D vector `a` to unit length.

### `v3 vm_v3_lerp(v3 a, v3 b, float t)`
Performs linear interpolation between two 3D vectors `a` and `b` by factor `t`.

### `float vm_v3_length_manhatten(v3 start, v3 end, float unit)`
Computes the Manhattan length between two 3D vectors `start` and `end`, with an optional `unit` factor.

### Vector 4D Functions

### `v4 vm_v4(float x, float y, float z, float w)`
Creates a 4D vector with the given `x`, `y`, `z`, and `w` components.

### `float *vm_v4_data(v4 *a)`
Returns a pointer to the data array of a 4D vector `a`.

### `v4 vm_v4_zero(void)`
Returns a 4D zero vector `(0.0f, 0.0f, 0.0f, 0.0f)`.

### `v4 vm_v4_one(void)`
Returns a 4D vector `(1.0f, 1.0f, 1.0f, 1.0f)`.

### `int vm_v4_equals(v4 a, v4 b)`
Checks if two 4D vectors `a` and `b` are equal.

### `v4 vm_v4_add(v4 a, v4 b)`
Adds two 4D vectors `a` and `b`.

### `v4 vm_v4_addf(v4 a, float b)`
Adds a scalar `b` to each component of the 4D vector `a`.

### `v4 vm_v4_sub(v4 a, v4 b)`
Subtracts vector `b` from vector `a`.

### `v4 vm_v4_subf(v4 a, float b)`
Subtracts scalar `b` from each component of the 4D vector `a`.

### `v4 vm_v4_mul(v4 a, v4 b)`
Multiplies two 4D vectors `a` and `b` component-wise.

### `v4 vm_v4_mulf(v4 a, float b)`
Multiplies each component of the 4D vector `a` by the scalar `b`.

### `v4 vm_v4_div(v4 a, v4 b)`
Divides each component of vector `a` by the corresponding component of vector `b`.

### `v4 vm_v4_divf(v4 a, float b)`
Divides each component of the 4D vector `a` by the scalar `b`.

### `float vm_v4_dot(v4 v1, v4 v2)`
Computes the dot product of two 4D vectors `v1` and `v2`.

### Matrix 4x4 Functions

### `m4x4 vm_m4x4_zero(void)`
Returns a 4x4 zero matrix.

### `m4x4 vm_m4x4_identity(void)`
Returns a 4x4 identity matrix.

### `m4x4 vm_m4x4_translate(v3 trans)`
Generates a translation matrix for a 3D vector `trans`.

### `m4x4 vm_m4x4_scale(v3 scale)`
Generates a scaling matrix for a 3D vector `scale`.

### `m4x4 vm_m4x4_rotate_x(float radians)`
Generates a rotation matrix around the x-axis for a given angle in radians.

### `m4x4 vm_m4x4_rotate_y(float radians)`
Generates a rotation matrix around the y-axis for a given angle in radians.

### `m4x4 vm_m4x4_rotate_z(float radians)`
Generates a rotation matrix around the z-axis for a given angle in radians.

### `m4x4 vm_m4x4_mul(m4x4 a, m4x4 b)`
Multiplies two 4x4 matrices `a` and `b`.

### `v3 vm_m4x4_mul_v3(m4x4 m, v3 v)`
Multiplies a 4x4 matrix `m` by a 3D vector `v`.

### Notes
- All functions are implemented in the C89 style, meaning they avoid the use of newer C features like variable-length arrays or function overloading.
- Vector and matrix types (`v2`, `v3`, `v4`, `m4x4`) are custom types that are typically defined in the source code with specific fields for `x`, `y`, `z`, `w`.
