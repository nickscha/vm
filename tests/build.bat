@echo off

set DEF_FLAGS_COMPILER=-std=c89 -pedantic -Wall -Wextra -Werror -Wvla -Wconversion -Wdouble-promotion -Wsign-conversion -Wuninitialized -Winit-self -Wunused -Wunused-macros -Wunused-local-typedefs
set DEF_FLAGS_LINKER=
set SOURCE_NAME=vm_test

cc -g3 %DEF_FLAGS_COMPILER% -o %SOURCE_NAME%.exe %SOURCE_NAME%.c %DEF_FLAGS_LINKER%
%SOURCE_NAME%.exe
