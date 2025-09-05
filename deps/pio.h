/* pio.h - v0.2 - public domain data structures - nickscha 2025

A C89 standard compliant, single header, nostdlib (no C Standard Library) Platform IO Provider (PIO).

LICENSE

  Placed in the public domain and also MIT licensed.
  See end of file for detailed license information.

*/
#ifndef PIO_H
#define PIO_H

/* #############################################################################
 * # COMPILER SETTINGS
 * #############################################################################
 */
/* Check if using C99 or later (inline is supported) */
#if __STDC_VERSION__ >= 199901L
#define PIO_INLINE inline
#define PIO_API static
#elif defined(__GNUC__) || defined(__clang__)
#define PIO_INLINE __inline__
#define PIO_API static
#elif defined(_MSC_VER)
#define PIO_INLINE __inline
#define PIO_API static
#else
#define PIO_INLINE
#define PIO_API static
#endif

/* #############################################################################
 * # Platform independant functions
 * #############################################################################
 */
PIO_API PIO_INLINE unsigned long pio_strlen(char *str)
{
  char *s = str;
  while (*s)
  {
    s++;
  }
  return (unsigned long)(s - str);
}

/* #############################################################################
 * # WIN32 Implementation
 * #############################################################################
 */
#ifdef _WIN32

/* Because windows.h is massivly bloated and increases compilation time a lot we declare only the function we really need */
#ifndef _WINDOWS_
#define PIO_WIN32_API(r) __declspec(dllimport) r __stdcall

#define INVALID_HANDLE ((void *)-1)
#define GENERIC_WRITE (0x40000000L)
#define CREATE_ALWAYS 2
#define FILE_ATTRIBUTE_DIRECTORY 0x00000010
#define FILE_ATTRIBUTE_NORMAL 0x00000080
#define INVALID_FILE_SIZE ((unsigned long)0xffffffff)
#define GENERIC_READ (0x80000000L)
#define FILE_SHARE_READ 0x00000001
#define OPEN_EXISTING 3

PIO_WIN32_API(int)
CloseHandle(void *hObject);

/* IO read */
PIO_WIN32_API(unsigned long)
GetFileSize(void *hFile, unsigned long *lpFileSizeHigh);

PIO_WIN32_API(int)
ReadFile(void *hFile, void *lpBuffer, unsigned long nNumberOfBytesToRead, unsigned long *lpNumberOfBytesRead, void *lpOverlapped);

/* IO write */
PIO_WIN32_API(void *)
CreateFileA(char *lpFileName, unsigned long dwDesiredAccess, unsigned long dwShareMode, void *lpSecurityAttributes, unsigned long dwCreationDisposition, unsigned long dwFlagsAndAttributes, void *hTemplateFile);

PIO_WIN32_API(int)
WriteFile(void *hFile, void *lpBuffer, unsigned long nNumberOfBytesToWrite, unsigned long *lpNumberOfBytesWritten, void *lpOverlapped);

/* Print to console */
PIO_WIN32_API(void *)
GetStdHandle(unsigned long nStdHandle);

PIO_WIN32_API(int)
WriteConsoleA(void *hConsoleOutput, void *lpBuffer, unsigned long nNumberOfCharsToWrite, unsigned long *lpNumberOfCharsWritten, void *lpReserved);

#endif /* _WINDOWS_ */

PIO_API PIO_INLINE unsigned long pio_file_size(char *filename)
{
  void *hFile;
  unsigned long fileSize;

  hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

  if (hFile == INVALID_HANDLE)
  {
    return 0;
  }

  fileSize = GetFileSize(hFile, 0);

  CloseHandle(hFile);

  if (fileSize == INVALID_FILE_SIZE)
  {
    return 0;
  }

  return fileSize;
}

PIO_API PIO_INLINE int pio_read(char *filename, unsigned char *file_buffer, unsigned long file_buffer_capacity, unsigned long *file_buffer_size)
{
  void *hFile;
  unsigned long fileSize;
  unsigned long bytesRead;

  hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

  if (hFile == INVALID_HANDLE)
  {
    return 0;
  }

  fileSize = GetFileSize(hFile, 0);

  if (fileSize == INVALID_FILE_SIZE)
  {
    CloseHandle(hFile);
    return 0;
  }

  /* +1 for null terminator */
  if (file_buffer_capacity < fileSize + 1)
  {
    CloseHandle(hFile);
    return 0;
  }

  if (!ReadFile(hFile, file_buffer, fileSize, &bytesRead, 0) || bytesRead != fileSize)
  {
    CloseHandle(hFile);
    return 0;
  }

  file_buffer[fileSize] = '\0';
  *file_buffer_size = fileSize;

  CloseHandle(hFile);

  return 1;
}

PIO_API PIO_INLINE int pio_write(char *filename, unsigned char *buffer, unsigned long size)
{
  void *hFile;
  unsigned long bytes_written;
  int success;

  hFile = CreateFileA(filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

  if (hFile == INVALID_HANDLE)
  {
    return 0;
  }

  success = WriteFile(hFile, buffer, size, &bytes_written, 0);
  success = CloseHandle(hFile);

  return (success && (bytes_written == size));
}

PIO_API PIO_INLINE int pio_print(char *str)
{
  unsigned long written;
  void *hConsole = GetStdHandle((unsigned long)-11);
  return WriteConsoleA(hConsole, str, pio_strlen(str), &written, ((void *)0));
}

/* #############################################################################
 * # LINUX/MACOS Implementation
 * #############################################################################
 */
#elif defined(__linux__) || defined(__APPLE__)

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

PIO_API PIO_INLINE unsigned long pio_file_size(char *filename)
{
  int fd;
  struct stat st;
  unsigned long size = 0;

  fd = open(filename, O_RDONLY);

  if (fd < 0)
  {
    return 0;
  }

  if (fstat(fd, &st) == 0)
  {
    size = (unsigned long)st.st_size;
  }

  close(fd);
  return size;
}

PIO_API PIO_INLINE int pio_read(char *filename, unsigned char *file_buffer, unsigned long file_buffer_capacity, unsigned long *file_buffer_size)
{
  int fd;
  struct stat st;
  ssize_t bytes_read;

  fd = open(filename, O_RDONLY);

  if (fd < 0)
  {
    return 0;
  }

  if (fstat(fd, &st) != 0)
  {
    close(fd);
    return 0;
  }

  if ((unsigned long)st.st_size + 1 > file_buffer_capacity)
  {
    close(fd);
    return 0;
  }

  bytes_read = read(fd, file_buffer, (size_t)st.st_size);

  if (bytes_read != st.st_size)
  {
    close(fd);
    return 0;
  }

  file_buffer[st.st_size] = '\0'; /* Optional: null-terminate */
  *file_buffer_size = (unsigned long)st.st_size;

  close(fd);
  return 1;
}

PIO_API PIO_INLINE int pio_write(char *filename, unsigned char *buffer, unsigned long size)
{
  int fd;
  ssize_t written;

  fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);

  if (fd < 0)
  {
    return 0;
  }

  written = write(fd, buffer, size);
  close(fd);

  return (written == (ssize_t)size);
}

PIO_API PIO_INLINE int pio_print(char *str)
{
  unsigned long len = pio_strlen(str);
  ssize_t written = write(1, str, len);
  return written == (ssize_t)len;
}

#else
#error "pio: unsupported operating system. please provide your own read/write file implementation"
#endif

#endif /* PIO_H */

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
