#ifndef BOX64_COMPAT_LINUX_AUXVEC_H
#define BOX64_COMPAT_LINUX_AUXVEC_H

#if defined(__linux__) && !defined(__APPLE__)
#include_next <linux/auxvec.h>
#else
#ifndef AT_HWCAP
#define AT_HWCAP 16
#endif
#ifndef AT_HWCAP2
#define AT_HWCAP2 26
#endif
#endif

#endif
