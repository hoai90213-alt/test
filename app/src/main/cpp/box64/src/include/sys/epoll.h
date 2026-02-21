#ifndef BOX64_COMPAT_SYS_EPOLL_H
#define BOX64_COMPAT_SYS_EPOLL_H

#if defined(__linux__) && !defined(__APPLE__)
#include_next <sys/epoll.h>
#else
#include <errno.h>
#include <signal.h>
#include <stdint.h>

typedef union epoll_data {
    void* ptr;
    int fd;
    uint32_t u32;
    uint64_t u64;
} epoll_data_t;

struct epoll_event {
    uint32_t events;
    epoll_data_t data;
} __attribute__((packed));

#ifndef EPOLL_CTL_ADD
#define EPOLL_CTL_ADD 1
#endif
#ifndef EPOLL_CTL_DEL
#define EPOLL_CTL_DEL 2
#endif
#ifndef EPOLL_CTL_MOD
#define EPOLL_CTL_MOD 3
#endif

#ifndef EPOLL_CLOEXEC
#define EPOLL_CLOEXEC 0x00080000
#endif

static inline int epoll_create(int size)
{
    (void)size;
    errno = ENOSYS;
    return -1;
}

static inline int epoll_create1(int flags)
{
    (void)flags;
    errno = ENOSYS;
    return -1;
}

static inline int epoll_ctl(int epfd, int op, int fd, struct epoll_event* event)
{
    (void)epfd;
    (void)op;
    (void)fd;
    (void)event;
    errno = ENOSYS;
    return -1;
}

static inline int epoll_wait(int epfd, struct epoll_event* events, int maxevents, int timeout)
{
    (void)epfd;
    (void)events;
    (void)maxevents;
    (void)timeout;
    errno = ENOSYS;
    return -1;
}

static inline int epoll_pwait(int epfd, struct epoll_event* events, int maxevents, int timeout, const sigset_t* sigmask)
{
    (void)epfd;
    (void)events;
    (void)maxevents;
    (void)timeout;
    (void)sigmask;
    errno = ENOSYS;
    return -1;
}
#endif

#endif
