#ifndef BOX64_COMPAT_SYS_PRCTL_H
#define BOX64_COMPAT_SYS_PRCTL_H

#if defined(__linux__) && !defined(__APPLE__)
#include_next <sys/prctl.h>
#else
#include <errno.h>
#include <stdarg.h>

#ifndef PR_SET_NAME
#define PR_SET_NAME 15
#endif

#ifndef PR_SET_SECCOMP
#define PR_SET_SECCOMP 22
#endif

static inline int prctl(int option, ...)
{
    (void)option;
    errno = ENOSYS;
    return -1;
}
#endif

#endif
