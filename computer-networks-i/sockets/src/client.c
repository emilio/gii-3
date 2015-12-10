/**
 * client.c
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
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netdb.h>

#include "logger.h"
#include "protocol.h"
#include "utils.h"
#include "socket-utils.h"

#define SIZE 512

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
    printf("  --help\t Display this message and exit\n");
    printf("  -udp, --use-udp\t Use UDP instead of TCP\n");
    printf("  -p, --port [port]\t Connect to [port]\n");
    printf("  -h, --host [host]\t Connect to [host]\n");
    printf("  -v, --verbose\t Be verbose about what is going on\n");
    printf("\n");
    printf("Author(s):\n");

    const struct program_author* current_author = AUTHORS;
    while (current_author->name) {
        printf("  %s (<%s>)\n", current_author->name, current_author->email);
        current_author++;
    }
}

int main(int argc, char** argv) {
    bool use_udp = false;
    const char* port = "8000";
    const char* host = "localhost";

    /// TODO: add file argument
    FILE* src_file = stdin;

    LOGGER_CONFIG.log_file = stderr;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--help") == 0) {
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
            port = argv[i];
        } else if (strcmp(argv[i], "-udp") == 0 ||
                   strcmp(argv[i], "--use-udp") == 0) {
            use_udp = true;
        } else if (strcmp(argv[i], "-h") == 0 ||
                   strcmp(argv[i], "--host") == 0) {
            ++i;
            if (i == argc)
                FATAL("The %s option needs a value", argv[i - 1]);
            host = argv[i];
        } else {
            WARN("Unrecognized option: %s", argv[i]);
        }
    }

    LOG("Using %s, port: %s, host: %s", use_udp ? "UDP" : "TCP", port, host);

    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = use_udp ? SOCK_DGRAM : SOCK_STREAM;

    int ret;
    struct addrinfo* info;
    ret = getaddrinfo(host, port, &hints, &info);
    if (ret != 0)
        FATAL("getaddrinfo: %s", gai_strerror(ret));

    // We use just the first configuration returned by getaddrinfo
    int sock;
    sock = socket(info->ai_family, info->ai_socktype, 0);
    if (sock == -1)
        FATAL("Error creating socket: %s", strerror(errno));

    /// Set a recv timeout, to allow closing connections if they fail
    /// This is neccesary overall for udp
    struct timeval tv;
    memset(&tv, 0, sizeof(struct timeval));
    tv.tv_sec = 30;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv,
               sizeof(struct timeval));

    struct sockaddr_in serv_addr;
    ret = connect(sock, info->ai_addr, info->ai_addrlen);
    if (ret == -1)
        FATAL("Error connecting to remote: %s", strerror(errno));

    memcpy(&serv_addr, info->ai_addr, info->ai_addrlen);

    freeaddrinfo(info); // Now we don't need this anymore

    char buff[SIZE];
    size_t len;
    bool interactive = src_file == stdin;
    size_t lines_read = 0;

    while (true) {
        printf("> ");

        if (buff != fgets(buff, SIZE, src_file)) {
            if (feof(src_file)) {
                LOG("EOF reached");
                break;
            }

            LOG("Ignoring fgets() error, err: %s", strerror(ferror(src_file)));
            continue;
        }

        lines_read++;

        if (!interactive)
            printf("%s", buff);

        len = strlen(buff);

        // Trim trailing newline
        if (len && buff[len - 1] == '\n')
            buff[--len] = 0;

        if (!len) {
            LOG("Ignoring empty line (%zu)", lines_read);
            continue;
        }

        LOG("sending %zu bytes: %s", len, buff);

        ret = send(sock, buff, len, 0);
        if (ret == -1) {
            WARN("Send error: %s", strerror(errno));
            continue;
        }

        struct sockaddr_in from_addr;
        socklen_t from_size = sizeof(from_addr);

        ssize_t recv_size;
        size_t recv_count = 0;
    retry_recv:
        /// The server always replies with and ending null char, that should
        /// not be present in the body response
        recv_size = recvfrom(sock, buff, SIZE, 0, (struct sockaddr*)&from_addr,
                             &from_size);

        if (recv_size < 0) {
            WARN("Recv error: %s", strerror(errno));
            break;
        }

        if (use_udp) {
            if (sockaddr_cmp((struct sockaddr*)&from_addr,
                             (struct sockaddr*)&serv_addr) != 0) {
                WARN("Received message not from server: %s", buff);
                goto retry_recv;
            }
        }

        if (recv_size == 0) {
            WARN("Server unexpectedly closed connection");
            break;
        }

        ++recv_count;

        // The server always replies with a trailing zero on end,
        // so maybe we have more data to receive.
        bool is_first_message = recv_count == 1;
        bool is_last_message = buff[recv_size - 1] == '\0';

        if (is_first_message)
            printf("< ");

        buff[recv_size] = 0;
        LOG("recv(%zu, last: %d): %s", recv_size, is_last_message, buff);

        printf("%s", buff);

        if (is_last_message)
            printf("\n");
        else
            goto retry_recv;
    }

    LOG("Closing client");
    close(sock);

    return 0;
}
