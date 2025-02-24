/* test.h - v0.1 - public domain data structures - nickscha 2025

A C89 standard compliant, single header, nostdlib (no C Standard Library) simple testing framework.

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
#elif defined(__GNUC__) || defined(__clang__)
#define TEST_INLINE __inline__
#elif defined(_MSC_VER)
#define TEST_INLINE __inline
#else
#define TEST_INLINE
#endif

#ifndef TEST_MAX_NUMBER_OF_TEST_RESULTS
#define TEST_MAX_NUMBER_OF_TEST_RESULTS 1024
#endif

typedef int bool;

typedef struct test_result
{
    char *file;
    int line;
    char *expression;
    bool conditional;
    bool result;

} test_result;

typedef struct test_state
{
    unsigned int passed;
    unsigned int failed;
    unsigned int length;
    test_result results[TEST_MAX_NUMBER_OF_TEST_RESULTS];

} test_state;

static test_state test_state_global = {0};

#ifdef _WIN32

void *GetStdHandle(unsigned long nStdHandle);
int SetConsoleTextAttribute(void *hConsoleOutput, unsigned short wAttributes);

#define COLOR_DEFAULT 7
#define COLOR_BLUE 9
#define COLOR_GREEN 10
#define COLOR_RED 12

static TEST_INLINE void set_console_color(unsigned short color)
{
    void *hConsole = GetStdHandle((unsigned long)-11);
    SetConsoleTextAttribute(hConsole, color);
}

#else

#define COLOR_DEFAULT 0
#define COLOR_BLUE 0
#define COLOR_GREEN 0
#define COLOR_RED 0
static TEST_INLINE void set_console_color(int color) { (void)color; }

#endif

#ifndef TEST_FUNCTION_PRINTF
#include <stdio.h>
#define TEST_FUNCTION_PRINTF(f, a1) (printf(f, a1))
#endif

static TEST_INLINE void test_result_print(test_result result)
{
    char *txt_header = "TEST";
    char *txt_pass = "PASS";
    char *txt_fail = "FAIL";

    TEST_FUNCTION_PRINTF("%s", "[");
    set_console_color(COLOR_BLUE);
    TEST_FUNCTION_PRINTF("%s", txt_header);
    set_console_color(COLOR_DEFAULT);
    TEST_FUNCTION_PRINTF("%s", "] ");

    TEST_FUNCTION_PRINTF("%s", "[");
    set_console_color(result.result ? COLOR_GREEN : COLOR_RED);
    TEST_FUNCTION_PRINTF("%s", result.result ? txt_pass : txt_fail);
    set_console_color(COLOR_DEFAULT);
    TEST_FUNCTION_PRINTF("%s", "] ");
    TEST_FUNCTION_PRINTF("%s:", result.file);
    TEST_FUNCTION_PRINTF("%-6d", result.line);
    TEST_FUNCTION_PRINTF(" %s\n", result.expression);
}

static TEST_INLINE void test_results_size_reached_print(void)
{
    char *txt_header = "TEST";
    char *txt_fail = "WARN";

    TEST_FUNCTION_PRINTF("%s", "[");
    set_console_color(COLOR_BLUE);
    TEST_FUNCTION_PRINTF("%s", txt_header);
    set_console_color(COLOR_DEFAULT);
    TEST_FUNCTION_PRINTF("%s", "] ");

    TEST_FUNCTION_PRINTF("%s", "[");
    set_console_color(COLOR_RED);
    TEST_FUNCTION_PRINTF("%s", txt_fail);
    set_console_color(COLOR_DEFAULT);
    TEST_FUNCTION_PRINTF("%s", "] ");

    TEST_FUNCTION_PRINTF("%s", "The maximum amount of stored results in [state.results] has reached its limit of: ");
    TEST_FUNCTION_PRINTF("%i", TEST_MAX_NUMBER_OF_TEST_RESULTS);
    TEST_FUNCTION_PRINTF("%s\n", " ! Please #define TEST_MAX_NUMBER_OF_TEST_RESULTS if you whish to increase this size!");
}

#define test_check(exp, con)                                                     \
    do                                                                           \
    {                                                                            \
        test_result result = {0};                                                \
        result.file = __FILE__;                                                  \
        result.line = __LINE__;                                                  \
        result.expression = #exp;                                                \
        result.conditional = (con);                                              \
        result.result = (exp);                                                   \
        result.result ? test_state_global.passed++ : test_state_global.failed++; \
        test_result_print(result);                                               \
        if (test_state_global.length >= TEST_MAX_NUMBER_OF_TEST_RESULTS)         \
        {                                                                        \
            test_results_size_reached_print();                                   \
            test_state_global.length = 0; /* Reset test result storage */        \
        }                                                                        \
        test_state_global.results[test_state_global.length] = result;            \
        test_state_global.length++;                                              \
        if (!(con) && !result.result)                                            \
        {                                                                        \
            *(volatile int *)0 = 0;                                              \
        }                                                                        \
    } while (0)

#define test(exp) test_check(exp, 1)
#define assert(exp) test_check(exp, 0)

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
