/**
 * soket-utils.h:
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
