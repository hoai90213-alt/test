#ifndef ZOMDROID_PLATFORM_COMPAT_H
#define ZOMDROID_PLATFORM_COMPAT_H

#include <stddef.h>
#include <stdint.h>

#if defined(__ANDROID__)
#include <android/dlext.h>
#include <android/native_window.h>
#include "liblinkernsbypass/android_linker_ns.h"
typedef ANativeWindow ZomdroidNativeWindow;
#define ZOMDROID_SHARED_LIB_SUFFIX ".so"
#else
typedef void ZomdroidNativeWindow;

typedef struct android_namespace_t android_namespace_t;
typedef struct {
    uint64_t flags;
    void* library_namespace;
} android_dlextinfo;

#ifndef ANDROID_DLEXT_USE_NAMESPACE
#define ANDROID_DLEXT_USE_NAMESPACE (1ULL << 9)
#endif

static inline android_namespace_t* android_get_exported_namespace(const char* name) {
    (void)name;
    return NULL;
}

#if defined(__APPLE__)
#define ZOMDROID_SHARED_LIB_SUFFIX ".dylib"
#else
#define ZOMDROID_SHARED_LIB_SUFFIX ".so"
#endif
#endif

#endif // ZOMDROID_PLATFORM_COMPAT_H
