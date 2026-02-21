#ifndef BOX64_COMPAT_SYSCALL_H
#define BOX64_COMPAT_SYSCALL_H

#if defined(__linux__) && !defined(__APPLE__)
#include_next <syscall.h>
#else
#include <sys/syscall.h>
#endif

#endif
