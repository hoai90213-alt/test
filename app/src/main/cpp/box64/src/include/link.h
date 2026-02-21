#ifndef BOX64_COMPAT_LINK_H
#define BOX64_COMPAT_LINK_H

#if defined(__linux__) && !defined(__APPLE__)
#include_next <link.h>
#else
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <elf.h>

typedef long Lmid_t;

#ifndef RTLD_DI_LINKMAP
#define RTLD_DI_LINKMAP 2
#endif

struct link_map {
    Elf64_Addr l_addr;
    char* l_name;
    Elf64_Dyn* l_ld;
    struct link_map* l_next;
    struct link_map* l_prev;
};

struct dl_phdr_info {
    ElfW(Addr) dlpi_addr;
    const char* dlpi_name;
    const ElfW(Phdr)* dlpi_phdr;
    ElfW(Half) dlpi_phnum;
};

static inline int dlinfo(void* handle, int request, void* info)
{
    (void)handle;
    if (request == RTLD_DI_LINKMAP && info) {
        *(void**)info = NULL;
    }
    errno = ENOSYS;
    return -1;
}

static inline int dl_iterate_phdr(int (*callback)(struct dl_phdr_info*, size_t, void*), void* data)
{
    (void)callback;
    (void)data;
    return 0;
}
#endif

#endif
