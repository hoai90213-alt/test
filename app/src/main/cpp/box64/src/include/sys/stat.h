#ifndef BOX64_COMPAT_SYS_STAT_H
#define BOX64_COMPAT_SYS_STAT_H

#include_next <sys/stat.h>

#if defined(__APPLE__)
#ifndef stat64
#define stat64 stat
#endif
#ifndef lstat64
#define lstat64 lstat
#endif
#ifndef fstat64
#define fstat64 fstat
#endif
#ifndef fstatat64
#define fstatat64 fstatat
#endif
#endif

#endif
