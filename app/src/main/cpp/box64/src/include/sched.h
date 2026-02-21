#ifndef BOX64_COMPAT_SCHED_H
#define BOX64_COMPAT_SCHED_H

#include_next <sched.h>

#if defined(__APPLE__)
#include <errno.h>

typedef int (*box64_clone_fn_t)(void*);

static inline int clone(box64_clone_fn_t fn, void* child_stack, int flags, void* arg, ...)
{
    (void)fn;
    (void)child_stack;
    (void)flags;
    (void)arg;
    errno = ENOSYS;
    return -1;
}
#endif

#endif
