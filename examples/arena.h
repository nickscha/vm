/* arena.h - v0.1 - public domain data structures - nickscha 2025

A C89 standard compliant, single header, nostdlib (no C Standard Library) arena allocator.

LICENSE

  Placed in the public domain and also MIT licensed.
  See end of file for detailed license information.

*/
#ifndef ARENA_H
#define ARENA_H

/* #############################################################################
 * # COMPILER SETTINGS
 * #############################################################################
 */
/* Check if using C99 or later (inline is supported) */
#if __STDC_VERSION__ >= 199901L
#define ARENA_INLINE inline
#elif defined(__GNUC__) || defined(__clang__)
#define ARENA_INLINE __inline__
#elif defined(_MSC_VER)
#define ARENA_INLINE __inline
#else
#define ARENA_INLINE
#endif

#ifdef _WIN32
/* Windows prototypes since include windows.h is immensily slow !!! */
#define MEM_COMMIT 0x00001000
#define MEM_RESERVE 0x00002000
#define MEM_RELEASE 0x00008000
#define PAGE_READWRITE 0x04
void *VirtualAlloc(void *lpAddress, unsigned long dwSize, unsigned long flAllocationType, unsigned long flProtect);
int VirtualFree(void *lpAddress, unsigned long dwSize, unsigned long dwFreeType);
void RtlMoveMemory(void *Destination, const void *Source, unsigned long Length);
#endif

#define ARENA_NULL ((void *)0)

#ifndef ARENA_ALIGNMENT
#define ARENA_ALIGNMENT 16 /* Align allocations to 16 bytes */
#endif

#define ARENA_ALIGN(size) (((size) + (ARENA_ALIGNMENT - 1)) & ~(ARENA_ALIGNMENT - 1))

typedef struct arena
{
    char *base;
    unsigned long offset;
    unsigned long offset_last;
    unsigned long size;
} arena;

static ARENA_INLINE int arena_init(arena *arena, unsigned long size)
{
    if (!arena->base)
    {
        arena->base = (char *)VirtualAlloc(ARENA_NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        if (!arena->base)
        {
            return 0;
        }
        arena->size = size;
        arena->offset = 0;
        arena->offset_last = 0;
    }
    return 1;
}

static ARENA_INLINE void *arena_malloc(arena *arena, unsigned long size)
{
    void *_ptr;
    unsigned int _size = ARENA_ALIGN(size);

    if (arena->offset + _size > arena->size)
    {
        return ARENA_NULL; /* Out of memory */
    }

    _ptr = arena->base + arena->offset;

    arena->offset_last = arena->offset;
    arena->offset += _ptr ? _size : 0;

    return (_ptr);
}

static ARENA_INLINE void *arena_realloc(arena *arena, void *ptr, unsigned long new_size)
{
    char *cptr;
    void *new_ptr;

    new_size = ARENA_ALIGN(new_size);

    if (!ptr)
    {
        return arena_malloc(arena, new_size);
    }

    cptr = (char *)ptr;

    /* Fast path: If it's the last allocation, extend in place */
    if (cptr == arena->base + arena->offset_last)
    {
        unsigned long old_size = arena->offset - arena->offset_last;

        if (arena->offset + (new_size - old_size) <= arena->size)
        {
            arena->offset_last = arena->offset;
            arena->offset += (new_size - old_size);
            return ptr;
        }
    }

    /* Slow path: Allocate new memory and copy */
    new_ptr = arena_malloc(arena, new_size);
    if (new_ptr)
    {
        RtlMoveMemory(new_ptr, ptr, new_size);
    }
    return (new_ptr);
}

static ARENA_INLINE void arena_reset(arena *arena)
{
    arena->offset = 0;
    arena->offset_last = 0;
}

static ARENA_INLINE void arena_free(arena *arena)
{
    if (arena->base)
    {
        VirtualFree(arena->base, 0, MEM_RELEASE);
        arena->base = (char *)ARENA_NULL;
        arena->offset = 0;
        arena->offset_last = 0;
        arena->size = 0;
    }
}

#endif /* ARENA_H */

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
