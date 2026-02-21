#ifndef BOX64_COMPAT_ASM_STAT_H
#define BOX64_COMPAT_ASM_STAT_H

#if defined(__linux__) && !defined(__APPLE__)
#include_next <asm/stat.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif

#endif
