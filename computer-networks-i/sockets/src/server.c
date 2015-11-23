/**
 * server.c
 *
 * @author Emilio Cobos Álvarez (70912324N)
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <unistd.h>

#include "logger.h"
#include "utils.h"

#define TCP_MAX_CONNECTIONS 1024
#define UDP_MAX_CONNECTIONS 1024
#define MAX_MESSAGE_SIZE 512

struct program_author {
    const char* name;
    const char* email;
};

const struct program_author AUTHORS[] = {
    {"Emilio Cobos Álvarez", "emiliocobos@usal.es"},
    {NULL, NULL}, // End of list
};

/// Shows usage of the program
void show_usage(int argc, char** argv) {
    printf("Usage: %s [options]\n", argv[0]);
    printf("Options:\n");
    printf("  -h, --help\t Display this message and exit\n");
    printf("  -p, --port [port]\t Listen to [port]\n");
    printf("  -v, --verbose\t Be verbose about what is going on\n");
    printf("\n");
    printf("Author(s):\n");

    const struct program_author* current_author = AUTHORS;
    while (current_author->name) {
        printf("  %s (<%s>)\n", current_author->name, current_author->email);
        current_author++;
    }
}

/// Parses the message stored in buff, and sends the correct reply to `sock`,
/// using `sendto()` and the given `target_addr`, and `target_addr_len`.
///
/// This function expects `buff` to be zero-terminated.
///
/// We exploit the fact that `sento()` can be used instead of `write()` in TCP,
/// given that `target_addr` is null and `target_addr_size` is zero.
void parse_message_and_reply(int sock, const char* buff,
                             struct sockaddr* target_addr,
                             socklen_t target_addr_len) {}

void* start_tcp_server(void* info) {
    long port = *((long*)info);
    struct sockaddr_in serv_addr;
    int sock;
    int ret;

    LOG("start_tcp_server(%p); port: %ld", info, port);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        ERROR("Couldn't open TCP socket: %s", strerror(errno));
        return NULL;
    }

    LOG("Opened TCP socket (fd: %d)", sock);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    ret = bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (ret == -1) {
        ERROR("Couldn't bind TCP socket: %s", strerror(errno));
        goto cleanup_and_return;
    }

    LOG("TCP socket bound to port: %ld", port);

    ret = listen(sock, TCP_MAX_CONNECTIONS);
    if (ret == -1) {
        ERROR("Listen: %s", strerror(errno));
        goto cleanup_and_return;
    }

    while (true) {
        char buff[MAX_MESSAGE_SIZE] = {0};
        struct sockaddr_in client_addr;
        socklen_t client_size = sizeof(client_addr);
        int new_socket =
            accept(sock, (struct sockaddr*)&client_addr, &client_size);

        if (new_socket == -1) {
            WARN("tcp: Ignoring accept() error: %s", strerror(errno));
            continue;
        }

        // TODO: We don't handle chunked messages because we know the message
        // size is fixed, and that messages longer than `MAX_MESSAGE_SIZE`
        // can't be valid. We should have a growing buffer otherwise.
        ssize_t len = read(new_socket, buff, sizeof(buff) - 1);
        if (len == -1) {
            WARN("tcp: Ignoring read() error: %s", strerror(errno));
            close(new_socket);
            continue;
        }
        buff[len] = 0;

        LOG("tcp: Received %zu bytes: %s", len, buff);

        parse_message_and_reply(new_socket, buff, NULL, 0);
        close(new_socket);
    }

cleanup_and_return:
    LOG("TCP server closing");
    close(sock);
    return NULL;
}

void* start_udp_server(void* info) {
    long port = *((long*)info);
    struct sockaddr_in serv_addr;
    int sock;
    int ret;

    LOG("start_udp_server(%p); port: %ld", info, port);

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        ERROR("Couldn't open UDP socket: %s", strerror(errno));
        return NULL;
    }

    LOG("Opened UDP socket (fd: %d)", sock);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    ret = bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (ret == -1) {
        ERROR("Couldn't bind UDP socket: %s", strerror(errno));
        close(sock);
        return NULL;
    }

    LOG("UDP socket bound to port: %ld", port);

    // TODO: handle chunked datagrams?
    // It's arguably difficult (if not impossible) with UDP.
    while (true) {
        struct sockaddr_in src_addr;
        socklen_t src_addr_len = sizeof(src_addr);
        ssize_t len;
        char buff[MAX_MESSAGE_SIZE];

        len = recvfrom(sock, buff, sizeof(buff) - 1, 0,
                       (struct sockaddr*)&src_addr, &src_addr_len);
        if (len == -1) {
            WARN("Ignoring UDP datagram due to error: %s", strerror(errno));
            continue;
        }

        buff[len] = 0;

        LOG("udp: Received %zu bytes: \"%s\"", len, buff);

        parse_message_and_reply(sock, buff, (struct sockaddr*)&src_addr,
                                src_addr_len);

        if (len == -1) {
            WARN("UDP response lost: %s", strerror(errno));
            continue;
        }
    }

    close(sock);
    return NULL;
}

int main(int argc, char** argv) {
    int i;
    long port = 8000;
    int thread_creation_status;

    pthread_t tcp_thread;
    pthread_t udp_thread;

    LOGGER_CONFIG.log_file = stderr;

    for (i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            show_usage(argc, argv);
            return 1;
        } else if (strcmp(argv[i], "-v") == 0 ||
                   strcmp(argv[i], "--verbose") == 0) {
            LOGGER_CONFIG.verbose = true;
        } else if (strcmp(argv[i], "-p") == 0 ||
                   strcmp(argv[i], "--port") == 0) {
            ++i;
            if (i == argc)
                FATAL("The %s option needs a value", argv[i - 1]);
            if (!read_long(argv[i], &port))
                WARN("Using default port %ld", port);
        }
    }

    LOG("Starting server on port %ld", port);

    /// NOTE: Passing a pointer to a variable on the stack is unsafe if `main()`
    /// does not
    /// live long enough. It does though, so...
    thread_creation_status =
        pthread_create(&tcp_thread, NULL, start_tcp_server, &port);
    if (thread_creation_status != 0)
        FATAL("Couldn't create TCP server thread: %s",
              strerror(thread_creation_status));

    LOG("TCP server thread created: %ld", tcp_thread);

    thread_creation_status =
        pthread_create(&udp_thread, NULL, start_udp_server, &port);
    if (thread_creation_status != 0)
        FATAL("Couldn't create UDP server thread: %s",
              strerror(thread_creation_status));

    LOG("UDP server thread created: %ld", udp_thread);

    pthread_join(tcp_thread, NULL);
    pthread_join(udp_thread, NULL);

    return 0;
}
