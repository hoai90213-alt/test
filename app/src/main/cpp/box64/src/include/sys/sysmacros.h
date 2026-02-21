#ifndef BOX64_COMPAT_SYS_SYSMACROS_H
#define BOX64_COMPAT_SYS_SYSMACROS_H

#if defined(__linux__) && !defined(__APPLE__)
#include_next <sys/sysmacros.h>
#else
#include <stdint.h>

#ifndef major
#define major(dev) ((unsigned int)(((uint64_t)(dev) >> 8) & 0xfff))
#endif

#ifndef minor
#define minor(dev) ((unsigned int)((((uint64_t)(dev) >> 0) & 0xff) | ((((uint64_t)(dev) >> 12) & 0xffffff00))))
#endif

#ifndef makedev
#define makedev(maj, min) ((uint64_t)((((uint64_t)(maj) & 0xfff) << 8) | (((uint64_t)(min) & 0xff)) | ((((uint64_t)(min) & 0xffffff00) << 12))))
#endif
#endif

#endif
