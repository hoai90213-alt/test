#ifndef BOX64_COMPAT_ASM_HWCAP_H
#define BOX64_COMPAT_ASM_HWCAP_H

#if defined(__linux__) && !defined(__APPLE__)
#include_next <asm/hwcap.h>
#else
#ifndef HWCAP_ASIMD
#define HWCAP_ASIMD   (1UL << 1)
#endif
#ifndef HWCAP_AES
#define HWCAP_AES     (1UL << 3)
#endif
#ifndef HWCAP_PMULL
#define HWCAP_PMULL   (1UL << 4)
#endif
#ifndef HWCAP_SHA1
#define HWCAP_SHA1    (1UL << 5)
#endif
#ifndef HWCAP_SHA2
#define HWCAP_SHA2    (1UL << 6)
#endif
#ifndef HWCAP_CRC32
#define HWCAP_CRC32   (1UL << 7)
#endif
#ifndef HWCAP_ATOMICS
#define HWCAP_ATOMICS (1UL << 8)
#endif
#ifndef HWCAP_USCAT
#define HWCAP_USCAT   (1UL << 25)
#endif
#ifndef HWCAP_FLAGM
#define HWCAP_FLAGM   (1UL << 27)
#endif

#ifndef HWCAP2_FLAGM2
#define HWCAP2_FLAGM2 (1UL << 7)
#endif
#ifndef HWCAP2_FRINT
#define HWCAP2_FRINT  (1UL << 8)
#endif
#ifndef HWCAP2_RNG
#define HWCAP2_RNG    (1UL << 16)
#endif
#ifndef HWCAP2_AFP
#define HWCAP2_AFP    (1UL << 20)
#endif
#endif

#endif
