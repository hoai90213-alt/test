#ifndef BOX64_COMPAT_OBSTACK_H
#define BOX64_COMPAT_OBSTACK_H

#if defined(__APPLE__)

#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

struct _obstack_chunk {
    char* limit;
    struct _obstack_chunk* prev;
    char contents[4];
};

struct obstack {
    long chunk_size;
    struct _obstack_chunk* chunk;
    char* object_base;
    char* next_free;
    char* chunk_limit;
    union {
        intptr_t tempint;
        char* tempptr;
    } temp;
    intptr_t alignment_mask;
    struct _obstack_chunk* (*chunkfun)(void*, long);
    void (*freefun)(void*, struct _obstack_chunk*);
    void* extra_arg;
    unsigned use_extra_arg : 1;
    unsigned maybe_empty_object : 1;
    unsigned alloc_failed : 1;
};

static inline void __box64_obstack_alloc_failed_default(void)
{
}

static void (*obstack_alloc_failed_handler)(void) = __box64_obstack_alloc_failed_default;

static inline int _obstack_begin(struct obstack* obstack, size_t size, size_t alignment, void* chunkfun, void* freefun)
{
    (void)size;
    (void)alignment;
    (void)chunkfun;
    (void)freefun;
    if (obstack) {
        obstack->chunk_size = 0;
        obstack->chunk = NULL;
        obstack->object_base = NULL;
        obstack->next_free = NULL;
        obstack->chunk_limit = NULL;
        obstack->temp.tempptr = NULL;
        obstack->alignment_mask = 0;
        obstack->chunkfun = NULL;
        obstack->freefun = NULL;
        obstack->extra_arg = NULL;
        obstack->use_extra_arg = 0;
        obstack->maybe_empty_object = 0;
        obstack->alloc_failed = 1;
    }
    errno = ENOSYS;
    return -1;
}

static inline void _obstack_free(struct obstack* obstack, void* block)
{
    (void)obstack;
    (void)block;
}

static inline void obstack_free(struct obstack* obstack, void* block)
{
    _obstack_free(obstack, block);
}

static inline void _obstack_newchunk(struct obstack* obstack, int length)
{
    (void)obstack;
    (void)length;
    errno = ENOSYS;
    if (obstack_alloc_failed_handler) {
        obstack_alloc_failed_handler();
    }
}

static inline int obstack_vprintf(struct obstack* obstack, const char* fmt, va_list ap)
{
    (void)obstack;
    (void)fmt;
    (void)ap;
    errno = ENOSYS;
    return -1;
}

#else

#include_next <obstack.h>

#endif

#endif
