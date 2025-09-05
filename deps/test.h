/* test.h - v0.3 - public domain data structures - nickscha 2025

A C89 standard compliant, single header, nostdlib (no C Standard Library) simple testing framework.

USAGE
    // Use assert to break/stop program when expression is not met
    assert(1 == 1);

    // Use test as an conditional expression. If it fails it will log out the result but the program will continue
    test(10 % 5 == 0);


LICENSE

  Placed in the public domain and also MIT licensed.
  See end of file for detailed license information.

*/
#ifndef TEST_H
#define TEST_H

/* #############################################################################
 * # COMPILER SETTINGS
 * #############################################################################
 */
/* Check if using C99 or later (inline is supported) */
#if __STDC_VERSION__ >= 199901L
#define TEST_INLINE inline
#define TEST_API static
#elif defined(__GNUC__) || defined(__clang__)
#define TEST_INLINE __inline__
#define TEST_API static
#elif defined(_MSC_VER)
#define TEST_INLINE __inline
#define TEST_API static
#else
#define TEST_INLINE
#define TEST_API static
#endif

typedef struct test_result
{
    char *file;
    int line;
    char *expression;
    int conditional;
    int result;

} test_result;

TEST_API TEST_INLINE int test_strlen(char *s)
{
    int len = 0;
    while (*s)
    {
        len++;
        s++;
    }
    return len;
}

#ifdef _WIN32

#ifndef _WINDOWS_
#define TEST_PLATFORM_WIN32_API(r) __declspec(dllimport) r __stdcall

TEST_PLATFORM_WIN32_API(void *)
GetStdHandle(unsigned long nStdHandle);
TEST_PLATFORM_WIN32_API(int)
SetConsoleTextAttribute(void *hConsoleOutput, unsigned short wAttributes);
TEST_PLATFORM_WIN32_API(int)
WriteFile(void *hFile, void *lpBuffer, unsigned long nNumberOfBytesToWrite, unsigned long *lpNumberOfBytesWritten, void *lpOverlapped);
#endif

#define COLOR_DEFAULT 7
#define COLOR_BLUE 9
#define COLOR_GREEN 10
#define COLOR_RED 12

TEST_API TEST_INLINE void set_console_color(unsigned short color)
{
    SetConsoleTextAttribute(GetStdHandle((unsigned long)-11), color);
}

TEST_API TEST_INLINE void test_print_string(char *str)
{
    unsigned long written;
    WriteFile(GetStdHandle((unsigned long)-11), str, (unsigned long)test_strlen(str), &written, 0);
}

TEST_API TEST_INLINE void test_print_int(int val)
{
    char buf[12]; /* Enough for a 32-bit integer and a sign */
    char *p = buf + 11;
    unsigned int v = (unsigned int)val;
    *p = '\0';

    if (val < 0)
    {
        v = -v;
    }

    do
    {
        *--p = (char)('0' + (v % 10));
        v /= 10;
    } while (v != 0);

    if (val < 0)
    {
        *--p = '-';
    }
    test_print_string(p);
}

#else

#include <unistd.h>

#define COLOR_DEFAULT 0
#define COLOR_BLUE 0
#define COLOR_GREEN 0
#define COLOR_RED 0
TEST_API TEST_INLINE void set_console_color(int color) { (void)color; }

TEST_API TEST_INLINE void test_print_string(char *str)
{
    int written_bytes = (int)write(STDOUT_FILENO, str, (unsigned int)test_strlen(str));
    (void)written_bytes;
}

TEST_API TEST_INLINE void test_print_int(int val)
{
    char buf[12];
    char *p = buf + 11;
    unsigned int v = (unsigned int)val;
    int written_bytes;

    *p = '\0';

    if (val < 0)
    {
        v = -v;
    }

    do
    {
        *--p = (char)('0' + (v % 10));
        v /= 10;
    } while (v != 0);

    if (val < 0)
    {
        *--p = '-';
    }
    written_bytes = (int)write(STDOUT_FILENO, p, (unsigned int)test_strlen(p));
    (void)written_bytes;
}

#endif

TEST_API TEST_INLINE void test_result_print(test_result *result)
{
    int i;
    int temp_line;
    int num_digits = 0;

    char *txt_header = "TEST";
    char *txt_pass = "PASS";
    char *txt_fail = "FAIL";

    test_print_string("[");
    set_console_color(COLOR_BLUE);
    test_print_string(txt_header);
    set_console_color(COLOR_DEFAULT);
    test_print_string("] [");
    set_console_color(result->result ? COLOR_GREEN : COLOR_RED);
    test_print_string(result->result ? txt_pass : txt_fail);
    set_console_color(COLOR_DEFAULT);
    test_print_string("] ");
    test_print_string(result->file);
    test_print_string(":");

    /* Calculate number of digits to print */
    temp_line = result->line;
    if (temp_line == 0)
    {
        num_digits = 1;
    }
    else
    {
        while (temp_line != 0)
        {
            temp_line /= 10;
            num_digits++;
        }
    }

    test_print_int(result->line);

    /* Add trailing spaces to pad to 5 characters */
    for (i = 0; i < 7 - num_digits; ++i)
    {
        test_print_string(" ");
    }

    test_print_string(result->expression);
    test_print_string("\n");
}

TEST_API TEST_INLINE float test_absf(float x)
{
    return (x < 0.0f ? -x : x);
}

TEST_API TEST_INLINE double test_absd(double x)
{
    return (x < 0.0 ? -x : x);
}

#define test_check(exp, con)          \
    do                                \
    {                                 \
        test_result result = {0};     \
        result.file = __FILE__;       \
        result.line = __LINE__;       \
        result.expression = #exp;     \
        result.conditional = (con);   \
        result.result = (exp);        \
        test_result_print(&result);   \
        if (!(con) && !result.result) \
        {                             \
            *(volatile int *)0 = 0;   \
        }                             \
    } while (0)

#define test(exp) test_check(exp, 1)
#define test_equalsf(a, b, e) test_check(test_absf((a) - (b)) < (e), 1)
#define test_equalsd(a, b, e) test_check(test_absd((a) - (b)) < (e), 1)
#define assert(exp) test_check(exp, 0)
#define assert_equalsf(a, b, e) test_check(test_absf((a) - (b)) < (e), 0)
#define assert_equalsd(a, b, e) test_check(test_absd((a) - (b)) < (e), 0)

#endif /* TEST_H */

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
