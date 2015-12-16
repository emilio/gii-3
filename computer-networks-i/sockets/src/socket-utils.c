/**
 * socket-utils.c:
 *   Socket comparison utils
 *
 * Copyright (C) 2015 Emilio Cobos Álvarez (70912324N) <emiliocobos@usal.es>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "socket-utils.h"

#ifdef SOCK_UN
int sockaddr_un_cmp(struct sockaddr_un* x, struct sockaddr_un* y) {
    return strcmp(x->sun_path, y->sun_path);
}
#endif

#ifdef SOCK_IN6
int sockaddr_in6_cmp(struct sockaddr_in6* x, struct sockaddr_in6* y) {
    if (x->sin6_port != y->sin6_port)
        return x->sin6_port - y->sin6_port;

    if (x->sin6_flowinfo != y->sin6_flowinfo)
        return x->sin6_flowinfo - y->sin6_flowinfo;

    if (x->sin6_scope_id != y->sin6_scope_id)
        return x->sin6_scope_id - y->sin6_scope_id;

    return memcmp(x->sin6_addr.s6_addr, y->sin6_addr.s6_addr,
                  sizeof(x->sin6_addr.s6_addr));
}
#endif

int sockaddr_in_cmp(struct sockaddr_in* x, struct sockaddr_in* y) {
    if (x->sin_addr.s_addr != y->sin_addr.s_addr)
        return x->sin_addr.s_addr - y->sin_addr.s_addr;

    return x->sin_port - y->sin_port;
}

int sockaddr_cmp(struct sockaddr* x, struct sockaddr* y) {
    if (x->sa_family != y->sa_family)
        return x->sa_family - y->sa_family;

    switch (x->sa_family) {
#ifdef SOCK_UN
        case AF_UNIX:
            return sockaddr_un_cmp(SOCKADDR_UN_PTR(x), SOCKADDR_UN_PTR(y));
#endif
        case AF_INET:
            return sockaddr_in_cmp(SOCKADDR_IN_PTR(x), SOCKADDR_IN_PTR(y));
#ifdef SOCK_IN6
        case AF_INET6:
            return sockaddr_in6_cmp(SOCKADDR_IN6_PTR(x), SOCKADDR_IN6_PTR(y));
#endif
        default:
            assert(0 && "Unknown sockaddr family");
    }
    return 0;
}
