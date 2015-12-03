#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <assert.h>

#define SOCKADDR_PTR(sa) ((struct sockaddr*)sa)

#ifdef SOCK_UN
#include <sys/un.h>
#define SOCKADDR_UN_PTR(sa) ((struct sockaddr_un*)sa)
int sockaddr_un_cmp(struct sockaddr_un* x, struct sockaddr_un* y);
#endif

#ifdef SOCK_IN6
#define SOCKADDR_IN6_PTR(sa) ((struct sockaddr_in6*)sa)
int sockaddr_in6_cmp(struct sockaddr_in6* x, struct sockaddr_in6* y);
#endif

#define SOCKADDR_IN_PTR(sa) ((struct sockaddr_in*)sa)
int sockaddr_in_cmp(struct sockaddr_in* x, struct sockaddr_in* y);

int sockaddr_cmp(struct sockaddr* x, struct sockaddr* y);
#endif
