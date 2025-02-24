# vm
A C89 standard compliant, single header, nostdlib (no C Standard Library) vector linear algebra implementation.

For more information please look at the "vm.h" file or take a look at the "examples" or "tests" folder.

## Quick Start

Download or clone vm.h and include it in your project.

```C
#include "vm.h"

int main() {

    v3 a = {2.0f, 2.0f, 2.0f};
    v3 b = vm_v3_one();
    v3 r = vm_v3_add(a, b);

    return 0;
}
```

## Run Example: nostdlib, freestsanding

In this repo you will find the "examples/vm_win32_nostdlib.c" with the corresponding "build.bat" file which
creates an executable only linked to "kernel32" and is not using the C standard library and executes the program afterwards.
