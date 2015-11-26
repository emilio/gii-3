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
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>

#include "logger.h"
#include "protocol.h"
#include "utils.h"
#include "socket_utils.h"

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
    printf("  -h, --help\t Display this message and exit\n");
    printf("  -udp, --use-udp\t Use UDP instead of TCP\n");
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


int main(int argc, char** argv) {
    bool use_udp = false;
    long int port = 8000;
    int ret;

    /// TODO: add file argument
    FILE* src_file = stdin;

    LOGGER_CONFIG.log_file = stderr;

    for (int i = 1; i < argc; ++i) {
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
        } else if (strcmp(argv[i], "-udp") == 0 || strcmp(argv[i], "--use-udp")) {
            use_udp = true;
        } else {
            WARN("Unrecognized option: %s", argv[i]);
        }
    }

    LOG("Using %s in port %ld", use_udp ? "UDP" : "TCP", port);

    int sock;
    if (use_udp)
        sock = socket(AF_INET, SOCK_DGRAM, 0);
    else
        sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1)
        FATAL("Error creating socket: %s", strerror(errno));

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    ret = connect(sock, (const struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (ret == -1)
        FATAL("Error connecting to remote: %s", strerror(errno));

    /// UDP requires to pass a pointer to the address to `sendto()`,
    /// while TCP requires NOT to pass it
    struct sockaddr* serv_addr_send_arg = NULL;
    size_t len_send_arg = 0;
    if (use_udp) {
        serv_addr_send_arg = (struct sockaddr*)&serv_addr;
        len_send_arg = sizeof(serv_addr);
    }

    char buff[SIZE];
    size_t len;
    bool interactive = src_file == stdin;
    size_t lines_read = 0;

    while (true) {
        printf("> ");

        if (buff != fgets(buff, SIZE, src_file)) {
            LOG("Ignoring fgets() error: %s", strerror(errno));
            continue;
        }

        lines_read++;

        if (!interactive)
            printf("%s", buff);

        if (feof(src_file)) {
            LOG("EOF reached");
            break;
        }

        len = strlen(buff);

        // Trim trailing newline
        if (len && buff[len - 1] == '\n')
            buff[--len] = 0;

        if (!len) {
            LOG("Ignoring empty line (%zu)", lines_read);
            continue;
        }

        LOG("sending %zu bytes: %s", len, buff);

        ret = sendto(sock, buff, len, 0, serv_addr_send_arg, len_send_arg);
        if (ret == -1) {
            WARN("Send error: %s", strerror(errno));
            continue;
        }

        struct sockaddr_in from_addr;
        socklen_t from_size = sizeof(from_addr);

        ssize_t recv_size;
retry_udp_recv:
        recv_size = recvfrom(sock, buff, SIZE, 0,
                                     (struct sockaddr*)&from_addr,
                                     &from_size);

        if (recv_size < 0) {
            WARN("Recv error: %s", strerror(errno));
            break;
        }

        buff[recv_size] = 0;
        if (use_udp) {
            if (sockaddr_cmp((struct sockaddr*)&from_addr, (struct sockaddr*)&serv_addr) != 0) {
                WARN("Received message not from server: %s", buff);
                goto retry_udp_recv;
            }
        }

        printf("< %s\n", buff);
    }

    LOG("Closing client");
    close(sock);

    return 0;
}
