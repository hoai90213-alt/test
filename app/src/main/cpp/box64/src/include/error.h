#ifndef BOX64_COMPAT_ERROR_H
#define BOX64_COMPAT_ERROR_H

#if defined(__linux__) && !defined(__APPLE__)
#include_next <error.h>
#else
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned int error_message_count = 0;
static int error_one_per_line = 0;
static void (*error_print_progname)(void) = NULL;

static inline void error(int status, int errnum, const char* format, ...)
{
    va_list ap;
    if (error_print_progname) {
        error_print_progname();
    }
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    if (errnum) {
        fprintf(stderr, ": %s", strerror(errnum));
    }
    fputc('\n', stderr);
    ++error_message_count;
    if (status) {
        exit(status);
    }
}

static inline void error_at_line(int status, int errnum, const char* filename, unsigned int linenum, const char* format, ...)
{
    va_list ap;
    if (error_print_progname) {
        error_print_progname();
    }
    if (filename) {
        fprintf(stderr, "%s:%u: ", filename, linenum);
    }
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    if (errnum) {
        fprintf(stderr, ": %s", strerror(errnum));
    }
    fputc('\n', stderr);
    ++error_message_count;
    if (status) {
        exit(status);
    }
}
#endif

#endif
