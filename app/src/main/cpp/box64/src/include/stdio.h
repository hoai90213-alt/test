#ifndef BOX64_COMPAT_STDIO_H
#define BOX64_COMPAT_STDIO_H

#include_next <stdio.h>

#if defined(__APPLE__)
#ifndef fseeko64
#define fseeko64 fseeko
#endif
#ifndef ftello64
#define ftello64 ftello
#endif
#endif

#endif
