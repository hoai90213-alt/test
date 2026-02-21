#ifndef BOX64_COMPAT_LINUX_NET_H
#define BOX64_COMPAT_LINUX_NET_H

#if defined(__linux__) && !defined(__APPLE__)
#include_next <linux/net.h>
#else
#include <sys/socket.h>
#endif

#endif
