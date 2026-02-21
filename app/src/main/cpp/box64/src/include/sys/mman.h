#ifndef BOX64_COMPAT_SYS_MMAN_H
#define BOX64_COMPAT_SYS_MMAN_H

#include_next <sys/mman.h>

#if defined(__APPLE__)
#ifndef MAP_ANONYMOUS
#ifdef MAP_ANON
#define MAP_ANONYMOUS MAP_ANON
#else
#define MAP_ANONYMOUS 0x1000
#endif
#endif

#ifndef MAP_NORESERVE
#define MAP_NORESERVE 0
#endif

#ifndef MAP_GROWSDOWN
#define MAP_GROWSDOWN 0
#endif

static inline void* mmap64(void* addr, size_t length, int prot, int flags, int fd, off_t offset)
{
    return mmap(addr, length, prot, flags, fd, offset);
}
#endif

#endif
